#pragma once
#include "features/signers/usecases/ScanDeviceUsecase.h"
#include "features/transactions/usecases/AddressToVerifyUseCase.h"
#include "features/transactions/usecases/LibBroadcastTxUseCase.h"
#include "features/transactions/usecases/LibSignTransactionUseCase.h"
#include "features/transactions/viewmodels/BaseTransactionViewModel.h"

namespace features::transactions::flows {
class TransactionFlow;
}

namespace features::transactions::viewmodels {
using features::signers::usecases::ScanDeviceUsecase;
using features::transactions::flows::TransactionFlow;
using features::transactions::usecases::AddressToVerifyUseCase;
using features::transactions::usecases::LibBroadcastTxUseCase;
using features::transactions::usecases::LibSignTransactionUseCase;
class TransactionDetailsViewModel : public BaseTransactionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QStringList, qrExported)
  public:
    explicit TransactionDetailsViewModel(QObject *parent = nullptr);
  public slots:
    void verifyAddress(const QString &address);
    void scanDevice();
    void setMemo(const QString &memo);
    void txSignRequest(const QString &xfp);
    void txSignImportQr(const QStringList &tags);
    void txSignImportFile(const QString &filePath);
    void txSignExportQr();
    void txSignExportBBQr();
    void txSignExportFile(const QString &filePath);
    void txBroadcast();

    void proceedTransactionResult() override;

  public:
    void startSigningTransaction(const QDevicePtr &device, bool isSoftware);
    void updateTransactionStatus(const nunchuk::Transaction &tx);
  signals:
    void finishedSigningTransaction();

  private:
    TransactionFlow *currentTransactionFlow();
    void exportQrOffChain();
    void exportQrOnChainOrNormal();
    void importQrOffChain(TransactionFlow *flow, const QStringList &tags);
    void importQrOnChainOrNormal(const QStringList &tags);
    void importFileOffChain(TransactionFlow *flow, const QString &filePath);
    void importFileOnChainOrNormal(const QString &filePath);

    AddressToVerifyUseCase m_addressToVerifyUC;
    ScanDeviceUsecase m_scanDeviceUC;
    LibSignTransactionUseCase m_signTransactionUC;
    LibBroadcastTxUseCase m_broadcastTxUC;
};
} // namespace features::transactions::viewmodels
