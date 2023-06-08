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

#define REST_API_TIME_OUT 5000

inline QString commandByNetwork(const QString& cmd){
    QString command = cmd;
    if ((int)AppSetting::Chain::TESTNET == AppSetting::instance()->primaryServer() && command.contains(DRAGON_USER_WALLETS_URL)) {
        command.replace(DRAGON_USER_WALLETS_URL, DRAGON_USER_WALLETS_TESTNET_URL);
    }
    return command;
}

Draco* Draco::m_instance = NULL;
Draco::Draco() :
    m_uid(""), m_pid(""),
    m_emailRequested(""),
    m_expireSec(-1),
    m_dracoToken(""),
    m_ChatId(""),
    m_loginHalfToken(""),
    m_deviceId(""),
    m_stayLoggedIn(false)
{
    m_machineUniqueId = QSysInfo::machineUniqueId();
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

void Draco::refreshContacts()
{
    CLIENT_INSTANCE->syncContacts(getContacts());
    CLIENT_INSTANCE->syncContactsSent(getContactsSent());
    CLIENT_INSTANCE->syncContactsReceived(getContactsReceived());
    CLIENT_INSTANCE->syncDevices(getDevices());
}

bool Draco::getCurrencies(QJsonObject &output, QString &errormsg)
{
    int reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_CURRENCIES], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        output = jsonObj;
        return true;
    }
    errormsg = reply_msg;
    return false;
}

void Draco::btcRates()
{
    std::unique_ptr<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QUrl url = QUrl::fromUserInput("https://api.nunchuk.io/v1/prices");
    QNetworkRequest requester_(url);
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->get(requester_));
    QEventLoop eventLoop;
    QObject::connect(reply.get(), SIGNAL(finished()), &eventLoop, SLOT(quit()));
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
}

void Draco::exchangeRates(const QString &currency)
{
    std::unique_ptr<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QUrl url = QUrl::fromUserInput("https://api.nunchuk.io/v1.1/forex/rates");
    QNetworkRequest requester_(url);
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
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
    reply.release()->deleteLater();
}

void Draco::feeRates()
{
    std::unique_ptr<QNetworkAccessManager> manager(new QNetworkAccessManager);
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
    reply.release()->deleteLater();
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::VERIFY_NEW_DEVICE], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK) {
            QJsonObject dataObj = jsonObj["data"].toObject();
            this->setDeviceId(dataObj["deviceId"].toString());
            this->setDracoToken(dataObj["tokenId"].toString());
            this->setExpireSec(dataObj["expireInSeconds"].toInt());
            this->getMe();
            this->getCurrentUserSubscription();
            CLIENT_INSTANCE->setIsNewDevice(true);
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::RESEND_VERIFY_NEW_DEVICE_CODE], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit resendVerifyNewDeviceCodeResult(reply_code, response_code, response_msg);
    }
}

void Draco::inviteFriends(const QStringList& emails)
{
    if(emails.count() > 0){
        QJsonObject data;
        data["friendEmails"] = QJsonArray::fromStringList(emails);
        int     reply_code = -1;
        QString reply_msg  = "";
        QJsonObject jsonObj = postSync(commands[CMD_IDX::INVITE_FRIEND], data, reply_code, reply_msg);
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            emit resendVerifyNewDeviceCodeResult(reply_code, response_code, response_msg);
        }
    }
}

void Draco::removeContact(const QString &contact_id)
{
    QJsonObject data;
    data["contact_id"] = contact_id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::REMOVE_FRIEND], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            refreshContacts();
        }
    }
}

void Draco::cancelFriendRequest(const QString &contact_id)
{
    QJsonObject data;
    data["contact_id"] = contact_id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::CANCEL_REQUEST_FRIEND], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
    }
}

