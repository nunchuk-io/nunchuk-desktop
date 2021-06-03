
#include "STATE_ID_SCR_HOME.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include <QClipboard>

void SCR_HOME_Entry(QVariant msg) {
    QSharedPointer<MasterSignerListModel> mastersigners = bridge::nunchukGetMasterSigners();
    if(mastersigners){
        AppModel::instance()->setMasterSignerList(mastersigners);
    }

    QSharedPointer<SingleSignerListModel> remoteSigners = bridge::nunchukGetRemoteSigners();
    if(remoteSigners){
        AppModel::instance()->setRemoteSignerList(remoteSigners);
    }

    QSharedPointer<WalletListModel> walletList = bridge::nunchukGetWallets();
    if(walletList){
        AppModel::instance()->setWalletList(walletList);
        if(walletList->rowCount() > 0){
            QString lastWalletId = bridge::nunchukGetSelectedWallet();
            int lastIndex = -1;
            if(lastWalletId != ""){
                lastIndex = AppModel::instance()->walletList()->getWalletIndexById(lastWalletId);
            }
            AppModel::instance()->setWalletListCurrentIndex(lastIndex);
            if(AppModel::instance()->walletInfo()){
                AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
                AppModel::instance()->walletInfo()->setUsedAddressList(bridge::nunchukGetUsedAddresses(AppModel::instance()->walletInfo()->id(), false));
                AppModel::instance()->walletInfo()->setunUsedAddressList(bridge::nunchukGetUnusedAddresses(AppModel::instance()->walletInfo()->id(), false));
                QSharedPointer<TransactionListModel> transactions = bridge::nunchukGetTransactionHistory(AppModel::instance()->walletInfo()->id());
                if(transactions){
                    AppModel::instance()->setTransactionHistory(transactions);
                }
            }
        }
    }
}

void SCR_HOME_Exit(QVariant msg) {

}

void EVT_HOME_WALLET_SELECTED_HANDLER(QVariant msg) {
    if(msg.toInt() >= 0){
        AppModel::instance()->setWalletListCurrentIndex(msg.toInt());
        if(AppModel::instance()->walletInfo()){
            QString wallet_id = AppModel::instance()->walletInfo()->id();
            AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
            AppModel::instance()->walletInfo()->setUsedAddressList(bridge::nunchukGetUsedAddresses(wallet_id, false));
            AppModel::instance()->walletInfo()->setunUsedAddressList(bridge::nunchukGetUnusedAddresses(wallet_id, false));
            QSharedPointer<TransactionListModel> transactions = bridge::nunchukGetTransactionHistory(wallet_id);
            if(transactions){
                AppModel::instance()->setTransactionHistory(transactions);
            }
            bool setLast = bridge::nunchukSetSelectedWallet(wallet_id);
            DBG_INFO << AppModel::instance()->walletInfo()->name() << setLast ;
        }
    }
}

void EVT_HOME_ADD_WALLET_REQUEST_HANDLER(QVariant msg) {
    QSharedPointer<Wallet> newWallet(new Wallet());
    newWallet.data()->setCapableCreate(false);
    AppModel::instance()->setNewWalletInfo(newWallet);
    AppModel::instance()->resetSignersChecked();
    AppModel::instance()->setSingleSignerInfo(QSharedPointer<SingleSigner>(new SingleSigner()));
    QQuickViewer::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_ADD_WALLET);
}

void EVT_HOME_MASTER_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setMasterSignerInfo(msg.toInt());
    QString masterSignerId = AppModel::instance()->masterSignerInfo()->id();
    AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByMasterSigner(masterSignerId));
}

void EVT_HOME_REMOTE_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    QSharedPointer<SingleSigner> it = AppModel::instance()->remoteSignerList()->getSingleSignerByIndex(msg.toInt());
    if(it) {
        AppModel::instance()->setSingleSignerInfo(it);
        AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByFingerPrint(it.data()->masterFingerPrint()));
    }
}

void EVT_HOME_SEND_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QSharedPointer<UTXOListModel> utxos = bridge::nunchukGetUnspentOutputs(AppModel::instance()->walletInfo()->id());
        if(utxos){
            AppModel::instance()->setUtxoList(utxos);
        }
    }
}

void EVT_HOME_RECEIVE_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_TRANSACTION_HISTORY_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_WALLET_INFO_REQUEST_HANDLER(QVariant msg) {

}

void EVT_APP_SETTING_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_WALLET_COPY_ADDRESS_HANDLER(QVariant msg) {
    if(!msg.toString().isEmpty()){
        qApp->clipboard()->setText(msg.toString());
    }
}

void EVT_HOME_TRANSACTION_INFO_REQUEST_HANDLER(QVariant msg) {
    if(msg.toString() != ""){
        QSharedPointer<Transaction> it = AppModel::instance()->getTransactionByTxid(msg.toString());
        AppModel::instance()->setTransactionInfo(it);
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_HOME_SETTING_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_DISPLAY_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->startDisplayAddress(AppModel::instance()->walletInfo()->id(),
                                                  msg.toString(),
                                                  "");
    }
}

void EVT_HOME_ADD_NEW_SIGNER_REQUEST_HANDLER(QVariant msg) {
    QQuickViewer::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_ADD_SIGNER);
}


