#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class WithdrawBitcoinViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, balanceDisplay)
    DEFINE_QT_PROPERTY(QString, balanceCurrency)
    DEFINE_QT_PROPERTY(QString, note)
  public:
    explicit WithdrawBitcoinViewModel(QObject *parent = nullptr);
    void forwardAmount();
  public slots:
    void next() override;
    void back() override;
    void customAMountClicked();
    void withdrawToWalletClicked();
    void withdrawToAddressClicked();
};
} // namespace features::claiming::viewmodels