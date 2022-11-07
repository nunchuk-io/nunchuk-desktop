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

Draco* Draco::m_instance = NULL;
Draco::Draco() :
    m_dracoManager(new QNetworkAccessManager()),
    m_exchangesMng(new QNetworkAccessManager()),
    m_feeRateMng(new QNetworkAccessManager()),
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
    DBG_INFO << QString(m_machineUniqueId);
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Draco::~Draco()
{
    m_dracoManager->deleteLater();
    m_exchangesMng->deleteLater();
    m_feeRateMng->deleteLater();
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

void Draco::exchangeRates()
{
#if 0
    if(exchangesMng_){
        QUrl url = QUrl::fromUserInput("https://api.coinbase.com/v2/exchange-rates");
        QUrlQuery params;
        params.addQueryItem("currency", "BTC");
        url.setQuery(params);
        QNetworkRequest requester_(url);
        requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        connectSingleShot(exchangesMng_, &QNetworkAccessManager::finished, this, [](QNetworkReply *reply){
            if(reply){
                QByteArray response_data = reply->readAll();
                DBG_INFO << "RATES:" << QString(response_data);
                QJsonDocument json = QJsonDocument::fromJson(response_data);
                QJsonObject jsonObj = json.object();
                QJsonObject rates = jsonObj["data"].toObject()["rates"].toObject();
                double rates_double  = rates["USD"].toString().toDouble();
                QString rates_string = rates["USD"].toString();
                DBG_INFO << "Exchange rates String is: " << rates_string;
                AppModel::instance()->setExchangeRates(rates_double);
                reply->deleteLater();
            }
        });
        exchangesMng_->get(requester_);
    }
#endif
    if(m_exchangesMng){
        QUrl url = QUrl::fromUserInput("https://api.nunchuk.io/v1/prices");
        QNetworkRequest requester_(url);
        requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QSharedPointer<QNetworkReply> reply = QSharedPointer<QNetworkReply>(m_exchangesMng->get(requester_), &QObject::deleteLater);
        QEventLoop eventLoop;
        QObject::connect(reply.data(), SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        if(reply){
            QByteArray response_data = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(response_data);
            QJsonObject jsonObj = json.object();
            QJsonObject data = jsonObj["data"].toObject();
            QJsonObject prices = data["prices"].toObject();
            QJsonObject btc = prices["BTC"].toObject();
            double rates_double  = btc["USD"].toDouble();
            AppModel::instance()->setExchangeRates(rates_double);
        }
    }
}

void Draco::feeRates()
{
    if(m_feeRateMng){
        QUrl url = QUrl::fromUserInput("https://api.nunchuk.io/v1.1/fees/recommended");
        QNetworkRequest requester_(url);
        requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QSharedPointer<QNetworkReply> reply = QSharedPointer<QNetworkReply>(m_feeRateMng->get(requester_), &QObject::deleteLater);
        QEventLoop eventLoop;
        QObject::connect(reply.data(), SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        if(reply){
            QByteArray response_data = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(response_data);
            QJsonObject jsonObj = json.object();
            AppModel::instance()->setFastestFee(jsonObj["fastestFee"].toInt());
            AppModel::instance()->setHalfHourFee(jsonObj["halfHourFee"].toInt());
            AppModel::instance()->setHourFee(jsonObj["hourFee"].toInt());
            AppModel::instance()->setMinFee(jsonObj["minimumFee"].toInt());
            AppModel::instance()->setLasttimeCheckEstimatedFee(QDateTime::currentDateTime());
        }
    }
}

void Draco::verifyNewDevice(const QString &pin)
{
    QJsonObject data;
    data["email"] = Uid();
    data["loginHalfToken"] = loginHalfToken();
    data["pin"] = pin;
    data["deviceId"] = deviceId();
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::VERIFY_NEW_DEVICE], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::VERIFY_NEW_DEVICE " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK) {
            QJsonObject dataObj = jsonObj["data"].toObject();
            this->setDeviceId(dataObj["deviceId"].toString());
            this->setDracoToken(dataObj["tokenId"].toString());
            this->setExpireSec(dataObj["expireInSeconds"].toInt());
            this->getMe();
            CLIENT_INSTANCE->setIsNewDevice(true);
        }
        emit verifyNewDeviceResult(https_code, error_code, error_msg);
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::resendVerifyNewDeviceCode()
{
    QJsonObject data;
    data["email"] = Uid();
    data["deviceId"] = deviceId();
    data["loginHalfToken"] = loginHalfToken();
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::RESEND_VERIFY_NEW_DEVICE_CODE], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        emit resendVerifyNewDeviceCodeResult(https_code, error_code, error_msg);
        DBG_INFO << "Draco::RESEND_VERIFY_NEW_DEVICE_CODE " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::inviteFriends(const QStringList& emails)
{
    if(emails.count() > 0){
        QJsonObject data;
        data["friendEmails"] = QJsonArray::fromStringList(emails);
        int https_code = -1;
        QJsonObject jsonObj = postSync(commands[CMD_IDX::INVITE_FRIEND], data, https_code);
        if(!jsonObj.isEmpty()){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int error_code = errorObj["code"].toInt();
            QString error_msg = errorObj["message"].toString();
            emit resendVerifyNewDeviceCodeResult(https_code, error_code, error_msg);
            DBG_INFO << "Draco::INVITE_FRIEND " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        }
        else{
            DBG_INFO << "ERROR, RETURN IS EMPTY";
        }
    }
}

void Draco::removeContact(const QString &contact_id)
{
    DBG_INFO << contact_id;
    QJsonObject data;
    data["contact_id"] = contact_id;
    int https_code = -1;
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::REMOVE_FRIEND], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::REMOVE_FRIEND" << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            refreshContacts();
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::cancelFriendRequest(const QString &contact_id)
{
    QJsonObject data;
    data["contact_id"] = contact_id;
    int https_code = -1;
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::CANCEL_REQUEST_FRIEND], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::CANCEL_REQUEST_FRIEND"<< contact_id << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

QJsonObject Draco::postSync(const QString &cmd, QJsonObject data, int& https_code)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(m_dracoManager){
        QNetworkRequest requester_(QUrl::fromUserInput(cmd));
        QString headerData = QString("Bearer %1").arg(this->dracoToken());
        requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
        requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        requester_.setRawHeader("Connection", "keep-alive");
        requester_.setRawHeader("x-nc-device-id", machineUniqueId());
        requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
        requester_.setRawHeader("x-nc-device-class", "Desktop");
        requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
        m_dracoManager->setCookieJar(new QNetworkCookieJar(m_dracoManager));
        QSharedPointer<QNetworkReply> reply = QSharedPointer<QNetworkReply>(m_dracoManager->post(requester_, QJsonDocument(data).toJson()),&QObject::deleteLater);
        QEventLoop eventLoop;
        QObject::connect(reply.data(), SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        if(reply){
            https_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QByteArray response_data = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(response_data);
            QJsonObject jsonObj = json.object();
            reply->close();
            qApp->restoreOverrideCursor();
            return jsonObj;
        }
        https_code = -1;
    }
    qApp->restoreOverrideCursor();
    return QJsonObject();
}

QJsonObject Draco::getSync(const QString &cmd, QJsonObject data, int &https_code)
{
    if(m_dracoManager){
        QUrl url = QUrl::fromUserInput(cmd);
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
        m_dracoManager->setCookieJar(new QNetworkCookieJar(m_dracoManager));
        QSharedPointer<QNetworkReply> reply = QSharedPointer<QNetworkReply>(m_dracoManager->get(requester_), &QObject::deleteLater);
        QTimer timer;
        timer.setSingleShot(true);
        QEventLoop eventLoop;
        connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
        QObject::connect(reply.data(), SIGNAL(finished()), &eventLoop, SLOT(quit()));
        timer.start(5000);
        eventLoop.exec();
        if(timer.isActive() && !reply.isNull()){
            timer.stop();
            if(reply->error() > 0) {
                // handle error
            }
            else {
                https_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                QByteArray response_data = reply->readAll();
                QJsonDocument json = QJsonDocument::fromJson(response_data);
                QJsonObject jsonObj = json.object();
                reply->close();
                return jsonObj;
            }
        }
        else {
           // timeout
           disconnect(reply.data(), SIGNAL(finished()), &eventLoop, SLOT(quit()));
           reply->abort();
        }
        https_code = -1;
    }
    return QJsonObject();
}

QJsonObject Draco::putSync(const QString &cmd, QJsonObject data, int &https_code)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(m_dracoManager){
        QNetworkRequest requester_(QUrl::fromUserInput(cmd));
        QString headerData = QString("Bearer %1").arg(this->dracoToken());
        requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
        requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        requester_.setRawHeader("Connection", "keep-alive");
        requester_.setRawHeader("x-nc-device-id", machineUniqueId());
        requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
        requester_.setRawHeader("x-nc-device-class", "Desktop");
        requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
        m_dracoManager->setCookieJar(new QNetworkCookieJar(m_dracoManager));
        QSharedPointer<QNetworkReply> reply = QSharedPointer<QNetworkReply>(m_dracoManager->put(requester_, QJsonDocument(data).toJson()),&QObject::deleteLater);
        QEventLoop eventLoop;
        QObject::connect(reply.data(), SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        if(reply){
            https_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QByteArray response_data = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(response_data);
            QJsonObject jsonObj = json.object();
            reply->close();
            qApp->restoreOverrideCursor();
            return jsonObj;
        }
        https_code = -1;
    }
    qApp->restoreOverrideCursor();
    return QJsonObject();
}

QJsonObject Draco::deleteSync(const QString &cmd, QJsonObject data, int &https_code)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(m_dracoManager){
        QNetworkRequest requester_(QUrl::fromUserInput(cmd));
        QString headerData = QString("Bearer %1").arg(this->dracoToken());
        requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
        requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        requester_.setRawHeader("Connection", "keep-alive");
        requester_.setRawHeader("x-nc-device-id", machineUniqueId());
        requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
        requester_.setRawHeader("x-nc-device-class", "Desktop");
        requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
        m_dracoManager->setCookieJar(new QNetworkCookieJar(m_dracoManager));
        QSharedPointer<QNetworkReply> reply = QSharedPointer<QNetworkReply>(m_dracoManager->sendCustomRequest(requester_, "DELETE", QJsonDocument(data).toJson()),&QObject::deleteLater);
        QEventLoop eventLoop;
        QObject::connect(reply.data(), SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        if(reply){
            https_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QByteArray response_data = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(response_data);
            QJsonObject jsonObj = json.object();
            reply->close();
            qApp->restoreOverrideCursor();
            return jsonObj;
        }
        https_code = -1;
    }
    qApp->restoreOverrideCursor();
    return QJsonObject();
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

    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::CREATE_ACCOUNT], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        emit createAccountResult(https_code, error_code, error_msg);
        DBG_INFO << "Draco::CREATE_ACCOUNT " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            this->setUid(email);
            QJsonObject dataObj = jsonObj["data"].toObject();
            this->setDeviceId(dataObj["deviceId"].toString());
            this->setDracoToken(dataObj["tokenId"].toString());
            this->setExpireSec(dataObj["expireInSeconds"].toInt());
            this->setEmailRequested(email);
            AppSetting::instance()->updateIsStarted(true);
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::singin(const QString& email, const QString& password)
{
    QJsonObject data;
    data["email"] = email;
    data["password"] = password;
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::SIGNIN], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::SIGNIN " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL) {
            this->setUid(email);
            this->setPid(password);
            if(error_code == DRACO_CODE::RESPONSE_OK){
                QJsonObject dataObj = jsonObj["data"].toObject();
                this->setDeviceId(dataObj["deviceId"].toString());
                this->setDracoToken(dataObj["tokenId"].toString());
                this->setExpireSec(dataObj["expireInSeconds"].toInt());
                this->setEmailRequested(email);
                this->getMe();
            }
            else if(error_code == DRACO_CODE::LOGIN_NEW_DEVICE){
                QJsonObject detailsObj = errorObj["details"].toObject();
                this->setDeviceId(detailsObj["deviceId"].toString());
                this->setLoginHalfToken(detailsObj["halfToken"].toString());
                this->setEmailRequested(email);
            }
            else{}
            AppSetting::instance()->updateIsStarted(true);
        }
        emit singinResult(https_code, error_code, error_msg);
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

bool Draco::signout()
{
    QJsonObject data;
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::SIGNOUT], data, https_code);
    if(https_code == DRACO_CODE::SUCCESSFULL){
        DBG_INFO << "SIGNOUT SUCCESSFULL";
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

    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::RECOVER_PASSWORD], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::RECOVER_PASSWORD " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        emit recoverPasswordResult(https_code, error_code, error_msg);
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::forgotPassword(const QString& email)
{
    QJsonObject data;
    data["email"] = email;
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::FORGOT_PASSWORD], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        emit forgotPasswordResult(https_code, error_code, error_msg);
        DBG_INFO << "Draco::FORGOT_PASSWORD " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            this->setEmailRequested(email);
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::changePassword(const QString& oldpassword, const QString& newpassword)
{
    QJsonObject data;
    data["oldPassword"] = oldpassword;
    data["newPassword"] = newpassword;

    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::CHANGE_PASSWORD], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            this->setPid(newpassword);
            this->getMe();
        }
        DBG_INFO << "Draco::CHANGE_PASSWORD " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        emit changePasswordResult(https_code, error_code, error_msg);
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::getMe()
{
    int https_code = -1;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_ME], QJsonObject(), https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::GET_ME" << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
        else if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::UNAUTHORIZED){
            CLIENT_INSTANCE->requestSignout();
            AppModel::instance()->stopCheckAuthorize();
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::getMepKey(const QString &public_address)
{
    QString cmd = QString("%1%2").arg(commands[CMD_IDX::PRIMARY_KEY_PUBLIC_ADDRESS], public_address);
    int https_code = -1;
    QJsonObject jsonObj = getSync(cmd, QJsonObject(), https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::PRIMARY_KEY_PUBLIC_ADDRESS" << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
        else if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::UNAUTHORIZED){
            CLIENT_INSTANCE->requestSignout();
            AppModel::instance()->stopCheckAuthorize();
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

DracoUser Draco::search(const QString &email)
{
    DracoUser user;
    QJsonObject data;
    data["email"] = email;
    int https_code = -1;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::SEARCH_FRIEND], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::SEARCH_FRIEND " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
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
    data["emails"] = QJsonArray::fromStringList(inputs);;
    int https_code = -1;
    QJsonObject jsonObj = putSync(commands[CMD_IDX::REQUEST_FRIEND], data, https_code);
    if(jsonObj.isEmpty()){
        DBG_INFO << "ERROR, RETURN IS EMPTY";
        return QVariant::fromValue(ret);
    }
    else{
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::REQUEST_FRIEND " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
        int https_code = -1;
        QJsonObject jsonObj = postSync(commands[CMD_IDX::REQUEST_FRIEND], data, https_code);
        if(!jsonObj.isEmpty()){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int error_code = errorObj["code"].toInt();
            QString error_msg = errorObj["message"].toString();
            DBG_INFO << "Draco::REQUEST_FRIEND " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
            if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
                ret = true;
            }
        }
        else{
            DBG_INFO << "ERROR, RETURN IS EMPTY";
        }
    }
    else{
        DBG_INFO << "DOESN'T EXIST USER";
    }
    return ret;
}

bool Draco::requestMutipleFriend(const QStringList &email, QStringList &failed_emails)
{
    if(email.count() > 0){
        QJsonObject data;
        data["emails"] = QJsonArray::fromStringList(email);;
        int https_code = -1;
        QJsonObject jsonObj = putSync(commands[CMD_IDX::REQUEST_FRIEND], data, https_code);
        if(!jsonObj.isEmpty()){
            QJsonObject errorObj = jsonObj["error"].toObject();
            int error_code = errorObj["code"].toInt();
            QString error_msg = errorObj["message"].toString();
            DBG_INFO << "Draco::REQUEST_FRIEND " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
            if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
                QJsonObject dataObj = jsonObj["data"].toObject();
                failed_emails = dataObj["failed_emails"].toVariant().toStringList();
                return true;
            }
        }
        else{
            DBG_INFO << "ERROR, RETURN IS EMPTY";
        }
    }
    else{
        DBG_INFO << "LIST REQUEST IS EMPTY";
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
    int https_code = -1;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_FRIENDS_LIST], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::GET_FRIENDS_LIST " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
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
    int https_code = -1;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_FRIENDS_SENT], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::GET_FRIENDS_SENT " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
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
    int https_code = -1;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_FRIENDS_RECEIVED], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::GET_FRIENDS_RECEIVED " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
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
    int https_code = -1;
    QJsonObject jsonObj = getSync(cmd, data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::GET_ROOM_MEMBERS " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
    return members;
}

