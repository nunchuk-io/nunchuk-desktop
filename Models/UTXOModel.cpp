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
#include "AppModel.h"
#include "bridgeifaces.h"
#include "qUtils.h"
#include <algorithm>
#include <QQmlEngine>
#include "QOutlog.h"

QCoinTagsModel::QCoinTagsModel()
{

}

QCoinTagsModel::~QCoinTagsModel()
{

}

int QCoinTagsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant QCoinTagsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(index.row() >= m_data.size())
        return QVariant();

    switch (role) {
    case tag_id_role:
        return m_data[index.row()].get_id();
    case tag_name_role:
        return QString::fromStdString(m_data[index.row()].get_name());
    case tag_color_role:
        return QString::fromStdString(m_data[index.row()].get_color());
    case tag_count_role:
        return get_count(m_data[index.row()].get_id());
    case tag_checked_role:
        return isChecked(m_data[index.row()].get_id());
    default:
        return QVariant();
    }
}

bool QCoinTagsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // FIXME
    // if(role == utxo_selected_role){
    //     m_data[index.row()]->setSelected(value.toBool());
    //     emit dataChanged(index, index, { role } );
    // }
    return true;
}

QHash<int, QByteArray> QCoinTagsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[tag_id_role]    = "coin_tag_id";
    roles[tag_name_role]  = "coin_tag_name";
    roles[tag_color_role] = "coin_tag_color";
    roles[tag_count_role] = "coin_tag_count";
    roles[tag_checked_role] = "coin_tag_checked";
    return roles;
}

QVariant QCoinTagsModel::get(int row)
{
    QHash<int,QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    while (i.hasNext()) {
        i.next();
        QModelIndex idx = index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
    }
    return QVariant::fromValue(res);
}

void QCoinTagsModel::addTag(const nunchuk::CoinTag data)
{
    beginResetModel();
    m_data.append(data);
    endResetModel();
    emit countChanged();
}

void QCoinTagsModel::refreshTags(QList<int> tags, const QString &wallet_id, bool isUtxo)
{
    std::vector cointags = bridge::nunchukGetCoinTags(wallet_id.toStdString());
    beginResetModel();
    m_data.clear();
    if (isUtxo) {
        // looking tags in cointags
        for (const auto &tag : tags) {
            for (const auto &cointag : cointags) {
                if (cointag.get_id() == tag) {
                    m_data.append(cointag);
                }
            }
        }
    } else {
        for (const auto &cointag: cointags) {
            increase(cointag.get_id(), wallet_id);
            m_data.append(cointag);
        }
    }
    endResetModel();
    emit countChanged();
}

int QCoinTagsModel::count() const
{
    return rowCount();
}

void QCoinTagsModel::clear()
{
    beginResetModel();
    m_data.clear();
    m_tags_count.clear();
    m_tags_checked.clear();
    endResetModel();
    emit countChanged();
}

QVariant QCoinTagsModel::tag()
{
    for (int i = 0; i < m_data.size(); ++i) {
        auto tag = m_data.at(i);
        if (tag.get_id() == m_tag_id) {
            return get(i);
        }
    }
    return {};
}

void QCoinTagsModel::setTag(const int tag_id)
{
    m_tag_id = tag_id;
    emit tagChanged();
}

nunchuk::CoinTag QCoinTagsModel::getCoinTagById(const int tag_id)
{
    // lookup
    for (const auto &tag : m_data) {
        if (tag.get_id() == tag_id) {
            return tag;
        }
    }

    return nunchuk::CoinTag(-1, "", "");
}

bool QCoinTagsModel::containColor(const QString &color)
{
    for (auto tag : m_data) {
        if (tag.get_color().find(color.toStdString()) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void QCoinTagsModel::increase(const int tag_id, QString wallet_id)
{
    int count = bridge::nunchukGetCoinByTag(wallet_id, tag_id).size();
    tag_t t {tag_id, count};
    m_tags_count.insert(tag_id, t);
}

int QCoinTagsModel::get_count(const int tag_id) const
{
    return m_tags_count.value(tag_id).m_count;
}

bool QCoinTagsModel::isChecked()
{
    return m_data.count() == m_tags_checked.count();
}

bool QCoinTagsModel::isPartiallyChecked()
{
    return m_tags_checked.count() > 0 && m_tags_checked.count() < m_data.count();
}

bool QCoinTagsModel::isChecked(const int tag_id) const
{
    return m_tags_checked.contains(tag_id);
}

void QCoinTagsModel::setChecked(const QList<int> tags)
{
    m_tags_checked = tags;
    emit existCheckedChanged();
}

void QCoinTagsModel::setChecked(const int tag_id, bool checked)
{
    if (checked) {
        m_tags_checked.append(tag_id);
    } else {
        m_tags_checked.removeAll(tag_id);
    }
    refresh();
    emit existCheckedChanged();
}

void QCoinTagsModel::refresh()
{
    beginResetModel();
    endResetModel();
}

QList<int> QCoinTagsModel::tagsSelected() const
{
    return m_tags_checked;
}

bool QCoinTagsModel::existChecked() const
{
    return m_tags_checked.size() > 0;
}

QList<nunchuk::CoinTag> QCoinTagsModel::fullList() const
{
    return m_data;
}

QStringList QCoinTagsModel::fullTags() const
{
    QStringList tags;
    for (const auto &tag : m_data) {
        tags.append(QString::fromStdString(tag.get_name()));
    }
    return tags;
}

bool QCoinTagsModel::hasTag(const QString &tag)
{
    return fullTags().contains(tag);
}

int QCoinTagsModel::selectedCount() const
{
    return m_tags_checked.size();
}

void QCoinTagsModel::storeSelected()
{
    m_store_filter_tags_checked = m_tags_checked;
}

void QCoinTagsModel::cancelSelected()
{
    m_tags_checked = m_store_filter_tags_checked;
    refresh();
    emit existCheckedChanged();
}

void QCoinTagsModel::selectAll(bool select)
{
    beginResetModel();
    m_tags_checked.clear();
    if (select) {
        for (int i = 0; i < m_data.size(); i++) {
            m_tags_checked.append(m_data[i].get_id());
        }
    }
    endResetModel();
    emit existCheckedChanged();
    DBG_INFO << select << m_tags_checked.size() << m_data.size();
}

QCoinCollectionsModel::QCoinCollectionsModel()
{

}

QCoinCollectionsModel::~QCoinCollectionsModel()
{

}

int QCoinCollectionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant QCoinCollectionsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(index.row() >= m_data.size())
        return QVariant();

    switch (role) {
    case collection_id_role:
        return m_data[index.row()].get_id();
    case collection_name_role:
        return QString::fromStdString(m_data[index.row()].get_name());
    case collection_add_new_coin_role:
        return m_data[index.row()].is_add_new_coin();
    case collection_auto_lock_role:
        return m_data[index.row()].is_auto_lock();
    case collection_add_tags_role:
        return qVariantFromValue(m_data[index.row()].get_add_coins_with_tag());
    case collection_count_role:
        return get_count(m_data[index.row()].get_id());
    case collection_checked_role:
        return isChecked(m_data[index.row()].get_id());
    default:
        return QVariant();
    }
}

bool QCoinCollectionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //FIXME
    return true;
}

QHash<int, QByteArray> QCoinCollectionsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[collection_id_role] = "coin_collection_id";
    roles[collection_name_role] = "coin_collection_name";
    roles[collection_add_new_coin_role] = "coin_collection_add_new_coin";
    roles[collection_auto_lock_role] = "coin_collection_auto_lock";
    roles[collection_add_tags_role] = "coin_collection_add_tags";
    roles[collection_count_role] = "coin_collection_count";
    roles[collection_checked_role] = "coin_collection_checked";
    return roles;
}

