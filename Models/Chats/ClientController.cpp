/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
#include "ClientController.h"
#include <QSharedPointer>
#include <QQmlEngine>
#include <QClipboard>
#include "QOutlog.h"
#include <csapi/content-repo.h>
#include <csapi/joining.h>
#include <csapi/profile.h>
#include <connection.h>
#include <networkaccessmanager.h>
#include <settings.h>
#include <logging.h>
#include <user.h>
#include "Servers/Draco.h"
#include "ViewsEnums.h"
#include "Chats/matrixbrigde.h"
#include "jobs/downloadfilejob.h"
#include <QSqlDatabase>
#include <database.h>
#include <QTextDocument>
#include "OnBoardingModel.h"
#include "ifaces/Servers/Byzantine.h"
#include "QAppEngine/QEventProcessor/Common/WorkerThread.h"
#include <QPointer>
#include <QTimer>
#include <QDir>
#include <QFileInfo>
#include <QTemporaryFile>
#include <thread>
#include <chrono>

using Quotient::NetworkAccessManager;
using Quotient::Settings;
using Quotient::AccountSettings;
using Quotient::Uri;

ClientController::ClientController()
    :m_connection(NULL)
    ,m_isNunchukLoggedIn(false)
    ,m_loginHandler(NULL)
    ,m_contacts(QContactModelPtr(new QContactModel()))
    ,m_contactsSent(QContactModelPtr(new QContactModel()))
    ,m_contactsReceived(QContactModelPtr(new QContactModel()))
    ,m_devicesLogged(QLoggedInDeviceModelPtr(new QLoggedInDeviceModel))
    ,m_rooms(NULL)
    ,m_imageprovider(new QNunchukImageProvider())
    ,m_isNewDevice(false)
    ,m_AttachmentEnable(false)
    ,m_ReadySupport(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(NetworkAccessManager::instance(), &QNetworkAccessManager::proxyAuthenticationRequired, this, &ClientController::proxyAuthenticationRequired);
    connect(NetworkAccessManager::instance(), &QNetworkAccessManager::sslErrors, this, &ClientController::sslErrors);
    setSubscriptions(QJsonArray());
    qRegisterMetaType<DracoUser>();
}

ClientController::~ClientController()
{

}

ClientController *ClientController::instance()
{
    static ClientController mInstance;
    return &mInstance;
}

QString ClientController::accessToken() const
{
    if(connection()){
        return connection()->accessToken();
    }
    return "";
}

Connection *ClientController::connection() const
{
    return m_connection.data();
}

void ClientController::setConnection(Connection *c)
{
    if(m_connection.data() == c){
        return;
    }

    if(m_connection){
        Connection* oldConnection = m_connection.data();
        oldConnection->stopSync();
        QObject::disconnect(oldConnection, nullptr, this, nullptr);
        if(m_loginHandler){
            QObject::disconnect(oldConnection, nullptr, m_loginHandler.data(), nullptr);
        }
        if(m_rooms){
            QObject::disconnect(oldConnection, nullptr, m_rooms.data(), nullptr);
        }
    }

    m_connection = OurSharedPointer<Quotient::Connection>(c);
    m_matrixDrainScheduled = false;

    if(!m_connection){
        m_matrixConnectionState = MatrixConnectionState::NoConnection;
        if(m_imageprovider){
            m_imageprovider->setConnection(QSharedPointer<Connection>{});
        }
        emit connectionChanged();
        emit isMatrixLoggedInChanged();
        return;
    }

    m_matrixConnectionState = MatrixConnectionState::Starting;
    const quint64 connectionGeneration = ++m_matrixConnectionGeneration;
    const QPointer<Connection> trackedConnection = m_connection.data();
    connect(m_connection.data(), &Connection::stateChanged, this,
            [this, trackedConnection, connectionGeneration] {
        if(!trackedConnection
                || connectionGeneration != m_matrixConnectionGeneration
                || connection() != trackedConnection){
            return;
        }
        if(m_matrixConnectionState == MatrixConnectionState::Starting
                && trackedConnection->encryptionEnabled()
                && trackedConnection->isLoggedIn()){
            m_matrixConnectionState = MatrixConnectionState::E2eePending;
            DBG_INFO << "[MATRIX_LOGIN_GATE] E2EE setup pending; generation:"
                     << connectionGeneration;
        }
        emit isMatrixLoggedInChanged();
    });
    connect(m_connection.data(), &Connection::ready, this,
            [this, trackedConnection, connectionGeneration] {
        if(!trackedConnection
                || connectionGeneration != m_matrixConnectionGeneration
                || connection() != trackedConnection){
            return;
        }
        m_matrixConnectionState = MatrixConnectionState::Ready;
        DBG_INFO << "[MATRIX_LOGIN_GATE] Matrix connection ready; generation:"
                 << connectionGeneration;
        if(m_pendingMatrixLogin){
            schedulePendingMatrixLoginDrain();
        }
    }, Qt::SingleShotConnection);
    connect(m_connection.data(), &Connection::connected, this,
            [this, trackedConnection, connectionGeneration] {
        // Let every connected() handler for the current attempt install its
        // sync lifecycle before exposing Matrix as usable to UI/background
        // flows.
        QTimer::singleShot(0, this,
                           [this, trackedConnection, connectionGeneration] {
            if(!trackedConnection
                    || connectionGeneration != m_matrixConnectionGeneration
                    || connection() != trackedConnection
                    || !m_isNunchukLoggedIn
                    || Draco::instance()->chatId() != m_activeMatrixUserId
                    || Draco::instance()->Uid() != m_activeNunchukAccountId
                    || m_pendingMatrixLogin
                    || m_matrixConnectionState != MatrixConnectionState::Ready){
                return;
            }
            setMatrixLoginTransitioning(false);
        });
    }, Qt::SingleShotConnection);
    if(m_imageprovider){
        // Image requests may outlive a login attempt. Give the provider a
        // shared reference so replacing the current Matrix connection cannot
        // leave an in-flight thumbnail response with a dangling pointer.
        m_imageprovider->setConnection(m_connection);
    }
    emit connectionChanged();
}

