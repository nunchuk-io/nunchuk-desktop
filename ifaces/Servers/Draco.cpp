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
#include "Draco.h"
#include "QOutlog.h"
#include <QQmlEngine>
#include <QAuthenticator>
#include <QNetworkCookieJar>
#include <QEventLoop>
#include <QUrlQuery>
#include <QSysInfo>
#include "ViewsEnums.h"
#include "localization/STR_CPP.h"
#include <QSysInfo>
#include "ServiceSetting.h"
#include "QThreadForwarder.h"

using namespace Command;
Draco* Draco::m_instance = NULL;
Draco::Draco() :
    m_uid(""), m_pid(""),
    m_emailRequested(""),
    m_expireSec(-1),
    m_ChatId(""),
    m_loginHalfToken(""),
    m_deviceId(""),
    m_stayLoggedIn(false),
    m_isSubscribed(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Draco::~Draco()
{

}

Draco *Draco::instance()
{
    if(NULL == m_instance){
        m_instance = new Draco();
    }
    return m_instance;
}

void Draco::refreshDevices()
{
    QtConcurrent::run([this]() {
        CLIENT_INSTANCE->syncDevices(getDevices());
    });
}

void Draco::refreshContacts()
{
    if(CLIENT_INSTANCE->isNunchukLoggedIn()){
        QtConcurrent::run([this]() {
            QList<DracoUser> contacts = getContacts();
            QList<DracoUser> contactsSent = getContactsSent();
            QList<DracoUser> contactsReceived = getContactsReceived();

            DBG_INFO << "contacts size: " << contacts.size();
            QThreadForwarder::instance()->forwardInQueuedConnection([contacts](){
                CLIENT_INSTANCE->syncContacts(contacts);
            });
            QThreadForwarder::instance()->forwardInQueuedConnection([contactsSent](){
                CLIENT_INSTANCE->syncContactsSent(contactsSent);
            });
            QThreadForwarder::instance()->forwardInQueuedConnection([contactsReceived](){
                CLIENT_INSTANCE->syncContactsReceived(contactsReceived);
            });
        });
    }
}

void Draco::checkAccountAvailability(const QString &email)
{
    DBG_INFO << email;
    QJsonObject data;
    data["username"] = email;
    data["type"] = "Email";
    int     reply_code      = -1;
    QString reply_msg    = "";

    QJsonObject jsonObj = getSync(commands[Common::CMD_IDX::ACCOUNT_AVAILABILITY], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code    = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj.value("data").toObject();
            bool activated = data.value("activated").toBool();
            if(!activated){
                response_code = DRACO_CODE::ACCOUNT_NOT_ACTIVATED;
            }
            bool has_subscription = data.value("has_subscription").toBool();
            setIsSubscribed(has_subscription);
        }
        // FIXME
        // else {
        //     AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        // }
        DBG_INFO << jsonObj;
        setEmailRequested(email);
        emit accountAvailabilityResult(reply_code, response_code, response_msg);
    }
}

bool Draco::getCurrencies(QJsonObject &output, QString &errormsg)
{
    int reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Premium::CMD_IDX::GET_CURRENCIES], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        output = jsonObj;
        return true;
    }
    errormsg = reply_msg;
    return false;
}

void Draco::btcRates()
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QUrl url = QUrl::fromUserInput("https://api.nunchuk.io/v1/prices");
    QNetworkRequest requester_(url);
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        return;
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->get(requester_));
    QEventLoop eventLoop;
    QObject::connect(reply.get(), &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        QJsonObject jsonObj = json.object();
        QJsonObject data = jsonObj["data"].toObject();
        QJsonObject prices = data["prices"].toObject();
        QJsonObject btc = prices["BTC"].toObject();
        double rates_double  = btc["USD"].toDouble();
        AppModel::instance()->setBtcRates(rates_double);
    }
    reply->deleteLater();
    reply.release();
}

void Draco::exchangeRates(const QString &currency)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QUrl url = QUrl::fromUserInput("https://api.nunchuk.io/v1.1/forex/rates");
    QNetworkRequest requester_(url);
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        return;
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->get(requester_));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        QJsonObject jsonObj = json.object();
        double rates_double  = jsonObj[currency].toDouble();
        AppModel::instance()->setExchangeRates(rates_double);
        AppSetting::instance()->updateUnit();
    }
    reply->deleteLater();
    reply.release();
}

void Draco::feeRates()
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QUrl url;
    switch (AppSetting::instance()->primaryServer()) {
    case (int)AppSetting::Chain::TESTNET:
        url = QUrl::fromUserInput("https://api.nunchuk.io/v1.1/fees/testnet/recommended");
        break;
    case (int)AppSetting::Chain::SIGNET:
        url = QUrl::fromUserInput("https://api.nunchuk.io/v1.1/fees/signet/recommended");
        break;
    default:
        url = QUrl::fromUserInput("https://api.nunchuk.io/v1.1/fees/recommended");
        break;
    }
    QNetworkRequest requester_(url);
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        return;
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->get(requester_));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        QJsonObject jsonObj = json.object();
        AppModel::instance()->setFastestFee(jsonObj["fastestFee"].toInt());
        AppModel::instance()->setHalfHourFee(jsonObj["halfHourFee"].toInt());
        AppModel::instance()->setHourFee(jsonObj["hourFee"].toInt());
        AppModel::instance()->setMinFee(jsonObj["minimumFee"].toInt());
        AppModel::instance()->setLasttimeCheckEstimatedFee(QDateTime::currentDateTime());
    }
    reply->deleteLater();
    reply.release();
}

void Draco::verifyNewDevice(const QString &pin)
{
    QJsonObject data;
    data["email"] = Uid();
    data["loginHalfToken"] = loginHalfToken();
    data["pin"] = pin;
    data["deviceId"] = deviceId();
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::VERIFY_NEW_DEVICE], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK) {
            QJsonObject dataObj = jsonObj["data"].toObject();
            setDeviceId(dataObj["deviceId"].toString());
            setDracoToken(dataObj["tokenId"].toString());
            setExpireSec(dataObj["expireInSeconds"].toInt());
            getMe();
            CLIENT_INSTANCE->setIsNewDevice(true);
        }
        else{
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
        emit verifyNewDeviceResult(reply_code, response_code, response_msg);
    }
}

void Draco::resendVerifyNewDeviceCode()
{
    QJsonObject data;
    data["email"] = Uid();
    data["deviceId"] = deviceId();
    data["loginHalfToken"] = loginHalfToken();
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::RESEND_VERIFY_NEW_DEVICE_CODE], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit resendVerifyNewDeviceCodeResult(reply_code, response_code, response_msg);
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::inviteFriends(const QStringList& emails)
{
    if(emails.count() > 0){
        QJsonObject data;
        data["friendEmails"] = QJsonArray::fromStringList(emails);
        int     reply_code = -1;
        QString reply_msg  = "";
        QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::INVITE_FRIEND], data, reply_code, reply_msg);
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            emit resendVerifyNewDeviceCodeResult(reply_code, response_code, response_msg);
            if(response_code != DRACO_CODE::RESPONSE_OK){
                AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            }
        }
    }
}

