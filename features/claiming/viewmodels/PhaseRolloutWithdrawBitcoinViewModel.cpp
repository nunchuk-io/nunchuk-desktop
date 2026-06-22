#include "PhaseRolloutWithdrawBitcoinViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/flows/ClaimingFlow.h"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using features::claiming::flows::ClaimingFlow;

PhaseRolloutWithdrawBitcoinViewModel::PhaseRolloutWithdrawBitcoinViewModel(QObject *parent) : ActionViewModel(parent) {
    setwithdrawAmountSats(0);
}

void PhaseRolloutWithdrawBitcoinViewModel::next() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceyourinheritance);
}

void PhaseRolloutWithdrawBitcoinViewModel::back() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->back();
}

void PhaseRolloutWithdrawBitcoinViewModel::forwardAmount() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->currentFlow();
    auto claimingFlow = qobject_cast<ClaimingFlow *>(flow);
    if (claimingFlow) {
        if (qUtils::strCompare(claimingFlow->distribution_method(), "CUSTOMIZE")) {
            if (withdrawAmountSats() == 0) {
                claimingFlow->setwithdrawAmountSats(claimingFlow->availableBalanceSats()); // when withdrawing all, set amount to full balance (Don't set amount)
            } else {
                claimingFlow->setwithdrawAmountSats(withdrawAmountSats());
            }
        } else {
            claimingFlow->setwithdrawAmountSats(0); // when withdrawing all, set amount to full balance (Don't set amount)
        }
        
    }
}

void PhaseRolloutWithdrawBitcoinViewModel::onInit() {
    ActionViewModel::onInit();
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->currentFlow();
    auto claimingFlow = qobject_cast<ClaimingFlow *>(flow);
    if (claimingFlow) {
        setbalanceDisplay(claimingFlow->balanceDisplay());
        setbalanceCurrency(claimingFlow->balanceCurrency());
        setavailableBalanceDisplay(claimingFlow->availableBalanceDisplay());
        setavailableBalanceCurrency(claimingFlow->availableBalanceCurrency());
        setavailableBalanceSats(claimingFlow->availableBalanceSats());
        setbalanceSats(claimingFlow->balanceSats());
    }
}

void PhaseRolloutWithdrawBitcoinViewModel::customAMountClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->currentFlow();
    auto claimingFlow = qobject_cast<ClaimingFlow *>(flow);
    if (claimingFlow) {
        if (qUtils::strCompare(claimingFlow->distribution_method(), "CUSTOMIZE")) {
            claimingFlow->setwithdrawAmountSats(withdrawAmountSats());
        }
    }
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::inheritances::qinheritancewidthdrawacustomamount);
}

void PhaseRolloutWithdrawBitcoinViewModel::withdrawToWalletClicked() {
    forwardAmount();
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::inheritances::qinheritancewidthdrawtowallet);
}

void PhaseRolloutWithdrawBitcoinViewModel::withdrawToAddressClicked() {
    forwardAmount();
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::screens::onlinemode::inheritances::qinheritancewidthdrawtoaddress);
}

void PhaseRolloutWithdrawBitcoinViewModel::viewReleaseScheduleClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::components::rightpannel::service::common::qserviceclaiminheritancereleaseschedule);
}

} // namespace features::claiming::viewmodels
