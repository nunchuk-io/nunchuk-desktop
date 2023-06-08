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
#include "UTXOModel.h"
#include "AppSetting.h"
#include "qUtils.h"
#include <QQmlEngine>

UTXO::UTXO (const QString &txid,
            const int vout,
            const QString &address,
            const qint64 amount,
            const int height,
            const QString &memo):
    txid_(txid),
    vout_(vout),
    address_(address),
    amount_(amount),
    height_(height),
    memo_(memo),
    selected_(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

UTXO::UTXO() : txid_(""),
    vout_(-1),
    address_(""),
    amount_(0),
    height_(-1),
    memo_(""),
    selected_(false){}

UTXO::~UTXO() { }

QString UTXO::txid() const
{
    return txid_;
}

void UTXO::setTxid(const QString &txid)
{
    if(txid_ != txid){
        txid_ = txid;
        emit txidChanged();
    }
}

int UTXO::vout() const
{
    return vout_;
}

void UTXO::setVout(int vout)
{
    if(vout_ != vout){
        vout_ = vout;
        emit voutChanged();
    }
}

QString UTXO::address() const
{
    return address_;
}

void UTXO::setAddress(const QString &address)
{
    if(address_ != address){
        address_ = address;
        emit addressChanged();
    }
}

QString UTXO::amountDisplay() const
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(amountSats());
    }
    else{
        return amountBTC();
    }
}

QString UTXO::amountBTC() const
{
    return qUtils::QValueFromAmount(amount_);
}

qint64 UTXO::amountSats() const
{
    return amount_;
}

void UTXO::setAmount(const qint64 amount)
{
    if(amount_ != amount){
        amount_ = amount;
        emit amountChanged();
    }
}

int UTXO::height() const
{
    return height_;
}

void UTXO::setHeight(int height)
{
    if(height_ != height){
        height_ = height;
        emit heightChanged();
    }
}

bool UTXO::selected() const
{
    return selected_;
}

void UTXO::setSelected(bool selected)
{
    if(selected_ != selected){
        selected_ = selected;
        emit selectedChanged();
    }
}

QString UTXO::scriptPublickey() const
{
    DBG_INFO << address_;
    return qUtils::QAddressToScriptPubKey(address_);
}

QString UTXO::memo() const
{
    return memo_;
}

void UTXO::setMemo(const QString &memo)
{
    if(memo_ != memo){
        memo_ = memo;
        emit memoChanged();
    }
}

UTXOListModel::UTXOListModel(){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

UTXOListModel::~UTXOListModel(){d_.clear();}

int UTXOListModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return d_.count();
}

QVariant UTXOListModel::data(const QModelIndex &index, int role) const{
    switch (role) {
    case utxo_txid_role:
        return d_[index.row()]->txid();
    case utxo_vout_role:
        return d_[index.row()]->vout();
    case utxo_address_role:
        return d_[index.row()]->address();
    case utxo_amount_role:
        return d_[index.row()]->amountDisplay();
    case utxo_height_role:
        return d_[index.row()]->height();
    case utxo_selected_role:
        return d_[index.row()]->selected();
    case utxo_memo_role:
        return d_[index.row()]->memo();
    default:
        return QVariant();
    }
}

bool UTXOListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == utxo_selected_role){
        d_[index.row()]->setSelected(value.toBool());
        emit dataChanged(index, index, { role } );
    }
    return true;
}

QHash<int, QByteArray> UTXOListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[utxo_txid_role] = "utxo_txid";
    roles[utxo_vout_role] = "utxo_vout";
    roles[utxo_address_role] = "utxo_address";
    roles[utxo_amount_role] = "utxo_amount";
    roles[utxo_height_role] = "utxo_height";
    roles[utxo_selected_role] = "utxo_selected";
    roles[utxo_memo_role] = "utxo_memo";
    return roles;
}

void UTXOListModel::addUTXO(const QString &txid, const int vout, const QString &address, const qint64 amount, const int height,  const QString &memo)
{
    beginResetModel();
    d_.append(QUTXOPtr(new UTXO(txid, vout, address, amount, height, memo)));
    endResetModel();
}

QUTXOPtr UTXOListModel::getUTXOByIndex(const int index)
{
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
        return NULL;
    }
    else {
        return d_.at(index);
    }
}

void UTXOListModel::updateSelected(const QString &txid, const int vout)
{
    foreach (QUTXOPtr it, d_) {
        if((it.data()->txid() == txid) && (it.data()->vout() == vout)){
            beginResetModel();
            it.data()->setSelected(true);
            endResetModel();
        }
    }
}

qint64 UTXOListModel::getAmount(const QString &txid, const int vout)
{
    foreach (QUTXOPtr it, d_) {
        if((it.data()->txid() == txid) && (it.data()->vout() == vout)){
            return it.data()->amountSats();
        }
    }
    return 0;
}

void UTXOListModel::requestSort(int role, int order)
{
    if(d_.count() > 1){
        beginResetModel();
        switch (role) {
        case utxo_address_role:
        {
            if(Qt::AscendingOrder == order) {qSort(d_.begin(), d_.end(), sortbyAddressAscending);}
            else {qSort(d_.begin(), d_.end(), sortbyAddressDescending);}
            break;
        }
        case utxo_amount_role:
        {
            if(Qt::AscendingOrder == order) {qSort(d_.begin(), d_.end(), sortbyAmountAscending);}
            else {qSort(d_.begin(), d_.end(), sortbyAmountDescending);}
            break;
        }
        case utxo_height_role:
        {
            if(Qt::AscendingOrder == order) {qSort(d_.begin(), d_.end(), sortbyHeightDescending);}
            else {qSort(d_.begin(), d_.end(), sortbyHeightAscending);}
            break;
        }
        case utxo_memo_role:
        {
            if(Qt::AscendingOrder == order) {qSort(d_.begin(), d_.end(), sortbyMemoDescending);}
            else {qSort(d_.begin(), d_.end(), sortbyMemoAscending);}
            break;
        }
        default:
            break;
        }

        endResetModel();
    }
}

void UTXOListModel::notifyUnitChanged()
{
    const QVector<int>& roles = {};
    for (int i = 0; i < d_.count(); i++) {
        if(d_.at(i).data()){
            const auto idx = index(i);
            emit dataChanged(idx, idx, roles);
        }
    }
}

QString UTXOListModel::amountDisplay()
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(amountSats());
    }
    else{
        return amountBTC();
    }
}

QString UTXOListModel::amountBTC()
{
    return qUtils::QValueFromAmount(amountSats());
}

int UTXOListModel::amountSats()
{
    int total = 0;
    foreach (QUTXOPtr it, d_) {
        if(it.data()->selected()){
            total += it.data()->amountSats();
        }
    }
    return total;
}

bool sortbyAmountAscending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return (v1.data()->amountSats()) < (v2.data()->amountSats());
}

bool sortbyAmountDescending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return (v1.data()->amountSats()) > (v2.data()->amountSats());
}

bool sortbyAddressAscending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return (QString::compare((v1.data()->address()), (v2.data()->address())) < 0);
}

bool sortbyAddressDescending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return (QString::compare((v1.data()->address()), (v2.data()->address())) > 0);
}

bool sortbyHeightAscending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return v1.data()->height() < v2.data()->height();
}

bool sortbyHeightDescending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return v1.data()->height() > v2.data()->height();
}

bool sortbyMemoAscending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return (QString::compare((v1.data()->memo()), (v2.data()->memo())) < 0);
}

bool sortbyMemoDescending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return (QString::compare((v1.data()->memo()), (v2.data()->memo())) > 0);
}
