#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/ActionViewModel.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;
class WithdrawSelectWalletViewModel : public ActionViewModel {
    Q_OBJECT
    Q_PROPERTY(WalletListModel *walletList READ walletList NOTIFY walletListChanged)
  public:
    explicit WithdrawSelectWalletViewModel(QObject *parent = nullptr);
    ~WithdrawSelectWalletViewModel() override = default;
    WalletListModel *walletList() const;
    void createWalletForWithdrawSelect();
    void onInit() override;
  signals:
    void walletListChanged();

  private:
    QWalletListModelPtr m_walletList;
};
} // namespace features::claiming::viewmodels