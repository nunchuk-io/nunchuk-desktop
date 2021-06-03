
#include "STATE_ID_SCR_CREATE_TRANSACTION.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"

void SCR_CREATE_TRANSACTION_Entry(QVariant msg) {
    AppModel::instance()->setEstimateFeeEconomical(bridge::nunchukEstimateFee(nunchuk::CONF_TARGET_ECONOMICAL));
    AppModel::instance()->setEstimateFeeStandard(bridge::nunchukEstimateFee(nunchuk::CONF_TARGET_STANDARD));
    AppModel::instance()->setEstimateFeePriority(bridge::nunchukEstimateFee(nunchuk::CONF_TARGET_PRIORITY));
    Q_UNUSED(msg);
}

void SCR_CREATE_TRANSACTION_Exit(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
    AppModel::instance()->setTxidReplacing("");
}

void EVT_CREATE_TRANSACTION_SIGN_REQUEST_HANDLER(QVariant msg) {
    bool subtractFromFeeAmout = msg.toMap().value("subtractFromFeeAmout").toBool();
    int feeRate = msg.toMap().value("feeRate").toDouble()*1000; // Convert sats/Byte to sats/kB
    bool manualFee = msg.toMap().value("manualFee").toBool();
    bool manualOutput = msg.toMap().value("manualOutput").toBool();
    if(!manualFee) feeRate = -1;

    if(AppModel::instance()->transactionInfo()){
        if(QQuickViewer::instance()->onsRequester() == E::STATE_ID_SCR_TRANSACTION_INFO){
            DBG_INFO << "REPLACE BY FEE REQUEST";
            if(AppModel::instance()->walletInfo() && AppModel::instance()->getTxidReplacing() != ""){
                QWarningMessage msgwarning;
                QSharedPointer<Transaction> trans = bridge::nunchukReplaceTransaction(AppModel::instance()->walletInfo()->id(),
                                                                                       AppModel::instance()->getTxidReplacing(),
                                                                                       feeRate,
                                                                                       msgwarning);

                if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                    if(trans){
                        trans.data()->setIsReceiveTx(false);
                        AppModel::instance()->setTransactionInfo(trans);
                        if(AppModel::instance()->transactionHistory()){
                            AppModel::instance()->transactionHistory()->replaceTransaction(trans.data()->txid(), trans);
                        }
                        if(AppModel::instance()->transactionHistoryShort()){
                            AppModel::instance()->transactionHistoryShort()->replaceTransaction(trans.data()->txid(), trans);
                        }
                        QQuickViewer::instance()->sendEvent(E::EVT_CREATE_TRANSACTION_SIGN_SUCCEED);
                        AppModel::instance()->setTxidReplacing("");
                    }
                }
                else{
                    AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                                 msgwarning.what(),
                                                                                                (EWARNING::WarningType)msgwarning.type(),
                                                                                                 "Cannot replace transaction");
                }
            }
        }
        else{
            DBG_INFO << "NEW TRANSACTION, REQUEST SIGNING";
            DBG_INFO << "subtract:" << subtractFromFeeAmout << "| manual Output:" << manualOutput << "| manual Fee:" << manualFee << "| free rate:" << feeRate;
            QSharedPointer<UTXOListModel> inputs = NULL;
            if(true == manualOutput){
                inputs = QSharedPointer<UTXOListModel>(new UTXOListModel());
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
            }

            QMap<QString, qint64> outputs;
            if(AppModel::instance()->destinationList()){
                outputs = AppModel::instance()->destinationList()->getOutputs();
            }

            QString wallet_id = "";
            if(AppModel::instance()->walletInfo()){
                wallet_id = AppModel::instance()->walletInfo()->id();
            }

            QString memo = "";
            if(AppModel::instance()->transactionInfo()){
                memo = AppModel::instance()->transactionInfo()->memo();
            }
            QWarningMessage msgwarning;
            QSharedPointer<Transaction> trans = bridge::nunchukCreateTransaction(wallet_id, outputs, memo, inputs, feeRate, subtractFromFeeAmout, msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                if(trans){
                    trans.data()->setIsReceiveTx(false);
                    AppModel::instance()->setTransactionInfo(trans);
                    if(AppModel::instance()->transactionHistory()){
                        AppModel::instance()->transactionHistory()->replaceTransaction(trans.data()->txid(), trans);
                    }
                    if(AppModel::instance()->transactionHistoryShort()){
                        AppModel::instance()->transactionHistoryShort()->replaceTransaction(trans.data()->txid(), trans);
                    }
                    QQuickViewer::instance()->sendEvent(E::EVT_CREATE_TRANSACTION_SIGN_SUCCEED);
                }
            }
            else{
                AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                             msgwarning.what(),
                                                                                             (EWARNING::WarningType)msgwarning.type(),
                                                                                             "Cannot create transaction");
            }
        }
    }
}

void EVT_CREATE_TRANSACTION_BACK_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_CREATE_TRANSACTION_SAVE_REQUEST_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if (file_path != ""){
        bool ret = bridge::nunchukExportTransaction(AppModel::instance()->walletInfo()->id(),
                                                     AppModel::instance()->transactionInfo()->txid(),
                                                     file_path);
        DBG_INFO << file_path << ret;
    }
}

void EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST_HANDLER(QVariant msg) {
    int sortRole = msg.toMap().value("sortRole").toInt();
    int sortOrder = msg.toMap().value("sortOrder").toInt();

    if(AppModel::instance()->utxoList()){
        AppModel::instance()->utxoList()->requestSort(sortRole, sortOrder);
    }
}

void EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX_HANDLER(QVariant msg) {
    bool subtractFromFeeAmout = msg.toMap().value("subtractFromFeeAmout").toBool();
    int feeRate = msg.toMap().value("feeRate").toDouble()*1000; // Convert sats/Byte to sats/kB
    bool manualFee = msg.toMap().value("manualFee").toBool();
    bool manualOutput = msg.toMap().value("manualOutput").toBool();
    if(!manualFee) feeRate = -1;

    DBG_INFO << "subtract:" << subtractFromFeeAmout << "| manual Output:" << manualOutput << "| manual Fee:" << manualFee << "| free rate:" << feeRate;
    QSharedPointer<UTXOListModel> inputs = NULL;
    if(true == manualOutput){
        inputs = QSharedPointer<UTXOListModel>(new UTXOListModel());
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
    }

    QMap<QString, qint64> outputs;
    if(AppModel::instance()->destinationList()){
        outputs = AppModel::instance()->destinationList()->getOutputs();
    }

    QString wallet_id = "";
    if(AppModel::instance()->walletInfo()){
        wallet_id = AppModel::instance()->walletInfo()->id();
    }

    QWarningMessage msgwarning;
    QSharedPointer<Transaction> trans = bridge::nunchukDraftTransaction(wallet_id, outputs, inputs, feeRate, subtractFromFeeAmout, msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
        if(trans){
            QString memo = "";
            if(AppModel::instance()->transactionInfo()){
                memo = AppModel::instance()->transactionInfo()->memo();
            }
            trans.data()->setIsReceiveTx(false);
            AppModel::instance()->setTransactionInfo(trans);
            AppModel::instance()->transactionInfo()->setMemo(memo);
        }
    }
    else{
        AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                     msgwarning.what(),
                                                                                     (EWARNING::WarningType)msgwarning.type(),
                                                                                     "Cannot create transaction");
    }
}

void EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_CREATE_TRANSACTION_SIGN_SUCCEED_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}


