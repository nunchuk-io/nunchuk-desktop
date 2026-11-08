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
    input.fallback_index = dashboard->isConfigurableReplacement() ? dashboard->editingFallbackIndex() : -1;
    m_timeLockGetUC.executeAsync(input, [this](core::usecase::Result<TimeLockReplacementGetResult> result){
        if (result.isSuccess()) {
            auto timelock = result.value().timelock;            
            clearTimeLock(timelock);
        }
    });
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
    bool configurable = dashboard->isConfigurableReplacement();
    input.fallback_index = configurable ? dashboard->editingFallbackIndex() : -1;
    m_timeLockUpdateUC.executeAsync(input, [this, configurable](core::usecase::Result<TimeLockReplacementUpdateGetResult> result) {
        if (result.isSuccess()) {
            auto timelock = result.value().timelock;
            clearTimeLock(timelock);
            if (configurable) {
                refreshReplacement();
            }
            close();
            QEventProcessor::instance()->sendEvent(E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST);
        } else {
            emit invalidDateAlert(result.error());
        }
    });
}

double ReplaceChangeOnChainTimeLockViewModel::selectedTimestamp() const {
    return qUtils::convertDateTimeToTimestamp(valueDate(), valueTime(), valueTimezone());
}

void ReplaceChangeOnChainTimeLockViewModel::refreshReplacement() {
    GUARD_DASHBOARD()
    dashboard->GetKeyReplacementStatus();
}

void ReplaceChangeOnChainTimeLockViewModel::resetEditingFallback() {
    GUARD_DASHBOARD()
    if (dashboard->isConfigurableReplacement()) {
        dashboard->setEditingFallbackIndex(-1);
    }
}

void ReplaceChangeOnChainTimeLockViewModel::close() {
    resetEditingFallback();
    OnChainTimelockViewModel::close();
}

} // namespace features::wallets::viewmodels
