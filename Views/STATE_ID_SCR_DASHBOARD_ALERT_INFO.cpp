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


#include "STATE_ID_SCR_DASHBOARD_ALERT_INFO.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include <QJsonObject>
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QKeyRecovery.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QRecurringPayment.h"

void SCR_DASHBOARD_ALERT_INFO_Entry(QVariant msg) {

}

void SCR_DASHBOARD_ALERT_INFO_Exit(QVariant msg) {

}

void EVT_DASHBOARD_ALERT_INFO_ENTER_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    auto dashboard = QGroupWallets::instance()->dashboardInfoPtr();
    if (dashboard.isNull()) return;
    QJsonObject payload = dashboard->alertJson()["payload"].toObject();
    if (type == "continue-health-check") {
        QEventProcessor::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
        QString xfp = payload["xfp"].toString();
        if (auto dummy = dashboard->groupDummyTxPtr()) {
            dummy->setCurrentXfp(xfp);
        }
    }
    else if (type == "air-gapped-QR-code") {
        AppModel::instance()->setQrExported(QStringList());
        QString wallet_id = dashboard->wallet_id();
        if(wallet_id != ""){
            QWarningMessage msgwarning;
            QStringList qrtags = bridge::nunchukExportKeystoneWallet(wallet_id, msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
                AppModel::instance()->setQrExported(qrtags);
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
            }
        }
    }
    else if (type == "bitbox-export-wallet") {
        QString wallet_id = dashboard->wallet_id();
        QWalletPtr walletPtr = AppModel::instance()->walletListPtr()->getWalletById(wallet_id);
        if (walletPtr) {
            QStringList addrs = bridge::nunchukGetUnusedAddresses(wallet_id, false);
            if (addrs.size() > 0) {
                dashboard->setWalletunUsedAddress(addrs.at(0));
                AppModel::instance()->startDisplayAddress(wallet_id, addrs.at(0));
            }
        }

    }
    else if (type == "coldcard-export-wallet") {
        QString currentFile = maps["currentFile"].toString();
        QString file_path = qUtils::QGetFilePath(currentFile);
        QString wallet_id = dashboard->wallet_id();
        DBG_INFO << wallet_id << file_path;
        if(wallet_id != "" && file_path != "") {
            bool ret = bridge::nunchukExportWallet(wallet_id,
                                                    file_path,
                                                    nunchuk::ExportFormat::COLDCARD);
            if (ret) {
                if (dashboard->registerKeyDone()) {
                    dashboard->setConfigFlow("register-Done");
                } else {
                    dashboard->registerKeyNext();
                }
            }
            DBG_INFO << file_path << ret;
        }
    }
    else if (type == "air-gapped-QR-Done" || type == "bitbox-export-wallet-Done") {
        if (dashboard->registerKeyDone()) {
            dashboard->setConfigFlow("register-Done");
        } else {
            dashboard->registerKeyNext();
        }
    }
    else if (type == "inheritance-plan-approve") {
        if (auto w = AppModel::instance()->walletInfoPtr()) {
            if (auto plan = w->inheritancePlanPtr()) {
                if (plan->InheritancePlanningRequestApprove()) {
                    dashboard->GetAlertsInfo();
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                    QString msg_name = QString("Inheritance planning request approved");
                    AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                }
            }
        }
    }
    else if (type == "inheritance-plan-deny") {
        if (auto w = AppModel::instance()->walletInfoPtr()) {
            if (auto plan = w->inheritancePlanPtr()) {
                if (plan->InheritancePlanningRequestDeny()) {
                    dashboard->GetAlertsInfo();
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                    QString msg_name = QString("Inheritance planning request denied");
                    AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                }
            }
        }
    }
    else if (type == "enter-backup-password") {
        QString password = maps["password"].toString();
        if (ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->startRecovery(password)) {
            dashboard->setFlow((int)AlertEnum::E_Alert_t::KEY_RECOVERY_SUCCESS);
            QEventProcessor::instance()->sendEvent(E::EVT_DASHBOARD_ALERT_SUCCESS_REQUEST);
            if (ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->UserKeysMarkRecoverStatus())
            {
                dashboard->GetAlertsInfo();
            }
        }
    }
    else if (type == "cancel-health-check-pending") {
        if (dashboard->healthPtr()) {
            if (dashboard->healthPtr()->CancelHealthCheckPending()) {
                dashboard->GetAlertsInfo();
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                QString msg_cancel = "Health check request has been canceled";
                AppModel::instance()->showToast(0, msg_cancel, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
    }
    else if (type == "review-recurring-payment") {
        if (auto payment = dashboard->recurringPaymentPtr()) {
            payment->addFlow((int)PaymentEnum::Enum_t::VIEWING_RECURRING_PAYMENTS_ALERT);
            QEventProcessor::instance()->sendEvent(E::EVT_RECURRING_PAYMENTS_REQUEST, true);
        }
    }
    else if (type == "cancel-recurring-payment-cancelation-pending") {
        if (auto payment = dashboard->recurringPaymentPtr()) {
            if (payment->CancelPaymentCancellationPending()) {
                dashboard->GetAlertsInfo();
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                QString msg_cancel = "Pending cancellation has been cancelled";
                AppModel::instance()->showToast(0, msg_cancel, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
    }
    else if (type == "cancel-recovery-key") {
        if (dashboard->CancelRecoveryKey()) {
            dashboard->GetAlertsInfo();
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
        }
    }
    if (type == "cancel-security-question-update") {
        if (auto dummy = dashboard->groupDummyTxPtr()) {
            if (dummy->CancelDummyTransaction()) {
                dashboard->GetAlertsInfo();
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                QString msg_cancel = "Security questions has been canceled";
                AppModel::instance()->showToast(0, msg_cancel, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
    }
    else if (type == "continue-security-question-update") {
        QEventProcessor::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
    }
    else if (type == "select-period-history") {
        QString history_period_id = maps["history_period_id"].toString();
        if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
            dashboard->UpdateGroupChat(history_period_id);
        }
    }
    else if (type == "cancel-change-email") {
        if (auto dummy = dashboard->groupDummyTxPtr()) {
            if (dummy->CancelDummyTransaction()) {
                dashboard->GetAlertsInfo();
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                QString msg_cancel = "Email address change has been canceled.";
                AppModel::instance()->showToast(0, msg_cancel, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
    }
    else if (type == "continue-change-email") {
        QEventProcessor::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
    }
    else if (type == "skip-health-check") {
        if (dashboard->healthPtr()) {
            if (dashboard->healthPtr()->SkipKeyHealthReminder()) {
                dashboard->GetAlertsInfo();
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
            }
        }
    }
    else if (type == "cancel-key-replacement") {
        if (dashboard->CancelKeyReplacement()) {
            dashboard->GetAlertsInfo();
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
        }
    }
    else if (type == "continue-key-replacement") {
        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
        if (dashboard->canReplaceKey()) {
            QMap<QString, QVariant> data;
            data["state_id"] = E::STATE_ID_SCR_DASHBOARD_ALERT_INFO;
            data["wallet_id"] = dashboard->wallet_id();
            QEventProcessor::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_REQUEST, data);
        }
    }
    else {}
}

void EVT_ADD_HARDWARE_KEY_EXIST_REQ_HANDLER(QVariant msg) {

}

void EVT_ADD_HARDWARE_KEY_REQUEST_HANDLER(QVariant msg) {

}

