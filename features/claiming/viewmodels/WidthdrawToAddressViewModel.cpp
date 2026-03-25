#include "WidthdrawToAddressViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/flows/ClaimingFlow.h"
#include "features/transactions/flows/ClaimTransactionFlow.h"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using features::claiming::flows::ClaimingFlow;
using features::transactions::flows::ClaimTransactionFlow;

WidthdrawToAddressViewModel::WidthdrawToAddressViewModel(QObject *parent) : ActionViewModel(parent) {}

void WidthdrawToAddressViewModel::createTransactionToAddress(const QString &withdrawAddress) {
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto claimingFlow = qobject_cast<ClaimingFlow *>(currentFlow);
    if (claimingFlow) {
        claimingFlow->setwithdrawAddress(withdrawAddress);
        claimingFlow->setwithdrawAmountSats(withdrawAmountSats());
        claimingFlow->createTransaction();
    }
}

void WidthdrawToAddressViewModel::proceedTransactionResult() {
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto claimingFlow = qobject_cast<ClaimingFlow *>(currentFlow);
    if (!claimingFlow) {
        return;
    }
    auto flow = flowMng->startFlow<ClaimTransactionFlow>();
    flow->setworkFlowId(claimingFlow->id());
    flow->setnunTx(claimingFlow->nunTx());
    flow->setnunWallet(claimingFlow->nunWallet());
    flow->setmagicWord(claimingFlow->magicWord());
    flow->settokenList(claimingFlow->tokenList());
    flow->setmessageId(claimingFlow->messageId());
    flow->setbsms(claimingFlow->bsms());
    flow->setwithdrawAddress(claimingFlow->withdrawAddress());
    flow->setfee_rate(claimingFlow->fee_rate());
    flow->setanti_fee_sniping(claimingFlow->anti_fee_sniping());
    flow->setsubtract_fee_from_amount(claimingFlow->subtract_fee_from_amount());
    flow->setwithdrawAmountSats(claimingFlow->withdrawAmountSats());
    flow->setbackupSigners(claimingFlow->backupSigners());
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::claiming::qconfirmtransaction);
}

} // namespace features::claiming::viewmodels