#ifndef TRANSACTIONLISTMODEL_H
#define TRANSACTIONLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "SingleSignerModel.h"
#include "QOutlog.h"
#include <nunchuk.h>

class Destination : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString address  READ address        NOTIFY addressChanged)
    Q_PROPERTY(QString amount   READ amountDisplay  NOTIFY amountChanged)
    Q_PROPERTY(QString amountBTC   READ amountBTC   NOTIFY amountChanged)
    Q_PROPERTY(QString amountUSD   READ amountUSD   NOTIFY amountChanged)
public:
    Destination();
    Destination(const QString &address, const qint64 amount);
    ~Destination();
    QString amountDisplay() const;
    QString amountBTC() const;
    QString amountUSD() const;
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
typedef QSharedPointer<Destination> QDestinationPtr;

class DestinationListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString reciever READ reciever CONSTANT)
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
        destination_amount_role,
        destination_amount_btc_role,
        destination_amount_usd_role
    };
    QString reciever();
private:
    QList<QDestinationPtr> d_;
};
typedef QSharedPointer<DestinationListModel> QDestinationListModelPtr;

class Transaction : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString txid READ txid NOTIFY txidChanged)
    Q_PROPERTY(int height READ height NOTIFY heightChanged)
    Q_PROPERTY(QString memo READ memo NOTIFY memoChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString fee READ feeDisplay NOTIFY feeChanged)
    Q_PROPERTY(QString feeBTC READ feeBTC NOTIFY feeChanged)
    Q_PROPERTY(QString feeUSD READ feeUSD NOTIFY feeChanged)
    Q_PROPERTY(QString feeRate READ feeRate NOTIFY feeRateChanged)
    Q_PROPERTY(int m READ m NOTIFY mChanged)
    Q_PROPERTY(bool hasChange READ hasChange NOTIFY hasChangeChanged)
    Q_PROPERTY(DestinationListModel* destinationList READ destinationList NOTIFY destinationListChanged)
    Q_PROPERTY(Destination* change READ change NOTIFY changeChanged)
    Q_PROPERTY(SingleSignerListModel* singleSignersAssigned READ singleSignersAssigned NOTIFY singleSignerAssignedChanged)
    Q_PROPERTY(QString subtotal READ subtotalDisplay NOTIFY subtotalChanged)
    Q_PROPERTY(QString total READ totalDisplay NOTIFY totalChanged)
    Q_PROPERTY(QString totalBTC READ totalBTC NOTIFY totalChanged)
    Q_PROPERTY(QString totalUSD READ totalUSD NOTIFY totalChanged)
    Q_PROPERTY(int numberSigned READ numberSigned NOTIFY numberSignedChanged)
    Q_PROPERTY(QString blocktime READ blocktimeDisplay NOTIFY blocktimeChanged)
    Q_PROPERTY(bool isReceiveTx READ isReceiveTx NOTIFY isReceiveTxChanged)
    Q_PROPERTY(bool subtractFromFeeAmount READ subtractFromFeeAmount NOTIFY subtractFromFeeAmountChanged)
    Q_PROPERTY(QString replacedTxid READ replacedTxid NOTIFY replacedTxidChanged)
    Q_PROPERTY(QString roomId READ roomId NOTIFY roomIdChanged)
    Q_PROPERTY(QString initEventId READ initEventId NOTIFY initEventIdChanged)
    Q_PROPERTY(bool createByMe READ createByMe NOTIFY createByMeChanged)
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
    QString feeUSD() const;
    qint64 feeSats() const;
    void setFee(const qint64 fee);
    int m() const; // number of signatures required
    void setM(int m);
    bool hasChange() const;
    void setHasChange(bool hasChange);
    Destination* change() const;
    QDestinationPtr changePtr() const;
    void setChange(const QDestinationPtr &change);
    DestinationListModel* destinationList() const;
    QDestinationListModelPtr destinationListPtr() const;
    void setDestinationList(const QDestinationListModelPtr &destinationList);
    SingleSignerListModel* singleSignersAssigned() const;
    QSingleSignerListModelPtr singleSignersAssignedPtr() const;
    void setSingleSignersAssigned(const QSingleSignerListModelPtr &singleSignersAssigned);
    QString subtotalDisplay() const;
    QString subtotalBTC() const;
    qint64 subtotalSats() const;
    void setSubtotal(const qint64 subtotal);
    QString totalDisplay() const;
    QString totalBTC() const;
    QString totalUSD() const;
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
    QString replacedTxid() const;
    void setReplacedTxid(const QString &replacedTxid);
    nunchuk::Transaction nunchukTransaction() const;
    void setNunchukTransaction(const nunchuk::Transaction &tx);
    QString roomId() const;
    void setRoomId(const QString &roomId);
    QString initEventId() const;
    void setInitEventId(const QString &initEventId);
    bool createByMe() const;
    void setCreateByMe(bool createByMe);
    QString walletId() const;
    void setWalletId(const QString &walletId);

private:
    QString txid_;
    QString memo_;
    int status_;
    qint64 fee_;
    int m_; // number of signatures required
    bool hasChange_;
    QDestinationListModelPtr destinationList_;
    QDestinationPtr change_;
    QSingleSignerListModelPtr singleSignersAssigned_;
    qint64 subtotal_;
    qint64 total_;
    int numberSigned_;
    time_t blocktime_;
    int height_;
    bool isReceiveTx_;
    bool subtractFromFeeAmount_;
    qint64 feeRate_;
    QString replacedTxid_;
    //ONLINE
    nunchuk::Transaction transaction_;
    QString walletId_;
    QString roomId_;
    QString initEventId_;
    bool createByMe_;
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
    void replacedTxidChanged();
    void roomIdChanged();
    void initEventIdChanged();
    void createByMeChanged();
    void walletIdChanged();
};
typedef QSharedPointer<Transaction> QTransactionPtr;

class TransactionListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    TransactionListModel();
    ~TransactionListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;
    QTransactionPtr getTransactionByIndex(const int index);
    QTransactionPtr getTransactionByTxid(const QString& txid);
    TransactionListModel *getTransactionShortList(const int cnt = 30);
    void requestSort(int role, int order);
    void addTransaction(const QTransactionPtr &d);
    void updateTransactionStatus(const QString &tx_id, const int status);
    void updateTransactionMemo(const QString &tx_id, const QString &memo);
    void replaceTransaction(const QString &tx_id, const QTransactionPtr &value);
    void removeTransaction(const QString &tx_id);
    void notifyUnitChanged();
    void linkingReplacedTransactions();
    void cleardata();
    int count() const;

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
        transaction_replacedTx_role,
        transaction_totalUSD_role,
        transaction_totalBTC_role,
    };
signals:
    void countChanged();
private:
    bool contains(const QString &tx_id);
    QList<QTransactionPtr> d_;
};
typedef QSharedPointer<TransactionListModel> QTransactionListModelPtr;

// Sort Block time
bool sortTXsByBlocktimeAscending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByBlocktimeDescending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByBlocktimeDescendingSkipZero(const QTransactionPtr &v1, const QTransactionPtr &v2);

// Sort Amount
bool sortTXsByAmountAscending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByAmountDescending(const QTransactionPtr &v1, const QTransactionPtr &v2);

// Sort Memo
bool sortTXsByMemoAscending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByMemoDescending(const QTransactionPtr &v1, const QTransactionPtr &v2);

// Sort Status
bool sortTXsByStatusAscending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByStatusDescending(const QTransactionPtr &v1, const QTransactionPtr &v2);

#endif // TRANSACTIONLISTMODEL_H
