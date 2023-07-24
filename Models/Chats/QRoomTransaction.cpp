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
#include <QQmlEngine>
#include "QRoomTransaction.h"
#include "bridgeifaces.h"

QRoomTransaction::QRoomTransaction()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QRoomTransaction::QRoomTransaction(nunchuk::RoomTransaction tx): roomtx(tx)
{

}

QRoomTransaction::~QRoomTransaction()
{

}

nunchuk::RoomTransaction QRoomTransaction::roomTransaction() const
{
    return roomtx;
}

QString QRoomTransaction::get_tx_id() const
{
    return QString::fromStdString(roomtx.get_tx_id());
}

void QRoomTransaction::set_tx_id(const QString &value)
{
    roomtx.set_tx_id(value.toStdString());
}

QString QRoomTransaction::get_wallet_id() const
{
    return QString::fromStdString(roomtx.get_wallet_id());
}

void QRoomTransaction::set_wallet_id(const QString &value)
{
    roomtx.set_wallet_id(value.toStdString());
}

QString QRoomTransaction::get_init_event_id() const
{
    return QString::fromStdString(roomtx.get_init_event_id());
}

void QRoomTransaction::set_init_event_id(const QString &value)
{
    roomtx.set_init_event_id(value.toStdString());
}

QList<QString> QRoomTransaction::get_sign_event_ids() const
{
    QList<QString> ret;
    ret.clear();
    std::vector<std::string> v = roomtx.get_sign_event_ids();
    foreach (std::string i, v) {
        ret.append(QString::fromStdString(i));
    }
    return ret;
}

void QRoomTransaction::add_sign_event_id(const QString &value)
{
    roomtx.add_sign_event_id(value.toStdString());
}

void QRoomTransaction::set_sign_event_ids(const QList<QString> &value)
{
    std::vector<std::string> v;
    if(v.size() > 0) v.clear();
    foreach (QString i, value) {
        v.push_back(i.toStdString());
    }
    roomtx.set_sign_event_ids(v);
}

QList<QString> QRoomTransaction::get_reject_event_ids() const
{
    QList<QString> ret;
    ret.clear();
    std::vector<std::string> v = roomtx.get_reject_event_ids();
    foreach (std::string i, v) {
        ret.append(QString::fromStdString(i));
    }
    return ret;
}

void QRoomTransaction::add_reject_event_id(const QString &value)
{
    roomtx.add_reject_event_id(value.toStdString());
}

void QRoomTransaction::set_reject_event_ids(const QList<QString> &value)
{
    std::vector<std::string> v;
    if(v.size() > 0) v.clear();
    foreach (QString i, value) {
        v.push_back(i.toStdString());
    }
    roomtx.set_reject_event_ids(v);
}

QString QRoomTransaction::get_broadcast_event_id() const
{
    return QString::fromStdString(roomtx.get_broadcast_event_id());
}

void QRoomTransaction::set_broadcast_event_id(const QString &value)
{
    roomtx.set_broadcast_event_id(value.toStdString());
}

QString QRoomTransaction::get_cancel_event_id() const
{
    return QString::fromStdString(roomtx.get_cancel_event_id());
}

void QRoomTransaction::set_cancel_event_id(const QString &value)
{
    roomtx.set_cancel_event_id(value.toStdString());
}

QString QRoomTransaction::get_ready_event_id() const
{
    return QString::fromStdString(roomtx.get_ready_event_id());
}

void QRoomTransaction::set_ready_event_id(const QString &value)
{
    roomtx.set_ready_event_id(value.toStdString());
}

Transaction *QRoomTransaction::transaction() const
{
    return m_transaction.data();
}

QTransactionPtr QRoomTransaction::transactionPtr() const
{
    return m_transaction;
}

void QRoomTransaction::setTransaction(const QTransactionPtr &transaction)
{
    if(m_transaction != transaction){
        m_transaction = transaction;
        emit transactionChanged();
    }
}

