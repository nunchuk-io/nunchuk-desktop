#include "GetMasterSignerUseCase.h"
#include "qUtils.h"
#include "ifaces/bridgeifaces.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<GetMasterSignerResult> GetMasterSignerUseCase::execute(const GetMasterSignerInput &input) {
    GetMasterSignerResult result;
    QWarningMessage msg;
    nunchuk::MasterSigner signer = bridge::nunchukGetOriginMasterSigner(input.id, msg);
    if (msg.isSuccess()) {
        result.signer = signer;
        return Result<GetMasterSignerResult>::success(result);
    } else {
        return Result<GetMasterSignerResult>::failure(msg.what());
    }
}
}