QJsonObject Draco::postSync(const QString &cmd, QJsonObject data, int& reply_code, QString &reply_msg)
{
    QJsonObject ret;
    qApp->setOverrideCursor(Qt::WaitCursor);
    std::unique_ptr<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QNetworkRequest requester_(QUrl::fromUserInput(commandByNetwork(cmd)));
    QString headerData = QString("Bearer %1").arg(this->dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    manager->setCookieJar(new QNetworkCookieJar(manager.get()));
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->post(requester_, QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code,
                                        STR_CPP_111,
                                        EWARNING::WarningType::EXCEPTION_MSG,
                                        STR_CPP_112);
    }
    reply.release()->deleteLater();
    qApp->restoreOverrideCursor();
    return ret;
}

QJsonObject Draco::postSync(const QString &cmd, QMap<QString, QString> params, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QJsonObject ret;
    qApp->setOverrideCursor(Qt::WaitCursor);
    std::unique_ptr<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QNetworkRequest requester_(QUrl::fromUserInput(commandByNetwork(cmd)));
    QString headerData = QString("Bearer %1").arg(this->dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    // Add addional params
    for(QString param : params.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(params.value(param).toStdString()));
    }
    manager->setCookieJar(new QNetworkCookieJar(manager.get()));
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->post(requester_, QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code,
                                        STR_CPP_111,
                                        EWARNING::WarningType::EXCEPTION_MSG,
                                        STR_CPP_112);
    }
    reply.release()->deleteLater();
    qApp->restoreOverrideCursor();
    return ret;
}

QJsonObject Draco::getSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QJsonObject ret;
    std::unique_ptr<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QUrl url = QUrl::fromUserInput(commandByNetwork(cmd));
    if(!data.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, data.keys()) {
            QJsonValue value = data.value(key);
            params.addQueryItem(key, value.toString());
        }
        url.setQuery(params);
    }
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(this->dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    manager->setCookieJar(new QNetworkCookieJar(manager.get()));
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->get(requester_));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code,
                                        STR_CPP_111,
                                        EWARNING::WarningType::EXCEPTION_MSG,
                                        STR_CPP_112);
    }
    reply.release()->deleteLater();
    return ret;
}

QJsonObject Draco::putSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QJsonObject ret;
    qApp->setOverrideCursor(Qt::WaitCursor);
    std::unique_ptr<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QNetworkRequest requester_(QUrl::fromUserInput(commandByNetwork(cmd)));
    QString headerData = QString("Bearer %1").arg(this->dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    manager->setCookieJar(new QNetworkCookieJar(manager.get()));
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->put(requester_, QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code,
                                        STR_CPP_111,
                                        EWARNING::WarningType::EXCEPTION_MSG,
                                        STR_CPP_112);
    }
    reply.release()->deleteLater();
    qApp->restoreOverrideCursor();
    return ret;
}

QJsonObject Draco::putSync(const QString &cmd, QMap<QString, QString> params, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QJsonObject ret;
    qApp->setOverrideCursor(Qt::WaitCursor);
    std::unique_ptr<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QUrl url = QUrl::fromUserInput(commandByNetwork(cmd));
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(this->dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    // Add addional params
    for(QString param : params.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(params.value(param).toStdString()));
    }
    manager->setCookieJar(new QNetworkCookieJar(manager.get()));
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->put(requester_, QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code,
                                        STR_CPP_111,
                                        EWARNING::WarningType::EXCEPTION_MSG,
                                        STR_CPP_112);
    }
    reply.release()->deleteLater();
    qApp->restoreOverrideCursor();
    return ret;
}

QJsonObject Draco::deleteSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QJsonObject ret;
    qApp->setOverrideCursor(Qt::WaitCursor);
    std::unique_ptr<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QNetworkRequest requester_(QUrl::fromUserInput(commandByNetwork(cmd)));
    QString headerData = QString("Bearer %1").arg(this->dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    manager->setCookieJar(new QNetworkCookieJar(manager.get()));
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->sendCustomRequest(requester_, "DELETE", QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code,
                                        STR_CPP_111,
                                        EWARNING::WarningType::EXCEPTION_MSG,
                                        STR_CPP_112);
    }
    reply.release()->deleteLater();
    qApp->restoreOverrideCursor();
    return ret;
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