void Draco::removeContact(const QString &contact_id)
{
    QJsonObject data;
    data["contact_id"] = contact_id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[Common::CMD_IDX::REMOVE_FRIEND], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::cancelFriendRequest(const QString &contact_id)
{
    QJsonObject data;
    data["contact_id"] = contact_id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[Common::CMD_IDX::CANCEL_REQUEST_FRIEND], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

QString Draco::deviceId() const
{
    return m_deviceId;
}

void Draco::setDeviceId(const QString &value)
{
    if(m_deviceId != value){
        m_deviceId = value;
        emit deviceIdChanged();
    }
}

QString Draco::loginHalfToken() const
{
    return m_loginHalfToken;
}

void Draco::setLoginHalfToken(const QString &value)
{
    if(m_loginHalfToken != value){
        m_loginHalfToken = value;
        emit loginHalfTokenChanged();
    }
}

QString Draco::chatId() const
{
    return m_ChatId;
}

void Draco::setChatId(const QString &value)
{
    if(m_ChatId != value){
        m_ChatId = value;
        emit chatIdChanged();
    }
}

void Draco::reset()
{
    setUid("");
    setPid("");
    setEmailRequested("");
    setExpireSec(0);
    setChatId("");
    setDracoToken("");
    setDeviceId("");
    setIsSubscribed(false);
}

int Draco::expireSec() const
{
    return m_expireSec;
}

void Draco::setExpireSec(int value)
{
    m_expireSec = value;
    emit expireSecChanged();
}

QString Draco::emailRequested() const
{
    return m_emailRequested;
}

void Draco::setEmailRequested(const QString &value)
{
    if(m_emailRequested != value){
        m_emailRequested = value;
        emit emailRequestedChanged();
    }
}

QString Draco::Uid() const
{
    return m_uid;
}

void Draco::setUid(const QString &currentUser)
{
    if(m_uid != currentUser){
        m_uid = currentUser;
        emit uidChanged();
    }
}

QString Draco::Pid() const
{
    return m_pid;
}

void Draco::setPid(const QString &pid)
{
    if(m_pid != pid){
        m_pid = pid;
        emit pidChanged();
    }
}

void Draco::createAccount(const QString& name, const QString& email)
{
    QJsonObject data;
    data["name"] = name;
    data["email"] =  email;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::CREATE_ACCOUNT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit createAccountResult(reply_code, response_code, response_msg);
        if(response_code == DRACO_CODE::RESPONSE_OK){
            setUid(email);
            QJsonObject dataObj = jsonObj["data"].toObject();
            setDeviceId(dataObj["deviceId"].toString());
            setDracoToken(dataObj["tokenId"].toString());
            setExpireSec(dataObj["expireInSeconds"].toInt());
            setEmailRequested(email);
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::singin(const QString& email, const QString& password)
{
    QJsonObject data;
    data["email"] = email;
    data["password"] = password;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::SIGNIN], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        setUid(email);
        setPid(password);
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            setDeviceId(dataObj["deviceId"].toString());
            setDracoToken(dataObj["tokenId"].toString());
            setExpireSec(dataObj["expireInSeconds"].toInt());
            setEmailRequested(email);
            getMe();
        }
        else if(response_code == DRACO_CODE::LOGIN_NEW_DEVICE){
            QJsonObject detailsObj = errorObj["details"].toObject();
            setDeviceId(detailsObj["deviceId"].toString());
            setLoginHalfToken(detailsObj["halfToken"].toString());
            setEmailRequested(email);
        }
        else{
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
        emit signinResult(reply_code, response_code, response_msg);
    }
}

bool Draco::signout()
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::SIGNOUT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        reset();
        AppModel::instance()->stopCheckAuthorize();
        return true;
    }
    else{
        return false;
    }
}

void Draco::recoverPassword(const QString& email, const QString& forgotToken, const QString& newpassword)
{
    QJsonObject data;
    data["email"] = email;
    data["forgotPasswordToken"] = forgotToken;
    data["newPassword"] = newpassword;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::RECOVER_PASSWORD], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code    = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit recoverPasswordResult(reply_code, response_code, response_msg);
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::forgotPassword(const QString& email)
{
    QJsonObject data;
    data["email"] = email;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::FORGOT_PASSWORD], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit forgotPasswordResult(reply_code, response_code, response_msg);
        if(response_code == DRACO_CODE::RESPONSE_OK){
            setEmailRequested(email);
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::resendPassword(const QString &email)
{
    QJsonObject data;
    data["email"] = email;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::RESEND_PASSWORD], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::changePassword(const QString& oldpassword, const QString& newpassword)
{
    QJsonObject data;
    data["oldPassword"] = oldpassword;
    data["newPassword"] = newpassword;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::CHANGE_PASSWORD], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            setPid(newpassword);
            getMe();
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
        emit changePasswordResult(reply_code, response_code, response_msg);
    }
}

void Draco::getMe()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    DracoUser user;
    QJsonObject jsonObj = getSync(commands[Common::CMD_IDX::GET_ME], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject userObj = dataObj["user"].toObject();
            setChatId(userObj["chat_id"].toString());
            user.id          = userObj["id"].toString();
            user.name        = userObj["name"].toString();
            user.email       = userObj["email"].toString();
            user.gender      = userObj["gender"].toString();
            user.avatar      = userObj["avatar"].toString();
            user.status      = userObj["status"].toString();
            user.status_text = userObj["status_text"].toString();
            user.chat_id     = userObj["chat_id"].toString();
            user.username    = userObj["username"].toString();
            user.login_type  = userObj["login_type"].toString();
            if(!qUtils::strCompare(user.email, AppSetting::instance()->groupSetting())){
                AppSetting::instance()->setGroupSetting(user.email);
            }
            AppModel::instance()->startCheckAuthorize();
            CLIENT_INSTANCE->setIsNunchukLoggedIn(true);
            getUserSubscriptions();
        }
        else if(response_code == DRACO_CODE::UNAUTHORIZED){
            CLIENT_INSTANCE->requestSignout();
            AppModel::instance()->stopCheckAuthorize();
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }

    CLIENT_INSTANCE->setMe(user);
}

void Draco::getMepKey(const QString &public_address)
{
    QString cmd = QString("%1%2").arg(commands[PrimaryKey::CMD_IDX::PRIMARY_KEY_PUBLIC_ADDRESS], public_address);
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(cmd, QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject userObj = dataObj["user"].toObject();
            setChatId(userObj["chat_id"].toString());
            DracoUser user;
            user.id          = userObj["id"].toString();
            user.name        = userObj["name"].toString();
            user.email       = userObj["email"].toString();
            user.gender      = userObj["gender"].toString();
            user.avatar      = userObj["avatar"].toString();
            user.status      = userObj["status"].toString();
            user.status_text = userObj["status_text"].toString();
            user.chat_id     = userObj["chat_id"].toString();
            user.username    = userObj["username"].toString();
            user.login_type  = userObj["login_type"].toString();
            if(!qUtils::strCompare(user.email, AppSetting::instance()->groupSetting())){
                AppSetting::instance()->setGroupSetting(user.email);
            }
            CLIENT_INSTANCE->setMe(user);
            AppModel::instance()->startCheckAuthorize();
            CLIENT_INSTANCE->setIsNunchukLoggedIn(true);
        }
        else if(response_code == DRACO_CODE::UNAUTHORIZED){
            CLIENT_INSTANCE->requestSignout();
            AppModel::instance()->stopCheckAuthorize();
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

DracoUser Draco::search(const QString &email)
{
    DracoUser user;
    QJsonObject data;
    data["email"] = email;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Common::CMD_IDX::SEARCH_FRIEND], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject userObj = dataObj["user"].toObject();
            user.id = userObj["id"].toString();
            user.name = userObj["name"].toString();
            user.email = userObj["email"].toString();
            user.gender = userObj["gender"].toString();
            user.avatar = userObj["avatar"].toString();
            user.status = userObj["status"].toString();
            user.status_text = userObj["status_text"].toString();
            user.chat_id = userObj["chat_id"].toString();
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return user;
}

QVariant Draco::requestFriends(const QVariant emails)
{
    QVariantMap ret;
    QStringList failed_emails;
    failed_emails.clear();
    ret.clear();
    ret["result"] = false;
    ret["failedEmails"] = failed_emails;
    QStringList inputs = emails.toStringList();
    if(inputs.count() <= 0){
        return QVariant::fromValue(ret);
    }
    QJsonObject data;
    data["emails"] = QJsonArray::fromStringList(inputs);
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = putSync(commands[Common::CMD_IDX::REQUEST_FRIEND], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QStringList failed_emails = dataObj["failed_emails"].toVariant().toStringList();
            refreshContacts();
            ret["result"] = true;
            ret["failedEmails"] = failed_emails;
            return QVariant::fromValue(ret);
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return QVariant::fromValue(ret);
}

bool Draco::requestSingleFriend(const QString &email)
{
    bool ret = false;
    DracoUser user = search(email);
    if(user.id != ""){
        QJsonObject data;
        data["contact_id"] = user.id;
        int     reply_code = -1;
        QString reply_msg  = "";
        QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::REQUEST_FRIEND], data, reply_code, reply_msg);
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                ret = true;
            }
            else {
                AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            }
        }
    }
    return ret;
}

bool Draco::requestMutipleFriend(const QStringList &email, QStringList &failed_emails)
{
    if(email.count() > 0){
        QJsonObject data;
        data["emails"] = QJsonArray::fromStringList(email);
        int     reply_code = -1;
        QString reply_msg  = "";
        QJsonObject jsonObj = putSync(commands[Common::CMD_IDX::REQUEST_FRIEND], data, reply_code, reply_msg);
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                QJsonObject dataObj = jsonObj["data"].toObject();
                failed_emails = dataObj["failed_emails"].toVariant().toStringList();
                return true;
            }
            else {
                AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            }
        }
    }
    return false;
}