bool ClientController::isNunchukLoggedIn() const
{
    return m_isNunchukLoggedIn;
}

void ClientController::setIsNunchukLoggedIn(bool isLogged)
{
    if(m_isNunchukLoggedIn != isLogged){
        m_isNunchukLoggedIn = isLogged;
        if(!m_isNunchukLoggedIn){
            // A deferred Matrix fallback belongs to the authenticated Nunchuk
            // account that requested it. Never resume it after sign-out.
            clearPendingMatrixLoginRequest();
            m_activeMatrixUserId.clear();
            m_activeNunchukAccountId.clear();
            // Invalidate Matrix immediately and emit the NOTIFY edge before
            // any synchronous sign-out request spins a nested event loop.
            // The next valid connection clears this only after ready().
            setMatrixLoginTransitioning(true);
        }
        if (m_subscriptionsReady) {
            m_subscriptionsReady = false;
            emit subscriptionsReadyChanged();
        }
        emit isNunchukLoggedInChanged();
        if (m_isNunchukLoggedIn) {
            AppModel::instance()->setTimeLogging(QDateTime::currentDateTime());
        }
    }
}

QLogginManager *ClientController::loginHandler() const
{
    return m_loginHandler.data();
}

void ClientController::setLoginHandler(const QLogginManagerPtr &login)
{
    m_loginHandler = login;
}

void ClientController::requestLogin(bool forcePasswordLogin)
{
    const bool restoreSupportRoomRequest = rooms()
            && rooms()->hasPendingSupportRoomRequest();

    // Replacing a Connection while Quotient is still loading its E2EE
    // pickling key leaves a raw-pointer continuation behind. Keep the current
    // attempt alive until ready(), then tear it down before starting the next
    // one. Requests arriving during the drain are coalesced as well.
    if(m_matrixLoginCriticalDepth > 0
            || m_matrixConnectionState == MatrixConnectionState::E2eePending
            || m_matrixConnectionState == MatrixConnectionState::Draining
            || m_pendingMatrixLogin){
        queueMatrixLoginRequest(forcePasswordLogin,
                                restoreSupportRoomRequest);
        return;
    }

    if(m_connection){
        setMatrixLoginTransitioning(true);
    }
    startMatrixLoginAttempt(forcePasswordLogin,
                            restoreSupportRoomRequest);
}

void ClientController::startMatrixLoginAttempt(bool forcePasswordLogin,
                                               bool restoreSupportRoomRequest)
{
    // isLoggedIn() becomes true before Quotient finishes loading the E2EE
    // pickling data. Keep Matrix consumers paused until ready()/connected().
    setMatrixLoginTransitioning(true);
    setReadySupport(false);
    const QString matrixUserId = Draco::instance()->chatId();
    const QString nunchukAccountId = Draco::instance()->Uid();
    const QString matrixPassword = Draco::instance()->dracoToken();
    m_activeMatrixUserId = matrixUserId;
    m_activeNunchukAccountId = nunchukAccountId;
    if(rooms()){
        // Invalidate timers/jobs from the old room session before its
        // Connection is replaced. The pending Support intent was captured
        // above and is restored on the new model below.
        rooms()->removeAll();
    }
    auto* matrixConnection = new Quotient::Connection();
    // libQuotient enables E2EE per connection. This must happen before both
    // cached-token and password login; room encryption is still controlled by
    // each room's m.room.encryption state.
    matrixConnection->enableEncryption(true);
    setConnection(matrixConnection);
    setLoginHandler(QLogginManagerPtr(new QLogginManager(connection())));
    setRooms(QNunchukRoomListModelPtr(new QNunchukRoomListModel(connection())));
    const quint64 connectionGeneration = m_matrixConnectionGeneration;
    const QPointer<Connection> loginConnection = connection();
    const QPointer<QLogginManager> matrixLoginHandler = loginHandler();
    const bool enteredCriticalSection = beginMatrixLoginCriticalSection(
            loginConnection.data(), matrixLoginHandler.data());
    if(loginHandler()){
        loginHandler()->invokeLogin(matrixUserId,
                                    matrixPassword,
                                    forcePasswordLogin);
    }
    endMatrixLoginCriticalSection(enteredCriticalSection);

    // Keychain access in invokeLogin() can run a nested event loop. A relogin
    // requested from that loop is queued by requestLogin(); never continue the
    // old attempt's post-login work after its generation changed.
    if(connectionGeneration != m_matrixConnectionGeneration
            || !loginConnection
            || connection() != loginConnection
            || !matrixLoginHandler
            || loginHandler() != matrixLoginHandler){
        return;
    }
    if(m_pendingMatrixLogin){
        m_pendingRestoreSupportRoomRequest =
                m_pendingRestoreSupportRoomRequest
                || restoreSupportRoomRequest;
        schedulePendingMatrixLoginDrain();
        return;
    }
    if(restoreSupportRoomRequest && rooms()){
        rooms()->createSupportRoom();
    }
}

void ClientController::queueMatrixLoginRequest(bool forcePasswordLogin,
                                               bool restoreSupportRoomRequest)
{
    const QString requestedUserId = Draco::instance()->chatId();
    const QString requestedAccountId = Draco::instance()->Uid();
    const bool sameIntent = m_pendingMatrixLogin
            && m_pendingMatrixUserId == requestedUserId
            && m_pendingNunchukAccountId == requestedAccountId;

    if(sameIntent){
        // A cached-token failure must not be downgraded by a later duplicate
        // normal-login request for the same account.
        m_pendingForcePasswordLogin = m_pendingForcePasswordLogin
                || forcePasswordLogin;
        m_pendingRestoreSupportRoomRequest =
                m_pendingRestoreSupportRoomRequest
                || restoreSupportRoomRequest;
    }
    else{
        // The latest account wins. forcePasswordLogin only belongs to the
        // account whose cached token failed.
        m_pendingMatrixLogin = true;
        m_pendingForcePasswordLogin = forcePasswordLogin;
        m_pendingRestoreSupportRoomRequest = restoreSupportRoomRequest;
        m_pendingMatrixUserId = requestedUserId;
        m_pendingNunchukAccountId = requestedAccountId;
    }

    setReadySupport(false);
    setMatrixLoginTransitioning(true);
    if(m_connection){
        m_connection->stopSync();
        if(m_loginHandler){
            // In particular, prevent the stale connected() handler from
            // starting sync after ready() completes the old E2EE attempt.
            QObject::disconnect(m_connection.data(), nullptr,
                                m_loginHandler.data(), nullptr);
        }
        if(m_rooms){
            QObject::disconnect(m_connection.data(), nullptr,
                                m_rooms.data(), nullptr);
        }
    }

    DBG_INFO << "[MATRIX_LOGIN_GATE] Login replacement queued; generation:"
             << m_matrixConnectionGeneration
             << "force password:" << m_pendingForcePasswordLogin;

    if(m_matrixConnectionState == MatrixConnectionState::Ready){
        schedulePendingMatrixLoginDrain();
    }
}

