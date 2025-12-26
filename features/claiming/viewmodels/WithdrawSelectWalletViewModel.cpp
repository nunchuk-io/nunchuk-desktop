#include "WithdrawSelectWalletViewModel.h"
#include "AppModel.h"
#include "ServiceSetting.h"

namespace features::claiming::viewmodels {

WithdrawSelectWalletViewModel::WithdrawSelectWalletViewModel(QObject *parent)
    : ActionViewModel(parent), m_walletList(QWalletListModelPtr(new WalletListModel())) {
}

WalletListModel *WithdrawSelectWalletViewModel::walletList() const {
    return m_walletList.data();
}

void WithdrawSelectWalletViewModel::createWalletForWithdrawSelect() {
    if (auto wallet = ctx()->serviceSetting()->walletInfoPtr()) {
        QString walletId = wallet.data()->walletId();
        if (auto walletList = ctx()->appModel()->walletListPtr()) {
            for (const auto &w : walletList->fullList()) {
                if (w.data()->walletId() != walletId) {
                    m_walletList->addWallet(w);
                }
            }
            emit walletListChanged();
        }
    }
}
void WithdrawSelectWalletViewModel::onInit() {
    createWalletForWithdrawSelect();
}

} // namespace features::claiming::viewmodels