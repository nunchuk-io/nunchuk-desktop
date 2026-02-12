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
#include "nunchuk.h"
#include "TypeDefine.h"
#include <QMutex>

class QCoinTagsModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QVariant tag READ tag NOTIFY tagChanged)
    Q_PROPERTY(bool existChecked        READ existChecked       NOTIFY existCheckedChanged)
    Q_PROPERTY(bool isChecked           READ isChecked          NOTIFY existCheckedChanged)
    Q_PROPERTY(bool isPartiallyChecked  READ isPartiallyChecked NOTIFY existCheckedChanged)
    Q_PROPERTY(int selectedCount        READ selectedCount      NOTIFY existCheckedChanged)
public:
    QCoinTagsModel();
    ~QCoinTagsModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int,QByteArray> roleNames() const;
    enum TagRoles {
        tag_id_role,
        tag_name_role,
        tag_color_role,
        tag_count_role,
        tag_checked_role
    };
    Q_INVOKABLE QVariant get(int row);
    void addTag(const nunchuk::CoinTag data);
    void refreshTags(const QList<int> tags, const QString &wallet_id, bool isUtxo);
    int count() const;
    void clear();
    QVariant tag();
    void setTag(const int tag_id);
    nunchuk::CoinTag getCoinTagById(const int tag_id);
    Q_INVOKABLE bool containColor(const QString &color);

    void increase(const int tag_id, QString wallet_id);
    int get_count(const int tag_id) const;

    bool isChecked();
    bool isPartiallyChecked();
    bool isChecked(const int tag_id) const;
    void setChecked(const QList<int> tags);
    void setChecked(const int tag_id, bool checked);
    void refresh();
    QList<int> tagsSelected() const;
    bool existChecked() const;
    QList<nunchuk::CoinTag> fullList() const;
    QStringList fullTags() const;

    bool hasTag(const QString &tag);
    int selectedCount() const;
signals:
    void countChanged();
    void tagChanged();
    void existCheckedChanged();
public slots:
    void cancelSelected();
    void storeSelected();
    void selectAll(bool select = true);
private:
    typedef struct tag_t {
        int m_id {0};
        int m_count {0};
    } tag_t;
    QList<int> m_tags_checked;
    QList<int> m_store_filter_tags_checked;
    QMap<int, tag_t> m_tags_count;
    int m_tag_id {-1};
    QList<nunchuk::CoinTag> m_data {};
};
typedef OurSharedPointer<QCoinTagsModel> QCoinTagsModelPtr;

// Coin collections
class QCoinCollectionsModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QVariant collection READ collection NOTIFY collectionChanged)
    Q_PROPERTY(bool existChecked        READ existChecked           NOTIFY existCheckedChanged)
    Q_PROPERTY(bool isChecked           READ isChecked              NOTIFY existCheckedChanged)
    Q_PROPERTY(bool isPartiallyChecked  READ isPartiallyChecked     NOTIFY existCheckedChanged)
    Q_PROPERTY(int selectedCount        READ selectedCount          NOTIFY existCheckedChanged)
public:
    QCoinCollectionsModel();
    ~QCoinCollectionsModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int,QByteArray> roleNames() const;
    enum CollectionRoles {
        collection_id_role,
        collection_name_role,
        collection_add_new_coin_role,
        collection_auto_lock_role,
        collection_add_tags_role,
        collection_count_role,
        collection_checked_role
    };
    Q_INVOKABLE QVariant get(int row);
    void addCollection(const nunchuk::CoinCollection data);
    void refreshCollections(const QList<int> collections, const QString &wallet_id, bool isUtxo);
    int count() const;
    void clear();
    QVariant collection();
    void setCollection(const int collection_id);

    void increase(const int collection_id, QString wallet_id);
    int get_count(const int collection_id) const;
    nunchuk::CoinCollection getCoinCollectionById(const int collection_id);

    bool isChecked();
    bool isPartiallyChecked();
    bool isChecked(const int collection_id) const;
    void setChecked(const QList<int> collections);
    void setChecked(const int collection_id, bool checked);
    void refresh();
    bool existChecked() const;
    QList<nunchuk::CoinCollection> fullList() const;

    bool hasCollection(const QString &searchText);
    int selectedCount() const;
