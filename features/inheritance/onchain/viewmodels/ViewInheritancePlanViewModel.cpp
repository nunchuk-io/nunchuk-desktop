#include "ViewInheritancePlanViewModel.h"
#include "ViewsEnums.h"
#include "core/ui/UiServices.inc"
#include "features/inheritance/common/flows/InheritanceFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::onchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::onchain::usecases;
using namespace features::inheritance::common::flows;

ViewInheritancePlanViewModel::ViewInheritancePlanViewModel(QObject *parent) : OnChainTimelockViewModel(parent) {}

void ViewInheritancePlanViewModel::onInit() {
    GUARD_SERVICE_WALLET_DASHBOARD()
    TimeLockSetupGetInput input;
    input.wallet_id = dashboard->wallet_id();
    input.group_id = dashboard->groupId();
    m_timeLockGetUC.executeAsync(input, [this](core::usecase::Result<TimeLockSetupGetResult> result) {
        if (result.isSuccess()) {
            auto timelock = result.value().timelock;
            clearTimeLock(timelock);
        }
    });
}

void ViewInheritancePlanViewModel::discardChanges() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<InheritanceFlow>();
    if (!flow) {
        return;
    }
    flow->discardChanges();
}

void ViewInheritancePlanViewModel::finalizeChanges() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<InheritanceFlow>();
    if (!flow) {
        return;
    }
    flow->finalizeInheritanceChanges();
}

void ViewInheritancePlanViewModel::securityQuestionVerified() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<InheritanceFlow>();
    if (!flow) {
        return;
    }
    flow->securityQuestionVerified();
}

void ViewInheritancePlanViewModel::cancelInheritancePlan() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<InheritanceFlow>();
    if (!flow) {
        return;
    }
    flow->cancelInheritancePlan();
}

} // namespace features::inheritance::onchain::viewmodels