QRoomTransactionModel::QRoomTransactionModel()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qRegisterMetaType<QRoomTransactionModelPtr>();
}

QRoomTransactionModel::~QRoomTransactionModel()
{

}

int QRoomTransactionModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant QRoomTransactionModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case room_tx_txid:
        return m_data[index.row()]->get_tx_id();
    case room_tx_init_id:
        return m_data[index.row()]->get_init_event_id();
    case room_tx_transaction:
        return QVariant::fromValue(m_data[index.row()]->transaction());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QRoomTransactionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[room_tx_txid]        = "room_tx_txid";
    roles[room_tx_init_id]     = "room_tx_init_id";
    roles[room_tx_transaction] = "room_tx_transaction";
    return roles;
}

void QRoomTransactionModel::removeTransaction(const QRoomTransactionPtr &tx)
{
    if(tx){
        beginResetModel();
        foreach (QRoomTransactionPtr it, m_data) {
            if(0 == QString::compare(it.data()->get_tx_id(), tx.data()->get_tx_id(), Qt::CaseInsensitive)){
                m_data.removeAll(it);
            }
        }
        endResetModel();
    }
}

void QRoomTransactionModel::addTransaction(const QRoomTransactionPtr &tx)
{
    if(tx && !contains(tx->get_tx_id())){
        beginResetModel();
        m_data.append(tx);
        endResetModel();
    }
}

int QRoomTransactionModel::transactionIndexById(const QString &txid)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(0 == QString::compare(m_data.at(i).data()->get_tx_id(), txid, Qt::CaseInsensitive)){
            return i;
        }
    }
    return -1;
}

void QRoomTransactionModel::transactionChanged(const QString &tx_id, const int status, const int height)
{
    beginResetModel();
    foreach (QRoomTransactionPtr it, m_data) {
        if(0 == QString::compare(it.data()->get_tx_id(), tx_id, Qt::CaseInsensitive) && it.data()->transaction()){
            if(status == (int)ENUNCHUCK::TransactionStatus::PENDING_SIGNATURES
                    || status == (int)ENUNCHUCK::TransactionStatus::READY_TO_BROADCAST
                    || status == (int)ENUNCHUCK::TransactionStatus::PENDING_CONFIRMATION)
            {
                it.data()->transaction()->setStatus(status);
                it.data()->transaction()->setHeight(height);
            }
            else{
                m_data.removeAll(it);
            }
        }
    }
    endResetModel();
}

bool QRoomTransactionModel::contains(const QString &txid)
{
    foreach (QRoomTransactionPtr it , m_data ){
        if(0 == QString::compare(it.data()->get_tx_id(), txid, Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

void QRoomTransactionModel::refresh()
{
    beginResetModel();
    endResetModel();
}

QString QRoomTransactionModel::transactionInitEventById(const QString &txid)
{
    foreach (QRoomTransactionPtr it , m_data ){
        if(0 == QString::compare(it.data()->get_tx_id(), txid, Qt::CaseInsensitive)){
            return it.data()->get_init_event_id();
        }
    }
    return "";
}

QSharedPointer<QRoomTransactionModel> QRoomTransactionModel::clone() const
{
    QRoomTransactionModelPtr clone = QRoomTransactionModelPtr(new QRoomTransactionModel());
    for (QRoomTransactionPtr tx : m_data) {
        if(tx){
            QRoomTransactionPtr ret = QRoomTransactionPtr(new QRoomTransaction(tx.data()->roomTransaction()));
            QTransactionPtr nunchukTx = tx.data()->transactionPtr();
            if(nunchukTx){
                QTransactionPtr rawtx = QTransactionPtr(new Transaction());
                rawtx.data()->setNunchukTransaction(nunchukTx.data()->nunchukTransaction());
                rawtx.data()->setInitEventId(nunchukTx.data()->initEventId());
                rawtx.data()->setRoomId(nunchukTx.data()->roomId());
                ret.data()->setTransaction(rawtx);
            }
            clone.data()->addTransaction(ret);
        }
    }
    return clone;
}
