#include "CancelTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<CancelTransactionResult> CancelTransactionUseCase::execute(const CancelTransactionInput &input) {
    CancelTransactionResult result;
    QJsonObject output;
    QString errormsg = "";
    if (qUtils::isValidGroup(input.group_id)) {
        Byzantine::instance()->CancelTransaction(input.group_id, input.wallet_id, input.txid, output, errormsg);
    } else if(input.isClaimed) {
        Draco::instance()->ClaimingWalletTransactionCancel(input.wallet_id, input.txid);
    } else {
        Draco::instance()->assistedWalletCancelTx(input.wallet_id, input.txid);
    }
    result.ret = true;
    return Result<CancelTransactionResult>::success(result);
}

} // namespace features::transactions::usecases