void ClientController::schedulePendingMatrixLoginDrain()
{
    const bool canDrain = m_matrixLoginCriticalDepth == 0
            && (m_matrixConnectionState == MatrixConnectionState::Ready
                || m_matrixConnectionState == MatrixConnectionState::Starting);
    if(m_matrixDrainScheduled
            || !m_pendingMatrixLogin
            || !canDrain
            || !m_connection){
        return;
    }

    m_matrixDrainScheduled = true;
    const quint64 connectionGeneration = m_matrixConnectionGeneration;
    const QPointer<Connection> trackedConnection = m_connection.data();
    QTimer::singleShot(0, this,
                       [this, trackedConnection, connectionGeneration] {
        if(connectionGeneration != m_matrixConnectionGeneration
                || !trackedConnection
                || connection() != trackedConnection){
            return;
        }
        m_matrixDrainScheduled = false;
        const bool stillSafeToDrain = m_matrixLoginCriticalDepth == 0
                && (m_matrixConnectionState == MatrixConnectionState::Ready
                    || m_matrixConnectionState == MatrixConnectionState::Starting);
        if(m_pendingMatrixLogin && stillSafeToDrain){
            drainCurrentMatrixConnection();
        }
    });
}

void ClientController::drainCurrentMatrixConnection()
{
    const bool canDrain = m_matrixLoginCriticalDepth == 0
            && (m_matrixConnectionState == MatrixConnectionState::Ready
                || m_matrixConnectionState == MatrixConnectionState::Starting);
    if(!m_connection
            || !m_pendingMatrixLogin
            || !canDrain){
        return;
    }

    if(rooms() && rooms()->hasPendingSupportRoomRequest()){
        m_pendingRestoreSupportRoomRequest = true;
    }

    Connection* oldConnection = m_connection.data();
    const QPointer<Connection> drainingConnection = oldConnection;
    const quint64 connectionGeneration = m_matrixConnectionGeneration;
    m_matrixConnectionState = MatrixConnectionState::Draining;
    m_matrixDrainScheduled = false;
    oldConnection->stopSync();

    if(m_loginHandler){
        QObject::disconnect(oldConnection, nullptr, m_loginHandler.data(), nullptr);
    }
    if(m_rooms){
        QObject::disconnect(oldConnection, nullptr, m_rooms.data(), nullptr);
        m_rooms->removeAll();
    }

    // Disconnect only app-level receivers. Do not globally disconnect the
    // Quotient object: its internal jobs still need to finish teardown.
    QObject::disconnect(oldConnection, nullptr, this, nullptr);
    connect(oldConnection, &QObject::destroyed, this,
            [this, connectionGeneration] {
        if(connectionGeneration != m_matrixConnectionGeneration
                || m_matrixConnectionState != MatrixConnectionState::Draining){
            return;
        }
        DBG_INFO << "[MATRIX_LOGIN_GATE] Previous connection destroyed; generation:"
                 << connectionGeneration;
        // Do not construct a new Connection from inside QObject's destructor
        // stack. Drop the raw-pointer holders now and resume on the next turn.
        setLoginHandler(QLogginManagerPtr{});
        setRooms(QNunchukRoomListModelPtr{});
        QTimer::singleShot(0, this, [this, connectionGeneration] {
            resumePendingMatrixLogin(connectionGeneration);
        });
    }, Qt::SingleShotConnection);

    // QNunchukImageProvider owns the same shared pointer. Release its base
    // reference before waiting for destroyed(); an active ThumbnailResponse
    // may still retain the old connection until its request finishes.
    if(m_imageprovider){
        m_imageprovider->setConnection(QSharedPointer<Connection>{});
    }
    m_connection.clear();
    emit connectionChanged();
    emit isMatrixLoggedInChanged();

    // Diagnostics only: never force-delete an E2EE Connection whose external
    // shared owner (for example an active thumbnail response) has not released
    // it yet; doing so would recreate the original lifetime bug.
    QTimer::singleShot(30000, this,
                       [this, drainingConnection, connectionGeneration] {
        if(drainingConnection
                && connectionGeneration == m_matrixConnectionGeneration
                && m_matrixConnectionState == MatrixConnectionState::Draining){
            DBG_INFO << "[MATRIX_LOGIN_GATE] Still waiting for previous connection owners; generation:"
                     << connectionGeneration;
        }
    });
}

void ClientController::resumePendingMatrixLogin(quint64 connectionGeneration)
{
    if(connectionGeneration != m_matrixConnectionGeneration
            || m_matrixConnectionState != MatrixConnectionState::Draining
            || m_connection){
        return;
    }

    m_matrixConnectionState = MatrixConnectionState::NoConnection;
    if(!m_pendingMatrixLogin
            || !isNunchukLoggedIn()
            || Draco::instance()->chatId() != m_pendingMatrixUserId
            || Draco::instance()->Uid() != m_pendingNunchukAccountId){
        clearPendingMatrixLoginRequest();
        setLoginHandler(QLogginManagerPtr{});
        setRooms(QNunchukRoomListModelPtr{});
        setMatrixLoginTransitioning(false);
        return;
    }

    const bool forcePasswordLogin = m_pendingForcePasswordLogin;
    const bool restoreSupportRoomRequest =
            m_pendingRestoreSupportRoomRequest;
    clearPendingMatrixLoginRequest();
    startMatrixLoginAttempt(forcePasswordLogin,
                            restoreSupportRoomRequest);
}

