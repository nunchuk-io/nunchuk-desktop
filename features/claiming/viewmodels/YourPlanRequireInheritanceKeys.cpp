#include "YourPlanRequireInheritanceKeys.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/KeySetupFlow.h"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using namespace features::signers::flows;
YourPlanRequireInheritanceKeys::YourPlanRequireInheritanceKeys(QObject *parent) : ActionViewModel(parent) {}

void YourPlanRequireInheritanceKeys::next() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::claiming::onchain::qprepareinheritancekey);
}

void YourPlanRequireInheritanceKeys::onAddFirstKeyClicked() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceoptionsproceed);
    close();
}
void YourPlanRequireInheritanceKeys::onAddSecondKeyClicked() {
    // Subsequent inheritance keys skip "How would you like to proceed?" (D02) and go
    // straight to "Add inheritance key" (D04) - same flow-handoff as ProceedOptionsViewModel.
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    auto currentFlowId = flowMng->currentFlow()->id();
    auto flow = flowMng->startFlow<KeySetupFlow>();
    flow->setworkFlowId(currentFlowId);
    subMng->show(qml::features::signers::qwhichtypeofkeyselection);
    close();
}

} // namespace features::claiming::viewmodels