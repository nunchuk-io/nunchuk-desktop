#include "TimeLockReplacementUpdateUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::common::usecases {
using namespace core::usecase;

Result<TimeLockReplacementUpdateGetResult> TimeLockReplacementUpdateUseCase::execute(const TimeLockReplacementUpdateGetInput &input) {
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
    bool ret{false};
    if (qUtils::isValidGroup(input.group_id)) {
        ret = Byzantine::instance()->walletChangeTimelock(input.wallet_id, input.group_id, data, input.verifyToken, output);
    } else {
        ret = Draco::instance()->walletChangeTimelock(input.wallet_id, data, input.verifyToken, output);
    }

    QJsonObject status = output["status"].toObject();
    QJsonObject timelock = status.value("timelock").toObject();
    TimeLockReplacementUpdateGetResult result;
    if (!timelock.isEmpty()) {
        result.timelock = timelock;
        return Result<TimeLockReplacementUpdateGetResult>::success(result);
    } else {
        return Result<TimeLockReplacementUpdateGetResult>::failure(QString("Invalid timelock data"));
    }
}
}