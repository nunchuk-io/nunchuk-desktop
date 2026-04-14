#pragma once
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"

namespace features::signers::viewmodels {
class AddAKeyViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(bool, isPrimaryKeyFlow)
    DEFINE_QT_PROPERTY(bool, canAddHardwareKey)
    DEFINE_QT_PROPERTY(bool, softwareSignerLimited)
    DEFINE_QT_PROPERTY(bool, platformKeyAvailable)

  public:
    explicit AddAKeyViewModel(QObject *parent = nullptr);

  protected:
    void onInit() override;

  public slots:
    void onAddPlatformKeyClicked();

  private:
    void updateIsPrimaryKeyFlow();
    void updateHardwareKeyAvailability();
    void updateSoftwareSignerLimitStatus();
    void updatePlatformKeyAvailability();
};
} // namespace features::signers::viewmodels
