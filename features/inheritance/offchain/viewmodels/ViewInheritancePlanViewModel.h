#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "features/common/viewmodels/OffChainTimelockViewModel.h"
#include "features/inheritance/usecases/GetInheritancePlanUseCase.h"

namespace features::inheritance::offchain::viewmodels {
using features::common::viewmodels::OffChainTimelockViewModel;
using features::inheritance::usecases::GetInheritancePlanUseCase;
class ViewInheritancePlanViewModel : public OffChainTimelockViewModel {
    Q_OBJECT
  public:
    explicit ViewInheritancePlanViewModel(QObject *parent = nullptr);

    void onInit() override;
  public slots:
    void timeLockEditClicked();
  private:
    GetInheritancePlanUseCase m_getInheritancePlanUC;
};
} // namespace features::inheritance::offchain::viewmodels
