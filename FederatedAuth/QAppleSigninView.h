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

#ifndef QAPPLESIGNINVIEW_H
#define QAPPLESIGNINVIEW_H

#include "QOutlog.h"

#include <QByteArray>
#include <QHash>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QUrl>

class QTcpServer;
class QTcpSocket;
class QTimer;

class QAppleSigninView : public QObject {
    Q_OBJECT

public:
    explicit QAppleSigninView(QObject *parent = nullptr);
    void startSignin();
    void cancelSignin(bool notifyUser = true);

signals:
    void loginSucceeded(QJsonObject data);
    void loginFailed(QString message);

private slots:
    void onNewConnection();
    void onTimeout();

private:
    QString generateAttemptToken() const;
    QUrl buildAuthUrl(const QString &clientId, const QUrl &redirectUri) const;
    void processSocket(QTcpSocket *socket);
    void rejectSocket(QTcpSocket *socket);
    void finishSucceeded(QTcpSocket *socket, const QJsonObject &result);
    void finishFailed(QTcpSocket *socket, const QString &message);
    void resetAttempt();
    void sendHtmlResponse(QTcpSocket *socket, int statusCode,
                          const QByteArray &reasonPhrase,
                          const QByteArray &body) const;

    QTcpServer *m_server = nullptr;
    QTimer *m_timeoutTimer = nullptr;
    QHash<QTcpSocket *, QByteArray> m_requestBuffers;
    QString m_attemptToken;
    bool m_active = false;
};

typedef OurSharedPointer<QAppleSigninView> QAppleSigninViewPtr;

#endif // QAPPLESIGNINVIEW_H
