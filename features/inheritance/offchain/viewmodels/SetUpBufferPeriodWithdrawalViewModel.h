#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
class SetUpBufferPeriodWithdrawalViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, buffer_apply_on)
  public:
    explicit SetUpBufferPeriodWithdrawalViewModel(QObject *parent = nullptr);

    void onInit() override;

  public slots:
    void onContinueClicked();
};
} // namespace features::inheritance::offchain::viewmodels
