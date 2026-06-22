#include "SetUpBufferPeriodWithdrawalViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

SetUpBufferPeriodWithdrawalViewModel::SetUpBufferPeriodWithdrawalViewModel(QObject *parent) : ActionViewModel(parent) {
    setbuffer_apply_on("FIRST_WITHDRAWAL"); // default to first withdrawal
}

void SetUpBufferPeriodWithdrawalViewModel::onInit() {
    // Initialize view model
}

void SetUpBufferPeriodWithdrawalViewModel::onContinueClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->setcurrent_step(ConfigStep_t::PassViaBufferPeriod); // Set current step to indicate we're passing through buffer period config
    GUARD_SUB_SCREEN_MANAGER()
    QJsonObject inheritance = flow->inheritance();
    QString release_method = inheritance.value("release_method").toString();
    if (qUtils::strCompare(release_method, "SHARED")) {
        inheritance["buffer_apply_on"] = buffer_apply_on();
        flow->setinheritance(inheritance);
        subMng->show(qml::features::inheritance::offchain::qreleaseschedule);
    } else {
        QJsonObject beneficiarySelected = flow->beneficiarySelected();
        beneficiarySelected["buffer_apply_on"] = buffer_apply_on();
        flow->setbeneficiarySelected(beneficiarySelected);
        subMng->show(qml::features::inheritance::offchain::qdetailbeneficiary);
    }
}

} // namespace features::inheritance::offchain::viewmodels