void ClientController::clearPendingMatrixLoginRequest()
{
    m_pendingMatrixLogin = false;
    m_pendingForcePasswordLogin = false;
    m_pendingRestoreSupportRoomRequest = false;
    m_pendingMatrixUserId.clear();
    m_pendingNunchukAccountId.clear();
}

void ClientController::setMatrixLoginTransitioning(bool transitioning)
{
    if(m_matrixLoginTransitioning == transitioning){
        return;
    }
    m_matrixLoginTransitioning = transitioning;
    emit isMatrixLoggedInChanged();
}

bool ClientController::beginMatrixLoginCriticalSection(
        Connection *expectedConnection,
        QLogginManager *expectedLoginHandler)
{
    if(!expectedConnection
            || !expectedLoginHandler
            || connection() != expectedConnection
            || loginHandler() != expectedLoginHandler){
        return false;
    }
    ++m_matrixLoginCriticalDepth;
    return true;
}

void ClientController::endMatrixLoginCriticalSection(bool entered)
{
    if(!entered){
        return;
    }
    Q_ASSERT(m_matrixLoginCriticalDepth > 0);
    --m_matrixLoginCriticalDepth;
    if(m_matrixLoginCriticalDepth == 0 && m_pendingMatrixLogin){
        schedulePendingMatrixLoginDrain();
    }
}

bool ClientController::hasPendingMatrixLoginReplacement(
        Connection *expectedConnection,
        QLogginManager *expectedLoginHandler) const
{
    return m_pendingMatrixLogin
            && connection() == expectedConnection
            && loginHandler() == expectedLoginHandler;
}

bool ClientController::isCurrentMatrixLoginAttempt(
        Connection *expectedConnection,
        QLogginManager *expectedLoginHandler) const
{
    return expectedConnection
            && expectedLoginHandler
            && m_isNunchukLoggedIn
            && connection() == expectedConnection
            && loginHandler() == expectedLoginHandler
            && expectedLoginHandler->userid() == m_activeMatrixUserId
            && Draco::instance()->chatId() == m_activeMatrixUserId
            && Draco::instance()->Uid() == m_activeNunchukAccountId;
}

void ClientController::syncContacts(QList<DracoUser> data)
{
    if(contacts()){
        contacts()->syncContacts(data);
        emit contactsChanged();
    }
}

void ClientController::syncDevices(QList<DracoDevice> data)
{
    this->devicesLogged()->syncDevices(data);
    emit devicesLoggedChanged();
}

QContactModel *ClientController::contacts() const
{
    return m_contacts.data();
}

QContactModelPtr ClientController::contactsPtr() const
{
    return m_contacts;
}

void ClientController::syncContactsReceived(QList<DracoUser> data)
{
    if(contactsReceived()){
        contactsReceived()->syncContacts(data);
        emit contactsReceivedChanged();
    }
}

QContactModel *ClientController::contactsReceived() const
{
    return m_contactsReceived.data();
}

QContactModelPtr ClientController::contactsReceivedPtr() const
{
    return m_contactsReceived;
}

QLoggedInDeviceModel *ClientController::devicesLogged() const
{
    return m_devicesLogged.data();
}

QNunchukRoom *ClientController::currentRoom() const
{
    if(rooms()){
        return rooms()->currentRoom();
    }
    return NULL;
}

QNunchukRoom *ClientController::GetRoomById(const QString &id) const
{
    if(rooms()){
        return rooms()->getRoomById(id).data();
    }
    return NULL;
}

void ClientController::syncContactsSent(QList<DracoUser> data)
{
    if(contactsSent()){
        contactsSent()->syncContacts(data);
        emit contactsSentChanged();
    }
}

QContactModel *ClientController::contactsSent() const
{
    return m_contactsSent.data();
}

QContactModelPtr ClientController::contactsSentPtr() const
{
    return m_contactsSent;
}

void ClientController::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    authenticator->setUser("User id here");
    authenticator->setPassword("Pass word here");
}

void ClientController::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    for (const auto& error: errors)
    {
        if (error.error() == QSslError::NoSslSupport)
        {
            static bool showMsgBox = true;
            if (showMsgBox)
            {
                DBG_INFO << "No SSL support:, Your SSL configuration does not allow Quaternion to establish secure connections.";
            }
            return;
        }
        DBG_INFO << "SSL errror";
        NetworkAccessManager::instance()->addIgnoredSslError(error);
    }
    reply->ignoreSslErrors(errors);
}

void ClientController::refreshContacts()
{
    Draco::instance()->refreshContacts();
}

void ClientController::refreshDevices()
{
    Draco::instance()->refreshDevices();
}

void ClientController::acceptFriendRequest(const QString &contact_id)
{
    if(contactsReceived()){
        contactsReceived()->acceptFriendRequest(contact_id);
    }
}

void ClientController::ignoreFriendRequest(const QString &contact_id)
{
    if(contactsReceived()){
        contactsReceived()->ignoreFriendRequest(contact_id);
    }
}

void ClientController::cancelFriendRequest(const QString &contact_id)
{
    if(contactsSent()){
        contactsSent()->cancelFriendRequest(contact_id);
    }
}

void ClientController::removeContact(const QString &contact_id)
{
    if(contacts()){
        contacts()->removeContact(contact_id);
    }
}

void ClientController::onUserAvatarChanged()
{
    if(connection()){
        QString url = connection()->user()->avatarUrl().toString();
        QString name = connection()->user()->name();
        if(name != "" && url != "" && url.localeAwareCompare(m_me.avatar) != 0){
            m_me.avatar = url;
            Draco::instance()->setUserProfile(m_me.name,url);
        }
    }
}

void ClientController::onUserDisplaynameChanged()
{
    if(connection()){
        QString name = connection()->user()->name();
        if(name != "" && name.localeAwareCompare(m_me.name) != 0){
            m_me.name = name;
            Draco::instance()->setUserProfile(name,m_me.avatar);
        }
    }
}

