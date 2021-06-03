
#include "STATE_ID_SCR_UTXOS.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/UTXOModel.h"
#include "bridgeifaces.h"

void SCR_UTXOS_Entry(QVariant msg) {

}

void SCR_UTXOS_Exit(QVariant msg) {

}

void EVT_UTXOS_BACK_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setUtxoInfo(QSharedPointer<UTXO>(new UTXO()));
}

void EVT_UTXOS_ITEM_REQUEST_HANDLER(QVariant msg) {
    if(msg.toInt() >= 0 && AppModel::instance()->utxoList()){
        QSharedPointer<UTXO> info = AppModel::instance()->utxoList()->getUTXOByIndex(msg.toInt());
        if(info){
            AppModel::instance()->setUtxoInfo(info);
        }
    }
}

void EVT_UTXOS_SORT_REQUEST_HANDLER(QVariant msg) {
    int sortRole = msg.toMap().value("sortRole").toInt();
    int sortOrder = msg.toMap().value("sortOrder").toInt();

    if(AppModel::instance()->utxoList()){
        AppModel::instance()->utxoList()->requestSort(sortRole, sortOrder);
    }
}

void EVT_UTXOS_CONSOLIDATE_REQUEST_HANDLER(QVariant msg) {

}

void EVT_UTXOS_ITEM_SELECTED_HANDLER(QVariant msg) {
    if(msg.toInt() >= 0 && AppModel::instance()->utxoList()){
        QSharedPointer<UTXO> info = AppModel::instance()->utxoList()->getUTXOByIndex(msg.toInt());
        if(info){
            AppModel::instance()->setUtxoInfo(info);
        }
    }
}