signals:
    void countChanged();
    void collectionChanged();
    void existCheckedChanged();
public slots:
    void cancelSelected();
    void storeSelected();
    void selectAll(bool select = true);
private:
    typedef struct collection_t {
        int m_id {0};
        int m_count {0};
    } collection_t;
    QList<int> m_collections_checked;
    QList<int> m_store_filter_collections_checked;
    QMap<int, collection_t> m_collections_count;
    int m_collection_id {-1};
    QList<nunchuk::CoinCollection> m_data;
};
typedef OurSharedPointer<QCoinCollectionsModel> QCoinCollectionsModelPtr;

class UTXO  : public QObject{

    Q_OBJECT
    Q_PROPERTY(QString                  txid                            READ txid                                           NOTIFY unspentOutputChanged)
    Q_PROPERTY(int                      vout                            READ vout                                           NOTIFY unspentOutputChanged)
    Q_PROPERTY(QString                  address                         READ address                                        NOTIFY unspentOutputChanged)
    Q_PROPERTY(QString                  amount                          READ amountDisplay                                  NOTIFY unspentOutputChanged)
    Q_PROPERTY(QString                  amountCurrency                  READ amountCurrency                                 NOTIFY unspentOutputChanged)
    Q_PROPERTY(QString                  scriptPublickey                 READ scriptPublickey                                NOTIFY unspentOutputChanged)
    Q_PROPERTY(int                      height                          READ height                                         NOTIFY unspentOutputChanged)
    Q_PROPERTY(QString                  memo                            READ memo                                           NOTIFY unspentOutputChanged)
    Q_PROPERTY(int                      status                          READ status                                         NOTIFY unspentOutputChanged)
    Q_PROPERTY(bool                     isChange                        READ isChange                                       NOTIFY unspentOutputChanged)
    Q_PROPERTY(bool                     isLocked                        READ isLocked                                       NOTIFY unspentOutputChanged)
    Q_PROPERTY(QString                  blocktimeDisplay                READ blocktimeDisplayTwo                            NOTIFY unspentOutputChanged)
    Q_PROPERTY(QString                  scheduleTimeDisplay             READ scheduleTimeDisplay                            NOTIFY unspentOutputChanged)
    Q_PROPERTY(bool                     selected                        READ selected                   WRITE setSelected   NOTIFY selectedChanged)
    Q_PROPERTY(QCoinTagsModel*          coinTags                        READ coinTags                                       NOTIFY coinTagsChanged)
    Q_PROPERTY(QCoinCollectionsModel*   coinCollections                 READ coinCollections                                NOTIFY coinCollectionsChanged)
    Q_PROPERTY(Transaction*             transaction                     READ transaction                                    NOTIFY coinCollectionsChanged)
    Q_PROPERTY(QString                  outpoint                        READ outpoint                                       CONSTANT)
    Q_PROPERTY(QVariant                 outgoing                        READ outgoing                                       CONSTANT)

    Q_PROPERTY(int                      timelockbase                    READ timelockbase                                   NOTIFY unspentOutputChanged)
    Q_PROPERTY(QVariantList             timelocklist                    READ timelocklist                                   NOTIFY unspentOutputChanged)
    Q_PROPERTY(int                      timelockCount                   READ timelockCount                                  NOTIFY unspentOutputChanged)
    Q_PROPERTY(bool                     timeLocked                      READ timeLocked                                     NOTIFY timeLockedChanged)

public:
    UTXO(QString wallet_id, nunchuk::UnspentOutput data);
    UTXO(QString wallet_id);
    ~UTXO();

    QString txid() const;
    void setTxid(const QString &txid);

    int vout() const;
    void setVout(int vout);

    QString address() const;
    void setAddress(const QString &address);

    QString amountDisplay() const;
    QString amountBTC() const;
    qint64  amountSats() const;
    QString amountCurrency() const;
    void setAmount(const qint64 amount);