QStringList ClientController::contactsByStringList()
{
    if(contacts()){
        QStringList ret = contacts()->contactsByStringList();
        ret << QString("%1|%2|%3|%4").arg(getMe().id).arg(getMe().name).arg(getMe().email).arg(getMe().chat_id);
        return ret;
    }
    return QStringList();
}

void ClientController::requestSignout()
{
    if(!isNunchukLoggedIn()){
        DBG_ERROR << "You are not logged in";
        return;
    }
    bridge::StopConsumeGroupEvent();
    if(m_loginHandler){
        m_loginHandler.data()->requestLogout();
    }
    // Draco::signout() is synchronous and processes nested events. Mark the
    // current Matrix attempt stale first so its keychain/network continuations
    // cannot resume while the server sign-out request is in flight.
    setIsNunchukLoggedIn(false);
    Draco::instance()->signout();
    bridge::stopNunchuk();
    QEventProcessor::instance()->sendEvent(E::EVT_LOGIN_MATRIX_REQUEST);
    setAttachmentEnable(false);
    deleteStayLoggedInData();
    setSubscriptions(QJsonArray());
    AppSetting::instance()->setGroupSetting("");
    AppModel::instance()->requestClearData();
    if(rooms()){
        rooms()->removeAll();
    }
    if(contacts()){
        contacts()->removeAll();
    }
}

void ClientController::forgetRoom(const int index)
{
    if(rooms()){
        rooms()->forgetRoom(index);
    }
}

void ClientController::leaveCurrentRoom()
{
    if(rooms()){
        rooms()->leaveCurrentRoom();
    }
}

void ClientController::leaveRoom(const int index)
{
    if(rooms()){
        rooms()->leaveRoom(index);
    }
}

void ClientController::leaveRoomById(const QString &roomId)
{
    if(rooms()){
        rooms()->leaveRoomById(roomId);
    }
}

void ClientController::joinRoom(QString roomAliasOrId)
{

}

QNunchukRoomListModel *ClientController::rooms() const
{
    return m_rooms.data();
}

void ClientController::setRooms(const QNunchukRoomListModelPtr &rs)
{
    m_rooms = rs;
    if(m_rooms){
        connect(m_rooms.data(), &QNunchukRoomListModel::noticeService,          this, &ClientController::refreshContacts);
        connect(m_rooms.data(), &QNunchukRoomListModel::byzantineRoomCreated,   this, &ClientController::byzantineRoomCreated);
        connect(m_rooms.data(), &QNunchukRoomListModel::byzantineRoomDeleted,   this, &ClientController::byzantineRoomDeleted);
    }
    emit roomsChanged();
}

QString ClientController::userAvatarById(const QString &id) const
{
    if(connection()){
        return connection()->user(id)->avatarMediaId();
    }
    return "";
}

void ClientController::setUserAvatar(const QString &url)
{
    if(connection()){
        if(url != ""){
            QString file_path = qUtils::QGetFilePath(url);
            connection()->user()->setAvatar(file_path);
            timeoutHandler(1000,[=, this]{
                connection()->user()->load();
                QString errormsg = "";
                bool ret = Draco::instance()->changeAvatar(file_path, errormsg);
                if(ret){
                    Draco::instance()->getMe();
                }
            });
        }
        else{
            connection()->user()->removeAvatar();
            timeoutHandler(1000,[=, this]{
                connection()->user()->load();
                QJsonObject output;
                QString errormsg = "";
                bool ret = Draco::instance()->removeAvatar(output, errormsg);
                if(ret){
                    Draco::instance()->getMe();
                }
            });
        }
    }
}

void ClientController::setUserDisplayname(const QString &name)
{
    if(connection()){
        connection()->user()->rename(name);
    }
}

void ClientController::createRoomChat(const QStringList invitees_id, const QStringList invitees_name, QVariant firstMessage)
{
    if(rooms()){
        QString roomname = invitees_name.join(",");
        rooms()->createRoomChat(invitees_id, roomname, firstMessage);
    }
}

void ClientController::createRoomDirectChat(const QString invitee_id, const QString invitee_name, QVariant firstMessage)
{
    if(rooms()){
        rooms()->createRoomDirectChat(invitee_id, invitee_name, firstMessage);
    }
}

void ClientController::createRoomByzantineChat(const QStringList invitees_id, const QStringList invitees_name, const QString group_id, QVariant firstMessage)
{
    if(rooms()){
        QString roomname = invitees_name.join(",");
        rooms()->createRoomByzantineChat(invitees_id, roomname, group_id, firstMessage);
    }
}

void ClientController::renameRoomByzantineChat(const QString room_id, const QString group_id, const QString newname)
{
    if(rooms()){
        rooms()->renameRoomByzantineChat(room_id, group_id, newname);
    }
}

void ClientController::createSupportRoom()
{
    DBG_INFO << "[SUPPORT_TRACE] ClientController::createSupportRoom rooms():" << (rooms() ? "OK" : "NULL")
             << "readySupport:" << readySupport()
             << "isMatrixLoggedIn:" << isMatrixLoggedIn();
    // readySupport reflects room hydration/request progress. It must not make
    // a Support-button click disappear; the room model queues/coalesces the
    // request until it is safe to reuse or create the room.
    if(rooms()){
        rooms()->createSupportRoom();
    }
}

void ClientController::notifySupportRoomReady()
{
    DBG_INFO << "[SUPPORT_TRACE] ClientController::notifySupportRoomReady — emitting supportRoomNavigated";
    emit supportRoomNavigated();
}

void ClientController::sendMessage(const QString &msg)
{
    if(rooms()){
        rooms()->sendMessage(msg);
    }
}

QString ClientController::getPlainText(const QString &msg)
{
    QTextDocument doc;
    doc.setHtml( msg );
    return doc.toPlainText();
}

void ClientController::copyMessage(const QString &msg)
{
    qApp->clipboard()->setText(msg);
}

