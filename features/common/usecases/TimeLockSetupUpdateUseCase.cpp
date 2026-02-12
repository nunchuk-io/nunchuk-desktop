#include "TimeLockSetupUpdateUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::common::usecases {
using namespace core::usecase;

Result<TimeLockSetupUpdateResult> TimeLockSetupUpdateUseCase::execute(const TimeLockSetupUpdateInput &input) {
    QJsonObject data;
    QJsonObject value;
    value["value"] = input.timestamp;
    value["timezone"] = input.timezoneId;
    if (input.isShowBlockHeight) {
        value["based"] = input.based;
        value["block_height"] = input.block_height;
    }
    data["timelock"] = value;
    QJsonObject output;
    QString error_msg = "";
    bool ret{false};
    if (qUtils::isValidGroup(input.group_id)) {
        ret = Byzantine::instance()->DraftWalletUpdateTimelock(input.group_id, data, output, error_msg);
    } else {
        ret = Draco::instance()->DraftWalletUpdateTimelock(data, output, error_msg);
    }
    QJsonObject draftWallet = output.value("draft_wallet").toObject();
    QJsonObject timelock = draftWallet.value("timelock").toObject();
    TimeLockSetupUpdateResult result;
    if (!timelock.isEmpty()) {
        result.timelock = timelock;
        return Result<TimeLockSetupUpdateResult>::success(result);
    } else {
        return Result<TimeLockSetupUpdateResult>::failure(QString("Invalid timelock data"));
    }
}

}