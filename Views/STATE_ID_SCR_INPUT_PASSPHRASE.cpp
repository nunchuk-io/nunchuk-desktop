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
#include "STATE_ID_SCR_INPUT_PASSPHRASE.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Chats/matrixbrigde.h"
#include "localization/STR_CPP.h"

static QVariant passPhraseObject = QVariant();

void SCR_INPUT_PASSPHRASE_Entry(QVariant msg) {
    passPhraseObject = msg;
}

void SCR_INPUT_PASSPHRASE_Exit(QVariant msg) {
    passPhraseObject = QVariant();
}

void EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE_HANDLER(QVariant msg) {
    QString passphraseInput = msg.toString();
    int state_id = passPhraseObject.toMap().value("state_id").toInt();
    switch (state_id) {
    case E::STATE_ID_SCR_TRANSACTION_INFO:
    {
        bool is_software = passPhraseObject.toMap().value("is_software").toBool();
        if(is_software){
            QString signer_xfp = passPhraseObject.toMap().value("signer_xfp").toString();
            if(AppModel::instance()->masterSignerList()){
                QMasterSignerPtr signer =  AppModel::instance()->masterSignerList()->getMasterSignerByXfp(signer_xfp);
                if(signer && signer.data()->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE && signer.data()->needPassphraseSent() ){
                    QString mastersignerId  = signer.data()->id();
                    QWarningMessage msgwarning;
                    bridge::nunchukSendSignerPassphrase( mastersignerId, passphraseInput, msgwarning);
                    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                        QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
                        if(mastersigners){
                            AppModel::instance()->setMasterSignerList(mastersigners);
                        }
                        QString wallet_id = passPhraseObject.toMap().value("wallet_id").toString();
                        QString tx_id = passPhraseObject.toMap().value("tx_id").toString();
                        QString device_xfp = passPhraseObject.toMap().value("device_xfp").toString();
                        DBG_INFO << "CONTINUE SIGNING TRANSACTION" << is_software << device_xfp;
                        AppModel::instance()->startSigningTransaction(wallet_id,
                                                                      tx_id,
                                                                      device_xfp,
                                                                      is_software);
                    }
                    else{
                        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
                    }
                }
                QQuickViewer::instance()->sendEvent(E::EVT_INPUT_PASSPHRASE_CLOSE);
            }
        }
        else{
            int device_idx = passPhraseObject.toMap().value("device_idx").toInt();
            AppModel::instance()->startSendPassphraseToDevice(state_id, device_idx, passphraseInput);
        }
        break;
    }
    case E::STATE_ID_SCR_MASTER_SIGNER_INFO:
    case E::STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT:
    {
        bool is_software = passPhraseObject.toMap().value("is_software").toBool();
        if(is_software){
            QString master_signer_id = passPhraseObject.toMap().value("master_signer_id").toString();
            QString master_signer_msg = passPhraseObject.toMap().value("master_signer_msg").toString();
            QString master_signer_xfp = passPhraseObject.toMap().value("master_signer_xfp").toString();
            QWarningMessage msgwarning;
            bridge::nunchukSendSignerPassphrase( master_signer_id, passphraseInput, msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                bool isCheckHealth = passPhraseObject.toMap().value("is_check_heath").toBool();
                bool isTopUp = passPhraseObject.toMap().value("is_top_up").toBool();
                bool isRemove = passPhraseObject.toMap().value("is_remove").toBool();
                if(isCheckHealth){
                    DBG_INFO << "CONTINUE HEALTH CHECK MASTER SIGNER";
                    AppModel::instance()->startHealthCheckMasterSigner(state_id,
                                                                       master_signer_xfp,
                                                                       master_signer_msg);
                }
                else if(isTopUp){
                    DBG_INFO << "CONTINUE TOP UP SIGNER";
                    QMap<QString, QVariant> data;
                    data["state_id"] = state_id;
                    data["masterSignerId"] = master_signer_id;
                    timeoutHandler(100,[=](){
                        AppModel::instance()->startTopXPUBsMasterSigner(QVariant::fromValue(data));
                    });
                }
                else if(isRemove){
                    QVariant var = passPhraseObject.toMap().value("func_remove_key");
                    NunchukType1 remove = var.value<NunchukType1>();
                    remove(master_signer_id);
                }
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
            }
            QQuickViewer::instance()->sendEvent(E::EVT_INPUT_PASSPHRASE_CLOSE);
        }
        else{
            int device_idx = passPhraseObject.toMap().value("device_idx").toInt();
            AppModel::instance()->startSendPassphraseToDevice(state_id, device_idx, passphraseInput);
        }
        break;
    }
    case E::STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION:
    {
        QString mastersigner_id = passPhraseObject.toMap().value("mastersigner_id").toString();
        int mastersigner_index = passPhraseObject.toMap().value("mastersigner_index").toInt();
        QWarningMessage msgwarning;
        bridge::nunchukSendSignerPassphrase( mastersigner_id, passphraseInput, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(AppModel::instance()->masterSignerList()){
                AppModel::instance()->masterSignerList()->setUserChecked(true, mastersigner_index);
            }
        }
        QQuickViewer::instance()->sendEvent(E::EVT_INPUT_PASSPHRASE_CLOSE);
        break;
    }
    case E::STATE_ID_SCR_ADD_HARDWARE_SIGNER:
    case E::STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET:
    {
        int device_idx = passPhraseObject.toMap().value("device_idx").toInt();
        AppModel::instance()->startSendPassphraseToDevice(state_id, device_idx, passphraseInput);
        break;
    }
    case E::STATE_ID_SCR_REPLACE_PRIMARY_KEY:
    {
        QString mastersigner_id = passPhraseObject.toMap().value("mastersigner_id").toString();
        QWarningMessage msgwarning;
        bridge::nunchukSendSignerPassphrase( mastersigner_id, passphraseInput, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            QQuickViewer::instance()->sendEvent(E::EVT_REPLACE_PRIMARY_KEY_REQUEST,true);
        }
        else{
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }
        QQuickViewer::instance()->sendEvent(E::EVT_INPUT_PASSPHRASE_CLOSE);
        break;
    }
    case E::STATE_ID_SCR_APP_SETTINGS:
    {
        QString mastersigner_id = passPhraseObject.toMap().value("master_signer_id").toString();
        QWarningMessage msgwarning;
        bridge::nunchukSendSignerPassphrase( mastersigner_id, passphraseInput, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            QQuickViewer::instance()->sendEvent(E::EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST);
        }
        else{
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }
        QQuickViewer::instance()->sendEvent(E::EVT_INPUT_PASSPHRASE_CLOSE);
        break;
    }
    default:
        break;
    }
}

void EVT_INPUT_PASSPHRASE_CLOSE_HANDLER(QVariant msg) {

}
