/*************************************************************************
* This file is part of the Nunchuk software (https://nunchuk.io/)        *
* Copyright (C) 2020-2022 Enigmo                                         *
* Copyright (C) 2022 Nunchuk                                             *
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

#ifndef QGOOGLESIGNINVIEW_H
#define QGOOGLESIGNINVIEW_H

#include "QOutlog.h"

#include <QJsonObject>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QVariantMap>

class QNetworkAccessManager;
class QNetworkReply;
class QOAuthHttpServerReplyHandler;
class QTimer;

// Kept under its historical name to avoid churn at call sites. This is now
// an OAuth controller; it never owns or displays an embedded browser view.
class QGoogleSigninView : public QObject
{
    Q_OBJECT

public:
    explicit QGoogleSigninView(QObject *parent = nullptr);
    ~QGoogleSigninView() override;

    void startSignin();
    void cancelSignin(bool notifyUser = true);

signals:
    void loginSucceeded(QJsonObject result);
    void loginFailed(const QString &message);

private:
    enum class Phase {
        Idle,
        Authorizing,
        SigningIn
    };

    static QString randomUrlSafeToken(int byteCount);
    static QString codeChallenge(const QString &verifier);

    void handleAuthorizationCallback(const QVariantMap &parameters);
    void sendAuthorizationCodeToNunchuk(const QString &authorizationCode);
    void handleNunchukReply();
    void failCurrentAttempt(const QString &message);
    void clearAttempt();

    Phase m_phase{Phase::Idle};
    QNetworkAccessManager *m_networkManager{nullptr};
    QOAuthHttpServerReplyHandler *m_replyHandler{nullptr};
    QPointer<QNetworkReply> m_nunchukReply;
    QTimer *m_timeoutTimer{nullptr};
    QString m_codeVerifier;
    QString m_state;
    QString m_redirectUri;
};

typedef OurSharedPointer<QGoogleSigninView> QGoogleSigninViewPtr;

#endif // QGOOGLESIGNINVIEW_H
