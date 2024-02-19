#ifndef QUSERWALLETDUMMYTX_H
#define QUSERWALLETDUMMYTX_H

#include <QObject>
#include "TypeDefine.h"
#include "Premiums/QWalletDummyTx.h"

class QUserWalletDummyTx : public QWalletDummyTx
{
    Q_OBJECT
public:
    explicit QUserWalletDummyTx(WalletId wallet_id);
    bool CreateDummyTransaction();
    bool CreateDummyTransaction(const QString& period_id);

    bool requestSignTx(const QString& xfp) final;
    bool requestSignTxViaQR(const QStringList &qrtags) final;
    bool requestSignTxViaFile(const QString &filepath) final;

    QStringList ExportPsbtViaQR(QWarningMessage &msg);
    void requestUpdateDummyTx(const QMap<QString, QString>& signatures);
signals:

private:
    int m_required_signatures {0};
};

#endif // QUSERWALLETDUMMYTX_H
