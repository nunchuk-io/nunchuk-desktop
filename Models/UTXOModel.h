#ifndef UNSPENTOUTPUTMODEL_H
#define UNSPENTOUTPUTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include "QOutlog.h"

class UTXO  : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString  txid    READ txid NOTIFY txidChanged)
    Q_PROPERTY(int      vout    READ vout NOTIFY voutChanged)
    Q_PROPERTY(QString  address READ address        NOTIFY addressChanged)
    Q_PROPERTY(QString  amount  READ amountDisplay  NOTIFY amountChanged)
    Q_PROPERTY(QString  scriptPublickey  READ scriptPublickey  NOTIFY scriptPublickeyChanged)
    Q_PROPERTY(int      height  READ height         NOTIFY heightChanged)
    Q_PROPERTY(QString  memo    READ memo           NOTIFY memoChanged)
public:
    UTXO(const QString &txid, const int vout, const QString &address, const qint64 amount, const int height, const QString &memo);
    UTXO();
    ~UTXO();

    QString txid() const;
    void setTxid(const QString &txid);

    int vout() const;
    void setVout(int vout);

    QString address() const;
    void setAddress(const QString &address);

    QString amountDisplay() const;
    QString amountBTC() const;
    qint64 amountSats() const;
    void setAmount(const qint64 amount);

    int height() const;
    void setHeight(int height);

    bool selected() const;
    void setSelected(bool selected);

    QString scriptPublickey() const;

    QString memo() const;
    void setMemo(const QString &memo);

private:
    QString txid_;
    int vout_;
    QString address_;
    qint64 amount_;
    int height_;
    QString memo_;
    bool selected_;
signals:
    void txidChanged();
    void voutChanged();
    void addressChanged();
    void amountChanged();
    void heightChanged();
    void selectedChanged();
    void scriptPublickeyChanged();
    void memoChanged();
};

class UTXOListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString  totalAmountSelected  READ amountDisplay  NOTIFY amountChanged)
public:
    UTXOListModel();
    ~UTXOListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int,QByteArray> roleNames() const;
    void addUTXO(const QString& txid,
                 const int vout,
                 const QString& address,
                 const qint64 amount,
                 const int height,
                 const QString &memo);
    QSharedPointer<UTXO> getUTXOByIndex(const int index);
    void updateSelected(const QString &txid, const int vout);
    qint64 getAmount(const QString &txid, const int vout);
    void requestSort(int role, int order);
    void notifyUnitChanged();

    enum UTXORoles {
        utxo_txid_role,
        utxo_vout_role,
        utxo_address_role,
        utxo_amount_role,
        utxo_height_role,
        utxo_selected_role,
        utxo_memo_role
    };

    QString amountDisplay();
    QString amountBTC();
    int amountSats();
private:
    QList<QSharedPointer<UTXO>> d_;
    int totalAmountSelected();
signals:
    void amountChanged();
};

// Sort Amount
bool sortbyAmountAscending(const QSharedPointer<UTXO> &v1, const QSharedPointer<UTXO> &v2);
bool sortbyAmountDescending(const QSharedPointer<UTXO> &v1, const QSharedPointer<UTXO> &v2);

// Sort Address
bool sortbyAddressAscending(const QSharedPointer<UTXO> &v1, const QSharedPointer<UTXO> &v2);
bool sortbyAddressDescending(const QSharedPointer<UTXO> &v1, const QSharedPointer<UTXO> &v2);

// Sort height
bool sortbyHeightAscending(const QSharedPointer<UTXO> &v1, const QSharedPointer<UTXO> &v2);
bool sortbyHeightDescending(const QSharedPointer<UTXO> &v1, const QSharedPointer<UTXO> &v2);

// Sort memo
bool sortbyMemoAscending(const QSharedPointer<UTXO> &v1, const QSharedPointer<UTXO> &v2);
bool sortbyMemoDescending(const QSharedPointer<UTXO> &v1, const QSharedPointer<UTXO> &v2);

#endif // UNSPENTOUTPUTMODEL_H
