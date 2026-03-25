#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "features/claiming/viewmodels/WidthdrawToAddressViewModel.h"

namespace features::claiming::viewmodels {
class WithdrawSelectWalletViewModel : public WidthdrawToAddressViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY_PTR(WalletListModel, walletList)
  public:
    explicit WithdrawSelectWalletViewModel(QObject *parent = nullptr);
    void createWalletForWithdrawSelect();
    void onInit() override;
  public slots:
    void createTransactionToWallet(const QString& wallet_id);
};
} // namespace features::claiming::viewmodels