QList<DracoUser> Draco::getContacts()
{
    QList<DracoUser> contacts;
    contacts.clear();
    QJsonObject data;
    data["lastContactId"] = "0";
    data["count"] = 20;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Common::CMD_IDX::GET_FRIENDS_LIST], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonArray friends = dataObj["friends"].toArray();
            for (auto v : friends) {
                QJsonObject it = v.toObject();
                DracoUser user;
                user.id          = it["id"].toString();
                user.name        = it["name"].toString();
                user.email       = it["email"].toString();
                user.gender      = it["gender"].toString();
                user.avatar      = it["avatar"].toString();
                user.status      = it["status"].toString();
                user.status_text = it["status_text"].toString();
                user.chat_id     = it["chat_id"].toString();
                contacts.append(user);
            }
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return contacts;
}

QList<DracoUser> Draco::getContactsSent()
{
    QList<DracoUser> contacts;
    contacts.clear();

    QJsonObject data;
    data["lastContactId"] = "0";
    data["count"] = 20;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Common::CMD_IDX::GET_FRIENDS_SENT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonArray friends = dataObj["users"].toArray();
            for (auto v : friends) {
                QJsonObject it = v.toObject();
                DracoUser user;
                user.id          = it["id"].toString();
                user.name        = it["name"].toString();
                user.email       = it["email"].toString();
                user.gender      = it["gender"].toString();
                user.avatar      = it["avatar"].toString();
                user.status      = it["status"].toString();
                user.status_text = it["status_text"].toString();
                user.chat_id     = it["chat_id"].toString();
                contacts.append(user);
            }
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return contacts;
}

QList<DracoUser> Draco::getContactsReceived()
{
    QList<DracoUser> contacts;
    contacts.clear();

    QJsonObject data;
    data["lastContactId"] = "0";
    data["count"] = 20;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Common::CMD_IDX::GET_FRIENDS_RECEIVED], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonArray friends = dataObj["users"].toArray();
            for (auto v : friends) {
                QJsonObject it = v.toObject();
                DracoUser user;
                user.id          = it["id"].toString();
                user.name        = it["name"].toString();
                user.email       = it["email"].toString();
                user.gender      = it["gender"].toString();
                user.avatar      = it["avatar"].toString();
                user.status      = it["status"].toString();
                user.status_text = it["status_text"].toString();
                user.chat_id     = it["chat_id"].toString();
                contacts.append(user);
            }
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return contacts;
}

QList<DracoUser> Draco::getRoomMembers(const QString &id)
{
    DBG_INFO;
    QList<DracoUser> members;
    members.clear();
    QString cmd = QString("%1/%2/members").arg(commands[Common::CMD_IDX::GET_ROOM_MEMBERS], id);
    QJsonObject data;
    data["lastContactId"] = "0";
    data["count"] = 20;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonArray friends = dataObj["users"].toArray();
            for (auto v : friends) {
                QJsonObject it = v.toObject();
                DracoUser user;
                user.id          = it["id"].toString();
                user.name        = it["name"].toString();
                user.email       = it["email"].toString();
                user.gender      = it["gender"].toString();
                user.avatar      = it["avatar"].toString();
                user.status      = it["status"].toString();
                user.status_text = it["status_text"].toString();
                user.chat_id     = it["chat_id"].toString();
                members.append(user);
            }
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return members;
}

QList<DracoDevice> Draco::getDevices()
{
    QList<DracoDevice> mDevices;
    mDevices.clear();
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Common::CMD_IDX::GET_DEVICES_LIST], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonArray devices = dataObj["devices"].toArray();
            QStringList ids;
            for (auto v : devices) {
                QJsonObject it = v.toObject();
                DracoDevice device;
                device.id          = it["id"].toString();
                QString names = it["name"].toString();
                QStringList _name = names.split("_");
                _name.removeOne("nunchuk-Desktop");
                QStringList listName;
                for(QString n: _name){
                    if(!listName.contains(n)){
                        listName.append(n);
                    }
                }
                device.name        = listName.join("_");
                device.lastIP      = it["lastIP"].toString();
                device.lastTs      = it["lastTs"].toString();
                if(CLIENT_INSTANCE->loginHandler()){
                    device.thisId = CLIENT_INSTANCE->loginHandler()->devicename().contains(device.name);
                }
                if(ids.contains(device.id) == false){
                    ids.append(device.id);
                    mDevices.append(device);
                }
            }
        }
        else {
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return mDevices;
}

void Draco::accecptFriendRequest(const QString &id)
{
    QJsonObject data;
    data["contact_id"] = id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::ACCEPT_FRIEND_REQUEST], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::ignoreFriendRequest(const QString &id)
{
    QJsonObject data;
    data["contact_id"] = id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[Common::CMD_IDX::IGNORE_FRIEND_REQUEST], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

bool Draco::deleteCurrentUser()
{
    bool ret = false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[Common::CMD_IDX::DELETE_ACCOUNT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            ret = true;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return ret;
}

bool Draco::sendDeleteConfirmation(const QString &code)
{
    bool ret = false;
    QJsonObject data;
    data["confirmation_code"] = code;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::DELETE_CONFIRM], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->stopCheckAuthorize();
            reset();
            AppModel::instance()->startRemoveAllWallets();
            ret = true;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    else{
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            AppModel::instance()->stopCheckAuthorize();
            reset();
            AppModel::instance()->startRemoveAllWallets();
            ret = true;
        }
    }
    return ret;
}

