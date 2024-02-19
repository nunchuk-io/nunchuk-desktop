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
#include "STATE_ID_SCR_ADD_WALLET_CONFIRMATION.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include <QClipboard>
#include "localization/STR_CPP.h"
#include "Chats/ClientController.h"

void SCR_ADD_WALLET_CONFIRMATION_Entry(QVariant msg) {
    if(AppModel::instance()->newWalletInfo()){
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
    }
}

void SCR_ADD_WALLET_CONFIRMATION_Exit(QVariant msg) {

}

void EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_ADD_WALLET_CONFIRM_CREATE_HANDLER(QVariant msg) {
    if((int)Wallet::CreationMode::CREATE_NEW_WALLET == AppModel::instance()->newWalletInfoPtr().data()->getCreationMode()){
        AppModel::instance()->startCreateWallet(false, "");
    }
}

void EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST_HANDLER(QVariant msg) {
    int signer_index = msg.toInt();
    if(signer_index >= 0 && AppModel::instance()->newWalletInfo() && AppModel::instance()->newWalletInfo()->singleSignersAssigned()){
        QSingleSignerPtr it = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->getSingleSignerByIndex(signer_index);
        if(it){
            QString master_signer_id = it.data()->masterSignerId();
            int     signer_type = it.data()->signerType();
            QWarningMessage warningmsg;
            bridge::nunchukCacheMasterSignerXPub(master_signer_id, warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                if(((int)ENUNCHUCK::SignerType::SOFTWARE == signer_type)
                        || (int)ENUNCHUCK::SignerType::HARDWARE == signer_type
                        || (int)ENUNCHUCK::SignerType::NFC == signer_type)
                {
                    int numberSignerRequired = AppModel::instance()->newWalletInfo()->n();
                    bool escrow = AppModel::instance()->newWalletInfo()->escrow();
                    ENUNCHUCK::WalletType walletType =  escrow ? ENUNCHUCK::WalletType::ESCROW :
                                                                 numberSignerRequired > 1 ? ENUNCHUCK::WalletType::MULTI_SIG
                                                                                          : ENUNCHUCK::WalletType::SINGLE_SIG;
                    ENUNCHUCK::AddressType addressType = (ENUNCHUCK::AddressType)AppModel::instance()->newWalletInfo()->addressType().toInt();
                    warningmsg.resetWarningMessage();
                    QSingleSignerPtr signer{nullptr};
                    if (ClientController::instance()->isSubscribed() && (int)ENUNCHUCK::SignerType::NFC == signer_type) {
                        signer = bridge::nunchukGetDefaultSignerFromMasterSigner(master_signer_id,
                                                                                 walletType,
                                                                                 addressType,
                                                                                 warningmsg);
                    }
                    else {
                        signer = bridge::nunchukGetUnusedSignerFromMasterSigner(master_signer_id,
                                                                                walletType,
                                                                                addressType,
                                                                                warningmsg);
                    }

                    if(signer && warningmsg.type() == (int)EWARNING::WarningType::NONE_MSG){
                        AppModel::instance()->newWalletInfo()->singleSignersAssigned()->replaceSingleSigner(msg.toInt(), signer);
                        AppModel::instance()->showToast(0, STR_CPP_097, EWARNING::WarningType::SUCCESS_MSG);
                    }
                    else{
                        it.data()->setNeedTopUpXpub(true);
                        AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
                    }
                }
            }
            else{
                AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
            }
        }
    }
    if(AppModel::instance()->newWalletInfo()){
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
    }
    emit AppModel::instance()->finishGenerateSigner();
}

void EVT_ADD_WALLET_SUCCESSFULLY_HANDLER(QVariant msg) {

}

void EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR_HANDLER(QVariant msg) {
    if((int)Wallet::CreationMode::CREATE_NEW_WALLET == AppModel::instance()->newWalletInfoPtr().data()->getCreationMode()){
        QString file_path = qUtils::QGetFilePath(msg.toString());
        AppModel::instance()->startCreateWallet(true, file_path);
    }
}

void EVT_ADD_WALLET_GENERATE_SIGNER_HANDLER(QVariant msg) {

}

void EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR_HANDLER(QVariant msg) {
    if(QQuickViewer::instance()->onsRequester() != E::STATE_ID_SCR_ADD_WALLET){
        QWarningMessage msgWarning;
        QString ret = bridge::nunchukDraftWallet(AppModel::instance()->newWalletInfo()->name(),
                                                  AppModel::instance()->newWalletInfo()->m(),
                                                  AppModel::instance()->newWalletInfo()->n(),
                                                  AppModel::instance()->newWalletInfo()->singleSignersAssigned(),
                                                  (ENUNCHUCK::AddressType)AppModel::instance()->newWalletInfo()->addressType().toInt(),
                                                  AppModel::instance()->newWalletInfo()->escrow(),
                                                  AppModel::instance()->newWalletInfo()->description(),
                                                  msgWarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
            AppModel::instance()->newWalletInfo()->setDescriptior(ret);
        }
        else{
            AppModel::instance()->newWalletInfo()->setDescriptior(QString("[ERROR][%1]").arg(msgWarning.contentDisplay()));
        }
    }
}

void EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR_HANDLER(QVariant msg) {
    if(!msg.toString().isEmpty()){
        qApp->clipboard()->setText(msg.toString());
    }
}

