#include "LetConfigureYourWalletViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "ViewsEnums.h"

namespace features::draftwallets::onchain::viewmodels {
using namespace core::viewmodels;
using namespace features::common::usecases;

LetConfigureYourWalletViewModel::LetConfigureYourWalletViewModel(QObject *parent) 
: OnChainTimelockViewModel(parent)
{
}


void LetConfigureYourWalletViewModel::onInit() {
    GUARD_DASHBOARD()
    TimeLockSetupGetInput input;
    input.group_id = dashboard->groupId();
    input.fallback_index = -1;
    m_timeLockGetUC.executeAsync(input, [this](core::usecase::Result<TimeLockSetupGetResult> result){
        if (result.isSuccess()) {
            auto timelock = result.value().timelock;
            clearTimeLock(timelock, true);
        }
    });
}

void LetConfigureYourWalletViewModel::onConfigureClicked() {
    onConfigureFallbackClicked(-1);
}

void LetConfigureYourWalletViewModel::onConfigureFallbackClicked(int fallback_index) {
    GUARD_DASHBOARD()
    dashboard->setEditingFallbackIndex(fallback_index);
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::setupwallets::timelocks::qsetuponchaintimelockdraftwallet);
    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
}

} // namespace features::draftwallets::onchain::viewmodels
