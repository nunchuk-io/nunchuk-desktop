#pragma once

#include "core/viewmodel/ActionViewModel.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class ExportCompletedViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit ExportCompletedViewModel(QObject *parent = nullptr);

  public slots:
    void next() override;
};
} // namespace features::claiming::viewmodels