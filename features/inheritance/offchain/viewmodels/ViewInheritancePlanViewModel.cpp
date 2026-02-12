#include "ViewInheritancePlanViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "core/utils/Utils.h"
#include "features/inheritance/offchain/flows/SetupOffChainTimelockFlow.h"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::usecases;
using features::inheritance::offchain::flows::SetupOffChainTimelockFlow;
const int DURATION = 2; // years
ViewInheritancePlanViewModel::ViewInheritancePlanViewModel(QObject *parent) 
: OffChainTimelockViewModel(parent)
{
}


void ViewInheritancePlanViewModel::onInit() {
    GUARD_SERVICE_WALLET_DASHBOARD()
    GetInheritancePlanInput input;
    input.wallet_id = dashboard->wallet_id();
    input.group_id = dashboard->groupId();
    m_getInheritancePlanUC.executeAsync(input, [this](core::usecase::Result<GetInheritancePlanResult> result){
        if (result.isSuccess()) {
            auto inheritance = result.value().inheritance;
            QJsonObject timelock;
            double activation_time_milis = inheritance.value("activation_time_milis").toDouble();
            if (activation_time_milis > 0) {
                timelock["value"] = inheritance.value("activation_time_milis");
                timelock["timezone"] = inheritance.value("timezone");
            }
            clearTimeLock(timelock);
        }
    });
}

void ViewInheritancePlanViewModel::onDispose() {
    
}

void ViewInheritancePlanViewModel::timeLockEditClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<SetupOffChainTimelockFlow>();
    if (isCurrentTime()) {
        flow->passInheritanceInfo(qUtils::increaseDateByYears(valueDate(), DURATION), valueTimezone());
    } else {
        flow->passInheritanceInfo(valueDate(), valueTimezone());
    }
    
    connect(flow, &SetupOffChainTimelockFlow::callbackTimelockChanged, this, [this](const QString& date, const QString& timezone){
        setvalueDate(date);
        setvalueTimezone(timezone);
    });
    subMng->show(qml::screens::onlinemode::setupwallets::timelocks::qsetupoffchaintimelockwallet);
}
} // namespace features::inheritance::offchain::viewmodels
