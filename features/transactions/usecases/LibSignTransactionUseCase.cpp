#include "LibSignTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<LibSignTransactionResult> LibSignTransactionUseCase::execute(const LibSignTransactionInput &input) {
    QWarningMessage msg;
    nunchuk::Transaction trans = nunchuk::Transaction();
    if (input.wallet.has_value()) {
        trans = input.tx.value();
        for(const auto& device : input.devices) {
            msg.resetWarningMessage();
            trans = bridge::SignTransaction(input.wallet.value(), trans, device, msg);
        }
    } else {
        for(const auto& device : input.devices) {
            msg.resetWarningMessage();
            trans = bridge::transaction::SignTransaction(input.wallet_id.value(), input.tx_id.value(), device, msg);
        }
    }
    LibSignTransactionResult result;
    if (msg.isSuccess()) {
        result.tx = trans;
        return Result<LibSignTransactionResult>::success(result);
    } else {
        return Result<LibSignTransactionResult>::failure(QString("Failed to sign transaction %1: %2").arg(input.tx_id.value()).arg(msg.what()));
    }
}

} // namespace features::transactions::usecases