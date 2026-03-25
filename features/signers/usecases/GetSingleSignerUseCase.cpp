#include "GetSingleSignerUseCase.h"
#include "core/ui/UiServices.inc"
#include "core/bridge/ExternalBridges.h"

namespace features::signers::usecases {
using namespace core::usecase;

core::usecase::Result<GetSingleSignerResult> GetSingleSignerUseCase::execute(const GetSingleSignerInput &input) {
    QWarningMessage msg;
    nunchuk::SingleSigner ret = bridge::signer::GetSigner(input.xfp, input.wallet_type, input.address_type, input.account_index, msg);
    GetSingleSignerResult result;
    if (msg.isSuccess()) {
        result.signer = ret;
        return core::usecase::Result<GetSingleSignerResult>::success(result);
    } else {
        msg.resetWarningMessage();
        ret = bridge::signer::GetSignerFromMasterSigner(input.xfp, input.wallet_type, input.address_type, input.account_index, msg);
        if (msg.isSuccess()) {
            result.signer = ret;
            return core::usecase::Result<GetSingleSignerResult>::success(result);
        }
    }
    return core::usecase::Result<GetSingleSignerResult>::failure("No supported signers found.");
}

}