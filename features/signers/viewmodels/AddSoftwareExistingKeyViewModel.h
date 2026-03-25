#pragma once
#include "features/signers/viewmodels/AddExistingKeyViewModel.h"

namespace features::signers::viewmodels {
class AddSoftwareExistingKeyViewModel : public AddExistingKeyViewModel {
    Q_OBJECT
  public:
    explicit AddSoftwareExistingKeyViewModel(QObject *parent = nullptr);
  
  public slots:
    void onContinueClicked();
    void onTakeMeAddNewKeyClicked();
};
} // namespace features::signers::viewmodels
