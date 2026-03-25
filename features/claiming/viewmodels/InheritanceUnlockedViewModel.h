#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/claiming/usecases/GetClaimingWalletUseCase.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;
using features::claiming::usecases::GetClaimingWalletUseCase;

class InheritanceUnlockedViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, balanceDisplay)
    DEFINE_QT_PROPERTY(QString, balanceCurrency)
    DEFINE_QT_PROPERTY(QString, note)
    DEFINE_QT_PROPERTY(bool, isOffClaim)
  public:
    explicit InheritanceUnlockedViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(bool, isRegistered)

    void requestWithdrawBitcoin();
  public slots:
    void viewWalletClicked();
    void withdrawBitcoinClicked();

  private:
    GetClaimingWalletUseCase m_getClaimingWalletUC;
};
} // namespace features::claiming::viewmodels