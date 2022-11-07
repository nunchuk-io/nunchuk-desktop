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
#include "STATE_ID_SCR_WALLET_INFO.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Chats/matrixbrigde.h"
#include "Chats/ClientController.h"

void SCR_WALLET_INFO_Entry(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
}

void SCR_WALLET_INFO_Exit(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        if(AppModel::instance()->walletInfo()->singleSignersAssigned()){
            AppModel::instance()->walletInfo()->singleSignersAssigned()->initIsColdCard();
        }
    }
}

void EVT_WALLET_INFO_EDIT_NAME_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletList()){
        bridge::nunchukUpdateWalletName(AppModel::instance()->walletInfo()->id(), msg.toString());
        AppModel::instance()->walletList()->updateName(AppModel::instance()->walletInfo()->id(), msg.toString());
        if(AppModel::instance()->walletList()){
            QString wallet_id = AppModel::instance()->walletInfo()->id();
            AppModel::instance()->walletList()->requestSort(WalletListModel::WalletRoles::wallet_Name_Role, Qt::AscendingOrder);
            int index = AppModel::instance()->walletList()->getWalletIndexById(wallet_id);
            if(-1 != index){
                AppModel::instance()->setWalletListCurrentIndex(index);
            }
        }
    }
}

void EVT_WALLET_INFO_REMOVE_HANDLER(QVariant msg) {
    Wallet *wallet = msg.value<Wallet*>();
    if(wallet != nullptr){
        if(bridge::nunchukDeleteWallet(wallet->id())){
            AppModel::instance()->removeWallet(wallet->id());
            QQuickViewer::instance()->sendEvent(E::EVT_WALLET_INFO_BACK_REQUEST);
            AppModel::instance()->setWalletListCurrentIndex(0);
        }
        if(wallet->isSharedWallet()){
            QString roomId = CLIENT_INSTANCE->rooms()->getRoomIdByWalletId(wallet->id());
            int index = CLIENT_INSTANCE->rooms()->getIndex(roomId);
            ClientController::instance()->leaveRoom(index);
        }
    }
}

void EVT_WALLET_INFO_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_WALLET_INFO_UTXOS_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QUTXOListModelPtr utxos = bridge::nunchukGetUnspentOutputs(AppModel::instance()->walletInfo()->id());
        if(utxos){
            AppModel::instance()->setUtxoList(utxos);
        }
    }
}

void EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST_HANDLER(QVariant msg) {

}

void EVT_WALLET_INFO_EXPORT_WALLET_REQUEST_HANDLER(QVariant msg) {

}

void EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
}

void EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
}

void EVT_WALLET_INFO_EXPORT_DB_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->walletInfo() && (file_path != "")){
        bool ret = bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->id(),
                                                file_path,
                                                nunchuk::ExportFormat::DB);
        DBG_INFO << file_path << ret;
    }
}

void EVT_WALLET_INFO_EXPORT_CSV_HANDLER(QVariant msg) {
    int csv_type = msg.toMap().value("csv_type").toInt();
    QString file = msg.toMap().value("file_path").toString();
    QString file_path = qUtils::QGetFilePath(file);
    if(AppModel::instance()->walletInfo() && (file_path != "")){
        if(csv_type > 0 ){
            bool ret = bridge::nunchukExportTransactionHistory(AppModel::instance()->walletInfo()->id(),
                                                                file_path,
                                                                nunchuk::ExportFormat::CSV);
            DBG_INFO << file_path << ret;
        }
        else{
            bool ret = bridge::nunchukExportUnspentOutputs(AppModel::instance()->walletInfo()->id(),
                                                            file_path,
                                                            nunchuk::ExportFormat::CSV);
            DBG_INFO << file_path << ret;
        }
    }
}

void EVT_WALLET_INFO_EXPORT_DESCRIPTOR_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->walletInfo() && (file_path != "")){
        bool ret = bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->id(),
                                               file_path,
                                               nunchuk::ExportFormat::BSMS);
        DBG_INFO << file_path << ret;
    }
}