QVariant ClientController::user() const
{
    QMap<QString,QVariant> maps;
    maps["id"]          = m_me.id;
    maps["name"]        = (m_me.name.isEmpty() || m_me.name.isNull()) ? "Guest" : m_me.name;
    maps["email"]       = m_me.email;
    maps["gender"]      = m_me.gender;
    maps["avatar"]      = m_me.avatar;
    maps["status"]      = m_me.status;
    maps["status_text"] = m_me.status_text;
    maps["chat_id"]     = m_me.chat_id;
    maps["username"]    = m_me.username;
    maps["login_type"]  = m_me.login_type;
    maps["isPrimaryKey"] = m_me.login_type.localeAwareCompare("PRIMARY_KEY") == 0;
    bool subscribed = CLIENT_INSTANCE->isSubscribed();
    if(subscribed){
        maps["isSubscribedUser"] = isSubscribed();
        maps["isByzantineUser"]  = isByzantine();
        maps["isByzantineUserPro"] = isByzantinePro();
        maps["isByzantineUserStandard"] = isByzantineStandard();
        maps["isByzantineUserPremier"]  = isByzantinePremier();
        maps["isHoneyBadgerUser"]    = isHoneyBadger();
        maps["isHoneyBadgerPlusUser"] = isHoneyBadgerPlus();
        maps["isHoneyBadgerPremierUser"] = isHoneyBadgerPremier();
        maps["isIronHandUser"]       = isIronHand();
        maps["isFinneyUserPro"]      = isFinneyPro();
        maps["isFinneyUserStandard"] = isFinneyStandard();
        maps["isFinneyUser"]         = isFinney();
        maps["isMultiSubscriptions"] = isMultiSubscriptions();
        maps["isGroupWallet"]       = isGroupWallet();
        maps["isUserWallet"]        = isUserWallet();
        maps["slugs"] = slugs();
    }
    else{
        maps["isSubscribedUser"] = false;
        maps["isByzantineUser"]  = false;
        maps["isByzantineUserPro"] = false;
        maps["isByzantineUserStandard"] = false;
        maps["isByzantineUserPremier"]  = false;
        maps["isHoneyBadgerUser"]    = false;
        maps["isHoneyBadgerPlusUser"]= false;
        maps["isHoneyBadgerPremierUser"] = false;
        maps["isIronHandUser"]       = false;
        maps["isFinneyUserPro"]      = false;
        maps["isFinneyUserStandard"] = false;
        maps["isFinneyUser"]         = false;
        maps["isMultiSubscriptions"] = false;
        maps["isGroupWallet"]       = false;
        maps["isUserWallet"]        = false;
        maps["slugs"] = "";
    }
    if(AppModel::instance()->getPrimaryKey()){
        maps["master_fingerprint"] = AppModel::instance()->getPrimaryKey()->fingerPrint();
    }
    else{
        maps["master_fingerprint"] = "";
    }
    return QVariant::fromValue(maps);
}

DracoUser ClientController::getMe() const
{
    return m_me;
}

void ClientController::setMe(const DracoUser &me)
{
    m_me = me;
    emit userChanged();
}

bool ClientController::isNewDevice() const
{
    return m_isNewDevice;
}

void ClientController::setIsNewDevice(bool enableEncryption)
{
    if (m_isNewDevice == enableEncryption)
        return;

    m_isNewDevice = enableEncryption;
    emit enableEncryptionChanged();
}

bool ClientController::checkStayLoggedIn()
{
    QByteArray stayLoggedInBytes = readDataFromKeyChain("stayLoggedIn");
    if(stayLoggedInBytes.isNull() || stayLoggedInBytes.isEmpty()){
        deleteStayLoggedInData();
        return false;
    }
    QByteArray uIdBytes = readDataFromKeyChain("uid");
    if(uIdBytes.isNull() || uIdBytes.isEmpty()){
        deleteStayLoggedInData();
        return false;
    }
    QByteArray chatIdBytes = readDataFromKeyChain("chatId");
    if(chatIdBytes.isNull() || chatIdBytes.isEmpty()){
        deleteStayLoggedInData();
        return false;
    }
    Draco::instance()->setStayLoggedIn(true);
    Draco::instance()->setUid(QString(uIdBytes));
    Draco::instance()->setChatId(QString(chatIdBytes));
    Draco::instance()->setDracoToken(QString(stayLoggedInBytes));
    return true;
}

bool ClientController::readySupport() const
{
    return m_ReadySupport;
}

void ClientController::setReadySupport(bool ReadySupport)
{
    if(m_ReadySupport != ReadySupport){
        m_ReadySupport = ReadySupport;
        emit readySupportChanged();
    }
}

void ClientController::updateMessageMaxLifeTime(QString &roomId, qint64 maxLifeTime)
{
    if(rooms()){
        rooms()->updateMaxLifeTime(roomId, maxLifeTime);
    }
}

bool ClientController::attachmentEnable() const
{
    return m_AttachmentEnable;
}

void ClientController::setAttachmentEnable(bool AttachmentEnable)
{
    if(m_AttachmentEnable != AttachmentEnable){
        m_AttachmentEnable = AttachmentEnable;
        emit attachmentEnableChanged();
    }
}

QJsonArray ClientController::subscriptions() const
{
    return m_subscriptions;
}

bool ClientController::subscriptionsReady() const
{
    return m_subscriptionsReady;
}

QJsonArray ClientController::filterValidSubscriptions(const QJsonArray &data, QStringList *slugs, bool logValidity) const
{
    if (slugs) {
        slugs->clear();
    }
    QJsonArray validSubscriptions;

    foreach (const QJsonValue &value, data) {
        QJsonObject subscription = value.toObject();
        qint64 grace_valid_until_utc_millis = static_cast<qint64>(subscription.value("grace_valid_until_utc_millis").toDouble());
        QDateTime grace_valid_until_utc = QDateTime::fromMSecsSinceEpoch(grace_valid_until_utc_millis);
        bool valid = grace_valid_until_utc.isValid() && grace_valid_until_utc >= QDateTime::currentDateTime();
        if (logValidity) {
            DBG_INFO << grace_valid_until_utc.toString("yyyy-MM-dd-hh-mm-ss-zzz") << valid;
        }
        if (AppSetting::instance()->primaryServer() == (int)nunchuk::Chain::TESTNET || valid) {
            if (slugs) {
                slugs->append(subscription["plan"].toObject()["slug"].toString());
            }
            validSubscriptions.push_back(subscription);
        }
    }

    return validSubscriptions;
}

