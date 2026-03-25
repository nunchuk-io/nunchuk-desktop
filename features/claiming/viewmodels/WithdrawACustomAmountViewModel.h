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
  public:
    explicit WithdrawACustomAmountViewModel(QObject *parent = nullptr);
    void forwardAmount();
  public slots:
    void withdrawToWalletClicked();
    void withdrawToAddressClicked();
};
} // namespace features::claiming::viewmodels