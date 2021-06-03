
#include "STATE_ID_SCR_ADD_WALLET_CONFIRMATION.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include <QClipboard>

void SCR_ADD_WALLET_CONFIRMATION_Entry(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->newWalletInfo()){
        AppModel::instance()->newWalletInfo()->warningMessage()->resetWarningMessage();
    }

    if(AppModel::instance()->newWalletInfo()){
        if(AppModel::instance()->walletInfo()->warningMessage()->type() == (int)EWARNING::WarningType::EXCEPTION_MSG){
            AppModel::instance()->newWalletInfo()->setCapableCreate(false);
        }
        else{
            bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
            AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        }
    }
}

void SCR_ADD_WALLET_CONFIRMATION_Exit(QVariant msg) {
    if(AppModel::instance()->newWalletInfo()){
        AppModel::instance()->newWalletInfo()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->newWalletInfo()){
        AppModel::instance()->newWalletInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->newWalletInfo()){
        AppModel::instance()->newWalletInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_ADD_WALLET_CONFIRM_CREATE_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->newWalletInfo()){
        AppModel::instance()->newWalletInfo()->warningMessage()->resetWarningMessage();
    }

    if((int)Wallet::CreationMode::CREATE_NEW_WALLET == AppModel::instance()->newWalletInfoPtr().data()->getCreationMode()){
        DBG_INFO << "Create new wallet";
        QWarningMessage msgWarning;
        QSharedPointer<Wallet> ret = bridge::nunchukCreateWallet(AppModel::instance()->newWalletInfo()->name(),
                                                                  AppModel::instance()->newWalletInfo()->m(),
                                                                  AppModel::instance()->newWalletInfo()->n(),
                                                                  AppModel::instance()->newWalletInfo()->singleSignersAssigned(),
                                                                  (ENUNCHUCK::AddressType)AppModel::instance()->newWalletInfo()->addressType().toInt(),
                                                                  AppModel::instance()->newWalletInfo()->escrow(),
                                                                  AppModel::instance()->newWalletInfo()->description(),
                                                                  msgWarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type() && ret){
            ret.data()->warningMessage()->setWarningMessage(0,
                                                            "Your wallet has been created.",
                                                            EWARNING::WarningType::SUCCESS_MSG,
                                                            "");
            // Check coldcard
            if(AppModel::instance()->masterSignerList() && ret.data()->singleSignersAssigned()){
                QStringList coldcard_lst = AppModel::instance()->masterSignerList()->getColdCardId();
                for (int i = 0; i < coldcard_lst.count(); i++) {
                    ret.data()->singleSignersAssigned()->setIsColdCard(coldcard_lst.at(i));
                }
            }

            int index = AppModel::instance()->walletList()->addWallet(ret);
            AppModel::instance()->resetSignersChecked();

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
            AppModel::instance()->walletList()->requestSort(WalletListModel::WalletRoles::wallet_Name_Role, Qt::AscendingOrder);
        }
        else{
            DBG_INFO << "CAN NOT CREATE WALLET";
            if(AppModel::instance()->walletInfo()){
                AppModel::instance()->walletInfo()->warningMessage()->setWarningMessage(msgWarning.code(),
                                                                                        msgWarning.what(),
                                                                                        (EWARNING::WarningType)msgWarning.type(),
                                                                                        "Can not create Wallet");
            }
            if(AppModel::instance()->newWalletInfo()){
                AppModel::instance()->newWalletInfo()->warningMessage()->setWarningMessage(msgWarning.code(),
                                                                                        msgWarning.what(),
                                                                                        (EWARNING::WarningType)msgWarning.type(),
                                                                                        "Can not create Wallet");
            }
        }
    }
}

void EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setCacheXpubsPercentage(0);
    if(AppModel::instance()->newWalletInfo() && AppModel::instance()->newWalletInfo()->singleSignersAssigned()){
        QSharedPointer<SingleSigner> it = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->getSingleSignerByIndex(msg.toInt());
        AppModel::instance()->startCacheMasterSignerXPub(it.data()->masterSignerId());
    }
}

void EVT_ADD_WALLET_SUCCESSFULLY_HANDLER(QVariant msg) {
}

void EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->walletInfo() && (file_path != "")){
        bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->id(),
                                    file_path,
                                    nunchuk::ExportFormat::BSMS);
    }
}

void EVT_ADD_WALLET_GENERATE_SIGNER_HANDLER(QVariant msg) {
    DBG_INFO << msg.toInt();
    AppModel::instance()->setCacheXpubsPercentage(0);
    if((msg.toInt() >= 0) && AppModel::instance()->newWalletInfo() && AppModel::instance()->newWalletInfo()->singleSignersAssigned()){
        QSharedPointer<SingleSigner> it = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->getSingleSignerByIndex(msg.toInt());

        ENUNCHUCK::WalletType walletType = ENUNCHUCK::WalletType::SINGLE_SIG;
        bool escrow = AppModel::instance()->newWalletInfo()->escrow();
        if(true == escrow){
            walletType = ENUNCHUCK::WalletType::ESCROW;
        }
        else{
            int numberSignerRequired = AppModel::instance()->newWalletInfo()->n();
            if(numberSignerRequired > 1){
                walletType = ENUNCHUCK::WalletType::MULTI_SIG;
            }
        }
        ENUNCHUCK::AddressType addressType = (ENUNCHUCK::AddressType)AppModel::instance()->newWalletInfo()->addressType().toInt();
        QSharedPointer<SingleSigner> signer = bridge::nunchukGetUnusedSignerFromMasterSigner(it.data()->masterSignerId(), walletType, addressType);
        if(signer){
            signer.data()->setIsRemote(false);
            signer.data()->setIsValid(true);
            AppModel::instance()->newWalletInfo()->singleSignersAssigned()->replaceSingleSigner(msg.toInt(), signer);
        }
        else{
            DBG_INFO << " CAN NOT CREATE SIGNER" << it.data()->name() << "FROM MASTER SIGNER";
        }

        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
    }
}

void EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR_HANDLER(QVariant msg) {
    if(QQuickViewer::instance()->onsRequester() != E::STATE_ID_SCR_ADD_WALLET){
        QWarningMessage msgWarning;
        QString ret = bridge::nunchukDraftWallet(AppModel::instance()->newWalletInfo()->name(),
                                                  AppModel::instance()->newWalletInfo()->m(),
                                                  AppModel::instance()->newWalletInfo()->n(),
                                                  AppModel::instance()->newWalletInfo()->singleSignersAssigned(),
                                                  (ENUNCHUCK::AddressType)AppModel::instance()->newWalletInfo()->addressType().toInt(),
                                                  AppModel::instance()->newWalletInfo()->escrow(),
                                                  AppModel::instance()->newWalletInfo()->description(),
                                                  msgWarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
            AppModel::instance()->newWalletInfo()->setDescriptior(ret);
        }
        else{
            AppModel::instance()->newWalletInfo()->setDescriptior(QString("[ERROR][%1]").arg(msgWarning.contentDisplay()));
        }
    }
}

void EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR_HANDLER(QVariant msg) {
    if(!msg.toString().isEmpty()){
        qApp->clipboard()->setText(msg.toString());
    }
}

