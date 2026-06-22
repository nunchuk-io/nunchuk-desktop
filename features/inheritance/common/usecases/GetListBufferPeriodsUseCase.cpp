#include "GetListBufferPeriodsUseCase.h"
#include "Servers/Draco.h"
#include <QJsonObject>

namespace features::inheritance::usecases {
using namespace core::usecase;

Result<GetListBufferPeriodsResult> GetListBufferPeriodsUseCase::execute(const GetListBufferPeriodsInput &input) {
    GetListBufferPeriodsResult result;
    QJsonArray periods;
    QString errormsg;

    bool ret = Draco::instance()->inheritancePlanBufferPeriod(periods, errormsg);
    if (!ret) {
        return Result<GetListBufferPeriodsResult>::failure(errormsg);
    }

    // Add "I don't need a buffer period" option
    QJsonObject notNeed;
    notNeed["id"] = "";
    notNeed["display_name"] = "I don't need a buffer period";
    notNeed["is_recommended"] = false;
    periods.append(notNeed);

    result.periods = periods;
    return Result<GetListBufferPeriodsResult>::success(result);
}

} // namespace features::inheritance::usecases
