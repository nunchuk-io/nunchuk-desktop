#include "BaseTransactionViewModel.h"
#include "core/bridge/ExternalBridges.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/transactions/flows/TransactionFlow.h"
#include "generated_qml_keys.hpp"

namespace features::transactions::viewmodels {
using features::transactions::flows::TransactionFlow;

BaseTransactionViewModel::BaseTransactionViewModel(QObject *parent) : ActionViewModel(parent) {
    setisClaimTx(false);
}

BaseTransactionViewModel::~BaseTransactionViewModel() {
    m_transactionInfo.clear();
    m_walletInfo.clear();
}

void BaseTransactionViewModel::onInit() {
    setwalletId(QString::fromStdString(nunWallet().get_id()));
    auto walletPtr = bridge::convertWallet(nunWallet());
    if (walletPtr) {
        setwalletInfo(walletPtr);
    }
    GUARD_APP_MODEL()
    if (appModel->walletInfoPtr() && !qUtils::strCompare(appModel->walletInfoPtr()->walletId(), walletId())) {
        appModel->setWalletInfo(walletPtr);
    }
    displayTransactionInfo();
}

void BaseTransactionViewModel::displayTransactionInfo() {
    QTransactionPtr trans = bridge::convertTransaction(nunTx(), walletId());
    if (trans) {
        trans->setIsClaimTx(isClaimTx());
        settransactionInfo(trans);
    }
}

void BaseTransactionViewModel::close() {
    ActionViewModel::close();
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto transactionFlow = qobject_cast<TransactionFlow *>(currentFlow);
    if (transactionFlow) {
        QString workFlowId = transactionFlow->workFlowId();
        flowMng->resumeFlow(workFlowId);
    }
}

} // namespace features::transactions::viewmodels
