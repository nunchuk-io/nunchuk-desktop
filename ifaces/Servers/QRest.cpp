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

#include "QRest.h"
#include "AppModel.h"
#include "QOutlog.h"
#include <QHttpPart>
#include <QHttpMultiPart>

#define DRAGON_URL            "https://api.nunchuk.io/v1.1"
#define DRAGON_TESTNET_URL    "https://api-testnet.nunchuk.io/v1.1"

QString    QRest::m_dracoToken      = "";
QByteArray QRest::m_machineUniqueId = QSysInfo::machineUniqueId();

QRest::QRest()
{

}

QRest::~QRest()
{

}

QString QRest::dracoToken()
{
    return m_dracoToken;
}

void QRest::setDracoToken(const QString token)
{
    m_dracoToken = token;
}

QByteArray QRest::machineUniqueId()
{
    return m_machineUniqueId;
}

QString QRest::url() const
{
    if ((int)AppSetting::Chain::TESTNET == AppSetting::instance()->primaryServer()) {
        return DRAGON_TESTNET_URL;
    } else {
        return DRAGON_URL;
    }
}

OurSharedPointer<QNetworkAccessManager> QRest::networkManager()
{
    if (!m_networkManager.hasLocalData()) {
        OurSharedPointer<QNetworkAccessManager> ptr(new QNetworkAccessManager);
        m_networkManager.setLocalData(ptr);
    }
    return m_networkManager.localData();
}

QJsonObject QRest::postSync(const QString &cmd, QJsonObject data, int& reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("POST %1").arg(command));
    QJsonObject ret;
    QNetworkRequest requester_(QUrl::fromUserInput(command));
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->post(requester_, QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}

QJsonObject QRest::postSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("POST %1").arg(command));
    QJsonObject ret;
    QUrl url = QUrl::fromUserInput(command);
    if(!paramsQuery.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, paramsQuery.keys()) {
            QString value           = paramsQuery.value(key);
            QString encodedValue    = QUrl::toPercentEncoding(value);
            params.addQueryItem(key, encodedValue);
        }
        url.setQuery(params);
    }
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    for(QString param : paramsHeader.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(paramsHeader.value(param).toStdString()));
    }
    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->post(requester_, QJsonDocument(data).toJson()));

    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}

QJsonObject QRest::postMultiPartSync(const QString &cmd, QMap<QString, QVariant> data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("POST %1").arg(command));
    QJsonObject ret;
    QNetworkRequest requester_(QUrl::fromUserInput(command));
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    for (auto key : data.keys()) {
        if (key == "file") {
            QString filePath = data[key].toString();
            QFile *file = new QFile(filePath);
            if (!file->open(QIODevice::ReadOnly)) {
                DBG_INFO << "Failed to open file:" << filePath;
                delete file;
                continue;
            }

            QHttpPart filePart;
            filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                               QVariant("form-data; name=\"file\"; filename=\"" + file->fileName() + "\""));
            filePart.setBodyDevice(file);
            file->setParent(multiPart);

            multiPart->append(filePart);
        } else {
            QHttpPart textPart;
            textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + key + "\""));
            textPart.setBody(data[key].toByteArray());
            multiPart->append(textPart);
        }
    }

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->post(requester_, multiPart));

    multiPart->setParent(reply.get());
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}

QJsonObject QRest::postMultiPartSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QMap<QString, QVariant> data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("POST %1").arg(command));
    QJsonObject ret;
    QUrl url = QUrl::fromUserInput(command);
    if(!paramsQuery.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, paramsQuery.keys()) {
            QString value           = paramsQuery.value(key);
            QString encodedValue    = QUrl::toPercentEncoding(value);
            params.addQueryItem(key, encodedValue);
        }
        url.setQuery(params);
    }
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    for(QString param : paramsHeader.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(paramsHeader.value(param).toStdString()));
    }
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    for (auto key : data.keys()) {
        if (key == "file") {
            QString filePath = data[key].toString();
            QFile *file = new QFile(filePath);
            if (!file->open(QIODevice::ReadOnly)) {
                DBG_INFO << "Failed to open file:" << filePath;
                delete file;
                continue;
            }

            QHttpPart filePart;
            filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                               QVariant("form-data; name=\"file\"; filename=\"" + file->fileName() + "\""));
            filePart.setBodyDevice(file);
            file->setParent(multiPart);

            multiPart->append(filePart);
        } else {
            QHttpPart textPart;
            textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + key + "\""));
            textPart.setBody(data[key].toByteArray());
            multiPart->append(textPart);
        }
    }

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->post(requester_, multiPart));

    multiPart->setParent(reply.get());
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}

