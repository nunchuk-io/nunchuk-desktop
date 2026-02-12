#include "TimeLockConvertUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::common::usecases {
using namespace core::usecase;

Result<TimeLockConvertResult> TimeLockConvertUseCase::execute(const TimeLockConvertInput &input) {
    QJsonObject data;
    QJsonObject value;
    value["value"] = input.timestamp;
    value["timezone"] = input.timezoneId;
    if (input.isShowBlockHeight) {
        value["based"] = "TIME_LOCK";
        value["block_height"] = QJsonValue();
    }
    data["timelock"] = value;
    QJsonObject output;
    bool ret = Draco::instance()->TimeLockConvert(data, output);
    QJsonObject timelock = output.value("converted_timelock").toObject();
    TimeLockConvertResult result;
    if (ret) {
        result.timelock = timelock;
        return Result<TimeLockConvertResult>::success(result);
    } else {
        return Result<TimeLockConvertResult>::failure(QString("Invalid timelock data"));
    }
}

}