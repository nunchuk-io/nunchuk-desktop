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

#include "QAppleSigninView.h"

#if ENABLE_WEBVIEW_SIGIN
#include <QWebEngineProfile>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QRegularExpression>
#include <QTextDocumentFragment>
#include "QOutlog.h"
#include "WorkerThread.h"
#include "Servers/Draco.h"

QInsecureRequestBlocker::QInsecureRequestBlocker(QObject *parent) : QWebEngineUrlRequestInterceptor(parent) {

}

void QInsecureRequestBlocker::interceptRequest(QWebEngineUrlRequestInfo &info) {
    if (info.requestUrl().scheme() == QLatin1String("http")) {
        DBG_INFO << "Blocked insecure HTTP request:" << info.requestUrl();
        info.block(true);
    }
}

QAppleSigninView::QAppleSigninView(QWidget *parent) : QWebEngineView(parent) {
    m_profile = new QWebEngineProfile("AppleOAuth", this);
    m_profile->setHttpCacheType(QWebEngineProfile::NoCache);
    m_profile->setUrlRequestInterceptor(new QInsecureRequestBlocker(m_profile));
    m_profile->settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);

    m_page = new QWebEnginePage(m_profile, this);
    setPage(m_page);
    setFixedSize(600, 800);
    setWindowTitle("Sign in with Apple");
    connect(m_page, &QWebEnginePage::urlChanged, this, &QAppleSigninView::handleUrlChanged);
    connect(m_page, &QWebEnginePage::loadFinished, this, &QAppleSigninView::handleLoadFinished);

    m_loadingOverlay = new QLoadingOverlay(this);
    m_loadingOverlay->resize(600, 800);

    connect(m_page, &QWebEnginePage::loadStarted, this, [this]() {
        m_loadingOverlay->show();
        m_loadingOverlay->raise();
    });
    connect(m_page, &QWebEnginePage::loadFinished, this, [this](bool) {
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

void QAppleSigninView::startSignin() {
    QUrl authUrl("https://appleid.apple.com/auth/authorize");
    QUrlQuery query;
    query.addQueryItem("client_id",     "io.nunchuk.signin");
    query.addQueryItem("redirect_uri",  "https://api.nunchuk.io/v1.1/passport/apple/signin_callback");
    query.addQueryItem("scope",         "name email");
    query.addQueryItem("response_mode", "form_post");
    query.addQueryItem("response_type", "code");
    authUrl.setQuery(query);
    load(authUrl);
    show();
}

void QAppleSigninView::handleUrlChanged(const QUrl &url) {
    DBG_INFO << "URL changed:" << url.toString();
}

void QAppleSigninView::handleLoadFinished(bool success) {
    if (success) {
        QUrl currentUrl = m_page->url();
        if (m_page->url().toString().startsWith("https://api.nunchuk.io/v1.1/passport/apple/signin_callback")) {
            m_page->toHtml([=, this](const QString &html) {
                QRegularExpression re(R"(<pre[^>]*>(.*?)</pre>)", QRegularExpression::DotMatchesEverythingOption);
                QRegularExpressionMatch match = re.match(html);
                if (match.hasMatch()) {
                    QString jsonText = match.captured(1);
                    QString cleanJson = QTextDocumentFragment::fromHtml(jsonText).toPlainText();
                    QJsonParseError parseError;
                    QJsonDocument doc = QJsonDocument::fromJson(cleanJson.toUtf8(), &parseError);
                    if (parseError.error != QJsonParseError::NoError) {
                        DBG_INFO << "JSON parse error:" << parseError.errorString();
                        return;
                    }
                    if (!doc.isObject()) {
                        DBG_INFO << "JSON is not an object.";
                        return;
                    }

                    QJsonObject htmlObj = doc.object();
                    DBG_INFO << "Parsed JSON:" << htmlObj;
                    this->close();
                    emit loginSucceeded(htmlObj);
                }
                else {
                    DBG_INFO << "No <pre> tag found in HTML.";
                }
            });
        }
    }
}
#endif
