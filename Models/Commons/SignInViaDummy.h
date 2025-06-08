#ifndef SIGNINVIADUMMY_H
#define SIGNINVIADUMMY_H
#include <QString>
#include "TypeDefine.h"
#include <nunchuk.h>
#include <QJsonObject>

class SignInViaDummy
{
public:
    bool SignInUsingXPUBorWallet(const QString &bsms);
    void initWallet();
    void initializeWalletData(const QString& walletId);
    void convertWallet();
    QGroupDashboardPtr dashboard() const;
    bool isSignIn() const;
    QJsonObject getSigner(const QString& xfp);
    void SignInCreateDummyTransaction();
    bool SignInRequestSignTx(const QString &xfp);
    bool SignInRequestSignTxViaQR(const QStringList &qrtags);
    bool SignInRequestSignTxViaFile(const QString &filepath);

    QStringList ExportPsbtViaQR();
    QStringList ExportPsbtViaBBQR();

private:
    void SignInRequestUpdateDummyTx(const QMap<QString, QString>& signatures);
private:
    nunchuk::Wallet m_wallet = {};
    QJsonObject m_dummy_SignIn = {};
    nunchuk::Transaction tx = {};
    QMap<QString, QString> m_card_ids;
};

#endif // SIGNINVIADUMMY_H
