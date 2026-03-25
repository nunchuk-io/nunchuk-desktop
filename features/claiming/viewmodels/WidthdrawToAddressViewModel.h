#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;

class WidthdrawToAddressViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(qint64, withdrawAmountSats)
  public:
    explicit WidthdrawToAddressViewModel(QObject *parent = nullptr);

  public slots:
    void createTransactionToAddress(const QString& withdrawAddress);
    void proceedTransactionResult();
};
} // namespace features::claiming::viewmodels