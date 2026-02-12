#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/common/viewmodels/OnChainTimelockViewModel.h"
#include "features/common/usecases/TimeLockSetupUpdateUseCase.h"
#include "features/common/usecases/TimeLockSetupGetUseCase.h"

namespace features::draftwallets::onchain::viewmodels {
using features::common::viewmodels::OnChainTimelockViewModel;
using features::common::usecases::TimeLockSetupUpdateUseCase;
using features::common::usecases::TimeLockSetupGetUseCase;

class SetupAnOnChainTimelockViewModel : public OnChainTimelockViewModel {
    Q_OBJECT
  public:
    explicit SetupAnOnChainTimelockViewModel(QObject *parent = nullptr);
    ~SetupAnOnChainTimelockViewModel() override = default;

  public slots:
    void onInit() override;
    void onDispose() override;
    void setupTimeLock();
    void close() override;
  private:
    TimeLockSetupUpdateUseCase m_timeLockUpdateUC;
    TimeLockSetupGetUseCase m_timeLockGetUC;
};
} // namespace features::draftwallets::onchain::viewmodels
