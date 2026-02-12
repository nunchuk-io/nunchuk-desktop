#include "RbfTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<RbfTransactionResult> RbfTransactionUseCase::execute(const RbfTransactionInput &input) {
    RbfTransactionResult result;
    QJsonObject data;
    QString msgerror = "";
    if (qUtils::isValidGroup(input.group_id)) {
        Byzantine::instance()->RbfTransaction(input.group_id, input.wallet_id, input.txid, input.psbt, data, msgerror);
    } else if(input.isClaimed) {
        Draco::instance()->ClaimingWalletTransactionRbf(input.wallet_id, input.txid, input.psbt, data, msgerror);
    } else {
        Draco::instance()->assistedRbfTx(input.wallet_id, input.txid, input.psbt, data, msgerror);
    }
    QJsonObject transaction = data.value("transaction").toObject();
    if (!transaction.isEmpty()) {
        return Result<RbfTransactionResult>::success(result);
    } else {
        return Result<RbfTransactionResult>::failure(QString("Failed to get transaction %1: %2").arg(input.wallet_id));
    }
}

} // namespace features::transactions::usecases