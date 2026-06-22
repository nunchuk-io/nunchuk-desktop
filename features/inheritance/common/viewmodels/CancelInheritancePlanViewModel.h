#pragma once
#include "InheritancePlanViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::common::viewmodels {
using core::viewmodels::BaseViewModel;

class CancelInheritancePlanViewModel : public InheritancePlanViewModel {
    Q_OBJECT
  public:
    explicit CancelInheritancePlanViewModel(QObject *parent = nullptr);

    void onInit() override;

  public slots:
    void confirmCancel();
    void cancelOperation();
};
} // namespace features::inheritance::common::viewmodels
