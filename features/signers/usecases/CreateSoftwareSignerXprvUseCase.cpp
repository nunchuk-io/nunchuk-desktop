#include "CreateSoftwareSignerXprvUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/utils/Utils.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<CreateSoftwareSignerXprvResult> CreateSoftwareSignerXprvUseCase::execute(const CreateSoftwareSignerXprvInput &input) {
    CreateSoftwareSignerXprvResult result;
    QWarningMessage msg;
    nunchuk::MasterSigner master = bridge::CreateSoftwareSignerFromMasterXprv(
        input.keyName,
        input.xprv,
        input.isPrimaryKey,
        input.replace,
        msg
    );
    result.master = master;
    if (msg.isSuccess()) {
        return Result<CreateSoftwareSignerXprvResult>::success(result);
    } else {
        return Result<CreateSoftwareSignerXprvResult>::failure(msg.contentDisplay());
    }
}
}