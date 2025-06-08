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
    ,m_ReadySupport(true)
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
    m_connection = OurSharedPointer<Quotient::Connection>(c);
    connect(m_connection.data(), &Connection::stateChanged, this, &ClientController::isMatrixLoggedInChanged);
    if(m_imageprovider){
        m_imageprovider->setConnection(m_connection.data());
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
    if (!isNunchukLoggedIn()){
        return;
    }
    bridge::StopConsumeGroupEvent();
    if(m_loginHandler){
        m_loginHandler.data()->requestLogout();
    }
    Draco::instance()->signout();
    bridge::stopNunchuk();
    QEventProcessor::instance()->sendEvent(E::EVT_LOGIN_MATRIX_REQUEST);
    setIsNunchukLoggedIn(false);
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
    if(rooms()){
        rooms()->createSupportRoom();
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

void ClientController::setSubscriptions(const QJsonArray &data)
{
    m_slugs.clear();
    QJsonArray valid_subcriptions;

    foreach (const QJsonValue &value, data) {
        QJsonObject subscription = value.toObject();
        qint64 grace_valid_until_utc_millis = static_cast<qint64>(subscription.value("grace_valid_until_utc_millis").toDouble());
        QDateTime grace_valid_until_utc = QDateTime::fromMSecsSinceEpoch(grace_valid_until_utc_millis);
        bool valid = grace_valid_until_utc.isValid() && grace_valid_until_utc >= QDateTime::currentDateTime();
        DBG_INFO << grace_valid_until_utc.toString("yyyy-MM-dd-hh-mm-ss-zzz") << valid;
        if (AppSetting::instance()->primaryServer() == (int)nunchuk::Chain::TESTNET){
            m_slugs.append(subscription["plan"].toObject()["slug"].toString());
            valid_subcriptions.push_back(subscription);
        }
        else if(valid){
            m_slugs.append(subscription["plan"].toObject()["slug"].toString());
            valid_subcriptions.push_back(subscription);
        }
    }
    m_subscriptions = valid_subcriptions;
    if(m_subscriptions.size() > 0){
        setAttachmentEnable(true);
    }
    else{
        setAttachmentEnable(false);
    }
    DBG_INFO << m_slugs;
    emit subscriptionsChanged();
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
                            if(!qUtils::strCompare(matrixType, NUNCHUK_EVENT_TRANSACTION)){
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
