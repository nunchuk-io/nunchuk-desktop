#pragma once
#include "features/transactions/viewmodels/GeneralTransactionDetailsViewModel.h"

namespace features::transactions::viewmodels {
class NormalTransactionDetailsViewModel : public GeneralTransactionDetailsViewModel {
    Q_OBJECT
  public:
    explicit NormalTransactionDetailsViewModel(QObject *parent = nullptr);
  public slots:
    void setMemo(const QString &memo);
    void txSignRequest(const QString &xfp);
    void txSignImportQr(const QStringList &tags);
    void txSignImportFile(const QString &filePath);
    void txSignExportQr();
    void txSignExportBBQr();
    void txSignExportFile(const QString &filePath);
    void txBroadcast();
};
} // namespace features::transactions::viewmodels