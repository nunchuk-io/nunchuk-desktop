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

#if ENABLE_WEBVIEW_SIGIN
#include "QOutlog.h"
#include <QWebEngineView>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineUrlRequestInfo>
#include <QJsonObject>
#include <QWebEngineView>
#include "QLoadingOverlay.h"

class QGoogleSigninView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit QGoogleSigninView(QWidget* parent = nullptr);
    void startSignin();
    void exchangeCodeForTokens(const QString &code);
    void completeGoogleSignin(const QString &idToken, const QString &accessToken);
    void fetchGoogleUserInfo(const QString &accessToken, QJsonObject data);
private:
    QWebEnginePage*         m_page;
    QWebEngineProfile*      m_profile;
    QNetworkAccessManager*  m_networkManager;
    QLoadingOverlay*        m_loadingOverlay;
    QString                 m_redirectUri;
    QString                 m_clientId;
    QString                 m_clientSecret;
    QString                 m_codeVerifier;

signals:
    void tokenReceived(const QString& accessToken);
    void loginSucceeded(QJsonObject data);

private slots:
    void handleUrlChanged(const QUrl& url);
};
typedef OurSharedPointer<QGoogleSigninView> QGoogleSigninViewPtr;
#endif

#endif // QGOOGLESIGNINVIEW_H
