#include "FirstWithdrawalDateViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

FirstWithdrawalDateViewModel::FirstWithdrawalDateViewModel(QObject *parent) : ActionViewModel(parent) {
    setinitialized(false);
    connect(this, &FirstWithdrawalDateViewModel::valueDateChanged, this, &FirstWithdrawalDateViewModel::onValidateDateTime);
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
    flow->seteditStageSelected(stage);
    GUARD_SUB_SCREEN_MANAGER()
    QJsonObject inheritance = flow->inheritance();
    QString release_method = inheritance.value("release_method").toString();
    if (qUtils::strCompare(release_method, "SHARED")) {
        QJsonArray stagesArray = inheritance["stages"].toArray();
        int index = stage["index"].toInt() - 1; // stage["index"] is 1-based; convert to 0-based array index
        // Validate index bounds before accessing array
        QJsonObject stageJs = stagesArray.at(index).toObject();
        stageJs["amount_per_release_percentage"] = stage.value("amountPerRelease").toDouble(); // NEW: amount released per interval as percentage
        stageJs["repeat_interval"] = stage.value("repeatInterval").toString(); // NEW: repeat interval unit (e.g., "DAY", "MONTH", "YEAR")
        stageJs["repeat_interval_count"] = stage.value("repeatEvery").toInt(); // NEW: repeat interval count (e.g., every 2 months)
        stageJs["total_stage_allocation_percentage"] = stage.value("percentage").toDouble(); // NEW: total allocation percentage for this stage
        stageJs["first_withdrawal_time_millis"] = timestamp;
        if (index < 0 || index >= stagesArray.size()) {
            stagesArray.append(stageJs);
        } else {
            stagesArray.replace(index, stageJs);
        }
        inheritance["stages"] = stagesArray;
        flow->setinheritance(inheritance);
        subMng->show(qml::features::inheritance::offchain::qreleaseschedule);
    } else {
        QJsonObject beneficiary = flow->beneficiarySelected();
        QJsonArray stagesArray = beneficiary["stages"].toArray();
        int index = stage["index"].toInt() - 1; // stage["index"] is 1-based; convert to 0-based array index
        // Validate index bounds before accessing array
        QJsonObject stageJs = stagesArray.at(index).toObject();
        stageJs["amount_per_release_percentage"] = stage.value("amountPerRelease").toDouble(); // NEW: amount released per interval as percentage
        stageJs["repeat_interval"] = stage.value("repeatInterval").toString(); // NEW: repeat interval unit (e.g., "DAY", "MONTH", "YEAR")
        stageJs["repeat_interval_count"] = stage.value("repeatEvery").toInt(); // NEW: repeat interval count (e.g., every 2 months)
        stageJs["total_stage_allocation_percentage"] = stage.value("percentage").toDouble(); // NEW: total allocation percentage for this stage
        stageJs["first_withdrawal_time_millis"] = timestamp;
        if (index < 0 || index >= stagesArray.size()) {
            stagesArray.append(stageJs);
        } else {
            stagesArray.replace(index, stageJs);
        }
        beneficiary["stages"] = stagesArray;
        flow->setbeneficiarySelected(beneficiary);
        subMng->show(qml::features::inheritance::offchain::qdetailbeneficiary);
    }
}

void FirstWithdrawalDateViewModel::onValidateDateTime() {
    if (!initialized()) {
        setinitialized(true);
        return; // Skip validation if not initialized
    }
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    int index = stage().toMap().value("index").toInt();
    if (index > 0) {
        auto prevFirstWithdrawalDate = stagesArray().at(index - 1).toObject();
        auto prevTimestamp = prevFirstWithdrawalDate.value("first_withdrawal_time_millis").toVariant().toLongLong();
        auto prevDateTime = helper::getDateFromTimestamp(prevTimestamp/1000);
        auto currentTimestamp = qUtils::convertDateTimeToTimestamp(valueDate(), valueTime(), "UTC");
        if (currentTimestamp <= prevTimestamp) {
            emit showToast(0, Strings.STR_QML_2233(), EWARNING::WarningType::ERROR_MSG);
        }

        if (index < stagesArray().size() - 1) {
            auto nextFirstWithdrawalDate = stagesArray().at(index + 1).toObject();
            auto nextTimestamp = nextFirstWithdrawalDate.value("first_withdrawal_time_millis").toVariant().toLongLong();
            if (currentTimestamp >= nextTimestamp) {
                emit showToast(0, Strings.STR_QML_2234(), EWARNING::WarningType::ERROR_MSG);
            }
        }
    }
}

} // namespace features::inheritance::offchain::viewmodels