void EVT_WALLET_INFO_EXPORT_COLDCARD_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->walletInfo() && (file_path != "")){
        bool ret = bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->id(),
                                                file_path,
                                                nunchuk::ExportFormat::COLDCARD);
        DBG_INFO << file_path << ret;
    }
}

void EVT_WALLET_INFO_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    if((msg.toInt() >= 0) && AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->singleSignersAssigned() ){
        QSingleSignerPtr it = AppModel::instance()->walletInfo()->singleSignersAssigned()->getSingleSignerByIndex(msg.toInt());
        if(it)
        {
            AppModel::instance()->setSingleSignerInfo(it);
            if((int)ENUNCHUCK::SignerType::AIRGAP == it.data()->signerType()){
                AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByFingerPrint(it.data()->masterFingerPrint()));
                QQuickViewer::instance()->sendEvent(E::EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST);
            }
            else{
                if(AppModel::instance()->masterSignerList()){
                    QMasterSignerPtr localsigner = AppModel::instance()->masterSignerList()->getMasterSignerById(it.data()->masterSignerId());
                    if(localsigner){
                        AppModel::instance()->setMasterSignerInfo(localsigner);
                        AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByMasterSigner(it.data()->masterSignerId()));
                        QQuickViewer::instance()->sendEvent(E::EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST);
                    }
                    else{
                        AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByFingerPrint(it.data()->masterFingerPrint()));
                        QQuickViewer::instance()->sendEvent(E::EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST);
                    }
                }
            }
        }
    }
}

void EVT_WALLET_INFO_EDIT_DESCRIPTION_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletList()){
        bridge::nunchukUpdateWalletDescription(AppModel::instance()->walletInfo()->id(), msg.toString());
        AppModel::instance()->walletList()->updateDescription(AppModel::instance()->walletInfo()->id(), msg.toString());
    }
}

void EVT_WALLET_INFO_EXPORT_QRCODE_HANDLER(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
    if(AppModel::instance()->walletInfo()){
        QString qrtype = msg.toString();
        QStringList qrtags;
        qrtags.clear();
        QWarningMessage msgwarning;
        if(0 == QString::compare(qrtype, "keystone", Qt::CaseInsensitive)){
            qrtags = bridge::nunchukExportKeystoneWallet(AppModel::instance()->walletInfo()->id(),
                                                         msgwarning);
        }
        else if(0 == QString::compare(qrtype, "passport", Qt::CaseInsensitive)){
            qrtags = bridge::nunchukExportPassportWallet(AppModel::instance()->walletInfo()->id(),
                                                         msgwarning);
        }
        else{ // Others support in TBD
            return;
        }
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            AppModel::instance()->setQrExported(qrtags);
        }
        else{
            AppModel::instance()->showToast(msgwarning.code(),
                                            msgwarning.what(),
                                            (EWARNING::WarningType)msgwarning.type(),
                                            STR_CPP_084);
        }
    }
}

void EVT_WALLET_INFO_IMPORT_PSBT_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if (file_path != "" && AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->id();
        QWarningMessage msgwarning;
        QTransactionPtr trans = bridge::nunchukImportTransaction(wallet_id, file_path, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(trans){
                AppModel::instance()->setTransactionInfo(trans);
                if(AppModel::instance()->walletInfo()){
                    AppModel::instance()->startGetTransactionHistory(wallet_id);
                    AppModel::instance()->startGetUsedAddresses(wallet_id);
                    AppModel::instance()->startGetUnusedAddresses(wallet_id);
                }
                QQuickViewer::instance()->sendEvent(E::EVT_WALLET_INFO_BACK_REQUEST);
                AppModel::instance()->showToast(0,
                                                STR_CPP_091,
                                                EWARNING::WarningType::SUCCESS_MSG,
                                                STR_CPP_091);
            }
        }
        else{
            AppModel::instance()->showToast(msgwarning.code(),
                                            msgwarning.what(),
                                            (EWARNING::WarningType)msgwarning.type(),
                                            STR_CPP_092);
        }
    }
}
