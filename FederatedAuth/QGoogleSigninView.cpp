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

#include <QAbstractOAuthReplyHandler>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QOAuthHttpServerReplyHandler>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QTimer>
#include <QUrl>
#include <QUrlQuery>
#include <QVariantMap>

namespace {
constexpr int kAuthorizationTimeoutMs = 5 * 60 * 1000;
constexpr int kNunchukSigninTimeoutMs = 30 * 1000;

const QUrl kGoogleAuthorizationUrl(
    QStringLiteral("https://accounts.google.com/o/oauth2/v2/auth"));
const QUrl kNunchukDesktopSigninUrl(
    QStringLiteral("https://api.nunchuk.io/v1.1/passport/google/desktop/signin"));
const QString kNunchukGoogleDesktopClientId = QStringLiteral(
    "712097058578-0n3tufg0oeapgnh5ahmtfb7bi6tu0kgo.apps.googleusercontent.com");

QString sanitizedErrorText(QString value, int maximumLength)
{
    value.replace(QRegularExpression(QStringLiteral("[\\x00-\\x1f\\x7f]")),
                  QStringLiteral(" "));
    return value.simplified().left(maximumLength);
}

QString nunchukFailureMessage(const QByteArray &body,
                              int httpStatus,
                              const QString &networkError)
{
    const QJsonDocument document = QJsonDocument::fromJson(body);
    if (document.isObject()) {
        const QJsonObject error = document.object()
                                      .value(QStringLiteral("error"))
                                      .toObject();
        const QString message = sanitizedErrorText(
            error.value(QStringLiteral("message")).toString(), 512);
        if (!message.isEmpty()) {
            return QGoogleSigninView::tr("Nunchuk sign-in failed: %1")
                .arg(message);
        }
    }

    return QGoogleSigninView::tr("Nunchuk sign-in failed (HTTP %1): %2")
        .arg(httpStatus)
        .arg(sanitizedErrorText(networkError, 512));
}
}

QGoogleSigninView::QGoogleSigninView(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_replyHandler(new QOAuthHttpServerReplyHandler(
          QHostAddress::LocalHost, 0, this))
    , m_timeoutTimer(new QTimer(this))
{
    m_replyHandler->setCallbackPath(QStringLiteral("/oauth2/callback"));
    m_replyHandler->setCallbackText(
        QStringLiteral("<script>"
                       "history.replaceState(null, document.title, location.pathname);"
                       "window.close();"
                       "</script><p>")
        + tr("Google sign-in was received. You can close this tab and return to Nunchuk.")
        + QStringLiteral("</p>"));
    m_replyHandler->close();

    connect(m_replyHandler,
            &QAbstractOAuthReplyHandler::callbackReceived,
            this,
            &QGoogleSigninView::handleAuthorizationCallback);

    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, [this]() {
        failCurrentAttempt(tr("Google sign-in timed out. Please try again."));
    });
}

QGoogleSigninView::~QGoogleSigninView()
{
    m_replyHandler->close();
    if (m_nunchukReply) {
        disconnect(m_nunchukReply, nullptr, this, nullptr);
        m_nunchukReply->abort();
    }
}

