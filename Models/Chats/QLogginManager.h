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

#ifndef QLOGGIN_H
#define QLOGGIN_H
#include "QOutlog.h"
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include "QEventProcessor.h"

#define HOME_SERVER "https://matrix.nunchuk.io/"

namespace Quotient {
    class AccountSettings;
    class Connection;
}
using namespace Quotient;

typedef QSharedPointer<Quotient::Connection> QuotientConnectionPtr;

class AccessTokenFile : public QFile {
    bool legacyLocation = false;

public:
    template <class KeySourceT>
    explicit AccessTokenFile(const KeySourceT& source, OpenMode mode = ReadOnly)
    {
        Q_ASSERT(mode == ReadOnly || mode == WriteOnly);
        if (mode == WriteOnly) {
            remove(accessTokenFileName(source, true));
            setFileName(accessTokenFileName(source, false));
            remove();
            const auto fileDir = QFileInfo(*this).dir();
            if (fileDir.exists() || fileDir.mkpath("."))
                open(QFile::WriteOnly);
            return;
        }
        for (bool getLegacyLocation: { false, true }) {
            setFileName(accessTokenFileName(source, getLegacyLocation));
            if (open(QFile::ReadOnly)) {
                if (size() < 1024) {
                    qDebug() << "Found access token file at" << fileName();
                    legacyLocation = getLegacyLocation;
                    return;
                }
                qWarning() << "File" << fileName() << "is" << size()
                           << "bytes long - too long for a token, ignoring it.";
            } else
                qWarning() << "Could not open access token file" << fileName();
            close();
        }
    }
    [[nodiscard]] bool isAtLegacyLocation() const { return legacyLocation; }
};

class QLogginManager : public QObject
{
    Q_OBJECT
public:
    QLogginManager(Connection *c);
    ~QLogginManager();
    Quotient::Connection *connection();
    void invokeLogin(const QString &userid, const QString &password);
    void requestLogout();
    void requestLogin();
    void loginWithPassword();
    void assumeIdentity(QByteArray actk);
    QString devicename();
    QString userid() const;
    void setUserid(const QString &userid);
    QString password() const;
    void setPassword(const QString &password);

private:
    QString m_userid;
    QString m_password;
    Quotient::Connection* m_connection;
    QString m_devicename;

    QString generateDeviceName();
public slots:
    void homeserverChanged(QUrl _url);
    void loginFlowsChanged();
    void resolveError(QString _message);
    void loginError(QString _t1, QString _t2);
    void reconnected();
    void stateChanged();
};

typedef QSharedPointer<QLogginManager> QLogginManagerPtr;

#endif // QLOGGIN_H
