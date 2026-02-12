#include "TimeLockSetupGetUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::inheritance::onchain::usecases {
using namespace core::usecase;

Result<TimeLockSetupGetResult> TimeLockSetupGetUseCase::execute(const TimeLockSetupGetInput &input) {
    TimeLockSetupGetResult result;
    QJsonObject output;
    QString error_msg = "";
    bool ret{false};
    if (qUtils::isValidGroup(input.group_id)) {
        ret = Byzantine::instance()->GetWalletGroupWallet(input.group_id, input.wallet_id, output, error_msg);
    } else {
        ret = Draco::instance()->assistedWalletGetInfo(input.wallet_id, output, error_msg);
    }

    QJsonObject draftWallet = output["wallet"].toObject();
    QJsonObject timelock = draftWallet.value("timelock").toObject();

    if (ret) {
        result.timelock = timelock;
        return Result<TimeLockSetupGetResult>::success(result);
    } else {
        return Result<TimeLockSetupGetResult>::failure(error_msg);
    }
}

}