#include "OnClaimTransactionDetailsViewModel.h"

namespace features::transactions::viewmodels {

OnClaimTransactionDetailsViewModel::OnClaimTransactionDetailsViewModel(QObject *parent) : GeneralTransactionDetailsViewModel(parent) {}

void OnClaimTransactionDetailsViewModel::setMemo(const QString &memo) {
    Q_UNUSED(memo)
}

void OnClaimTransactionDetailsViewModel::txSignRequest(const QString &xfp) {
    Q_UNUSED(xfp)
}

void OnClaimTransactionDetailsViewModel::txSignImportQr(const QStringList &tags) {
    Q_UNUSED(tags)
}

void OnClaimTransactionDetailsViewModel::txSignImportFile(const QString &filePath) {
    Q_UNUSED(filePath)
}

void OnClaimTransactionDetailsViewModel::txSignExportQr() {}

void OnClaimTransactionDetailsViewModel::txSignExportBBQr() {}

void OnClaimTransactionDetailsViewModel::txSignExportFile(const QString &filePath) {
    Q_UNUSED(filePath)
}

void OnClaimTransactionDetailsViewModel::txBroadcast() {}

} // namespace features::transactions::viewmodels