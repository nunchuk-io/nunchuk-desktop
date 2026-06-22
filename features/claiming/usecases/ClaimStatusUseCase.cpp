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
        result.stages = output.value("stages").toArray();
        result.available_to_withdraw = output.value("available_to_withdraw").toDouble();
        result.current_stage_index = output.value("current_stage_index").toInt();
        result.current_installment_index = output.value("current_installment_index").toInt();
        #if 0
        QJsonObject inheritance;
        inheritance["distribution_method"] = "CUSTOMIZE";
        inheritance["note"] = "This is a note for the inheritance.";
        result.inheritance = inheritance;
        
        QJsonArray stages;
        QJsonArray timestamps;
        timestamps << 1767225600000LL << 1769904000000LL << 1772496000000LL << 1775174400000LL;
        for (int i = 0; i < 4; ++i) {
            QJsonObject stage;
            stage["amount_per_release_percentage"] = 25;
            stage["repeat_interval"] = "MONTH";
            stage["repeat_interval_count"] = 1;
            stage["total_stage_allocation_percentage"] = 25;
            stage["first_withdrawal_time_millis"] = timestamps[i];

            QJsonObject installment;
            installment["index"] = 0;
            installment["withdrawal_time_millis"] = timestamps[i];
            installment["allocation_percentage"] = 25;

            QJsonArray installments;
            installments.append(installment);
            installment["index"] = 1;
            installments.append(installment);
            installment["index"] = 2;
            installments.append(installment);
            stage["expanded_installments"] = installments;
            stages.append(stage);
        }
        result.stages = stages;
        result.available_to_withdraw = 0.001;
        result.current_stage_index = 1;
        result.current_installment_index = 1;
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
} // namespace features::claiming::usecases