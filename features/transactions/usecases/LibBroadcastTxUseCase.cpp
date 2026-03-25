#include "LibBroadcastTxUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<LibBroadcastTxResult> LibBroadcastTxUseCase::execute(const LibBroadcastTxInput &input) {
    QWarningMessage msg;
    nunchuk::Transaction tx = bridge::transaction::BroadcastTransaction(input.wallet_id, input.tx_id, msg);
    LibBroadcastTxResult result;
    if (msg.isSuccess()) {
        result.tx = tx;
        return Result<LibBroadcastTxResult>::success(result);
    } else {
        return Result<LibBroadcastTxResult>::failure(QString("Failed to broadcast transaction %1: %2").arg(input.tx_id).arg(msg.what()), msg.code());
    }
}

} // namespace features::transactions::usecases