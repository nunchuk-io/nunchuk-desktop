#include "CreateInheritancePlanUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::inheritance::usecases {
using namespace core::usecase;

Result<CreateInheritancePlanResult> CreateInheritancePlanUseCase::execute(const CreateInheritancePlanInput &input) {
    CreateInheritancePlanResult result;
    QJsonObject output;
    QString error_msg = "";
    bool ret{false};
    if (qUtils::isValidGroup(input.group_id)) {
        ret = Byzantine::instance()->GetCurrentGroupDraftWallet(input.group_id, output, error_msg);
    } else {
        ret = Draco::instance()->DraftWalletGetCurrent(output, error_msg);
    }

    QJsonObject draftWallet = output["draft_wallet"].toObject();
    QJsonObject timelock = draftWallet.value("timelock").toObject();

    if (ret) {
        result.timelock = timelock;
        return Result<CreateInheritancePlanResult>::success(result);
    } else {
        return Result<CreateInheritancePlanResult>::failure(error_msg);
    }
}

}