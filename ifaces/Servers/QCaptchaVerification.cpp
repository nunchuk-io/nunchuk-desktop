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

#include "QCaptchaVerification.h"
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QWebChannel>
#include <QTimer>
#include <QUrl>
#include <QtCore/qjsondocument.h>
#include "QAppEngine/QEventProcessor/QEventProcessor.h"
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

void VerificationBridge::onMessage(const QString &json) {
    const auto doc = QJsonDocument::fromJson(json.toUtf8());
    const auto obj = doc.object();
    if (obj.value("event").toString() == "token") {
        emit tokenReceived(obj.value("payload").toString());
    }
}

void VerificationBridge::onToken(const QString &token) {
    emit tokenReceived(token);
}

QCaptchaVerification::QCaptchaVerification(QWidget *parent)
    : QWebEngineView(parent)
{
    resize(600, 800);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(this, &QWebEngineView::loadFinished,
            this, &QCaptchaVerification::onLoadFinished);
}

void QCaptchaVerification::startVerifyCaptcha()
{
    cleanup();

    m_profile = new QWebEngineProfile();
    m_profile->setHttpCacheType(QWebEngineProfile::NoCache);
    m_profile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);

    m_page = new QWebEnginePage(m_profile);
    setPage(m_page);

    QWebEngineScript script;
    script.setName("center-content");
    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setRunsOnSubFrames(true);
    script.setWorldId(QWebEngineScript::MainWorld);

    script.setSourceCode(R"(
    document.body.style.display = 'flex';
    document.body.style.justifyContent = 'center';
    document.body.style.alignItems = 'center';
    document.body.style.height = '100vh';
    document.body.style.margin = '0';
    )");

    m_page->scripts().insert(script);

    m_channel = new QWebChannel();
    m_bridge  = new VerificationBridge();

    m_channel->registerObject("verificationBridge", m_bridge);
    m_page->setWebChannel(m_channel);

    connect(m_bridge, &VerificationBridge::tokenReceived,
            this, &QCaptchaVerification::handleToken);

    connect(m_page, &QWebEnginePage::loadFinished,
            this, &QCaptchaVerification::onLoadFinished);

    QTimer::singleShot(120000, this, [this]() {
        if (!m_tokenReceived) {
            emit cancelled();
            safeClose();
        }
    });

    // ===== LOADING UI =====
    if (!m_loadingOverlay) {
        m_loadingOverlay = new QLoadingOverlay(this);
        m_loadingOverlay->resize(600, 800);

        connect(m_page, &QWebEnginePage::loadStarted, this, [this]() {
            m_loadingOverlay->show();
            m_loadingOverlay->raise();
        });

        connect(m_page, &QWebEnginePage::loadProgress, this, [this](int progress) {
            if (progress >= 90) {
                m_loadingOverlay->hide();
            }
        });

        connect(m_page, &QWebEnginePage::loadFinished, this, [this](bool) {
            m_loadingOverlay->hide();
        });

        connect(QEventProcessor::instance(),
                &QEventProcessor::visibleChanged,
                this, [this](bool visible) {
                    if (!visible) {
                        safeClose();
                        if (m_loadingOverlay) {
                            m_loadingOverlay->hide();
                        }
                    }
                });
    }

    setWindowFlags(Qt::Dialog |
                   Qt::WindowTitleHint |
                   Qt::WindowSystemMenuHint |
                   Qt::WindowStaysOnTopHint |
                   Qt::WindowCloseButtonHint);

    load(QUrl("https://verification.nunchuk.io/"));
    show();
}

void QCaptchaVerification::safeClose()
{
    if (m_closing) return;
    m_closing = true;

    if (page()) {
        page()->triggerAction(QWebEnginePage::Stop);
    }

    setPage(nullptr);

    QTimer::singleShot(0, this, [this]() {
        cleanup();
        close();
    });
}

void QCaptchaVerification::cleanup()
{
    if (m_page) {
        m_page->setParent(nullptr);
        m_page->deleteLater();
        m_page = nullptr;
    }

    if (m_profile) {
        m_profile->setParent(nullptr);
        m_profile->deleteLater();
        m_profile = nullptr;
    }

    if (m_channel) {
        m_channel->deleteLater();
        m_channel = nullptr;
    }

    if (m_bridge) {
        m_bridge->deleteLater();
        m_bridge = nullptr;
    }

    if (m_loadingOverlay) {
        m_loadingOverlay->hide();
    }

    m_channelInjected = false;
    m_tokenReceived = false;
}

void QCaptchaVerification::handleToken(const QString &token)
{
    if (m_tokenReceived) return;
    m_tokenReceived = true;

    emit verified(token);
    safeClose();
}

void QCaptchaVerification::onLoadFinished(bool ok)
{
    if (!ok) {
        emit cancelled();
        safeClose();
        return;
    }

    if (m_channelInjected) return;
    m_channelInjected = true;

    QTimer::singleShot(100, this, [this]() {
        injectWebChannel();
    });
}

void QCaptchaVerification::injectWebChannel()
{
    if (!page()) return;

    page()->runJavaScript(R"(
        (function() {
          function setupChannel() {
            new QWebChannel(qt.webChannelTransport, function(channel) {
              window.verificationBridge = channel.objects.verificationBridge;
            });
          }

          if (typeof QWebChannel === 'undefined') {
            var script = document.createElement('script');
            script.src = 'qrc:///qtwebchannel/qwebchannel.js';
            script.onload = setupChannel;
            document.head.appendChild(script);
          } else {
            setupChannel();
          }
        })();
    )");
}
