#include "FetchCancelTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"
#include <QJsonArray>

namespace features::transactions::usecases {
using namespace core::usecase;

Result<FetchCancelTransactionResult> FetchCancelTransactionUseCase::execute(const FetchCancelTransactionInput &input) {
    FetchCancelTransactionResult result;
    QJsonArray transactionList = fetchCancelledTransactions(input);
    for (QJsonValue js_value : transactionList) {
        QJsonObject transaction = js_value.toObject();
        QString transaction_id = transaction.value("transaction_id").toString();
        bridge::nunchukDeleteTransaction(input.wallet_id, transaction_id);
    }
    if (transactionList.size() > 0) {
        result.transactions = transactionList;
        return Result<FetchCancelTransactionResult>::success(result);
    } else {
        return Result<FetchCancelTransactionResult>::failure("Not found cancelled transactions");
    }    
}

QJsonArray FetchCancelTransactionUseCase::fetchCancelledTransactions(const FetchCancelTransactionInput &input) {
    int offset = 0;
    const int limit = 10;
    QJsonArray transactionList;
    while (true) {
        QJsonObject output;
        QString errormsg = "";
        if (qUtils::isValidGroup(input.group_id)) {
            Byzantine::instance()->GetAllCancelledTransaction(input.group_id, input.wallet_id, offset, limit, output, errormsg);        
        } else if(input.isClaimed) {
            Draco::instance()->CancelClaimingWalletTransactionGetList(input.wallet_id, offset, limit, output, errormsg);
        } else {
            output = Draco::instance()->assistedWalletDeleteListTx(input.wallet_id, offset, limit);
        }
        QJsonArray transactions = output.value("transactions").toArray();
        if(transactions.size() > 0){            
            for (QJsonValue js_value : transactions) {
                QJsonObject transaction = js_value.toObject();
                transactionList.append(transaction);
            }
            if (transactions.size() == 0 || transactions.size() < limit) {
                return transactionList; // exit while loop
            }
            offset += transactions.size();
        }
        else{
            return transactionList;
        }
    }
    return transactionList;
}

} // namespace features::transactions::usecases