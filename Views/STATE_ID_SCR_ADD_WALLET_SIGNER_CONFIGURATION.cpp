
#include "STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_ADD_WALLET_SIGNER_CONFIGURATION_Entry(QVariant msg) {
    Q_UNUSED(msg);
}

void SCR_ADD_WALLET_SIGNER_CONFIGURATION_Exit(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER_HANDLER(QVariant msg) {
    QList<QVariant> indexSelected = msg.toList();
    for (int i = 0; i < indexSelected.count(); i++) {
        int masterSignerIdx = indexSelected.at(i).toInt();
        QSharedPointer<MasterSigner> ret = AppModel::instance()->   masterSignerList()->getMasterSignerByIndex(masterSignerIdx);
        QSharedPointer<SingleSigner> signer = QSharedPointer<SingleSigner>(new SingleSigner());
        signer.data()->setName(ret.data()->name());
        signer.data()->setMasterSignerId(ret.data()->id());
        signer.data()->setIsRemote(false);
        signer.data()->setIsValid(false);
        signer.data()->setMasterFingerPrint(ret.data()->fingerPrint());
        if(signer){
            AppModel::instance()->newWalletInfo()->singleSignersAssigned()->addSingleSigner(signer);
            AppModel::instance()->masterSignerList()->setUserChecked(true, masterSignerIdx);
            emit AppModel::instance()->newWalletInfo()->nChanged();
        }
    }
}

void EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER_HANDLER(QVariant msg) {
    QList<QVariant> indexSelected = msg.toList();
    for (int i = 0; i < indexSelected.count(); i++) {
        int idx = indexSelected.at(i).toInt();
        QSharedPointer<SingleSigner> signer = AppModel::instance()->remoteSignerList()->getSingleSignerByIndex(idx);
        if(signer){
            signer.data()->setIsRemote(true);
            AppModel::instance()->newWalletInfo()->singleSignersAssigned()->addSingleSigner(signer);
            AppModel::instance()->remoteSignerList()->setUserChecked(true, idx);
            emit AppModel::instance()->newWalletInfo()->nChanged();
        }
    }
}

void EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER_HANDLER(QVariant msg) {
    int signerIndex = msg.toInt();
    QSharedPointer<SingleSigner> signer = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->getSingleSignerByIndex(signerIndex);
    if(signer){
        if(signer.data()->isRemote()){
            AppModel::instance()->remoteSignerList()->setUserCheckedByFingerprint(false, signer->masterFingerPrint());
        }
        else{
            AppModel::instance()->masterSignerList()->setUserCheckedById(false, signer.data()->masterSignerId());
            if(AppModel::instance()->masterSignerList()->getMasterSignerById(signer.data()->masterSignerId())->isSoftwareSigner()){
                QWarningMessage warningmsg;
                bridge::nunchukClearSignerPassphrase(signer.data()->masterSignerId(), warningmsg);
                if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                    QSharedPointer<MasterSignerListModel> mastersigners = bridge::nunchukGetMasterSigners();
                    QSharedPointer<MasterSigner> currentMastersigner = mastersigners.data()->getMasterSignerById(signer.data()->masterSignerId());
                    AppModel::instance()->masterSignerList()->updateDeviceNeedPassphraseSent(signer.data()->masterFingerPrint(),
                                                                                             currentMastersigner.data()->needPassphraseSent());

                }
            }
        }
    }
    AppModel::instance()->newWalletInfo()->singleSignersAssigned()->removeSingleSignerByIndex(signerIndex);

    if(AppModel::instance()->newWalletInfo()->m() > AppModel::instance()->newWalletInfo()->singleSignersAssigned()->rowCount()){
        AppModel::instance()->newWalletInfo()->setM(AppModel::instance()->newWalletInfo()->singleSignersAssigned()->rowCount());
    }

    emit AppModel::instance()->newWalletInfo()->nChanged();
}

void EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK_HANDLER(QVariant msg) {

}

void EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW_HANDLER(QVariant msg) {
    if(AppModel::instance()->newWalletInfo() && AppModel::instance()->newWalletInfo()->singleSignersAssigned()){
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
        for (int i = 0; i < AppModel::instance()->newWalletInfo()->singleSignersAssigned()->rowCount(); i++) {
            QSharedPointer<SingleSigner> it = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->getSingleSignerByIndex(i);
            if(it && !(it.data()->isRemote())){
                if(!(it.data()->isValid())){
                    // Request cached xpub
                    QSharedPointer<SingleSigner> signer = bridge::nunchukGetUnusedSignerFromMasterSigner(it.data()->masterSignerId(), walletType, addressType);
                    if(signer){
                        signer.data()->setIsRemote(false);
                        signer.data()->setIsValid(true);
                        AppModel::instance()->newWalletInfo()->singleSignersAssigned()->replaceSingleSigner(i, signer);
                    }
                    else{
                        DBG_INFO << " CAN NOT CREATE SIGNER" << it.data()->name() << "FROM MASTER SIGNER";
                    }
                }
                else{
                    DBG_INFO << " SIGNER" << it.data()->name() << "ALREADY CREATED FROM MASTER SIGNER";
                }
            }
        }

        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
    }
}

void EVT_ADD_SIGNER_TO_WALLET_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE_HANDLER(QVariant msg) {
    if(AppModel::instance()->newWalletInfo()){
        QString passphraseInput = msg.toMap().value("passphraseInput").toString();
        QString mastersignerId  = msg.toMap().value("mastersignerId").toString();
        QWarningMessage msgwarning;
        bridge::nunchukSendSignerPassphrase( mastersignerId, passphraseInput, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(AppModel::instance()->masterSignerList()){
                AppModel::instance()->masterSignerList()->updateDeviceNeedPassphraseSentById(mastersignerId, false);
            }
        }
        AppModel::instance()->newWalletInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                   msgwarning.what(),
                                                                                   (EWARNING::WarningType)msgwarning.type(),
                                                                                   msgwarning.explaination());
    }
}
