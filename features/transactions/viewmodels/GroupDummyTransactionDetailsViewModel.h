#pragma once
#include "features/transactions/usecases/CancelGroupDummyTransactionUseCase.h"
#include "features/transactions/usecases/SignGroupDummyTransactionUseCase.h"
#include "features/transactions/viewmodels/GeneralTransactionDetailsViewModel.h"

namespace features::transactions::viewmodels {
using features::transactions::usecases::CancelGroupDummyTransactionUseCase;
using features::transactions::usecases::SignGroupDummyTransactionUseCase;

class GroupDummyTransactionDetailsViewModel : public GeneralTransactionDetailsViewModel {
    Q_OBJECT
  public:
    explicit GroupDummyTransactionDetailsViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(nunchuk::GroupDummyTransaction, dummyTx)
    void onInit() override;
  public slots:
    void setMemo(const QString &memo);
    void txSignRequest(const QString &xfp);
    void txSignImportQr(const QStringList &tags);
    void txSignImportFile(const QString &filePath);
    void txSignExportQr();
    void txSignExportBBQr();
    void txSignExportFile(const QString &filePath);
    void txBroadcast();

    void requestUpdateDummyTx(const QMap<QString, QString> &signatures);
    void cancelDummyTransaction();

  private:
    void setDummyTxData(const nunchuk::GroupDummyTransaction &dummyTx);
    void setJsonTxData(const nunchuk::GroupDummyTransaction &dummyTx);
    void signDeviceRequest(const QString &xfp);

  private:
    CancelGroupDummyTransactionUseCase m_cancelGroupDummyTransactionUC;
    SignGroupDummyTransactionUseCase m_signGroupDummyTransactionUC;
};
} // namespace features::transactions::viewmodels
