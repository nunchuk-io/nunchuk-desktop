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
#include "STATE_ID_SCR_REENTER_YOUR_PASSWORD.h"
#include "QQuickViewer.h"
#include "Draco.h"
#include "Chats/QUserWallets.h"
#include "ServiceSetting.h"

static QVariant passWordObject = QVariant();
void SCR_REENTER_YOUR_PASSWORD_Entry(QVariant msg) {
    passWordObject = msg;
}

void SCR_REENTER_YOUR_PASSWORD_Exit(QVariant msg) {
    passWordObject = QVariant();
}

void EVT_INPUT_PASSWORD_REQUEST_HANDLER(QVariant msg) {
    QString password = msg.toString();
    int state_id = passWordObject.toMap().value("state_id").toInt();
    DBG_INFO << state_id;
    switch (state_id) {
    case E::STATE_ID_SCR_KEY_RECOVERY:
        if (QUserWallets::instance()->requestRecoverKeyVerifyPassword(password)) {
            if (QUserWallets::instance()->createLockdownPeriods()) {
                QQuickViewer::instance()->sendEvent(E::EVT_KEY_RECOVERY_REQUEST);
            }
        }
        break;
    case E::STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD:
        if (QUserWallets::instance()->requestLockDownVerifyPassword(password)) {
            if (QUserWallets::instance()->createLockdownPeriods()) {
                QQuickViewer::instance()->sendEvent(E::EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST);
            }
        }
        break;
    case E::STATE_ID_SCR_SERVICE_SETTINGS:
        if (QUserWallets::instance()->requestServerKeyVerifyPassword(password)) {
            QStringList walletIds = AppModel::instance()->getUserWallets();
            QString m_wallet_id {};
            if (walletIds.size() > 0) {
                m_wallet_id = walletIds.first();
            } else {
                m_wallet_id = "";
            }
            if (walletIds.size() == 1) {
                emit QUserWallets::instance()->serverKeyVerifyPasswordAlert();
                QUserWallets::instance()->serverKeyGetCurrentPolicies(walletIds.first());
                QTimer::singleShot(100,[=](){
                    QQuickViewer::instance()->sendEvent(E::EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, E::STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE);
                });
            } else if (walletIds.size() > 1) {
                emit QUserWallets::instance()->serverKeyVerifyPasswordAlert();
                QQuickViewer::instance()->sendEvent(E::EVT_WALLET_CO_SIGN_POLICE_REQUEST);
            } else {
                emit QUserWallets::instance()->thereNoAssistedWalletAlert();
                QTimer::singleShot(100,[=](){
                    QQuickViewer::instance()->sendEvent(E::EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, E::STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE);
                });
            }
        }
        break;
    case E::STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN: {
        QString walletName = passWordObject.toMap().value("walletName").toString();
        if (QUserWallets::instance()->requestInheritancePlanVerifyPassword(password)) {
            ServiceSetting::instance()->setInheritanceWalletName(walletName);
            QQuickViewer::instance()->sendEvent(E::EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST);
        }
    }break;
    default:
        break;
    }
}

void EVT_REENTER_YOUR_PASSWORD_BACK_HANDLER(QVariant msg) {

}

void EVT_KEY_RECOVERY_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST_HANDLER(QVariant msg) {

}

void EVT_WALLET_CO_SIGN_POLICE_REQUEST_HANDLER(QVariant msg) {

}
