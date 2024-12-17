#ifndef QGROUPWALLETDUMMYTX_H
#define QGROUPWALLETDUMMYTX_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "TypeDefine.h"
#include "Premiums/QWalletDummyTx.h"
#include "Models/TransactionModel.h"

class QGroupWalletDummyTx : public QWalletDummyTx
{
    Q_OBJECT
    Q_PROPERTY(QVariant requester_user                            READ requester_user     NOTIFY transactionInfoChanged)
    Q_PROPERTY(QVariant txJson                                    READ txJson             NOTIFY transactionInfoChanged)
public:
    QGroupWalletDummyTx(WalletId wallet_id);
    void setDummyTxData(QJsonObject data);
    bool requestForceSyncTx(const QString &group_id, const QString &wallet_id, const QString &txid);
    bool requestSignTx(const QString& xfp) final;
    bool requestSignTxViaQR(const QStringList &qrtags) final;
    bool requestSignTxViaFile(const QString &filepath) final;
    void requestUpdateDummyTx(const QMap<QString, QString>& signatures, const QMap<QString, QString> &signers);
    void finishScanDevices();
    QString textForToast(int flow);
    QString tx_id() const;

    QStringList ExportPsbtViaQR(QWarningMessage &msg);
    QStringList ExportPsbtViaBBQR(QWarningMessage &msg);

    QVariant requester_user();

    QVariant txJson() const;
    bool CancelDummyTransaction();

    QString currentXfp() const;
    void setCurrentXfp(const QString &currentXfp);
    QJsonObject tx() const;
signals:
    void registerWalletWithDevice();
    void requestSignout();
private:
    QJsonObject             m_tx {};
};
#endif // QGROUPWALLETDUMMYTX_H
