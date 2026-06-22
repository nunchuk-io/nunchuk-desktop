#include "CreateInheritancePlanViewModel.h"
#include "Premiums/QInheritancePlan.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::inheritance::common::viewmodels {
using namespace core::viewmodels;

CreateInheritancePlanViewModel::CreateInheritancePlanViewModel(QObject *parent) : InheritancePlanViewModel(parent) {}

void CreateInheritancePlanViewModel::onInit() {
    InheritancePlanViewModel::onInit();
    GUARD_APP_MODEL_WALLET()
    auto inheritancePlanInfo = wallet->inheritancePlanPtr();
    auto planInfoNew = inheritancePlanInfo->planInfoNew().toMap();
    QString new_activation_date = planInfoNew.value("activation_date").toString();
    setactivation_date(new_activation_date);
    QString activation_timezone = planInfoNew.value("activation_timezone").toString();
    setactivation_timezone(activation_timezone);
    auto notification_preferences = planInfoNew.value("notification_preferences");
    setnotification_preferences(notification_preferences);
    QString note = planInfoNew.value("note").toString();
    setnote(note);
    processTimeData();
}

void CreateInheritancePlanViewModel::createPlan() {}

void CreateInheritancePlanViewModel::savePlan() {}
} // namespace features::inheritance::common::viewmodels
