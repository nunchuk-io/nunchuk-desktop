#include "GetWalletUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::wallets::usecases {
using namespace core::usecase;

Result<GetWalletResult> GetWalletUseCase::execute(const GetWalletInput &input) {
    GetWalletResult result;
    nunchuk::Wallet createdWallet;
    QWarningMessage msg;
    createdWallet = bridge::nunchukGetOriginWallet(input.wallet_id, msg);

    if (msg.isSuccess()) {
        result.wallet = createdWallet;
        return Result<GetWalletResult>::success(result);
    } else {
        return Result<GetWalletResult>::failure(QString("Failed to get wallet %1: %2").arg(input.wallet_id).arg(msg.what()));
    }
}

} // namespace features::wallets::usecases