QVariant QCoinCollectionsModel::get(int row)
{
    QHash<int,QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    while (i.hasNext()) {
        i.next();
        QModelIndex idx = index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
    }
    return QVariant::fromValue(res);
}

void QCoinCollectionsModel::addCollection(const nunchuk::CoinCollection data)
{
    beginResetModel();
    m_data.append(data);
    endResetModel();
    emit countChanged();
}

void QCoinCollectionsModel::refreshCollections(const QList<int> collections, const QString &wallet_id, bool isUtxo)
{
    std::vector coinCollections = bridge::nunchukGetCoinCollections(wallet_id.toStdString());
    if (isUtxo) {
        beginResetModel();
        m_data.clear();
        // looking collections in coinCollections
        for (const auto &collection : collections) {
            for (const auto &coinCollection : coinCollections) {
                if (coinCollection.get_id() == collection) {
                    m_data.append(coinCollection);
                }
            }
        }
        endResetModel();
        emit countChanged();
    } else {
        beginResetModel();
        m_data.clear();
        for (const auto &coinCollection : coinCollections) {
            increase(coinCollection.get_id(), wallet_id);
            m_data.append(coinCollection);
        }
        endResetModel();
        emit countChanged();
    }
}

int QCoinCollectionsModel::count() const
{
    return rowCount();
}

void QCoinCollectionsModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
    emit countChanged();
    m_collections_count.clear();
}

QVariant QCoinCollectionsModel::collection()
{
    for (int i = 0; i < m_data.size(); ++i) {
        auto collection = m_data.at(i);
        if (collection.get_id() == m_collection_id) {
            return get(i);
        }
    }
    return {};
}

void QCoinCollectionsModel::setCollection(const int collection_id)
{
    m_collection_id = collection_id;
    emit collectionChanged();
}

void QCoinCollectionsModel::increase(const int collection_id, QString wallet_id)
{
    int count = bridge::nunchukGetCoinByCollection(wallet_id, collection_id).size();
    collection_t t {collection_id, count};
    m_collections_count.insert(collection_id, t);
}

int QCoinCollectionsModel::get_count(const int collection_id) const
{
    return m_collections_count.value(collection_id).m_count;
}

nunchuk::CoinCollection QCoinCollectionsModel::getCoinCollectionById(const int collection_id)
{
    // lookup
    for (const auto &collection : m_data) {
        if (collection.get_id() == collection_id) {
            return collection;
        }
    }
    return nunchuk::CoinCollection(-1, "");
}

bool QCoinCollectionsModel::isChecked()
{
    return m_collections_count.count() == m_collections_checked.count();
}

bool QCoinCollectionsModel::isPartiallyChecked()
{
    return m_collections_checked.count() > 0 && m_collections_checked.count() < m_data.count();
}

bool QCoinCollectionsModel::isChecked(const int collection_id) const
{
    return m_collections_checked.contains(collection_id);
}

void QCoinCollectionsModel::setChecked(const QList<int> collections)
{
    m_collections_checked = collections;
    emit existCheckedChanged();
}

void QCoinCollectionsModel::setChecked(const int collection_id, bool checked)
{
    if (checked) {
        m_collections_checked.append(collection_id);
    } else {
        m_collections_checked.removeAll(collection_id);
    }
    refresh();
    emit existCheckedChanged();
}

void QCoinCollectionsModel::refresh()
{
    beginResetModel();
    endResetModel();
}

bool QCoinCollectionsModel::existChecked() const
{
    return m_collections_checked.size() > 0;
}

QList<nunchuk::CoinCollection> QCoinCollectionsModel::fullList() const
{
    return m_data;
}

