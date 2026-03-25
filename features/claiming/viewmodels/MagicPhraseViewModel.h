#pragma once

#include "features/claiming/viewmodels/BaseClaimingViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/claiming/usecases/InitializeInheritanceClaimingUseCase.h"

namespace features::claiming::viewmodels {
using features::claiming::viewmodels::BaseClaimingViewModel;
using features::claiming::usecases::InitializeInheritanceClaimingUseCase;

class MagicPhraseViewModel : public BaseClaimingViewModel {
    Q_OBJECT
  public:
    explicit MagicPhraseViewModel(QObject *parent = nullptr);
  public:
    void startOffChainClaimingFlow(const QString& magicPhrase, const QJsonArray& keyOrigins, int inheritance_key_count);
  public slots:
    void claimInit(const QString& magicPhrase);
  signals:
    void gotoOnChain(int inheritance_key_count);
  private:    
    InitializeInheritanceClaimingUseCase initializeInheritanceClaimingUC;
};
} // namespace features::claiming::viewmodels