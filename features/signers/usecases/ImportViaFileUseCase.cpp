#include "ImportViaFileUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/utils/Utils.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<ImportViaFileResult> ImportViaFileUseCase::execute(const ImportViaFileInput &input) {
    ImportViaFileResult result;
    QWarningMessage msg;
    QString path = qUtils::QGetFilePath(input.filePath);
    std::vector<nunchuk::SingleSigner> signers = bridge::signer::ParseJSONSigners(path, input.signer_type, msg);
    if (msg.isSuccess()) {
        result.signers = signers;
        return Result<ImportViaFileResult>::success(result);
    } else {
        return Result<ImportViaFileResult>::failure(msg.what());
    }
}
}