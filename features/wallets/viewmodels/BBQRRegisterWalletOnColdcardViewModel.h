#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::wallets::viewmodels {
using core::viewmodels::ActionViewModel;

class BBQRRegisterWalletOnColdcardViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit BBQRRegisterWalletOnColdcardViewModel(QObject *parent = nullptr);
    ~BBQRRegisterWalletOnColdcardViewModel() override = default;

};
} // namespace features::wallets::viewmodels
