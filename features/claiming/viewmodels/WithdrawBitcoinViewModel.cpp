#include "WithdrawBitcoinViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "features/claiming/flows/ClaimingFlow.h"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using features::claiming::flows::ClaimingFlow;

WithdrawBitcoinViewModel::WithdrawBitcoinViewModel(QObject *parent)
    : ActionViewModel(parent) {
}

void WithdrawBitcoinViewModel::next() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceyourinheritance);
}

void WithdrawBitcoinViewModel::back() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->back();
}

void WithdrawBitcoinViewModel::forwardAmount() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->currentFlow();
    auto claimingFlow = qobject_cast<ClaimingFlow*>(flow);
    if (claimingFlow) {
        claimingFlow->setwithdrawAmountSats(0); // when withdrawing all, set amount to full balance (Don't set amount)
    }
}

void WithdrawBitcoinViewModel::customAMountClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::inheritances::qinheritancewidthdrawacustomamount);
}

void WithdrawBitcoinViewModel::withdrawToWalletClicked() {
    forwardAmount();
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::inheritances::qinheritancewidthdrawtowallet);
}

void WithdrawBitcoinViewModel::withdrawToAddressClicked() {
    forwardAmount();
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::inheritances::qinheritancewidthdrawtoaddress);
}

} // namespace features::claiming::viewmodels