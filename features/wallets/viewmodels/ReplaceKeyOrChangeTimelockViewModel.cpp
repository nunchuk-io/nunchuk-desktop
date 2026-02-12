#include "ReplaceKeyOrChangeTimelockViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "ViewsEnums.h"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;
using namespace features::common::usecases;

ReplaceKeyOrChangeTimelockViewModel::ReplaceKeyOrChangeTimelockViewModel(QObject *parent) 
: OnChainTimelockViewModel(parent)
{
}


void ReplaceKeyOrChangeTimelockViewModel::onInit() {
    GUARD_DASHBOARD()
    TimeLockReplacementGetInput input;
    input.wallet_id = dashboard->wallet_id();
    input.group_id = dashboard->groupId();
    m_timeLockGetUC.executeAsync(input, [this](core::usecase::Result<TimeLockReplacementGetResult> result){
        if (result.isSuccess()) {
            auto timelock = result.value().timelock;            
            clearTimeLock(timelock);
        }
    });
}

void ReplaceKeyOrChangeTimelockViewModel::onDispose() {
}

void ReplaceKeyOrChangeTimelockViewModel::onConfigureClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::setupwallets::timelocks::qsetuponchaintimelockwallet);
    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
}

} // namespace features::wallets::viewmodels
