/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

#ifndef TRANSACTIONLISTMODEL_H
#define TRANSACTIONLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "SingleSignerModel.h"
#include "QOutlog.h"
#include <nunchuk.h>
#include <QTimer>

class Wallet;

class Destination : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString address          READ address        NOTIFY addressChanged)
    Q_PROPERTY(QString amount           READ amountDisplay  NOTIFY amountChanged)
    Q_PROPERTY(QString amountBTC        READ amountBTC      NOTIFY amountChanged)
    Q_PROPERTY(QString amountCurrency   READ amountCurrency NOTIFY amountChanged)
public:
    Destination();
    Destination(const QString &address, const qint64 amount);
    ~Destination();
    QString amountDisplay() const;
    QString amountBTC() const;
    QString amountCurrency() const;
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
typedef OurSharedPointer<Destination> QDestinationPtr;

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
        destination_amount_currency_role
    };
    QString reciever();
    void clearAll();
private:
    QList<QDestinationPtr> m_data;
};
typedef OurSharedPointer<DestinationListModel> QDestinationListModelPtr;

class Transaction : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString                      txid                    READ txid                   CONSTANT)
    Q_PROPERTY(QString                      memo                    READ memo                   NOTIFY memoChanged)
    Q_PROPERTY(int                          height                  READ height                 NOTIFY heightChanged)
    Q_PROPERTY(int                          status                  READ status                 NOTIFY statusChanged)
    Q_PROPERTY(int                          m                       READ m                      NOTIFY mChanged)
    Q_PROPERTY(bool                         hasChange               READ hasChange              NOTIFY hasChangeChanged)
    Q_PROPERTY(QString                      fee                     READ feeDisplay             NOTIFY feeChanged)
    Q_PROPERTY(QString                      feeBTC                  READ feeBTC                 NOTIFY feeChanged)
    Q_PROPERTY(QString                      feeCurrency             READ feeCurrency            NOTIFY feeChanged)
    Q_PROPERTY(QString                      feeRate                 READ feeRate                NOTIFY feeRateChanged)
    Q_PROPERTY(Destination*                 change                  READ change                 NOTIFY changeChanged)
    Q_PROPERTY(DestinationListModel*        destinationList         READ destinationList        NOTIFY destinationListChanged)
    Q_PROPERTY(SingleSignerListModel*       singleSignersAssigned   READ singleSignersAssigned  NOTIFY singleSignerAssignedChanged)
    Q_PROPERTY(QString                      subtotal                READ subtotalDisplay        NOTIFY subtotalChanged)
    Q_PROPERTY(QString                      subtotalCurrency        READ subtotalCurrency       NOTIFY subtotalChanged)
    Q_PROPERTY(QString                      total                   READ totalDisplay           NOTIFY totalChanged)
    Q_PROPERTY(QString                      totalCurrency           READ totalCurrency          NOTIFY totalChanged)
    Q_PROPERTY(int                          numberSigned            READ numberSigned           NOTIFY numberSignedChanged)
    Q_PROPERTY(QString                      blocktime               READ blocktimeDisplay       NOTIFY blocktimeChanged)
    Q_PROPERTY(bool                         isReceiveTx             READ isReceiveTx            NOTIFY isReceiveTxChanged)
    Q_PROPERTY(bool                         subtractFromFeeAmount   READ subtractFromFeeAmount  NOTIFY subtractFromFeeAmountChanged)
    Q_PROPERTY(QString                      replacedTxid            READ replacedTxid           NOTIFY replacedTxidChanged)
    Q_PROPERTY(QString                      roomId                  READ roomId                 NOTIFY roomIdChanged)
    Q_PROPERTY(QString                      initEventId             READ initEventId            NOTIFY initEventIdChanged)
    Q_PROPERTY(bool                         createByMe              READ createByMe             NOTIFY createByMeChanged)
    Q_PROPERTY(QString                      psbt                    READ psbt                   NOTIFY psbtChanged)
    Q_PROPERTY(QString                      serverKeyMessage        READ serverKeyMessage       NOTIFY serverKeyMessageChanged)
    Q_PROPERTY(QString                      destination             READ destination            NOTIFY destinationListChanged)
