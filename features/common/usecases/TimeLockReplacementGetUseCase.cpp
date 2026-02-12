#include "TimeLockReplacementGetUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::common::usecases {
using namespace core::usecase;

Result<TimeLockReplacementGetResult> TimeLockReplacementGetUseCase::execute(const TimeLockReplacementGetInput &input) {
    TimeLockReplacementGetResult result;
    QJsonObject output;
    QString error_msg = "";
    bool ret{false};
    
    if (qUtils::isValidGroup(input.group_id)) {
        ret = Byzantine::instance()->GetKeyReplacementStatus(input.group_id, input.wallet_id, output, error_msg);
    } else {
        ret = Draco::instance()->GetKeyReplacementStatus(input.wallet_id, output, error_msg);
    }

    QJsonObject status = output["status"].toObject();
    QJsonObject timelock = status.value("timelock").toObject();

    if (ret) {
        result.timelock = timelock;
        return Result<TimeLockReplacementGetResult>::success(result);
    } else {
        return Result<TimeLockReplacementGetResult>::failure(QString("Invalid timelock data"));
    }
}
}