QByteArray Draco::machineUniqueId() const
{
    return m_machineUniqueId;
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
}

QString Draco::dracoToken() const
{
    return m_dracoToken;
}

void Draco::setDracoToken(const QString &value)
{
    if(m_dracoToken != value){
        m_dracoToken = value;
        emit dracoTokenChanged();
    }
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::CREATE_ACCOUNT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit createAccountResult(reply_code, response_code, response_msg);
        if(response_code == DRACO_CODE::RESPONSE_OK){
            this->setUid(email);
            QJsonObject dataObj = jsonObj["data"].toObject();
            this->setDeviceId(dataObj["deviceId"].toString());
            this->setDracoToken(dataObj["tokenId"].toString());
            this->setExpireSec(dataObj["expireInSeconds"].toInt());
            this->setEmailRequested(email);
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::SIGNIN], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        this->setUid(email);
        this->setPid(password);
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            this->setDeviceId(dataObj["deviceId"].toString());
            this->setDracoToken(dataObj["tokenId"].toString());
            this->setExpireSec(dataObj["expireInSeconds"].toInt());
            this->setEmailRequested(email);
            this->getMe();
            this->getCurrentUserSubscription();
        }
        else if(response_code == DRACO_CODE::LOGIN_NEW_DEVICE){
            QJsonObject detailsObj = errorObj["details"].toObject();
            this->setDeviceId(detailsObj["deviceId"].toString());
            this->setLoginHalfToken(detailsObj["halfToken"].toString());
            this->setEmailRequested(email);
        }
        else{
            AppModel::instance()->showToast(response_code,
                                            response_msg,
                                            EWARNING::WarningType::EXCEPTION_MSG,
                                            STR_CPP_112);
        }
        emit singinResult(reply_code, response_code, response_msg);
    }
}

bool Draco::signout()
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[CMD_IDX::SIGNOUT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        this->reset();
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::RECOVER_PASSWORD], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit recoverPasswordResult(reply_code, response_code, response_msg);
    }
}

void Draco::forgotPassword(const QString& email)
{
    QJsonObject data;
    data["email"] = email;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[CMD_IDX::FORGOT_PASSWORD], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit forgotPasswordResult(reply_code, response_code, response_msg);
        if(response_code == DRACO_CODE::RESPONSE_OK){
            this->setEmailRequested(email);
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::CHANGE_PASSWORD], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            this->setPid(newpassword);
            this->getMe();
            this->getCurrentUserSubscription();
        }
        emit changePasswordResult(reply_code, response_code, response_msg);
    }
}

void Draco::getMe()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    DracoUser user;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_ME], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject userObj = dataObj["user"].toObject();
            this->setChatId(userObj["chat_id"].toString());
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
            AppModel::instance()->startCheckAuthorize();
            CLIENT_INSTANCE->setIsNunchukLoggedIn(true);
        }
        else if(response_code == DRACO_CODE::UNAUTHORIZED){
            CLIENT_INSTANCE->requestSignout();
            AppModel::instance()->stopCheckAuthorize();
        }
        else{
            DBG_INFO << response_code;
        }
    }
    CLIENT_INSTANCE->setMe(user);
}

