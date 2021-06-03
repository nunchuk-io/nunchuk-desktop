
#include "STATE_ID_SCR_WALLET_CHANGE_ADDRESSES.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include <QClipboard>

void SCR_WALLET_CHANGE_ADDRESSES_Entry(QVariant msg) {

}

void SCR_WALLET_CHANGE_ADDRESSES_Exit(QVariant msg) {

}

void EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->id();
        if(AppModel::instance()->walletInfo()->unUsedChangeddAddressList().count() < 20){
            bridge::nunchukGenNewAddresses(wallet_id, true);
        }
        AppModel::instance()->walletInfo()->setUsedChangeAddressList(bridge::nunchukGetUsedAddresses(AppModel::instance()->walletInfo()->id(), true));
        AppModel::instance()->walletInfo()->setUnUsedChangeddAddressList(bridge::nunchukGetUnusedAddresses(AppModel::instance()->walletInfo()->id(), true));
    }
}

void EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_WALLET_CHANGE_ADDRESSES_COPY_HANDLER(QVariant msg) {
    if(!msg.toString().isEmpty()){
        qApp->clipboard()->setText(msg.toString());
    }
}