QJsonObject QRest::getSync(const QString &cmd, QJsonObject paramsQuery, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("GET %1").arg(command));
    QJsonObject ret;
    QUrl url = QUrl::fromUserInput(command);
    if(!paramsQuery.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, paramsQuery.keys()) {
            QJsonValue value        = paramsQuery.value(key);
            QString encodedValue    = QUrl::toPercentEncoding(value.toString());
            params.addQueryItem(key, encodedValue);
        }
        url.setQuery(params);
    }
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->get(requester_));

    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    DBG_INFO << requester_.url().toString();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}

QJsonObject QRest::getSync(const QString &cmd, QMap<QString, QString> paramsHeader, QJsonObject paramsQuery, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("GET %1").arg(command));
    QJsonObject ret;
    QUrl url = QUrl::fromUserInput(command);
    if(!paramsQuery.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, paramsQuery.keys()) {
            QJsonValue value        = paramsQuery.value(key);
            QString encodedValue    = QUrl::toPercentEncoding(value.toString());
            params.addQueryItem(key, encodedValue);
        }
        url.setQuery(params);
    }
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    for(QString param : paramsHeader.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(paramsHeader.value(param).toStdString()));
    }


    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->get(requester_));

    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    DBG_INFO << requester_.url().toString();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}

QJsonObject QRest::putSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("PUT %1").arg(command));
    QJsonObject ret;
    QNetworkRequest requester_(QUrl::fromUserInput(command));
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->put(requester_, QJsonDocument(data).toJson()));

    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    DBG_INFO << QString(reply->readAll());
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}

QJsonObject QRest::putSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QJsonObject ret;
    QUrl url = QUrl::fromUserInput(command);
    if(!paramsQuery.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, paramsQuery.keys()) {
            QString value           = paramsQuery.value(key);
            QString encodedValue    = QUrl::toPercentEncoding(value);
            params.addQueryItem(key, encodedValue);
        }
        url.setQuery(params);
    }
    QFunctionTime f(QString("PUT %1").arg(url.toString()));
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    for(QString param : paramsHeader.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(paramsHeader.value(param).toStdString()));
    }

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->put(requester_, QJsonDocument(data).toJson()));

    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    DBG_INFO << requester_.url().toString();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}

QJsonObject QRest::deleteSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("DELETE %1").arg(command));
    QJsonObject ret;
    QNetworkRequest requester_(QUrl::fromUserInput(command));
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->sendCustomRequest(requester_, "DELETE", QJsonDocument(data).toJson()));

    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    DBG_INFO << requester_.url().toString();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}

QJsonObject QRest::deleteSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QUrl url = QUrl::fromUserInput(command);
    if(!paramsQuery.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, paramsQuery.keys()) {
            QString value           = paramsQuery.value(key);
            QString encodedValue    = QUrl::toPercentEncoding(value);
            params.addQueryItem(key, encodedValue);
        }
        url.setQuery(params);
    }
    QFunctionTime f(QString("DELETE %1").arg(url.toString()));
    QJsonObject ret;
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    for(QString param : paramsHeader.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(paramsHeader.value(param).toStdString()));
    }

    OurSharedPointer<QNetworkAccessManager> manager = networkManager();
    if (manager.isNull()) {
        reply_code = -1;
        reply_msg = "Network manager is not available.";
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
        return QJsonObject();
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(manager->sendCustomRequest(requester_, "DELETE", QJsonDocument(data).toJson()));

    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    DBG_INFO << requester_.url().toString();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    if (json.isNull() || !json.isObject()) {
        DBG_ERROR << "Invalid JSON response:" << response_data;
    }
    else {
        ret = json.object();
    }
    reply->deleteLater();
    reply.release();
    return ret;
}
