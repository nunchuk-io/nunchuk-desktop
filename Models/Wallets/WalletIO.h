#ifndef WALLETIO_H
#define WALLETIO_H

#include <QObject>
#include "BaseWallet.h"

class WalletIO : public BaseWallet
{
    Q_OBJECT

public:
    WalletIO(const nunchuk::Wallet &w);
    ~WalletIO() override = default;

  
public slots:
    void requestExportWalletViaBSMS(const QString &file);
    void requestExportWalletViaQRBCUR2Legacy();
    void requestExportWalletViaQRBCUR2();
    void requestExportWalletViaQRBBQRColdcard();
    void requestExportWalletViaDB(const QString &file);
    void requestExportWalletViaDESCRIPTOR(const QString &file);
    void requestExportWalletViaCOLDCARD(const QString &file);

    void requestExportTransactionViaCSV(const QString &file);
    void requestExportTransactionViaPDF(const QString &file);

    void requestExportUtxoCSV(const QString &file);
signals:
private:
};

#endif // WALLETIO_H