public:
    Transaction();
    ~Transaction();

    bool isReceiveTx() const;
    void setIsReceiveTx(bool receive);

    QString txid() const;

    QString memo() const;
    void setMemo(const QString &memo);

    int status() const;
    void setStatus(int status);

    int m() const; // number of signatures required

    int height() const;
    void setHeight(const int value);

    QString feeDisplay() const;
    QString feeBTC() const;
    QString feeCurrency() const;
    qint64  feeSats() const;
    void    setFee(const qint64 fee);

    bool subtractFromFeeAmount() const;

    QString feeRate() const;

    QString psbt() const;

    QString subtotalCurrency() const;
    QString subtotalDisplay() const;
    qint64  subtotalSats() const;
    QString subtotalBTC() const;

    QString totalCurrency() const;
    QString totalDisplay() const;
    qint64  totalSats() const;
    QString totalBTC() const;

    DestinationListModel* destinationList();

    bool hasChange() const;

    Destination* change();

    QString blocktimeDisplay() const;
    time_t  blocktime() const;

    QString walletId() const;
    void setWalletId(const QString &walletId);

    SingleSignerListModel* singleSignersAssigned();
    int numberSigned();

    QString replacedTxid() const;

    nunchuk::Transaction nunchukTransaction() const;
    void setNunchukTransaction(const nunchuk::Transaction &tx);
    QString roomId() const;
    void setRoomId(const QString &roomId);
    QString initEventId() const;
    void setInitEventId(const QString &initEventId);
    bool createByMe() const;
    void setCreateByMe(bool createByMe);
    QString serverKeyMessage() const;
    void setServerKeyMessage(const QJsonObject &data);

    QString destination();
private:
    QDestinationListModelPtr    m_destinations;
    QSingleSignerListModelPtr   m_signers;
    QDestinationPtr             m_change;

    nunchuk::Transaction        m_transaction;
    QString                     m_walletId;
    QString                     m_roomId;
    QString                     m_initEventId;
    bool                        m_createByMe;
    QString                     m_serverKeyMessage;

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
    void psbtChanged();
    void serverKeyMessageChanged();
};
typedef OurSharedPointer<Transaction> QTransactionPtr;

class TransactionListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
//    Q_PROPERTY(TransactionListModel* txsShort READ txsShort NOTIFY txsShortChanged)

public:
    TransactionListModel();
    ~TransactionListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;
    QTransactionPtr getTransactionByIndex(const int index);
    QTransactionPtr getTransactionByTxid(const QString& txid);
    void requestSort(int role, int order);
    void addTransaction(const QTransactionPtr &d);
    void updateTransactionMemo(const QString &tx_id, const QString &memo);
    void updateTransaction(const QString &tx_id, const QTransactionPtr &tx);
    void removeTransaction(const QString &tx_id);
    void notifyUnitChanged();
    void linkingReplacedTransactions();
    void cleardata();
    int  count() const;

    enum TransactionRoles {
        transaction_txid_role,
        transaction_memo_role,
        transaction_status_role,
        transaction_fee_role,
        transaction_m_role,
        transaction_hasChange_role,
        transaction_destinationList_role,
        transaction_destinationDisp_role,
        transaction_change_role,
        transaction_singleSignersAssigned_role,
        transaction_subtotal_role,
        transaction_total_role,
        transaction_numberSigned_role,
        transaction_blocktime_role,
        transaction_height_role,
        transaction_isReceiveTx_role,
        transaction_replacedTx_role,
        transaction_totalCurrency_role,
        transaction_subtotalCurrency_role,
    };

signals:
    void countChanged();

private:
    bool contains(const QString &tx_id);
    QList<QTransactionPtr> m_data;
};
typedef OurSharedPointer<TransactionListModel> QTransactionListModelPtr;

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
