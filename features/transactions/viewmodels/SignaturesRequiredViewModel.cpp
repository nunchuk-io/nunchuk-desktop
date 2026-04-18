#include "SignaturesRequiredViewModel.h"

#include "core/ui/UiServices.inc"
#include "features/signers/flows/SetupPlatformKeyPolicyFlow.h"
#include "features/transactions/flows/DummyTransactionFlow.h"
#include "generated_qml_keys.hpp"

namespace features::transactions::viewmodels {
using features::signers::flows::SetupPlatformKeyPolicyFlow;
using features::transactions::flows::DummyTransactionFlow;

SignaturesRequiredViewModel::SignaturesRequiredViewModel(QObject *parent) : ActionViewModel(parent) {
    setamountLimit(qUtils::currencyLocale(10000));
    setpending_signatures(0);
}

void SignaturesRequiredViewModel::doThisLater() {
    GUARD_FLOW_MANAGER();
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot sign dummy transaction.";
        return;
    }
    flow->RequestGroupPlatformKeyPolicyUpdateLater();
}

void SignaturesRequiredViewModel::signDummyTransaction() {
    GUARD_FLOW_MANAGER();
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot sign dummy transaction.";
        return;
    }
    flow->RequestGroupPlatformKeyPolicyUpdate();
}

void SignaturesRequiredViewModel::slotStartDummyTransaction() {
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot start dummy transaction.";
        return;
    }
    int pending_signatures = flow->pendingSignatures();
    auto dummyTx = flow->dummyTx();
    auto dummyFlow = flowMng->startFlow<DummyTransactionFlow>();
    if (dummyFlow) {
        dummyFlow->setpendingSignatures(pending_signatures);
        dummyFlow->setdummyTx(dummyTx);
        GUARD_SUB_SCREEN_MANAGER()
        subMng->show(qml::features::transactions::qgroupdummytransactiondetails);
    }
}

} // namespace features::transactions::viewmodels
