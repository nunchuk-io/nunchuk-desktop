#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class PhaseRolloutWithdrawBitcoinViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, balanceDisplay)
    DEFINE_QT_PROPERTY(QString, balanceCurrency)
    DEFINE_QT_PROPERTY(QString, availableBalanceDisplay)
    DEFINE_QT_PROPERTY(QString, availableBalanceCurrency)
    DEFINE_QT_PROPERTY(qint64, availableBalanceSats)
    DEFINE_QT_PROPERTY(qint64, withdrawAmountSats)
    DEFINE_QT_PROPERTY(qint64, balanceSats)
  public:
    explicit PhaseRolloutWithdrawBitcoinViewModel(QObject *parent = nullptr);
    void forwardAmount();

    void onInit() override;
  public slots:
    void next() override;
    void back() override;
    void customAMountClicked();
    void withdrawToWalletClicked();
    void withdrawToAddressClicked();
    void viewReleaseScheduleClicked();
};
} // namespace features::claiming::viewmodels
