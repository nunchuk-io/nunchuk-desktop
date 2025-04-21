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
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Chats/matrixbrigde.h"
#include "Chats/ClientController.h"
#include "Servers/Draco.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"

void SCR_WALLET_INFO_Entry(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->isSharedWallet() && AppModel::instance()->walletInfo()->singleSignersAssigned()){
        if(AppModel::instance()->walletInfo()->singleSignersAssigned()->needSyncNunchukEmail()){
            AppModel::instance()->walletInfo()->syncCollabKeyname();
        }
    }
}

void SCR_WALLET_INFO_Exit(QVariant msg) {

}

void EVT_WALLET_INFO_EDIT_NAME_HANDLER(QVariant msg) {
    QString name        = msg.toMap().value("name").toString();
    QString description = msg.toMap().value("description").toString();
    DBG_INFO << name << description;
    if (auto w = AppModel::instance()->walletInfo()) {
        bridge::nunchukUpdateWallet(w->walletId(), name, description);
    }
}

void EVT_WALLET_INFO_REMOVE_HANDLER(QVariant msg) {
    Wallet *wallet = msg.value<Wallet*>();
    if(wallet){
        QString wallet_id = wallet->walletId();
        if (wallet->isDeleting()) {
            if (ServiceSetting::instance()->servicesTagPtr()->secQuesAnswer()) {
                if (wallet->DeleteAssistedWallet()) {
                    QWarningMessage msgwarning;
                    bool ret = bridge::nunchukDeleteWallet(wallet_id, msgwarning);
                    if(ret && (int)EWARNING::WarningType::NONE_MSG == msgwarning.type()) {
                        AppModel::instance()->removeWallet(wallet_id);
                        QEventProcessor::instance()->sendEvent(E::EVT_WALLET_INFO_BACK_REQUEST);
                        AppModel::instance()->setWalletListCurrentIndex(0);
                    }
                    else {
                        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
                    }
                }
            }
        }
        else {
            if (wallet->isAssistedWallet()) {
                QMap<QString, QVariant> data;
                data["state_id"] = E::STATE_ID_SCR_WALLET_INFO;
                data["wallet_id"] = wallet->walletId();
                QEventProcessor::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_REQUEST, data);
            }
            else {
                bool is_shared_wallet = wallet->isSharedWallet();
                QWarningMessage msgwarning;
                bool ret = bridge::nunchukDeleteWallet(wallet_id, msgwarning);
                if(ret && (int)EWARNING::WarningType::NONE_MSG == msgwarning.type()) {
                    AppModel::instance()->removeWallet(wallet_id);
                    QEventProcessor::instance()->sendEvent(E::EVT_WALLET_INFO_BACK_REQUEST);
                    AppModel::instance()->setWalletListCurrentIndex(0);
                    if(is_shared_wallet){
                        QString roomId = CLIENT_INSTANCE->rooms()->getRoomIdByWalletId(wallet_id);
                        int index = CLIENT_INSTANCE->rooms()->getIndex(roomId);
                        if(index >= 0){
                            ClientController::instance()->leaveRoom(index);
                        }
                    }
                }
                else {
                    AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
                }
            }
        }
    }
}

void EVT_WALLET_INFO_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_WALLET_INFO_UTXOS_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->walletInfo()->RequestGetCoins();
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
        bool ret = bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->walletId(),
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
        if(csv_type == 0 ){
            bool ret = bridge::nunchukExportTransactionHistory(AppModel::instance()->walletInfo()->walletId(),
                                                                file_path,
                                                                nunchuk::ExportFormat::CSV);
            DBG_INFO << file_path << ret;
        }
        else if(csv_type == 1 ){
            // PDF
            DBG_INFO << file_path;
        }
        else{
            bool ret = bridge::nunchukExportUnspentOutputs(AppModel::instance()->walletInfo()->walletId(),
                                                            file_path,
                                                            nunchuk::ExportFormat::CSV);
            DBG_INFO << file_path << ret;
        }
    }
}

void EVT_WALLET_INFO_EXPORT_DESCRIPTOR_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->walletInfo() && (file_path != "")){
        bool ret = bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->walletId(), file_path, nunchuk::ExportFormat::BSMS);
        DBG_INFO << file_path << ret;
    }
}

void EVT_WALLET_INFO_EXPORT_COLDCARD_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->walletInfo() && (file_path != "")){
        bool ret = bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->walletId(), file_path, nunchuk::ExportFormat::COLDCARD);
        DBG_INFO << file_path << ret;
    }
}

