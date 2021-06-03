
#include "STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_SOFTWARE_SIGNER_CONFIGURATION_Entry(QVariant msg) {
    AppModel::instance()->setAddSignerPercentage(0);
}

void SCR_SOFTWARE_SIGNER_CONFIGURATION_Exit(QVariant msg) {
    AppModel::instance()->setAddSignerPercentage(0);
}

void EVT_SOFTWARE_SIGNER_REQUEST_CREATE_HANDLER(QVariant msg) {
    QString signername = msg.toMap().value("signername").toString();
    QString passphrase = msg.toMap().value("passphrase").toString();

    QString mnemonic = AppModel::instance()->getMnemonic();
    AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
}

void EVT_ADD_SOFTWARE_SIGNER_RESULT_HANDLER(QVariant msg) {

}

void EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_HANDLER(QVariant msg) {

}

void EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER(QVariant msg) {

}
