
#include "STATE_ID_SCR_TRANSACTION_INFO.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"

void SCR_TRANSACTION_INFO_Entry(QVariant msg) {
    // Update usable
    AppModel::instance()->checkDeviceUsableToSign();
    if(AppModel::instance()->deviceList()){
        if(AppModel::instance()->transactionInfo()){
            AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
            if(AppModel::instance()->transactionInfo()->status() == (int)nunchuk::TransactionStatus::PENDING_SIGNATURES){
                AppModel::instance()->startScanDevices();
            }
        }
    }
}

void SCR_TRANSACTION_INFO_Exit(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
}

void EVT_TRANSACTION_SIGN_REQUEST_HANDLER(QVariant msg) {
    bool isSoftware = msg.toMap().value("isSoftware").toBool();
    int deviceIndx = msg.toMap().value("deviceIndx").toInt();
    DBG_INFO << isSoftware << deviceIndx;
    if(AppModel::instance()->walletInfo() && AppModel::instance()->transactionInfo()){
        AppModel::instance()->startSigningTransaction(AppModel::instance()->walletInfo()->id(),
                                                      AppModel::instance()->transactionInfo()->txid(),
                                                      deviceIndx,
                                                      isSoftware);
    }
}

void EVT_TRANSACTION_EXPORT_REQUEST_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(file_path != ""){
        bool ret = bridge::nunchukExportTransaction(AppModel::instance()->walletInfo()->id(),
                                                     AppModel::instance()->transactionInfo()->txid(),
                                                     file_path);
        DBG_INFO << ret << file_path;
    }
}

void EVT_TRANSACTION_IMPORT_REQUEST_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if (file_path != ""){
        QSharedPointer<Transaction> trans = bridge::nunchukImportTransaction(AppModel::instance()->walletInfo()->id(), file_path);
        if(trans){
            AppModel::instance()->setTransactionInfo(trans);
            if(AppModel::instance()->transactionHistory()){
                AppModel::instance()->transactionHistory()->replaceTransaction(trans.data()->txid(), trans);
            }
            if(AppModel::instance()->transactionHistoryShort()){
                AppModel::instance()->transactionHistoryShort()->replaceTransaction(trans.data()->txid(), trans);
            }
        }
    }
}

void EVT_TRANSACTION_BROADCAST_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->walletInfo() && AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
        QString wallet_id = AppModel::instance()->walletInfo()->id();

        QWarningMessage msgwarning;
        QSharedPointer<Transaction> trans = bridge::nunchukBroadcastTransaction(wallet_id,
                                                                                 AppModel::instance()->transactionInfo()->txid(),
                                                                                 msgwarning);

        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(trans){
                AppModel::instance()->setTransactionInfo(trans);

                // Get transaction history
                QSharedPointer<TransactionListModel> transactions = bridge::nunchukGetTransactionHistory(wallet_id);
                if(transactions){
                    AppModel::instance()->setTransactionHistory(transactions);
                }

                // Update wallet info
                QSharedPointer<Wallet> wallet_result = bridge::nunchukGetWallet(wallet_id);
                if(wallet_result){
                    AppModel::instance()->walletList()->updateBalance(wallet_id, wallet_result.data()->balanceSats());
                }

                // Update address
                if(AppModel::instance()->walletList()){
                    AppModel::instance()->walletList()->updateAddress(wallet_id,
                                                                      bridge::nunchukGetUsedAddresses(wallet_id, false),
                                                                      bridge::nunchukGetUnusedAddresses(wallet_id, false));
                }
                AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(0,
                                                                                             "The Transaction has been broadcast successfully and pending confirmations.",
                                                                                             EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        else{
            AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                         msgwarning.what(),
                                                                                         (EWARNING::WarningType)msgwarning.type(),
                                                                                         "Cannot broadcast transaction");
        }
    }
}

void EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
        DBG_INFO << AppModel::instance()->transactionInfo()->txid();
        AppModel::instance()->setTxidReplacing(AppModel::instance()->transactionInfo()->txid());
    }
}

void EVT_TRANSACTION_SET_MEMO_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo() && AppModel::instance()->transactionInfo() && msg.toString() != ""){
        bridge::nunchukUpdateTransactionMemo(AppModel::instance()->walletInfo()->id(),
                                              AppModel::instance()->transactionInfo()->txid(),
                                              msg.toString());
        if(AppModel::instance()->transactionHistory()){
            AppModel::instance()->transactionHistory()->updateTransactionMemo(AppModel::instance()->transactionInfo()->txid(),
                                                                              msg.toString());
        }
        if(AppModel::instance()->transactionHistoryShort()){
            AppModel::instance()->transactionHistoryShort()->updateTransactionMemo(AppModel::instance()->transactionInfo()->txid(),
                                                                              msg.toString());
        }
    }
}

void EVT_TRANSACTION_INFO_BACK_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_TRANSACTION_SCAN_DEVICE_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    AppModel::instance()->startScanDevices();
}

