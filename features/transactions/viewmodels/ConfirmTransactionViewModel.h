#pragma once
#include "features/transactions/viewmodels/BaseTransactionViewModel.h"

namespace features::transactions::viewmodels {
class ConfirmTransactionViewModel : public BaseTransactionViewModel {
    Q_OBJECT
  public:
    explicit ConfirmTransactionViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(bool, isCreateTx)
  public slots:
    void createDraftTransaction(QVariant feeInput);
    void createTransaction(QVariant feeInput);
    void proceedTransactionResult() override;
  signals:
    void startLoading();
    void stopLoading();
};
} // namespace features::transactions::viewmodels
