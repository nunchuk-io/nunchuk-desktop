#pragma once
#include "core/viewmodel/BaseViewModel.h"

namespace features::rightpanel::viewmodels {
using core::viewmodels::BaseViewModel;
class ClaimInheritanceViewModel : public BaseViewModel {
    Q_OBJECT
  public:
    explicit ClaimInheritanceViewModel(QObject *parent = nullptr);

  public:
    void onInit() override;
};
} // features::rightpanel::viewmodels
