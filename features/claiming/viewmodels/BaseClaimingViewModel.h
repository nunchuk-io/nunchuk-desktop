#pragma once

#include "core/viewmodel/ActionViewModel.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class BaseClaimingViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit BaseClaimingViewModel(QObject *parent = nullptr);
    void handleErrorCode(int error_code, const QString &error_msg);
};
} // namespace features::claiming::viewmodels