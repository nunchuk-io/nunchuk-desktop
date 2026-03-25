#include "SuggestMnemonicsUseCase.h"
#include "core/bridge/ExternalBridges.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<SuggestMnemonicsResult> SuggestMnemonicsUsecase::execute(const SuggestMnemonicsInput &input) {
    SuggestMnemonicsResult result;
    result.suggests = bridge::nunchuckGetBIP39WordList();
    return Result<SuggestMnemonicsResult>::success(result);
}
}