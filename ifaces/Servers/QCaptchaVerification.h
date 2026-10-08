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

#ifndef QCAPTCHAVERIFICATION_H
#define QCAPTCHAVERIFICATION_H

#include "FederatedAuth/QLoadingOverlay.h"
#include <QWebEngineView>
#include <QWebChannel>
#include <QPointer>

class VerificationBridge : public QObject {
    Q_OBJECT
public:
    using QObject::QObject;

public slots:
    void onMessage(const QString &json);
    void onToken(const QString &token);

signals:
    void tokenReceived(const QString &token);
};

class QCaptchaVerification : public QWebEngineView {
    Q_OBJECT

public:
    explicit QCaptchaVerification(QWidget *parent = nullptr);
    ~QCaptchaVerification() override = default;

    void startVerifyCaptcha();
    void safeClose();

signals:
    void verified(const QString &token);
    void cancelled();

private slots:
    void handleToken(const QString &token);
    void onLoadFinished(bool ok);

private:
    void injectWebChannel();
    void cleanup();

private:
    QWebEnginePage*     m_page = nullptr;
    QWebEngineProfile*  m_profile = nullptr;
    QWebChannel*        m_channel = nullptr;
    VerificationBridge* m_bridge = nullptr;
    QLoadingOverlay*    m_loadingOverlay = nullptr;
    bool m_tokenReceived = false;
    bool m_channelInjected = false;
    bool m_closing = false;
};

#endif // QCAPTCHAVERIFICATION_H
