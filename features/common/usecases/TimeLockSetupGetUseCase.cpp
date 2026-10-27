#include "TimeLockSetupGetUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"
#include <QJsonArray>

namespace features::common::usecases {
using namespace core::usecase;

Result<TimeLockSetupGetResult> TimeLockSetupGetUseCase::execute(const TimeLockSetupGetInput &input) {
    TimeLockSetupGetResult result;
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
    if (input.fallback_index >= 0) {
        timelock = {};
        QJsonArray fallbacks = draftWallet.value("onchain_inheritance_policy").toObject().value("fallbacks").toArray();
        for (const auto &js : fallbacks) {
            QJsonObject fallback = js.toObject();
            if (fallback.value("index").toInt(-1) == input.fallback_index) {
                timelock = fallback.value("timelock").toObject();
                break;
            }
        }
    }

    if (ret) {
        result.timelock = timelock;
        return Result<TimeLockSetupGetResult>::success(result);
    } else {
        return Result<TimeLockSetupGetResult>::failure(error_msg);
    }
}

}