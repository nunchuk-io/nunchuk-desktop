#pragma once
#include "core/viewmodel/BaseViewModel.h"

namespace features::rightpanel::viewmodels {
using core::viewmodels::BaseViewModel;
class RightPanelViewModel : public BaseViewModel {
    Q_OBJECT
  public:
    explicit RightPanelViewModel(QObject *parent = nullptr);
    ~RightPanelViewModel();
  public slots:
    void registerObject(QObject *object);
};
} // namespace features::rightpanel::viewmodels
