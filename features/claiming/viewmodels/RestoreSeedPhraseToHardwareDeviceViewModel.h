#pragma once

#include "core/viewmodel/ActionViewModel.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class RestoreSeedPhraseToHardwareDeviceViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit RestoreSeedPhraseToHardwareDeviceViewModel(QObject *parent = nullptr);

  public slots:
    void next() override;
    void startClaimInheritanceHardwareSetup(const QString &tag);
  signals:
    void openHardwareAddKeyPopup();
};
} // namespace features::claiming::viewmodels