QList<DracoDevice> Draco::getDevices()
{
    QList<DracoDevice> mDevices;
    mDevices.clear();
    QJsonObject data;
    int https_code = -1;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::GET_DEVICES_LIST], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::GET_DEVICES_LIST " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
    return mDevices;
}

void Draco::accecptFriendRequest(const QString &id)
{
    QJsonObject data;
    data["contact_id"] = id;
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::ACCEPT_FRIEND_REQUEST], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::ACCEPT_FRIEND_REQUEST " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::ignoreFriendRequest(const QString &id)
{
    QJsonObject data;
    data["contact_id"] = id;
    int https_code = -1;
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::IGNORE_FRIEND_REQUEST], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::IGNORE_FRIEND_REQUEST " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){

        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

bool Draco::deleteCurrentUser()
{
    bool ret = false;
    QJsonObject data;
    int https_code = -1;
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::DELETE_ACCOUNT], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::DELETE_ACCOUNT " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            ret = true;
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
    return ret;
}

bool Draco::sendDeleteConfirmation(const QString &code)
{
    bool ret = false;
    QJsonObject data;
    data["confirmation_code"] = code;
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::DELETE_CONFIRM], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::DELETE_CONFIRM " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->stopCheckAuthorize();
            this->reset();
            AppModel::instance()->startRemoveAllWallets();
            ret = true;
        }
    }
    else{
        if(https_code == DRACO_CODE::SUCCESSFULL){
            AppModel::instance()->stopCheckAuthorize();
            this->reset();
            AppModel::instance()->startRemoveAllWallets();
            ret = true;
        }else{
            DBG_INFO << "ERROR, RETURN IS EMPTY";
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
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::USER_PROFILE], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::USER_PROFILE " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        emit updateProfileResult(https_code,error_code,error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK) {
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
    else{
        DBG_INFO << "ERROR, IS EMPTY";
    }
}

