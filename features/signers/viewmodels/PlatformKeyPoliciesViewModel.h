#pragma once
#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::signers::viewmodels {
using core::viewmodels::BaseViewModel;

class PlatformKeyPoliciesViewModel : public BaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, selectedPolicyId)
    DEFINE_QT_PROPERTY(bool, isWallet)
    DEFINE_QT_PROPERTY(bool, isDummyTx)
  public:
    explicit PlatformKeyPoliciesViewModel(QObject *parent = nullptr);
      void onInit() override;

  public slots:
    void onContinueClicked();
  
  public:
    int backToScreen() override;
};
} // namespace features::signers::viewmodels