    int confirmedCount();
    int height() const;
    void setHeight(int height);

    bool selected() const;
    void setSelected(bool selected);

    QString scriptPublickey() const;

    QString memo() const;
    void setMemo(const QString &memo);

    QString outpoint() const;
    QVariant outgoing() const;

    int status() const;
    void setStatus(int status);

    bool isChange() const;
    void setIsChange(bool change);

    bool isLocked() const;
    void setIsLocked(bool locked);

    QList<int /*id*/> const getTags() const;
    void setTags(const QList<int /*id*/> &tags);

    QList<int /*id*/> const getCollections() const;
    void setCollections(const QList<int /*id*/> &collections);

    time_t blocktime() const;
    QString blocktimeDisplay();
    QString blocktimeDisplayTwo();
    void setBlocktime(const time_t blocktime);

    time_t scheduleTime() const;
    QString scheduleTimeDisplay();
    void setScheduleTime(const time_t scheduleTime);

    nunchuk::UnspentOutput getUnspentOutput() const;
    void setUnspentOutput(const nunchuk::UnspentOutput &data);

    QCoinTagsModel* coinTags() const;
    QCoinTagsModelPtr coinTagsPtr() const;
    void setCoinTags(QCoinTagsModelPtr coinTags);

    QCoinCollectionsModel* coinCollections() const;
    QCoinCollectionsModelPtr coinCollectionsPtr() const;
    void setCoinCollections(QCoinCollectionsModelPtr coinCollections);

    Transaction* transaction();
    QTransactionPtr transactionPtr();

    bool hasTag(const QString &searchText);
    bool hasCollection(const QString &searchText);
    bool hasAmount(const QString &searchText);
    bool hasAddress(const QString &searchText);
    bool hasMemo(const QString &searchText);
    bool hasBlocktime(const QString &searchText);
    bool hasTransactionID(const QString &searchText);

    bool hasResultSearching(const QString &searchText);

    int lockedIndex();
    int timelockbase();
    int timelockCount();
    QVariantList timelocklist();

    bool timeLocked() const;
    void setTimeLocked(bool newTimeLocked);

private:
    bool m_selected;
    nunchuk::UnspentOutput mUnspentOutputOrigin;
    QCoinTagsModelPtr m_coinTags;
    QCoinCollectionsModelPtr m_coinCollections;
    QTransactionPtr m_trans;
    QString m_wallet_id;
    bool    m_timeLocked {false};


signals:
    void selectedChanged();
    void unspentOutputChanged();
    void coinTagsChanged();
    void coinCollectionsChanged();
    void timeLockedChanged();
};
typedef OurSharedPointer<UTXO> QUTXOPtr;

class QUTXOListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int      selectedCount           READ selectedCount                                         NOTIFY selectedCountChanged)
    Q_PROPERTY(int      count                   READ count                                                 NOTIFY countChanged)
    Q_PROPERTY(qint64   amountSats              READ amountSats                                            NOTIFY amountChanged)
    Q_PROPERTY(QString  amountDisplay           READ amountDisplay                                         NOTIFY amountChanged)
    Q_PROPERTY(QString  amountCurrency          READ amountCurrency                                        NOTIFY amountChanged)
    Q_PROPERTY(bool     isChecked               READ isChecked                                             NOTIFY selectedCountChanged)
    Q_PROPERTY(bool     isPartiallyChecked      READ isPartiallyChecked                                    NOTIFY selectedCountChanged)
    Q_PROPERTY(bool     isUnChecked             READ isUnChecked                                           NOTIFY selectedCountChanged)
    Q_PROPERTY(int      countVisible            READ countVisible                                          NOTIFY countVisibleChanged)
    Q_PROPERTY(int      lockedCount             READ lockedCount                                           NOTIFY selectedCountChanged)
    Q_PROPERTY(int      selectedLockedCount     READ selectedLockedCount                                   NOTIFY selectedCountChanged)
    Q_PROPERTY(bool     isFiltered              READ isFiltered                                            NOTIFY countVisibleChanged)
    Q_PROPERTY(QVariant filter                  READ filter                                                NOTIFY countVisibleChanged)
    Q_PROPERTY(QString  totalDisplay            READ totalDisplay                                          CONSTANT)
    Q_PROPERTY(QString  totalCurrency           READ totalCurrency                                         CONSTANT)

    // spendable
    Q_PROPERTY(QString   spendableDisplay       READ spendableDisplay                                      NOTIFY spendableChanged)
    Q_PROPERTY(qint64    spendableSats          READ spendableSats              WRITE setSpendableSats     NOTIFY spendableChanged)
    Q_PROPERTY(QString   spendableBTC           READ spendableBTC                                          NOTIFY spendableChanged)
    Q_PROPERTY(QString   spendableCurency       READ spendableCurency                                      NOTIFY spendableChanged)

