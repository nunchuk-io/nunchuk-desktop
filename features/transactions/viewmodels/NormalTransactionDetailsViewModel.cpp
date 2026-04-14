#include "NormalTransactionDetailsViewModel.h"

namespace features::transactions::viewmodels {

NormalTransactionDetailsViewModel::NormalTransactionDetailsViewModel(QObject *parent) : GeneralTransactionDetailsViewModel(parent) {}

void NormalTransactionDetailsViewModel::setMemo(const QString &memo) {
    Q_UNUSED(memo)
}

void NormalTransactionDetailsViewModel::txSignRequest(const QString &xfp) {
    Q_UNUSED(xfp)
}

void NormalTransactionDetailsViewModel::txSignImportQr(const QStringList &tags) {
    Q_UNUSED(tags)
}

void NormalTransactionDetailsViewModel::txSignImportFile(const QString &filePath) {
    Q_UNUSED(filePath)
}

void NormalTransactionDetailsViewModel::txSignExportQr() {}

void NormalTransactionDetailsViewModel::txSignExportBBQr() {}

void NormalTransactionDetailsViewModel::txSignExportFile(const QString &filePath) {
    Q_UNUSED(filePath)
}

void NormalTransactionDetailsViewModel::txBroadcast() {}

} // namespace features::transactions::viewmodels