bool QCoinCollectionsModel::hasCollection(const QString &searchText)
{
    for (auto collection : m_data) {
        std::string name = collection.get_name();
        if (name.find(searchText.toStdString()) != std::string::npos) {  // Check if searchTerm is found in str
            return true;                // Print matching string
        }
    }
    return false;
}

int QCoinCollectionsModel::selectedCount() const
{
    return m_collections_checked.size();
}

void QCoinCollectionsModel::cancelSelected()
{
    m_collections_checked = m_store_filter_collections_checked;
    refresh();
    emit existCheckedChanged();
}

void QCoinCollectionsModel::storeSelected()
{
    m_store_filter_collections_checked = m_collections_checked;
}

void QCoinCollectionsModel::selectAll(bool select)
{
    beginResetModel();
    m_collections_checked.clear();
    if (select) {
        for (int i = 0; i < m_data.count(); i++) {
            m_collections_checked.append(m_data[i].get_id());
        }
    }
    endResetModel();
    emit existCheckedChanged();
}

UTXO::UTXO(QString wallet_id, nunchuk::UnspentOutput data)
    : m_selected(false)
    , mUnspentOutputOrigin(data)
    , m_coinTags(new QCoinTagsModel())
    , m_coinCollections(new QCoinCollectionsModel())
    , m_wallet_id(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

UTXO::UTXO(QString wallet_id)
    : m_selected(false)
    , mUnspentOutputOrigin()
    , m_coinTags(new QCoinTagsModel())
    , m_coinCollections(new QCoinCollectionsModel())
    , m_wallet_id(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

UTXO::~UTXO() { }

QString UTXO::txid() const
{
    return QString::fromStdString(mUnspentOutputOrigin.get_txid());
}

void UTXO::setTxid(const QString &txid)
{
    if(txid != this->txid()){
        mUnspentOutputOrigin.set_txid(txid.toStdString());
        emit unspentOutputChanged();
    }
}

int UTXO::vout() const
{
    return mUnspentOutputOrigin.get_vout();
}

void UTXO::setVout(int vout)
{
    if(vout != this->vout()){
        mUnspentOutputOrigin.set_vout(vout);
        emit unspentOutputChanged();
    }
}

QString UTXO::address() const
{
    return QString::fromStdString(mUnspentOutputOrigin.get_address());
}

void UTXO::setAddress(const QString &address)
{
    if(address != this->address()){
        mUnspentOutputOrigin.set_address(address.toStdString());
        emit unspentOutputChanged();
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
    return qUtils::QValueFromAmount(amountSats());
}

qint64 UTXO::amountSats() const
{
    return mUnspentOutputOrigin.get_amount();
}

QString UTXO::amountCurrency() const
{
    return qUtils::currencyLocale(amountSats());
}

void UTXO::setAmount(const qint64 amount)
{
    if(amount != amountSats()){
        mUnspentOutputOrigin.set_amount(amount);
        emit unspentOutputChanged();
    }
}

int UTXO::confirmedCount()
{
    qint64 conf = 0;
    if((int)nunchuk::CoinStatus::CONFIRMED == this->status()){
        conf = std::max(0, (AppModel::instance()->blockHeight() - this->height())+1);
    }
    return conf;
}

int UTXO::height() const
{
    return mUnspentOutputOrigin.get_height();
}

void UTXO::setHeight(int height)
{
    if(height != this->height()){
        mUnspentOutputOrigin.set_height(height);
        emit unspentOutputChanged();
    }
}

bool UTXO::selected() const
{
    return m_selected;
}

void UTXO::setSelected(bool selected)
{
    if(m_selected != selected){
        m_selected = selected;
        emit selectedChanged();
    }
}

QString UTXO::scriptPublickey() const
{
    return qUtils::QAddressToScriptPubKey(address());
}

QString UTXO::memo() const
{
    return QString::fromStdString(mUnspentOutputOrigin.get_memo());
}

void UTXO::setMemo(const QString &memo)
{
    if(memo != this->memo()){
        mUnspentOutputOrigin.set_memo(memo.toStdString());
        emit unspentOutputChanged();
    }
}

QString UTXO::outpoint() const
{
    return QString("%1:%2").arg(txid()).arg(vout());
}

const QMap<nunchuk::CoinStatus, QString> mapsName = {
    { nunchuk::CoinStatus::INCOMING_PENDING_CONFIRMATION, "Incoming (pending conf.)" },
    { nunchuk::CoinStatus::CONFIRMED, "Confirmed" },
    { nunchuk::CoinStatus::OUTGOING_PENDING_SIGNATURES, "Outgoing (pending sig.)" },
    { nunchuk::CoinStatus::OUTGOING_PENDING_BROADCAST, "Outgoing (pending broadcast)" },
    { nunchuk::CoinStatus::OUTGOING_PENDING_CONFIRMATION, "Outgoing (pending conf.)" },
    { nunchuk::CoinStatus::SPENT, "Spent" },
};

const QMap<nunchuk::CoinStatus, QString> mapsColor = {
    { nunchuk::CoinStatus::INCOMING_PENDING_CONFIRMATION, "#E8DAFF" },
    { nunchuk::CoinStatus::CONFIRMED, "#B4DCFF" },
    { nunchuk::CoinStatus::OUTGOING_PENDING_SIGNATURES, "#FFD7D9" },
    { nunchuk::CoinStatus::OUTGOING_PENDING_BROADCAST, "#FDEBD2" },
    { nunchuk::CoinStatus::OUTGOING_PENDING_CONFIRMATION, "#E8DAFF" },
    { nunchuk::CoinStatus::SPENT, "#9EC063" },
    };

QVariant UTXO::outgoing() const
{
    QMap<QString, QVariant> maps;
    maps["label"] = mapsName.value(static_cast<nunchuk::CoinStatus>(status()), "Unknown");
    maps["color"] = mapsColor.value(static_cast<nunchuk::CoinStatus>(status()), "#FFFFFF");
    return QVariant::fromValue(maps);
}

int UTXO::status() const
{
    return (int)mUnspentOutputOrigin.get_status();
}

void UTXO::setStatus(int status)
{
    if(status != this->status()){
        mUnspentOutputOrigin.set_status((nunchuk::CoinStatus)status);
        emit unspentOutputChanged();
    }
}

bool UTXO::isChange() const
{
    return mUnspentOutputOrigin.is_change();
}

bool UTXO::isLocked() const
{
    return mUnspentOutputOrigin.is_locked();
}

void UTXO::setIsLocked(bool locked)
{
    mUnspentOutputOrigin.set_locked(locked);
    emit unspentOutputChanged();
}

const QList<int> UTXO::getTags() const
{
    const std::vector<int>& vector = mUnspentOutputOrigin.get_tags();
    QList<int> list;
    list.reserve(vector.size());
    std::copy(vector.begin(), vector.end(), std::back_inserter(list));
    return list;
}

void UTXO::setTags(const QList<int> &tags)
{
    std::vector<int> vector;
    vector.reserve(tags.size());
    std::copy(tags.begin(), tags.end(), std::back_inserter(vector));
    mUnspentOutputOrigin.set_tags(vector);
}

const QList<int> UTXO::getCollections() const
{
    const std::vector<int>& vector = mUnspentOutputOrigin.get_collections();
    QList<int> list;
    list.reserve(vector.size());
    std::copy(vector.begin(), vector.end(), std::back_inserter(list));
    return list;
}

void UTXO::setCollections(const QList<int> &collections)
{
    std::vector<int> vector;
    vector.reserve(collections.size());
    std::copy(collections.begin(), collections.end(), std::back_inserter(vector));
    mUnspentOutputOrigin.set_collections(vector);
}

time_t UTXO::blocktime() const
{
    return mUnspentOutputOrigin.get_blocktime();
}

QString UTXO::blocktimeDisplay()
{
    if(0 >= this->blocktime()){
        return "--/--/----"; // There is no time
    }
    else{
        return QDateTime::fromTime_t(this->blocktime()).toString( "MM/dd/yyyy hh:mm AP");
    }
}

QString UTXO::blocktimeDisplayTwo()
{
    if(0 >= this->blocktime()){
        return "--/--/----"; // There is no time
    }
    else{
        QDateTime date_time = QDateTime::fromTime_t(this->blocktime());
        return QString("%1 at %2")
            .arg(date_time.date().toString("MM/dd/yyyy"))
            .arg(date_time.time().toString("hh:mm AP"));
    }
}

void UTXO::setBlocktime(const time_t blocktime)
{
    if(blocktime != this->blocktime()){
        mUnspentOutputOrigin.set_blocktime(blocktime);
        emit unspentOutputChanged();
    }
}

time_t UTXO::scheduleTime() const
{
    return mUnspentOutputOrigin.get_schedule_time();
}

QString UTXO::scheduleTimeDisplay()
{
    if(0 >= this->scheduleTime()){
        return "--/--/----"; // There is no time
    }
    else{
        return QDateTime::fromTime_t(this->scheduleTime()).toString( "MM/dd/yyyy hh:mm AP");
    }
}

void UTXO::setScheduleTime(const time_t scheduleTime)
{
    if(scheduleTime != this->scheduleTime()){
        mUnspentOutputOrigin.set_schedule_time(scheduleTime);
        emit unspentOutputChanged();
    }
}

nunchuk::UnspentOutput UTXO::getUnspentOutput() const
{
    return mUnspentOutputOrigin;
}

void UTXO::setUnspentOutput(const nunchuk::UnspentOutput &data)
{
    mUnspentOutputOrigin = data;
    emit unspentOutputChanged();
}

QCoinTagsModel *UTXO::coinTags() const
{
    return coinTagsPtr().data();
}

QCoinTagsModelPtr UTXO::coinTagsPtr() const
{
    m_coinTags->refreshTags(getTags(), m_wallet_id, true);
    return m_coinTags;
}

void UTXO::setCoinTags(QCoinTagsModelPtr coinTags)
{
    m_coinTags = coinTags;
    emit coinTagsChanged();
}

QCoinCollectionsModel *UTXO::coinCollections() const
{
    return coinCollectionsPtr().data();
}

QCoinCollectionsModelPtr UTXO::coinCollectionsPtr() const
{
    m_coinCollections->refreshCollections(getCollections(), m_wallet_id, true);
    return m_coinCollections;
}

void UTXO::setCoinCollections(QCoinCollectionsModelPtr coinCollections)
{
    m_coinCollections = coinCollections;
    emit coinCollectionsChanged();
}

Transaction *UTXO::transaction()
{
    return transactionPtr().data();
}

QTransactionPtr UTXO::transactionPtr()
{
    if (m_trans.isNull()) {
        if (auto w = AppModel::instance()->walletListPtr()->getWalletById(m_wallet_id)) {
            m_trans = w->transactionHistory()->getTransactionByTxid(txid());
        }
    }
    if(m_trans.isNull()){
        m_trans = bridge::nunchukGetTransaction(m_wallet_id, txid());
    }
    return m_trans;
}

bool UTXO::hasTag(const QString &searchText)
{
    return m_coinTags->hasTag(searchText);
}

bool UTXO::hasCollection(const QString &searchText)
{
    return m_coinCollections->hasCollection(searchText);
}

bool UTXO::hasAmount(const QString &searchText)
{
    if (amountDisplay().toStdString().find(searchText.toStdString())  != std::string::npos) {
        return true;
    }
    return false;
}

bool UTXO::hasAddress(const QString &searchText)
{
    if (address().toStdString().find(searchText.toStdString())  != std::string::npos) {
        return true;
    }
    return false;
}

bool UTXO::hasMemo(const QString &searchText)
{
    if (memo().toStdString().find(searchText.toStdString())  != std::string::npos) {
        return true;
    }
    return false;
}

bool UTXO::hasBlocktime(const QString &searchText)
{
    if (blocktimeDisplay().toStdString().find(searchText.toStdString())  != std::string::npos) {
        return true;
    }
    return false;
}

bool UTXO::hasTransactionID(const QString &searchText)
{
    if (txid().toStdString().find(searchText.toStdString())  != std::string::npos) {
        return true;
    }
    return false;
}

bool UTXO::hasResultSearching(const QString &searchText)
{
    return hasTag(searchText) || hasCollection(searchText) || hasAmount(searchText) || hasAddress(searchText) || hasMemo(searchText) || hasBlocktime(searchText) || hasTransactionID(searchText);
}

int UTXO::lockedIndex()
{
    return -1;
}

int UTXO::timelockbase()
{
    return (int)mUnspentOutputOrigin.get_lock_based();
}

int UTXO::timelockCount()
{
    return (int)mUnspentOutputOrigin.get_timelocks().size();
}

QVariantList UTXO::timelocklist()
{
    QVariantList ret;
    std::vector<int64_t> timelocks = mUnspentOutputOrigin.get_timelocks();

    bool anyLocked = false;

    for (int i = 0; i < (int)timelocks.size(); ++i) {
        int64_t val = timelocks[i];

        QVariantMap obj;
        QString valueNodeStr;
        QString valueRemainingStr;
        QString remainingString  = "";
        qint64  remainingNumeric = 0;
        bool    needVisibleWarning = false;
        bool    isLocked = false;

        if (timelockbase() == (int)nunchuk::Timelock::Based::TIME_LOCK) {
            DBG_INFO << val << QDateTime::currentDateTimeUtc().toTime_t();
            QDateTime dt = QDateTime::fromSecsSinceEpoch(val, Qt::UTC);
            valueNodeStr = dt.toString("MM/dd/yyyy");
            QDate dateNode = dt.date();
            QDate today = QDateTime::currentDateTimeUtc().date();
            int days = today.daysTo(dateNode);
            if (days > 0) {
                valueRemainingStr = QString::number(days) + (days == 1 ? " day left" : " days left");
                remainingString  = QString::number(days) + (days == 1 ? " day" : " days");
                isLocked = true;
                remainingNumeric = days;
                needVisibleWarning = days < 7;
            }
            else {
                qint64 secondsTo = QDateTime::currentDateTimeUtc().secsTo(dt);
                qint64 hours = secondsTo / 3600;
                qint64 minutes = (secondsTo % 3600) / 60;
                if (hours >= 1) {
                    valueRemainingStr = QString::number(hours) + (hours == 1 ? " hour left" : " hours left");
                    remainingString  = QString::number(hours) + (hours == 1 ? " hour" : " hours");
                    remainingNumeric = 1;
                    isLocked = true;
                }
                else if (minutes >= 1) {
                    valueRemainingStr = QString::number(minutes) + (minutes == 1 ? " minute left" : " minutes left");
                    remainingString  = QString::number(minutes) + (minutes == 1 ? " minute" : " minutes");
                    remainingNumeric = 1;
                    isLocked = true;
                }
                else {
                    valueRemainingStr = "";
                    remainingString = "";
                    remainingNumeric = 0;
                    isLocked = false;
                }
            }
        }
        else {
            QLocale locale(QLocale::English);
            int64_t currentHeight = AppModel::instance()->blockHeight();
            int64_t remain = val - currentHeight;
            valueNodeStr = locale.toString(val);
            if (val > currentHeight) {
                QString remainStr = locale.toString(remain);
                valueRemainingStr = remainStr + (remain == 1 ? " block left" : " blocks left");
                remainingString  = remainStr + (remain == 1 ? " block" : " blocks");
                remainingNumeric = remain;
                needVisibleWarning = remain < 1008;
                isLocked = true;
            } else {
                valueRemainingStr = "";
            }
        }

        if (isLocked) anyLocked = true;

        obj["valueNode"] = valueNodeStr;
        obj["valueIndex"] = i;
        obj["valueLocked"] = isLocked;
        obj["valueRemaining"] = valueRemainingStr;
        obj["valueRemainingString"]  = remainingString;
        obj["valueRemainingNumeric"] = remainingNumeric;
        obj["valueNeedVisibleWarning"] = needVisibleWarning;
        ret << obj;
    }

    if (ret.size() == 1) {
        QVariantMap emptyObj;
        emptyObj["valueNode"] = "";
        emptyObj["valueIndex"] = ret.size();
        emptyObj["valueLocked"] = false;
        emptyObj["valueRemaining"] = anyLocked ? "" : "Unlocked";
        emptyObj["valueRemainingString"]  = "";
        emptyObj["valueRemainingNumeric"] = 0;
        emptyObj["valueNeedVisibleWarning"] = false;
        ret << emptyObj;
    }
    setTimeLocked(anyLocked);
    return ret;
}

bool UTXO::timeLocked() const
{
    return m_timeLocked;
}

void UTXO::setTimeLocked(bool newTimeLocked)
{
    if (m_timeLocked == newTimeLocked)
        return;
    m_timeLocked = newTimeLocked;
    emit timeLockedChanged();
}

QUTXOListModel::QUTXOListModel(QString wallet_id) : m_wallet_id(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QUTXOListModel::~QUTXOListModel(){
    m_data.clear();
}

int QUTXOListModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant QUTXOListModel::data(const QModelIndex &index, int role) const{
    switch (role) {
    case utxo_txid_role:
        return m_data[index.row()]->txid();
    case utxo_vout_role:
        return m_data[index.row()]->vout();
    case utxo_address_role:
        return m_data[index.row()]->address();
    case utxo_amount_role:
        return m_data[index.row()]->amountDisplay();
    case utxo_height_role:
        return m_data[index.row()]->height();
    case utxo_selected_role:
        return m_data[index.row()]->selected();
    case utxo_memo_role:
        return m_data[index.row()]->memo();
    case utxo_confirmed_role:
        return m_data[index.row()]->confirmedCount();
    case utxo_tags_role:
        return qVariantFromValue(m_data[index.row()]->coinTags());
    case utxo_collections_role:
        return qVariantFromValue(m_data[index.row()]->coinCollections());
    case utxo_amount_currency_role:
        return m_data[index.row()]->amountCurrency();
    case utxo_blocktime_role:
        return m_data[index.row()]->blocktimeDisplayTwo();
    case utxo_coin_is_locked:
        return m_data[index.row()]->isLocked();
    case utxo_coin_is_scheduled:
        return (m_data[index.row()]->scheduleTime() > 0);
    case utxo_coin_visible_role: {
        if (!m_searchText.isEmpty()) {
            return m_data[index.row()]->hasResultSearching(m_searchText);
        } else if (!mFilters.isEmpty()){
            return hasResultFiltering(index.row());
        } else {
            return true;
        }
    }
    case utxo_outgoing_label_role: {
        return mapsName.value(static_cast<nunchuk::CoinStatus>(m_data[index.row()]->status()), "Unknown");
    }
    case utxo_outgoing_color_role: {
        return mapsColor.value(static_cast<nunchuk::CoinStatus>(m_data[index.row()]->status()), "#FFFFFF");
    }
    case utxo_coin_is_change_role: {
        return m_data[index.row()]->isChange();
    }
    case utxo_coin_timelockbase_role: {
        return m_data[index.row()]->timelockbase();
    }
    case utxo_coin_timelocklist_role: {
        return m_data[index.row()]->timelocklist();
    }
    case utxo_coin_timelockCount_role: {
        return m_data[index.row()]->timelockCount();
    }
    case utxo_coin_timeLocked_role: {
        return m_data[index.row()]->timeLocked();
    }
    default:
        return QVariant();
    }
}

bool QUTXOListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == utxo_selected_role){
        DBG_INFO << value.toBool();
        m_data[index.row()]->setSelected(value.toBool());
        emit dataChanged(index, index, { role } );
        emit selectedCountChanged();
        emit amountChanged();
    }
    return true;
}

QHash<int, QByteArray> QUTXOListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[utxo_txid_role]       = "utxo_txid";
    roles[utxo_vout_role]       = "utxo_vout";
    roles[utxo_address_role]    = "utxo_address";
    roles[utxo_amount_role]     = "utxo_amount";
    roles[utxo_height_role]     = "utxo_height";
    roles[utxo_selected_role]   = "utxo_selected";
    roles[utxo_confirmed_role]  = "utxo_confirmed";
    roles[utxo_memo_role]       = "utxo_memo";
    roles[utxo_amount_currency_role] = "utxo_amount_currency";
    roles[utxo_blocktime_role]  = "utxo_blocktime";
    roles[utxo_tags_role]       = "utxo_coin_tags";
    roles[utxo_collections_role]    = "utxo_outgoing";
    roles[utxo_coin_is_locked]      = "utxo_coin_is_locked";
    roles[utxo_coin_is_scheduled]   = "utxo_coin_is_scheduled";
    roles[utxo_coin_visible_role]   = "utxo_coin_visible";
    roles[utxo_outgoing_label_role] = "utxo_outgoing_label";
    roles[utxo_outgoing_color_role] = "utxo_outgoing_color";
    roles[utxo_coin_is_change_role] = "utxo_coin_is_change";
    roles[utxo_coin_timelockbase_role]  = "utxo_coin_timelockbase";
    roles[utxo_coin_timelocklist_role]  = "utxo_coin_timelocklist";
    roles[utxo_coin_timelockCount_role] = "utxo_coin_timelockCount";
    roles[utxo_coin_timeLocked_role]    = "utxo_coin_timeLocked";

    return roles;
}

void QUTXOListModel::setWalletId(const QString &wallet_id)
{
    m_wallet_id = wallet_id;
}

void QUTXOListModel::addUTXO(const nunchuk::UnspentOutput data )
{
    beginResetModel();
    m_data.append(QUTXOPtr(new UTXO(m_wallet_id, data)));
    endResetModel();
    emit countChanged();
}

QUTXOPtr QUTXOListModel::getUTXOByIndex(const int index)
{
    if(index < 0 || index >= m_data.count()){
        DBG_INFO << "Index out of range";
        return NULL;
    }
    else {
        return m_data.at(index);
    }
}

QUTXOPtr QUTXOListModel::getUTXOByTxid(const QString &txid)
{
    for (QUTXOPtr it: m_data) {
        if(it->txid() == txid){
            return it;
        }
    }
    return NULL;
}

void QUTXOListModel::updateSelected(const QString &txid, const int vout)
{
    foreach (QUTXOPtr it, m_data) {
        if((it.data()->txid() == txid) && (it.data()->vout() == vout)){
            beginResetModel();
            it.data()->setSelected(true);
            endResetModel();
        }
    }
    emit selectedCountChanged();
    emit amountChanged();
}

qint64 QUTXOListModel::getAmount(const QString &txid, const int vout)
{
    foreach (QUTXOPtr it, m_data) {
        if((it.data()->txid() == txid) && (it.data()->vout() == vout)){
            return it.data()->amountSats();
        }
    }
    return 0;
}

void QUTXOListModel::requestSort(int role, int order)
{
    if(m_data.count() > 1){
        beginResetModel();
        switch (role) {
        case utxo_address_role:
        {
            if(Qt::AscendingOrder == order) {qSort(m_data.begin(), m_data.end(), sortbyAddressAscending);}
            else {qSort(m_data.begin(), m_data.end(), sortbyAddressDescending);}
            break;
        }
        case utxo_amount_role:
        {
            if(Qt::AscendingOrder == order) {qSort(m_data.begin(), m_data.end(), sortbyAmountAscending);}
            else {qSort(m_data.begin(), m_data.end(), sortbyAmountDescending);}
            break;
        }
        case utxo_height_role:
        {
            if(Qt::AscendingOrder == order) {qSort(m_data.begin(), m_data.end(), sortbyHeightDescending);}
            else {qSort(m_data.begin(), m_data.end(), sortbyHeightAscending);}
            break;
        }
        case utxo_memo_role:
        {
            if(Qt::AscendingOrder == order) {qSort(m_data.begin(), m_data.end(), sortbyMemoDescending);}
            else {qSort(m_data.begin(), m_data.end(), sortbyMemoAscending);}
            break;
        }
        case utxo_blocktime_role:
        {
            if(Qt::AscendingOrder == order) {qSort(m_data.begin(), m_data.end(), sortbyBlocktimeDescending);}
            else {qSort(m_data.begin(), m_data.end(), sortbyBlocktimeAscending);}
            break;
        }
        default:
            break;
        }

        endResetModel();
    }
}

void QUTXOListModel::notifyUnitChanged()
{
    const QVector<int>& roles = {};
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data()){
            const auto idx = index(i);
            emit dataChanged(idx, idx, roles);
        }
    }
}

