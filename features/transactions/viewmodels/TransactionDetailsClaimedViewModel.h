#pragma once
#include "features/transactions/usecases/AddressToVerifyUseCase.h"
#include "features/transactions/viewmodels/BaseTransactionViewModel.h"

namespace features::transactions::viewmodels {
using features::transactions::usecases::AddressToVerifyUseCase;
class TransactionDetailsClaimedViewModel : public BaseTransactionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(bool, isAddressFlow)
  public:
    explicit TransactionDetailsClaimedViewModel(QObject *parent = nullptr);
  public slots:
    void verifyAddress(const QString &address);
    void setMemo(const QString &memo);

  private:
    AddressToVerifyUseCase m_addressToVerifyUC;
};
} // namespace features::transactions::viewmodels
