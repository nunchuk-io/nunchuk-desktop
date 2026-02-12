#include "GetInheritancePlanUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::inheritance::usecases {
using namespace core::usecase;

Result<GetInheritancePlanResult> GetInheritancePlanUseCase::execute(const GetInheritancePlanInput &input) {
    GetInheritancePlanResult result;
    QJsonObject output;
    QString error_msg = "";
    bool ret{false};
    if (qUtils::isValidGroup(input.group_id)) {
        ret = Draco::instance()->inheritanceGetPlan(input.wallet_id, input.group_id, output, error_msg);
    } else {
        ret = Draco::instance()->inheritanceGetPlan(input.wallet_id, "", output, error_msg);
    }

    QJsonObject inheritance = output["inheritance"].toObject();

    if (ret) {
        result.inheritance = inheritance;
        return Result<GetInheritancePlanResult>::success(result);
    } else {
        return Result<GetInheritancePlanResult>::failure(error_msg);
    }
}

}