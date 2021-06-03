
#include "STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_REMOTE_SIGNER_RESULT_Entry(QVariant msg) {

}

void SCR_REMOTE_SIGNER_RESULT_Exit(QVariant msg) {
    if(QQuickViewer::instance()->onsRequester() == E::STATE_ID_SCR_ADD_HARDWARE_SIGNER){
        QSharedPointer<WalletListModel> walletList = bridge::nunchukGetWallets();
        if(walletList){
            AppModel::instance()->setWalletList(walletList);
            if(walletList->rowCount() > 0){
                AppModel::instance()->setWalletListCurrentIndex(AppModel::instance()->walletListCurrentIndex());
                if(AppModel::instance()->walletInfo()){
                    AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
                    QSharedPointer<TransactionListModel> transactions = bridge::nunchukGetTransactionHistory(AppModel::instance()->walletInfo()->id());
                    if(transactions){
                        AppModel::instance()->setTransactionHistory(transactions);
                    }
                }
            }
        }
    }
}

void EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK_HANDLER(QVariant msg) {
    if(AppModel::instance()->singleSignerInfoPtr()){
        QWarningMessage msg;
        int status = (int)bridge::nunchukHealthCheckSingleSigner(AppModel::instance()->singleSignerInfoPtr(), msg);
        AppModel::instance()->singleSignerInfo()->setHealth(status);
    }
}

void EVT_REMOTE_SIGNER_RESULT_EDIT_NAME_HANDLER(QVariant msg) {

}

void EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG_HANDLER(QVariant msg) {
    AppModel::instance()->newWalletInfo()->singleSignersAssigned()->addSingleSigner(AppModel::instance()->singleSignerInfoPtr());
    AppModel::instance()->remoteSignerList()->setUserCheckedByFingerprint(true, AppModel::instance()->singleSignerInfo()->masterFingerPrint());
}

void EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->singleSignerInfo() && (file_path != "")){
        QString signature = bridge::nunchukImportHealthCheckSignature(file_path);
        AppModel::instance()->singleSignerInfo()->setSignature(signature);
    }
}

void EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->singleSignerInfo() && (file_path != "")){
        bridge::nunchukExportHealthCheckMessage(file_path, AppModel::instance()->singleSignerInfo()->message());
    }
}


