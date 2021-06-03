
#include "STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_ADD_NEW_SOFTWARE_SIGNER_Entry(QVariant msg) {
    QString mnemonic = qUtils::GenerateMnemonic();
    AppModel::instance()->setMnemonic(mnemonic);
}

void SCR_ADD_NEW_SOFTWARE_SIGNER_Exit(QVariant msg) {

}

void EVT_ADD_NEW_SOFTWARE_SIGNER_BACK_HANDLER(QVariant msg) {
    AppModel::instance()->setMnemonic("");
}

void EVT_CREATE_NEW_SEED_HANDLER(QVariant msg) {

}

void EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK_HANDLER(QVariant msg) {

}

void EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER(QVariant msg) {

}
