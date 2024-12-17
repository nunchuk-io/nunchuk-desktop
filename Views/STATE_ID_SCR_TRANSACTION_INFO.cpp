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
#include "STATE_ID_SCR_TRANSACTION_INFO.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include "Chats/matrixbrigde.h"
#include "Chats/ClientController.h"
#include "localization/STR_CPP.h"
#include "Servers/Draco.h"

void SCR_TRANSACTION_INFO_Entry(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
}

void SCR_TRANSACTION_INFO_Exit(QVariant msg) {
}

void EVT_TRANSACTION_SIGN_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO << msg;
    QString signerXfp = msg.toString();
    QTransactionPtr transaction = AppModel::instance()->transactionInfoPtr();
    QWalletPtr wallet = AppModel::instance()->walletInfoPtr();
    if(wallet && transaction && transaction.data()->singleSignersAssigned()){
        QString wallet_id = wallet.data()->id();
        QString tx_id = transaction.data()->txid();
        QSingleSignerPtr signer = transaction.data()->singleSignersAssigned()->getSingleSignerByFingerPrint(signerXfp);
        if(!signer){
            //FIXME SHOW TOAST ?
            emit AppModel::instance()->finishedSigningTransaction();
        }
        int signerType = signer.data()->signerType();
        if((int)ENUNCHUCK::SignerType::SOFTWARE == signerType){
            QDevicePtr device = AppModel::instance()->softwareSignerDeviceList()->getDeviceByXfp(signerXfp);
            int deviceIndex = AppModel::instance()->softwareSignerDeviceList()->getDeviceIndexByXfp(signerXfp);
            if(device){
                if(device.data()->needsPassPhraseSent()){
                    QMap<QString, QVariant> passPhraseData;
                    passPhraseData["state_id"] = E::STATE_ID_SCR_TRANSACTION_INFO;
                    passPhraseData["signer_xfp"] = signerXfp;
                    passPhraseData["wallet_id"] = wallet_id;
                    passPhraseData["tx_id"] = tx_id;
                    passPhraseData["device_index"] = deviceIndex;
                    passPhraseData["device_xfp"] = device.data()->masterFingerPrint();
                    passPhraseData["is_software"] = true;
                    QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
                }
                else{
                    AppModel::instance()->startSigningTransaction(wallet_id,
                                                                  tx_id,
                                                                  device.data()->masterFingerPrint(),
                                                                  true);
                }
            }
        }
        else if((int)ENUNCHUCK::SignerType::HARDWARE == signerType || (int)ENUNCHUCK::SignerType::COLDCARD_NFC == signerType){
            QDevicePtr device = AppModel::instance()->deviceList()->getDeviceByXfp(signerXfp);
            if(device){
                AppModel::instance()->startSigningTransaction(wallet_id,
                                                              tx_id,
                                                              device.data()->masterFingerPrint(),
                                                              false);
            }
            else{
                //FIXME SHOW TOAST ?
                emit AppModel::instance()->finishedSigningTransaction();
            }
        }
        else{
            //FIXME SHOW TOAST ?
            DBG_INFO << "SIGNER IS NOT SOFTWARE OR HARDWARE";
            emit AppModel::instance()->finishedSigningTransaction();
        }
    }
}

void EVT_TRANSACTION_EXPORT_REQUEST_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(file_path != ""){
        QWarningMessage msgwarning;
        bool ret = bridge::nunchukExportTransaction(AppModel::instance()->walletInfo()->id(),
                                                    AppModel::instance()->transactionInfo()->txid(),
                                                    file_path,
                                                    msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG != msgwarning.type()){
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }

        DBG_INFO << ret << file_path;
    }
}

void EVT_TRANSACTION_IMPORT_REQUEST_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    QWalletPtr wallet = AppModel::instance()->walletInfoPtr();
    if (file_path != "" && wallet){
        QString wallet_id = wallet.data()->id();
        QWarningMessage msgwarning;
        QTransactionPtr trans = bridge::nunchukImportTransaction(wallet_id, file_path, msgwarning);
        if(trans){
            AppModel::instance()->setTransactionInfo(trans);
            wallet.data()->SignAsisstedTxs(trans.data()->txid(), trans.data()->psbt(), trans.data()->memo());
            AppModel::instance()->requestSyncWalletDb(wallet_id);
        }
    }
}