void Draco::checkForUpdate()
{
    QString title = "";
    QString message = "";
    QString doItLaterCTALbl = "";
    int result = 0;
    int https_code = -1;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::CHECK_FOR_UPDATE], QJsonObject(), https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
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
    else{
        DBG_INFO << "ERROR, CHECK FOR UPDATE";
    }
}

void Draco::signoutDeice(const QString &device_id)
{
    QJsonObject data;
    QJsonArray array;
    array.push_back(device_id);
    data["devices"] = array;
    int https_code = -1;
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::DEVICE_SIGNOUT], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::DEVICE_SIGNOUT " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        emit loggedInDeviceChanged(https_code,error_code,error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            DBG_INFO << "SEND signout SUCCESSFULL";
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::signoutAllDeices()
{
    QJsonObject data;
    int https_code = -1;
    QJsonObject jsonObj = deleteSync(commands[CMD_IDX::DEVICE_ALL_SIGNOUT], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::DEVICE_ALL_SIGNOUT " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        emit loggedInDeviceChanged(https_code,error_code,error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            DBG_INFO << "SEND signoutAllDeices SUCCESSFULL";
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

void Draco::markAsCompromised(const QString &device_id)
{
    QJsonObject data;
    QJsonArray array;
    array.push_back(device_id);
    data["devices"] = array;
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::DEVICE_MARK_COMPROMISED], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::DEVICE_MARK_COMPROMISED " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        emit loggedInDeviceChanged(https_code,error_code,error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK) {
            DBG_INFO << "SEND markAsCompromised SUCCESSFULL";
        }
    }
    else{
        DBG_INFO << "ERROR, IS EMPTY";
    }
}

