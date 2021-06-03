
#include "STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_ADD_MASTER_SIGNER_RESULT_Entry(QVariant msg) {

}

void SCR_ADD_MASTER_SIGNER_RESULT_Exit(QVariant msg) {
    AppModel::instance()->masterSignerInfo()->setSignature("");
    AppModel::instance()->masterSignerInfo()->setMessage(qUtils::QGenerateRandomMessage());
    AppModel::instance()->setMasterSignerInfo( QSharedPointer<MasterSigner>(new MasterSigner()));
    AppModel::instance()->setWalletsUsingSigner(QStringList());
    if(AppModel::instance()->masterSignerInfo()){
        AppModel::instance()->masterSignerInfo()->warningMessage()->resetWarningMessage();
    }

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

void EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        AppModel::instance()->masterSignerInfo()->warningMessage()->resetWarningMessage();
        AppModel::instance()->startHealthCheckMasterSigner(AppModel::instance()->masterSignerInfo()->id(),
                                                           AppModel::instance()->masterSignerInfo()->message());
    }
}

void EVT_ADD_MASTER_SIGNER_FINISHED_HANDLER(QVariant msg) {
    if((ENUNCHUCK::HealthStatus::SUCCESS == (ENUNCHUCK::HealthStatus)AppModel::instance()->masterSignerInfo()->health() )
            && (AppModel::instance()->masterSignerInfo()->id() != ""))
    {
        QSharedPointer<MasterSignerListModel> mastersigners = bridge::nunchukGetMasterSigners();
        if(mastersigners){
            AppModel::instance()->setMasterSignerList(mastersigners);
        }
        AppModel::instance()->masterSignerList()->setUserCheckedById(false, AppModel::instance()->masterSignerInfo()->id());
        AppModel::instance()->setMasterSignerInfo( QSharedPointer<MasterSigner>(new MasterSigner()));
    }
}

void EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        QString masterSignerId = AppModel::instance()->masterSignerInfo()->id();
        if((ENUNCHUCK::HealthStatus::SUCCESS == (ENUNCHUCK::HealthStatus)AppModel::instance()->masterSignerInfo()->health()) && ("" != masterSignerId) )
        {
            QSharedPointer<MasterSignerListModel> mastersigners = bridge::nunchukGetMasterSigners();
            if(mastersigners){
                AppModel::instance()->setMasterSignerList(mastersigners);
            }

            QSharedPointer<SingleSigner> signer = QSharedPointer<SingleSigner>(new SingleSigner());
            signer.data()->setName(AppModel::instance()->masterSignerInfo()->name());
            signer.data()->setMasterSignerId(masterSignerId);
            signer.data()->setIsRemote(false);
            signer.data()->setIsValid(false);
            signer.data()->setMasterFingerPrint(AppModel::instance()->masterSignerInfo()->device()->masterFingerPrint());
            if(signer){
                AppModel::instance()->newWalletInfo()->singleSignersAssigned()->addSingleSigner(signer);
                AppModel::instance()->masterSignerList()->setUserCheckedById(true, masterSignerId);
            }
        }
    }
}

void EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()->device()){
        QString devicePath = AppModel::instance()->masterSignerInfo()->device()->path();
        QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceByPath(devicePath) ;
        QWarningMessage msgwarning;
        bridge::nunchukPromtPinOnDevice(selectedDv, msgwarning);
    }
}

void EVT_ADD_MASTER_SIGNER_RESULT_SEND_PIN_HANDLER(QVariant msg) {
    QString pinInputted = msg.toString();
    if(AppModel::instance()->masterSignerInfo()->device()){
        QString devicePath = AppModel::instance()->masterSignerInfo()->device()->path();
        QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceByPath(devicePath) ;
        if(selectedDv){
            QWarningMessage msgwarning;
            bridge::nunchukSendPinToDevice(selectedDv, pinInputted, msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                AppModel::instance()->startHealthCheckMasterSigner(AppModel::instance()->masterSignerInfo()->id(),
                                                                   AppModel::instance()->masterSignerInfo()->message());
                AppModel::instance()->startScanDevices();
            }
        }
        else{
            emit AppModel::instance()->sentPINToDeviceResult((int)EWARNING::WarningType::ERROR_MSG);
        }
    }
}
