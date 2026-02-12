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

    nunchuk::Wallet CreateWallet(const std::string &name, int m, int n, const std::vector<nunchuk::SingleSigner> &signers, nunchuk::AddressType address_type,
                                 bool is_escrow, const std::string &description, bool allow_used_signer, const std::string &decoy_pin,
                                 nunchuk::WalletTemplate wallet_template) {
        QWarningMessage msg;
        return nunchukiface::instance()->CreateWallet(name, m, n, signers, address_type, is_escrow, description, msg);
    }

    nunchuk::Wallet CreateWallet(const std::string &name, int m, int n, const std::vector<nunchuk::SingleSigner> &signers, nunchuk::AddressType address_type,
                                 nunchuk::WalletType wallet_type, const std::string &description, bool allow_used_signer, const std::string &decoy_pin,
                                 nunchuk::WalletTemplate wallet_template) {
        QWarningMessage msg;
        return nunchukiface::instance()->CreateWallet(name, m, n, signers, address_type, wallet_type, description, allow_used_signer, decoy_pin,
                                                      wallet_template, msg);
    }

    nunchuk::Wallet CreateWallet(const nunchuk::Wallet &wallet, bool allow_used_signer, const std::string &decoy_pin) {
        QWarningMessage msg;
        return nunchukiface::instance()->CreateWallet(wallet, allow_used_signer, decoy_pin, msg);
    }
}