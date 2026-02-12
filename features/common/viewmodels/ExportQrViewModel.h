#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::common::viewmodels {
using core::viewmodels::ActionViewModel;
class ExportQrViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit ExportQrViewModel(QObject *parent = nullptr);
    ~ExportQrViewModel() override = default;
};
} // namespace features::common::viewmodels
