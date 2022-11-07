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
#include "Draco.h"
#include "ViewsEnums.h"
#include "Chats/matrixbrigde.h"
#include "jobs/downloadfilejob.h"
#include "localization/STR_CPP.h"
#include <QSqlDatabase>
#include <database.h>
#include <QTextDocument>

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
    ,m_guestMode(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(NetworkAccessManager::instance(), &QNetworkAccessManager::proxyAuthenticationRequired, this, &ClientController::proxyAuthenticationRequired);
    connect(NetworkAccessManager::instance(), &QNetworkAccessManager::sslErrors, this, &ClientController::sslErrors);
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
    return m_connection;
}

void ClientController::setConnection(Connection *c)
{
    if(m_connection){
        delete m_connection;
        m_connection = NULL;
    }
    m_connection = c;
    connect(m_connection, &Connection::stateChanged, this, &ClientController::isMatrixLoggedInChanged);
    if(m_imageprovider){
        m_imageprovider->setConnection(m_connection);
    }
    emit connectionChanged();
    connectSingleShot(m_connection, &Connection::connected, this, [this]{
        setIsNunchukLoggedIn(true);
        connection()->loadState();
        connection()->sync();
        connect(m_connection->user(), &User::defaultAvatarChanged, this, &ClientController::onUserAvatarChanged );
        connect(m_connection->user(), &User::defaultNameChanged, this, &ClientController::onUserDisplaynameChanged );
        connectSingleShot(m_connection, &Connection::syncDone, this, [this] {
            if(rooms()){
                rooms()->downloadRooms();
            }
            connection()->syncLoop();
        });
        QTimer::singleShot(1000, [this]() {
            refreshContacts();
        });
        emit userChanged();
    }, Qt::QueuedConnection);
    connectSingleShot(m_connection, &Connection::loggedOut, this, []{
        DBG_INFO << "MATRIX HAS BEEN LOGGEDOUT";
    }, Qt::QueuedConnection);
}

bool ClientController::isNunchukLoggedIn() const
{
    return m_isNunchukLoggedIn;
}

void ClientController::setIsNunchukLoggedIn(bool isLogged)
{
    if(m_isNunchukLoggedIn != isLogged){
        m_isNunchukLoggedIn = isLogged;
        emit isNunchukLoggedInChanged();
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

void ClientController::requestLogin()
{
    setConnection(new Quotient::Connection());
    setLoginHandler(QLogginManagerPtr(new QLogginManager(connection())));
    setRooms(QNunchukRoomListModelPtr(new QNunchukRoomListModel(connection())));
    if(loginHandler()){
        loginHandler()->invokeLogin(Draco::instance()->chatId(), Draco::instance()->dracoToken());
    }
}

void ClientController::syncContacts(QList<DracoUser> data)
{
    this->contacts()->syncContacts(data);
    emit contactsChanged();
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

void ClientController::setContacts(const QContactModelPtr &contacts)
{
    m_contacts = contacts;
    emit contactsChanged();
}

void ClientController::syncContactsReceived(QList<DracoUser> data)
{
    this->contactsReceived()->syncContacts(data);
    emit contactsReceivedChanged();
}

QContactModel *ClientController::contactsReceived() const
{
    return m_contactsReceived.data();
}

QContactModelPtr ClientController::contactsReceivedPtr() const
{
    return m_contactsReceived;
}

void ClientController::setContactsReceived(const QContactModelPtr &contactsReceived)
{
    m_contactsReceived = contactsReceived;
    emit contactsReceivedChanged();
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
    this->contactsSent()->syncContacts(data);
    emit contactsSentChanged();
}

QContactModel *ClientController::contactsSent() const
{
    return m_contactsSent.data();
}

QContactModelPtr ClientController::contactsSentPtr() const
{
    return m_contactsSent;
}

void ClientController::setContactsSent(const QContactModelPtr &contactsSent)
{
    m_contactsSent = contactsSent;
    emit contactsSentChanged();
}

void ClientController::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    DBG_INFO;
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
    if(isNunchukLoggedIn()){
        if(m_loginHandler){
            m_loginHandler.data()->requestLogout();
        }
        if(Draco::instance()->signout()){
            QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_MATRIX_REQUEST);
            setIsNunchukLoggedIn(false);
            deleteStayLoggedInData();
            AppSetting::instance()->setGroupSetting("");
            AppModel::instance()->signoutClearData();
            if(rooms()){
                rooms()->removeAll();
            }
            if(contacts()){
                contacts()->removeAll();
            }
        }
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
    connect(m_rooms.data(), &QNunchukRoomListModel::noticeService, this, &ClientController::refreshContacts);
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
            if(connection()->user()->setAvatar(file_path)){
                QTimer::singleShot(1000,[=]{
                    connection()->user()->load();
                });
            }
        }else{
            connection()->user()->removeAvatar();
            QTimer::singleShot(1000,[=]{
                Draco::instance()->setUserProfile(m_me.name,"");
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

void ClientController::createRoomChat(const QStringList &invitees, const QStringList &name, QVariant firstMessage)
{
    if(rooms()){
        QString roomname = name.join(",");
        QString roomtopic = QString("Private conversation of %1").arg(roomname);
        rooms()->createRoomChat(invitees, roomname, roomname, firstMessage);
    }
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
    QKeychain::ReadPasswordJob job(qAppName());
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
    QKeychain::WritePasswordJob job(qAppName());
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
    QKeychain::DeletePasswordJob job(qAppName());
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

void ClientController::UploadFile(const QString &file_name,
                                  const QString &mine_type,
                                  const QString &json_info,
                                  const char *data,
                                  int data_length)
{
    if(connection()){
        QString filename = QString("%1_%2.txt").arg(file_name).arg(QDateTime::currentDateTime().toTime_t());
        QSaveFile file(filename);
        if ( file.open(QIODevice::WriteOnly) )
        {
            QDataStream stream( &file );
            stream.writeRawData(data, data_length);
        }
        file.commit();
        if(file.error() == QFileDevice::NoError){
            auto job = connection()->uploadFile(filename);
            job->setMaxRetries(std::numeric_limits<int>::max());
            connect(job, &BaseJob::success, this, [job, json_info,filename] {
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
}

void ClientController::DownloadFile(const QString &file_name, const QString &mine_type, const QString &json_info, const QString &mxc_uri)
{
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
                QtConcurrent::run([this, targetFileName, json_info, bufferToCompress]() {
                    if(rooms()){
                        matrixbrigde::DownloadFileCallback(json_info, bufferToCompress);
                        QJsonObject jsonResult = matrixbrigde::stringToJson(json_info);
                        QString event_id = jsonResult["event_id"].toString();
                        QString matrixType = jsonResult["type"].toString();
                        QString room_id = jsonResult["room_id"].toString();
                        QNunchukRoomPtr room = rooms()->getRoomById(room_id);
                        if(room){
                            if(0 != QString::compare(matrixType, NUNCHUK_EVENT_TRANSACTION, Qt::CaseInsensitive)){
                                return;
                            }
                            Conversation cons;
                            QWarningMessage evnmsg;
                            QNunchukMatrixEvent originEvent = matrixbrigde::GetEvent(room_id, event_id, evnmsg);
                            QJsonObject jsonEvent = matrixbrigde::stringToJson(originEvent.get_content());
                            room->extractNunchukEvent(matrixType,event_id,jsonEvent,cons);
                        }
                    }
                });
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
    if(connection()){
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
