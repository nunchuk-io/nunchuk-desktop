#include "FetchTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<FetchTransactionResult> FetchTransactionUseCase::execute(const FetchTransactionInput &input) {
    FetchTransactionResult result;
    QJsonObject data;
    QString msgerror = "";
    if (qUtils::isValidGroup(input.group_id)) {
        Byzantine::instance()->GetOneTransaction(input.group_id, input.wallet_id, input.txid, data, msgerror);
    } else if(input.isClaimed) {
        data = Draco::instance()->ClaimingWalletTransactionGetOne(input.wallet_id, input.txid);
    } else {
        data = Draco::instance()->assistedWalletGetTx(input.wallet_id, input.txid);
    }
    QJsonObject transaction = data.value("transaction").toObject();
    if (!transaction.isEmpty()) {
        result.transaction = transaction;
        return Result<FetchTransactionResult>::success(result);
    } else {
        return Result<FetchTransactionResult>::failure(QString("Failed to get transaction %1: %2").arg(input.wallet_id));
    }
}

} // namespace features::transactions::usecases