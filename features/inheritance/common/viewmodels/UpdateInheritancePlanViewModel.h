#pragma once
#include "InheritancePlanViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::common::viewmodels {
using core::viewmodels::BaseViewModel;

class UpdateInheritancePlanViewModel : public InheritancePlanViewModel {
    Q_OBJECT
   
  public:
    explicit UpdateInheritancePlanViewModel(QObject *parent = nullptr);

    void onInit() override;
    void phaseRolloutRefresh();

  public slots:

};
} // namespace features::inheritance::common::viewmodels
