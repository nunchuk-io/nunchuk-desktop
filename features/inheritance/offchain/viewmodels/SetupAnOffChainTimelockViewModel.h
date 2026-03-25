#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "features/common/viewmodels/OffChainTimelockViewModel.h"

namespace features::inheritance::offchain::viewmodels {
using features::common::viewmodels::OffChainTimelockViewModel;
class SetupAnOffChainTimelockViewModel : public OffChainTimelockViewModel {
    Q_OBJECT
  public:
    explicit SetupAnOffChainTimelockViewModel(QObject *parent = nullptr);

    void onInit() override;
  public slots:
    void timeLockContinueClicked();
    private:
};
} // namespace features::inheritance::offchain::viewmodels
