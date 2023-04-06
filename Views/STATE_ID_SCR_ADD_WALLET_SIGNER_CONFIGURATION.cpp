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
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

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
                QSingleSignerPtr signer = QSingleSignerPtr(new QSingleSigner());
                signer.data()->setName(ret.data()->name());
                signer.data()->setMasterSignerId(ret.data()->id());
                signer.data()->setSignerType(ret.data()->signerType());
                signer.data()->setDevicetype(ret.data()->device()->type());
                signer.data()->setCardId(ret.data()->device()->cardId());
                signer.data()->setMasterFingerPrint(ret.data()->fingerPrint());
                if(signer){
                    AppModel::instance()->newWalletInfo()->singleSignersAssigned()->addSingleSigner(signer);
                }
            }
        }
    }
    emit AppModel::instance()->newWalletInfo()->nChanged();
}

void EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER_HANDLER(QVariant msg) {
    if(AppModel::instance()->remoteSignerList()){
        for (int i = 0; i < AppModel::instance()->remoteSignerList()->rowCount(); i++) {
            QSingleSignerPtr signer = AppModel::instance()->remoteSignerList()->getSingleSignerByIndex(i);
            if(signer && signer.data()->checked()){
                signer.data()->setSignerType((int)signer.data()->signerType());
                AppModel::instance()->newWalletInfo()->singleSignersAssigned()->addSingleSigner(signer);
            }
        }
    }
    emit AppModel::instance()->newWalletInfo()->nChanged();
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
    if(AppModel::instance()->newWalletInfo()->m() > AppModel::instance()->newWalletInfo()->singleSignersAssigned()->rowCount()){
        AppModel::instance()->newWalletInfo()->setM(AppModel::instance()->newWalletInfo()->singleSignersAssigned()->rowCount());
    }
    emit AppModel::instance()->newWalletInfo()->nChanged();
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
        QQuickViewer::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
    }
}

void EVT_SIGNER_CONFIGURATION_TRY_REVIEW_HANDLER(QVariant msg) {
    DBG_INFO << msg;
    if(AppModel::instance()->newWalletInfo() && AppModel::instance()->newWalletInfo()->singleSignersAssigned()){
        int numberSignerRequired = AppModel::instance()->newWalletInfo()->n();
        DBG_INFO << "numberSignerRequired " << numberSignerRequired;
        bool escrow = AppModel::instance()->newWalletInfo()->escrow();
        ENUNCHUCK::WalletType walletType =  escrow ? ENUNCHUCK::WalletType::ESCROW :
                                                     numberSignerRequired > 1 ? ENUNCHUCK::WalletType::MULTI_SIG
                                                                              : ENUNCHUCK::WalletType::SINGLE_SIG;
        ENUNCHUCK::AddressType addressType = (ENUNCHUCK::AddressType)AppModel::instance()->newWalletInfo()->addressType().toInt();
        DBG_INFO << "addressType " << (int)addressType;
        for (int i = 0; i < AppModel::instance()->newWalletInfo()->singleSignersAssigned()->rowCount(); i++) {
            QSingleSignerPtr it = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->getSingleSignerByIndex(i);
            DBG_INFO << "signerType " << it.data()->signerType() << it.data()->isPrimaryKey();
            if(it && !((int)ENUNCHUCK::SignerType::AIRGAP == it.data()->signerType())){
                if(!(it.data()->needTopUpXpub())){
                    // Request cached xpub
                    QWarningMessage warningmsg;
                    QSingleSignerPtr signer = bridge::nunchukGetUnusedSignerFromMasterSigner(it.data()->masterSignerId(),
                                                                                             walletType,
                                                                                             addressType,
                                                                                             warningmsg);
                    if(signer && warningmsg.type() == (int)EWARNING::WarningType::NONE_MSG){
                        signer.data()->setIsPrimaryKey(it.data()->isPrimaryKey());
                        AppModel::instance()->newWalletInfo()->singleSignersAssigned()->replaceSingleSigner(i, signer);
                        AppModel::instance()->showToast(0,
                                                        STR_CPP_067,
                                                        EWARNING::WarningType::SUCCESS_MSG,
                                                        STR_CPP_067);
                    }
                    else{
                        it.data()->setNeedTopUpXpub(true);
                    }
                }
            }
        }
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        QQuickViewer::instance()->sendEvent(E::EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW);
        if(needTopUp){
            AppModel::instance()->showToast(0,
                                            0,
                                            EWARNING::WarningType::WARNING_MSG,
                                            STR_CPP_071);
        }
    }
}