void ClientController::applySubscriptions(const QJsonArray &data)
{
    QStringList slugs;
    m_subscriptions = filterValidSubscriptions(data, &slugs, true);
    m_slugs = slugs;
}

bool ClientController::hasValidSubscriptions(const QJsonArray &data) const
{
    return !filterValidSubscriptions(data, nullptr, false).isEmpty();
}

void ClientController::setSubscriptions(const QJsonArray &data)
{
    applySubscriptions(data);
    if(m_subscriptions.size() > 0){
        setAttachmentEnable(true);
    }
    else{
        setAttachmentEnable(false);
    }
    DBG_INFO << m_slugs;
    emit subscriptionsChanged();
}

bool ClientController::resolveSubscriptions(const QJsonArray &data, bool allSourcesSucceeded)
{
    QStringList resolvedSlugs;
    const QJsonArray resolvedSubscriptions = filterValidSubscriptions(data, &resolvedSlugs, true);
    const bool hasSubscriptions = !resolvedSubscriptions.isEmpty();

    // A partial failure with no positive entitlement evidence is Unknown. Keep
    // the last known entitlement cache intact and only invalidate readiness.
    if (!hasSubscriptions && !allSourcesSucceeded) {
        if (m_subscriptionsReady) {
            m_subscriptionsReady = false;
            emit subscriptionsReadyChanged();
        }
        return false;
    }

    m_subscriptions = resolvedSubscriptions;
    m_slugs = resolvedSlugs;
    const bool readinessChanged = !m_subscriptionsReady;
    m_subscriptionsReady = true;

    setAttachmentEnable(hasSubscriptions);
    DBG_INFO << m_slugs;
    if (readinessChanged) {
        emit subscriptionsReadyChanged();
    }
    emit subscriptionsChanged();
    return hasSubscriptions;
}

QStringList ClientController::slugs() const
{
    if(isNunchukLoggedIn()){
        return m_slugs;
    }
    else {
        return QStringList();
    }
}

void ClientController::saveStayLoggedInData()
{
    if(Draco::instance()->stayLoggedIn()){
        saveDataToKeyChain("uid", Draco::instance()->Uid().toUtf8());
        saveDataToKeyChain("chatId", Draco::instance()->chatId().toUtf8());
        saveDataToKeyChain("stayLoggedIn", Draco::instance()->dracoToken().toUtf8());
    }
}

void ClientController::deleteStayLoggedInData()
{
    Draco::instance()->setStayLoggedIn(false);
    deleteDataFromKeyChain("uid");
    deleteDataFromKeyChain("chatId");
    deleteDataFromKeyChain("stayLoggedIn");
}

QByteArray ClientController::readDataFromKeyChain(const QString& key)
{
    QKeychain::ReadPasswordJob job("NunchukClient");
    job.setAutoDelete(false);
    job.setKey(key);
    QEventLoop loop;
    QKeychain::ReadPasswordJob::connect(&job, &QKeychain::Job::finished, &loop, &QEventLoop::quit);
    job.start();
    loop.exec();
    if (job.error() == QKeychain::Error::NoError){
        return job.binaryData();
    }
    else{
        DBG_INFO << "Could not read data from the keychain: " << qPrintable(job.errorString());
    }
    return QByteArray();
}

bool ClientController::saveDataToKeyChain(const QString& key, const QByteArray& data)
{
    QKeychain::WritePasswordJob job("NunchukClient");
    job.setAutoDelete(false);
    job.setKey(key);
    job.setBinaryData(data);
    QEventLoop loop;
    QKeychain::WritePasswordJob::connect(&job, &QKeychain::Job::finished, &loop, &QEventLoop::quit);
    job.start();
    loop.exec();
    if (job.error()){
        DBG_INFO << "Could not save data to the keychain: " << qPrintable(job.errorString());
        return false;
    }
    return true;
}

bool ClientController::deleteDataFromKeyChain(const QString &key)
{
    QKeychain::DeletePasswordJob job("NunchukClient");
    job.setAutoDelete(true);
    job.setKey(key);
    QEventLoop loop;
    QKeychain::DeletePasswordJob::connect(&job, &QKeychain::Job::finished, &loop, &QEventLoop::quit);
    job.start();
    loop.exec();
    if (job.error()){
        DBG_INFO << "Could not delete data from the keychain: " << qPrintable(job.errorString());
        return false;
    }
    return true;
}

bool ClientController::readAllDataFromKeyChain()
{
    QKeychain::ReadPasswordJob job("NunchukClient");
    job.setAutoDelete(false);
    QEventLoop loop;
    QKeychain::DeletePasswordJob::connect(&job, &QKeychain::Job::finished, &loop, &QEventLoop::quit);
    job.start();
    loop.exec();
    DBG_INFO  << job.key();

    return true;
}

void ClientController::UploadFile(const QString &file_name,
                                  const QString &mine_type,
                                  const QString &json_info,
                                  const char *data,
                                  int data_length)
{
    Q_UNUSED(file_name)
    Q_UNUSED(mine_type)

    if(connection()){
        if (data_length < 0 || (data_length > 0 && data == nullptr)) {
            DBG_INFO << "ClientController::UploadFile: invalid payload";
            return;
        }

        const QString tempDirectory = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        if (tempDirectory.isEmpty()) {
            DBG_INFO << "ClientController::UploadFile: temporary directory is unavailable";
            return;
        }

        // Never use the remote-provided file name as part of a local path. Apart
        // from path traversal, a relative path also depends on the process CWD.
        const QString fileTemplate = QDir(tempDirectory).absoluteFilePath(
                    QStringLiteral("nunchuk-matrix-upload-XXXXXX.txt"));
        QTemporaryFile file(fileTemplate);
        if (!file.open()) {
            DBG_INFO << "ClientController::UploadFile: cannot create temporary file"
                     << file.errorString();
            return;
        }
        file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);

        qint64 totalWritten = 0;
        while (totalWritten < data_length) {
            const qint64 written = file.write(data + totalWritten,
                                              data_length - totalWritten);
            if (written <= 0) {
                break;
            }
            totalWritten += written;
        }
        if (totalWritten != data_length || !file.flush()
                || file.error() != QFileDevice::NoError) {
            DBG_INFO << "ClientController::UploadFile: cannot write temporary file"
                     << file.errorString();
            return;
        }
        file.close();

        const QString filename = file.fileName();
        if (!QFileInfo(filename).isAbsolute()) {
            DBG_INFO << "ClientController::UploadFile: temporary path is not absolute";
            return;
        }

        // The uploader reads the path asynchronously, so keep the file after
        // this stack frame and remove it only after the upload succeeds.
        file.setAutoRemove(false);
        auto job = connection()->uploadFile(filename);
        if (!job) {
            QFile::remove(filename);
            DBG_INFO << "ClientController::UploadFile: cannot create upload job";
            return;
        }
        job->setMaxRetries(std::numeric_limits<int>::max());
        connect(job, &BaseJob::success, this, [job, json_info,filename]() {
            QString targetUri = job->contentUri().toString();
            DBG_INFO << "ClientController::UploadFile::success" << targetUri;
            QtConcurrent::run([targetUri, json_info, filename]() {
                matrixbrigde::UploadFileCallback(json_info, targetUri);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                QFile::remove(filename);
            });
            job->disconnect();
        }, Qt::QueuedConnection);
    }
}