int QUTXOListModel::selectedCount()
{
    int selectCount = 0;
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && m_data.at(i).data()->selected()){
            selectCount++;
        }
    }
    return selectCount;
}

int QUTXOListModel::lockedCount()
{
    int lockedCount = 0;
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && m_data.at(i).data()->isLocked()){
            lockedCount++;
        }
    }
    return lockedCount;
}

int QUTXOListModel::selectedLockedCount()
{
    int lockedCount = 0;
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && m_data.at(i).data()->isLocked() && m_data.at(i).data()->selected()){
            lockedCount++;
        }
    }
    return lockedCount;
}

bool QUTXOListModel::isChecked()
{
    return selectedCount() == m_data.count();
}

bool QUTXOListModel::isUnChecked()
{
    return selectedCount() == 0;
}

bool QUTXOListModel::isPartiallyChecked()
{
    int selectCount = selectedCount();
    return selectCount > 0 && selectCount <= m_data.count();
}

void QUTXOListModel::searchByString(const QString &searchText)
{
    m_searchText = searchText;
    refresh();
    emit countVisibleChanged();
}

void QUTXOListModel::searchByFilter(const QVariant &filter)
{
    mFilters = filter.toMap();
    DBG_INFO << mFilters;
    int sortOrder = mFilters["sortOrder"].toInt();
    if (sortOrder == 0) {
        sortOrder = Qt::DescendingOrder;
    } else {
        sortOrder = Qt::AscendingOrder;
    }

    bool sortByCoinAge = mFilters["sortByCoinAge"].toBool();
    AppModel::instance()->walletInfo()->setSortByCoinAge(sortByCoinAge);
    if(AppModel::instance()->walletInfo()->sortByCoinAge()){
        requestSort(utxo_blocktime_role, sortOrder);
    }
    else {
        requestSort(utxo_amount_role, sortOrder);
    }
    emit countVisibleChanged();
}

