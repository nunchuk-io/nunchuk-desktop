#pragma once

#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class PhaseRolloutGuidelineViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, note)
  public:
    explicit PhaseRolloutGuidelineViewModel(QObject *parent = nullptr);
  public slots:
    void back() override;
    void onContinueClicked();
};
} // namespace features::claiming::viewmodels
