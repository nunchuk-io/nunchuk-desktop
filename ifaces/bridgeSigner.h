#pragma once
#include "nunchuckiface.h"
#include <QString>

namespace bridge::signer {

nunchuk::SingleSigner GetSigner(const QString &xfp, const nunchuk::WalletType &wallet_type, const nunchuk::AddressType &address_type,
                                                   const int index, QWarningMessage &msg);

nunchuk::SingleSigner GetSignerFromMasterSigner(const QString &xfp, const nunchuk::WalletType &wallet_type,
                                                                     const nunchuk::AddressType &address_type, const int index, QWarningMessage &msg);

QString GetSignerMasterXprv(const QString &signer_id, QWarningMessage &msg);
bool HasSignerMasterXprv(const QString &signer_id, QWarningMessage &msg);
bool HasSignerMnemonic(const QString& signer_id, QWarningMessage &msg);
bool IsValidSignerPassphrase(const QString& mastersigner_id, const QString& passphrase, QWarningMessage &msg);
QString GetSignerMnemonic(const QString& signer_id, const QString& passphrase, QWarningMessage &msg);

nunchuk::MasterSigner CreateMasterSigner(const QString &name, const nunchuk::Device &device, std::function<bool(int)> progress,
                                                       QWarningMessage &msg);

std::vector<nunchuk::SingleSigner> ParseJSONSigners(const QString &filePathName, nunchuk::SignerType signer_type, QWarningMessage &msg);

std::vector<nunchuk::SingleSigner> ParseQRSigners(const QStringList& qr_data, QWarningMessage &msg);

nunchuk::SingleSigner PickSignerFromList(const std::vector<nunchuk::SingleSigner>& signers, int account_index);
} // namespace bridge::signer