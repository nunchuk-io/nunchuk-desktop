#pragma once
#include "features/signers/viewmodels/AddExistingKeyViewModel.h"

namespace features::signers::viewmodels {
class AddHardwareExistingKeyViewModel : public AddExistingKeyViewModel {
    Q_OBJECT
  public:
    explicit AddHardwareExistingKeyViewModel(QObject *parent = nullptr);
    void initialize();
    void initializeTextGuide();
  protected:
    void onInit() override;
  public slots:
    void onContinueClicked();
    void onTakeMeAddNewKeyClicked();
};
} // namespace features::signers::viewmodels
