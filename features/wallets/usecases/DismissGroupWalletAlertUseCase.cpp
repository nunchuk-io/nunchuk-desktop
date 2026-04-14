#include "DismissGroupWalletAlertUseCase.h"
#include "core/bridge/ExternalBridges.h"

namespace features::wallets::usecases {
using namespace core::usecase;

Result<DismissGroupWalletAlertResult> DismissGroupWalletAlertUseCase::execute(const DismissGroupWalletAlertInput &input) {
    QWarningMessage warningMsg;
    bridge::DismissGroupWalletAlert(input.wallet_id, input.alert_id, warningMsg);
    
    if ((int)EWARNING::WarningType::NONE_MSG != warningMsg.type()) {
        return Result<DismissGroupWalletAlertResult>::failure(warningMsg.what(), warningMsg.code());
    }
    
    DismissGroupWalletAlertResult result;
    return Result<DismissGroupWalletAlertResult>::success(result);
}

} // namespace features::wallets::usecases