bool QUTXOListModel::hasResultFiltering(int row) const
{
    QMap<QString, QVariant> maps = mFilters;
    auto data = m_data[row];
    QString strMaxAmount = maps["maxAmount"].toString();
    QString strMinAmount = maps["minAmount"].toString();
    quint64 minAmount = 0;
    quint64 maxAmount = 0;
    bool isCurrency = maps["isCurrency"].toBool();
    filter_t hasMount;
    quint64 amount = data->amountSats();
    if (isCurrency) {
        minAmount = strMinAmount.isEmpty() ? std::numeric_limits<quint64>::min() : qUtils::QAmountFromCurrency(strMinAmount);
        maxAmount = strMaxAmount.isEmpty() ? std::numeric_limits<quint64>::max() : qUtils::QAmountFromCurrency(strMaxAmount);
    }
    else if (AppSetting::instance()->unit() == (int)AppSetting::Unit::SATOSHI) {
        minAmount = strMinAmount.isEmpty() ? std::numeric_limits<quint64>::min() : static_cast<quint64>(strMinAmount.toDouble());
        maxAmount = strMaxAmount.isEmpty() ? std::numeric_limits<quint64>::max() : static_cast<quint64>(strMaxAmount.toDouble());
    }
    else {
        minAmount = strMinAmount.isEmpty() ? std::numeric_limits<quint64>::min() : qUtils::QAmountFromValue(strMinAmount);
        maxAmount = strMaxAmount.isEmpty() ? std::numeric_limits<quint64>::max() : qUtils::QAmountFromValue(strMaxAmount);
    }
    hasMount.m_hasFilter = !strMinAmount.isEmpty() || !strMaxAmount.isEmpty(); // have input value
    hasMount.m_isValid = ((maxAmount >= amount && minAmount <= amount) || maxAmount == amount || minAmount == amount) && amount != 0;

    bool isLocked = maps["isLocked"].toBool();
    filter_t hasLocked = {isLocked && data->isLocked(), isLocked};
    bool isUnLocked = maps["isUnLocked"].toBool();
    filter_t hasUnLocked = {isUnLocked && !data->isLocked(), isUnLocked};

    QString fromDate = maps["fromDate"].toString();
    QString toDate = maps["toDate"].toString();
    long int fromDate_int = qUtils::GetTimeSecond(fromDate);
    long int toDate_int = qUtils::GetTimeSecond(toDate);
    filter_t hasDate = {fromDate_int < data->blocktime() && toDate_int > data->blocktime(), !(fromDate.isEmpty() && toDate.isEmpty())};
    filter_t hasTag;
    filter_t hasCollection;
    if (auto w = AppModel::instance()->walletListPtr()->getWalletById(m_wallet_id)) {
        auto coinTags = w->coinTags();
        auto coinCollections = w->coinCollections();
        hasTag.m_hasFilter = coinTags->existChecked();
        hasCollection.m_hasFilter = coinCollections->existChecked();

        auto dataTags = data->coinTags();
        for (auto tag : dataTags->fullList()) {
            if (coinTags->isChecked(tag.get_id())) {
                if (dataTags->hasTag(QString::fromStdString(tag.get_name()))) {
                    hasTag.m_isValid = true;
                    break;
                }
            }
        }
        auto dataCollections = data->coinCollections();
        for (auto collection : dataCollections->fullList()) {
            if (coinCollections->isChecked(collection.get_id())) {
                if(dataCollections->hasCollection(QString::fromStdString(collection.get_name()))) {
                    hasCollection.m_isValid = true;
                    break;
                }
            }
        }
    }
    bool ret = hasMount.result() && hasLocked.result() && hasUnLocked.result() && hasDate.result() && hasTag.result() && hasCollection.result();
    return ret;
}

