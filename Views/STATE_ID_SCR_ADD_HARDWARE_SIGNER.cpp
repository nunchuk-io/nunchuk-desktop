
#include "STATE_ID_SCR_ADD_HARDWARE_SIGNER.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_ADD_HARDWARE_SIGNER_Entry(QVariant msg) {
    Q_UNUSED(msg);
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    AppModel::instance()->startScanDevices();
    AppModel::instance()->setAddSignerStep(0);
    AppModel::instance()->setCacheXpubsPercentage(0);
    AppModel::instance()->setAddSignerPercentage(0);
    AppModel::instance()->checkDeviceUsableToAdd();
}

void SCR_ADD_HARDWARE_SIGNER_Exit(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setAddSignerStep(0);
    AppModel::instance()->setCacheXpubsPercentage(0);
    AppModel::instance()->setAddSignerPercentage(0);

    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
}

void EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST_HANDLER(QVariant msg) {
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString();
    int deviceSelectedIndex    = msg.toMap().value("deviceIndexSelected").toInt();
    AppModel::instance()->startCreateMasterSigner(signerNameInputted, deviceSelectedIndex);
}

void EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setSingleSignerInfo(QSharedPointer<SingleSigner>(new SingleSigner()));
    // Trimmed input
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString().simplified();
    QString xpubInputted = msg.toMap().value("xpubInputted").toString().simplified();
    QString bip32Inputted = msg.toMap().value("bip32Inputted").toString().simplified();
    QString masterFingerPrintInputted = msg.toMap().value("masterFingerPrintInputted").toString().simplified();
    bool isValidFingerPrint = false;

    // Adding a Remote Signer whose Master Signer already exists: disallowed
    if(AppModel::instance()->masterSignerList() && AppModel::instance()->masterSignerList()->containsFingerPrint(masterFingerPrintInputted)){
        isValidFingerPrint = false;
    }
    else{
        isValidFingerPrint = qUtils::QIsValidFingerPrint(masterFingerPrintInputted);
    }
    bool isValidDerivationPath = qUtils::QIsValidDerivationPath(bip32Inputted);
    QString xpubOutput = "";
    bool isValidXpub = qUtils::QIsValidXPub(xpubInputted, xpubOutput);
    bool inputValid = isValidDerivationPath && isValidFingerPrint && isValidXpub;
    if(inputValid){
        QWarningMessage msgwarning;
        QSharedPointer<SingleSigner> ret = bridge::nunchukCreateSigner(signerNameInputted,
                                                                        xpubOutput,
                                                                        "",
                                                                        bip32Inputted,
                                                                        masterFingerPrintInputted,
                                                                        msgwarning);

        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && ret){
            ret.data()->setIsValid(true);
            ret.data()->setIsRemote(true);
            AppModel::instance()->setSingleSignerInfo(ret);
            QSharedPointer<SingleSignerListModel> remoteSigners = bridge::nunchukGetRemoteSigners();
            if(remoteSigners){
                AppModel::instance()->setRemoteSignerList(remoteSigners);
            }
            QQuickViewer::instance()->sendEvent(E::EVT_ADD_REMOTE_SIGNER_RESULT);
        }
        else{
            ret = QSharedPointer<SingleSigner>(new SingleSigner());
            ret.data()->setName(signerNameInputted);
            ret.data()->setXpub(xpubOutput);
            ret.data()->setPublickey("");
            ret.data()->setDerivationPath(bip32Inputted);
            ret.data()->setMasterFingerPrint(masterFingerPrintInputted);
            ret.data()->setIsValid(false);
            ret.data()->setIsRemote(true);
            AppModel::instance()->singleSignerInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                          msgwarning.what(),
                                                                                          (EWARNING::WarningType)msgwarning.type(),
                                                                                          "Cannot create signer");
        }
    }
    else{
       if(AppModel::instance()->singleSignerInfo()){
           AppModel::instance()->singleSignerInfo()->setDerivationPath(!isValidDerivationPath ? "false" : "");
           AppModel::instance()->singleSignerInfo()->setMasterFingerPrint(!isValidFingerPrint ? "false" : "");
           AppModel::instance()->singleSignerInfo()->setXpub(!isValidXpub ? "false" : "");
           AppModel::instance()->singleSignerInfo()->setPublickey("");
       }
    }
}

void EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ADD_MASTER_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    AppModel::instance()->startScanDevices();
}

void EVT_ADD_REMOTE_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST_HANDLER(QVariant msg) {
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString();
    int deviceSelectedIndex    = msg.toMap().value("deviceIndexSelected").toInt();
    QString pinInputted        = msg.toMap().value("pinInputted").toString();
    if(deviceSelectedIndex >= 0){
        QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(deviceSelectedIndex) ;
        if(selectedDv){
            QWarningMessage msgwarning;
            bridge::nunchukSendPinToDevice(selectedDv, pinInputted, msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                AppModel::instance()->startCreateMasterSigner(signerNameInputted, deviceSelectedIndex);
                AppModel::instance()->startScanDevices();
            }
        }
        else{
            emit AppModel::instance()->sentPINToDeviceResult((int)EWARNING::WarningType::ERROR_MSG);
        }
    }
}

void EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST_HANDLER(QVariant msg) {
    int deviceSelectedIndex    = msg.toMap().value("deviceIndexSelected").toInt();
    if(deviceSelectedIndex >= 0){
        QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(deviceSelectedIndex) ;
        QWarningMessage msgwarning;
        bridge::nunchukPromtPinOnDevice(selectedDv, msgwarning);
    }
}