void Draco::setUserProfile(const QString &name, const QString &avartaPath)
{
    DracoUser user = CLIENT_INSTANCE->getMe();
    QJsonObject data;
    if(name != "" && name != user.name){
        data["name"] = name;
    }
    if(avartaPath != "" && avartaPath != user.avatar){
        data["avatar_url"] = avartaPath;
    }
    data["gender"] = user.gender;
    data["status_text"] = user.status_text;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::USER_PROFILE], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit updateProfileResult(reply_code,response_code,response_msg);
        if(response_code == DRACO_CODE::RESPONSE_OK) {
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject userObj = dataObj["user"].toObject();
            user.id = userObj["id"].toString();
            user.name = userObj["name"].toString();
            user.email = userObj["email"].toString();
            user.gender = userObj["gender"].toString();
            user.avatar = userObj["avatar"].toString();
            user.status = userObj["status"].toString();
            user.status_text = userObj["status_text"].toString();
            user.chat_id = userObj["chat_id"].toString();
            CLIENT_INSTANCE->setMe(user);
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::checkForUpdate()
{
    QString title = "";
    QString message = "";
    QString doItLaterCTALbl = "";
    int result = 0;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Common::CMD_IDX::CHECK_FOR_UPDATE], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            bool isUpdateAvailable = dataObj["isUpdateAvailable"].toBool();
            bool isUpdateRequired = dataObj["isUpdateRequired"].toBool();
            doItLaterCTALbl = dataObj["btnCTA"].toString();
            message = dataObj["message"].toString();
            title = dataObj["title"].toString();
            if(isUpdateRequired){
                result = 2;
            }
            else if(isUpdateAvailable){
                result = 1;
            }
            emit startCheckForUpdate(result,title,message,doItLaterCTALbl);
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::signoutDeice(const QString &device_id)
{
    QJsonObject data;
    QJsonArray array;
    array.push_back(device_id);
    data["devices"] = array;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[Common::CMD_IDX::DEVICE_SIGNOUT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit loggedInDeviceChanged(reply_code,response_code,response_msg);
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::signoutAllDeices()
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[Common::CMD_IDX::DEVICE_ALL_SIGNOUT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit loggedInDeviceChanged(reply_code,response_code,response_msg);
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

void Draco::markAsCompromised(const QString &device_id)
{
    QJsonObject data;
    QJsonArray array;
    array.push_back(device_id);
    data["devices"] = array;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Common::CMD_IDX::DEVICE_MARK_COMPROMISED], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit loggedInDeviceChanged(reply_code,response_code,response_msg);
        if(response_code != DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
}

bool Draco::pkey_signup(const QString &address, const QString &username, const QString &signature)
{
    bool ret = false;
    QJsonObject data;
    data["address"] = address;
    data["username"] =  username;
    data["signature"] =  signature;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[PrimaryKey::CMD_IDX::PRIMARY_KEY_SIGNUP], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        setUid(username);
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            setDeviceId(dataObj["deviceId"].toString());
            setDracoToken(dataObj["tokenId"].toString());
            setExpireSec(dataObj["expireInSeconds"].toInt());
            getMe();
            AppModel::instance()->setPrimaryKey(username);
            ret = true;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
        emit signalpkey_signup(reply_code, response_code, response_msg);
    }
    return ret;
}

bool Draco::pkey_signin(const QString &address, const QString &username, const QString &signature)
{
    bool ret = false;
    QJsonObject data;
    if(address != ""){
        data["address"] = address;
    }
    if(username != ""){
        data["username"] =  username;
    }
    data["signature"] =  signature;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[PrimaryKey::CMD_IDX::PRIMARY_KEY_SIGNIN], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(reply_code == DRACO_CODE::SUCCESSFULL) {
            setUid(username);
            if(response_code == DRACO_CODE::RESPONSE_OK){
                ret = true;
                QJsonObject dataObj = jsonObj["data"].toObject();
                setDeviceId(dataObj["deviceId"].toString());
                setDracoToken(dataObj["tokenId"].toString());
                setExpireSec(dataObj["expireInSeconds"].toInt());
                getMe();
                AppModel::instance()->setPrimaryKey(username);
            }
            else if(response_code == DRACO_CODE::LOGIN_NEW_DEVICE){
                ret = true;
                QJsonObject detailsObj = errorObj["details"].toObject();
                setDeviceId(detailsObj["deviceId"].toString());
                setLoginHalfToken(detailsObj["halfToken"].toString());
            }
            else{
                AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG );
            }
        }
        emit signalpkey_signin(reply_code, response_code, response_msg);
    }
    return ret;
}

QString Draco::get_pkey_nonce(const QString &address,const QString &username)
{
    QJsonObject data;
    data["address"] = address;
    data["username"] =  username;
    QString ret = "";
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[PrimaryKey::CMD_IDX::PRIMARY_KEY_NONCE], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            ret = dataObj["nonce"].toString();
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return ret;
}

QString Draco::pkey_manual_nonce(const QString &address, const QString &username, const QString &nonce, const QString &type)
{
    QString ret = "";
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject data;
    if(address != ""){
        data["address"] = address;
    }
    if(username != ""){
        data["username"] =  username;
    }
    if(username != ""){
        data["nonce"] =  nonce;
    }
    QJsonObject jsonObj;
    if(type == "change_pkey"){
        QString cmd = QString("%1%2").arg(commands[PrimaryKey::CMD_IDX::PRIMARY_KEY_NONCE], "?type=change_pkey");
        jsonObj = postSync(cmd, data, reply_code, reply_msg);
    }else{
        jsonObj = postSync(commands[PrimaryKey::CMD_IDX::PRIMARY_KEY_NONCE], data, reply_code, reply_msg);
    }
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            ret = dataObj["nonce"].toString();
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return ret;
}

bool Draco::pkey_username_availability(const QString &username)
{
    QJsonObject data;
    data["username"] =  username;
    bool ret = false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[PrimaryKey::CMD_IDX::PRIMARY_KEY_USERNAME], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            ret = true;
        }
        else {
            DBG_INFO << response_code << response_msg;
#if 0 // NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return ret;
}

bool Draco::pkey_delete_confirmation(const QString &signed_message)
{
    QJsonObject data;
    data["signed_message"] = signed_message;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[PrimaryKey::CMD_IDX::PRIMARY_KEY_DELETE_CONFIRMTION], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->stopCheckAuthorize();
            reset();
            AppModel::instance()->startRemoveAllWallets();
            return true;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    else{
        reset();
        AppModel::instance()->stopCheckAuthorize();
        AppModel::instance()->startRemoveAllWallets();
        return true;
    }
    return false;
}

bool Draco::pkey_change_pkey(const QString &new_key, const QString &old_signed_message, const QString &new_signed_message)
{
    QJsonObject data;
    data["new_key"] = new_key;
    data["old_signed_message"] =  old_signed_message;
    data["new_signed_message"] =  new_signed_message;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[PrimaryKey::CMD_IDX::PRIMARY_KEY_CHANGE_KEY], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return false;
}

bool Draco::stayLoggedIn() const
{
    return m_stayLoggedIn;
}

void Draco::setStayLoggedIn(bool value)
{
    if(m_stayLoggedIn != value){
        m_stayLoggedIn = value;
        emit stayLoggedInChanged();
    }
}

bool Draco::getUserSubscriptions()
{
    if (AppSetting::instance()->primaryServer() == (int)nunchuk::Chain::TESTNET)
    {
        if (!getUserSubscriptionsTestnet()) {
            return getUserSubscriptionsMainnet();
        }
    } else if (AppSetting::instance()->primaryServer() == (int)nunchuk::Chain::MAIN) {
        if (!getUserSubscriptionsMainnet()) {
            return getUserSubscriptionsTestnet();
        }
    }
    return false;
}

bool Draco::getUserSubscriptionsMainnet()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Common::CMD_IDX::USER_SUBCRIPTIONS_STATUS];
    QJsonObject jsonObj = getSync(cmd, QJsonObject(), reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj["data"].toObject();
            QJsonArray subs = data["subscriptions"].toArray();
            CLIENT_INSTANCE->setSubscriptions(subs);
            return CLIENT_INSTANCE->subscriptions().size() > 0;
        }
    }
    return false;
}

bool Draco::getUserSubscriptionsTestnet()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Common::CMD_IDX::USER_SUBCRIPTIONS_STATUS_TESTNET];
    QJsonObject jsonObj = getSync(cmd, QJsonObject(), reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj["data"].toObject();
            QJsonArray subs = data["subscriptions"].toArray();
            CLIENT_INSTANCE->setSubscriptions(subs);
            return CLIENT_INSTANCE->subscriptions().size() > 0;
        }
    }
    return false;
}

bool Draco::getAssistedWallets(QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET];
    QJsonObject jsonObj = getSync(cmd, QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
#if 0 // NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return false;
}

bool Draco::assistedWalletCreateTx(const QString &wallet_id, const QString &psbt, const QString &memo)
{
    QJsonObject data;
    data["psbt"] = psbt;
    data["note"] = memo;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_CREATE_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return false;
}

bool Draco::assistedWalletUpdateTx(const QString &wallet_id, const QString &txid, const QString &memo)
{
    QJsonObject data;
    data["note"] = memo;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_UPDATE_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id).replace("{transaction_id}",txid);
    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else {
#if 0 // NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return false;
}

QJsonObject Draco::assistedWalletSignTx(const QString &wallet_id, const QString &transaction_id, const QString &psbt, const QString &memo)
{
    QJsonObject data;
    data["psbt"] = psbt;
    data["note"] = memo;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_SIGN_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id).replace("{transaction_id}",transaction_id);
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj.value("data").toObject();
            return data;
        }
        else {
#if 0 // NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return QJsonObject();
}

bool Draco::assistedWalletCancelTx(const QString &wallet_id, const QString &transaction_id)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_CANCEL_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id).replace("{transaction_id}",transaction_id);
    QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else {
#if 0 // NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return false;
}

QJsonObject Draco::assistedWalletGetTx(const QString &wallet_id, const QString &transaction_id)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id).replace("{transaction_id}",transaction_id);
    QJsonObject jsonObj = getSync(cmd, QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj.value("data").toObject();
            return data;
        }
        else {
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return QJsonObject();
}

QJsonObject Draco::assistedWalletGetListTx(const QString &wallet_id)
{
    QJsonObject data;
    data["offset"] = "0";
    data["limit"] = "100";
    data["statuses"] = "PENDING_SIGNATURES,READY_TO_BROADCAST";
    data["types"] = "STANDARD,SCHEDULED,CLAIMING,ROLLOVER,RECURRING";
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_LIST_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj.value("data").toObject();
            return data;
        }
        else {
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return QJsonObject();
}

QJsonObject Draco::assistedWalletDeleteListTx(const QString &wallet_id, const int offset, const int limit)
{
    QJsonObject data;
    data["offset"] = QString("%1").arg(offset);
    data["limit"] = QString("%1").arg(limit);
    data["statuses"] = "CANCELED";
    data["types"] = "STANDARD,SCHEDULED,CLAIMING,ROLLOVER,RECURRING";
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_DELETE_LIST_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj.value("data").toObject();
            return data;
        }
        else {
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return QJsonObject();
}