void QUTXOListModel::clearAllFilter()
{
    mFilters.clear();
    refresh();
    emit countVisibleChanged();
}

void QUTXOListModel::refresh()
{
    beginResetModel();
    endResetModel();
}

int QUTXOListModel::countVisible()
{
    int countVisible = 0;
    auto visible = [this](int index) ->bool {
        if (!m_searchText.isEmpty()) {
            return m_data[index]->hasResultSearching(m_searchText);
        } else if (!mFilters.isEmpty()){
            return hasResultFiltering(index);
        } else {
            return true;
        }
    };
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && visible(i)){
            countVisible++;
        }
    }
    return countVisible;
}

bool QUTXOListModel::isFiltered()
{
    return !mFilters.isEmpty();
}

QVariant QUTXOListModel::filter()
{
    return QVariant::fromValue(mFilters);
}

QString QUTXOListModel::amountDisplay()
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(amountSats());
    }
    else{
        return amountBTC();
    }
}

QString QUTXOListModel::amountCurrency()
{
    return qUtils::currencyLocale(amountSats());
}

QString QUTXOListModel::amountBTC()
{
    return qUtils::QValueFromAmount(amountSats());
}

qint64 QUTXOListModel::amountSats()
{
    qint64 total = 0;
    foreach (QUTXOPtr it, m_data) {
        if(it.data()->selected()){
            total += it.data()->amountSats();
        }
    }
    return total;
}

