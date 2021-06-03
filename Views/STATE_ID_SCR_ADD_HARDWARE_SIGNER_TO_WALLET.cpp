
#include "STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_ADD_HARDWARE_SIGNER_TO_WALLET_Entry(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setSingleSignerInfo(QSharedPointer<SingleSigner>(new SingleSigner()));
    AppModel::instance()->startScanDevices();
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    if(AppModel::instance()->masterSignerInfo()){
        AppModel::instance()->masterSignerInfo()->warningMessage()->resetWarningMessage();
    }
    AppModel::instance()->setAddSignerStep(0);
    AppModel::instance()->setCacheXpubsPercentage(0);
    AppModel::instance()->setAddSignerPercentage(0);
}

void SCR_ADD_HARDWARE_SIGNER_TO_WALLET_Exit(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setAddSignerStep(0);
    AppModel::instance()->setCacheXpubsPercentage(0);
    AppModel::instance()->setAddSignerPercentage(0);
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_HANDLER(QVariant msg) {
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString();
    int deviceSelectedIndex    = msg.toMap().value("deviceIndexSelected").toInt();
    AppModel::instance()->startCreateMasterSigner(signerNameInputted, deviceSelectedIndex);
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_HANDLER(QVariant msg) {
    AppModel::instance()->setSingleSignerInfo(QSharedPointer<SingleSigner>(new SingleSigner()));
    // Trimmed input
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString().simplified();
    QString xpubOrPublInputted = msg.toMap().value("xpubOrPublInputted").toString().simplified();
    QString bip32Inputted = msg.toMap().value("bip32Inputted").toString().simplified();
    QString masterFingerPrintInputted = msg.toMap().value("masterFingerPrintInputted").toString().simplified();

    bool isValidDerivationPath = qUtils::QIsValidDerivationPath(bip32Inputted) ;
    bool isValidFingerPrint = qUtils::QIsValidFingerPrint(masterFingerPrintInputted);
    bool isValidXpub = false;
    bool isValidPublicKey = false;
    bool inputValid = false;

    QString publickey = "";
    QString xpub = "";
    if(AppModel::instance()->newWalletInfo()->escrow()){
        isValidPublicKey = qUtils::QIsValidPublicKey(xpubOrPublInputted);
        if(isValidPublicKey){
            DBG_INFO << "Input is Public key";
            publickey = xpubOrPublInputted;
            xpub = "";
        }
        else{
            isValidXpub = qUtils::QIsValidXPub(xpubOrPublInputted, xpub);
            if(isValidXpub){ publickey = ""; }
        }
        inputValid = isValidDerivationPath && isValidFingerPrint && (isValidXpub || isValidPublicKey);
    }
    else {
        isValidXpub = qUtils::QIsValidXPub(xpubOrPublInputted, xpub);
        if(isValidXpub){ publickey = ""; }
        inputValid = isValidDerivationPath && isValidFingerPrint && isValidXpub;
    }
    if(inputValid){
        QWarningMessage msgwarning;
        QSharedPointer<SingleSigner> ret = bridge::nunchukCreateSigner(signerNameInputted,
                                                                        xpub,
                                                                        publickey,
                                                                        bip32Inputted,
                                                                        masterFingerPrintInputted,
                                                                        msgwarning);

        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && ret){
            ret.data()->setIsValid(true);
            ret.data()->setIsRemote(true);
            AppModel::instance()->setSingleSignerInfo(ret);
            AppModel::instance()->remoteSignerList()->addSingleSigner(ret);
            QQuickViewer::instance()->sendEvent(E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT);
        }
        else{
            ret = QSharedPointer<SingleSigner>(new SingleSigner());
            ret.data()->setName(signerNameInputted);
            ret.data()->setXpub(xpub);
            ret.data()->setPublickey(publickey);
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
       DBG_INFO << "Input doesn't validate";
       if(AppModel::instance()->singleSignerInfo()){
           AppModel::instance()->singleSignerInfo()->setDerivationPath(!isValidDerivationPath ? "false" : "");
           AppModel::instance()->singleSignerInfo()->setMasterFingerPrint(!isValidFingerPrint ? "false" : "");
           AppModel::instance()->singleSignerInfo()->setXpub(!isValidXpub ? "false" : "");
           AppModel::instance()->singleSignerInfo()->setPublickey(!isValidPublicKey ? "false" : "");
           DBG_INFO << AppModel::instance()->singleSignerInfo()->xpub();
       }
    }
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
    if(AppModel::instance()->deviceList()){
        AppModel::instance()->deviceList()->warningMessage()->resetWarningMessage();
    }
    AppModel::instance()->startScanDevices();
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN_HANDLER(QVariant msg) {
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString();
    int deviceSelectedIndex    = msg.toMap().value("deviceIndexSelected").toInt();
    QString pinInputted        = msg.toMap().value("pinInputted").toString();
    if(deviceSelectedIndex >= 0){
        QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(deviceSelectedIndex) ;
        QWarningMessage msgwarning;
        bridge::nunchukSendPinToDevice(selectedDv, pinInputted, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            AppModel::instance()->startCreateMasterSigner(signerNameInputted, deviceSelectedIndex);
            AppModel::instance()->startScanDevices();
        }
    }
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN_HANDLER(QVariant msg) {
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString();
    int deviceSelectedIndex    = msg.toMap().value("deviceIndexSelected").toInt();
    if(deviceSelectedIndex >= 0){
        QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(deviceSelectedIndex) ;
        QWarningMessage msgwarning;
        bridge::nunchukPromtPinOnDevice(selectedDv, msgwarning);
    }
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}


void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

