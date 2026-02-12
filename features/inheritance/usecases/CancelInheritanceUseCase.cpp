#include "CancelInheritanceUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::inheritance::usecases {
using namespace core::usecase;

Result<CancelInheritanceResult> CancelInheritanceUseCase::execute(const CancelInheritanceInput &input) {
    CancelInheritanceResult result;
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
        return Result<CancelInheritanceResult>::success(result);
    } else {
        return Result<CancelInheritanceResult>::failure(error_msg);
    }
}

}