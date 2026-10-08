#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class WithdrawACustomAmountViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, balanceDisplay)
    DEFINE_QT_PROPERTY(QString, balanceCurrency)
    DEFINE_QT_PROPERTY(QString, note)
    DEFINE_QT_PROPERTY(qint64, withdrawAmountSats)
    // Client-side cap so the amount box can block/flag an over-the-limit entry before hitting the
    // backend - CUSTOMIZE distribution caps at available_to_withdraw, otherwise the full balance.
    DEFINE_QT_PROPERTY(qint64, maxWithdrawSats)
  public:
    explicit WithdrawACustomAmountViewModel(QObject *parent = nullptr);
    void forwardAmount();
    void onInit() override;
  public slots:
    void withdrawToWalletClicked();
    void withdrawToAddressClicked();
};
} // namespace features::claiming::viewmodels