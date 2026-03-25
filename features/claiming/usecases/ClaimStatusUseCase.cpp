#include "ClaimStatusUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::claiming::usecases {
using namespace core::usecase;

Result<ClaimStatusResult> ClaimStatusUseCase::execute(const ClaimStatusInput &input) {
    QJsonObject body;
    body["magic"] = input.magic;
    body["bsms"] = input.bsms;
    body["message_id"] = input.messageId;
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = body;
    ClaimStatusResult result;
    QJsonObject output;
    bool ret = Draco::instance()->inheritanceClaimStatus(data, input.authos, output);
    if (ret) {
        result.inheritance = output.value("inheritance").toObject();
        result.balance = output.value("balance").toDouble();
        result.buffer_period_countdown = output.value("buffer_period_countdown").toObject();
        #if 0
        QJsonObject buffer;
        buffer["activation_time_milis"] = 1700000000000.0;
        buffer["claimable_time_milis"] = 1700003600000.0;
        buffer["buffer_interval"] = "daily";
        buffer["buffer_interval_count"] = 3;
        buffer["remaining_count"] = 2;
        buffer["remaining_display_name"] = "2 days remaining";
        result.buffer_period_countdown = buffer;
        #endif
        return Result<ClaimStatusResult>::success(result);
    } else {
        int response_code = output.value("code").toInt();
        QString error_msg = output.value("message").toString();
        return Result<ClaimStatusResult>::failure(QString("Error %1: %2").arg(response_code).arg(error_msg));
    }
}
}