void EVT_TRANSACTION_BROADCAST_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        QString wallet_id = AppModel::instance()->transactionInfo()->walletId();
        if( AppModel::instance()->transactionInfo()->roomId() != ""){
            QNunchukRoom* room = CLIENT_INSTANCE->GetRoomById(AppModel::instance()->transactionInfo()->roomId());
            if(room){
                QString init_event_id = AppModel::instance()->transactionInfo()->initEventId();
                QWarningMessage msgwarning;
                matrixbrigde::BroadcastTransaction(room->id(),
                                                   init_event_id,
                                                   msgwarning);
                DBG_INFO << wallet_id << init_event_id << msgwarning.type();
                if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                    if((int)ENUNCHUCK::TabSelection::CHAT_TAB == AppModel::instance()->tabIndex()){
                        QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                    }
                    else{
                        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                    }
                    room->startGetPendingTxs();
                    AppModel::instance()->requestSyncWalletDb(wallet_id);
                    AppModel::instance()->showToast(0, STR_CPP_085, EWARNING::WarningType::SUCCESS_MSG);
                }
                else{
                    AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
                }
            }
        }
        else{
            QWarningMessage msgwarning;
            QTransactionPtr trans = bridge::nunchukBroadcastTransaction(wallet_id, AppModel::instance()->transactionInfo()->txid(), msgwarning);
            DBG_INFO << wallet_id << AppModel::instance()->transactionInfo()->txid() << msgwarning.type();
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                if(trans){
                    if (auto wallet = AppModel::instance()->walletList()->getWalletById(wallet_id)) {
                        wallet.data()->AssignTagsToTxChange();
                    }
                    AppModel::instance()->setTransactionInfo(trans);
                    AppModel::instance()->showToast(0, STR_CPP_085, EWARNING::WarningType::SUCCESS_MSG);
                }
            }
            else{
                if(nunchuk::NunchukException::SERVER_REQUEST_ERROR == msgwarning.code()){
                    AppModel::instance()->transactionInfo()->setStatus((int)ENUNCHUCK::TransactionStatus::NETWORK_REJECTED);
                    AppModel::instance()->showToast(nunchuk::NunchukException::SERVER_REQUEST_ERROR, STR_CPP_086, EWARNING::WarningType::EXCEPTION_MSG);
                }
                else{
                    AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
                }
            }
            wallet_id = AppModel::instance()->transactionInfo()->walletId();
            QWalletPtr wallet_result = bridge::nunchukGetWallet(wallet_id);
            if(wallet_result && AppModel::instance()->walletList()){
                AppModel::instance()->walletList()->updateBalance(wallet_id, wallet_result.data()->balanceSats());
            }
            AppModel::instance()->requestSyncWalletDb(wallet_id);
        }
    }
}

void EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->setTxidReplacing(AppModel::instance()->transactionInfo()->txid());
    }
}

void EVT_TRANSACTION_SET_MEMO_REQUEST_HANDLER(QVariant msg) {
    QString memo = msg.toString();
    if(AppModel::instance()->transactionInfo() && memo != ""){
        QString tx_id = AppModel::instance()->transactionInfo()->txid();
        QString wallet_id = AppModel::instance()->transactionInfo()->walletId();
        bridge::nunchukUpdateTransactionMemo(wallet_id, tx_id, memo);
        if(AppModel::instance()->walletInfo()){
            if(AppModel::instance()->walletInfo()->transactionHistory()){
                AppModel::instance()->walletInfo()->transactionHistory()->updateTransactionMemo(tx_id, memo);
            }
            AppModel::instance()->walletInfo()->UpdateAssistedTxs(tx_id, memo);
        }
        if(CLIENT_INSTANCE){
            CLIENT_INSTANCE->updateTransactionMemo(wallet_id, tx_id, memo);
        }
    }
}

void EVT_TRANSACTION_INFO_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST_HANDLER(QVariant msg) {

}

void EVT_TRANSACTION_SCAN_DEVICE_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->startScanDevices(E::STATE_ID_SCR_TRANSACTION_INFO);
}

