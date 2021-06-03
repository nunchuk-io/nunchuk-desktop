
#include "STATE_ID_SCR_CONSOLIDATE_OUTPUT.h"
#include "Models/AppModel.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"

void SCR_CONSOLIDATE_Entry(QVariant msg) {
    AppModel::instance()->setTransactionInfo(QSharedPointer<Transaction>(new Transaction()));
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->escrow()){
        AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(0,
                                                                                     "For Escrow Wallet, you must spend all the out standing balance.",
                                                                                     EWARNING::WarningType::WARNING_MSG);
    }
}

void SCR_CONSOLIDATE_Exit(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_CONSOLIDATE_BACK_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setTransactionInfo(QSharedPointer<Transaction>(new Transaction()));
}

void EVT_CONSOLIDATE_MAKE_TRANSACTION_HANDLER(QVariant msg) {
    QString toAddress = msg.toMap().value("toAddress").toString();
    int toAmount    = msg.toMap().value("toAmount").toInt();
    QString destinationMemo    = msg.toMap().value("destinationMemo").toString();
    DBG_INFO << toAddress << toAmount << destinationMemo;

    QSharedPointer<DestinationListModel> destinationList = QSharedPointer<DestinationListModel>(new DestinationListModel());
    destinationList.data()->addDestination( toAddress, toAmount);
    AppModel::instance()->setDestinationList(destinationList);

    QSharedPointer<UTXOListModel> inputs = QSharedPointer<UTXOListModel>(new UTXOListModel());
    if(AppModel::instance()->utxoList()){
        for (int i = 0; i < AppModel::instance()->utxoList()->rowCount(); i++) {
            QSharedPointer<UTXO> it = AppModel::instance()->utxoList()->getUTXOByIndex(i);
            if(it.data() && it.data()->selected()){
                DBG_INFO << "UTXO Selected:" << it.data()->txid() << it.data()->amountSats();
                inputs->addUTXO(it.data()->txid(),
                                it.data()->vout(),
                                it.data()->address(),
                                it.data()->amountSats(),
                                it.data()->height(),
                                it.data()->memo());
            }
        }
    }

    QMap<QString, qint64> outputs;
    if(AppModel::instance()->destinationList()){
        outputs = AppModel::instance()->destinationList()->getOutputs();
        DBG_INFO << "Destination:" << outputs;
    }

    QString wallet_id = "";
    if(AppModel::instance()->walletInfo()){
        wallet_id = AppModel::instance()->walletInfo()->id();
    }
    bool subtractFromFeeAmout = true;

    DBG_INFO << "subtract:" << subtractFromFeeAmout << "| manual Output: false" << "| manual Fee:" << true << "| free rate:" << -1;
    QWarningMessage msgwarning;
    QSharedPointer<Transaction> trans = bridge::nunchukDraftTransaction(wallet_id, outputs, inputs, -1, subtractFromFeeAmout, msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
        if(trans){
            QString memo = "";
            if(AppModel::instance()->transactionInfo()){
                memo = AppModel::instance()->transactionInfo()->memo();
            }
            AppModel::instance()->setTransactionInfo(trans);
            AppModel::instance()->transactionInfo()->setMemo(memo);
            QQuickViewer::instance()->sendEvent(E::EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED);
        }
    }
    else{
        AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                     msgwarning.what(),
                                                                                     (EWARNING::WarningType)msgwarning.type(),
                                                                                     "Cannot create transaction");
    }

}


void EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED_HANDLER(QVariant msg) {

}


