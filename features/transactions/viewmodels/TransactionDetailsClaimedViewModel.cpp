#include "TransactionDetailsClaimedViewModel.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::transactions::viewmodels {
using namespace features::transactions::usecases;

TransactionDetailsClaimedViewModel::TransactionDetailsClaimedViewModel(QObject *parent) : BaseTransactionViewModel(parent) {}

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

} // namespace features::transactions::viewmodels
