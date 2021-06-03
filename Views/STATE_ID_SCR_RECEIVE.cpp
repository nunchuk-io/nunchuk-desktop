
#include "STATE_ID_SCR_RECEIVE.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include <QClipboard>

void SCR_RECEIVE_Entry(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->id();
        AppModel::instance()->walletInfo()->setUsedAddressList(bridge::nunchukGetUsedAddresses(wallet_id, false));
        AppModel::instance()->walletInfo()->setunUsedAddressList(bridge::nunchukGetUnusedAddresses(wallet_id, false));
    }
}

void SCR_RECEIVE_Exit(QVariant msg) {
    AppModel::instance()->setAddressBalance(0);
}

void EVT_BTN_GEN_NEW_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->id();
        if(AppModel::instance()->walletInfo()->unUsedAddressList().count() < 20){
            bridge::nunchukGenNewAddresses(wallet_id, false);
        }
        AppModel::instance()->walletInfo()->setUsedAddressList(bridge::nunchukGetUsedAddresses(wallet_id, false));
        AppModel::instance()->walletInfo()->setunUsedAddressList(bridge::nunchukGetUnusedAddresses(wallet_id, false));
    }
}

void EVT_RECEIVE_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_RECEIVE_COPY_ADDRESS_HANDLER(QVariant msg) {
    if(!msg.toString().isEmpty()){
        qApp->clipboard()->setText(msg.toString());
    }
}

void EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS_HANDLER(QVariant msg) {
    // Need handle set quick receive address here
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->setAddress(msg.toString());
    }
}


void EVT_RECEIVE_DISPLAY_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->startDisplayAddress(AppModel::instance()->walletInfo()->id(),
                                                  msg.toString(),
                                                  "");
    }
}

void EVT_RECEIVE_ADDRESS_BALANCE_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        qint64 amount = bridge::nunchukGetAddressBalance(AppModel::instance()->walletInfo()->id(), msg.toString());
        AppModel::instance()->setAddressBalance(amount);
    }
}
