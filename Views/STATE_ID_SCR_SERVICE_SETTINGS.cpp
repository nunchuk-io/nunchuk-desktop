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

#include "STATE_ID_SCR_SERVICE_SETTINGS.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include "Chats/ClientController.h"
#include "ServiceSetting.h"
#include "QEventProcessor.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritancePlan.h"
#include "Servers/Draco.h"

void SCR_SERVICE_SETTINGS_Entry(QVariant msg) {
    ServiceSetting::instance()->setWalletInfo(NULL);
    QGroupWallets::instance()->findPermissionAccount();
    AppModel::instance()->setTabIndex((int)ENUNCHUCK::TabSelection::SERVICE_TAB);
    QStringList mnemonics = qUtils::GetBIP39WordList();
    AppModel::instance()->setSuggestMnemonics(mnemonics);
    ServiceSetting::instance()->clearWalletInfo();
    if(AppModel::instance()->walletList()){
        AppModel::instance()->walletList()->checkContainsGroup();
    }
    ServiceSetting::instance()->servicesTagPtr()->ConfigServiceTag();
}

void SCR_SERVICE_SETTINGS_Exit(QVariant msg) {
    ServiceSetting::instance()->clearWalletInfo();
}

void EVT_CLAIM_INHERITANCE_CHECK_REQUEST_HANDLER(QVariant msg)
{
#if 0
    Draco::instance()->inheritanceFakeUpdate();//FIXME need to remove
#else
    DBG_INFO << msg.toMap();
    QMap<QString,QVariant> maps = msg.toMap();
    QString magic = maps["magic"].toString();
    QString backupPassword = maps["backupPassword"].toString();
    QString backupPasswordTwo = maps["backupPasswordTwo"].toString();
    if (ServiceSetting::instance()->servicesTagPtr()->inheritanceCheck()) {
        ServiceSetting::instance()->servicesTagPtr()->inheritanceDownloadBackup(magic, backupPassword, backupPasswordTwo);
    }
#endif
}

void EVT_SERVICE_SUPPORT_REQUEST_HANDLER(QVariant msg) {
    CLIENT_INSTANCE->createSupportRoom();
}

void EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST_HANDLER(QVariant msg) {}

void EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST_HANDLER(QVariant msg)
{
    auto w = ServiceSetting::instance()->walletInfoPtr();
    if (w && w->serverKeyPtr()) {
        if (ServiceSetting::instance()->walletInfoPtr() && ServiceSetting::instance()->walletInfoPtr()->dashboard()) {
            ServiceSetting::instance()->walletInfoPtr()->dashboard()->setFlow((int)AlertEnum::E_Alert_t::SERVICE_TAG_POLICY_UPDATE);
        }
        w->serverKeyPtr()->ServerKeyRequiredSignature();
    }
}

void EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED_HANDLER(QVariant msg)
{
    if (ServiceSetting::instance()->servicesTagPtr()->secQuesAnswer()) {
        auto w = ServiceSetting::instance()->walletInfoPtr();
        if (w && w->serverKeyPtr()) {
            w->serverKeyPtr()->ServerKeyUpdatePoliciesSucceed();
        }
    }
}

void EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST_HANLDER(QVariant msg) {
    int action = msg.toInt();
    ServiceSetting::instance()->walletInfoPtr()->inheritancePlanPtr()->setActionPlan(msg.toInt());
    if (action == (int)ServiceSetting::InheritanceEdit::IE_BUFFER_PERIOD) {
        ServiceSetting::instance()->walletInfoPtr()->inheritancePlanPtr()->createPeriods();
    }
}

void EVT_SERVICE_SELECT_WALLET_REQUEST_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    if (type == "emergency-lockdown") {
        QString wallet_id = maps["wallet_id"].toString();
        if (auto wallet = AppModel::instance()->walletList()->getWalletById(wallet_id)) {
            QMap<QString, QVariant> data;
            data["state_id"] = E::STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD;
            data["wallet_id"] = wallet->walletId();
            QEventProcessor::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_REQUEST, data);
        }
    } else if (type == "wallet-lockdown") {
        if (ServiceSetting::instance()->servicesTagPtr()->createLockdownPeriods()) {
            DBG_INFO << type;
            QEventProcessor::instance()->sendEvent(E::EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST);
        }
    } else if (type == "platform-key-co-signing-policies") {
        QString wallet_id = maps["wallet_id"].toString();
        QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        if (!wallet.isNull()) {
            QMap<QString, QVariant> data;
            data["state_id"] = E::STATE_ID_SCR_SERVICE_SETTINGS;
            data["wallet_id"] = wallet->walletId();
            QEventProcessor::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_REQUEST, data);
        }
    } else if (type == "inheritance-planing") {
        QString wallet_id = maps["wallet_id"].toString();
        QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        if (!wallet.isNull()) {
            QMap<QString, QVariant> data;
            data["state_id"] = E::STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN;
            data["wallet_id"] = wallet->walletId();
            QEventProcessor::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_REQUEST, data);
        }
    }
}

void EVT_INHERITANCE_PLAN_FINALIZE_REQUEST_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString option = maps["option"].toString();
    if (option == "discard-change") {
        emit ServiceSetting::instance()->servicesTagPtr()->inheritanceDiscardChangeAlert();
    }
    else if(option == "finalize-change") {
        if (auto w = ServiceSetting::instance()->walletInfoPtr()) {
            if (auto plan = w->inheritancePlanPtr()) {
                if (plan->IsActived()) {
                    w->setFlow((int)AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_UPDATE);
                    plan->RequestInheritancePlanUpdate();
                } else {
                    w->setFlow((int)AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CREATE);
                    plan->RequestInheritancePlanCreate();
                }
            }
        }
    }
    else if(option == "security-question") {
        if (ServiceSetting::instance()->servicesTagPtr()->secQuesAnswer()) {
            if (auto w = ServiceSetting::instance()->walletInfoPtr()) {
                int flow = w->flow();
                switch ((AlertEnum::E_Alert_t)flow) {
                case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_UPDATE:
                    if (auto plan = w->inheritancePlanPtr()) {
                        plan->InheritancePlanUpdateSucceed();
                    }
                    break;
                case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CANCEL:
                    if (auto plan = w->inheritancePlanPtr()) {
                        plan->InheritancePlanCancelSucceed();
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    else if(option == "cancel-inheritance-plan") {
        if (auto w = ServiceSetting::instance()->walletInfoPtr()) {
            w->setFlow((int)AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CANCEL);
            if (auto plan = w->inheritancePlanPtr()) {
                plan->RequestInheritancePlanCancel();
            }
        }
    }
}


void EVT_KEY_RECOVERY_REQUEST_HANDLER(QVariant msg)
{

}

void EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST_HANDLER(QVariant msg)
{

}

void EVT_WALLET_CO_SIGN_POLICE_REQUEST_HANDLER(QVariant msg)
{

}

void EVT_LOCKDOWN_SUCCESS_REQUEST_HANDLER(QVariant msg)
{

}

void EVT_SETUP_SECURITY_QUESTION_REQUEST_HANDLER(QVariant msg) {

}
