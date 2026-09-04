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
#include <QPointer>
#include <QTimer>
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

void QLogginManager::invokeLogin(const QString &userid, const QString &password,
                                 bool forcePasswordLogin)
{
    DBG_INFO << "invokeLogin" << userid;
    m_forcePasswordLogin = forcePasswordLogin;
    m_initialSyncCompleted = false;
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
    CLIENT_INSTANCE->setReadySupport(false);
    if(connection()){
        if(AppSetting::instance()->enableMultiDeviceSync()){
            AppModel::instance()->startMultiDeviceSync(false);
        }
        connection()->stopSync();
    }
}

void QLogginManager::requestLogin()
{
    CLIENT_INSTANCE->setReadySupport(false);
    if(connection()){
        const QPointer<Connection> loginConnection = connection();
        const QPointer<QLogginManager> loginManager = this;
        const QString loginUserId = userid();
        if(!CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(
                loginConnection.data(), loginManager.data())){
            return;
        }
        connection()->setCacheState(false); // FIXME
        connectSingleShot(loginConnection.data(), &Connection::connected, this,
                          [this, loginConnection]{
            if(!CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(
                    loginConnection.data(), this)){
                return;
            }
            loginConnection->loadState();
            connect(loginConnection->user(), &User::defaultAvatarChanged,
                    this, [this, loginConnection] {
                if(CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(
                        loginConnection.data(), this)){
                    CLIENT_INSTANCE->onUserAvatarChanged();
                }
            });
            connect(loginConnection->user(), &User::defaultNameChanged,
                    this, [this, loginConnection] {
                if(CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(
                        loginConnection.data(), this)){
                    CLIENT_INSTANCE->onUserDisplaynameChanged();
                }
            });
            connectSingleShot(loginConnection.data(), &Connection::syncDone, this,
                              [this, loginConnection] {
                if(!CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(
                        loginConnection.data(), this)){
                    return;
                }
                m_initialSyncCompleted = true;
                const QPointer<QLogginManager> loginManager = this;
                const QString loginUserId = userid();
                // Persist only a token that has completed an authenticated
                // sync; assumeIdentity emits connected before token validation.
                const bool enteredCriticalSection =
                        CLIENT_INSTANCE->beginMatrixLoginCriticalSection(
                            loginConnection.data(), loginManager.data());
                const bool tokenSaved = CLIENT_INSTANCE->saveDataToKeyChain(
                    loginUserId, loginConnection->accessToken());
                CLIENT_INSTANCE->endMatrixLoginCriticalSection(
                        enteredCriticalSection);
                // Keychain APIs may run a nested event loop. Recheck the login
                // manager and attempt before dereferencing either again.
                if(!CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(
                        loginConnection.data(), loginManager.data())
                        || CLIENT_INSTANCE->hasPendingMatrixLoginReplacement(
                            loginConnection.data(), loginManager.data())){
                    return;
                }
                if(!tokenSaved){
                    DBG_INFO << "Couldn't save access token";
                }
                QNunchukRoomListModel* currentRooms = CLIENT_INSTANCE->rooms();
                if(currentRooms && currentRooms->connection() == loginConnection){
                    currentRooms->downloadRooms();
                }
                loginConnection->syncLoop();
            });
            // Install the first-sync handler before starting the request so a
            // very fast response can never leave room hydration untriggered.
            loginConnection->sync();
            CLIENT_INSTANCE->refreshContacts();
            CLIENT_INSTANCE->refreshDevices();
            AppSetting::instance()->setIsStarted(true,true);
            emit CLIENT_INSTANCE->userChanged();
        });
        const bool forcePasswordLogin = m_forcePasswordLogin;
        m_forcePasswordLogin = false;
        QByteArray actk;
        if(!forcePasswordLogin){
            const bool enteredCriticalSection =
                    CLIENT_INSTANCE->beginMatrixLoginCriticalSection(
                        loginConnection.data(), loginManager.data());
            actk = CLIENT_INSTANCE->readDataFromKeyChain(loginUserId);
            CLIENT_INSTANCE->endMatrixLoginCriticalSection(
                    enteredCriticalSection);
            // readDataFromKeyChain runs a nested event loop; a relogin can
            // be requested while it is active. The controller queues that
            // request until this critical section exits.
            if(!loginManager
                    || CLIENT_INSTANCE->loginHandler() != loginManager.data()
                    || !loginConnection
                    || CLIENT_INSTANCE->connection() != loginConnection
                    || !CLIENT_INSTANCE->isNunchukLoggedIn()
                    || CLIENT_INSTANCE->hasPendingMatrixLoginReplacement(
                        loginConnection.data(), loginManager.data())){
                return;
            }
        }
        // Apply the same account/attempt guard to both token and forced
        // password branches. loginFlowsChanged and keychain APIs can complete
        // after logout or after another Nunchuk account becomes active.
        if(!CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(
                loginConnection.data(), loginManager.data())
                || CLIENT_INSTANCE->hasPendingMatrixLoginReplacement(
                    loginConnection.data(), loginManager.data())){
            return;
        }
        if(actk.isNull() || actk.isEmpty()){
            loginManager->loginWithPassword();
        }
        else{
            loginManager->assumeIdentity(actk);
        }
    }
}

void QLogginManager::loginWithPassword()
{
    if(connection()
            && CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(connection(), this)){
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
    if(!CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(connection(), this)){
        return;
    }
    DBG_INFO << "Already has accesstoken, resume connection" << userid();
    QString device_id = QString("%1%2").arg(Draco::instance()->deviceId()).arg(userid());
    const QPointer<Connection> loginConnection = connection();
    connectSingleShot(loginConnection.data(), &Connection::loginError, this,
                      [this, loginConnection](QString message, QString details){
        if(m_initialSyncCompleted
                || !CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(
                    loginConnection.data(), this)){
            return;
        }
        // Never print access tokens. A failed cached-token login has already
        // consumed the one-shot connected handler, so reset the attempt and
        // re-arm the complete password-login lifecycle instead of calling
        // loginWithPassword() directly.
        DBG_INFO << "Can not login by assumeidentity:" << message << details;
        CLIENT_INSTANCE->setReadySupport(false);
        CLIENT_INSTANCE->setMatrixLoginTransitioning(true);
        const QString failedUserId = userid();
        const QPointer<QLogginManager> loginManager = this;
        loginConnection->stopSync();
        QObject::disconnect(loginConnection, nullptr, this, nullptr);
        // Do not delete the cached entry here: the keychain helper runs a
        // nested event loop and a stale callback could erase a newer token.
        // The forced password login bypasses it, and the first successful sync
        // overwrites it with the validated token.
        QTimer::singleShot(0, CLIENT_INSTANCE,
                          [failedUserId, loginConnection, loginManager] {
            if(CLIENT_INSTANCE->isCurrentMatrixLoginAttempt(
                    loginConnection.data(), loginManager.data())
                    && Draco::instance()->chatId() == failedUserId){
                CLIENT_INSTANCE->requestLogin(true);
            }
        });
    }, Qt::QueuedConnection);
    loginConnection->assumeIdentity(userid(),
                                    device_id,
                                    QString::fromUtf8(actk));
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
