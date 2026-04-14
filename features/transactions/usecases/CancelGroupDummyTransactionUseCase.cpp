#include "CancelGroupDummyTransactionUseCase.h"

#include "core/bridge/ExternalBridges.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<CancelGroupDummyTransactionResult> CancelGroupDummyTransactionUseCase::execute(const CancelGroupDummyTransactionInput &input) {
    QWarningMessage msg;
    bridge::CancelGroupDummyTransaction(input.wallet_id, input.dummy_transaction_id, msg);

    if (!msg.isSuccess()) {
        return Result<CancelGroupDummyTransactionResult>::failure(msg.contentDisplay(), msg.code());
    }

    CancelGroupDummyTransactionResult result;
    result.cancelled = true;
    return Result<CancelGroupDummyTransactionResult>::success(result);
}

} // namespace features::transactions::usecases