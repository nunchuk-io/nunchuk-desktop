#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/common/viewmodels/OnChainTimelockViewModel.h"
#include "features/common/usecases/TimeLockReplacementGetUseCase.h"

namespace features::wallets::viewmodels {
using features::common::viewmodels::OnChainTimelockViewModel;
using features::common::usecases::TimeLockReplacementGetUseCase;

class ReplaceKeyOrChangeTimelockViewModel : public OnChainTimelockViewModel {
    Q_OBJECT
  public:
    explicit ReplaceKeyOrChangeTimelockViewModel(QObject *parent = nullptr);
    ~ReplaceKeyOrChangeTimelockViewModel() override = default;

    void onInit() override;
    void onDispose() override;
  public slots:
    void onConfigureClicked();
  private:
    TimeLockReplacementGetUseCase m_timeLockGetUC;
};
} // namespace features::wallets::viewmodels
