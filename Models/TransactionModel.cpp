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
#include "TransactionModel.h"
#include "AppModel.h"
#include "AppSetting.h"
#include "Chats/matrixbrigde.h"
#include "Premiums/QGroupDashboard.h"
#include "QOutlog.h"
#include "QtGui/qclipboard.h"
#include "Servers/Byzantine.h"
#include "bridgeifaces.h"
#include "qUtils.h"
#include <QQmlEngine>
#include <nunchukmatrix.h>

Transaction::Transaction() : MiniscriptTransaction() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Transaction::Transaction(const nunchuk::Transaction &tx) : MiniscriptTransaction(tx) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

TransactionListModel::TransactionListModel() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

TransactionListModel::~TransactionListModel() {
    m_data.clear();
}

int TransactionListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant TransactionListModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case transaction_txid_role:
        return m_data[index.row()]->txid();
    case transaction_memo_role:
        return m_data[index.row()]->memo();
    case transaction_status_role:
        return m_data[index.row()]->status();
    case transaction_fee_role:
        return m_data[index.row()]->feeDisplay();
    case transaction_m_role:
        return m_data[index.row()]->m();
    case transaction_hasChange_role:
        return m_data[index.row()]->hasChange();
    case transaction_destinationList_role:
        return qVariantFromValue(m_data[index.row()]->destinationList());
    case transaction_destinationDisp_role:
        return m_data[index.row()]->destination();
    case transaction_change_role:
        return qVariantFromValue(m_data[index.row()]->change());
    case transaction_singleSignersAssigned_role:
        return qVariantFromValue(m_data[index.row()]->singleSignersAssigned());
    case transaction_subtotal_role:
        return m_data[index.row()]->subtotalDisplay();
    case transaction_total_role:
        return m_data[index.row()]->totalDisplay();
    case transaction_numberSigned_role:
        return m_data[index.row()]->numberSigned();
    case transaction_blocktime_role:
        return m_data[index.row()]->blocktimeDisplay();
    case transaction_height_role:
        return m_data[index.row()]->height();
    case transaction_isReceiveTx_role:
        return m_data[index.row()]->isReceiveTx();
    case transaction_replacedTx_role:
        return m_data[index.row()]->get_replaced_by_txid();
    case transaction_subtotalCurrency_role:
        return m_data[index.row()]->subtotalCurrency();
    case transaction_totalCurrency_role:
        return m_data[index.row()]->totalCurrency();
    case transaction_isRbf_role:
        return m_data[index.row()]->get_replace_txid() != "";
    case transaction_miniScript_role:
        return m_data[index.row()]->miniTreeForSigning();
    case transaction_timelockedUntil_role:
        return m_data[index.row()]->timelockedUntil();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TransactionListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[transaction_txid_role]                    = "transaction_txid";
    roles[transaction_memo_role]                    = "transaction_memo";
    roles[transaction_status_role]                  = "transaction_status";
    roles[transaction_fee_role]                     = "transaction_fee";
    roles[transaction_m_role]                       = "transaction_m";
    roles[transaction_hasChange_role]               = "transaction_hasChange";
    roles[transaction_destinationList_role]         = "transaction_destinationList";
    roles[transaction_destinationDisp_role]         = "transaction_destinationDisp_role";
    roles[transaction_change_role]                  = "transaction_change_role";
    roles[transaction_singleSignersAssigned_role]   = "transaction_singleSignersAssigned";
    roles[transaction_subtotal_role]                = "transaction_subtotal";
    roles[transaction_total_role]                   = "transaction_total";
    roles[transaction_numberSigned_role]            = "transaction_numberSigned";
    roles[transaction_blocktime_role]               = "transaction_blocktime";
    roles[transaction_height_role]                  = "transaction_height";
    roles[transaction_isReceiveTx_role]             = "transaction_isReceiveTx";
    roles[transaction_replacedTx_role]              = "transaction_replacedTx";
    roles[transaction_subtotalCurrency_role]        = "transaction_subtotalCurrency";
    roles[transaction_totalCurrency_role]           = "transaction_totalCurrency";
    roles[transaction_isRbf_role]                   = "transaction_isRbf";
    roles[transaction_miniScript_role]              = "transaction_miniScript";
    roles[transaction_timelockedUntil_role]         = "transaction_timelockedUntil";
    return roles;
}

