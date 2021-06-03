#ifndef TRANSACTIONLISTMODEL_H
#define TRANSACTIONLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "SingleSignerModel.h"
#include "QWarningMessage.h"
#include "QOutlog.h"

class Destination : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString address  READ address        NOTIFY addressChanged)
    Q_PROPERTY(QString amount   READ amountDisplay  NOTIFY amountChanged)
public:
    Destination();
    Destination(const QString &address, const qint64 amount);
    ~Destination();
    QString amountDisplay() const;
    QString amountBTC() const;
    qint64 amountSats() const;
    void setAmount(const qint64 value);

    QString address() const;
    void setAddress(const QString & value);
private:
    QString address_;
    qint64 amount_;

signals:
    void addressChanged();
    void amountChanged();
};

class DestinationListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DestinationListModel();
    ~DestinationListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;
    void addDestination(const QString& address, const qint64 amount);
    bool contains(const QString &address, qint64 &amount);
    qint64 getAmountByAddress(const QString &address);
    QMap<QString, qint64> getOutputs() const;
    void notifyUnitChanged();
    enum DestinationRoles {
        destination_address_role,
        destination_amount_role
    };
private:
    QList<QSharedPointer<Destination>> d_;
};

class Transaction : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString txid READ txid NOTIFY txidChanged)
    Q_PROPERTY(int height READ height NOTIFY heightChanged)
    Q_PROPERTY(QString memo READ memo NOTIFY memoChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString fee READ feeDisplay NOTIFY feeChanged)
    Q_PROPERTY(QString feeRate READ feeRate NOTIFY feeRateChanged)
    Q_PROPERTY(int m READ m NOTIFY mChanged)
    Q_PROPERTY(bool hasChange READ hasChange NOTIFY hasChangeChanged)
    Q_PROPERTY(DestinationListModel* destinationList READ destinationList NOTIFY destinationListChanged)
    Q_PROPERTY(Destination* change READ change NOTIFY changeChanged)
    Q_PROPERTY(SingleSignerListModel* singleSignersAssigned READ singleSignersAssigned NOTIFY singleSignerAssignedChanged)
    Q_PROPERTY(QString subtotal READ subtotalDisplay NOTIFY subtotalChanged)
    Q_PROPERTY(QString total READ totalDisplay NOTIFY totalChanged)
    Q_PROPERTY(int numberSigned READ numberSigned NOTIFY numberSignedChanged)
    Q_PROPERTY(QString blocktime READ blocktimeDisplay NOTIFY blocktimeChanged)
    Q_PROPERTY(bool isReceiveTx READ isReceiveTx NOTIFY isReceiveTxChanged)
    Q_PROPERTY(bool subtractFromFeeAmount READ subtractFromFeeAmount NOTIFY subtractFromFeeAmountChanged)
    Q_PROPERTY(QWarningMessage* warningMessage READ warningMessage NOTIFY warningMessageChanged)
    Q_PROPERTY(QString replacedTxid READ replacedTxid NOTIFY replacedTxidChanged)
public:
    Transaction();
    ~Transaction();

    QString txid() const;
    void setTxid(const QString &txid);

    QString memo() const;
    void setMemo(const QString &memo);

    int status() const;
    void setStatus(int status);

    QString feeDisplay() const;
    QString feeBTC() const;
    qint64 feeSats() const;
    void setFee(const qint64 fee);

    int m() const; // number of signatures required
    void setM(int m);

    bool hasChange() const;
    void setHasChange(bool hasChange);

    Destination* change() const;
    QSharedPointer<Destination> changePtr() const;
    void setChange(const QSharedPointer<Destination> &change);

    DestinationListModel* destinationList() const;
    QSharedPointer<DestinationListModel> destinationListPtr() const;
    void setDestinationList(const QSharedPointer<DestinationListModel> &destinationList);

    SingleSignerListModel* singleSignersAssigned() const;
    QSharedPointer<SingleSignerListModel> singleSignersAssignedPtr() const;
    void setSingleSignersAssigned(const QSharedPointer<SingleSignerListModel> &singleSignersAssigned);

    QString subtotalDisplay() const;
    QString subtotalBTC() const;
    qint64 subtotalSats() const;
    void setSubtotal(const qint64 subtotal);

    QString totalDisplay() const;
    QString totalBTC() const;
    qint64 totalSats() const;
    void setTotal(const qint64 total);

    int numberSigned() const;
    void setNumberSigned(int numberSigned);

    QString blocktimeDisplay() const;
    time_t blocktime() const;
    void setBlocktime(const time_t blocktime);

    int height() const;
    void setHeight(int height);

    bool isReceiveTx() const;
    void setIsReceiveTx(bool isReceiveTx);

    bool subtractFromFeeAmount() const;
    void setSubtractFromFeeAmount(bool subtractFromFeeAmount);

    QString feeRate() const;
    void setFeeRate(qint64 feeRate);

    QWarningMessage* warningMessage() const;
    QSharedPointer<QWarningMessage> warningMessagePtr() const;
    void setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage);

    QString replacedTxid() const;
    void setReplacedTxid(const QString &replacedTxid);

