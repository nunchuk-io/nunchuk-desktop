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


#include "STATE_ID_SCR_ONBOARDING.h"
#include "Models/AppModel.h"
#include "Models/OnBoardingModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QSharedWallets.h"
#include "Premiums/GroupSandboxModel.h"

void SCR_ONBOARDING_Entry(QVariant msg) {
    if (OnBoardingModel::instance()->screenFlow() == "seedPhrase") {
        if (auto w = AppModel::instance()->walletInfoPtr()) {
            DBG_INFO << w->walletId();
            QString mnemonic = bridge::GetHotWalletMnemonic(w->walletId(), "");
            AppModel::instance()->setMnemonic(mnemonic);
        }
    } else {
        AppModel::instance()->setMnemonic("");
        AppModel::instance()->setSuggestMnemonics(bridge::nunchuckGetBIP39WordList());
        OnBoardingModel::instance()->GetCountryCodeList();
        QGroupWallets::instance()->findPermissionAccount();
    }
}

void SCR_ONBOARDING_Exit(QVariant msg) {

}

void EVT_ONBOARDING_ACTION_REQUEST_HANDLER(QVariant msg) {
    QString type = msg.toMap()["type"].toString();
    DBG_INFO << type;
    if (type == "send-query") {
        QString country_code = msg.toMap()["country_code"].toString();
        QString email = msg.toMap()["email"].toString();
        QString note = msg.toMap()["note"].toString();
        if (OnBoardingModel::instance()->RequestOnboardingNoAdvisor(country_code, email, note)) {
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
        }
    }
    else if (type == "create-a-hot-wallet-now") {
        if (auto w = OnBoardingModel::instance()->CreateAHotWallet()) {
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
            timeoutHandler(1000, [w]() {
                int index = AppModel::instance()->walletListPtr()->getWalletIndexById(w->walletId());
                DBG_INFO << w->walletId() << index;
                if (index >= 0) {
                    AppModel::instance()->setWalletListCurrentIndex(index);
                }
            });
        }
    }
    else if (type == "create-an-account") {
        OnBoardingModel::instance()->setScreenFlow("create-account");
        QEventProcessor::instance()->sendEvent(E::EVT_STARTING_APPLICATION_ONLINEMODE);
    }
    else if (type == "sign-in-account") {
        OnBoardingModel::instance()->setScreenFlow("sign-in");
        QEventProcessor::instance()->sendEvent(E::EVT_STARTING_APPLICATION_ONLINEMODE);
    }
    else if (type == "create-new-wallet") {
        QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_WALLET_REQUEST);
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->setWalletOptType((int)ENUNCHUCK::WalletOptionType_t::E_PERSONAL_WALLET);
        }
    }
    else if (type == "create-new-group-wallet") {
        if (QSharedWallets::instance()->CheckGroupConfig()) {
            QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_WALLET_REQUEST);
            if (auto w = AppModel::instance()->newWalletInfoPtr()) {
                w->setWalletOptType((int)ENUNCHUCK::WalletOptionType_t::E_GROUP_WALLET);
            }
        }
    }
    else if (type == "recover-existing-wallet") {
        QString filePath = msg.toMap()["filePath"].toString();
        QString file_path = qUtils::QGetFilePath(filePath);
        QString recoverType = msg.toMap()["recoverType"].toString();
        if (recoverType == "recover-via-bsms-config-file" || recoverType == "recover-via-coldcard") {
            if(OnBoardingModel::instance()->ImportWalletDescriptor(file_path)) {
                OnBoardingModel::instance()->setScreenFlow("updateWalletName");
            }
        }
        else if (recoverType == "recover-sandbox-wallet") {
            QSharedWallets::instance()->RecoverSandboxWallet(file_path);
        }
        else if (recoverType == "import-db") {
            QWalletPtr wallet = OnBoardingModel::instance()->ImportWalletDB(file_path);
            timeoutHandler(1000, [wallet]() {
                if (wallet.isNull()) {
                    return;
                }
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                int index = AppModel::instance()->walletListPtr()->getWalletIndexById(wallet->walletId());
                DBG_INFO << wallet->walletId() << index;
                if (index >= 0) {
                    AppModel::instance()->setWalletListCurrentIndex(index);
                }
                QString msg = QString("%1 has been successfully recovered").arg(wallet->walletName());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            });
        }        
    }
    else if (type == "recover-hot-wallet") {
        QString mnemonic = msg.toMap()["mnemonic"].toString();
        DBG_INFO << mnemonic;
        bool checkmnemonic = qUtils::CheckMnemonic(mnemonic);
        if(checkmnemonic){
            if (auto wallet = OnBoardingModel::instance()->CreateAHotWallet(mnemonic, false)) {
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                timeoutHandler(1000, [wallet]() {
                    int index = AppModel::instance()->walletListPtr()->getWalletIndexById(wallet->walletId());
                    DBG_INFO << wallet->walletId() << index;
                    if (index >= 0) {
                        AppModel::instance()->setWalletListCurrentIndex(index);
                    }
                });
                QString msg = QString("%1 has been recovered.").arg(wallet->walletName());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        else{
            AppModel::instance()->setMnemonic("-101");
            AppModel::instance()->showToast(0, STR_CPP_081, EWARNING::WarningType::ERROR_MSG);
        }
    }
    else if (type == "enter-link-url" || type == "enter-link-qr-url") {
        QString sandboxUrl = msg.toMap()["sandboxUrl"].toString();
        DBG_INFO << sandboxUrl;
        AppModel::instance()->setNewWalletInfo(new Wallet());
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->setWalletOptType((int)ENUNCHUCK::WalletOptionType_t::E_GROUP_WALLET);
            if (auto sandbox = w->groupSandboxPtr()) {
                if (sandbox->JoinGroup(sandboxUrl)) {
                    sandbox->setScreenFlow("setup-group-wallet");
                    QJsonObject json;
                    json["type"] = "setup-group-wallet";
                    QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST, json);
                    QSharedWallets::instance()->GetAllGroups();
                }
            }
        }
    }
    else if (type == "sign-up") {
        QEventProcessor::instance()->sendEvent(E::EVT_STARTING_APPLICATION_ONLINEMODE);
    }
    else if (type == "update-new-wallet-name") {
        QString walletName = msg.toMap()["new_wallet_name"].toString();
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            QWarningMessage msg;
            nunchuk::Wallet wallet_result = w->nunchukWallet();
            wallet_result.set_name(walletName.toStdString());
            bridge::nunchukCreateWallet(wallet_result, true, msg);
            if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                AppModel::instance()->startReloadUserDb();
                QString msg = QString("%1 has been successfully recovered").arg(walletName);
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
                return;
            }
            else {
                AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
            }            
        }
    }
}

void EVT_ONBOARDING_CLOSE_HANDLER(QVariant msg) {

}

