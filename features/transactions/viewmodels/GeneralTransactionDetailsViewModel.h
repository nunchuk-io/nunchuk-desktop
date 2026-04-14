#pragma once
#include "features/signers/usecases/ScanDeviceUsecase.h"
#include "features/transactions/usecases/AddressToVerifyUseCase.h"
#include "features/transactions/viewmodels/BaseTransactionViewModel.h"

namespace features::transactions::viewmodels {
using features::signers::usecases::ScanDeviceUsecase;
using features::transactions::usecases::AddressToVerifyUseCase;

class GeneralTransactionDetailsViewModel : public BaseTransactionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QStringList, qrExported)
  public:
    explicit GeneralTransactionDetailsViewModel(QObject *parent = nullptr);

  public slots:
    void verifyAddress(const QString &address);
    void scanDevice();
  signals:
    void finishedSigningTransaction();

  private:
    AddressToVerifyUseCase m_addressToVerifyUC;
    ScanDeviceUsecase m_scanDeviceUC;
};
} // namespace features::transactions::viewmodels
