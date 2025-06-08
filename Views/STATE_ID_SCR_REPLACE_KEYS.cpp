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


#include "STATE_ID_SCR_REPLACE_KEYS.h"
#include "QOutlog.h"
#include "AppModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Premiums/QSharedWallets.h"
#include "Signers/QSignerManagement.h"

void SCR_REPLACE_KEYS_Entry(QVariant msg) {

}

void SCR_REPLACE_KEYS_Exit(QVariant msg) {

}

void EVT_REPLACE_KEYS_ENTER_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type << AppModel::instance()->newWalletInfoPtr().isNull();
    if (auto w = AppModel::instance()->newWalletInfoPtr()) {
        if (type == "switch-screen-flow") {
            QString screen_name = maps["screen_name"].toString();
            w->setReplaceFlow(screen_name);
        } else if (type == "select-replace-key") {
            w->MixMasterSignerAndSingleSignerAll();
            w->SelectKeyToReplace(maps["fingerXfp"].toString(), maps["indexXfp"].toInt());
            w->setReplaceFlow("replace-existing-key");
        } else if (type == "request-add-a-key") {
            QSignerManagement::instance()->requestCreateSignerFromReplaceScreen();
            QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_NEW_SIGNER_REQUEST);
            QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_USER_FREE);
        } else if (type == "replace-Key-Enter") {
            w->CreateANewWallet();
        } else if (type == "replace-cancel") {
            AppModel::instance()->setNewWalletInfo(NULL);
        } else if (type == "create-an-new-transaction") {
            QString rollover_address = w->unUseAddress();
            QEventProcessor::instance()->sendEvent(E::EVT_HOME_SEND_REQUEST, rollover_address);
        } else if (type == "replace-add-signer") {
            QString signerNameInputted = maps["signerNameInputted"].toString();
            int deviceIndexSelected    = maps["deviceIndexSelected"].toInt();
            DeviceListModel *deviceList = AppModel::instance()->newWalletInfoPtr()->deviceList();
            if (deviceList && AppModel::instance()->deviceList()) {
                QDevicePtr selectedDv = deviceList->getDeviceByIndex(deviceIndexSelected);
                if (selectedDv) {
                    int actualIndex = AppModel::instance()->deviceList()->getDeviceIndexByXfp(selectedDv->masterFingerPrint());
                    if (selectedDv.data()->needsPinSent() || selectedDv.data()->needsPassPhraseSent()) {
                        AppModel::instance()->showToast(0, STR_CPP_095, EWARNING::WarningType::WARNING_MSG);
                    } else {
                        if (signerNameInputted.isEmpty()) {
                            signerNameInputted = selectedDv->name();
                        }
                        AppModel::instance()->setAddSignerWizard(2);//eADD_LOADING: 2
                        DBG_INFO << signerNameInputted << actualIndex;
                        AppModel::instance()->startCreateMasterSigner(signerNameInputted, actualIndex);
                    }
                }
            }
        } else if (type == "import-coldcard-via-file") {
            QString file_path = maps["file_path"].toString();
            int new_index = maps["new_index"].toInt();
            w->ImportColdcardViaFile(file_path, new_index);
        } else if (type == "replace-transfer-funds") {
            if(auto old_w = AppModel::instance()->walletInfo()){
                // Check balance > 0
                DBG_INFO << old_w->walletId() << old_w->walletName() << old_w->balanceSats();
                if(old_w->balanceSats() > 0){
                    w->setReplaceFlow(type);
                }
                else {
                    QString new_wallet_id = w->walletId();
                    int index = AppModel::instance()->walletList()->getWalletIndexById(new_wallet_id);
                    if(-1 != index){
                        AppModel::instance()->setWalletListCurrentIndex(index);
                    }
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                }
            }
        } else if (type == "replace-select-keys-sandbox-wallet") {
            auto w = AppModel::instance()->walletInfoPtr();
            auto nw = AppModel::instance()->newWalletInfo();
            if(w && nw) {
                nw->setWalletName(w->walletName());
                nw->setWalletDescription(w->walletDescription());
                nw->setWalletAddressType(w->walletAddressType());
                if (auto sandbox = nw->groupSandboxPtr()) {
                    if (sandbox->CreateReplaceGroup(w->walletId())) {
                        sandbox->setScreenFlow("setup-group-wallet");
                        QJsonObject json;
                        json["type"] = "setup-group-wallet";
                        QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST, json);
                        QSharedWallets::instance()->GetAllGroups();
                    }
                }
            }
        } else {

        }
    }
}

