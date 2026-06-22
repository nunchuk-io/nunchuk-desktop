#include "GetOrCreateAssociateMagicUseCase.h"
#include "Servers/Draco.h"
#include <QJsonObject>

namespace features::inheritance::usecases {
using namespace core::usecase;

Result<GetOrCreateAssociateMagicResult> GetOrCreateAssociateMagicUseCase::execute(const GetOrCreateAssociateMagicInput &input) {
    GetOrCreateAssociateMagicResult result;
    QJsonObject output;
    QString errormsg;
    QJsonObject data;
    data["wallet"] = input.wallet_id;
    data["group_id"] = input.group_id;
    data["beneficiaries"] = input.beneficiaries;

    bool ret = Draco::instance()->inheritanceAssociateMagic(data, output, errormsg);
    if (!ret) {
        return Result<GetOrCreateAssociateMagicResult>::failure(errormsg);
    }

    result.output = output;
    return Result<GetOrCreateAssociateMagicResult>::success(result);
}

} // namespace features::inheritance::usecases
