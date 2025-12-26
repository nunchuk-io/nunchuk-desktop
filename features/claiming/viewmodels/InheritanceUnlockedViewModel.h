#pragma once

#include "core/viewmodel/ActionViewModel.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class InheritanceUnlockedViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit InheritanceUnlockedViewModel(QObject *parent = nullptr);
    ~InheritanceUnlockedViewModel() override = default;

  public slots:
    void withdrawBitcoinClicked(QVariant msg);
  private:
    bool isRegistered {false};
};
} // namespace features::claiming::viewmodels