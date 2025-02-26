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

#ifndef QREST_H
#define QREST_H

#include <QObject>
#include "QOutlog.h"
#include <QQmlEngine>
#include <QAuthenticator>
#include <QNetworkCookieJar>
#include <QEventLoop>
#include <QUrlQuery>
#include <QSysInfo>
#include "localization/STR_CPP.h"
#include <QSysInfo>
#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include "DracoDefines.h"

class QRest : public QObject
{
    Q_OBJECT
public:
    QRest();
    ~QRest();

    static QString dracoToken();
    static void setDracoToken(const QString token);
    static QByteArray machineUniqueId();
    QString url() const;
protected:
    OurSharedPointer<QNetworkAccessManager> m_networkManager;
    QJsonObject postSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg);
    QJsonObject postSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg);
    QJsonObject postMultiPartSync(const QString &cmd, QMap<QString, QVariant> data, int &reply_code, QString &reply_msg);
    QJsonObject postMultiPartSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QMap<QString, QVariant> data, int &reply_code, QString &reply_msg);
    QJsonObject getSync(const QString &cmd, QJsonObject paramsQuery, int &reply_code, QString &reply_msg);
    QJsonObject getSync(const QString &cmd, QMap<QString, QString> paramsHeader, QJsonObject paramsQuery, int &reply_code, QString &reply_msg);
    QJsonObject putSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg);
    QJsonObject putSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg);
    QJsonObject deleteSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg);
    QJsonObject deleteSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg);

private:
    static QString      m_dracoToken;
    static QByteArray   m_machineUniqueId;
    QMutex              m_networkManagerMutex;
};

#endif // QREST_H
