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


#if ENABLE_WEBVIEW_SIGIN
#include "QLoadingOverlay.h"
#include "QOutlog.h"
#include <QWebEngineView>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineUrlRequestInfo>
#include <QJsonObject>

class QInsecureRequestBlocker : public QWebEngineUrlRequestInterceptor {
public:
    explicit QInsecureRequestBlocker(QObject* parent = nullptr);
    void interceptRequest(QWebEngineUrlRequestInfo &info) override;
};

class QAppleSigninView : public QWebEngineView {
    Q_OBJECT
public:
    explicit QAppleSigninView(QWidget *parent = nullptr);
    void startSignin();

signals:
    void loginSucceeded(QJsonObject data);

private slots:
    void handleUrlChanged(const QUrl &url);
    void handleLoadFinished(bool success);

private:
    QWebEnginePage*    m_page;
    QWebEngineProfile* m_profile;
    QLoadingOverlay*   m_loadingOverlay;
};

typedef OurSharedPointer<QAppleSigninView> QAppleSigninViewPtr;
#endif

#endif // QAPPLESIGNINVIEW_H
