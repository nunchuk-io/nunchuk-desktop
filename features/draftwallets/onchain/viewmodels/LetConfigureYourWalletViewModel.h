#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "features/common/viewmodels/OnChainTimelockViewModel.h"
#include "features/common/usecases/TimeLockSetupGetUseCase.h"

namespace features::draftwallets::onchain::viewmodels {
using features::common::viewmodels::OnChainTimelockViewModel;
using features::common::usecases::TimeLockSetupGetUseCase;
class LetConfigureYourWalletViewModel : public OnChainTimelockViewModel {
    Q_OBJECT
  public:
    explicit LetConfigureYourWalletViewModel(QObject *parent = nullptr);
    ~LetConfigureYourWalletViewModel() override = default;

    void onInit() override;
    void onDispose() override;
  public slots:
    void onConfigureClicked();
  private:
    TimeLockSetupGetUseCase m_timeLockGetUC;
};
} // namespace features::draftwallets::onchain::viewmodels