void EVT_WALLET_INFO_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {

    QString requestType = msg.toMap().value("requestType").toString();
    if(qUtils::strCompare("signerInfo", requestType)){
        int index = msg.toMap().value("requestData").toInt();
        if((index >= 0) && AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->singleSignersAssigned() ){
            QSingleSignerPtr it = AppModel::instance()->walletInfo()->singleSignersAssigned()->getSingleSignerByIndex(index);
            if(it)
            {
                AppModel::instance()->setSingleSignerInfo(it);
                if((int)ENUNCHUCK::SignerType::AIRGAP == it.data()->signerType()){
                    AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByFingerPrint(it.data()->masterFingerPrint()));
                    QEventProcessor::instance()->sendEvent(E::EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST);
                }
                else{
                    if(AppModel::instance()->masterSignerList()){
                        QMasterSignerPtr localsigner = AppModel::instance()->masterSignerList()->getMasterSignerById(it.data()->masterSignerId());
                        if(localsigner){
                            AppModel::instance()->setMasterSignerInfo(localsigner);
                            AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByMasterSigner(it.data()->masterSignerId()));
                            QEventProcessor::instance()->sendEvent(E::EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST);
                        }
                        else{
                            AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByFingerPrint(it.data()->masterFingerPrint()));
                            QEventProcessor::instance()->sendEvent(E::EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST);
                        }
                    }
                }
            }
        }
    }
    else if(qUtils::strCompare("viewPolicies", requestType)){
        QString wallet_id = msg.toMap().value("requestData").toString();
        DBG_INFO << wallet_id << "REQUEST POLICES";
        QEventProcessor::instance()->sendEvent(E::EVT_GOTO_SERVICE_SETTING_TAB, 7);
    }
    else {
        DBG_INFO << msg;
    }
}

void EVT_WALLET_INFO_EDIT_DESCRIPTION_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletList()){
        QString name        = msg.toMap().value("name").toString();
        QString description = msg.toMap().value("description").toString();

        bridge::nunchukUpdateWallet(AppModel::instance()->walletInfo()->walletId(), name, description);
        AppModel::instance()->walletList()->updateDescription(AppModel::instance()->walletInfo()->walletId(), description);
    }
}

void EVT_WALLET_INFO_EXPORT_QRCODE_HANDLER(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
    if(AppModel::instance()->walletInfo()){
        QString qrtype = msg.toMap().value("qrtype").toString();;
        DBG_INFO << qrtype;
        QWarningMessage msgwarning;
        if(qUtils::strCompare(qrtype, "BC-UR2-QR")){ // Seedhammer
            QStringList qrtags = bridge::nunchukExportBCR2020010Wallet(AppModel::instance()->walletInfo()->walletId(), msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
                AppModel::instance()->setQrExported(qrtags);
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
            }
        }
        else if(qUtils::strCompare(qrtype, "BC-UR2-QR-Legacy")){ //Keystone, Passport, SeedSigner, Jade
            QStringList qrtags = bridge::nunchukExportKeystoneWallet(AppModel::instance()->walletInfo()->walletId(), msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
                AppModel::instance()->setQrExported(qrtags);
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
            }
        }
        else { // if(qUtils::strCompare(qrtype, "BBQR-Coldcard")){ //Keystone, Passport, SeedSigner, Jade
            QStringList qrtags = qUtils::ExportBBQRWallet(AppModel::instance()->walletInfo()->nunchukWallet(), msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
                AppModel::instance()->setQrExported(qrtags);
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
            }
        }
    }
}

void EVT_WALLET_INFO_IMPORT_PSBT_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    QWalletPtr wallet = AppModel::instance()->walletInfoPtr();
    if (file_path != "" && wallet){
        QString wallet_id = wallet->walletId();
        QWarningMessage msgwarning;
        QTransactionPtr trans = bridge::nunchukImportTransaction(wallet_id, file_path, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(trans){
                AppModel::instance()->setTransactionInfo(trans);
                wallet.data()->CreateAsisstedTxs(trans.data()->txid(), trans.data()->psbt(), trans.data()->memo());
                AppModel::instance()->requestSyncWalletDb(wallet_id);
                QEventProcessor::instance()->sendEvent(E::EVT_WALLET_INFO_BACK_REQUEST);
                AppModel::instance()->showToast(0, STR_CPP_091, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        else{
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
        }
    }
}

void EVT_WALLET_INFO_REFRESH_WALLET_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO << msg;
    QString wallet_id = msg.toString();
    QWarningMessage message;
    bridge::ForceRefreshWallet(wallet_id,message);
}

void EVT_WALLET_INFO_GAP_LIMIT_REQUEST_HANDLER(QVariant msg)
{
    if (AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->walletId();
        uint gap_limit = msg.toUInt();
        bridge::nunchukUpdateWalletGapLimit(wallet_id, gap_limit);
        AppModel::instance()->walletInfo()->setWalletGapLimit(gap_limit);
    }
}
