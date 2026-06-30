#include "TransactionDetailsClaimedViewModel.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "core/viewmodel/BaseViewModel.h"
#include "features/transactions/flows/ClaimTransactionFlow.h"
#include "generated_qml_keys.hpp"

namespace features::transactions::viewmodels {
using namespace features::transactions::usecases;
using features::transactions::flows::ClaimTransactionFlow;

TransactionDetailsClaimedViewModel::TransactionDetailsClaimedViewModel(QObject *parent) : BaseTransactionViewModel(parent) {}

void TransactionDetailsClaimedViewModel::onInit() {
    BaseTransactionViewModel::onInit();
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<ClaimTransactionFlow>();
    auto walletId = flow->destinationWalletId();
    DBG_INFO << "TransactionDetailsClaimedViewModel::onInit() destinationWalletId: " << walletId;
    bool hasWallet = bridge::nunchukHasWallet(walletId);
    setisAddressFlow(!hasWallet);
}

void TransactionDetailsClaimedViewModel::verifyAddress(const QString &address) {
    GUARD_CLIENT_CONTROLLER()
    AddressToVerifyInput input;
    input.isLoginRequired = clientCtrl->isNunchukLoggedIn();
    input.address = address;
    input.wallet_id = walletId();
    m_addressToVerifyUC.executeAsync(input, [this](const core::usecase::Result<AddressToVerifyResult> &result) {
        if (result.isSuccess()) {
            QString msg = QString("Address successfully verified");
            emit showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
        } else {
            QString msg = QString("Address verification failed");
            emit showToast(0, msg, EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void TransactionDetailsClaimedViewModel::setMemo(const QString &memo) {
    GUARD_CLIENT_CONTROLLER()
    QString tx_id = transactionInfo()->txid();
    bridge::nunchukUpdateTransactionMemo(walletId(), tx_id, memo);
    GUARD_WALLET(walletId())
    if (wallet->transactionHistory()) {
        wallet->transactionHistory()->updateTransactionMemo(tx_id, memo);
    }
    wallet->UpdateAssistedTxs(tx_id, memo);
    clientCtrl->updateTransactionMemo(walletId(), tx_id, memo);
}

void TransactionDetailsClaimedViewModel::closeToWalletTab() {
    // Focus the wallet that received the claim (destination), not the claim source wallet.
    // destinationWalletId is stored in ClaimTransactionFlow by WithdrawSelectWalletViewModel
    // when the user picks a Nunchuk wallet in the "Withdraw to a Nunchuk wallet" dialog.
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<ClaimTransactionFlow>();
    auto walletId = flow->destinationWalletId();
    if (!walletId.isEmpty()) {
        GUARD_APP_MODEL()
        if (auto walletList = appModel->walletListPtr()) {
            auto walletPtr = walletList->getWalletById(walletId);
            if (!walletPtr.isNull()) {
                appModel->setWalletInfo(walletPtr);
            }
        }
    }
    core::viewmodels::BaseViewModel::closeToWalletTab();
}

} // namespace features::transactions::viewmodels
