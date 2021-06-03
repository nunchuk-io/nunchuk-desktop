
#include "STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_RECOVER_SOFTWARE_SIGNER_Entry(QVariant msg) {
    AppModel::instance()->setMnemonic("");
    AppModel::instance()->setSuggestMnemonics(bridge::nunchuckGetBIP39WordList());
}

void SCR_RECOVER_SOFTWARE_SIGNER_Exit(QVariant msg) {

}

void EVT_RECOVER_SOFTWARE_SIGNER_REQUEST_HANDLER(QVariant msg) {
    QString mnemonicinput = msg.toString();
    bool checkmnemonic = qUtils::CheckMnemonic(mnemonicinput);
    if(checkmnemonic){
        QQuickViewer::instance()->sendEvent(E::EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED);
        AppModel::instance()->setMnemonic(mnemonicinput);
    }
    else{
        AppModel::instance()->setMnemonic("-101");
    }
}

void EVT_RECOVER_SOFTWARE_SIGNER_BACK_HANDLER(QVariant msg) {

}

void EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED_HANDLER(QVariant msg) {

}

void EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK_HANDLER(QVariant msg) {

}

void EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER(QVariant msg) {

}
