#include "bridgeWallet.h"
#include "QWarningMessage.h"

namespace bridge::wallet {
    QString GetWalletExportData(const QString &wallet_id, nunchuk::ExportFormat format, QWarningMessage &msg) {
        std::string ret = nunchukiface::instance()->GetWalletExportData(wallet_id.toStdString(), format, msg);
        return QString::fromStdString(ret);
    }

    QString GetWalletExportData(const nunchuk::Wallet &wallet, nunchuk::ExportFormat format, QWarningMessage &msg) {
        std::string ret = nunchukiface::instance()->GetWalletExportData(wallet, format, msg);
        return QString::fromStdString(ret);
    }
}