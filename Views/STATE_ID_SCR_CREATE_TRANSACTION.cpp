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
#include "STATE_ID_SCR_CREATE_TRANSACTION.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include "Chats/matrixbrigde.h"
#include "Chats/ClientController.h"
#include "localization/STR_CPP.h"
#include "Servers/Draco.h"

void SCR_CREATE_TRANSACTION_Entry(QVariant msg) {
    AppModel::instance()->startGetEstimatedFee();
    Q_UNUSED(msg);
}

void SCR_CREATE_TRANSACTION_Exit(QVariant msg) {
    AppModel::instance()->setTxidReplacing("");
}

void EVT_CREATE_TRANSACTION_SIGN_REQUEST_HANDLER(QVariant msg) {
    bool subtractFromFeeAmout = msg.toMap().value("subtractFromFeeAmout").toBool();
    qint64 feeRate = msg.toMap().value("feeRate").toDouble()*1000; // Convert sats/Byte to sats/kB
    QTransactionPtr transaction = AppModel::instance()->transactionInfoPtr();
    if(transaction){
        QString wallet_id = transaction.data()->walletId();
        if(AppModel::instance()->getTxidReplacing() != "") {
            DBG_INFO << "REPLACE BY FEE REQUEST " << "new fee: " << feeRate;
            nunchuk::Transaction current = transaction.data()->nunchukTransaction();
            std::vector<nunchuk::TxOutput> outputs = current.get_user_outputs();
            std::vector<nunchuk::TxInput> inputs = current.get_inputs();
            subtractFromFeeAmout = current.subtract_fee_from_amount();
            std::string replace_id = current.get_txid();
            QWarningMessage msgwarning;
            nunchuk::Transaction draftrans = bridge::nunchukDraftOriginTransaction(wallet_id.toStdString(),
                                                                                   outputs,
                                                                                   inputs,
                                                                                   feeRate,
                                                                                   subtractFromFeeAmout,
                                                                                   replace_id,
                                                                                   msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()) {
                msgwarning.resetWarningMessage();
                QTransactionPtr trans = bridge::nunchukReplaceTransaction(wallet_id,
                                                                          AppModel::instance()->getTxidReplacing(),
                                                                          feeRate,
                                                                          msgwarning);
                if(trans && (int)EWARNING::WarningType::NONE_MSG == msgwarning.type()) {
                    if (auto wallet = AppModel::instance()->walletListPtr()->getWalletById(wallet_id)) {
                        wallet->RbfAsisstedTxs(trans->txid(), trans->psbt());
                    }
                    AppModel::instance()->setTransactionInfo(trans);
                    AppModel::instance()->requestSyncWalletDb(wallet_id);
                    QEventProcessor::instance()->sendEvent(E::EVT_CREATE_TRANSACTION_SIGN_SUCCEED);
                    AppModel::instance()->setTxidReplacing("");
                    QString msg_name = QString("The transaction has been replaced");
                    AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                }
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
            }
        }
        else
        {
            bool manualFee = msg.toMap().value("manualFee").toBool();
            bool manualOutput = msg.toMap().value("manualOutput").toBool();
            if(!manualFee){ // Auto fee = hourFee by recommendation from API
                feeRate = AppModel::instance()->hourFeeOrigin();
            }
            DBG_INFO << "CREATE NEW TRANSACTION"
                     << "subtract:" << subtractFromFeeAmout
                     << "| manual Output:" << manualOutput
                     << "| manual Fee:" << manualFee
                     << "| free rate:" << feeRate;

            QUTXOListModelPtr inputs = QUTXOListModelPtr(new QUTXOListModel(AppModel::instance()->walletInfo()->id()));
            if(transaction->inputCoins()){
                for (int i = 0; i < transaction->inputCoins()->rowCount(); i++) {
                    QUTXOPtr it = transaction->inputCoins()->getUTXOByIndex(i);
                    if(it.data()){
                        DBG_INFO << "UTXO Selected:" << it.data()->txid() << it.data()->amountSats();
                        inputs->addUTXO(it.data()->getUnspentOutput());
                    }
                }
            }

            QMap<QString, qint64> outputs;
            if(AppModel::instance()->destinationList()){
                outputs = AppModel::instance()->destinationList()->getOutputs();
            }
            QString memo = transaction.data()->memo();
            QWalletPtr wallet = AppModel::instance()->walletInfoPtr();
            if(wallet){
                if(wallet.data()->isSharedWallet()){
                    QString room_id = CLIENT_INSTANCE->getRoomIdByWalletId(wallet_id);
                    if(room_id != ""){
                        QWarningMessage msginit;
                        QNunchukMatrixEvent evtmatrix = matrixbrigde::InitTransaction(room_id,
                                                                                      outputs,
                                                                                      memo,
                                                                                      inputs,
                                                                                      feeRate,
                                                                                      subtractFromFeeAmout,
                                                                                      msginit);
                        if((int)ENUNCHUCK::TabSelection::CHAT_TAB == AppModel::instance()->tabIndex()){
                            QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                        }
                        else{
                            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                        }
                        if((int)EWARNING::WarningType::NONE_MSG == msginit.type()){
                            AppModel::instance()->requestSyncWalletDb(wallet_id);
                        }
                        else{
                            AppModel::instance()->showToast(msginit.code(), msginit.what(), (EWARNING::WarningType)msginit.type());
                        }
                    }
                }
                else{
                    QWarningMessage msgwarning;
                    QString unUseAddress = msg.toMap().value("unUseAddress").toString();
                    DBG_INFO << "unUseAddress: " << unUseAddress;
                    QTransactionPtr trans = NULL;
                    if (unUseAddress.isEmpty()) {
                        trans = bridge::nunchukCreateTransaction(wallet_id,
                                                                 outputs,
                                                                 memo,
                                                                 inputs,
                                                                 feeRate,
                                                                 subtractFromFeeAmout,
                                                                 {},
                                                                 msgwarning);
                    }
                    else {
                        QString unUseAddress = msg.toMap().value("unUseAddress").toString();
                        trans = bridge::nunchukCancelCreateTransaction(wallet_id,
                                                                       transaction->nunchukTransaction(),
                                                                       unUseAddress,
                                                                       memo,
                                                                       feeRate,
                                                                       transaction->txid(),
                                                                       msgwarning);
                    }
                    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                        if(trans){
                            AppModel::instance()->setTransactionInfo(trans);
                            wallet.data()->AssignTagsToTxChange();
                            if(wallet.data()->isAssistedWallet()){
                                wallet.data()->CreateAsisstedTxs(trans->txid(), trans->psbt(), trans->memo());
                            }
                            AppModel::instance()->requestSyncWalletDb(wallet_id);
                            QEventProcessor::instance()->sendEvent(E::EVT_CREATE_TRANSACTION_SIGN_SUCCEED);
                            if (!unUseAddress.isEmpty()) {
                                QString msg_name = QString("The transaction has been replaced");
                                AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                            }
                        }
                    }
                    else{
                        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
                    }
                }
            }
        }
    }
}

void EVT_CREATE_TRANSACTION_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_CREATE_TRANSACTION_SAVE_REQUEST_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if (file_path != ""){
        QWarningMessage msgwarning;
        bool ret = bridge::nunchukExportTransaction(AppModel::instance()->walletInfo()->id(),
                                                    AppModel::instance()->transactionInfo()->txid(),
                                                    file_path,
                                                    msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG != msgwarning.type()){
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }
        DBG_INFO << file_path << ret;
    }
}

void EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->utxoList()){
        int sortRole = msg.toMap().value("sortRole").toInt();
        int sortOrder = msg.toMap().value("sortOrder").toInt();
        AppModel::instance()->walletInfo()->utxoList()->requestSort(sortRole, sortOrder);
    }
}

void EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX_HANDLER(QVariant msg) {
    if (auto w = AppModel::instance()->walletInfo()) {
        w->UpdateDraftTransaction(msg);
    }
}

void EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE_HANDLER(QVariant msg) {

}

void EVT_CREATE_TRANSACTION_SIGN_SUCCEED_HANDLER(QVariant msg) {

}


