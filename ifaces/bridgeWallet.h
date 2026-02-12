#pragma once
#include "nunchuckiface.h"
#include <QString>

namespace bridge::wallet {
    QString GetWalletExportData(const QString &wallet_id, nunchuk::ExportFormat format, QWarningMessage &msg);

    QString GetWalletExportData(const nunchuk::Wallet &wallet, nunchuk::ExportFormat format, QWarningMessage &msg);
    nunchuk::Wallet CreateWallet(
        const std::string& name, int m, int n,
        const std::vector<nunchuk::SingleSigner>& signers, nunchuk::AddressType address_type,
        bool is_escrow, const std::string& description = {},
        bool allow_used_signer = false, const std::string& decoy_pin = {},
        nunchuk::WalletTemplate wallet_template = nunchuk::WalletTemplate::DEFAULT);

    nunchuk::Wallet CreateWallet(
        const std::string& name, int m, int n,
        const std::vector<nunchuk::SingleSigner>& signers, nunchuk::AddressType address_type,
        nunchuk::WalletType wallet_type, const std::string& description = {},
        bool allow_used_signer = false, const std::string& decoy_pin = {},
        nunchuk::WalletTemplate wallet_template = nunchuk::WalletTemplate::DEFAULT);

    nunchuk::Wallet CreateWallet(
        const nunchuk::Wallet& wallet,
        bool allow_used_signer = false,
        const std::string& decoy_pin = {});
}