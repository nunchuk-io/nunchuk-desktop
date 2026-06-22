#pragma once
#include "InheritancePlanViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::common::viewmodels {
using core::viewmodels::BaseViewModel;

class CreateInheritancePlanViewModel : public InheritancePlanViewModel {
    Q_OBJECT
  public:
    explicit CreateInheritancePlanViewModel(QObject *parent = nullptr);

    void onInit() override;

  public slots:
    void createPlan();
    void savePlan();
};
} // namespace features::inheritance::common::viewmodels
