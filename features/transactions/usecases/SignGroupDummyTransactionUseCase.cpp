#include "SignGroupDummyTransactionUseCase.h"

#include "core/bridge/ExternalBridges.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<SignGroupDummyTransactionResult> SignGroupDummyTransactionUseCase::execute(const SignGroupDummyTransactionInput &input) {
    QWarningMessage msg;
    SignGroupDummyTransactionResult result;
    result.dummy_transaction = bridge::SignGroupDummyTransaction(input.wallet_id, input.dummy_transaction_id, input.signatures, msg);

    if (msg.isSuccess()) {
        return Result<SignGroupDummyTransactionResult>::success(result);
    }
    return Result<SignGroupDummyTransactionResult>::failure(msg.what(), msg.code());
}

} // namespace features::transactions::usecases