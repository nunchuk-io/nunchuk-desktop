#include "SetupAnOnChainTimelockViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "ViewsEnums.h"

namespace features::draftwallets::onchain::viewmodels {
using namespace core::viewmodels;
using namespace features::common::usecases;

SetupAnOnChainTimelockViewModel::SetupAnOnChainTimelockViewModel(QObject *parent) 
: OnChainTimelockViewModel(parent)
{
}


void SetupAnOnChainTimelockViewModel::onInit() {
    GUARD_DASHBOARD()
    TimeLockSetupGetInput input;
    input.group_id = dashboard->groupId();    
    m_timeLockGetUC.executeAsync(input, [this](core::usecase::Result<TimeLockSetupGetResult> result){
        if (result.isSuccess()) {
            auto timelock = result.value().timelock;            
            clearTimeLock(timelock);
        }
    });
}

void SetupAnOnChainTimelockViewModel::onDispose() {
}

void SetupAnOnChainTimelockViewModel::setupTimeLock() {
    GUARD_DASHBOARD()
    TimeLockSetupUpdateInput input;
    input.group_id = dashboard->groupId();
    input.timezoneId = qUtils::extractTimeZoneId(valueTimezone());
    input.timestamp = qUtils::convertDateTimeToTimestamp(valueDate(), valueTime(), valueTimezone());
    input.based = based();
    input.block_height = blockHeight();
    input.isShowBlockHeight = isShowBlockHeight();
    m_timeLockUpdateUC.executeAsync(input, [this](core::usecase::Result<TimeLockSetupUpdateResult> result) {
        if (result.isSuccess()) {
            auto timelock = result.value().timelock;
            clearTimeLock(timelock);
            close();
            QEventProcessor::instance()->sendEvent(E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST);
        } else {
            emit invalidDateAlert();
        }
    });
}

void SetupAnOnChainTimelockViewModel::close() {
    OnChainTimelockViewModel::close();
    QEventProcessor::instance()->sendEvent(E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST);
}

} // namespace features::draftwallets::onchain::viewmodels
