#pragma once
#include "nunchuckiface.h"
#include <QString>

namespace bridge::wallet {
    QString GetWalletExportData(const QString &wallet_id, nunchuk::ExportFormat format, QWarningMessage &msg);

    QString GetWalletExportData(const nunchuk::Wallet &wallet, nunchuk::ExportFormat format, QWarningMessage &msg);
}