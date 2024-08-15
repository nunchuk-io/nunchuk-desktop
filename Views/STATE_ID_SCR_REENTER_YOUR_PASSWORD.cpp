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
#include "QEventProcessor.h"
#include "Servers/Draco.h"
#include "AppModel.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QKeyRecovery.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QGroupWallets.h"

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
    QString wallet_id = passWordObject.toMap().value("wallet_id").toString();
    DBG_INFO << state_id << wallet_id;
    QWalletPtr wallet {};
    if (!wallet_id.isEmpty()) {
        wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
    }
    switch (state_id) {
    case E::STATE_ID_SCR_KEY_RECOVERY:
        if (ServiceSetting::instance()->servicesTagPtr()->requestRecoverKeyVerifyPassword(password)) {
            ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->CreateTapsigners();
            QEventProcessor::instance()->sendEvent(E::EVT_KEY_RECOVERY_REQUEST);
        }
        break;
    case E::STATE_ID_SCR_SETUP_SECURITY_QUESTION:
        if (ServiceSetting::instance()->servicesTagPtr()->list2FA().size() > 0) {
            QString w_id = ServiceSetting::instance()->servicesTagPtr()->list2FA().first();
            if (auto w = AppModel::instance()->walletList()->getWalletById(w_id)) {
                ServiceSetting::instance()->setWalletInfo(w);
                w->setFlow((int) AlertEnum::E_Alert_t::SERVICE_TAG_UPDATE_SECURITY_QUESTION);
            }
        }
        if (ServiceSetting::instance()->servicesTagPtr()->requestUpdateSecurityQuestionPassword(password)) {
            ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->setRequireQuestions({});
            ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->CreateAllSecurityQuestions();
            QEventProcessor::instance()->sendEvent(E::EVT_SETUP_SECURITY_QUESTION_REQUEST);
        }
        break;
    case E::STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD:
        ServiceSetting::instance()->setWalletInfo(wallet);
        if (ServiceSetting::instance()->servicesTagPtr()->requestLockDownVerifyPassword(password)) {
            QEventProcessor::instance()->sendEvent(E::EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST);
        }
        break;
    case E::STATE_ID_SCR_SERVICE_SETTINGS:
        ServiceSetting::instance()->setWalletInfo(wallet);
        if (ServiceSetting::instance()->servicesTagPtr()->requestServerKeyVerifyPassword(password)) {
            if (wallet && wallet->serverKeyPtr()) {
                QtConcurrent::run([wallet]() {
                    wallet->serverKeyPtr()->serverKeyGetCurrentPolicies();
                });
            }
            QTimer::singleShot(100, [ = ]() {
                QEventProcessor::instance()->sendEvent(E::EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST);
            });
        }
        break;
    case E::STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN: {
        ServiceSetting::instance()->setWalletInfo(wallet);
        if (ServiceSetting::instance()->servicesTagPtr()->requestInheritancePlanVerifyPassword(password)) {
            if (wallet && wallet->inheritancePlanPtr()) {
                QtConcurrent::run([wallet]() {
                    wallet->inheritancePlanPtr()->GetInheritancePlan();
                });
            }
            QTimer::singleShot(100, [ = ]() {
                QEventProcessor::instance()->sendEvent(E::EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST);
            });
        }
    }
    break;
    case E::STATE_ID_SCR_WALLET_INFO: {
        if (ServiceSetting::instance()->servicesTagPtr()->requestDeleteWalletVerifyPassword(password)) {
            if (wallet && wallet->DeleteWalletRequiredSignatures()) {
                AppModel::instance()->walletInfoPtr()->setIsDeleting(wallet->isDeleting());
            }
            QEventProcessor::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_BACK);
        }
    }
    break;
    case E::STATE_ID_SCR_CHANGE_EMAIL: {
        if (ServiceSetting::instance()->servicesTagPtr()->requestChangeEmailVerifyPassword(password)) {
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
            QEventProcessor::instance()->sendEvent(E::EVT_CHANGE_EMAIL_REQUEST);
        }
    }
    break;
    case E::STATE_ID_SCR_DASHBOARD_ALERT_INFO: {
        if (ServiceSetting::instance()->servicesTagPtr()->requestReplaceKeysVerifyPassword(password)) {
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
            QEventProcessor::instance()->sendEvent(E::EVT_REPLACE_SELECT_KEY_REQUEST);
        }
    }
    break;
    case E::STATE_ID_SCR_EDIT_MEMBERS: {
        if (ServiceSetting::instance()->servicesTagPtr()->requestEditMemberVerifyPassword(password)) {
            QEventProcessor::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_BACK);
            if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
                dashboard->CalculateRequireSignaturesForEditingMembers();
            }
        }
    }
    break;
    default:
        break;
    }
}


void EVT_REENTER_YOUR_PASSWORD_BACK_HANDLER(QVariant msg) {

}
