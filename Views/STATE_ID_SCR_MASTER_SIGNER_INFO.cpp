
#include "STATE_ID_SCR_MASTER_SIGNER_INFO.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_MASTER_SIGNER_INFO_Entry(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        AppModel::instance()->masterSignerInfo()->warningMessage()->resetWarningMessage();
        if(AppModel::instance()->masterSignerInfo()->isSoftwareSigner()){
            QWarningMessage msgwarning;
            bridge::nunchukClearSignerPassphrase(AppModel::instance()->masterSignerInfo()->id(), msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                QSharedPointer<MasterSignerListModel> mastersigners = bridge::nunchukGetMasterSigners();
                if(mastersigners){
                    AppModel::instance()->setMasterSignerList(mastersigners);
                }
                QSharedPointer<MasterSigner> currentMastersigner = mastersigners.data()->getMasterSignerById(AppModel::instance()->masterSignerInfo()->id());
                AppModel::instance()->masterSignerInfo()->setNeedPassphraseSent(currentMastersigner.data()->needPassphraseSent());
            }
        }
    }
}

void SCR_MASTER_SIGNER_INFO_Exit(QVariant msg) {
    AppModel::instance()->masterSignerInfo()->setSignature("");
    AppModel::instance()->masterSignerInfo()->setMessage(qUtils::QGenerateRandomMessage());
    AppModel::instance()->setMasterSignerInfo( QSharedPointer<MasterSigner>(new MasterSigner()));
    AppModel::instance()->setWalletsUsingSigner(QStringList());
    if(AppModel::instance()->masterSignerInfo()){
        AppModel::instance()->masterSignerInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_MASTER_SIGNER_INFO_EDIT_NAME_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        if(msg.toString() != AppModel::instance()->masterSignerInfo()->name()){
            bridge::nunchukUpdateMasterSigner(AppModel::instance()->masterSignerInfo()->id(), msg.toString());
            AppModel::instance()->updateMasterSignerInfoName(msg.toString());
            if(AppModel::instance()->masterSignerList()){
                AppModel::instance()->masterSignerList()->requestSort(MasterSignerListModel::MasterSignerRoles::master_signer_name_Role, Qt::AscendingOrder);
            }
        }
    }
}

void EVT_MASTER_SIGNER_INFO_HEALTH_CHECK_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        AppModel::instance()->masterSignerInfo()->warningMessage()->resetWarningMessage();
        AppModel::instance()->startHealthCheckMasterSigner(AppModel::instance()->masterSignerInfo()->id(),
                                                           AppModel::instance()->masterSignerInfo()->message());
    }
}

void EVT_MASTER_SIGNER_INFO_BACK_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        AppModel::instance()->masterSignerInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        AppModel::instance()->masterSignerInfo()->warningMessage()->resetWarningMessage();
    }
    if(bridge::nunchukDeleteMasterSigner(msg.toString())){
        QSharedPointer<MasterSignerListModel> mastersigners = bridge::nunchukGetMasterSigners();
        if(mastersigners){
            AppModel::instance()->setMasterSignerList(mastersigners);
        }
        QQuickViewer::instance()->sendEvent(E::EVT_MASTER_SIGNER_INFO_BACK_REQUEST);
        AppModel::instance()->setMasterSignerInfo( QSharedPointer<MasterSigner>(new MasterSigner()));
    }
}

void EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        AppModel::instance()->masterSignerInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_MASTER_SIGNER_INFO_PROMT_PIN_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()->device()){
        QString device_xfp = AppModel::instance()->masterSignerInfo()->device()->masterFingerPrint();

        QSharedPointer<DeviceListModel> deviceList = bridge::nunchukGetDevices();
        if(deviceList){
            bool needPinSent = deviceList.data()->containsNeedPinSent();
            AppModel::instance()->slotFinishScanDevices(deviceList, device_xfp);
            AppModel::instance()->masterSignerInfo()->device()->setNeedsPinSent(needPinSent);
            AppModel::instance()->masterSignerList()->updateDeviceNeedPinSent(device_xfp, needPinSent);
            if(needPinSent){
                QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceNeedPinSent() ;
                QWarningMessage msgwarning;
                bridge::nunchukPromtPinOnDevice(selectedDv, msgwarning);
            }
        }
    }
}

void EVT_MASTER_SIGNER_INFO_SEND_PIN_HANDLER(QVariant msg) {
    QString pinInputted = msg.toString();
    if(AppModel::instance()->masterSignerInfo()->device()){
        QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceNeedPinSent() ;
        if(selectedDv){
            QWarningMessage msgwarning;
            bridge::nunchukSendPinToDevice(selectedDv, pinInputted, msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                AppModel::instance()->startHealthCheckMasterSigner(AppModel::instance()->masterSignerInfo()->id(),
                                                                   AppModel::instance()->masterSignerInfo()->message());
            }
        }
        else{
            emit AppModel::instance()->sentPINToDeviceResult((int)EWARNING::WarningType::ERROR_MSG);
        }
    }
}

void EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE_HANDLER(QVariant msg) {
    if(AppModel::instance()->masterSignerInfo()){
        QString passphraseInput = msg.toMap().value("passphraseInput").toString();
        QString mastersignerId  = msg.toMap().value("mastersignerId").toString();
        QWarningMessage msgwarning;
        bridge::nunchukSendSignerPassphrase( mastersignerId, passphraseInput, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(AppModel::instance()->masterSignerInfo()){
                AppModel::instance()->masterSignerInfo()->setNeedPassphraseSent(false);
            }
            if(AppModel::instance()->masterSignerList()){
                AppModel::instance()->masterSignerList()->updateDeviceNeedPassphraseSentById(mastersignerId ,false);
            }
        }
        DBG_INFO << msgwarning.what();
        AppModel::instance()->masterSignerInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                      msgwarning.what(),
                                                                                      (EWARNING::WarningType)msgwarning.type(),
                                                                                      msgwarning.explaination());
    }
}