private:
    QString txid_;
    QString memo_;
    int status_;
    qint64 fee_;
    int m_; // number of signatures required
    bool hasChange_;
    QSharedPointer<DestinationListModel> destinationList_;
    QSharedPointer<Destination> change_;
    QSharedPointer<SingleSignerListModel> singleSignersAssigned_;
    qint64 subtotal_;
    qint64 total_;
    int numberSigned_;
    time_t blocktime_;
    int height_;
    bool isReceiveTx_;
    bool subtractFromFeeAmount_;
    qint64 feeRate_;
    QSharedPointer<QWarningMessage> warningMessage_;

    QString replacedTxid_;
signals:
    void txidChanged();
    void memoChanged();
    void statusChanged();
    void feeChanged();
    void mChanged();
    void hasChangeChanged();
    void destinationListChanged();
    void changeChanged();
    void singleSignerAssignedChanged();
    void subtotalChanged();
    void totalChanged();
    void numberSignedChanged();
    void isValidChanged();
    void blocktimeChanged();
    void heightChanged();
    void isReceiveTxChanged();
    void subtractFromFeeAmountChanged();
    void feeRateChanged();
    void warningMessageChanged();
    void replacedTxidChanged();
};

class TransactionListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    TransactionListModel();
    ~TransactionListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;
    QSharedPointer<Transaction> getTransactionByIndex(const int index);
    QSharedPointer<Transaction> getTransactionByTxid(const QString& txid);
    TransactionListModel *getTransactionShortList(const int cnt = 30);
    void requestSort(int role, int order);
    void addTransaction(const QSharedPointer<Transaction> &d);
    void updateTransactionStatus(const QString &tx_id, const int status);
    void updateTransactionMemo(const QString &tx_id, const QString &memo);
    void replaceTransaction(const QString &tx_id, const QSharedPointer<Transaction> &value);
    void removeTransaction(const QString &tx_id);
    void notifyUnitChanged();
    void linkingReplacedTransactions();

    enum TransactionRoles {
        transaction_txid_role,
        transaction_memo_role,
        transaction_status_role,
        transaction_fee_role,
        transaction_m_role,
        transaction_hasChange_role,
        transaction_destinationList_role,
        transaction_change_role,
        transaction_singleSignersAssigned_role,
        transaction_subtotal_role,
        transaction_total_role,
        transaction_numberSigned_role,
        transaction_blocktime_role,
        transaction_height_role,
        transaction_isReceiveTx_role,
        transaction_replacedTx_role
    };
private:
    bool contains(const QString &tx_id);

    QList<QSharedPointer<Transaction>> d_;
};

// Sort Block time
bool sortTXsByBlocktimeAscending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2);
bool sortTXsByBlocktimeDescending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2);
bool sortTXsByBlocktimeDescendingSkipZero(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2);

// Sort Amount
bool sortTXsByAmountAscending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2);
bool sortTXsByAmountDescending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2);

// Sort Memo
bool sortTXsByMemoAscending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2);
bool sortTXsByMemoDescending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2);

// Sort Status
bool sortTXsByStatusAscending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2);
bool sortTXsByStatusDescending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2);

#endif // TRANSACTIONLISTMODEL_H