void Draco::getMepKey(const QString &public_address)
{
    QString cmd = QString("%1%2").arg(commands[CMD_IDX::PRIMARY_KEY_PUBLIC_ADDRESS], public_address);
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
            this->setChatId(userObj["chat_id"].toString());
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
            CLIENT_INSTANCE->setMe(user);
            AppModel::instance()->startCheckAuthorize();
        }
        else if(response_code == DRACO_CODE::UNAUTHORIZED){
            CLIENT_INSTANCE->requestSignout();
            AppModel::instance()->stopCheckAuthorize();
        }
        else{
            DBG_INFO << response_code;
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
    QJsonObject jsonObj = getSync(commands[CMD_IDX::SEARCH_FRIEND], data, reply_code, reply_msg);
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
    QJsonObject jsonObj = putSync(commands[CMD_IDX::REQUEST_FRIEND], data, reply_code, reply_msg);
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
        QJsonObject jsonObj = postSync(commands[CMD_IDX::REQUEST_FRIEND], data, reply_code, reply_msg);
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                ret = true;
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
        QJsonObject jsonObj = putSync(commands[CMD_IDX::REQUEST_FRIEND], data, reply_code, reply_msg);
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                QJsonObject dataObj = jsonObj["data"].toObject();
                failed_emails = dataObj["failed_emails"].toVariant().toStringList();
                return true;
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
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_FRIENDS_LIST], data, reply_code, reply_msg);
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
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_FRIENDS_SENT], data, reply_code, reply_msg);
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
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_FRIENDS_RECEIVED], data, reply_code, reply_msg);
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
    }
    return contacts;
}

QList<DracoUser> Draco::getRoomMembers(const QString &id)
{
    QList<DracoUser> members;
    members.clear();
    QString cmd = QString("%1/%2/members").arg(commands[CMD_IDX::GET_ROOM_MEMBERS], id);
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
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_DEVICES_LIST], data, reply_code, reply_msg);
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
    }
    return mDevices;
}

void Draco::accecptFriendRequest(const QString &id)
{
    QJsonObject data;
    data["contact_id"] = id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[CMD_IDX::ACCEPT_FRIEND_REQUEST], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
        }
    }
}

void Draco::ignoreFriendRequest(const QString &id)
{
    QJsonObject data;
    data["contact_id"] = id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::IGNORE_FRIEND_REQUEST], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){

        }
    }
}

bool Draco::deleteCurrentUser()
{
    bool ret = false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::DELETE_ACCOUNT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            ret = true;
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::DELETE_CONFIRM], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->stopCheckAuthorize();
            this->reset();
            AppModel::instance()->startRemoveAllWallets();
            ret = true;
        }
    }
    else{
        if(reply_code == DRACO_CODE::SUCCESSFULL){
            AppModel::instance()->stopCheckAuthorize();
            this->reset();
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
    data["name"] = name;
    data["gender"] = user.gender;
    data["status_text"] = user.status_text;
    data["avatar_url"] = avartaPath;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[CMD_IDX::USER_PROFILE], data, reply_code, reply_msg);
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
    QJsonObject jsonObj = getSync(commands[CMD_IDX::CHECK_FOR_UPDATE], QJsonObject(), reply_code, reply_msg);
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
            emit this->startCheckForUpdate(result,title,message,doItLaterCTALbl);
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
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::DEVICE_SIGNOUT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit loggedInDeviceChanged(reply_code,response_code,response_msg);
    }
}

void Draco::signoutAllDeices()
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::DEVICE_ALL_SIGNOUT], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit loggedInDeviceChanged(reply_code,response_code,response_msg);
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::DEVICE_MARK_COMPROMISED], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        emit loggedInDeviceChanged(reply_code,response_code,response_msg);
    }
}

