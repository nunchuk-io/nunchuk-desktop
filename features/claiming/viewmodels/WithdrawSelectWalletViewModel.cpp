#include "WithdrawSelectWalletViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include <QLocale>
#include "features/claiming/flows/ClaimingFlow.h"
#include "features/transactions/flows/ClaimTransactionFlow.h"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using features::claiming::flows::ClaimingFlow;
using features::transactions::flows::ClaimTransactionFlow;

WithdrawSelectWalletViewModel::WithdrawSelectWalletViewModel(QObject *parent)
    : WidthdrawToAddressViewModel(parent) {
    setwalletList(QWalletListModelPtr(new WalletListModel()));
}

void WithdrawSelectWalletViewModel::createWalletForWithdrawSelect() {
    GUARD_SERVICE_SETTING()
    GUARD_APP_MODEL()
    QString walletId = "";
    if (auto wallet = serviceSetting->walletInfoPtr()) {
        walletId = wallet.data()->walletId();
    }
    if (auto walletList = appModel->walletListPtr()) {
        auto walletListModel = QWalletListModelPtr(new WalletListModel());
        for (const auto &w : walletList->fullList()) {
            if (walletId.isEmpty()) {
                walletListModel->addWallet(w);
            } else if (w.data()->walletId() != walletId) {
                walletListModel->addWallet(w);
            }
        }
        setwalletList(walletListModel);
    }
}

void WithdrawSelectWalletViewModel::onInit() {
    createWalletForWithdrawSelect();
}

void WithdrawSelectWalletViewModel::createTransactionToWallet(const QString& wallet_id) {
    QWalletPtr ptr = walletList()->getWalletById(wallet_id);
    QString withdrawAddress;
    if (!ptr.isNull()) {
        QStringList addrs = bridge::nunchukGetUnusedAddresses(wallet_id, false);
        if (addrs.size() > 0) {
            withdrawAddress = addrs.first();
        } else {
            withdrawAddress = bridge::nunchukGenNewAddresses(wallet_id, false);
        }
        DBG_INFO << withdrawAddress;
    }
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto claimingFlow = qobject_cast<ClaimingFlow*>(currentFlow);
    if (claimingFlow) {
        createTransactionToAddress(withdrawAddress);
    }
}

} // namespace features::claiming::viewmodels