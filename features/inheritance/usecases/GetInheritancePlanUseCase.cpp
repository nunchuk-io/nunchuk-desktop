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
#if 0
    QString distribution_method = inheritance.value("distribution_method").toString();
    // Example: Create test data for SINGLE BENEFICIARY with CUSTOMIZE distribution
    if (distribution_method.isEmpty()) {

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

        QJsonObject fallbackPolicy;
        fallbackPolicy["type"] = "INACTIVITY";
        fallbackPolicy["inactivity_interval"] = "YEAR";
        fallbackPolicy["inactivity_interval_count"] = 1;
        fallbackPolicy["fallback_time_millis"] = 0;

        QJsonObject bufferPeriod;
        bufferPeriod["id"] = "buffer_7d";
        bufferPeriod["interval"] = "DAY";
        bufferPeriod["interval_count"] = 7;
        bufferPeriod["enabled"] = true;
        bufferPeriod["is_recommended"] = true;
        bufferPeriod["display_name"] = "7 days";

        inheritance["distribution_method"] = "CUSTOMIZE";
        inheritance["beneficiary_mode"] = "MULTIPLE";
        inheritance["buffer_apply_on"] = "FIRST_WITHDRAWAL";
        inheritance["release_method"] = "INDIVIDUAL";
        inheritance["fallback_policy"] = fallbackPolicy;
        inheritance["stages"] = stages;
        inheritance["buffer_period"] = bufferPeriod;

        // Create 3 beneficiaries
        QJsonArray beneficiaries;
        QJsonArray emails;
        emails << "alice@example.com" << "bob@example.com" << "charlie@example.com";
        QJsonArray percentages;
        percentages << 50 << 30 << 20;
        QJsonArray notes;
        notes << "Primary beneficiary - spouse" << "Secondary beneficiary - son" << "Tertiary beneficiary - daughter";

        for (int i = 0; i < 3; ++i) {
            QJsonObject beneficiary;
            beneficiary["email"] = emails[i].toString();
            beneficiary["asset_percentage"] = percentages[i].toInt();
            beneficiary["magic"] = QString("MAGIC_BENEFICIARY_%1").arg(i + 1);
            beneficiary["note"] = notes[i].toString();
            beneficiary["buffer_period_id"] = "buffer_30d";

            QJsonObject bufferPeriod;
            bufferPeriod["id"] = "buffer_30d";
            bufferPeriod["interval"] = "DAY";
            bufferPeriod["interval_count"] = 3 * i;
            bufferPeriod["enabled"] = true;
            bufferPeriod["is_recommended"] = true;
            bufferPeriod["display_name"] = QString("%1 days").arg(3 * i);
            beneficiary["buffer_period"] = bufferPeriod;

            beneficiary["buffer_apply_on"] = "FIRST_WITHDRAWAL";

            QJsonArray beneficiaryStages;
            int numStages = i + 1; // 1 stage for alice, 2 for bob, 3 for charlie

            for (int j = 0; j < numStages; ++j) {
                QJsonObject beneficiaryStage;
                beneficiaryStage["amount_per_release_percentage"] = 20 + i * 5;
                beneficiaryStage["repeat_interval"] = "MONTH";
                beneficiaryStage["repeat_interval_count"] = 1;
                beneficiaryStage["total_stage_allocation_percentage"] = percentages[i].toInt() / numStages;
                beneficiaryStage["first_withdrawal_time_millis"] = 1767225600000LL + (i * 2592000000LL) + (j * 2592000000LL);

                QJsonObject benInstallment;
                benInstallment["index"] = 0;
                benInstallment["withdrawal_time_millis"] = 1767225600000LL + (i * 2592000000LL) + (j * 2592000000LL);
                benInstallment["allocation_percentage"] = percentages[i].toInt() / numStages;

                QJsonArray benInstallments;
                benInstallments.append(benInstallment);
                benInstallment["index"] = 1;
                benInstallments.append(benInstallment);
                benInstallment["index"] = 2;
                benInstallments.append(benInstallment);
                beneficiaryStage["expanded_installments"] = benInstallments;
                beneficiaryStages.append(beneficiaryStage);
            }

            beneficiary["stages"] = beneficiaryStages;
            beneficiaries.append(beneficiary);
        }

        inheritance["beneficiaries"] = beneficiaries;
    }
#endif

    if (ret) {
        result.inheritance = inheritance;
        return Result<GetInheritancePlanResult>::success(result);
    } else {
        return Result<GetInheritancePlanResult>::failure(error_msg);
    }
}

} // namespace features::inheritance::usecases