void EVT_TRANSACTION_REMOVE_REQUEST_HANDLER(QVariant msg) {
    if (auto trans = AppModel::instance()->transactionInfo()) {
        if (trans->status() == (int)ENUNCHUCK::TransactionStatus::PENDING_CONFIRMATION) {
            DBG_INFO;
            QEventProcessor::instance()->sendEvent(E::EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST);
        }
        else {
            QString wallet_id = trans->walletId();
            QString txid = trans->txid();
            bool ret = bridge::nunchukDeleteTransaction(wallet_id, txid);
            if(ret){
                QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
                if(wallet){
                    wallet.data()->CancelAssistedTxs(txid);
                }
                AppModel::instance()->requestSyncWalletDb(wallet_id);
                if(QEventProcessor::instance()->onsRequester() == E::STATE_ID_SCR_TRANSACTION_HISTORY){
                    QEventProcessor::instance()->sendEvent(E::EVT_TRANSACTION_INFO_BACK_REQUEST);
                }
                else{
                    if((int)ENUNCHUCK::TabSelection::CHAT_TAB == AppModel::instance()->tabIndex()){
                        QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                    }
                    else{
                        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                    }
                }
                AppModel::instance()->setTransactionInfo( QTransactionPtr(new Transaction()));
            }
        }
    }
}

void EVT_TRANSACTION_INFO_BACK_UTXO_INFO_HANDLE(QVariant msg) {

}

void EVT_TRANSACTION_SEND_PIN_REQUEST_HANDLER(QVariant msg) {

}

void EVT_TRANSACTION_PROMT_PIN_REQUEST_HANDLER(QVariant msg) {

}

void EVT_TRANSACTION_EXPORT_QRCODE_HANDLER(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
    if(AppModel::instance()->walletInfo() && AppModel::instance()->transactionInfo()){
        QString qrtype = msg.toMap().value("qrtype").toString();;
        DBG_INFO << qrtype;
        QWarningMessage msgwarning;

        if(qUtils::strCompare(qrtype, "QR-transaction")){
            QStringList qrtags = bridge::nunchukExportQRTransaction(AppModel::instance()->walletInfo()->id(),
                                                                    AppModel::instance()->transactionInfo()->txid(),
                                                                    msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
                AppModel::instance()->setQrExported(qrtags);
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
            }
        } else if(qUtils::strCompare(qrtype, "BBQR-transaction")){
            QStringList qrtags = qUtils::ExportBBQRTransaction(AppModel::instance()->transactionInfo()->psbt(), msgwarning);
            DBG_INFO << qrtags;
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
                AppModel::instance()->setQrExported(qrtags);
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
            }
        }
    }
}

void EVT_TRANSACTION_IMPORT_QRCODE_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo() && AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->SignAsisstedTxs(AppModel::instance()->transactionInfo()->txid(),
                                                            AppModel::instance()->transactionInfo()->psbt(),
                                                            AppModel::instance()->transactionInfo()->memo());
    }
    AppModel::instance()->showToast(0, STR_CPP_101, EWARNING::WarningType::SUCCESS_MSG);
}

void EVT_TRANSACTION_VERIFY_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->startDisplayAddress(AppModel::instance()->walletInfo()->id(), msg.toString());
    }
}

void EVT_TRANSACTION_SEND_PASSPHRASE_HANDLER(QVariant msg) {

}

void EVT_TRANSACTION_CANCEL_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo() && AppModel::instance()->transactionInfo()->roomId() != ""){
        QNunchukRoom* room = CLIENT_INSTANCE->GetRoomById(AppModel::instance()->transactionInfo()->roomId());
        if(room){
            QString init_event_id = AppModel::instance()->transactionInfo()->initEventId();
            QWarningMessage warningmsg;
            matrixbrigde::CancelTransaction(room->id(),
                                            init_event_id,
                                            STR_CPP_077,
                                            warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                AppModel::instance()->requestSyncWalletDb(AppModel::instance()->transactionInfo()->walletId());
                room->startGetPendingTxs();
                if(QEventProcessor::instance()->onsRequester() == E::STATE_ID_SCR_TRANSACTION_HISTORY){
                    QEventProcessor::instance()->sendEvent(E::EVT_TRANSACTION_INFO_BACK_REQUEST);
                }
                else{
                    if((int)ENUNCHUCK::TabSelection::CHAT_TAB == AppModel::instance()->tabIndex()){
                        QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                    }
                    else{
                        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                    }
                }
            }
            else{
                AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
            }
        }
    }
}
