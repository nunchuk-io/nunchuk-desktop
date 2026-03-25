#pragma once

#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class RecoverInheritanceKeyViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, key_option)
  public:
    explicit RecoverInheritanceKeyViewModel(QObject *parent = nullptr);

  public slots:
    void next() override;
};
} // namespace features::claiming::viewmodels
