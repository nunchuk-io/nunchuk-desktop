#include "GetGroupDummyTransactionsUseCase.h"

#include "core/bridge/ExternalBridges.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<GetGroupDummyTransactionsResult> GetGroupDummyTransactionsUseCase::execute(const GetGroupDummyTransactionsInput &input) {
    GetGroupDummyTransactionsResult result;
    result.dummy_transactions = bridge::GetGroupDummyTransactions(input.wallet_id);
    return Result<GetGroupDummyTransactionsResult>::success(result);
}

} // namespace features::transactions::usecases