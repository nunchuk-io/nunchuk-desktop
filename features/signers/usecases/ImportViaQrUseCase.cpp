#include "ImportViaQrUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/utils/Utils.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<ImportViaQrResult> ImportViaQrUseCase::execute(const ImportViaQrInput &input) {
    ImportViaQrResult result;
    QWarningMessage msg;
    std::vector<nunchuk::SingleSigner> signers = bridge::signer::ParseQRSigners(input.qr_data, msg);
    if (msg.isSuccess()) {
        result.signers = signers;
        return Result<ImportViaQrResult>::success(result);
    } else {
        return Result<ImportViaQrResult>::failure(msg.what());
    }
}
}