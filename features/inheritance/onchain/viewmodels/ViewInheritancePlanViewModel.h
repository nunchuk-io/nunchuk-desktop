#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "features/common/viewmodels/OnChainTimelockViewModel.h"
#include "features/inheritance/onchain/usecases/TimeLockSetupGetUseCase.h"

namespace features::inheritance::onchain::viewmodels {
using features::common::viewmodels::OnChainTimelockViewModel;
using features::inheritance::onchain::usecases::TimeLockSetupGetUseCase;
class ViewInheritancePlanViewModel : public OnChainTimelockViewModel {
    Q_OBJECT
  public:
    explicit ViewInheritancePlanViewModel(QObject *parent = nullptr);
    ~ViewInheritancePlanViewModel() override = default;

    void onInit() override;
    void onDispose() override;
  private:
    TimeLockSetupGetUseCase m_timeLockGetUC;
};
} // namespace features::inheritance::onchain::viewmodels
