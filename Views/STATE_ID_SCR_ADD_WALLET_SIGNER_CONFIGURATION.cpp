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
#include "STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Chats/ClientController.h"

void SCR_ADD_WALLET_SIGNER_CONFIGURATION_Entry(QVariant msg) {
    Q_UNUSED(msg);
}

void SCR_ADD_WALLET_SIGNER_CONFIGURATION_Exit(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER_HANDLER(QVariant msg) {
    if( AppModel::instance()->masterSignerList() ){
        for (int i = 0; i < AppModel::instance()->masterSignerList()->rowCount(); i++) {
            QMasterSignerPtr ret = AppModel::instance()->masterSignerList()->getMasterSignerByIndex(i);
            if(ret && ret.data()->checked()){
                AppModel::instance()->newWalletInfo()->singleSignersAssigned()->addSingleSigner(ret->cloneSingleSigner());
                AppModel::instance()->newWalletInfo()->singleSignersAssigned()->signers();
            }
        }
        AppModel::instance()->newWalletInfo()->setWalletN(AppModel::instance()->newWalletInfo()->singleSignersAssigned()->rowCount());
        emit AppModel::instance()->newWalletInfo()->walletChanged();
    }
}

void EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER_HANDLER(QVariant msg) {
    auto nw = AppModel::instance()->newWalletInfoPtr();
    if (nw) {
        if (nw->walletAddressType() == (int)nunchuk::AddressType::TAPROOT) {
            std::vector<nunchuk::SingleSigner> signerList;
            nunchuk::WalletType walletType =  nw->walletN() > 1 ? nunchuk::WalletType::MULTI_SIG : nunchuk::WalletType::SINGLE_SIG;
            nunchuk::AddressType addressType = (nunchuk::AddressType)nw->walletAddressType();
            QWarningMessage msg;
            for (QSingleSignerPtr p : nw->assignAvailableSigners()->fullList()) {
                if (p->checked()) {
                    auto signer = bridge::nunchukGetUnusedSignerFromMasterSigner(p->masterSignerId(),
                                                                            (ENUNCHUCK::WalletType)walletType,
                                                                            (ENUNCHUCK::AddressType)addressType,
                                                                            msg);
                    signerList.push_back(signer->originSingleSigner());
                }
            }
            nw->CreateSignerListReviewWallet(signerList);
        }
        else {
            if(AppModel::instance()->remoteSignerList()){
                for (int i = 0; i < AppModel::instance()->remoteSignerList()->rowCount(); i++) {
                    QSingleSignerPtr signer = AppModel::instance()->remoteSignerList()->getSingleSignerByIndex(i);
                    if(signer && signer.data()->checked()){
                        signer.data()->setSignerType((int)signer.data()->signerType());
                        nw->singleSignersAssigned()->addSingleSigner(signer);
                        nw->singleSignersAssigned()->signers();
                    }
                }
            }
        }
        nw->setWalletN(nw->singleSignersAssigned()->rowCount());
    }
}

void EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER_HANDLER(QVariant msg) {
    int signerIndex = msg.toInt();
    QSingleSignerPtr signer = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->getSingleSignerByIndex(signerIndex);
    if(signer){
        QString xfp = signer.data()->masterFingerPrint();
        QString master_signer_id = signer.data()->masterSignerId();
        AppModel::instance()->masterSignerList()->setUserCheckedByFingerprint(false, xfp);
        AppModel::instance()->remoteSignerList()->setUserCheckedByFingerprint(false, xfp);
        if(signer->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE){
            QWarningMessage warningmsg;
            bridge::nunchukClearSignerPassphrase(master_signer_id, warningmsg);
        }
    }
    AppModel::instance()->newWalletInfo()->singleSignersAssigned()->removeSingleSignerByIndex(signerIndex);
    int wallet_n = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->rowCount();
    AppModel::instance()->newWalletInfo()->setWalletN(wallet_n);
    if(AppModel::instance()->newWalletInfo()->walletM() > wallet_n){
        AppModel::instance()->newWalletInfo()->setWalletM(wallet_n);
    }
    emit AppModel::instance()->newWalletInfo()->walletChanged();
}

void EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK_HANDLER(QVariant msg) {

}

void EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW_HANDLER(QVariant msg) {

}

void EVT_ADD_SIGNER_TO_WALLET_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE_HANDLER(QVariant msg) {
    if(AppModel::instance()->newWalletInfo()){
        QMap<QString, QVariant> passPhraseData;
        passPhraseData["state_id"] = E::STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION;
        passPhraseData["mastersigner_id"] = msg.toMap().value("mastersigner_id");
        passPhraseData["mastersigner_index"] = msg.toMap().value("mastersigner_index");
        QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
    }
}

void EVT_SIGNER_CONFIGURATION_TRY_REVIEW_HANDLER(QVariant msg) {
    DBG_INFO << msg;
    if(AppModel::instance()->newWalletInfo() && AppModel::instance()->newWalletInfo()->singleSignersAssigned()){
        int numberSignerRequired = AppModel::instance()->newWalletInfo()->walletN();
        nunchuk::WalletType walletType =  numberSignerRequired > 1 ? nunchuk::WalletType::MULTI_SIG : nunchuk::WalletType::SINGLE_SIG;
        AppModel::instance()->newWalletInfo()->setWalletType((int)walletType);
        DBG_INFO << "CREATE" << (int)walletType << AppModel::instance()->newWalletInfo()->walletType();
        nunchuk::AddressType addressType = (nunchuk::AddressType)AppModel::instance()->newWalletInfo()->walletAddressType();
        for (int i = 0; i < AppModel::instance()->newWalletInfo()->singleSignersAssigned()->rowCount(); i++) {
            QSingleSignerPtr it = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->getSingleSignerByIndex(i);
            int signerType = it.data()->signerType();
            if((it && (int)nunchuk::SignerType::SOFTWARE == signerType) || ((int)nunchuk::SignerType::HARDWARE == signerType)|| ((int)nunchuk::SignerType::NFC == signerType)){
                QSingleSignerPtr signer{nullptr};
                QWarningMessage warningmsg;
                if (ClientController::instance()->isSubscribed() && (int)nunchuk::SignerType::NFC == it.data()->signerType()) {
                    signer = bridge::nunchukGetDefaultSignerFromMasterSigner(it.data()->masterSignerId(),
                                                                             (ENUNCHUCK::WalletType)walletType,
                                                                             (ENUNCHUCK::AddressType)addressType,
                                                                             warningmsg);
                }
                else {
                    signer = bridge::nunchukGetUnusedSignerFromMasterSigner(it.data()->masterSignerId(),
                                                                            (ENUNCHUCK::WalletType)walletType,
                                                                            (ENUNCHUCK::AddressType)addressType,
                                                                            warningmsg);
                }
                if(signer && warningmsg.type() == (int)EWARNING::WarningType::NONE_MSG){
                    AppModel::instance()->newWalletInfo()->singleSignersAssigned()->replaceSingleSigner(i, signer);
                }
                else{
                    it.data()->setNeedTopUpXpub(true);
                    AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type() );
                }
            }
        }
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        QEventProcessor::instance()->sendEvent(E::EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW);
        if(needTopUp){
            AppModel::instance()->showToast(0, STR_CPP_071, EWARNING::WarningType::ERROR_MSG );
        }
    }
}