QString QUTXOListModel::totalCurrency()
{
    return qUtils::currencyLocale(totalSats());
}

QString QUTXOListModel::totalBTC()
{
    return qUtils::QValueFromAmount(totalSats());
}

qint64 QUTXOListModel::totalSats()
{
    qint64 total = 0;
    foreach (QUTXOPtr it, m_data) {
        total += it.data()->amountSats();
    }
    return total;
}

QString QUTXOListModel::totalDisplay()
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(totalSats());
    }
    else{
        return qUtils::QValueFromAmount(totalSats());
    }
}

int QUTXOListModel::count() const
{
    return rowCount();
}

QList<QUTXOPtr> QUTXOListModel::fullList() const
{
    return m_data;
}

void QUTXOListModel::clearAll()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void QUTXOListModel::selectAll(bool select)
{
    beginResetModel();
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data()) {
            m_data[i]->setSelected(select);
        }
    }
    endResetModel();
    emit selectedCountChanged();
    emit amountChanged();
}

QString QUTXOListModel::spendableDisplay()
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(spendableSats());
    }
    else{
        return spendableBTC();
    }
}

qint64 QUTXOListModel::spendableSats()
{
    foreach (QUTXOPtr it, m_data) {
        DBG_INFO << it.data()->isLocked() << it.data()->amountSats();
        if(!it.data()->isLocked()){
            m_spendableSats += it.data()->amountSats();
        }
    }
    return m_spendableSats;
}