void ClientController::DownloadFile(const QString &file_name, const QString &mine_type, const QString &json_info, const QString &mxc_uri)
{
    struct DownloadEventResult {
        QString matrixType;
        QString eventId;
        QString roomId;
    };
    struct DownloadedNunchukEventResult {
        QJsonObject event;
        NunchukEventBackendResolution backendResolution;
    };

    if(connection()){
        auto job = connection()->downloadFile(mxc_uri);
        job->setMaxRetries(std::numeric_limits<int>::max());
        connect(job, &BaseJob::success, this, [this, job, json_info, mxc_uri] () {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            QString targetFileName = job->targetFileName();
            QFile sourceFile(targetFileName);
            if(sourceFile.open(QIODevice::ReadOnly)){
                QByteArray byteArrayBuffer = sourceFile.readAll();
                sourceFile.close();
                std::vector<unsigned char> bufferToCompress(byteArrayBuffer.begin(), byteArrayBuffer.end());
                DBG_INFO << "DownloadFile: " << mxc_uri << byteArrayBuffer.size() << sourceFile.size() << bufferToCompress.size() << json_info.size();
                if (rooms()) {
                    QPointer<ClientController> safeThis(this);
                    runInThread(
                        this,
                        [json_info, bufferToCompress]() -> DownloadEventResult {
                            DownloadEventResult result;
                            matrixbrigde::DownloadFileCallback(json_info, bufferToCompress);
                            QJsonObject jsonResult = matrixbrigde::stringToJson(json_info);
                            result.eventId = jsonResult["event_id"].toString();
                            result.matrixType = jsonResult["type"].toString();
                            result.roomId = jsonResult["room_id"].toString();
                            return result;
                        },
                        [safeThis](DownloadEventResult result) {
                            if (!safeThis || !safeThis->rooms()) {
                                return;
                            }
                            QNunchukRoomPtr room = safeThis->rooms()->getRoomById(result.roomId);
                            if (!room) {
                                return;
                            }
                            if (!qUtils::strCompare(result.matrixType, NUNCHUK_EVENT_TRANSACTION)) {
                                return;
                            }

                            runInThread(
                                safeThis.data(),
                                [roomId = result.roomId,
                                 matrixType = result.matrixType,
                                 eventId = result.eventId]() -> DownloadedNunchukEventResult {
                                    QWarningMessage eventMessage;
                                    const nunchuk::NunchukMatrixEvent originEvent =
                                        matrixbrigde::GetEventData(roomId, eventId, eventMessage);
                                    DownloadedNunchukEventResult downloadedEvent;
                                    downloadedEvent.event = matrixbrigde::stringToJson(
                                                QString::fromStdString(
                                                    originEvent.get_content()));
                                    downloadedEvent.backendResolution =
                                            QNunchukRoom::resolveNunchukEventBackend(
                                                roomId,
                                                matrixType,
                                                eventId,
                                                downloadedEvent.event);
                                    return downloadedEvent;
                                },
                                [safeThis,
                                 room,
                                 matrixType = result.matrixType,
                                 eventId = result.eventId](
                                        DownloadedNunchukEventResult downloadedEvent) {
                                    if (!safeThis) {
                                        return;
                                    }
                                    Conversation cons;
                                    room->extractNunchukEvent(
                                                matrixType,
                                                eventId,
                                                downloadedEvent.event,
                                                cons,
                                                downloadedEvent.backendResolution);
                                });
                        });
                }
            }
            else{
                DBG_INFO << "ClientController::DownloadFile::success > CAN NOT OPEN FILE" << targetFileName;
            }
            job->disconnect();
        }, Qt::QueuedConnection);
    }
}

bool ClientController::isMatrixLoggedIn() const
{
    if(m_isNunchukLoggedIn
            && Draco::instance()->chatId() == m_activeMatrixUserId
            && Draco::instance()->Uid() == m_activeNunchukAccountId
            && !m_matrixLoginTransitioning
            && m_matrixConnectionState == MatrixConnectionState::Ready
            && connection()){
        return connection()->isLoggedIn();
    }
    return false;
}

QNunchukImageProvider* ClientController::imageprovider() const
{
    return m_imageprovider;
}

void ClientController::setImageprovider(QNunchukImageProvider *imageprovider)
{
    m_imageprovider = imageprovider;
}

QString ClientController::getRoomIdByWalletId(const QString &wallet_id) const
{
    if(rooms()){
        return rooms()->getRoomIdByWalletId(wallet_id);
    }
    return "";
}

void ClientController::transactionChanged(const QString &wallet_id, const QString &tx_id, const int status, const int height)
{
    if(rooms()){
        rooms()->transactionChanged(wallet_id, tx_id, status, height);
    }
}

void ClientController::updateTransactionMemo(const QString& wallet_id, const QString &tx_id, const QString &memo)
{
    if(rooms()){
        rooms()->updateTransactionMemo(wallet_id, tx_id, memo);
    }
}
