#pragma once
#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::signers::viewmodels {
using core::viewmodels::BaseViewModel;

class BasePlatformKeyPoliciesViewModel : public BaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(bool, isEntryPointAlert)
    DEFINE_QT_PROPERTY(bool, isEntryPointWallet)
    DEFINE_QT_PROPERTY(bool, isEntryPointGroup)
  public:
    explicit BasePlatformKeyPoliciesViewModel(QObject *parent = nullptr);
  public:
    int backToScreen() override;
};
} // namespace features::signers::viewmodels
