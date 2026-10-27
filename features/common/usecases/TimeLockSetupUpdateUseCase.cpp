#include "TimeLockSetupUpdateUseCase.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"
#include <QJsonArray>

namespace features::common::usecases {
using namespace core::usecase;

Result<TimeLockSetupUpdateResult> TimeLockSetupUpdateUseCase::execute(const TimeLockSetupUpdateInput &input) {
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
    QString error_msg = "";
    bool ret{false};

    DBG_INFO << data << input.based;

    bool isGroup = qUtils::isValidGroup(input.group_id);
    if (input.fallback_index >= 0) {
        if (isGroup) {
            ret = Byzantine::instance()->DraftWalletUpdateFallback(input.group_id, input.fallback_index, data, output, error_msg);
        } else {
            ret = Draco::instance()->DraftWalletUpdateFallback(input.fallback_index, data, output, error_msg);
        }
    } else if (isGroup) {
        ret = Byzantine::instance()->DraftWalletUpdateTimelock(input.group_id, data, output, error_msg);
    } else {
        ret = Draco::instance()->DraftWalletUpdateTimelock(data, output, error_msg);
    }
    QJsonObject draftWallet = output.value("draft_wallet").toObject();
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
    TimeLockSetupUpdateResult result;
    if (ret && !timelock.isEmpty()) {
        result.timelock = timelock;
        result.draft_wallet = draftWallet;
        return Result<TimeLockSetupUpdateResult>::success(result);
    } else {
        return Result<TimeLockSetupUpdateResult>::failure(QString("Invalid timelock data"));
    }
}

}
