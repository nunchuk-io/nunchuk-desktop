#include "MarkGroupWalletAlertViewedUseCase.h"
#include "core/bridge/ExternalBridges.h"

namespace features::wallets::usecases {
using namespace core::usecase;

Result<MarkGroupWalletAlertViewedResult> MarkGroupWalletAlertViewedUseCase::execute(const MarkGroupWalletAlertViewedInput &input) {
    QWarningMessage warningMsg;
    bridge::MarkGroupWalletAlertViewed(input.wallet_id, input.alert_id, warningMsg);
    
    if ((int)EWARNING::WarningType::NONE_MSG != warningMsg.type()) {
        return Result<MarkGroupWalletAlertViewedResult>::failure(warningMsg.what(), warningMsg.code());
    }
    
    MarkGroupWalletAlertViewedResult result;
    return Result<MarkGroupWalletAlertViewedResult>::success(result);
}

} // namespace features::wallets::usecases
