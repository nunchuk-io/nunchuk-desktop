#include "DummyTransactionFlow.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/transactions/viewmodels/GroupDummyTransactionDetailsViewModel.h"
#include "generated_qml_keys.hpp"

namespace features::transactions::flows {
using namespace features::transactions::viewmodels;
using namespace features::transactions::usecases;

DummyTransactionFlow::DummyTransactionFlow(FlowContext *ctx, QObject *parent) : TransactionFlow(ctx, parent) {
    setisClaimTx(false);
    setworkFlowId(id());
}

void DummyTransactionFlow::bind(QObject *vm) {
    auto groupDummyTxDetailsVm = qobject_cast<GroupDummyTransactionDetailsViewModel *>(vm);
    if (groupDummyTxDetailsVm) {
        groupDummyTxDetailsVm->setdummyTx(dummyTx());
    }
}

} // namespace features::transactions::flows