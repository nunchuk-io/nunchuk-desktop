#include "ViewInheritancePlanViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "ViewsEnums.h"

namespace features::inheritance::onchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::onchain::usecases;

ViewInheritancePlanViewModel::ViewInheritancePlanViewModel(QObject *parent) 
: OnChainTimelockViewModel(parent)
{
}


void ViewInheritancePlanViewModel::onInit() {
    GUARD_SERVICE_WALLET_DASHBOARD()
    TimeLockSetupGetInput input;
    input.wallet_id = dashboard->wallet_id();
    input.group_id = dashboard->groupId();    
    m_timeLockGetUC.executeAsync(input, [this](core::usecase::Result<TimeLockSetupGetResult> result){
        if (result.isSuccess()) {
            auto timelock = result.value().timelock;            
            clearTimeLock(timelock);
        }
    });
}

void ViewInheritancePlanViewModel::onDispose() {
    
}

} // namespace features::inheritance::onchain::viewmodels
