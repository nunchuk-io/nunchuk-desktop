#include "bridgeSigner.h"
#include "QOutlog.h"
#include "QWarningMessage.h"
#include "AppSetting.h"
#include "qUtils.h"
namespace bridge::signer {

nunchuk::SingleSigner GetSigner(const QString &xfp, const nunchuk::WalletType &wallet_type, const nunchuk::AddressType &address_type,
                                                   const int index, QWarningMessage &msg) {
    nunchuk::SingleSigner signer = nunchukiface::instance()->GetSigner(xfp.toStdString(), wallet_type, address_type, index, msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        return signer;
    } else {
        return nunchuk::SingleSigner("", "", "", "", {}, "", 0, "");
    }
}

nunchuk::SingleSigner GetSignerFromMasterSigner(const QString &xfp, const nunchuk::WalletType &wallet_type,
                                                             const nunchuk::AddressType &address_type, const int index, QWarningMessage &msg) {
    return nunchukiface::instance()->GetSignerFromMasterSigner(xfp.toStdString(), wallet_type, address_type, index,
                                                               msg);
}

QString GetSignerMasterXprv(const QString &signer_id, QWarningMessage &msg) {
    auto xprv = nunchukiface::instance()->GetSignerMasterXprv(signer_id.toStdString(), msg);
    return QString::fromStdString(xprv);
}

bool HasSignerMasterXprv(const QString &signer_id, QWarningMessage &msg) {
    return nunchukiface::instance()->HasSignerMasterXprv(signer_id.toStdString(), msg);
}

bool HasSignerMnemonic(const QString &signer_id, QWarningMessage &msg) {
    return nunchukiface::instance()->HasSignerMnemonic(signer_id.toStdString(), msg);
}

bool IsValidSignerPassphrase(const QString &mastersigner_id, const QString &passphrase, QWarningMessage &msg) {
    return nunchukiface::instance()->IsValidSignerPassphrase(mastersigner_id.toStdString(),
                                                            passphrase.toStdString(), msg);
}

QString GetSignerMnemonic(const QString &signer_id, const QString &passphrase, QWarningMessage &msg) {
    auto mnemonic = nunchukiface::instance()->GetSignerMnemonic(signer_id.toStdString(),
                                                                passphrase.toStdString(), msg);
    return QString::fromStdString(mnemonic);
}

nunchuk::MasterSigner CreateMasterSigner(const QString &name, const nunchuk::Device &device, std::function<bool(int)> progress,
                                                       QWarningMessage &msg) {
    return nunchukiface::instance()->CreateMasterSigner(name.toStdString(), device, progress, msg);
}

std::vector<nunchuk::SingleSigner> ParseJSONSigners(const QString &filePathName, nunchuk::SignerType signer_type, QWarningMessage &msg) {
    QString json_str = qUtils::ImportDataViaFile(filePathName);    
    return nunchukiface::instance()->ParseJSONSigners(json_str.toStdString(), signer_type, msg);
}

std::vector<nunchuk::SingleSigner> ParseQRSigners(const QStringList& qr_data, QWarningMessage &msg) {
    QStringList in = qr_data;
    in.removeDuplicates();
    if(in.isEmpty()){
        return {};
    }
    std::vector<std::string> qr_strings;
    for (const auto& qr : qr_data) {
        qr_strings.push_back(qr.toStdString());
    }
    return nunchukiface::instance()->ParseQRSigners(qr_strings, msg);
}

nunchuk::SingleSigner PickSignerFromList(const std::vector<nunchuk::SingleSigner>& signers, int account_index) {
    bool foundNetworkMatch = false;
    bool foundPurposeMatch = false;
    bool foundAccountMatch = false;
    nunchuk::SingleSigner signer("","","","", {},"",0,"");
    if(signers.size() > 0) {
        QWarningMessage msg;
        for (auto s : signers) {
            int coinType = qUtils::GetCoinTypeFromPath(QString::fromStdString(s.get_derivation_path()));
            if (coinType == AppSetting::instance()->primaryServer()) {
                foundNetworkMatch = true;
                DBG_INFO << "coinType matched:" << coinType;
                int purpose = qUtils::GetPurposeFromPath(QString::fromStdString(s.get_derivation_path()));
                if (purpose == 48) {
                    foundPurposeMatch = true;
                    DBG_INFO << "purpose matched:" << purpose;
                    int index = qUtils::GetIndexFromPath(QString::fromStdString(s.get_derivation_path()));
                    if (index == account_index) {
                        foundAccountMatch = true;
                        signer = s;
                        DBG_INFO << "account_index matched:" << index;
                        break;
                    } else if (account_index == -1) {
                        // If account_index is -1, return the first matching purpose and network
                        foundAccountMatch = true;
                        signer = s;
                        DBG_INFO << "account_index matched any:" << index;
                        break;
                    }
                }
            }
        }
        if (!foundNetworkMatch) {
            msg.setWarningMessage(-102, "No signer found matching the current network", EWARNING::WarningType::EXCEPTION_MSG);
        }
        else if (!foundPurposeMatch) {
            msg.setWarningMessage(-101, "No signer found with purpose m/48h'", EWARNING::WarningType::EXCEPTION_MSG);
        }
        else if (!foundAccountMatch) {
            msg.setWarningMessage(-103, "No signer found matching the account index", EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return signer;
}

} // namespace bridge::signer