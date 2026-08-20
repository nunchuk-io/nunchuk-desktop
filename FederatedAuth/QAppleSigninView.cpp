/**************************************************************************
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

#include "QAppleSigninView.h"

#include "qUtils.h"

#include <QDesktopServices>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QRandomGenerator>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QUrlQuery>

namespace {

constexpr quint16 kCallbackPort = 3001;
constexpr int kSigninTimeoutMs = 3 * 60 * 1000;
constexpr int kMaximumHeaderBytes = 16 * 1024;
constexpr int kMaximumBodyBytes = 64 * 1024;

const char kDefaultClientId[] = "io.nunchuk.signin";
const char kDefaultRedirectUri[] =
    "https://api.nunchuk.io/v1.1/passport/apple/signin_callback";
const char kAuthorizeEndpoint[] = "https://appleid.apple.com/auth/authorize";

const QByteArray kSuccessHtml =
    "<!doctype html><html><head><meta charset=\"utf-8\"><title>Sign-in complete</title>"
    "<script>history.replaceState(null, document.title, location.pathname);window.close();</script>"
    "</head><body><h2>Sign-in complete</h2>"
    "<p>You can close this tab and return to Nunchuk.</p></body></html>";
const QByteArray kFailureHtml =
    "<!doctype html><html><head><meta charset=\"utf-8\"><title>Sign-in failed</title>"
    "</head><body><h2>Sign-in failed</h2>"
    "<p>Return to Nunchuk and try again.</p></body></html>";
const QByteArray kInvalidRequestHtml =
    "<!doctype html><html><head><meta charset=\"utf-8\"><title>Invalid callback</title>"
    "</head><body><h2>Invalid callback request</h2></body></html>";

QString environmentValueOrDefault(const char *name, const char *defaultValue)
{
    const QString value = qEnvironmentVariable(name).trimmed();
    return value.isEmpty() ? QString::fromLatin1(defaultValue) : value;
}

QHash<QByteArray, QByteArray> parseHeaders(const QList<QByteArray> &lines,
                                           bool *ok)
{
    QHash<QByteArray, QByteArray> headers;
    *ok = true;

    for (const QByteArray &line : lines) {
        if (line.isEmpty()) {
            continue;
        }

        const int separator = line.indexOf(':');
        if (separator <= 0) {
            *ok = false;
            return {};
        }

        const QByteArray name = line.left(separator).trimmed().toLower();
        const QByteArray value = line.mid(separator + 1).trimmed();
        if (name.isEmpty() || headers.contains(name)) {
            *ok = false;
            return {};
        }
        headers.insert(name, value);
    }

    return headers;
}

QString decodeFormComponent(QByteArray value)
{
    value.replace('+', ' ');
    return QUrl::fromPercentEncoding(value);
}

bool extractResponse(const QByteArray &body, QString *response)
{
    bool found = false;
    for (const QByteArray &part : body.split('&')) {
        const int separator = part.indexOf('=');
        const QByteArray encodedName =
            separator < 0 ? part : part.left(separator);
        if (decodeFormComponent(encodedName) != QStringLiteral("response")) {
            continue;
        }

        if (found || separator < 0) {
            return false;
        }
        *response = decodeFormComponent(part.mid(separator + 1));
        found = true;
    }
    return found && !response->isEmpty();
}

} // namespace

QAppleSigninView::QAppleSigninView(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
    , m_timeoutTimer(new QTimer(this))
{
    m_timeoutTimer->setSingleShot(true);
    connect(m_server, &QTcpServer::newConnection,
            this, &QAppleSigninView::onNewConnection);
    connect(m_timeoutTimer, &QTimer::timeout,
            this, &QAppleSigninView::onTimeout);
}

QString QAppleSigninView::generateAttemptToken() const
{
    QByteArray randomBytes;
    randomBytes.reserve(32);
    QRandomGenerator *generator = QRandomGenerator::system();
    for (int i = 0; i < 8; ++i) {
        const quint32 value = generator->generate();
        randomBytes.append(static_cast<char>((value >> 24) & 0xff));
        randomBytes.append(static_cast<char>((value >> 16) & 0xff));
        randomBytes.append(static_cast<char>((value >> 8) & 0xff));
        randomBytes.append(static_cast<char>(value & 0xff));
    }
    return QString::fromLatin1(
        randomBytes.toBase64(QByteArray::Base64UrlEncoding |
                             QByteArray::OmitTrailingEquals));
}

QUrl QAppleSigninView::buildAuthUrl(const QString &clientId,
                                    const QUrl &redirectUri) const
{
    QUrl callbackUrl(QStringLiteral("http://localhost:3001/callback"));
    QUrlQuery callbackQuery;
    callbackQuery.addQueryItem(QStringLiteral("attempt"), m_attemptToken);
    callbackUrl.setQuery(callbackQuery);

    // Keep these legacy fields intact because the backend decodes `state` and
    // relays its form-POST result to `ru`.
    QJsonObject stateObject{
        {QStringLiteral("id"), qUtils::deviceId()},
        {QStringLiteral("platform"), QStringLiteral("PC")},
        {QStringLiteral("ru"), callbackUrl.toString(QUrl::FullyEncoded)}
    };
    const QString encodedState = QString::fromLatin1(
        QJsonDocument(stateObject).toJson(QJsonDocument::Compact)
            .toBase64(QByteArray::Base64UrlEncoding));

    QUrl authUrl(QString::fromLatin1(kAuthorizeEndpoint));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("client_id"), clientId);
    query.addQueryItem(QStringLiteral("redirect_uri"),
                       redirectUri.toString());
    query.addQueryItem(QStringLiteral("scope"), QStringLiteral("name email"));
    query.addQueryItem(QStringLiteral("response_mode"), QStringLiteral("form_post"));
    query.addQueryItem(QStringLiteral("response_type"), QStringLiteral("code"));
    query.addQueryItem(QStringLiteral("state"), encodedState);
    authUrl.setQuery(query);
    return authUrl;
}

void QAppleSigninView::startSignin()
{
    if (m_active) {
        emit loginFailed(QStringLiteral("Apple sign-in is already in progress."));
        return;
    }

    const QString clientId = environmentValueOrDefault(
        "APPLE_OAUTH_CLIENT_ID", kDefaultClientId);
    const QUrl redirectUri(environmentValueOrDefault(
        "APPLE_OAUTH_REDIRECT_URI", kDefaultRedirectUri));
    if (clientId.isEmpty() || !redirectUri.isValid() ||
        redirectUri.scheme().compare(QStringLiteral("https"), Qt::CaseInsensitive) != 0 ||
        redirectUri.host().isEmpty()) {
        emit loginFailed(QStringLiteral("Apple sign-in configuration is invalid."));
        return;
    }

    m_attemptToken = generateAttemptToken();
    if (!m_server->listen(QHostAddress::LocalHost, kCallbackPort)) {
        m_attemptToken.clear();
        emit loginFailed(QStringLiteral("Unable to start the Apple sign-in callback service."));
        return;
    }

    m_active = true;
    m_timeoutTimer->start(kSigninTimeoutMs);
    const QUrl authUrl = buildAuthUrl(clientId, redirectUri);
    if (!QDesktopServices::openUrl(authUrl)) {
        resetAttempt();
        emit loginFailed(QStringLiteral("Unable to open Apple sign-in in the system browser."));
    }
}

void QAppleSigninView::cancelSignin(bool notifyUser)
{
    if (!m_active) {
        return;
    }
    resetAttempt();
    if (notifyUser) {
        emit loginFailed(QStringLiteral("Apple sign-in was cancelled."));
    }
}

void QAppleSigninView::onNewConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket *socket = m_server->nextPendingConnection();
        if (!socket) {
            continue;
        }

        connect(socket, &QTcpSocket::disconnected, this,
                [this, socket]() {
                    m_requestBuffers.remove(socket);
                    socket->deleteLater();
                });

        if (!m_active || !socket->peerAddress().isLoopback()) {
            rejectSocket(socket);
            continue;
        }

        m_requestBuffers.insert(socket, QByteArray());
        connect(socket, &QTcpSocket::readyRead, this,
                [this, socket]() { processSocket(socket); });

        if (socket->bytesAvailable() > 0) {
            processSocket(socket);
        }
    }
}

void QAppleSigninView::processSocket(QTcpSocket *socket)
{
    auto bufferIt = m_requestBuffers.find(socket);
    if (bufferIt == m_requestBuffers.end()) {
        rejectSocket(socket);
        return;
    }

    QByteArray &buffer = bufferIt.value();
    buffer.append(socket->readAll());
    if (buffer.size() > kMaximumHeaderBytes + 4 + kMaximumBodyBytes) {
        rejectSocket(socket);
        return;
    }

    const int headerEnd = buffer.indexOf("\r\n\r\n");
    if (headerEnd < 0) {
        if (buffer.size() > kMaximumHeaderBytes) {
            rejectSocket(socket);
        }
        return;
    }
    if (headerEnd > kMaximumHeaderBytes) {
        rejectSocket(socket);
        return;
    }

    QList<QByteArray> headerLines = buffer.left(headerEnd).split('\n');
    if (headerLines.isEmpty()) {
        rejectSocket(socket);
        return;
    }
    for (QByteArray &line : headerLines) {
        if (line.endsWith('\r')) {
            line.chop(1);
        }
    }

    QList<QByteArray> requestParts = headerLines.takeFirst().split(' ');
    requestParts.removeAll(QByteArray());
    if (requestParts.size() != 3 || requestParts.at(0) != "POST" ||
        requestParts.at(2) != "HTTP/1.1") {
        rejectSocket(socket);
        return;
    }

    const QUrl requestTarget = QUrl::fromEncoded(requestParts.at(1), QUrl::StrictMode);
    if (!requestTarget.isValid() || !requestTarget.isRelative() ||
        !requestTarget.scheme().isEmpty() || !requestTarget.host().isEmpty() ||
        requestTarget.hasFragment() ||
        requestTarget.path(QUrl::FullyEncoded) != QStringLiteral("/callback")) {
        rejectSocket(socket);
        return;
    }

    const auto callbackItems = QUrlQuery(requestTarget).queryItems(QUrl::FullyDecoded);
    if (callbackItems.size() != 1 ||
        callbackItems.first().first != QStringLiteral("attempt") ||
        callbackItems.first().second != m_attemptToken) {
        rejectSocket(socket);
        return;
    }

    bool headersOk = false;
    const QHash<QByteArray, QByteArray> headers = parseHeaders(headerLines, &headersOk);
    if (!headersOk || headers.value("host").toLower() != "localhost:3001" ||
        headers.contains("transfer-encoding")) {
        finishFailed(socket, QStringLiteral("Apple sign-in callback was invalid."));
        return;
    }

    const QByteArray contentType =
        headers.value("content-type").split(';').value(0).trimmed().toLower();
    if (contentType != "application/x-www-form-urlencoded") {
        finishFailed(socket, QStringLiteral("Apple sign-in callback was invalid."));
        return;
    }

    bool lengthOk = false;
    const qulonglong contentLength =
        headers.value("content-length").trimmed().toULongLong(&lengthOk);
    if (!lengthOk || contentLength == 0 || contentLength > kMaximumBodyBytes) {
        finishFailed(socket, QStringLiteral("Apple sign-in callback was invalid."));
        return;
    }

    const qint64 requestSize = headerEnd + 4 + static_cast<qint64>(contentLength);
    if (buffer.size() < requestSize) {
        return;
    }
    if (buffer.size() != requestSize) {
        finishFailed(socket, QStringLiteral("Apple sign-in callback was invalid."));
        return;
    }

    const QByteArray body = buffer.mid(headerEnd + 4,
                                       static_cast<int>(contentLength));
    QString encodedResponse;
    if (!extractResponse(body, &encodedResponse)) {
        finishFailed(socket, QStringLiteral("Apple sign-in returned an invalid response."));
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument document =
        QJsonDocument::fromJson(encodedResponse.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        finishFailed(socket, QStringLiteral("Apple sign-in returned an invalid response."));
        return;
    }

    const QJsonObject result = document.object();
    const QJsonObject error = result.value(QStringLiteral("error")).toObject();
    if (!error.contains(QStringLiteral("code")) ||
        error.value(QStringLiteral("code")).toInt(-1) != 0) {
        QString message = error.value(QStringLiteral("message")).toString().trimmed();
        if (message.isEmpty()) {
            message = QStringLiteral("Apple sign-in failed.");
        }
        finishFailed(socket, message);
        return;
    }

    finishSucceeded(socket, result);
}

void QAppleSigninView::rejectSocket(QTcpSocket *socket)
{
    if (!socket) {
        return;
    }
    m_requestBuffers.remove(socket);
    sendHtmlResponse(socket, 400, QByteArrayLiteral("Bad Request"),
                     kInvalidRequestHtml);
    socket->disconnectFromHost();
}

void QAppleSigninView::finishSucceeded(QTcpSocket *socket,
                                       const QJsonObject &result)
{
    m_requestBuffers.remove(socket);
    sendHtmlResponse(socket, 200, QByteArrayLiteral("OK"), kSuccessHtml);
    socket->disconnectFromHost();
    resetAttempt();
    emit loginSucceeded(result);
}

void QAppleSigninView::finishFailed(QTcpSocket *socket,
                                    const QString &message)
{
    m_requestBuffers.remove(socket);
    sendHtmlResponse(socket, 200, QByteArrayLiteral("OK"), kFailureHtml);
    socket->disconnectFromHost();
    resetAttempt();
    emit loginFailed(message);
}

void QAppleSigninView::onTimeout()
{
    if (!m_active) {
        return;
    }
    resetAttempt();
    emit loginFailed(QStringLiteral("Apple sign-in timed out."));
}

void QAppleSigninView::resetAttempt()
{
    m_timeoutTimer->stop();
    m_server->close();
    m_active = false;
    m_attemptToken.clear();

    const auto sockets = m_requestBuffers.keys();
    m_requestBuffers.clear();
    for (QTcpSocket *socket : sockets) {
        if (socket) {
            socket->disconnectFromHost();
        }
    }
}

void QAppleSigninView::sendHtmlResponse(QTcpSocket *socket, int statusCode,
                                        const QByteArray &reasonPhrase,
                                        const QByteArray &body) const
{
    if (!socket) {
        return;
    }

    QByteArray response;
    response.reserve(body.size() + 512);
    response.append("HTTP/1.1 ");
    response.append(QByteArray::number(statusCode));
    response.append(' ');
    response.append(reasonPhrase);
    response.append("\r\nContent-Type: text/html; charset=utf-8");
    response.append("\r\nContent-Length: ");
    response.append(QByteArray::number(body.size()));
    response.append("\r\nCache-Control: no-store, no-cache, max-age=0");
    response.append("\r\nPragma: no-cache");
    response.append("\r\nExpires: 0");
    response.append(
        "\r\nContent-Security-Policy: default-src 'none'; "
        "script-src 'sha256-WBSC4uAA5sdOZfJAdNXHpPYbUNF7BAszGneCOdlkVy8='");
    response.append("\r\nReferrer-Policy: no-referrer");
    response.append("\r\nX-Content-Type-Options: nosniff");
    response.append("\r\nConnection: close\r\n\r\n");
    response.append(body);
    socket->write(response);
    socket->flush();
}
