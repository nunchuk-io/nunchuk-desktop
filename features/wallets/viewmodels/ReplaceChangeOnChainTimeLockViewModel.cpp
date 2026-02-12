#include "ReplaceChangeOnChainTimeLockViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "ViewsEnums.h"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;
using namespace features::common::usecases;

ReplaceChangeOnChainTimeLockViewModel::ReplaceChangeOnChainTimeLockViewModel(QObject *parent) 
: OnChainTimelockViewModel(parent)
{
}


void ReplaceChangeOnChainTimeLockViewModel::onInit() {
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

void ReplaceChangeOnChainTimeLockViewModel::onDispose() {
}

void ReplaceChangeOnChainTimeLockViewModel::setupTimeLock() {
    GUARD_DASHBOARD()
    GUARD_SERVICE_TAG()
    TimeLockReplacementUpdateGetInput input;
    input.wallet_id = dashboard->wallet_id();
    input.group_id = dashboard->groupId();
    input.timezoneId = qUtils::extractTimeZoneId(valueTimezone());
    input.timestamp = qUtils::convertDateTimeToTimestamp(valueDate(), valueTime(), valueTimezone());
    input.based = based();
    input.block_height = blockHeight();
    input.isShowBlockHeight = isShowBlockHeight();
    input.verifyToken = serviceTag->passwordToken();
    m_timeLockUpdateUC.executeAsync(input, [this](core::usecase::Result<TimeLockReplacementUpdateGetResult> result) {
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

} // namespace features::wallets::viewmodels