void Draco::assistedSyncTx(const QString &wallet_id, const QString &transaction_id, const QString &psbt, const QString &note)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_SYNC_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    cmd.replace("{transaction_id}",transaction_id);

    QJsonObject data;
    data["psbt"] = psbt;
    data["note"] = note;

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
        }
        else {
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
}

bool Draco::assistedRbfTx(const QString &wallet_id, const QString &transaction_id, const QString &psbt, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_RBF_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    cmd.replace("{transaction_id}",transaction_id);

    QJsonObject data;
    data["psbt"] = psbt;

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::assistedWalletGetListKey(QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["statuses"] = "PENDING";
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_LIST_KEY], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj.value("data").toObject();
            return true;
        }
        else {
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return false;
}

bool Draco::assistedWalletAddKey(const QString &request_id, const QJsonObject& data, bool &isDuplicateKey, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> paramsQuery;
    paramsQuery["request_id"] = request_id;
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::ASSISTED_WALLET_ADD_KEY], paramsQuery, {}, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else {
            errormsg = response_msg;
            QString message = errorObj["message"].toString();
            if (message.contains("Duplicate")) {
                isDuplicateKey = true;
            }
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return false;
}

bool Draco::assistedWalletRemoveId(const QString &request_id)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_REMOVE_ID];
    cmd.replace("{request_id}", request_id);
    QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return false;
}

QJsonObject Draco::assistedGetWalletConfig()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_CONFIG], {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj.value("data").toObject();
            return data;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return {};
}

bool Draco::assistedWalletGetInfo(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_INFO];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject jsonObj = getSync(cmd, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return false;
}

bool Draco::assistedWalletUpdate(const QString &wallet_id, const QString &name, const QString &description, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    if(name != ""){
        data["name"] = name;
    }
    if(description != ""){
        data["description"] = description;
    }

    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_UPDATE_NAME];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
        }
    }
    return false;
}

bool Draco::assistedKeyUpdateName(const QString &fingerPrint, const QString &name, const QJsonObject& json)
{
    QJsonObject data;
    if (json.isEmpty()) {
        data["name"] = name;
    }
    else {
        data = json;
    }
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_KEY_UPDATE_NAME];
    cmd.replace("{xfp}", fingerPrint);
    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return false;
}

