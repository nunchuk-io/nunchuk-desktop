#include "SetupAnOffChainTimelockViewModel.h"
#include "core/ui/UiServices.inc"
#include "Premiums/QInheritancePlan.h"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;

SetupAnOffChainTimelockViewModel::SetupAnOffChainTimelockViewModel(QObject *parent) 
: OffChainTimelockViewModel(parent)
{
}


void SetupAnOffChainTimelockViewModel::onInit() {
    
}

void SetupAnOffChainTimelockViewModel::onDispose() {
    
}

void SetupAnOffChainTimelockViewModel::timeLockContinueClicked() {
    GUARD_SERVICE_WALLET_INHERITANCE_PLAN()
    QJsonObject timeChange;
    timeChange["activation_date"] = valueDate();
    timeChange["activation_timezone"] = valueTimezone();
    ihPlan->editPlanInfo(QVariant::fromValue(timeChange));
}

} // namespace features::inheritance::offchain::viewmodels