void Draco::pkey_signup(const QString &address, const QString &username, const QString &signature)
{
    QJsonObject data;
    data["address"] = address;
    data["username"] =  username;
    data["signature"] =  signature;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_SIGNUP], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(reply_code == DRACO_CODE::SUCCESSFULL) {
            this->setUid(username);
            if(response_code == DRACO_CODE::RESPONSE_OK){
                QJsonObject dataObj = jsonObj["data"].toObject();
                this->setDeviceId(dataObj["deviceId"].toString());
                this->setDracoToken(dataObj["tokenId"].toString());
                this->setExpireSec(dataObj["expireInSeconds"].toInt());
                this->getMe();
                this->getCurrentUserSubscription();
                AppModel::instance()->setPrimaryKey(username);
            }
        }
        emit signalpkey_signup(reply_code, response_code, response_msg);
    }
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_SIGNIN], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(reply_code == DRACO_CODE::SUCCESSFULL) {
            this->setUid(username);
            if(response_code == DRACO_CODE::RESPONSE_OK){
                ret = true;
                QJsonObject dataObj = jsonObj["data"].toObject();
                this->setDeviceId(dataObj["deviceId"].toString());
                this->setDracoToken(dataObj["tokenId"].toString());
                this->setExpireSec(dataObj["expireInSeconds"].toInt());
                this->getMe();
                this->getCurrentUserSubscription();
                AppModel::instance()->setPrimaryKey(username);
            }
            else if(response_code == DRACO_CODE::LOGIN_NEW_DEVICE){
                ret = true;
                QJsonObject detailsObj = errorObj["details"].toObject();
                this->setDeviceId(detailsObj["deviceId"].toString());
                this->setLoginHalfToken(detailsObj["halfToken"].toString());
            }
            else{
                AppModel::instance()->showToast(response_code,
                                                response_msg,
                                                EWARNING::WarningType::EXCEPTION_MSG,
                                                STR_CPP_112);
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
    QJsonObject jsonObj = getSync(commands[CMD_IDX::PRIMARY_KEY_NONCE], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            ret = dataObj["nonce"].toString();
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
        QString cmd = QString("%1%2").arg(commands[CMD_IDX::PRIMARY_KEY_NONCE], "?type=change_pkey");
        jsonObj = postSync(cmd, data, reply_code, reply_msg);
    }else{
        jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_NONCE], data, reply_code, reply_msg);
    }
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            ret = dataObj["nonce"].toString();
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
    QJsonObject jsonObj = getSync(commands[CMD_IDX::PRIMARY_KEY_USERNAME], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            ret = true;
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_DELETE_CONFIRMTION], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->stopCheckAuthorize();
            this->reset();
            AppModel::instance()->startRemoveAllWallets();
            return true;
        }
    }
    else{
        this->reset();
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_CHANGE_KEY], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
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

void Draco::getCurrentUserSubscription()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::USER_SUBCRIPTIONS_CURRENT];
    QJsonObject jsonObj = getSync(cmd, QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            CLIENT_INSTANCE->setSubCur(dataObj);
            ServiceSetting::instance()->setIsSubscriber(true);
            return;
        }
    }
    ServiceSetting::instance()->setIsSubscriber(false);
}

QJsonObject Draco::getAssistedWallets()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::ASSISTED_WALLET_GET];
    QJsonObject jsonObj = getSync(cmd, QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            return dataObj;
        }
    }
    return QJsonObject();
}

bool Draco::assistedWalletCreateTx(const QString &wallet_id, const QString &psbt, const QString &memo)
{
    QJsonObject data;
    data["psbt"] = psbt;
    data["note"] = memo;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::ASSISTED_WALLET_CREATE_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
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
    QString cmd = commands[CMD_IDX::ASSISTED_WALLET_UPDATE_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id).replace("{transaction_id}",txid);
    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
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
    QString cmd = commands[CMD_IDX::ASSISTED_WALLET_SIGN_TX];
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
    }
    return QJsonObject();
}

bool Draco::assistedWalletCancelTx(const QString &wallet_id, const QString &transaction_id)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::ASSISTED_WALLET_CANCEL_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id).replace("{transaction_id}",transaction_id);
    QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
    }
    return false;
}

QJsonObject Draco::assistedWalletGetTx(const QString &wallet_id, const QString &transaction_id)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::ASSISTED_WALLET_GET_TX];
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
    }
    return QJsonObject();
}

QJsonObject Draco::assistedWalletGetListTx(const QString &wallet_id)
{
    QJsonObject data;
    data["offset"] = "0";
    data["limit"] = "100";
    data["statuses"] = "PENDING_SIGNATURES,READY_TO_BROADCAST";
    data["types"] = "STANDARD,SCHEDULED,CLAIMING,ROLLOVER";
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::ASSISTED_WALLET_GET_LIST_TX];
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
    }
    return QJsonObject();
}

