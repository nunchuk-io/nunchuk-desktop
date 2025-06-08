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
}

void WalletIO::requestExportWalletViaQRBCUR2Legacy()
{
    QWarningMessage msgwarning;
    QStringList qrtags = bridge::nunchukExportKeystoneWallet(walletId(), msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
        AppModel::instance()->setQrExported(qrtags);
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