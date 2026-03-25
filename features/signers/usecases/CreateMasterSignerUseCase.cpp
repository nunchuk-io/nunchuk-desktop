#include "CreateMasterSignerUseCase.h"
#include "core/bridge/ExternalBridges.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<CreateMasterSignerResult> CreateMasterSignerUseCase::execute(const CreateMasterSignerInput &input) {
    CreateMasterSignerResult result;
    QWarningMessage msg;
    nunchuk::MasterSigner signer = bridge::signer::CreateMasterSigner(
        input.name,
        input.device,
        input.progress,
        msg
    );
    if (msg.isSuccess()) {
        result.signer = signer;
        return Result<CreateMasterSignerResult>::success(result);
    } else {
        return Result<CreateMasterSignerResult>::failure(msg.what());
    }
}
}