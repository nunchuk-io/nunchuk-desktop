#include "TimeLockReplacementUpdateUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"
#include <QJsonArray>

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
    if (input.fallback_index >= 0) {
        data["fallback_index"] = input.fallback_index;
    }
    
    QJsonObject output;
    bool ret{false};
    if (qUtils::isValidGroup(input.group_id)) {
        ret = Byzantine::instance()->walletChangeTimelock(input.wallet_id, input.group_id, data, input.verifyToken, output);
    } else {
        ret = Draco::instance()->walletChangeTimelock(input.wallet_id, data, input.verifyToken, output);
    }

    TimeLockReplacementUpdateGetResult result;
    if (!ret) {
        QString message = output.value("message").toString();
        return Result<TimeLockReplacementUpdateGetResult>::failure(message.isEmpty() ? QString("Invalid timelock data") : message);
    }
    QJsonObject statusOutput;
    QString error_msg;
    bool fetched{false};
    if (qUtils::isValidGroup(input.group_id)) {
        fetched = Byzantine::instance()->GetKeyReplacementStatus(input.group_id, input.wallet_id, statusOutput, error_msg);
    } else {
        fetched = Draco::instance()->GetKeyReplacementStatus(input.wallet_id, statusOutput, error_msg);
    }
    QJsonObject status = statusOutput.value("status").toObject();
    QJsonObject timelock = status.value("timelock").toObject();
    if (input.fallback_index >= 0) {
        QJsonArray fallbacks = status.value("fallback_timelocks").toArray();
        timelock = input.fallback_index < fallbacks.size() ? fallbacks.at(input.fallback_index).toObject() : QJsonObject();
    }
    result.timelock = fetched && !timelock.isEmpty() ? timelock : value;
    return Result<TimeLockReplacementUpdateGetResult>::success(result);
}
}