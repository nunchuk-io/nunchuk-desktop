#include "WithdrawACustomAmountViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "features/claiming/flows/ClaimingFlow.h"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using features::claiming::flows::ClaimingFlow;

WithdrawACustomAmountViewModel::WithdrawACustomAmountViewModel(QObject *parent)
    : ActionViewModel(parent) {
}

void WithdrawACustomAmountViewModel::forwardAmount() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->currentFlow();
    auto claimingFlow = qobject_cast<ClaimingFlow*>(flow);
    if (claimingFlow) {
        claimingFlow->setwithdrawAmountSats(withdrawAmountSats());
    }
}

void WithdrawACustomAmountViewModel::withdrawToWalletClicked() {
    forwardAmount();
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::inheritances::qinheritancewidthdrawtowallet);
}

void WithdrawACustomAmountViewModel::withdrawToAddressClicked() {
    forwardAmount();
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::inheritances::qinheritancewidthdrawtoaddress);
}

} // namespace features::claiming::viewmodels