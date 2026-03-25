#include "ConfirmTransactionViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/flows/ClaimingFlow.h"
#include "features/transactions/flows/TransactionFlow.h"
#include "generated_qml_keys.hpp"

namespace features::transactions::viewmodels {
using features::claiming::flows::ClaimingFlow;
using features::transactions::flows::TransactionFlow;

ConfirmTransactionViewModel::ConfirmTransactionViewModel(QObject *parent) : BaseTransactionViewModel(parent) {
    setisCreateTx(false);
}

void ConfirmTransactionViewModel::createDraftTransaction(QVariant feeInput) {
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto transactionFlow = qobject_cast<TransactionFlow *>(currentFlow);
    if (transactionFlow) {
        setisCreateTx(false);
        transactionFlow->proceedTransaction(feeInput);
    }
}

void ConfirmTransactionViewModel::createTransaction(QVariant feeInput) {
    emit startLoading();
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto transactionFlow = qobject_cast<TransactionFlow *>(currentFlow);
    if (transactionFlow) {
        setisCreateTx(true);
        transactionFlow->proceedTransaction(feeInput);
    }
}

void ConfirmTransactionViewModel::proceedTransactionResult() {
    emit stopLoading();
    displayTransactionInfo();
    if (isCreateTx()) {
        GUARD_FLOW_MANAGER()
        auto currentFlow = flowMng->currentFlow();
        auto transactionFlow = qobject_cast<TransactionFlow *>(currentFlow);
        auto claimFlow = qobject_cast<ClaimingFlow *>(flowMng->getFlow(transactionFlow->workFlowId()));
        if (claimFlow) {
            if (claimFlow->isManualClaiming()) {
                GUARD_SUB_SCREEN_MANAGER()
                subMng->show(qml::features::claiming::qtransactiondetails);
            } else {
                if (transactionFlow->isAllreadySigned()) {
                    transactionFlow->manualClaimingTransaction(transactionFlow->nunTx()); // auto broadcast if all keys is backup password for off-claim flow
                } else {
                    transactionFlow->autoSignClaimingTransaction();
                }
            }
        }
    }
}

} // namespace features::transactions::viewmodels
