#pragma once
#include "features/transactions/usecases/AddressToVerifyUseCase.h"
#include "features/transactions/viewmodels/BaseTransactionViewModel.h"

namespace features::transactions::viewmodels {
using features::transactions::usecases::AddressToVerifyUseCase;
class TransactionDetailsClaimedViewModel : public BaseTransactionViewModel {
    Q_OBJECT
  public:
    explicit TransactionDetailsClaimedViewModel(QObject *parent = nullptr);

    void onInit() override;
  public slots:
    void verifyAddress(const QString &address);
    void setMemo(const QString &memo);
    void closeToWalletTab(); // overrides BaseViewModel::closeToWalletTab to focus the claim destination wallet

  private:
    AddressToVerifyUseCase m_addressToVerifyUC;
};
} // namespace features::transactions::viewmodels