void Draco::pkey_signup(const QString &address, const QString &username, const QString &signature)
{
    QJsonObject data;
    data["address"] = address;
    data["username"] =  username;
    data["signature"] =  signature;
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_SIGNUP], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::PRIMARY_KEY_SIGNUP " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL) {
            this->setUid(username);
            if(error_code == DRACO_CODE::RESPONSE_OK){
                QJsonObject dataObj = jsonObj["data"].toObject();
                this->setDeviceId(dataObj["deviceId"].toString());
                this->setDracoToken(dataObj["tokenId"].toString());
                this->setExpireSec(dataObj["expireInSeconds"].toInt());
            }
            this->getMe();
            AppModel::instance()->setPrimaryKey(username);
            AppSetting::instance()->updateIsStarted(true);
        }
        emit signalpkey_signup(https_code, error_code, error_msg);
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }      
}

void Draco::pkey_signin(const QString &address, const QString &username, const QString &signature)
{
    QJsonObject data;
    if(address != ""){
        data["address"] = address;
    }
    if(username != ""){
        data["username"] =  username;
    }
    data["signature"] =  signature;
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_SIGNIN], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::PRIMARY_KEY_SIGNIN " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL) {
            this->setUid(username);
            if(error_code == DRACO_CODE::RESPONSE_OK){
                QJsonObject dataObj = jsonObj["data"].toObject();
                this->setDeviceId(dataObj["deviceId"].toString());
                this->setDracoToken(dataObj["tokenId"].toString());
                this->setExpireSec(dataObj["expireInSeconds"].toInt());
            }
            else if(error_code == DRACO_CODE::LOGIN_NEW_DEVICE){
                QJsonObject detailsObj = errorObj["details"].toObject();
                this->setDeviceId(detailsObj["deviceId"].toString());
                this->setLoginHalfToken(detailsObj["halfToken"].toString());
            }
            else{}
            this->getMe();
            AppModel::instance()->setPrimaryKey(username);
            AppSetting::instance()->updateIsStarted(true);
        }
        emit signalpkey_signin(https_code, error_code, error_msg);
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
}