QTransactionPtr TransactionListModel::getTransactionByIndex(const int index) {
    if (index < 0 || index >= m_data.count()) {
        DBG_INFO << "Index out of range";
        return NULL;
    } else {
        return m_data.at(index);
    }
}

QTransactionPtr TransactionListModel::getTransactionByTxid(const QString &txid) {
    foreach (QTransactionPtr i, m_data) {
        if (qUtils::strCompare(txid, i.data()->txid())) {
            return i;
        }
    }
    return NULL;
}

void TransactionListModel::addTransaction(const QTransactionPtr &d) {
    if (d) {
        if (!contains(d.data()->txid())) {
            m_data.append(d);
            emit countChanged();
        }
    }
}

void TransactionListModel::updateTransactionMemo(const QString &tx_id, const QString &memo) {
    for (int i = 0; i < m_data.count(); i++) {
        if (m_data.at(i) && qUtils::strCompare(tx_id, m_data.at(i).data()->txid())) {
            m_data.at(i)->setMemo(memo);
            emit dataChanged(index(i), index(i));
        }
    }
}

void TransactionListModel::updateTransaction(const QString &tx_id, const QTransactionPtr &tx) {
    if (tx) {
        bool existed = false;
        for (int i = 0; i < m_data.count(); i++) {
            if (m_data.at(i) && qUtils::strCompare(tx_id, m_data.at(i).data()->txid())) {
                m_data.at(i)->setNunchukTransaction(tx.data()->nunchukTransaction());
                emit dataChanged(index(i), index(i));
                existed = true;
                break;
            }
        }
        if (!existed) {
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_data.append(tx);
            endInsertRows();
            emit countChanged();
        }
    }
}

void TransactionListModel::updateTransaction(const QString &wallet_id, std::vector<nunchuk::Transaction> txs) {
    beginResetModel();
    m_data.clear();
    for (auto it = txs.begin(); it != txs.end(); ++it) {
        const nunchuk::Transaction &element = *it;
        QTransactionPtr tx = bridge::convertTransaction(element, wallet_id);
        m_data.append(tx);
    }
    endResetModel();
    emit countChanged();
}

void TransactionListModel::removeTransaction(const QString &tx_id) {
    foreach (QTransactionPtr it, m_data) {
        if (it.data() && it.data()->txid() == tx_id) {
            beginResetModel();
            m_data.removeAll(it);
            endResetModel();
            break;
        }
    }
    emit countChanged();
}

bool TransactionListModel::contains(const QString &tx_id) {
    foreach (QTransactionPtr i, m_data) {
        if (qUtils::strCompare(tx_id, i.data()->txid())) {
            return true;
        }
    }
    return false;
}

void TransactionListModel::requestSort(int role, int order) {
    beginResetModel();
    if (m_data.count() > 1) {
        switch (role) {
        case transaction_txid_role:
            break;
        case transaction_memo_role: {
            if (Qt::DescendingOrder == order) {
                qSort(m_data.begin(), m_data.end(), sortTXsByMemoDescending);
            } else {
                qSort(m_data.begin(), m_data.end(), sortTXsByMemoAscending);
            }
        } break;
        case transaction_status_role: {
            if (Qt::DescendingOrder == order) {
                qSort(m_data.begin(), m_data.end(), sortTXsByStatusDescending);
            } else {
                qSort(m_data.begin(), m_data.end(), sortTXsByStatusAscending);
            }
        } break;
        case transaction_subtotal_role:
        case transaction_total_role: {
            if (Qt::DescendingOrder == order) {
                qSort(m_data.begin(), m_data.end(), sortTXsByAmountDescending);
            } else {
                qSort(m_data.begin(), m_data.end(), sortTXsByAmountAscending);
            }
        } break;
        case transaction_blocktime_role: {
            if (Qt::DescendingOrder == order) {
                qSort(m_data.begin(), m_data.end(), sortTXsByBlocktimeDescending);
            } else {
                qSort(m_data.begin(), m_data.end(), sortTXsByBlocktimeAscending);
            }
            linkingReplacedTransactions();
        } break;
        default:
            break;
        }
    }
    endResetModel();
}

void TransactionListModel::notifyUnitChanged() {
    const QVector<int> &roles = {};
    for (int i = 0; i < m_data.count(); i++) {
        if (m_data.at(i).data()) {
            const auto idx = index(i);
            emit dataChanged(idx, idx, roles);
        }
    }
}

