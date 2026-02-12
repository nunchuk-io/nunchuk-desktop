#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::wallets::viewmodels {
using core::viewmodels::ActionViewModel;

class WalletConfigViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit WalletConfigViewModel(QObject *parent = nullptr);
    ~WalletConfigViewModel() override = default;
  
//   public slots:
//     void asBBQRClicked();
//     void toCOLDCARDClicked();
};
} // namespace features::wallets::viewmodels
