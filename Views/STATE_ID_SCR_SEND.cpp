
#include "STATE_ID_SCR_SEND.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/AppSetting.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_SEND_Entry(QVariant msg) {
    AppModel::instance()->setTransactionInfo(QSharedPointer<Transaction>(new Transaction()));
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->escrow()){
        AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(0,
                                                                                     "For Escrow Wallet, you must spend all the out standing balance.",
                                                                                     EWARNING::WarningType::WARNING_MSG);
    }
}

void SCR_SEND_Exit(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_SEND_ADD_DESTINATION_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SEND_BACK_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setTransactionInfo(QSharedPointer<Transaction>(new Transaction()));
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}

void EVT_SEND_CREATE_TRANSACTION_REQUEST_HANDLER(QVariant msg) {
    QString memo = msg.toMap().value("destinationMemo").toString();
    QList<QVariant> destinationInputed = msg.toMap().value("destinationList").toList();
    QSharedPointer<DestinationListModel> destinationList = QSharedPointer<DestinationListModel>(new DestinationListModel());
    for (int i = 0; i < destinationInputed.count(); i++) {
        qint64 toAmount = 0;
        if(1 == AppSetting::instance()->unit()){
            toAmount = destinationInputed.at(i).toMap()["toAmount"].toInt();
        }
        else{
            toAmount = qUtils::QAmountFromValue(destinationInputed.at(i).toMap()["toAmount"].toString());
        }
        DBG_INFO << toAmount << destinationInputed.at(i).toMap()["toAmount"].toString();
        destinationList.data()->addDestination( destinationInputed.at(i).toMap()["toAddress"].toString(), toAmount);
    }
    AppModel::instance()->setDestinationList(destinationList);

    QMap<QString, qint64> outputs;
    QString wallet_id = "";
    bool subtractFromAmount = false;
    if(AppModel::instance()->walletInfo()){
        wallet_id = AppModel::instance()->walletInfo()->id();
        if(AppModel::instance()->walletInfo()->escrow()){
            subtractFromAmount = true;
        }
    }
    if(AppModel::instance()->destinationList()){
        outputs = AppModel::instance()->destinationList()->getOutputs();
        DBG_INFO << "Destination : " << outputs;
    }
    QWarningMessage msgwarning;
    QSharedPointer<Transaction> trans = bridge::nunchukDraftTransaction(wallet_id, outputs, NULL, -1, subtractFromAmount, msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
        if(trans){
            AppModel::instance()->setTransactionInfo(trans);
            AppModel::instance()->transactionInfo()->setMemo(memo);
            QQuickViewer::instance()->sendEvent(E::EVT_SEND_CREATE_TRANSACTION_SUCCEED);
        }
    }
    else{
        if(     ((int)EWARNING::WarningType::EXCEPTION_MSG == msgwarning.type()) &&
                (msgwarning.code() == nunchuk::NunchukException::COIN_SELECTION_ERROR) &&
                subtractFromAmount == false)
        {
            DBG_INFO << "Retry make draft transaction with subtract true";
            msgwarning.resetWarningMessage();
            QSharedPointer<Transaction> trans = bridge::nunchukDraftTransaction(wallet_id, outputs, NULL, -1, true, msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                if(trans){
                    AppModel::instance()->setTransactionInfo(trans);
                    AppModel::instance()->transactionInfo()->setMemo(memo);
                    QQuickViewer::instance()->sendEvent(E::EVT_SEND_CREATE_TRANSACTION_SUCCEED);
                }
            }
            else{
                AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(msgwarning.code(),
                                                                                             msgwarning.what(),
                                                                                             (EWARNING::WarningType)msgwarning.type(),
                                                                                             "Cannot create transaction");
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

void EVT_SEND_CREATE_TRANSACTION_SUCCEED_HANDLER(QVariant msg) {
    if(AppModel::instance()->transactionInfo()){
        AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
    }
}


