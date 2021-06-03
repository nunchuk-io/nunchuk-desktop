
#include "STATE_ID_SCR_ADD_WALLET.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_ADD_WALLET_Entry(QVariant msg) {
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    AppModel::instance()->resetSignersChecked();
    if(AppModel::instance()->newWalletInfo()){
        AppModel::instance()->newWalletInfo()->warningMessage()->resetWarningMessage();
    }
}

void SCR_ADD_WALLET_Exit(QVariant msg) {
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->newWalletInfo()){
        AppModel::instance()->newWalletInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_ADD_WALLET_IMPORT_HANDLER(QVariant msg) {
    int importType = msg.toMap().value("importType").toInt();
    QString walletname = msg.toMap().value("walletname").toString();
    QString walletdescription = msg.toMap().value("walletdescription").toString();
    QSharedPointer<Wallet> walletImported;
    if(3 == importType) // Import Via QR Code
    {
        QStringList qrTags = msg.toMap().value("qrTags").toStringList();
        walletImported = bridge::nunchukImportCoboWallet(qrTags, walletdescription);
        walletImported.data()->setCreationMode((int)Wallet::CreationMode::CREATE_BY_IMPORT_QRCODE);

    }
    else{
        QString fileSelected = msg.toMap().value("filePath").toString();
        QString file_path = qUtils::QGetFilePath(fileSelected);
        if(file_path != ""){
            if(0 == importType){
                walletImported = bridge::nunchukImportWallet(file_path);
                walletImported.data()->setCreationMode((int)Wallet::CreationMode::CREATE_BY_IMPORT_DB);
            }
            else if(1 == importType){
                walletImported = bridge::nunchukImportWalletDescriptor(file_path, walletname, walletdescription);
                walletImported.data()->setCreationMode((int)Wallet::CreationMode::CREATE_BY_IMPORT_DESCRIPTOR);
            }
            else{
                walletImported = bridge::nunchukImportWalletConfigFile(file_path, walletdescription);
                walletImported.data()->setCreationMode((int)Wallet::CreationMode::CREATE_BY_IMPORT_CONFIGURATION);
            }
        }
    }

    if(walletImported && walletImported->warningMessage()->type() != (int)EWARNING::WarningType::EXCEPTION_MSG){
        int index = AppModel::instance()->walletList()->addWallet(walletImported);
        if(-1 != index){
            AppModel::instance()->setWalletListCurrentIndex(index);
            if(AppModel::instance()->walletInfo()){
                AppModel::instance()->walletInfo()->setUsedAddressList(bridge::nunchukGetUsedAddresses(AppModel::instance()->walletInfo()->id(), false));
                AppModel::instance()->walletInfo()->setunUsedAddressList(bridge::nunchukGetUnusedAddresses(AppModel::instance()->walletInfo()->id(), false));

                QSharedPointer<TransactionListModel> transactions = bridge::nunchukGetTransactionHistory(AppModel::instance()->walletInfo()->id());
                if(transactions){
                    AppModel::instance()->setTransactionHistory(transactions);
                }
            }
        }
        QQuickViewer::instance()->sendEvent(E::EVT_ADD_WALLET_IMPORT_SUCCEED);
    }
    else{
        AppModel::instance()->setNewWalletInfo(walletImported);
    }
}

void EVT_ADD_WALLET_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST_HANDLER(QVariant msg) {
    QString walletNameInputted = msg.toMap().value("walletNameInputted").toString();
    QString walletDescription  = msg.toMap().value("walletDescription").toString();
    bool walletEscrow = msg.toMap().value("walletEscrow").toBool();
    QString addressType = msg.toMap().value("addressType").toString();
    if(AppModel::instance()->newWalletInfo()){
        AppModel::instance()->newWalletInfo()->setName(walletNameInputted);
        AppModel::instance()->newWalletInfo()->setDescription(walletDescription);
        AppModel::instance()->newWalletInfo()->setEscrow(walletEscrow);
        AppModel::instance()->newWalletInfo()->setAddressType(addressType);
    }
}

void EVT_ADD_WALLET_IMPORT_SUCCEED_HANDLER(QVariant msg) {

}
