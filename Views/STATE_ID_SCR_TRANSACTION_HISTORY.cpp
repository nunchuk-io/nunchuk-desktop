
#include "STATE_ID_SCR_TRANSACTION_HISTORY.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_TRANSACTION_HISTORY_Entry(QVariant msg) {

}

void SCR_TRANSACTION_HISTORY_Exit(QVariant msg) {

}

void EVT_TRANSACTION_HISTORY_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_TRANSACTION_INFO_ITEM_SELECTED_HANDLER(QVariant msg) {
    if(msg.toString() != ""){
        QSharedPointer<Transaction> it = AppModel::instance()->getTransactionByTxid(msg.toString());
        AppModel::instance()->setTransactionInfo(it);
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_TRANSACTION_HISTORY_SORT_REQUEST_HANDLER(QVariant msg) {
    int sortRole = msg.toMap().value("sortRole").toInt();
    int sortOrder = msg.toMap().value("sortOrder").toInt();

    if(AppModel::instance()->transactionHistory()){
        AppModel::instance()->transactionHistory()->requestSort(sortRole, sortOrder);
    }
}


