
#include "STATE_ID_SCR_UTXO_OUTPUT.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/UTXOModel.h"
#include "bridgeifaces.h"

void SCR_UTXO_OUTPUT_Entry(QVariant msg) {

}

void SCR_UTXO_OUTPUT_Exit(QVariant msg) {

}

void EVT_UTXO_INFO_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_UTXO_INFO_CHECKOUT_TX_RELATED_HANDLER(QVariant msg) {
    if(msg.toString() != ""){
        QSharedPointer<Transaction> it = AppModel::instance()->getTransactionByTxid(msg.toString());
        if(it){
            AppModel::instance()->setTransactionInfo(it);
            QQuickViewer::instance()->sendEvent(E::EVT_UTXO_INFO_VIEW_TX_RELATED);
        }
    }
}

void EVT_UTXO_INFO_VIEW_TX_RELATED_HANDLER(QVariant msg) {

}
