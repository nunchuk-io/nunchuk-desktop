#include "ReleaseMethodViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::usecases;
using namespace features::inheritance::offchain::flows;

ReleaseMethodViewModel::ReleaseMethodViewModel(QObject *parent) : ActionViewModel(parent) {
}

void ReleaseMethodViewModel::onInit() {
    GUARD_SERVICE_WALLET_DASHBOARD()
    GetInheritancePlanInput input;
    input.wallet_id = dashboard->wallet_id();
    input.group_id = dashboard->groupId();
    m_getInheritancePlanUC.executeAsync(input, [this](core::usecase::Result<GetInheritancePlanResult> result) {
        if (result.isSuccess()) {
            auto inheritance = result.value().inheritance;
            setselectedMethod(inheritance.value("release_method").toString());
            setselectedMethodOld(selectedMethod());
        }
    });
}

void ReleaseMethodViewModel::onContinueClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QJsonObject inheritance = flow->inheritance();
    inheritance["release_method"] = selectedMethod();
    flow->setinheritance(inheritance);
    GUARD_SUB_SCREEN_MANAGER()
    if (qUtils::strCompare(selectedMethod(), "SHARED")) {
        subMng->show(qml::features::inheritance::offchain::qreleaseschedule);
    } else {
        subMng->show(qml::features::inheritance::offchain::qbeneficiaryschedules);
    }
}

} // namespace features::inheritance::offchain::viewmodels
