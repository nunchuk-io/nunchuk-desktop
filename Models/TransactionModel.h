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
#include "UTXOModel.h"
#include "Transactions/MiniscriptTransaction.h"

class Transaction : public MiniscriptTransaction {
    Q_OBJECT
    
public:
    Transaction();
    Transaction(const nunchuk::Transaction &tx);
    ~Transaction() = default;

};
typedef OurSharedPointer<Transaction> QTransactionPtr;

class TransactionListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int      count   READ count      NOTIFY countChanged)

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
    void updateTransaction(const QString &wallet_id, std::vector<nunchuk::Transaction> txs);
    void removeTransaction(const QString &tx_id);
    void notifyUnitChanged();
    void linkingReplacedTransactions();
    void cleardata();
    int  count() const;
    bool contains(const QString &tx_id);
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
        transaction_isRbf_role,
        transaction_miniScript_role,
        transaction_timelockedUntil_role,
    };

signals:
    void countChanged();

private:
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
