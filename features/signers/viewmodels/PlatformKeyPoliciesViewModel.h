#pragma once
#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "BasePlatformKeyPoliciesViewModel.h"

namespace features::signers::viewmodels {
using core::viewmodels::BaseViewModel;

class PlatformKeyPoliciesViewModel : public BasePlatformKeyPoliciesViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, selectedPolicyId)
  public:
    explicit PlatformKeyPoliciesViewModel(QObject *parent = nullptr);
    void onInit() override;

  public slots:
    void onContinueClicked();
};
} // namespace features::signers::viewmodels
