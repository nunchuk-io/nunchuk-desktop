#include "TransactionFlow.h"
#include "core/ui/UiServices.inc"
#include "features/transactions/viewmodels/BaseTransactionViewModel.h"
#include "generated_qml_keys.hpp"

namespace features::transactions::flows {
using features::transactions::viewmodels::BaseTransactionViewModel;

TransactionFlow::TransactionFlow(FlowContext *ctx, QObject *parent) : core::flow::BaseFlow(ctx, parent) {}

bool TransactionFlow::isOffClaim() {
    return qUtils::strCompare(workFlowId(), "OffChainClaimingFlow");
}

bool TransactionFlow::isOnClaim() {
    return qUtils::strCompare(workFlowId(), "OnChainClaimingFlow");
}

bool TransactionFlow::isNormal() {
    return qUtils::strCompare(workFlowId(), "NormalTransactionFlow");
}

void TransactionFlow::bind(QObject *vm) {
    auto realVm = qobject_cast<BaseTransactionViewModel *>(vm);
    if (realVm) {
        realVm->setnunTx(nunTx());
        realVm->setnunWallet(nunWallet());
        realVm->setisClaimTx(isClaimTx());
        realVm->setisOffChain(isOffClaim());
        connect(this, &TransactionFlow::forwardTransaction, realVm, &BaseTransactionViewModel::proceedTransactionResult);
    }
}
} // namespace features::transactions::flows
