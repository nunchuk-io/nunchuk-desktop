#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/common/viewmodels/OnChainTimelockViewModel.h"
#include "features/common/usecases/TimeLockReplacementGetUseCase.h"
#include "features/common/usecases/TimeLockReplacementUpdateUseCase.h"

namespace features::wallets::viewmodels {
using features::common::viewmodels::OnChainTimelockViewModel;
using features::common::usecases::TimeLockReplacementGetUseCase;
using features::common::usecases::TimeLockReplacementUpdateUseCase;

class ReplaceChangeOnChainTimeLockViewModel : public OnChainTimelockViewModel {
    Q_OBJECT
  public:
    explicit ReplaceChangeOnChainTimeLockViewModel(QObject *parent = nullptr);
    ~ReplaceChangeOnChainTimeLockViewModel() override = default;

  public slots:
    void onInit() override;
    void onDispose() override;
    void setupTimeLock();
  private:
    TimeLockReplacementUpdateUseCase m_timeLockUpdateUC;
    TimeLockReplacementGetUseCase m_timeLockGetUC;
};
} // namespace features::wallets::viewmodels
