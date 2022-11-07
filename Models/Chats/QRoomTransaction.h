#ifndef QROOMTRANSACTION_H
#define QROOMTRANSACTION_H

#include <QObject>
#include <QAbstractListModel>
#include <QSharedPointer>
#include "nunchukmatrix.h"
#include "TransactionModel.h"

class QRoomTransaction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Transaction*  info     READ transaction    NOTIFY transactionChanged)
    Q_PROPERTY(QString  txid          READ get_tx_id      CONSTANT)
    Q_PROPERTY(QString  initEventId   READ get_init_event_id    CONSTANT)
    Q_PROPERTY(QString  cancelEventId READ get_cancel_event_id  CONSTANT)
public:
    QRoomTransaction();
    QRoomTransaction(nunchuk::RoomTransaction tx);
    ~QRoomTransaction();
    nunchuk::RoomTransaction roomTransaction() const;
    QString get_tx_id() const;
    void set_tx_id(const QString& value);
    QString get_wallet_id() const;
    void set_wallet_id(const QString& value);
    QString get_init_event_id() const;
    void set_init_event_id(const QString& value);
    QList<QString> get_sign_event_ids() const;
    void add_sign_event_id(const QString& value);
    void set_sign_event_ids(const QList<QString>& value);
    QList<QString> get_reject_event_ids() const;
    void add_reject_event_id(const QString& value);
    void set_reject_event_ids(const QList<QString>& value);
    QString get_broadcast_event_id() const;
    void set_broadcast_event_id(const QString& value);
    QString get_cancel_event_id() const;
    void set_cancel_event_id(const QString& value);
    QString get_ready_event_id() const;
    void set_ready_event_id(const QString& value);
    Transaction *transaction() const;
    QTransactionPtr transactionPtr() const;
    void setTransaction(const QTransactionPtr &transactionPtr);

private:
    nunchuk::RoomTransaction roomtx;
    QTransactionPtr m_transaction;

signals:
    void transactionChanged();
};
typedef QSharedPointer<QRoomTransaction> QRoomTransactionPtr;

class QRoomTransactionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    QRoomTransactionModel();
    ~QRoomTransactionModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    void removeTransaction(const QRoomTransactionPtr &tx);
    void addTransaction(const QRoomTransactionPtr &tx);
    int transactionIndexById(const QString& txid);
    void transactionChanged(const QString &tx_id, const int status, const int height);
    bool contains(const QString &txid);
    void refresh();
    QString transactionInitEventById(const QString& txid);
    enum UserRoles {
        room_tx_txid,
        room_tx_init_id,
        room_tx_transaction,
    };

    QSharedPointer<QRoomTransactionModel> clone() const;
private:
    QList<QRoomTransactionPtr> m_data;
};
typedef QSharedPointer<QRoomTransactionModel> QRoomTransactionModelPtr;

Q_DECLARE_METATYPE(QRoomTransaction*)
Q_DECLARE_METATYPE(QRoomTransactionModelPtr)

#endif // QROOMTRANSACTION_H
