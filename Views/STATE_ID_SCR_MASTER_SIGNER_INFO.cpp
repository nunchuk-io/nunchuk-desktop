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
#include "STATE_ID_SCR_MASTER_SIGNER_INFO.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QGroupWalletHealthCheck.h"

void SCR_MASTER_SIGNER_INFO_Entry(QVariant msg) {

}

void SCR_MASTER_SIGNER_INFO_Exit(QVariant msg) {
    AppModel::instance()->startReloadMasterSigners();
}

void EVT_MASTER_SIGNER_INFO_EDIT_NAME_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        int signerType = AppModel::instance()->masterSignerInfo()->signerType();
        QString xfp = AppModel::instance()->masterSignerInfo()->fingerPrint();
        QString newname = msg.toString();
        AppModel::instance()->masterSignerInfo()->setName(newname);
        if (signerType == (int)ENUNCHUCK::SignerType::COLDCARD_NFC){
            if(AppModel::instance()->remoteSignerList()) {
                QSingleSignerPtr signer = AppModel::instance()->remoteSignerList()->getSingleSignerByFingerPrint(xfp);
                if(signer){
                    signer.data()->setName(newname);
                }
                AppModel::instance()->remoteSignerList()->requestSort();
                bridge::nunchukUpdateRemoteSigner(signer);
            }
        }
        else {
            bridge::nunchukUpdateMasterSigner(AppModel::instance()->masterSignerInfoPtr());
            if(AppModel::instance()->masterSignerList()){
                AppModel::instance()->masterSignerList()->requestSort(MasterSignerListModel::MasterSignerRoles::master_signer_name_Role, Qt::AscendingOrder);
            }
        }
    }
}

void EVT_MASTER_SIGNER_INFO_HEALTH_CHECK_HANDLER(QVariant msg) {
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        if (dashboard->healthPtr()->HealthCheckAddReminderClicked(msg)) {
            return;
        }
    }
    if(AppModel::instance()->masterSignerInfo()){
        if(AppModel::instance()->masterSignerInfo()->needPassphraseSent()
            && (AppModel::instance()->masterSignerInfo()->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE)){
            QMap<QString, QVariant> passPhraseData;
            passPhraseData["state_id"] = E::STATE_ID_SCR_MASTER_SIGNER_INFO;
            passPhraseData["master_signer_xfp"] = AppModel::instance()->masterSignerInfo()->fingerPrint();
            passPhraseData["master_signer_id"] = AppModel::instance()->masterSignerInfo()->id();
            passPhraseData["master_signer_msg"] = AppModel::instance()->masterSignerInfo()->message();
            passPhraseData["is_software"] = true;
            passPhraseData["is_check_heath"] = true;
            QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
        }
        else{
            AppModel::instance()->startHealthCheckMasterSigner(E::STATE_ID_SCR_MASTER_SIGNER_INFO,
                                                               AppModel::instance()->masterSignerInfo()->fingerPrint(),
                                                               AppModel::instance()->masterSignerInfo()->message());
        }
    }
}

void EVT_MASTER_SIGNER_INFO_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST_HANDLER(QVariant msg) {
    QString mastersigner_id = msg.toString();
    QMasterSignerPtr key = AppModel::instance()->masterSignerInfoPtr();
    NunchukType1 func = [](const QString &id){
        DBG_INFO << "Delete Key " << id;
        if (AppModel::instance()->masterSignerInfo()->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC) {
            QSingleSignerPtr ptr = AppModel::instance()->singleSignerInfoPtr();
            if (ptr) {
                QString master_fingerprint = ptr->masterFingerPrint();
                QString derivation_path    = ptr->derivationPath();
                if(bridge::nunchukDeleteRemoteSigner(master_fingerprint, derivation_path)){
                    QSingleSignerListModelPtr remoteSigners = bridge::nunchukGetRemoteSigners();
                    if(remoteSigners){
                        AppModel::instance()->setRemoteSignerList(remoteSigners);
                    }
                    QEventProcessor::instance()->sendEvent(E::EVT_MASTER_SIGNER_INFO_BACK_REQUEST);
                    AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new QSingleSigner()));
                }
            }
        } else {
            if (id == "") {
                return;
            }
            bridge::nunchukDeleteMasterSigner(id);
            QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
            if (mastersigners) {
                AppModel::instance()->setMasterSignerList(mastersigners);
            }
            QEventProcessor::instance()->sendEvent(E::EVT_MASTER_SIGNER_INFO_BACK_REQUEST);
            AppModel::instance()->setMasterSignerInfo(QMasterSignerPtr(new QMasterSigner()));
            if (AppModel::instance()->walletList()) {
                AppModel::instance()->walletList()->notifyMasterSignerDeleted(id);
            }
        }
    };
    if(key->needPassphraseSent() && key->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE){
        QMap<QString, QVariant> passPhraseData;
        passPhraseData["state_id"] = E::STATE_ID_SCR_MASTER_SIGNER_INFO;
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

void EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO_HANDLER(QVariant msg) {

}

void EVT_MASTER_SIGNER_INFO_PROMT_PIN_HANDLER(QVariant msg) {

}

void EVT_MASTER_SIGNER_INFO_SEND_PIN_HANDLER(QVariant msg) {

}

void EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE_HANDLER(QVariant msg) {

}

void EVT_MASTER_SIGNER_INFO_GET_XPUBS_HANDLER(QVariant msg) {
    Q_UNUSED(msg)
    QMasterSignerPtr signer = AppModel::instance()->masterSignerInfoPtr();
    if(signer){
        if(signer->needPassphraseSent() && signer->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE){
            QMap<QString, QVariant> passPhraseData;
            passPhraseData["state_id"] = E::STATE_ID_SCR_MASTER_SIGNER_INFO;
            passPhraseData["master_signer_xfp"] = signer->fingerPrint();
            passPhraseData["master_signer_id"] = signer->id();
            passPhraseData["master_signer_msg"] = signer->message();
            passPhraseData["is_software"] = true;
            passPhraseData["is_top_up"] = true;
            QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
        }
        else{
            QMap<QString, QVariant> data;
            data["state_id"] = E::STATE_ID_SCR_MASTER_SIGNER_INFO;
            data["masterSignerId"] = signer->id();
            AppModel::instance()->startTopXPUBsMasterSigner(QVariant::fromValue(data));
        }
    }
}