QJsonObject Draco::assistedWalletDeleteListTx(const QString &wallet_id, const int offset, const int limit)
{
    QJsonObject data;
    data["offset"] = QString("%1").arg(offset);
    data["limit"] = QString("%1").arg(limit);
    data["statuses"] = "CANCELED";
    data["types"] = "STANDARD,SCHEDULED,CLAIMING,ROLLOVER";
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::ASSISTED_WALLET_DELETE_LIST_TX];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj.value("data").toObject();
            return data;
        }
    }
    return QJsonObject();
}

void Draco::assistedSyncTx(const QString &wallet_id, const QString &transaction_id, const QString &psbt)
{
    QJsonObject data;
    data["psbt"] = psbt;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::TX_SYNC];
    cmd.replace("{wallet_id_or_local_id}",wallet_id).replace("{transaction_id}",transaction_id);
    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
        }
    }
}

bool Draco::verifyPasswordToken(const QString& password, const int action, QString& errormsg_or_token)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject data;
    data["password"] = password;
    QString command = QString("%1/%2").arg(commands[CMD_IDX::VERIFY_PASSWORD_TOKEN]).arg(target_actions_command[action]);
    QJsonObject jsonObj = postSync(command, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject token = dataObj["token"].toObject();

            // token["token"] using for lockdown
            errormsg_or_token = token["token"].toString();
            return true;
        }
        else{
            errormsg_or_token = response_msg;
            return false;
        }
    }
    errormsg_or_token = reply_msg;
    return false;
}

QString Draco::randomNonce()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_RANDOM_NONCE], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject nonceObj = dataObj["nonce"].toObject();
            return nonceObj["nonce"].toString();
        }
    }
    return "";
}

bool Draco::secQuesGet(QJsonArray& output, QString& errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[CMD_IDX::SEC_QUES_GET], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonArray questions = dataObj["questions"].toArray();
            for (auto q : questions) {
                auto q_js = q.toObject();
                bool is_answered = q_js["is_answered"].toBool();
                if (is_answered) {
                    output.append(q);
                }
            }
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

bool Draco::secQuesAnswer(QList<SecurityQuestion> answers,
                          QString& secQuesToken,
                          int &correct_answer,
                          QString& errormsg)
{
    QJsonArray questions;
    for (auto v : answers) {
        QJsonObject question;
        question["question_id"] = v.id;
        question["answer"] =  v.answer;
        questions.append(question);
    }

    QJsonObject data;
    data["questions_and_answers"] = questions;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[CMD_IDX::SEC_QUES_VERIFY_ANSWER], data, reply_code, reply_msg);
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
            DBG_INFO << response_code;
            errormsg = response_msg;
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
    QJsonObject jsonObj = getSync(commands[CMD_IDX::LOCKDOWN_GET_PERIOD], QJsonObject(), reply_code, reply_msg);
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
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::lockdownRequiredSignatures(const QString &period_id,
                                       const QString &wallet_id,
                                       ReqiredSignaturesInfo& output,
                                       QString& errormsg)
{
    QJsonObject data;
    data["period_id"] = period_id;
    data["wallet"] = wallet_id;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[CMD_IDX::LOCKDOWN_REQUIRED_SIGNATURES], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject resultObj = dataObj["result"].toObject();
            output.type = required_signatures_type[resultObj["type"].toString()];
            output.required_signatures = resultObj["required_signatures"].toInt();
            output.required_answers = resultObj["required_answers"].toInt();
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::LOCKDOWN_SET], params, data, reply_code, reply_msg);
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
    QJsonObject jsonObj = postSync(commands[CMD_IDX::LOCKDOWN_SET], params, data, reply_code, reply_msg);
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
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::userKeysDownloadBackup(const QString &verify_token,
                                   const QString &xfpOrId,
                                   const QList<SecurityQuestion> answers,
                                   QString& output,
                                   QString &errormsg)
{
    QJsonArray questions;
    for (auto v : answers) {
        QJsonObject question;
        question["question_id"] = v.id;
        question["answer"] =  v.answer;
        questions.append(question);
    }

    QJsonObject data;
    data["questions_and_answers"] = questions;

    QMap<QString, QString> params;
    params["Verify-token"] = verify_token;

    QString cmd = commands[CMD_IDX::USER_KEYS_DOWNLOAD_BACKUP];
    cmd.replace("{key_id_or_xfp}", xfpOrId);

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, params, data, reply_code, reply_msg);
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
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritanceDownloadBackup(const QString &magic, int &response_code, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["magic"] = magic;
    QString cmd = commands[CMD_IDX::INHERITANCE_DOWNLOAD_BACKUP];
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
    QString cmd = commands[CMD_IDX::INHERITANCE_CLAIM_REQUEST];
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
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::inheritanceClaimStatus(const QJsonObject& data, const QString& autho, QJsonObject &output, QString &errormsg)
{
    QMap<QString, QString> params;
    params[QString("AuthorizationX-1")] = autho;
    QString cmd = commands[CMD_IDX::INHERITANCE_CLAIM_STATUS];
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, params, data, reply_code, reply_msg);
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

bool Draco::inheritanceCreateTx(const QJsonObject& data, const QString& autho, QJsonObject &output, QString &errormsg)
{
    QMap<QString, QString> params;
    params[QString("AuthorizationX-1")] = autho;
    QString cmd = commands[CMD_IDX::INHERITANCE_CREATE_TX];
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(cmd, params, data, reply_code, reply_msg);
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

bool Draco::inheritanceCheck(const QString& magic, const QString& environment, QJsonObject& output, QString& errormsg)
{
    QJsonObject data;
    data["magic"] = magic;
    data["environment"] = environment;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[CMD_IDX::INHERITANCE_CHECK], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
        }
    }
    return false;
}