QString Draco::get_pkey_nonce(const QString &address,const QString &username)
{
    QJsonObject data;
    data["address"] = address;
    data["username"] =  username;
    QString ret = "";
    int https_code = -1;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::PRIMARY_KEY_NONCE], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::PRIMARY_KEY_NONCE" << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            ret = dataObj["nonce"].toString();
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
    return ret;
}

QString Draco::pkey_manual_nonce(const QString &address, const QString &username, const QString &nonce, const QString &type)
{
    QString ret = "";
    int https_code = -1;
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
        jsonObj = postSync(cmd, data, https_code);
    }else{
        jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_NONCE], data, https_code);
    }
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::PRIMARY_KEY_NONCE " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            ret = dataObj["nonce"].toString();
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
    return ret;
}

bool Draco::pkey_username_availability(const QString &username)
{
    QJsonObject data;
    data["username"] =  username;
    bool ret = false;
    int https_code = -1;
    QJsonObject jsonObj = getSync(commands[CMD_IDX::PRIMARY_KEY_USERNAME], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::PRIMARY_KEY_USERNAME" << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            ret = true;
        }
    }
    else{
        DBG_INFO << "ERROR, RETURN IS EMPTY";
    }
    return ret;
}

bool Draco::pkey_delete_confirmation(const QString &signed_message)
{
    QJsonObject data;
    data["signed_message"] = signed_message;
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_DELETE_CONFIRMTION], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::PRIMARY_KEY_DELETE_CONFIRMTION " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            AppModel::instance()->stopCheckAuthorize();
            this->reset();
            AppModel::instance()->startRemoveAllWallets();
            return true;
        }
    }
    else{
        AppModel::instance()->stopCheckAuthorize();
        this->reset();
        AppModel::instance()->startRemoveAllWallets();
        DBG_INFO << "ERROR, RETURN IS EMPTY";
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
    int https_code = -1;
    QJsonObject jsonObj = postSync(commands[CMD_IDX::PRIMARY_KEY_CHANGE_KEY], data, https_code);
    if(!jsonObj.isEmpty()){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int error_code = errorObj["code"].toInt();
        QString error_msg = errorObj["message"].toString();
        DBG_INFO << "Draco::PRIMARY_KEY_CHANGE_KEY " << QString("https[%1] error[%2] message[%3]").arg(https_code).arg(error_code).arg(error_msg);
        if(https_code == DRACO_CODE::SUCCESSFULL && error_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
    }
    else{
        if(https_code == DRACO_CODE::SUCCESSFULL){
            DBG_INFO << "ERROR, RETURN IS EMPTY";
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