void TransactionListModel::linkingReplacedTransactions() {
    QMap<QString, QString> replaces;
    for (int i = 0; i < m_data.count(); i++) {
        if (m_data.at(i) && (int)nunchuk::TransactionStatus::REPLACED == m_data.at(i).data()->status()) {
            replaces[m_data.at(i).data()->txid()] = m_data.at(i).data()->get_replaced_by_txid();
        }
    }

    int from_index = -1;
    int to_index = -1;
    for (int j = 0; j < replaces.keys().count(); j++) {
        from_index = -1;
        to_index = -1;
        for (int k = 0; k < m_data.count(); k++) {
            if (m_data.at(k)) {
                if (m_data.at(k).data()->txid() == replaces.keys()[j]) { // old tx replaced
                    from_index = k;
                }
                if (m_data.at(k).data()->txid() == replaces[replaces.keys()[j]]) { // Find new tx
                    to_index = k;
                }
            }
        }

        if ((-1 != from_index) && (-1 != to_index) && (to_index != from_index)) {
            m_data.move(from_index, to_index);
        }
    }
}

void TransactionListModel::cleardata() {
    beginResetModel();
    m_data.clear();
    endResetModel();
    emit countChanged();
}

int TransactionListModel::count() const {
    return m_data.size();
}

bool sortTXsByBlocktimeAscending(const QTransactionPtr &v1, const QTransactionPtr &v2) {
    const auto d1 = v1.data();
    const auto d2 = v2.data();

    const qint64 blocktime1 = d1->blocktime();
    const qint64 blocktime2 = d2->blocktime();
    const int status1 = d1->status();
    const int status2 = d2->status();
    const qint64 sats1 = d1->totalSats();
    const qint64 sats2 = d2->totalSats();

    if (blocktime1 <= 0 && blocktime2 <= 0) {
        if (status1 != status2) {
            return status1 < status2;
        }
        if (sats1 != sats2) {
            return sats1 > sats2;
        }
        return false;
    }
    if (blocktime1 <= 0)
        return true;
    if (blocktime2 <= 0)
        return false;
    return blocktime1 < blocktime2;
}

bool sortTXsByBlocktimeDescending(const QTransactionPtr &v1, const QTransactionPtr &v2) {
    const auto d1 = v1.data();
    const auto d2 = v2.data();

    const qint64 blocktime1 = d1->blocktime();
    const qint64 blocktime2 = d2->blocktime();
    const int status1 = d1->status();
    const int status2 = d2->status();
    const qint64 sats1 = d1->totalSats();
    const qint64 sats2 = d2->totalSats();

    if (blocktime1 <= 0 && blocktime2 <= 0) {
        if (status1 != status2) {
            return status1 < status2;
        }
        if (sats1 != sats2) {
            return sats1 > sats2;
        }
        return false;
    }
    if (blocktime1 <= 0)
        return true;
    if (blocktime2 <= 0)
        return false;
    return blocktime1 > blocktime2;
}

bool sortTXsByAmountAscending(const QTransactionPtr &v1, const QTransactionPtr &v2) {
    return ((v1.data()->subtotalBTC() + v1.data()->totalBTC()) < (v2.data()->subtotalBTC() + v2.data()->totalBTC()));
}

bool sortTXsByAmountDescending(const QTransactionPtr &v1, const QTransactionPtr &v2) {
    return ((v1.data()->subtotalBTC() + v1.data()->totalBTC()) > (v2.data()->subtotalBTC() + v2.data()->totalBTC()));
}

bool sortTXsByMemoAscending(const QTransactionPtr &v1, const QTransactionPtr &v2) {
    return (QString::compare((v1.data()->memo()), (v2.data()->memo())) < 0);
}

bool sortTXsByMemoDescending(const QTransactionPtr &v1, const QTransactionPtr &v2) {
    return (QString::compare((v1.data()->memo()), (v2.data()->memo())) > 0);
}

bool sortTXsByStatusAscending(const QTransactionPtr &v1, const QTransactionPtr &v2) {
    return ((v1.data()->status()) < (v2.data()->status()));
}

bool sortTXsByStatusDescending(const QTransactionPtr &v1, const QTransactionPtr &v2) {
    return ((v1.data()->status()) > (v2.data()->status()));
}
