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
#include "STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Servers/Draco.h"
#include "STATE_ID_SCR_LOGIN_ONLINE.h"

void SCR_ADD_MASTER_SIGNER_RESULT_Entry(QVariant msg) {

}

void SCR_ADD_MASTER_SIGNER_RESULT_Exit(QVariant msg) {
}

void EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        if(AppModel::instance()->masterSignerInfo()->needPassphraseSent() &&
                (AppModel::instance()->masterSignerInfo()->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE)){
            QMap<QString, QVariant> passPhraseData;
            passPhraseData["state_id"] = E::STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT;
            passPhraseData["master_signer_xfp"] = AppModel::instance()->masterSignerInfo()->fingerPrint();
            passPhraseData["master_signer_id"] = AppModel::instance()->masterSignerInfo()->id();
            passPhraseData["master_signer_msg"] = AppModel::instance()->masterSignerInfo()->message();
            passPhraseData["is_software"] = true;
            QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
        }
        else{
            AppModel::instance()->startHealthCheckMasterSigner(E::STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT,
                                                               AppModel::instance()->masterSignerInfo()->fingerPrint(),
                                                               AppModel::instance()->masterSignerInfo()->message());
        }
    }
}

void EVT_ADD_MASTER_SIGNER_FINISHED_HANDLER(QVariant msg) {
}

void EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION_HANDLER(QVariant msg) {

}

void EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN_HANDLER(QVariant msg) {

}

void EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS_HANDLER(QVariant msg) {
    Q_UNUSED(msg)
    QMasterSignerPtr signer = AppModel::instance()->masterSignerInfoPtr();
    if(signer){
        if(signer->needPassphraseSent() && signer->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE){
            QMap<QString, QVariant> passPhraseData;
            passPhraseData["state_id"] = E::STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT;
            passPhraseData["master_signer_xfp"] = signer->fingerPrint();
            passPhraseData["master_signer_id"] = signer->id();
            passPhraseData["master_signer_msg"] = signer->message();
            passPhraseData["is_software"] = true;
            passPhraseData["is_top_up"] = true;
            QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
        }
        else{
            QMap<QString, QVariant> data;
            data["state_id"] = E::STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT;
            data["masterSignerId"] = signer->id();
            AppModel::instance()->startTopXPUBsMasterSigner(QVariant::fromValue(data));
        }
    }
}


void EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST_HANDLER(QVariant msg) {
    QString mastersigner_id = msg.toString();
    if (mastersigner_id == "") {
        return;
    }
    QMasterSignerPtr key = AppModel::instance()->masterSignerInfoPtr();
    NunchukType1 func = [](const QString &id){
        DBG_INFO << "Delete Key " << id;
        bridge::nunchukDeleteMasterSigner(id);
        QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
        if (mastersigners) {
            AppModel::instance()->setMasterSignerList(mastersigners);
        }
        QEventProcessor::instance()->sendEvent(E::EVT_ADD_MASTER_SIGNER_FINISHED);
        AppModel::instance()->setMasterSignerInfo(QMasterSignerPtr(new QMasterSigner()));
        if (AppModel::instance()->walletList()) {
            AppModel::instance()->walletList()->notifyMasterSignerDeleted(id);
        }
    };
    if(key->needPassphraseSent() && key->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE){
        QMap<QString, QVariant> passPhraseData;
        passPhraseData["state_id"] = E::STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT;
        passPhraseData["master_signer_id"] = key->id();
        passPhraseData["is_software"] = true;
        passPhraseData["is_remove"] = true;
        passPhraseData["func_remove_key"] = QVariant::fromValue(func);
        QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
    }
    else{
        func(mastersigner_id);
    }
}