bool Draco::assistedWalletGetTxNotes(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["offset"] = "0";
    data["limit"] = "10000";
    data["greater_than_time_millis"] = 0,
    data["statuses"] = "PENDING_CONFIRMATION,CONFIRMED,NETWORK_REJECTED";
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_LIST_TX_NOTES];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj.value("data").toObject();
            return true;
        }
        else {
            errormsg = response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DeleteAssistedWallet(const QString &wallet_id, const QStringList &signatures, const QString &passwordToken, const QString& secQuesToken, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject data;
    data["nonce"] = randomNonce();
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    if (!secQuesToken.isEmpty()) {
        params["Security-Question-token"] = secQuesToken;
    }
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_REMOVE_WALLET];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    QJsonObject jsonObj = deleteSync(cmd, {}, params, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DeleteWalletRequiredSignatures(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_REMOVE_WALLET_REQUIRED_SIGNATURES];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    QJsonObject jsonObj = postSync(cmd, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::verifyPasswordToken(const QString& password, const int action, QString& string_token, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject data;
    data["password"] = password;
    QString command = QString("%1/%2").arg(commands[Common::CMD_IDX::VERIFY_PASSWORD_TOKEN]).arg(target_actions_command[action]);
    QJsonObject jsonObj = postSync(command, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject token = dataObj["token"].toObject();
            DBG_INFO << dataObj;
            // token["token"] using for lockdown
            string_token = token["token"].toString();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return false;
}

QString Draco::randomNonce()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Premium::CMD_IDX::GET_RANDOM_NONCE], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject nonceObj = dataObj["nonce"].toObject();
            return nonceObj["nonce"].toString();
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return "";
}

bool Draco::secQuesGet(QJsonObject& output, QString& errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Premium::CMD_IDX::SEC_QUES_GET], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::SecQuesUpdate(const QJsonObject &request_body,
                          const QStringList &signatures,
                          const QString &passwordToken,
                          const QString &secQuesToken,
                          const QString &confirmToken,
                          bool isDraft,
                          QJsonObject &output,
                          QString &errormsg)
{
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    if (!secQuesToken.isEmpty()) {
        params["Security-Question-token"] = secQuesToken;
    }
    if (!confirmToken.isEmpty()) {
        params["Confirmation-token"] = confirmToken;
    }
    QMap<QString, QString> paramsQuery;
    paramsQuery["draft"] = isDraft ? "true" : "false";
    DBG_INFO << confirmToken << request_body;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = putSync(commands[Premium::CMD_IDX::SEC_QUES_UPDATE], paramsQuery, params, request_body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UpdateSecQuesRequiredSignatures(const QJsonObject& request_body,
                                            QJsonObject &output,
                                            QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::SEC_QUES_REQUIRED_SIGNATURES], request_body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::secQuesAnswer(const QJsonArray& answers,
                          QString& secQuesToken,
                          int &correct_answer,
                          QString& errormsg)
{
    QJsonObject data;
    data["questions_and_answers"] = answers;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::SEC_QUES_VERIFY_ANSWER], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject token = dataObj["token"].toObject();
            secQuesToken = token["token"].toString();
            correct_answer = token["correct_answers"].toInt();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::lockdownPeriods(QJsonArray& output, QString& errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Premium::CMD_IDX::LOCKDOWN_GET_PERIOD], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonArray periods = dataObj["periods"].toArray();
            output = periods;
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::lockdownRequiredSignatures(const QString &period_id,
                                       const QString &wallet_id,
                                       QJsonObject &output,
                                       QString& errormsg)
{
    QJsonObject data;
    data["period_id"] = period_id;
    data["wallet"] = wallet_id;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::LOCKDOWN_REQUIRED_SIGNATURES], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::lockdownByAnswerSecQues(const QString &passwordToken,
                                    const QString &secQuesToken,
                                    const QString &period_id,
                                    const QString &wallet_id,
                                    QString &until_time,
                                    QString &errormsg)
{
    QJsonObject body;
    body["period_id"] = period_id;
    body["wallet"] = wallet_id;
    QJsonObject data;
    data["nonce"] = randomNonce();
    data["body"] = body;

    QMap<QString, QString> params;
    params["Verify-token"] = passwordToken;
    params["Security-Question-token"] = secQuesToken;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::LOCKDOWN_SET_ANSWER_SECURITY_QUESTION], {}, params, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject resultObj = dataObj["period"].toObject();
            until_time = resultObj["display_name"].toString();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::lockdownBySignDummyTx(const QStringList &signatures,
                                  const QString &passwordToken,
                                  const QString &period_id,
                                  const QString &wallet_id,
                                  QString &until_time,
                                  QString &errormsg)
{

    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;

    QJsonObject body;
    body["period_id"] = period_id;
    body["wallet"] = wallet_id;
    QJsonObject data;
    data["nonce"] = randomNonce();
    data["body"] = body;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::LOCKDOWN_SET_SIGN_DUMMY_TX], {}, params, data, reply_code, reply_msg);
    qInfo() << jsonObj;
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject resultObj = dataObj["period"].toObject();
            until_time = resultObj["display_name"].toString();
            qInfo() << resultObj;
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UserKeysMarkAKeyAsVerified(const QString &xfpOrId,
                                       const QString &key_checksum,
                                       const QString &verification_type,
                                       QJsonObject &output,
                                       QString &errormsg)
{
    QJsonObject body;
    body["key_checksum"] = key_checksum;
    body["verification_type"] = verification_type;

    QString cmd = commands[Premium::CMD_IDX::USER_KEYS_MARK_A_KEY_AS_VERIFIED];
    cmd.replace("{key_id_or_xfp}", xfpOrId);
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, {}, {}, body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UserKeysRequestRecoveryKey(const QString &xfpOrId,
                                       const QStringList &signatures,
                                       const QString &passwordToken,
                                       const QString &secQuesToken,
                                       const QString &confirmToken,
                                       const QJsonObject &body,
                                       QJsonObject &output,
                                       QString &errormsg)
{
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    if (!secQuesToken.isEmpty()) {
        params["Security-Question-token"] = secQuesToken;
    }
    if (!confirmToken.isEmpty()) {
        params["Confirmation-token"] = confirmToken;
    }
    QString cmd = commands[Premium::CMD_IDX::USER_KEYS_REQUEST_RECOVERY_KEY];
    cmd.replace("{key_id_or_xfp}", xfpOrId);
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, {}, params, body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UserKeysRecoveryKey(const QString &xfpOrId,
                                const QStringList &signatures,
                                const QString &passwordToken,
                                const QString &secQuesToken,
                                QJsonObject &output,
                                QString &errormsg)
{
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    if (!secQuesToken.isEmpty()) {
        params["Security-Question-token"] = secQuesToken;
    }
    QString cmd = commands[Premium::CMD_IDX::USER_KEYS_RECOVERY_KEY];
    cmd.replace("{key_id_or_xfp}", xfpOrId);
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, {}, params, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UserKeysMarkRecoverStatus(const QString &xfpOrId,
                                      QJsonObject &output,
                                      QString &errormsg)
{
    QJsonObject data;
    data["status"] = "SUCCESS";
    QString cmd = commands[Premium::CMD_IDX::USER_KEYS_MARK_RECOVER_STATUS];
    cmd.replace("{key_id_or_xfp}", xfpOrId);
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, {}, {}, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UserKeysCalculateRequiredSignatures(const QString &xfpOrId,
                                                QJsonObject &output,
                                                QString &errormsg)
{
    QString cmd = commands[Premium::CMD_IDX::USER_KEYS_CALCULATE_REQUIRED_SIGNATURES];
    cmd.replace("{key_id_or_xfp}", xfpOrId);
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, {}, {}, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UserKeysGetListUserKey(const QString &passwordToken,
                                   QJsonArray &output,
                                   QString &errormsg)
{
    QMap<QString, QString> params;
    params["Verify-token"] = passwordToken;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Premium::CMD_IDX::USER_KEYS_GET_LIST_USER_KEYS], params, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toArray();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UserKeysGetUserKey(const QString &xfpOrId,
                               const QString &passwordToken,
                               QJsonObject &output,
                               QString &errormsg)
{
    QMap<QString, QString> params;
    params["Verify-token"] = passwordToken;
    QString cmd = commands[Premium::CMD_IDX::USER_KEYS_RECOVERY_KEY];
    cmd.replace("{key_id_or_xfp}", xfpOrId);
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, {}, params, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UserKeysCreateUserCustomizedQuestion(const QString &question, QJsonObject &output, QString &errormsg)
{
    QJsonObject body;
    body["question"] = question;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::SEC_QUES_CREATE], {}, {}, body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UserKeysDownloadBackup(const QString &verify_token,
                                   const QString &xfpOrId,
                                   const QJsonArray& answers,
                                   QString& output,
                                   QString &errormsg)
{
    QJsonObject data;
    data["questions_and_answers"] = answers;
    DBG_INFO << data;
    QMap<QString, QString> params;
    params["Verify-token"] = verify_token;

    QString cmd = commands[Premium::CMD_IDX::USER_KEYS_DOWNLOAD_BACKUP];
    cmd.replace("{key_id_or_xfp}", xfpOrId);

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, {}, params, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            output = dataObj["key_backup_base64"].toString();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritanceDownloadBackup(const QString &magic, const QStringList &backup_passwords, int &response_code, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["magic"] = magic;
    QJsonArray hashed_bps;
    for (QString bp : backup_passwords) {
        QString hashed_bp = qUtils::GetSHA256(qUtils::GetSHA256(bp));
        hashed_bps.push_back(hashed_bp);
    }
    data["hashed_bps"] = hashed_bps;
    QString cmd = commands[Premium::CMD_IDX::INHERITANCE_DOWNLOAD_BACKUP];
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
//            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritanceClaimRequest(const QString& magic, const QString& psbt, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["magic"] = magic;
    data["psbt"] = psbt;
    QString cmd = commands[Premium::CMD_IDX::INHERITANCE_CLAIM_REQUEST];
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            output = dataObj["transaction"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritanceClaimStatus(const QJsonObject& data, const QStringList& authos, QJsonObject &output, QString &errormsg)
{
    QMap<QString, QString> params;
    for (int i = 0; i < authos.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = authos.at(i);
    }
    QString cmd = commands[Premium::CMD_IDX::INHERITANCE_CLAIM_STATUS];
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, {}, params, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritanceCreateTx(const QJsonObject& data, const QStringList& authos, QJsonObject &output, QString &errormsg)
{
    QMap<QString, QString> params;
    for (int i = 0; i < authos.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = authos.at(i);
    }
    QString cmd = commands[Premium::CMD_IDX::INHERITANCE_CREATE_TX];
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, {}, params, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritanceCheck(const QString& magic, const QString& environment, QJsonObject& output, QString& errormsg)
{
    QJsonObject data;
    data["magic"] = magic;
    data["environment"] = environment;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::INHERITANCE_CHECK], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return false;
}

bool Draco::inheritanceGetPlan(const QString& wallet_id, const QString &group_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["wallet"] = wallet_id;
    if (group_id == wallet_id) {
        data["group_id"] = "";
    } else {
        data["group_id"] = group_id;
    }
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Premium::CMD_IDX::INHERITANCE_GET_PLAN], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg << group_id;
            if(errormsg == "on") {
                // FIXME
                // Temporary check timing show toast
                AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            }
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritancePlanCreate(const QStringList &signatures, const QJsonObject data, bool isDraft, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }

    QMap<QString, QString> paramsQuery;
//    paramsQuery["draft"] = isDraft ? "true" : "false";

    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::INHERITANCE_PLAN_CREATE], paramsQuery, params, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritancePlanUpdate(const QString &passwordToken,
                                  const QString &secQuesToken,
                                  const QStringList &signatures,
                                  const QJsonObject data,
                                  bool isDraft,
                                  QJsonObject &output,
                                  QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    params["Security-Question-token"] = secQuesToken;

    QMap<QString, QString> paramsQuery;
    paramsQuery["draft"] = isDraft ? "true" : "false";

    QJsonObject jsonObj = putSync(commands[Premium::CMD_IDX::INHERITANCE_PLAN_UPDATE], paramsQuery, params, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritancePlanCancel(const QString &passwordToken,
                                  const QString &secQuesToken,
                                  const QStringList &signatures,
                                  const QJsonObject data,
                                  bool isDraft,
                                  QJsonObject &output,
                                  QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    params["Security-Question-token"] = secQuesToken;

    QMap<QString, QString> paramsQuery;
    paramsQuery["draft"] = isDraft ? "true" : "false";

    QJsonObject jsonObj = deleteSync(commands[Premium::CMD_IDX::INHERITANCE_PLAN_CANCEL], paramsQuery, params, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritanceFakeUpdate()
{
    QJsonObject data;
    data["activation_time_milis"] = 0;
    data["buffer_period_id"] = "5MINUTES";
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = putSync(commands[Premium::CMD_IDX::INHERITANCE_FAKE_UPDATE], data, reply_code, reply_msg);
    qInfo() << jsonObj;
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            DBG_INFO << "OK";
            return true;
        }
        else {
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return false;
}

bool Draco::inheritancePlanRequiredSignatures(const QJsonObject &data, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::INHERITANCE_PLAN_REQUIRED_SIGNATURES], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return false;
}

bool Draco::inheritancePlanBufferPeriod(QJsonArray& output, QString& errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Premium::CMD_IDX::INHERITANCE_PLAN_BUFFER_PERIODS], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonArray periods = dataObj["periods"].toArray();
            output = periods;
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::InheritancePlanningRequestApprove(const QString &request_id, const QString &wallet_id, const QString &group_id, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> paramsQuery;
    paramsQuery["wallet"] = wallet_id;
    if (group_id == wallet_id) {
        paramsQuery["group_id"] = "";
    } else {
        paramsQuery["group_id"] = group_id;
    }
    QString cmd = commands[Premium::CMD_IDX::INHERITANCE_REQUEST_PLANING_APPROVE];
    cmd.replace("{request_id}", request_id);

    QJsonObject jsonObj = putSync(cmd, paramsQuery, {}, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::InheritancePlanningRequestDeny(const QString &request_id, const QString &wallet_id, const QString &group_id, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> paramsQuery;
    paramsQuery["wallet"] = wallet_id;
    if (group_id == wallet_id) {
        paramsQuery["group_id"] = "";
    } else {
        paramsQuery["group_id"] = group_id;
    }
    QString cmd = commands[Premium::CMD_IDX::INHERITANCE_REQUEST_PLANING_DENY];
    cmd.replace("{request_id}", request_id);

    QJsonObject jsonObj = putSync(cmd, paramsQuery, {}, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::ServerKeysGet(const QString &id_or_xfp, const QString &derivation_path, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::SERVER_KEYS_GET];
    cmd.replace("{key_id_or_xfp}", id_or_xfp);
    QJsonObject data;
    data["derivation_path"] = derivation_path;

    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj["data"].toObject();
            output = data["key"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::ServerKeysUpdate(const QString& passwordToken,
                             const QString& secQuesToken,
                             const QString& id_or_xfp,
                             const QString& derivation_path,
                             const QStringList& signatures,
                             const QJsonObject& request_body,
                             QJsonObject &output,
                             QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    params["Security-Question-token"] = secQuesToken;

    QMap<QString, QString> paramsQuery;
    paramsQuery["derivation_path"] = derivation_path;

    QString cmd = commands[Premium::CMD_IDX::SERVER_KEYS_UPDATE];
    cmd.replace("{key_id_or_xfp}", id_or_xfp);

    QJsonObject jsonObj = putSync(cmd, paramsQuery, params, request_body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::ServerKeysRequiredSignature(const QString &key_id_or_xfp,
                                        const QString &derivation_path,
                                        const QJsonObject& request_body,
                                        QJsonObject& output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::SERVER_KEYS_REQUIRED_SIGNATURES];
    cmd.replace("{key_id_or_xfp}", key_id_or_xfp);
    QMap<QString, QString> paramsQuery;
    paramsQuery["derivation_path"] = derivation_path;
    QJsonObject jsonObj = postSync(cmd, paramsQuery, {}, request_body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::RequestConfirmationCode(const QString &action,
                                        const QJsonObject &requestBody,
                                        QJsonObject &output,
                                        QString &errormsg)
{


    QMap<QString, QString> paramsQuery;
    paramsQuery["action"] = action ;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::CONFIRMATION_CODE], paramsQuery, {}, requestBody, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::VerifyConfirmationCode(const QString &code_id,
                                   const QString &code,
                                   QJsonObject &output,
                                   QString &errormsg)
{
    QJsonObject data;
    data["code"] = code;
    QString cmd = commands[Premium::CMD_IDX::VERIFY_CONFIRMATION_CODE];
    cmd.replace("{code_id}", code_id);
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetAlerts(const QString wallet_id, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    const int size = 20;
    auto getAlerts = [&] (int index, QJsonArray &alert) -> bool {
        QJsonObject data;
        data["offset"] = QString("%1").arg(index); // Adjust offset
        data["limit"] = QString("%1").arg(size);  // Adjust limit

        int     reply_code = -1;
        QString reply_msg  = "";
        QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_ALERTS];
        cmd.replace("{wallet_id_or_local_id}", wallet_id);
        QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                QJsonObject ret = jsonObj["data"].toObject();
                alert = ret["alerts"].toArray();
                return true;
            }
            else{
                errormsg = response_msg;
                DBG_INFO << response_code << response_msg;
                return false;
            }
        }
        errormsg = reply_msg;
        return false;
    };
    QJsonArray alerts;
    int index = 0;
    while (true) {
        QJsonArray alert {};
        bool ret = getAlerts(index, alert);
        if (ret == false) {
            return false;
        }
        if (alert.isEmpty()) {
            break;
        }
        for (const auto &item : alert) {
            alerts.append(item);
        }
        if (alert.size() < size) {
            break;
        }
        index += size;
    }
    output["alerts"] = alerts;
    return true;
}

bool Draco::GetAlertsCount(const QString wallet_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_ALERTS_COUNT];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DismissAlert(const QString wallet_id, const QString alert_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_ALERTS_DISMISS];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{alert_id}", alert_id);

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::MarkAlertAsRead(const QString wallet_id, const QString alert_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_ALERTS_READ];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{alert_id}", alert_id);

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::HealthCheckForKey(const QString &wallet_id, const QString &xfp, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_HEALTHCHECK];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{xfp}", xfp);

    data["nonce"] = randomNonce();
    data["body"] = QJsonObject();

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetWalletHealthStatus(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_HEALTH];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetDummyTransaction(const QString &wallet_id, const QString &txid, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_DUMMY_TX];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{dummy_transaction_id}", txid);

    QJsonObject data;
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UpdateDummyTransaction(const QString &wallet_id, const QStringList &authorizations, const QString &txid, QJsonObject &output, QString &errormsg)
{
    QMap<QString, QString> params;
    for (int i = 0; i < authorizations.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = authorizations.at(i);
    }
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_UPDATE_DUMMY_TX];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{dummy_transaction_id}", txid);

    QJsonObject data;
    QJsonObject jsonObj = putSync(cmd, {}, params, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::CancelDummyTransaction(const QString &wallet_id, const QString &txid, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_DUMMY_TX];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{dummy_transaction_id}", txid);

    QJsonObject data;
    QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::FinalizeDummyTransaction(const QString &wallet_id, const QString &txid, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_FINALIZE_DUMMY_TX];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{dummy_transaction_id}", txid);

    QJsonObject data;
    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetHistorySignerList(const QString &xfp, QJsonObject& output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_HEALTH_CHECK_HISTORY_GET];
    cmd.replace("{xfp}", xfp);

    QJsonObject jsonObj = getSync(cmd, {}, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetCountryCodeList(QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::COUNTRY_CODE_LIST];

    QJsonObject jsonObj = getSync(cmd, {}, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            return true;
        }
        response_msg = reply_msg;
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::RequestOnboardingNoAdvisor(const QString &country_code, const QString &email, const QString &note, QString &errormsg)
{
    QJsonObject data;
    data["country_code"] = country_code;
    data["email"] = email;
    data["note"] = note;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::ONBOARDING_NO_ADVISOR], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK) {

            return true;
        }
        response_msg = reply_msg;
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetElectrumServers(QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Common::CMD_IDX::GET_ELECTRUM_SERVERS];

    QJsonObject jsonObj = getSync(cmd, {}, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            return true;
        }
        response_msg = reply_msg;
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::ChangeEmail(const QJsonObject &request_body,
                        const QStringList &signatures,
                        const QString &passwordToken,
                        const QString &secQuesToken,
                        const QString &confirmToken,
                        bool isDraft,
                        QJsonObject &output,
                        QString &errormsg)
{
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    if (!secQuesToken.isEmpty()) {
        params["Security-Question-token"] = secQuesToken;
    }
    if (!confirmToken.isEmpty()) {
        params["Confirmation-token"] = confirmToken;
    }
    QMap<QString, QString> paramsQuery;
    paramsQuery["draft"] = isDraft ? "true" : "false";
    DBG_INFO << confirmToken << request_body;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::CHANGING_EMAIL], paramsQuery, params, request_body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::CalculateRequireSignaturesForChangingEmail(const QString &new_email, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["new_email"] = new_email;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::CALCULATE_REQUIRED_SIGNATURES_FOR_CHANGING_EMAIL], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}


bool Draco::GetKeyHealthReminder(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_KEY_HEALTH_REMINDER];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::AddOrUpdateKeyHealthReminder(const QString &wallet_id, const QJsonObject &request_body, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_ADD_OR_UPDATE_KEY_HEALTH_REMINDER];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject jsonObj = putSync(cmd, request_body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DeleteKeyHealthReminder(const QString &wallet_id, const QStringList &xfps, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_DELETE_KEY_HEALTH_REMINDER];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QMap<QString, QString> paramsQuery;
    for (int i = 0; i < xfps.count(); i++) {
        paramsQuery.insertMulti("xfps", xfps.at(i));
    }
    QJsonObject jsonObj = deleteSync(cmd, paramsQuery, {}, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::SkipKeyHealthReminder(const QString &wallet_id, const QString &xfp, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_SKIP_KEY_HEALTH_REMINDER];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{xfp}", xfp);
    QJsonObject jsonObj = deleteSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::ConfigureWalletReplacement(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_CONFIGURE_WALLET_REPLACEMENT];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject jsonObj = putSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::InitiateKeyReplacement(const QString &wallet_id, const QString &xfp, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_INITIATE_KEY_REPLACEMENT];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{xfp}", xfp);
    QJsonObject jsonObj = postSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::CancelKeyReplacement(const QString &wallet_id, const QString &xfp, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_CANCEL_KEY_REPLACEMENT];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{xfp}", xfp);
    QJsonObject jsonObj = deleteSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::ReplaceKey(const QString &wallet_id, const QString &xfp, const QString& passwordToken, const QJsonObject& request_body, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> params;
    params["Verify-token"] = passwordToken;
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_REPLACE_KEY];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{xfp}", xfp);
    QJsonObject jsonObj = postSync(cmd, {} , params, request_body, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::FinalizeKeyReplacement(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_FINALIZE_KEY_REPLACEMENT];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject jsonObj = postSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetKeyReplacementStatus(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_STATUS_KEY_REPLACEMENT];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject jsonObj = getSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            return true;
        }
        response_msg = reply_msg;
        errormsg = reply_msg;
    }
    return false;
}

bool Draco::ResetKeyReplacement(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (wallet_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_RESET_KEY_REPLACEMENT];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject jsonObj = deleteSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if (response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            return true;
        }
        errormsg = response_msg;
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetSavedAddress(QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_GET_SAVED_ADDRESSES];

    QJsonObject criteria;
    criteria["offset"]  = 0;
    criteria["limit"]   = 0;

    QJsonObject data;
    data["criteria"] = criteria;
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::AddOrUpdateSavedAddress(const QString &label, const QString &address, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_ADD_SAVED_ADDRESS];

    QJsonObject data;
    data["label"]   = label;
    data["address"] = address;

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            DBG_INFO << output;
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << errormsg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DeleteSavedAddress(const QString &label, const QString &address, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_DELETE_SAVED_ADDRESS];
    cmd.replace("{address}", address);
    QJsonObject jsonObj = deleteSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if (response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            DBG_INFO << output;
            return true;
        }
        errormsg = response_msg;
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::SignDummyTransactionToSignIn(const QString &dummy_transaction_id, const QStringList& signatures, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";

    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }

    QString cmd = commands[Premium::CMD_IDX::SIGN_DUMMY_TRANSACTION_TO_SIGN_IN];
    cmd.replace("{dummy_transaction_id}", dummy_transaction_id);

    QJsonObject jsonObj = putSync(cmd, {}, params, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::SignInUsingXPUBorWallet(const QString &bsms, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["data"] = bsms;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Premium::CMD_IDX::SIGN_IN_USING_XPUB_WALLET], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit createAccountResult(reply_code, response_code, response_msg);
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetCoinControl(const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_COINCONTROL_GET];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject data;
    QJsonObject jsonObj = getSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            DBG_INFO << output;
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << errormsg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::UpdateCoinControl(const QString &wallet_id, const QString &data, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_COINCONTROL_UPDATE];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    // Post with data
    QJsonObject body;
    body["data"] = data;
    QJsonObject jsonObj = postSync(cmd, body, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();

        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            DBG_INFO << output;
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << errormsg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::isSubscribed() const
{
    return m_isSubscribed;
}

void Draco::setIsSubscribed(bool newIsSubscribed)
{
    if (m_isSubscribed == newIsSubscribed)
        return;
    m_isSubscribed = newIsSubscribed;
    emit isSubscribedChanged();
}

bool Draco::DraftWalletMarkAnAlertAsRead(const QString alert_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_MARK_ALERT_AS_READ];
    cmd.replace("{alert_id}", alert_id);

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletDismissAnAlert(const QString alert_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_DISMISS_ALERT];
    cmd.replace("{alert_id}", alert_id);

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletGetAlerts(QJsonObject &output, QString &errormsg)
{
    const int size = 20;
    auto getAlerts = [&] (int index, QJsonArray &alert) -> bool {
        QJsonObject data;
        data["offset"] = QString("%1").arg(index); // Adjust offset
        data["limit"] = QString("%1").arg(size);  // Adjust limit

        int     reply_code = -1;
        QString reply_msg  = "";
        QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_GET_ALERTS];
        QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                QJsonObject ret = jsonObj["data"].toObject();
                alert = ret["alerts"].toArray();
                return true;
            }
            else{
                errormsg = response_msg;
                DBG_INFO << response_code << response_msg;
                return false;
            }
        }
        errormsg = reply_msg;
        return false;
    };
    QJsonArray alerts;
    int index = 0;
    while (true) {
        QJsonArray alert {};
        bool ret = getAlerts(index, alert);
        if (ret == false) {
            return false;
        }
        if (alert.isEmpty()) {
            break;
        }
        for (const auto &item : alert) {
            alerts.append(item);
        }
        if (alert.size() < size) {
            break;
        }
        index += size;
    }
    output["alerts"] = alerts;
    return true;
}

bool Draco::DraftWalletGetAlertsCount(QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_GET_TOTAL_ALERTS];
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletSetServerKey(const QString &server_key_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["server_key_id"] = server_key_id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_SET_SERVER_KEY];
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletRequestAddKey(QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    QJsonArray tags;
    tags.append("INHERITANCE");
    data["tags"] = tags;
    data["key_index"] = 0;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_GET_REQUEST_ADD_KEY];
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletPushRequestAddKey(const QString &request_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    QJsonArray tags;
    tags.append("INHERITANCE");
    data["tags"] = tags;
    data["key_index"] = 0;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_PUSH_REQUEST_ADD_KEY];
    cmd.replace("{request_id}", request_id);
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletInit(int m, int n, bool required_server_key, bool allow_inheritance, QJsonObject &output, QString &errormsg)
{
    QJsonObject wallet_config;
    wallet_config["m"] = m;
    wallet_config["n"] = n;
    wallet_config["required_server_key"] = required_server_key;
    wallet_config["allow_inheritance"] = allow_inheritance;
    QJsonObject data;
    data["wallet_config"] = wallet_config;

    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_INIT];
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletGetStatusOfARequestAddKey(const QString &request_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_GET_REQUEST_ADD_KEY];
    cmd.replace("{request_id}", request_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletGetCurrent(QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_GET_CURRENT];
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
#if 0
            // AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletResetCurrent(QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_RESET_CURRENT];
    QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::ReplacementDownloadBackupFile(const QString& wallet_id, const QString& xfp, const QString& passwordToken, QJsonObject& output, QString& errormsg)
{
    QMap<QString, QString> params;
    params["Verify-token"] = passwordToken;

    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_DOWNLOAD_BACKUP_KEY_REPLACEMENT];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{xfp}", xfp);

    QJsonObject jsonObj = postSync(cmd,{}, params, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::ReplacementUploadBackupFile(const QString& wallet_id, const QString& passwordToken, const QMap<QString, QVariant>& requestBody, QJsonObject& output, QString& errormsg)
{
    QMap<QString, QString> params;
    params["Verify-token"] = passwordToken;

    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::ASSISTED_WALLET_UPLOAD_BACKUP_KEY_REPLACEMENT];
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    QJsonObject jsonObj = postMultiPartSync(cmd,{}, params, requestBody, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            DBG_INFO << jsonObj;
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletDownloadBackupFile(const QString& xfp, QJsonObject& output, QString& errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_DOWNLOAD_BACKUP];
    cmd.replace("{xfp}", xfp);

    QJsonObject jsonObj = postSync(cmd, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::DraftWalletUploadBackupFile(const QMap<QString, QVariant>& requestBody, QJsonObject& output, QString& errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Premium::CMD_IDX::DRAFT_WALLET_UPLOAD_BACKUP];

    QJsonObject jsonObj = postMultiPartSync(cmd, requestBody, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::GetTaprootSupportedSigners(QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Common::CMD_IDX::TAPROOT_SUPPORTED_SIGNERS];
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

QSet<int> Draco::GetTaprootSupportedCached(bool reset)
{
    if(reset && m_taproot_support_types.size() > 0){
        m_taproot_support_types.clear();
    }
    if(m_taproot_support_types.count() == 0){
        QJsonObject data;
        QString     error_msg = "";
        bool get_result = Draco::instance()->GetTaprootSupportedSigners(data, error_msg);
        if(get_result){
            DBG_INFO << data;
            QJsonArray supported_signers = data["supported_signers"].toArray();
            foreach (const QJsonValue &value, supported_signers) {
                QJsonObject obj = value.toObject();
                QString type = obj["signer_type"].toString();
                nunchuk::SignerType type_enum = qUtils::GetSignerType(type);
                m_taproot_support_types.insert((int)type_enum);
            }
        }
    }
    return m_taproot_support_types;
}

