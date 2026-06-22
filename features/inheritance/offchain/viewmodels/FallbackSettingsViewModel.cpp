#include "FallbackSettingsViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

FallbackSettingsViewModel::FallbackSettingsViewModel(QObject *parent) : ActionViewModel(parent) {
    setselectedFallback("NONE");
    setrepeatEvery(1);
    setinactivityUnit("DAY");
}

void FallbackSettingsViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QVariantMap stage = flow->editStageSelected();
    QString formattedDate = stage.value("firstWithdrawalDate").toString();
    if (formattedDate.isEmpty()) {
        QDateTime dt = QDateTime::currentDateTime();
        formattedDate = dt.toString("MM/dd/yyyy");
    }
    setvalueDate(formattedDate);
    setfallbackOption((FallbackOption)flow->fallbackOption());
}

void FallbackSettingsViewModel::onContinueClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QJsonObject inheritance = flow->inheritance();
    QJsonObject fallbackPolicy = inheritance.value("fallback_policy").toObject();
    fallbackPolicy["type"] = selectedFallback();
    if(qUtils::strCompare(selectedFallback(), "DATE_BASED")) {
        QDateTime dt = QDateTime::currentDateTime();
        QString formattedTime = dt.toString("HH:mm");
        auto timestamp = qUtils::convertDateTimeToTimestamp(valueDate(), formattedTime, flow->valueTimezone())*1000; // Convert to milliseconds
        fallbackPolicy["fallback_time_millis"] = timestamp;
    } else if (qUtils::strCompare(selectedFallback(), "INACTIVITY")) {
        fallbackPolicy["inactivity_interval"] = inactivityUnit();
        fallbackPolicy["inactivity_interval_count"] = repeatEvery();
    }
    inheritance["fallback_policy"] = fallbackPolicy;
    flow->setinheritance(inheritance);
    if (fallbackOption() == FallbackOption::FallbackSecond) {
        flow->setcurrent_step(ConfigStep_t::PassViaFallbackSettings); // Set current step to indicate we're passing through fallback settings config
        GUARD_SUB_SCREEN_MANAGER()
        subMng->show(qml::features::inheritance::offchain::qbeneficiaryschedules);
    } else {
        close();
    }
}

} // namespace features::inheritance::offchain::viewmodels
