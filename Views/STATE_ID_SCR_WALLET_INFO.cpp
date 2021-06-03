
#include "STATE_ID_SCR_WALLET_INFO.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_WALLET_INFO_Entry(QVariant msg) {

}

void SCR_WALLET_INFO_Exit(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
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
            AppModel::instance()->walletList()->requestSort(WalletListModel::WalletRoles::wallet_Name_Role, Qt::AscendingOrder);
        }
    }
}

void EVT_WALLET_INFO_REMOVE_HANDLER(QVariant msg) {
    if(bridge::nunchukDeleteWallet(msg.toString())){
        AppModel::instance()->removeWallet(msg.toString());
        QQuickViewer::instance()->sendEvent(E::EVT_WALLET_INFO_BACK_REQUEST);
        AppModel::instance()->setWalletListCurrentIndex(0);
        if(AppModel::instance()->walletInfo()){
            AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
            QSharedPointer<TransactionListModel> transactions = bridge::nunchukGetTransactionHistory(AppModel::instance()->walletInfo()->id());
            if(transactions){
                AppModel::instance()->setTransactionHistory(transactions);
            }
        }
    }
}

void EVT_WALLET_INFO_BACK_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_WALLET_INFO_UTXOS_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QSharedPointer<UTXOListModel> utxos = bridge::nunchukGetUnspentOutputs(AppModel::instance()->walletInfo()->id());
        AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
        if(utxos){
            AppModel::instance()->setUtxoList(utxos);
        }
    }
}

void EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->setUsedChangeAddressList(bridge::nunchukGetUsedAddresses(AppModel::instance()->walletInfo()->id(), true));
        AppModel::instance()->walletInfo()->setUnUsedChangeddAddressList(bridge::nunchukGetUnusedAddresses(AppModel::instance()->walletInfo()->id(), true));
        AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_WALLET_INFO_EXPORT_WALLET_REQUEST_HANDLER(QVariant msg) {

}

void EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO;
}

void EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO;
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
        QSharedPointer<SingleSigner> it = AppModel::instance()->walletInfo()->singleSignersAssigned()->getSingleSignerByIndex(msg.toInt());
        if(it)
        {
            AppModel::instance()->setSingleSignerInfo(it);
            if(it.data()->isRemote()){
                AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByFingerPrint(it.data()->masterFingerPrint()));
                QQuickViewer::instance()->sendEvent(E::EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST);
            }
            else{
                if(AppModel::instance()->masterSignerList()){
                    QSharedPointer<MasterSigner> localsigner = AppModel::instance()->masterSignerList()->getMasterSignerById(it.data()->masterSignerId());
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
    if(msg.toString() == ""){
        if(AppModel::instance()->walletInfo()){
            AppModel::instance()->setQrExported(QStringList());
            QWarningMessage msgwarning;
            QStringList qrtags = bridge::nunchukExportCoboWallet(AppModel::instance()->walletInfo()->id(), msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                AppModel::instance()->setQrExported(qrtags);
            }
        }
    }
    else{
        QString file_path = qUtils::QGetFilePath(msg.toString());
        if(AppModel::instance()->walletInfo() && (file_path != "")){
            bool ret = bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->id(),
                                                    file_path,
                                                    nunchuk::ExportFormat::COBO);
            DBG_INFO << file_path << ret;
        }
        if(AppModel::instance()->walletInfo()){
            AppModel::instance()->setQrExported(QStringList());
            QWarningMessage msgwarning;
            QStringList qrtags = bridge::nunchukExportCoboWallet(AppModel::instance()->walletInfo()->id(), msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                AppModel::instance()->setQrExported(qrtags);
            }
        }
    }
}

