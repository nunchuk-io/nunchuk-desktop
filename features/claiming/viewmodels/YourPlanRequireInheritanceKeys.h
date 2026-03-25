#pragma once

#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class YourPlanRequireInheritanceKeys : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(int, inheritance_key_count)
  public:
    explicit YourPlanRequireInheritanceKeys(QObject *parent = nullptr);
    ~YourPlanRequireInheritanceKeys() override = default;

  public slots:
    void next() override;
    void onAddFirstKeyClicked();
    void onAddSecondKeyClicked();
};
} // namespace features::claiming::viewmodels