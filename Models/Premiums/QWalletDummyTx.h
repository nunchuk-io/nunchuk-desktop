#ifndef QWALLETDUMMYTX_H
#define QWALLETDUMMYTX_H

#include <QJsonObject>
#include <QJsonArray>
#include "TypeDefine.h"
#include "Premiums/QBasePremium.h"
#include "Models/TransactionModel.h"

class QDummyTransaction : public Transaction {
    Q_OBJECT
public:
    QString dummyXfp() const final;
    void setDummyXfp(const QString& xfp);
    bool isDummyTx() const final;
    QStringList hideSignBtns() const final;
    void setHideSignBtns(const QStringList &newHideSignBtns);

private:
    QString     mDummyXfp {};
    QStringList m_hideSignBtns;
};
typedef OurSharedPointer<QDummyTransaction> QDummyTransactionPtr;

class QWalletDummyTx : public QBasePremium
{
    Q_OBJECT
    Q_PROPERTY(QDummyTransaction* transactionInfo     READ transactionInfo    NOTIFY transactionInfoChanged)
    Q_PROPERTY(QString amountLimit                    READ amountLimit        CONSTANT)
    Q_PROPERTY(int pending_signatures                 READ pending_signatures NOTIFY transactionInfoChanged)
public:
    QWalletDummyTx(WalletId wallet_id);
    virtual ~QWalletDummyTx();

    QString amountLimit() const;
    int pending_signatures() const;

protected:
    virtual bool CreateDummyTx(const QString& bodyString);

public:
    QDummyTransactionPtr transactionPtr() const;
    QDummyTransaction* transactionInfo() const;
    void setTransactionInfo(QDummyTransactionPtr tx);
    virtual bool requestSignTx(const QString&) { return false;}
    virtual bool requestSignTxViaQR(const QStringList&) { return false;}
    virtual bool requestSignTxViaFile(const QString&) { return false;}
    void finishScanDevices();
    QString tx_id() const;

    QStringList ExportPsbtViaQR(const QString& request_body, QWarningMessage &msg);
    QStringList ExportPsbtViaBBQR(const QString& request_body, QWarningMessage &msg);
    QString ImportPsbtViaFile(const QString &filepath);
    void ExportPsbtViaFile(const QString &filepath);
    QJsonObject nonceBody() const;
protected:
    QString bodyString() const;
public:
    void init(WalletId wallet_id);
    template<typename T>
    T get();

signals:
    void transactionInfoChanged();
protected:
    QJsonObject             m_nonceBody;
private:
    QDummyTransactionPtr    m_transaction;
    QJsonObject             m_txJson;
    QGroupWalletDummyTxPtr mGroup;
    QUserWalletDummyTxPtr  mUser;
};

#endif // QWALLETDUMMYTX_H
