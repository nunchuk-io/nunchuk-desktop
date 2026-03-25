#pragma once

#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class ProceedOptionsViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(bool, isBackupPasswordUsed)
  public:
    explicit ProceedOptionsViewModel(QObject *parent = nullptr);
  public slots:
    void confirmProceed();
};
} // namespace features::claiming::viewmodels