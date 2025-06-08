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
#include "QGoogleSigninView.h"

#if ENABLE_WEBVIEW_SIGIN
#include <QObject>
#include "WorkerThread.h"
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QtCore/qeventloop.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>
#include <QtWebEngineWidgets/qwebengineprofile.h>
#include <QtWebEngineWidgets/qwebenginesettings.h>
#include <QApplication>
#include "QEventProcessor.h"

QGoogleSigninView::QGoogleSigninView(QWidget* parent) : QWebEngineView(parent) {
    m_clientId = QStringLiteral(OAUTH_CLIENT_ID);
    if (m_clientId.isEmpty()) {
        m_clientId = qEnvironmentVariable("OAUTH_CLIENT_ID");
    }
    m_clientSecret = QStringLiteral(OAUTH_CLIENT_SECRET);
    if (m_clientSecret.isEmpty()) {
        m_clientSecret = qEnvironmentVariable("OAUTH_CLIENT_SECRET");
    }
    m_redirectUri = QStringLiteral(OAUTH_REDIRECT_URI);
    if (m_redirectUri.isEmpty()) {
        m_redirectUri = qEnvironmentVariable("OAUTH_REDIRECT_URI");
    }

    m_profile = new QWebEngineProfile("GoogleOAuth", this);
    m_profile->setHttpCacheType(QWebEngineProfile::NoCache);
    m_profile->settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::XSSAuditingEnabled, false);
    m_profile->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    m_page = new QWebEnginePage(m_profile, this);
    setPage(m_page);
    setFixedSize(600, 800);
    setWindowTitle("Sign in with Google");
    m_networkManager = new QNetworkAccessManager(this);
    connect(this, &QWebEngineView::urlChanged, this, &QGoogleSigninView::handleUrlChanged);

    m_loadingOverlay = new QLoadingOverlay(this);
    m_loadingOverlay->resize(600, 800);

    connect(m_page, &QWebEnginePage::loadStarted, this, [this]() {
        DBG_INFO << "Loading started";
        m_loadingOverlay->show();
        m_loadingOverlay->raise();
    });
    connect(m_page, &QWebEnginePage::loadProgress, this, [this](int progress) {
        DBG_INFO << "Loading finished" << progress;
        if(progress >= 90) {
            m_loadingOverlay->hide();
        }
    });
    connect(m_page, &QWebEnginePage::loadFinished, this, [this](bool finished) {
        DBG_INFO << "Loading finished" << finished;
        m_loadingOverlay->hide();
    });

    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    connect(QEventProcessor::instance(), &QEventProcessor::visibleChanged, this, [this](bool visible) {
        if(!visible){
            close();
            m_loadingOverlay->hide();
        }
    });
}

void QGoogleSigninView::startSignin()
{
    QUrl url("https://accounts.google.com/o/oauth2/v2/auth");
    QUrlQuery query;
    query.addQueryItem("scope", "openid email profile");
    query.addQueryItem("response_type", "code");
    query.addQueryItem("client_id", m_clientId);
    query.addQueryItem("redirect_uri", m_redirectUri);
    url.setQuery(query);
    this->load(url);
    this->show();
}

void QGoogleSigninView::handleUrlChanged(const QUrl& url)
{
    if (url.toString().startsWith(m_redirectUri)) {
        QUrlQuery query(url.query());
        if (query.hasQueryItem("code")) {
            QString code = query.queryItemValue("code");
            exchangeCodeForTokens(code);
            this->close();
        }
    }
}

void QGoogleSigninView::exchangeCodeForTokens(const QString& code)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    QUrl tokenUrl("https://oauth2.googleapis.com/token");
    QNetworkRequest request(tokenUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("code", code);
    params.addQueryItem("client_id", m_clientId);
    params.addQueryItem("client_secret", m_clientSecret);
    params.addQueryItem("redirect_uri", m_redirectUri);
    params.addQueryItem("grant_type", "authorization_code");

    QByteArray postData = params.query(QUrl::FullyEncoded).toUtf8();

    QNetworkReply* reply = m_networkManager->post(request, postData);
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        QByteArray responseData = reply->readAll();
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObj = jsonDoc.object();
            QString idToken = jsonObj.value("id_token").toString();
            QString accessToken = jsonObj.value("access_token").toString();
            completeGoogleSignin(idToken, accessToken);
        }
        else {
            DBG_INFO << "Error exchanging code for tokens:" << reply->errorString();
        }
        qApp->restoreOverrideCursor();
        reply->deleteLater();
    });
}

void QGoogleSigninView::completeGoogleSignin(const QString& idToken, const QString &accessToken)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    QUrl nunchukUrl("https://api.nunchuk.io/v1.1/passport/google/signin");
    QNetworkRequest request(nunchukUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["id_token"] = idToken;
    QJsonDocument jsonDoc(json);
    QByteArray postData = jsonDoc.toJson();

    QNetworkReply* reply = m_networkManager->post(request, postData);
    connect(reply, &QNetworkReply::finished, this, [reply, this, accessToken]() {
        QByteArray responseData = reply->readAll();
        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject responseObj = QJsonDocument::fromJson(responseData).object();
            emit loginSucceeded(responseObj);
        }
        else {
            DBG_INFO << "Error sending id_token to Nunchuk API:" << reply->errorString();
        }
        reply->deleteLater();
        qApp->restoreOverrideCursor();
    }, Qt::QueuedConnection);
}

void QGoogleSigninView::fetchGoogleUserInfo(const QString &accessToken, QJsonObject data)
{
    QNetworkRequest userInfoRequest(QUrl("https://www.googleapis.com/oauth2/v3/userinfo"));
    QString bearerHeader = QString("Bearer %1").arg(accessToken);
    userInfoRequest.setRawHeader("Authorization", bearerHeader.toUtf8());

    QNetworkReply* userReply = m_networkManager->get(userInfoRequest);
    connect(userReply, &QNetworkReply::finished, this, [userReply, data, this]() {
        if (userReply->error() == QNetworkReply::NoError) {
            QByteArray userInfoData = userReply->readAll();
            QJsonObject userInfoObj = QJsonDocument::fromJson(userInfoData).object();
            QString email = userInfoObj.value("email").toString();
            QString name = userInfoObj.value("name").toString();
            QJsonObject output = data;
            output.insert("email", email);
            output.insert("name", name);
            emit loginSucceeded(output);
        }
        userReply->deleteLater();
    }, Qt::QueuedConnection);
}
#endif