QString QUTXOListModel::spendableBTC()
{
    return qUtils::QValueFromAmount(spendableSats());
}

QString QUTXOListModel::spendableCurency()
{
    return qUtils::currencyLocale(spendableSats());
}

void QUTXOListModel::setSpendableSats(qint64 sats)
{
    if (m_spendableSats != sats) {
        m_spendableSats = sats;
        emit spendableChanged();
    }
}

QVariantMap QUTXOListModel::timelockInfo()
{
    QVariantMap ret;
    qint64 minValue = std::numeric_limits<qlonglong>::max();
    for (int i = 0; i < m_data.count(); i++) {
        if(auto utxo = m_data.at(i).data()) {
            QVariantList timelocklist = utxo->timelocklist();
            for (const auto &item : timelocklist) {
                QVariantMap map = item.toMap();
                if (map.contains("valueRemainingNumeric") && map.contains("valueNode")) {
                    qint64 value = map["valueRemainingNumeric"].toLongLong();
                    QString valueNode = map["valueNode"].toString();
                    if (value > 0 && (value < minValue) && (valueNode != "")) {
                        minValue = value;
                        ret = map;
                    }
                }
            }
        }
    }
    return ret;
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

bool sortbyBlocktimeAscending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return v1.data()->blocktime() < v2.data()->blocktime();
}

bool sortbyBlocktimeDescending(const QUTXOPtr &v1, const QUTXOPtr &v2)
{
    return v1.data()->blocktime() > v2.data()->blocktime();
}
