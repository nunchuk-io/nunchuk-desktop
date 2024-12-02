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
    Q_PROPERTY(QString  status  READ status         NOTIFY statusChanged)
public:
    UTXO(const QString &txid,
         const int vout,
         const QString &address,
         const qint64 amount,
         const int height,
         const QString &memo,
         const int status);
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

    int status() const;
    void setStatus(int status);

private:
    QString txid_;
    int vout_;
    QString address_;
    qint64 amount_;
    int height_;
    QString memo_;
    bool selected_;
    int  status_;
signals:
    void txidChanged();
    void voutChanged();
    void addressChanged();
    void amountChanged();
    void heightChanged();
    void selectedChanged();
    void scriptPublickeyChanged();
    void memoChanged();
    void statusChanged();
};
typedef QSharedPointer<UTXO> QUTXOPtr;

class UTXOListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString  amountDisplay  READ amountDisplay  NOTIFY amountChanged)
    Q_PROPERTY(QString  amountCurrency READ amountCurrency NOTIFY amountChanged)
    Q_PROPERTY(qint64  amountSats         READ amountSats     NOTIFY amountChanged)

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
                 const QString &memo,
                 const int status);
    QUTXOPtr getUTXOByIndex(const int index);
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
        utxo_confirmed_role,
        utxo_memo_role
    };

    QString amountDisplay();
    QString amountBTC();
    qint64 amountSats();
    QString amountCurrency();
private:
    QList<QUTXOPtr> d_;

signals:
    void amountChanged();
};
typedef QSharedPointer<UTXOListModel> QUTXOListModelPtr;

// Sort Amount
bool sortbyAmountAscending(const QUTXOPtr &v1, const QUTXOPtr &v2);
bool sortbyAmountDescending(const QUTXOPtr &v1, const QUTXOPtr &v2);

// Sort Address
bool sortbyAddressAscending(const QUTXOPtr &v1, const QUTXOPtr &v2);
bool sortbyAddressDescending(const QUTXOPtr &v1, const QUTXOPtr &v2);

// Sort height
bool sortbyHeightAscending(const QUTXOPtr &v1, const QUTXOPtr &v2);
bool sortbyHeightDescending(const QUTXOPtr &v1, const QUTXOPtr &v2);

// Sort memo
bool sortbyMemoAscending(const QUTXOPtr &v1, const QUTXOPtr &v2);
bool sortbyMemoDescending(const QUTXOPtr &v1, const QUTXOPtr &v2);

#endif // UNSPENTOUTPUTMODEL_H
