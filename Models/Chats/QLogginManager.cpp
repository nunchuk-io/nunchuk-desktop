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
#include "QLogginManager.h"
#include <QQmlEngine>
#include <QHostInfo>
#include "qt_connection_util.h"
#include "room.h"
#include <connection.h>
#include <ssosession.h>
#include <settings.h>
#include "QOutlog.h"
#include "Servers/Draco.h"

inline QString accessTokenFileName(const AccountSettings& account) {
  QString fileName = account.userId();
  fileName.replace(':', '_');
  QString ret = QString("%1/%2").arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).arg(fileName);
  return ret;
}

QLogginManager::QLogginManager(Connection *c):
    m_userid(""),
    m_password(""),
    m_connection(c),
    m_devicename("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QLogginManager::~QLogginManager()
{
    disconnect();
}

Connection *QLogginManager::connection()
{
    return m_connection;
}

void QLogginManager::invokeLogin(const QString &userid, const QString &password)
{
    if(connection()){
        auto url = QUrl::fromUserInput(HOME_SERVER);
        url.setScheme("https"); // Qt defaults to http (or even ftp for some)
        QString device_id = QString("%1%2").arg(Draco::instance()->deviceId()).arg(userid);

        this->setUserid(userid);
        this->setPassword(password);
        if (connection()->homeserver() == url && !connection()->loginFlows().empty()){
            requestLogin();
        }
        else if (!url.isValid()){
            DBG_INFO << "URL invalid";
        }
        else {
            connection()->setHomeserver(url);
            connectSingleShot(connection(), &Connection::loginFlowsChanged, this, [this] {
                DBG_INFO << "invokeLogin : Received login flows, trying to login";
                requestLogin();
            });
        }
    }
}

void QLogginManager::requestLogout()
{
    DBG_INFO << "DO NOT SIGNOUT FROM MATRIX - E2EE REQUIRED";
    if(connection()){
        if(AppSetting::instance()->enableMultiDeviceSync()){
            AppModel::instance()->startMultiDeviceSync(false);
        }
        connection()->stopSync();
    }
}

void QLogginManager::requestLogin()
{
    if(connection()){
        connection()->setCacheState(false); // FIXME
        connectSingleShot(connection(), &Connection::connected, this, [this]{
            connection()->loadState();
            connection()->sync();
            if (!CLIENT_INSTANCE->saveDataToKeyChain(userid(), connection()->accessToken())){
                DBG_INFO << "Couldn't save access token";
            }
            connect(connection()->user(), &User::defaultAvatarChanged, CLIENT_INSTANCE, &ClientController::onUserAvatarChanged );
            connect(connection()->user(), &User::defaultNameChanged, CLIENT_INSTANCE, &ClientController::onUserDisplaynameChanged );
            connectSingleShot(connection(), &Connection::syncDone, this, [this] {
                if(CLIENT_INSTANCE->rooms()){
                    CLIENT_INSTANCE->rooms()->downloadRooms();
                }
                connection()->syncLoop();
            });
            CLIENT_INSTANCE->refreshContacts();
            CLIENT_INSTANCE->refreshDevices();
            AppSetting::instance()->setIsStarted(true,true);
            emit CLIENT_INSTANCE->userChanged();
        });
        QByteArray actk = CLIENT_INSTANCE->readDataFromKeyChain(userid());
        if(actk.isNull() || actk.isEmpty()){
            loginWithPassword();
        }
        else{
            assumeIdentity(actk);
        }
    }
}

void QLogginManager::loginWithPassword()
{
    DBG_INFO;
    if(connection()){
        QString device_id = QString("%1%2").arg(Draco::instance()->deviceId()).arg(userid());
        QString device_name = devicename();
        connection()->loginWithPassword(userid(),
                                        password(),
                                        device_name,
                                        device_id);
    }
}

void QLogginManager::assumeIdentity(QByteArray actk)
{
    DBG_INFO << "Already has accesstoken, resume connection" << userid();
    QString device_id = QString("%1%2").arg(Draco::instance()->deviceId()).arg(userid());
    QString device_name = devicename();
    connectSingleShot(m_connection, &Connection::loginError, this, [=](QString message, QString details){
        DBG_INFO << "Can not login by assumeidentity ::::" << message << details << QString(actk) << QString(connection()->accessToken());
        loginWithPassword();
    }, Qt::QueuedConnection);
    connection()->assumeIdentity(userid(),
                                 QString(actk),
                                 device_id);
}

QString QLogginManager::devicename()
{
    if(m_devicename == ""){
        m_devicename = generateDeviceName();
    }
    return m_devicename;
}

QString QLogginManager::userid() const
{
    return m_userid;
}

void QLogginManager::setUserid(const QString &userid)
{
    m_userid = userid;
}

QString QLogginManager::password() const
{
    return m_password;
}

void QLogginManager::setPassword(const QString &password)
{
    m_password = password;
}

QString QLogginManager::generateDeviceName()
{
    QString ret = QString("nunchuk-Desktop_%1_%2_%3_%4")
            .arg(QSysInfo::machineHostName())
            .arg(QSysInfo::productType())
            .arg(QSysInfo::productVersion())
            .arg(QSysInfo::currentCpuArchitecture());
    DBG_INFO << ret;
    return ret;
}

void QLogginManager::homeserverChanged(QUrl _url)
{
    DBG_INFO << _url;
}

void QLogginManager::loginFlowsChanged()
{
    DBG_INFO << (connection()->isUsable() ? "The homeserver is available" : "Could not connect to the homeserver");
}

void QLogginManager::resolveError(QString _message)
{
    DBG_INFO << _message;
}

void QLogginManager::loginError(QString _t1, QString _t2)
{
    DBG_INFO << _t1 << _t2;
}

void QLogginManager::reconnected()
{
    DBG_INFO ;
}

void QLogginManager::stateChanged()
{
    DBG_INFO ;
}