QString QGoogleSigninView::randomUrlSafeToken(int byteCount)
{
    QByteArray bytes(byteCount, '\0');
    QRandomGenerator *generator = QRandomGenerator::system();
    for (int offset = 0; offset < byteCount; offset += 4) {
        const quint32 value = generator->generate();
        const int remaining = qMin(4, byteCount - offset);
        for (int index = 0; index < remaining; ++index) {
            bytes[offset + index] = static_cast<char>(
                (value >> (index * 8)) & 0xff);
        }
    }
    return QString::fromLatin1(bytes.toBase64(
        QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
}

QString QGoogleSigninView::codeChallenge(const QString &verifier)
{
    const QByteArray digest = QCryptographicHash::hash(
        verifier.toLatin1(), QCryptographicHash::Sha256);
    return QString::fromLatin1(digest.toBase64(
        QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
}

void QGoogleSigninView::startSignin()
{
    if (m_phase != Phase::Idle) {
        emit loginFailed(tr("Google sign-in is already in progress."));
        return;
    }

    m_phase = Phase::Authorizing;
    m_replyHandler->close();
    if (!m_replyHandler->listen(QHostAddress::LocalHost, 0)) {
        failCurrentAttempt(
            tr("Could not start the local Google sign-in callback."));
        return;
    }

    // 32 random bytes produce a 43-character RFC 7636 verifier.
    m_codeVerifier = randomUrlSafeToken(32);
    m_state = randomUrlSafeToken(32);
    m_redirectUri = m_replyHandler->callback();

    QUrl authorizationUrl = kGoogleAuthorizationUrl;
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("client_id"),
                       kNunchukGoogleDesktopClientId);
    query.addQueryItem(QStringLiteral("redirect_uri"), m_redirectUri);
    query.addQueryItem(QStringLiteral("response_type"),
                       QStringLiteral("code"));
    query.addQueryItem(QStringLiteral("scope"),
                       QStringLiteral("openid email profile"));
    query.addQueryItem(QStringLiteral("code_challenge"),
                       codeChallenge(m_codeVerifier));
    query.addQueryItem(QStringLiteral("code_challenge_method"),
                       QStringLiteral("S256"));
    query.addQueryItem(QStringLiteral("state"), m_state);
    authorizationUrl.setQuery(query);

    m_timeoutTimer->start(kAuthorizationTimeoutMs);
    if (!QDesktopServices::openUrl(authorizationUrl)) {
        failCurrentAttempt(
            tr("Could not open the system browser for Google sign-in."));
    }
}

void QGoogleSigninView::cancelSignin(bool notifyUser)
{
    if (m_phase == Phase::Idle) {
        return;
    }
    clearAttempt();
    if (notifyUser) {
        emit loginFailed(tr("Google sign-in was cancelled."));
    }
}

void QGoogleSigninView::handleAuthorizationCallback(
    const QVariantMap &parameters)
{
    if (m_phase != Phase::Authorizing) {
        return;
    }

    // Ignore unrelated/forged loopback requests instead of allowing them to
    // terminate the real browser attempt.
    const QString returnedState =
        parameters.value(QStringLiteral("state")).toString();
    if (returnedState.isEmpty() || returnedState != m_state) {
        return;
    }

    m_replyHandler->close();
    const QString error =
        parameters.value(QStringLiteral("error")).toString();
    if (!error.isEmpty()) {
        const QString description = parameters
                                        .value(QStringLiteral("error_description"))
                                        .toString();
        const QString detail = sanitizedErrorText(
            description.isEmpty() ? error : description, 512);
        failCurrentAttempt(
            tr("Google sign-in was cancelled or rejected: %1").arg(detail));
        return;
    }

    // Qt 5 leaves reserved characters percent-encoded in callback values.
    // Decode Google's authorization code exactly once before sending JSON.
    const QString authorizationCode = QUrl::fromPercentEncoding(
        parameters.value(QStringLiteral("code")).toString().toUtf8());
    if (authorizationCode.isEmpty()) {
        failCurrentAttempt(
            tr("Google did not return an authorization code."));
        return;
    }

    m_phase = Phase::SigningIn;
    m_timeoutTimer->start(kNunchukSigninTimeoutMs);
    sendAuthorizationCodeToNunchuk(authorizationCode);
}

void QGoogleSigninView::sendAuthorizationCodeToNunchuk(
    const QString &authorizationCode)
{
    QNetworkRequest request(kNunchukDesktopSigninUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QStringLiteral("application/json"));
    request.setRawHeader("Accept", "application/json");

    const QJsonObject payload{
        {QStringLiteral("code"), authorizationCode},
        {QStringLiteral("code_verifier"), m_codeVerifier},
        {QStringLiteral("redirect_uri"), m_redirectUri}
    };
    m_nunchukReply = m_networkManager->post(
        request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    connect(m_nunchukReply, &QNetworkReply::finished,
            this, &QGoogleSigninView::handleNunchukReply);
}

void QGoogleSigninView::handleNunchukReply()
{
    QNetworkReply *reply = m_nunchukReply.data();
    if (!reply) {
        return;
    }
    m_nunchukReply.clear();

    const QNetworkReply::NetworkError networkError = reply->error();
    const QString networkErrorText = reply->errorString();
    const int httpStatus = reply
                               ->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                               .toInt();
    const QByteArray body = reply->readAll();
    reply->deleteLater();

    if (m_phase != Phase::SigningIn) {
        return;
    }
    if (networkError != QNetworkReply::NoError) {
        failCurrentAttempt(nunchukFailureMessage(
            body, httpStatus, networkErrorText));
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        failCurrentAttempt(
            tr("Nunchuk returned an invalid Google sign-in response."));
        return;
    }

    const QJsonObject result = document.object();
    clearAttempt();
    emit loginSucceeded(result);
}

void QGoogleSigninView::failCurrentAttempt(const QString &message)
{
    if (m_phase == Phase::Idle) {
        return;
    }
    clearAttempt();
    emit loginFailed(message);
}

void QGoogleSigninView::clearAttempt()
{
    m_timeoutTimer->stop();

    if (m_nunchukReply) {
        disconnect(m_nunchukReply, nullptr, this, nullptr);
        m_nunchukReply->abort();
        m_nunchukReply->deleteLater();
        m_nunchukReply.clear();
    }
    m_replyHandler->close();

    m_codeVerifier.fill(QChar('\0'));
    m_codeVerifier.clear();
    m_state.clear();
    m_redirectUri.clear();
    m_phase = Phase::Idle;
}
