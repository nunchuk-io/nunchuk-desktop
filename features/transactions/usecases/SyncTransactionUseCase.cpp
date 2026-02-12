#include "SyncTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<SyncTransactionResult> SyncTransactionUseCase::execute(const SyncTransactionInput &input) {
    SyncTransactionResult result;
    QJsonObject data;
    QString msgerror = "";
    if (qUtils::isValidGroup(input.group_id)) {
        Byzantine::instance()->SyncTransaction(input.group_id, input.wallet_id, input.txid, input.psbt, input.note, data, msgerror);
    } else if(input.isClaimed) {
        Draco::instance()->ClaimingWalletTransactionSync(input.wallet_id, input.psbt, data, msgerror);
    } else {
        Draco::instance()->assistedSyncTx(input.wallet_id, input.txid, input.psbt, input.note);
    }
    result.synced = true;
    return Result<SyncTransactionResult>::success(result);
}

} // namespace features::transactions::usecases