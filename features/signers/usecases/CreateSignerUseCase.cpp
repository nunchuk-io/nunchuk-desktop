#include "CreateSignerUseCase.h"
#include "qUtils.h"
#include "ifaces/bridgeifaces.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<CreateSignerResult> CreateSignerUseCase::execute(const CreateSignerInput &input) {
    CreateSignerResult result;
    QWarningMessage msg;
    nunchuk::SingleSigner signer = bridge::nunchukCreateOriginSigner(
        input.name,
        input.xpub,
        input.publicKey,
        input.derivationPath,
        input.masterFingerprint,
        input.type,
        input.tags,
        input.replaceExisting,
        msg
    );
    if (msg.isSuccess()) {
        result.signer = signer;
        return Result<CreateSignerResult>::success(result);
    } else {
        return Result<CreateSignerResult>::failure(msg.what());
    }
}
}