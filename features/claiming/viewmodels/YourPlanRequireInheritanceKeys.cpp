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
    // BUGFIX: do NOT call close() here - unlike onAddFirstKeyClicked() (which navigates via
    // rightPanel->request(), a separate stack), this screen uses subMng->show(), and
    // close()/subMng->clear() runs synchronously on the same stack, wiping out the show()
    // above before the user ever sees it (blank subscreen).
    subMng->show(qml::features::signers::qwhichtypeofkeyselection);
}

} // namespace features::claiming::viewmodels