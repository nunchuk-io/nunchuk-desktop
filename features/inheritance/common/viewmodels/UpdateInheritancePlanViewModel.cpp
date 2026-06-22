#include "UpdateInheritancePlanViewModel.h"
#include "Premiums/QInheritancePlan.h"
#include "core/ui/UiServices.inc"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::common::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

UpdateInheritancePlanViewModel::UpdateInheritancePlanViewModel(QObject *parent) : InheritancePlanViewModel(parent) {}

void UpdateInheritancePlanViewModel::onInit() {
    InheritancePlanViewModel::onInit();
    GUARD_APP_MODEL_WALLET()
    auto inheritancePlanInfo = wallet->inheritancePlanPtr();
    auto planInfoUpdate = inheritancePlanInfo->planInfoUpdate().toMap();
    QString update_activation_date = planInfoUpdate.value("activation_date").toString();
    setactivation_date(update_activation_date);
    QString activation_timezone = planInfoUpdate.value("activation_timezone").toString();
    setactivation_timezone(activation_timezone);
    auto notification_preferences = planInfoUpdate.value("notification_preferences");
    setnotification_preferences(notification_preferences);
    QString note = planInfoUpdate.value("note").toString();
    setnote(note);
    QString display_emails = planInfoUpdate.value("display_emails").toString();
    setdisplay_emails(display_emails);
    processTimeData();
    phaseRolloutRefresh();
}

void UpdateInheritancePlanViewModel::phaseRolloutRefresh() {
    
}

} // namespace features::inheritance::common::viewmodels
