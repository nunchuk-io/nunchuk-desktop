#pragma once

#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class PrepareInheritanceKeyViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, key_option)
  public:
    explicit PrepareInheritanceKeyViewModel(QObject *parent = nullptr);

  public slots:
    void next() override;
    void startClaimInheritanceHardwareSetup(const QString &tag);
  signals:
    void openHardwareAddKeyPopup();
};
} // namespace features::claiming::viewmodels
