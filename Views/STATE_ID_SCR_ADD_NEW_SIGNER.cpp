
#include "STATE_ID_SCR_ADD_NEW_SIGNER.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_ADD_NEW_SIGNER_Entry(QVariant msg) {

}

void SCR_ADD_NEW_SIGNER_Exit(QVariant msg) {

}

void EVT_ADD_NEW_SIGNER_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED_HANDLER(QVariant msg) {
    AppModel::instance()->setMnemonic("");
}

void EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED_HANDLER(QVariant msg) {

}

void EVT_ADD_HARDWARE_SIGNER_REQUEST_HANDLER(QVariant msg) {

}


