#include "CreateSoftwareSignerUseCase.h"
#include "qUtils.h"
#include "ifaces/bridgeifaces.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<CreateSoftwareResult> CreateSoftwareSignerUseCase::execute(const CreateSoftwareInput &input) {
    CreateSoftwareResult result;
    QWarningMessage msg;
    nunchuk::MasterSigner master = bridge::nunchukCreateOriginSoftwareSigner(
        input.keyName,
        input.mnemonic,
        input.passphrase,
        input.isPrimaryKey,
        input.replace,
        msg
    );
    result.master = master;
    if (msg.isSuccess()) {
        return Result<CreateSoftwareResult>::success(result);
    } else {
        return Result<CreateSoftwareResult>::failure(msg.contentDisplay());
    }
}
}