public:
    QUTXOListModel(QString wallet_id);
    ~QUTXOListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int,QByteArray> roleNames() const;
    void setWalletId(const QString &wallet_id);
    void addUTXO(const nunchuk::UnspentOutput data);
    QUTXOPtr getUTXOByIndex(const int index);
    QUTXOPtr getUTXOByTxid(const QString &txid);
    void updateSelected(const QString &txid, const int vout);
    qint64 getAmount(const QString &txid, const int vout);
    void requestSort(int role, int order);
    void notifyUnitChanged();
    int selectedCount();
    int lockedCount();
    int selectedLockedCount();
    bool isChecked();
    bool isUnChecked();
    bool isPartiallyChecked();
    void searchByString(const QString &searchText);
    void searchByFilter(const QVariant &filter);
    bool hasResultFiltering(int row) const;
    void clearAllFilter();
    void refresh();
    int countVisible();
    bool isFiltered();
    QVariant filter();
    enum UTXORoles {
        utxo_txid_role,
        utxo_vout_role,
        utxo_address_role,
        utxo_amount_role,
        utxo_height_role,
        utxo_selected_role,
        utxo_confirmed_role,
        utxo_memo_role,
        utxo_tags_role,
        utxo_collections_role,
        utxo_amount_currency_role,
        utxo_coin_is_locked,
        utxo_coin_is_scheduled,
        utxo_blocktime_role,
        utxo_coin_visible_role,
        utxo_outgoing_label_role,
        utxo_outgoing_color_role,
        utxo_coin_is_change_role,
        utxo_coin_timelockbase_role,
        utxo_coin_timelocklist_role,
        utxo_coin_timelockCount_role,
        utxo_coin_timeLocked_role,
    };

    qint64 amountSats();
    QString amountDisplay();
    QString amountCurrency();
    QString amountBTC();
    QString totalCurrency();
    QString totalBTC();
    qint64 totalSats();
    QString totalDisplay();
    int count() const;
    QList<QUTXOPtr> fullList() const;
    void clearAll();

    QString spendableDisplay();
    qint64  spendableSats();
    QString spendableBTC();
    QString spendableCurency();
    void setSpendableSats(qint64 sats);
    QVariantMap timelockInfo();

signals:
    void amountChanged();
    void countChanged();
    void selectedCountChanged();
    void countVisibleChanged();
    void spendableChanged();

public slots:
    void selectAll(bool select = true);

private:
    QString m_wallet_id;
    QList<QUTXOPtr> m_data;
    QString m_searchText {};
    QMap<QString, QVariant> mFilters {};
    typedef struct Filter_t {
        bool m_isValid {false};
        bool m_hasFilter {false};
        bool result() {
            if (m_hasFilter) {
                return m_isValid;
            } else {
                return true;
            }
        }
    } filter_t;

    qint64  m_spendableSats {0};
};
typedef OurSharedPointer<QUTXOListModel> QUTXOListModelPtr;

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

// Sort block time
bool sortbyBlocktimeAscending(const QUTXOPtr &v1, const QUTXOPtr &v2);
bool sortbyBlocktimeDescending(const QUTXOPtr &v1, const QUTXOPtr &v2);

#endif // UNSPENTOUTPUTMODEL_H
