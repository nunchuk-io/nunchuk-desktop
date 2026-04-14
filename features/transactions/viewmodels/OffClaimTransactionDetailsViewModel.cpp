#include "OffClaimTransactionDetailsViewModel.h"

namespace features::transactions::viewmodels {

OffClaimTransactionDetailsViewModel::OffClaimTransactionDetailsViewModel(QObject *parent) : GeneralTransactionDetailsViewModel(parent) {}

void OffClaimTransactionDetailsViewModel::setMemo(const QString &memo) {
    Q_UNUSED(memo)
}

void OffClaimTransactionDetailsViewModel::txSignRequest(const QString &xfp) {
    Q_UNUSED(xfp)
}

void OffClaimTransactionDetailsViewModel::txSignImportQr(const QStringList &tags) {
    Q_UNUSED(tags)
}

void OffClaimTransactionDetailsViewModel::txSignImportFile(const QString &filePath) {
    Q_UNUSED(filePath)
}

void OffClaimTransactionDetailsViewModel::txSignExportQr() {}

void OffClaimTransactionDetailsViewModel::txSignExportBBQr() {}

void OffClaimTransactionDetailsViewModel::txSignExportFile(const QString &filePath) {
    Q_UNUSED(filePath)
}

void OffClaimTransactionDetailsViewModel::txBroadcast() {}

} // namespace features::transactions::viewmodels