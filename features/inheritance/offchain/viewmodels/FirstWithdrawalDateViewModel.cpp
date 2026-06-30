#include "FirstWithdrawalDateViewModel.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

FirstWithdrawalDateViewModel::FirstWithdrawalDateViewModel(QObject *parent) : ActionViewModel(parent) {
    // Validation is in onContinueClicked() — ensures toast appears on the correct screen.
}

void FirstWithdrawalDateViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QVariantMap stage = flow->editStageSelected();
    setstage(QVariant::fromValue(stage));
    QString formattedDate = stage.value("firstWithdrawalDate").toString();
    setvalueDate(formattedDate);
    QString formattedTime = stage.value("firstWithdrawalTime").toString();
    setvalueTime(formattedTime);
    QJsonObject inheritance = flow->inheritance();
    QString release_method = inheritance.value("release_method").toString();
    if (qUtils::strCompare(release_method, "SHARED")) {
        QJsonArray stagesArray = inheritance["stages"].toArray();
        setstagesArray(stagesArray);
    } else {
        QJsonArray stagesArray = flow->editStageSelected().value("stages").toJsonArray();
        setstagesArray(stagesArray);
    }
}

void FirstWithdrawalDateViewModel::onContinueClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->setcurrent_step(ConfigStep_t::PassViaStageConfig); // Set current step to indicate we're passing through stage config
    QVariantMap stage = flow->editStageSelected();
    stage["firstWithdrawalDate"] = valueDate();
    stage["firstWithdrawalTime"] = valueTime();
    auto timestamp = qUtils::convertDateTimeToTimestamp(valueDate(), valueTime(), flow->valueTimezone()) * 1000; // Convert to milliseconds

    GUARD_SUB_SCREEN_MANAGER()
    QJsonObject inheritance = flow->inheritance();
    QString release_method = inheritance.value("release_method").toString();
    int index = stage["index"].toInt() - 1; // stage["index"] is 1-based; convert to 0-based array index
    if (qUtils::strCompare(release_method, "SHARED")) {
        QJsonArray stagesArray = inheritance["stages"].toArray();

        // Validate: current stage must start AFTER the last installment of the previous stage
        // (matches checkStageCanGoNext / validateDateTime semantics exactly)
        if (index > 0 && index <= stagesArray.size()) {
            qint64 prevLastWithdrawalMs = helper::getLastStageWithdrawalTimeMillis(stagesArray.at(index - 1).toObject().toVariantMap());
            if (timestamp <= prevLastWithdrawalMs) {
                emit showToast(0, Strings.STR_QML_2233(), EWARNING::WarningType::ERROR_MSG);
                return; // Block navigation — user stays on this screen
            }
        }
        // Validate: current stage must start before the next stage's first withdrawal (if configured)
        if (index >= 0 && index + 1 < stagesArray.size()) {
            qint64 nextTimestampMs = stagesArray.at(index + 1).toObject().value("first_withdrawal_time_millis").toVariant().toLongLong();
            if (nextTimestampMs > 0 && timestamp >= nextTimestampMs) {
                emit showToast(0, Strings.STR_QML_2234(), EWARNING::WarningType::ERROR_MSG);
                return; // Block navigation
            }
        }

        QJsonObject stageJs = stagesArray.at(index).toObject();
        stageJs["amount_per_release_percentage"] = stage.value("amountPerRelease").toDouble();
        stageJs["repeat_interval"] = stage.value("repeatInterval").toString();
        stageJs["repeat_interval_count"] = stage.value("repeatEvery").toInt();
        stageJs["total_stage_allocation_percentage"] = stage.value("percentage").toDouble();
        stageJs["first_withdrawal_time_millis"] = timestamp;
        QJsonArray installments = helper::generateInstallmentSchedule(stageJs.toVariantMap());
        stageJs["expanded_installments"] = installments;
        if (index < 0 || index >= stagesArray.size()) {
            stagesArray.append(stageJs);
        } else {
            stagesArray.replace(index, stageJs);
        }
        inheritance["stages"] = stagesArray;
        auto stagesList = helper::convertStagesData(stagesArray);
        auto newStage = stagesList.at(index).toMap();
        flow->seteditStageSelected(newStage);
        flow->setinheritance(inheritance);
        subMng->show(qml::features::inheritance::offchain::qreleaseschedule);
    } else {
        QJsonObject beneficiary = flow->beneficiarySelected();
        QJsonArray stagesArray = beneficiary["stages"].toArray();

        // Validate: current stage must start AFTER the last installment of the previous stage
        // (matches checkStageCanGoNext / validateDateTime semantics exactly)
        if (index > 0 && index <= stagesArray.size()) {
            qint64 prevLastWithdrawalMs = helper::getLastStageWithdrawalTimeMillis(stagesArray.at(index - 1).toObject().toVariantMap());
            if (timestamp <= prevLastWithdrawalMs) {
                emit showToast(0, Strings.STR_QML_2233(), EWARNING::WarningType::ERROR_MSG);
                return; // Block navigation
            }
        }
        // Validate: current stage must start before the next stage's first withdrawal (if configured)
        if (index >= 0 && index + 1 < stagesArray.size()) {
            qint64 nextTimestampMs = stagesArray.at(index + 1).toObject().value("first_withdrawal_time_millis").toVariant().toLongLong();
            if (nextTimestampMs > 0 && timestamp >= nextTimestampMs) {
                emit showToast(0, Strings.STR_QML_2234(), EWARNING::WarningType::ERROR_MSG);
                return; // Block navigation
            }
        }

        QJsonObject stageJs = stagesArray.at(index).toObject();
        stageJs["amount_per_release_percentage"] = stage.value("amountPerRelease").toDouble();
        stageJs["repeat_interval"] = stage.value("repeatInterval").toString();
        stageJs["repeat_interval_count"] = stage.value("repeatEvery").toInt();
        stageJs["total_stage_allocation_percentage"] = stage.value("percentage").toDouble();
        stageJs["first_withdrawal_time_millis"] = timestamp;
        QJsonArray installments = helper::generateInstallmentSchedule(stageJs.toVariantMap());
        stageJs["expanded_installments"] = installments;
        if (index < 0 || index >= stagesArray.size()) {
            stagesArray.append(stageJs);
        } else {
            stagesArray.replace(index, stageJs);
        }
        beneficiary["stages"] = stagesArray;
        auto stagesList = helper::convertStagesData(stagesArray);
        auto newStage = stagesList.at(index).toMap();
        flow->seteditStageSelected(newStage);
        flow->setbeneficiarySelected(beneficiary);
        // Do NOT write to inheritance["beneficiaries"] here — this is mid-flow in-progress data
        // that may exceed 100%. inheritance["beneficiaries"] is only updated by
        // DetailBeneficiaryViewModel::onContinueClicked(), which is gated on totalPercentage <= 100.
        subMng->show(qml::features::inheritance::offchain::qdetailbeneficiary);
    }
}

} // namespace features::inheritance::offchain::viewmodels
