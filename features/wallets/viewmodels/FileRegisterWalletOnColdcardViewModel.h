#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::wallets::viewmodels {
using core::viewmodels::ActionViewModel;

class FileRegisterWalletOnColdcardViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit FileRegisterWalletOnColdcardViewModel(QObject *parent = nullptr);
    ~FileRegisterWalletOnColdcardViewModel() override = default;

};
} // namespace features::wallets::viewmodels