bool Draco::inheritanceGetPlan(const QString& wallet_id, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["wallet"] = wallet_id;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[CMD_IDX::INHERITANCE_GET_PLAN], data, reply_code, reply_msg);
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

bool Draco::inheritanceFakeUpdate()
{
    QJsonObject data;
    data["activation_time_milis"] = 0;
    data["buffer_period_id"] = "5MINUTES";
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = putSync(commands[CMD_IDX::INHERITANCE_FAKE_UPDATE], data, reply_code, reply_msg);
    qInfo() << jsonObj;
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            DBG_INFO << "OK";
            return true;
        }
    }
    DBG_INFO << reply_msg;
    return false;
}

bool Draco::serverKeysGet(const QString &id_or_xfp, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::SERVER_KEYS_GET];
    cmd.replace("{key_id_or_xfp}", id_or_xfp);

    QJsonObject jsonObj = getSync(cmd, QJsonObject(), reply_code, reply_msg);
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
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Draco::serverKeysUpdate(const QString& passwordToken, const QString& secQuesToken, const QString &id_or_xfp, const QStringList &signatures, const QJsonObject data, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    params["Security-Question-token"] = secQuesToken;

    QString cmd = commands[CMD_IDX::SERVER_KEYS_UPDATE];
    cmd.replace("{key_id_or_xfp}", id_or_xfp);

    QJsonObject jsonObj = putSync(cmd, params, data, reply_code, reply_msg);
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

bool Draco::serverKeysRequiredSignatures(const QString &id_or_xfp,
                                         const QJsonObject data,
                                         ReqiredSignaturesInfo &output,
                                         QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[CMD_IDX::SERVER_KEYS_REQUIRED_SIGNATURES];
    cmd.replace("{key_id_or_xfp}", id_or_xfp);

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject resultObj = dataObj["result"].toObject();
            output.type = required_signatures_type[resultObj["type"].toString()];
            output.required_signatures = resultObj["required_signatures"].toInt();
            output.required_answers = resultObj["required_answers"].toInt();
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


