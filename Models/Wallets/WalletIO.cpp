#include "WalletIO.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "QThreadForwarder.h"


WalletIO::WalletIO(const nunchuk::Wallet &w) :
    BaseWallet{w}
{}

void WalletIO::requestExportWalletViaBSMS(const QString &file)
{
    DBG_INFO << walletId();
    QString file_path = qUtils::QGetFilePath(file);
    bridge::nunchukExportWallet(walletId(), file_path, nunchuk::ExportFormat::BSMS);
    dynamic_cast<CreatingWallet *>(this)->setNeedBackup(false);
}

void WalletIO::requestExportWalletViaQRBCUR2Legacy()
{
    QWarningMessage msgwarning;
    QStringList qrtags = bridge::nunchukExportKeystoneWallet(walletId(), msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
        AppModel::instance()->setQrExported(qrtags);
        dynamic_cast<CreatingWallet *>(this)->setNeedRegistered(false);
    }
    else{
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
    }
}

void WalletIO::requestExportWalletViaQRBCUR2()
{
    QWarningMessage msgwarning;
    QStringList qrtags = bridge::nunchukExportBCR2020010Wallet(walletId(), msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
        AppModel::instance()->setQrExported(qrtags);
        dynamic_cast<CreatingWallet *>(this)->setNeedRegistered(false);
    }
    else{
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
    }
}

void WalletIO::requestExportWalletViaQRBBQRColdcard() {
    QWarningMessage msgwarning;
    QStringList qrtags = qUtils::ExportBBQRWallet(nunchukWallet(), msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
        AppModel::instance()->setQrExported(qrtags);
        dynamic_cast<CreatingWallet *>(this)->setNeedRegistered(false);
    }
    else{
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
    }
}

void WalletIO::requestExportWalletViaDB(const QString &file)
{
    DBG_INFO << walletId();
    QString file_path = qUtils::QGetFilePath(file);
    bridge::nunchukExportWallet(walletId(), file_path, nunchuk::ExportFormat::DB);
}

void WalletIO::requestExportWalletViaDESCRIPTOR(const QString &file)
{
    DBG_INFO << walletId();
    QString file_path = qUtils::QGetFilePath(file);
    bridge::nunchukExportWallet(walletId(), file_path, nunchuk::ExportFormat::DESCRIPTOR);
}

void WalletIO::requestExportWalletViaCOLDCARD(const QString &file)
{
    DBG_INFO << walletId();
    QString file_path = qUtils::QGetFilePath(file);
    bridge::nunchukExportWallet(walletId(), file_path, nunchuk::ExportFormat::COLDCARD);
    dynamic_cast<CreatingWallet *>(this)->setNeedRegistered(false);
}

void WalletIO::requestExportTransactionViaCSV(const QString &file)
{
    DBG_INFO << walletId();
    QString file_path = qUtils::QGetFilePath(file);
    bridge::nunchukExportTransactionHistory(walletId(), file_path, nunchuk::ExportFormat::CSV);
}

void WalletIO::requestExportTransactionViaPDF(const QString &file)
{
    DBG_INFO << walletId();
    QString file_path = qUtils::QGetFilePath(file);
    bridge::nunchukExportTransactionHistory(walletId(), file_path, nunchuk::ExportFormat::CSV);
}

void WalletIO::requestExportUtxoCSV(const QString &file)
{
    DBG_INFO << walletId();
    QString file_path = qUtils::QGetFilePath(file);
    bridge::nunchukExportUnspentOutputs(walletId(), file_path, nunchuk::ExportFormat::CSV);
}

void WalletIO::requestViaDescriptor(const QString &file) {
    DBG_INFO << walletId();
    QString file_path = qUtils::QGetFilePath(file);
    auto descriptor = nunchukWallet().get_descriptor(nunchuk::DescriptorPath::EXTERNAL_ALL);
    if (descriptor.empty()) {
        DBG_ERROR << "Descriptor is empty for wallet" << walletId();
        return;
    }
    qUtils::ExportDataViaFile(file_path, QString::fromStdString(descriptor));
}

bool WalletIO::hasHardwareOrAirgap()
{
    set<nunchuk::SignerType> hardware_signers = {
        nunchuk::SignerType::HARDWARE,
        nunchuk::SignerType::AIRGAP,
        nunchuk::SignerType::NFC,
        nunchuk::SignerType::COLDCARD_NFC,
        nunchuk::SignerType::PORTAL_NFC,
    };

    auto signers = singleSignersAssigned()->fullList();
    for (const auto &signer : signers) {
        auto signerType = static_cast<nunchuk::SignerType>(signer->signerType());
        if (hardware_signers.find(signerType) != hardware_signers.end()) {
            return true;
        }
    }
    return false;
}

bool WalletIO::hasNativeSegwitDerivationPath()
{
    auto signers = singleSignersAssigned()->fullList();
    for (const auto &signer : signers) {
        if (signer->derivationPath().startsWith("m/84h/0h/0h")) {
            return true;
        }
    }
    return false;
}
