#pragma once

#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/claiming/usecases/GetClaimingWalletUseCase.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;
using features::claiming::usecases::GetClaimingWalletUseCase;

class ReleaseScheduleViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariantList, stagesData)
    DEFINE_QT_PROPERTY(int, availablePercentage)
    DEFINE_QT_PROPERTY(int, availableBalanceSats)
    DEFINE_QT_PROPERTY(QString, availableBalanceDisplay)
    DEFINE_QT_PROPERTY(QString, availableBalanceCurrency)
    DEFINE_QT_PROPERTY(int, current_stage_index)
    DEFINE_QT_PROPERTY(int, current_installment_index)

  public:
    explicit ReleaseScheduleViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(bool, isRegistered)

    void onInit() override;
    void requestWithdrawBitcoin();
  public slots:
    void withdrawBitcoinClicked();
  private:
    GetClaimingWalletUseCase m_getClaimingWalletUC;
};
} // namespace features::claiming::viewmodels
