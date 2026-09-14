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
#include <QCloseEvent>
#include <QWebEngineProfile>
#include <QWebChannel>
#include <QTimer>
#include <QUrl>
#include <QVariant>
#include <QtCore/qjsondocument.h>
#include "QAppEngine/QEventProcessor/QEventProcessor.h"

namespace {
// How long the user has to complete the challenge before we give up and
// report it as cancelled.
constexpr int kCaptchaVerificationTimeoutMs = 120 * 1000;

// The challenge widget (Turnstile/hCaptcha) renders asynchronously *after*
// the page itself finishes loading, so we poll for it instead of hiding the
// loading overlay on a fixed loadProgress heuristic. kChallengeReadyMaxWaitMs
// is a safety ceiling so a blocked/broken challenge script doesn't leave the
// overlay stuck forever, hiding the page's own "Try again" button.
constexpr int kChallengeReadyPollIntervalMs = 150;
constexpr int kChallengeReadyMaxWaitMs = 8 * 1000;
}

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

    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, [this]() {
        if (!m_tokenReceived) {
            emit cancelled();
            safeClose();
        }
    });
}

void QCaptchaVerification::startVerifyCaptcha()
{
    cleanup();

    m_profile = new QWebEngineProfile();
    m_profile->setHttpCacheType(QWebEngineProfile::NoCache);
    m_profile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);

    m_page = new QWebEnginePage(m_profile);
    setPage(m_page);

    // verification.nunchuk.io already centers its own content via CSS, so no
    // extra layout script needs to be injected here.

    m_channel = new QWebChannel();
    m_bridge  = new VerificationBridge();

    m_channel->registerObject("verificationBridge", m_bridge);
    m_page->setWebChannel(m_channel);

    connect(m_bridge, &VerificationBridge::tokenReceived,
            this, &QCaptchaVerification::handleToken);

    connect(m_page, &QWebEnginePage::loadFinished,
            this, &QCaptchaVerification::onLoadFinished);

    m_timeoutTimer->start(kCaptchaVerificationTimeoutMs);

    // ===== LOADING UI =====
    if (!m_loadingOverlay) {
        m_loadingOverlay = new QLoadingOverlay(this);
        m_loadingOverlay->resize(600, 800);

        connect(m_page, &QWebEnginePage::loadStarted, this, [this]() {
            m_loadingOverlay->show();
            m_loadingOverlay->raise();
        });

        // Don't hide on loadProgress/loadFinished alone: the page's HTML
        // finishes loading before the Turnstile/hCaptcha widget has fetched
        // its script and rendered, so that would reveal an empty box for a
        // moment. Poll the DOM for the actual widget instead.
        m_challengeReadyPollTimer = new QTimer(this);
        m_challengeReadyPollTimer->setInterval(kChallengeReadyPollIntervalMs);

        connect(m_challengeReadyPollTimer, &QTimer::timeout, this, [this]() {
            m_challengeReadyElapsedMs += kChallengeReadyPollIntervalMs;
            if (m_challengeReadyElapsedMs >= kChallengeReadyMaxWaitMs) {
                m_challengeReadyPollTimer->stop();
                if (m_loadingOverlay) {
                    m_loadingOverlay->hide();
                }
                return;
            }
            if (!page()) {
                m_challengeReadyPollTimer->stop();
                return;
            }

            QPointer<QCaptchaVerification> self(this);
            page()->runJavaScript(
                QStringLiteral(
                    "(function(){"
                    "var el=document.getElementById('challengeWidget');"
                    "return !!(el && el.childElementCount > 0);"
                    "})();"),
                [self](const QVariant &ready) {
                    if (!self || !ready.toBool()) return;
                    if (self->m_challengeReadyPollTimer) {
                        self->m_challengeReadyPollTimer->stop();
                    }
                    if (self->m_loadingOverlay) {
                        self->m_loadingOverlay->hide();
                    }
                });
        });

        connect(m_page, &QWebEnginePage::loadFinished, this, [this](bool ok) {
            if (!ok) {
                if (m_loadingOverlay) {
                    m_loadingOverlay->hide();
                }
                return;
            }
            m_challengeReadyElapsedMs = 0;
            m_challengeReadyPollTimer->start();
        });

        connect(QEventProcessor::instance(),
                &QEventProcessor::visibleChanged,
                this, [this](bool visible) {
                    if (!visible) {
                        if (!m_tokenReceived) {
                            emit cancelled();
                        }
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

    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
    }
    if (m_challengeReadyPollTimer) {
        m_challengeReadyPollTimer->stop();
    }

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
    if (m_challengeReadyPollTimer) {
        m_challengeReadyPollTimer->stop();
    }

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
    // No manual QWebChannel injection is needed here: verification.nunchuk.io
    // already detects window.qt.webChannelTransport (exposed by setWebChannel()
    // above) and wires up "verificationBridge" itself via its own setupQtBridge().
    if (!ok) {
        emit cancelled();
        safeClose();
    }
}

void QCaptchaVerification::closeEvent(QCloseEvent *event)
{
    // Reached either because safeClose() is already tearing us down (in which
    // case just let the close proceed), or because the user closed the window
    // directly (titlebar close button) without completing verification. The
    // latter used to leave the caller (Draco::requireCaptchaVerification)
    // waiting forever and leaked m_page/m_profile/m_channel/m_bridge, since
    // none of them have this widget as their QObject parent.
    if (m_closing) {
        event->accept();
        return;
    }

    m_closing = true;
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
    }
    if (m_challengeReadyPollTimer) {
        m_challengeReadyPollTimer->stop();
    }
    if (page()) {
        page()->triggerAction(QWebEnginePage::Stop);
    }
    setPage(nullptr);
    cleanup();

    event->accept();
    emit cancelled();
}