void EVT_TRANSACTION_REMOVE_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->walletInfo() && AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
        QString txid = AppModel::instance()->transactionInfo()->txid();
        bool ret = bridge::nunchukDeleteTransaction(AppModel::instance()->walletInfo()->id(), txid);
        if(ret){
            if(AppModel::instance()->transactionHistoryShort()){
                AppModel::instance()->transactionHistoryShort()->removeTransaction(txid);
            }
            if(AppModel::instance()->transactionHistory()){
                AppModel::instance()->transactionHistory()->removeTransaction(txid);
            }
            if(QQuickViewer::instance()->onsRequester() == E::STATE_ID_SCR_TRANSACTION_HISTORY){
                QQuickViewer::instance()->sendEvent(E::EVT_TRANSACTION_INFO_BACK_REQUEST);
            }
            else{
                QQuickViewer::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
            }
            AppModel::instance()->setTransactionInfo( QSharedPointer<Transaction>(new Transaction()));
        }
    }
}

void EVT_TRANSACTION_INFO_BACK_UTXO_INFO_HANDLE(QVariant msg) {

}

void EVT_TRANSACTION_SEND_PIN_REQUEST_HANDLER(QVariant msg) {
    int deviceSelectedIndex    = msg.toMap().value("deviceIndexSelected").toInt();
    QString pinInputted        = msg.toMap().value("pinInputted").toString();
    if(deviceSelectedIndex >= 0 && AppModel::instance()->deviceList() && AppModel::instance()->transactionInfo()){
        QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(deviceSelectedIndex) ;
        if(selectedDv){
            QWarningMessage msgwarning;
            bridge::nunchukSendPinToDevice(selectedDv, pinInputted, msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                AppModel::instance()->startSigningTransaction(AppModel::instance()->walletInfo()->id(),
                                                              AppModel::instance()->transactionInfo()->txid(),
                                                              msg.toInt(),
                                                              false);
                AppModel::instance()->startScanDevices();
            }
        }
        else{
            emit AppModel::instance()->sentPINToDeviceResult((int)EWARNING::WarningType::ERROR_MSG);
        }
    }
}

void EVT_TRANSACTION_PROMT_PIN_REQUEST_HANDLER(QVariant msg) {
    int deviceSelectedIndex    = msg.toInt();
    if(deviceSelectedIndex >= 0){
        QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(deviceSelectedIndex) ;
        QWarningMessage msgwarning;
        bridge::nunchukPromtPinOnDevice(selectedDv, msgwarning);
    }
}

void EVT_TRANSACTION_EXPORT_QRCODE_HANDLER(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
    if(AppModel::instance()->walletInfo() && AppModel::instance()->transactionInfo()){
        QWarningMessage msgwarning;
        QStringList qrtags = bridge::nunchukExportCoboTransaction(AppModel::instance()->walletInfo()->id(),
                                                                   AppModel::instance()->transactionInfo()->txid(),
                                                                   msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            AppModel::instance()->setQrExported(qrtags);
        }
    }
}

void EVT_TRANSACTION_IMPORT_QRCODE_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->id();
        QStringList qrtags;
        qrtags.clear();
        for(QVariant i : msg.toList()){
            qrtags.append(i.toString());
        }
        QWarningMessage msgwarning;
        QSharedPointer<Transaction> trans = bridge::nunchukImportCoboTransaction(wallet_id, qrtags, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(trans){
                AppModel::instance()->setTransactionInfo(trans);
                // Get transaction history
                QSharedPointer<TransactionListModel> transactions = bridge::nunchukGetTransactionHistory(wallet_id);
                if(transactions){
                    AppModel::instance()->setTransactionHistory(transactions);
                }
            }
        }
    }
}

void EVT_TRANSACTION_VERIFY_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->startDisplayAddress(AppModel::instance()->walletInfo()->id(),
                                                  msg.toString(),
                                                  "");
    }
}

void EVT_TRANSACTION_SEND_PASSPHRASE_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        QString passphraseInput = msg.toMap().value("passphraseInput").toString();
        QString mastersignerId  = msg.toMap().value("mastersignerId").toString();
        DBG_INFO << mastersignerId << passphraseInput;

        QWarningMessage msgwarning;
        bridge::nunchukSendSignerPassphrase( mastersignerId, passphraseInput, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(AppModel::instance()->softwareSignerDeviceList()){
                AppModel::instance()->softwareSignerDeviceList()->updateDeviceNeedPassphraseSentByMasterSignerId(mastersignerId, false);
            }
            if(AppModel::instance()->masterSignerList()){
                AppModel::instance()->masterSignerList()->updateDeviceNeedPassphraseSentById(mastersignerId ,false);
            }
        }
        AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                     msgwarning.what(),
                                                                                     (EWARNING::WarningType)msgwarning.type(),
                                                                                     msgwarning.explaination());
    }
}
