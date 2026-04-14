#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::wallets::viewmodels {
using core::viewmodels::ActionViewModel;

class WalletConfigViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit WalletConfigViewModel(QObject *parent = nullptr);

  public slots:
    void onPlatformKeyConfigClicked(const QString &walletId);
};
} // namespace features::wallets::viewmodels
