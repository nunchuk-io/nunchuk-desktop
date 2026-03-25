#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class BufferPeriodHasStartedViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, remaining_display_name)
  public:
    explicit BufferPeriodHasStartedViewModel(QObject *parent = nullptr);

  public slots:
    void next() override;
};
} // namespace features::claiming::viewmodels