#include "GetGroupDummyTransactionUseCase.h"

#include "core/bridge/ExternalBridges.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<GetGroupDummyTransactionResult> GetGroupDummyTransactionUseCase::execute(const GetGroupDummyTransactionInput &input) {
    QWarningMessage msg;
    GetGroupDummyTransactionResult result;
    result.dummy_transaction = bridge::GetGroupDummyTransaction(input.wallet_id, input.dummy_transaction_id, msg);

    if (msg.isSuccess()) {
        return Result<GetGroupDummyTransactionResult>::success(result);
    }
    return Result<GetGroupDummyTransactionResult>::failure(msg.contentDisplay(), msg.code());
}

} // namespace features::transactions::usecases