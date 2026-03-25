#include "ProceedOptionsViewModel.h"
#include "core/bridge/ExternalBridges.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/flows/OffChainClaimingFlow.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "features/signers/viewmodels/WhichTypeOfKeySelectionViewModel.h"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using namespace features::signers::flows;
using namespace features::signers::viewmodels;
using features::claiming::flows::OffChainClaimingFlow;

ProceedOptionsViewModel::ProceedOptionsViewModel(QObject *parent) : ActionViewModel(parent) {}

void ProceedOptionsViewModel::confirmProceed() {
    if (isBackupPasswordUsed()) {
        GUARD_RIGHT_PANEL_NAV()
        rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceinputbackuppassword);
    } else {
        GUARD_SUB_SCREEN_MANAGER()
        GUARD_FLOW_MANAGER()
        auto currentFlowId = flowMng->currentFlow()->id();
        auto flow = flowMng->startFlow<KeySetupFlow>();
        flow->setworkFlowId(currentFlowId);
        subMng->show(qml::features::signers::qwhichtypeofkeyselection);
    }
}
} // namespace features::claiming::viewmodels