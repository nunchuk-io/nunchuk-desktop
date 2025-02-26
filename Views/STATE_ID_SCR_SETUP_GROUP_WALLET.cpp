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


#include "STATE_ID_SCR_SETUP_GROUP_WALLET.h"
#include "Premiums/QSharedWallets.h"
#include "Premiums/GroupSandboxModel.h"
#include "AppModel.h"
#include "Premiums/QSignerManagement.h"
#include "bridgeifaces.h"
#include "WalletModel.h"

void SCR_SETUP_GROUP_WALLET_Entry(QVariant msg) {

}

void SCR_SETUP_GROUP_WALLET_Exit(QVariant msg) {

}

void EVT_SETUP_GROUP_WALLET_ENTER_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << maps;
    if (qUtils::strCompare(type, "finalize-group-sandbox")) {
        DBG_INFO << type << AppModel::instance()->newWalletInfoPtr();
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            DBG_INFO << type << w->groupSandboxPtr();
            if (auto sandbox = w->groupSandboxPtr()) {
                sandbox->FinalizeGroup();
            }
        }
    }
    else if (type == "review-group-sandbox") {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                if (sandbox->groupId().isEmpty()) {
                    QWarningMessage msg;
                    auto w = bridge::nunchukImportWalletDescriptor(sandbox->filePathRecovery(), sandbox->groupName(), "", msg);
                    if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
                        AppModel::instance()->startReloadUserDb();
                        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                        QString msg_name = QString("%1 has been successfully recovered").arg(sandbox->groupName());
                        AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                        timeoutHandler(1000,[w](){
                            int lastIndex = AppModel::instance()->walletList()->getWalletIndexById(w->walletId());
                            lastIndex = max(0, lastIndex);
                            AppModel::instance()->setWalletListCurrentIndex(lastIndex, true);
                        });
                    } else {
                        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
                    }
                } else {
                    sandbox->setScreenFlow("review-wallet");
                }
            }
        }
    }
    else if (type == "register-wallet-on-hardware") {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                sandbox->setScreenFlow("register-wallet-hardware");
            }
        }
    }
    else if (type == "setting-group-sandbox") {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                sandbox->setScreenFlow("group-sandbox-setting");
            }
        }
    }
    else if (type == "setting-sandbox-close") {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                sandbox->setScreenFlow("setup-group-wallet");
            }
        }
    }
    else if (type == "update-group-wallet") {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                QString walletNameInputted = maps["walletNameInputted"].toString();
                int walletM = maps["walletM"].toInt();
                int walletN = maps["walletN"].toInt();
                int  addressType = maps["addressType"].toInt();
                w->setWalletName(walletNameInputted);
                if (!w->walletId().isEmpty()) {
                    QWarningMessage msg;
                    bridge::UpdateWallet(w->nunchukWallet(), msg);
                }
                sandbox->setGroupName(walletNameInputted);
                if (!sandbox->groupId().isEmpty()) {
                    sandbox->UpdateGroup(walletNameInputted, walletM, walletN, addressType);
                }
                sandbox->setScreenFlow("setup-group-wallet");
            }
        }
    }
    else if (type == "group-sandbox-add-existing-key") {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                QString xfp = maps["xfp"].toString();
                QString name = maps["keyName"].toString();
                auto master = AppModel::instance()->masterSignerListPtr()->getMasterSignerByXfpName(xfp, name);
                if (master) {
                    AppModel::instance()->setMasterSignerInfo(master);
                    QSignerManagement::instance()->finishCreateMasterSigner();
                } else {
                    auto single = AppModel::instance()->remoteSignerListPtr()->getSingleSignerByFingerPrint(xfp, name);
                    AppModel::instance()->setSingleSignerInfo(single);
                    QSignerManagement::instance()->finishCreateRemoteSigner();
                }
                sandbox->setScreenFlow("setup-group-wallet");
            }
        }
    } else if (type == "export-config-as-a-file") {
        QString file = maps["file"].toString();
        QString file_path = qUtils::QGetFilePath(file);
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                sandbox->ExportWalletViaBSMS(file_path);
            }
        }
    } else if (type == "export-config-as-QrCodeBCUR2_Legacy") {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                sandbox->ExportWalletViaQRBCUR2Legacy();
            }
        }
    } else if (type == "export-config-as-QrCode-BCUR2") {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                sandbox->ExportWalletViaQRBCUR2();
            }
        }
    }
}

void EVT_SETUP_GROUP_WALLET_BACK_HANDLER(QVariant msg) {

}

