#pragma once

#include "core/viewmodel/ActionViewModel.h"
#include "features/claiming/usecases/InitializeInheritanceClaimingUseCase.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;
using features::claiming::usecases::InitializeInheritanceClaimingUseCase;

class MagicPhraseAndBackupPasswordViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit MagicPhraseAndBackupPasswordViewModel(QObject *parent = nullptr);
    ~MagicPhraseAndBackupPasswordViewModel() override = default;

  public slots:
    void claimInit(const QString& magicPhrase);
  signals:
    void gotoOnChain(int inheritance_key_count);
    void gotoOffChain();
  private:
    InitializeInheritanceClaimingUseCase initializeInheritanceClaimingUC;
};
} // namespace features::claiming::viewmodels