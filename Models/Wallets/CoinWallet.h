#ifndef COINWALLET_H
#define COINWALLET_H

#include <QObject>
#include "ReplacedWallet.h"

class CoinWallet : public ReplacedWallet
{
    Q_OBJECT
    Q_PROPERTY(bool                 isViewCoinShow                  READ isViewCoinShow             WRITE setIsViewCoinShow     NOTIFY isViewCoinShowChanged)
    Q_PROPERTY(QString              coinFlow                        READ coinFlow                   WRITE setCoinFlow           NOTIFY coinFlowChanged)
    Q_PROPERTY(QUTXOListModel*      utxoList                        READ utxoList                                               NOTIFY utxoListChanged)
    Q_PROPERTY(UTXO*                utxoInfo                        READ utxoInfo                                               NOTIFY utxoInfoChanged)
    Q_PROPERTY(QUTXOListModel*      utxoFilterTag                   READ utxoFilterTag                                          NOTIFY utxoFilterTagChanged)
    Q_PROPERTY(QUTXOListModel*      utxoFilterCollection            READ utxoFilterCollection                                   NOTIFY utxoFilterCollectionChanged)
    Q_PROPERTY(QCoinCollectionsModel* coinCollections               READ coinCollections                                        NOTIFY coinCollectionsChanged)
    Q_PROPERTY(QCoinTagsModel*      coinTags                        READ coinTags                                               NOTIFY coinTagsChanged)
    Q_PROPERTY(QString              searchText                      READ searchText                 WRITE setSearchText         NOTIFY searchTextChanged)
    Q_PROPERTY(QUTXOListModel*      utxoListLocked                  READ utxoListLocked                                         NOTIFY utxoListChanged)
    Q_PROPERTY(QList<QUTXOListModel *> ancestryList                 READ ancestryList                                           NOTIFY ancestryListChanged)
    Q_PROPERTY(QCoinTagsModel*      coinTagsFilter                  READ coinTagsFilter                                         NOTIFY coinTagsFilterChanged)
    Q_PROPERTY(bool                 sortByCoinAge                   READ sortByCoinAge              WRITE setSortByCoinAge      NOTIFY sortCoinByAgeChanged)

public:
    CoinWallet(const nunchuk::Wallet &w);
    ~CoinWallet() override = default;
    bool isViewCoinShow() const;
    void setIsViewCoinShow(bool newIsViewCoinShow);

    QString coinFlow() const;
    void setCoinFlow(const QString &newCoinFlow);

    QUTXOListModel *utxoList();
    QUTXOListModelPtr utxoListPtr();
    void setUtxoList(const QUTXOListModelPtr &utxoList);

    UTXO *utxoInfo() const;
    QUTXOPtr utxoInfoPtr() const;
    void setUtxoInfo(const QUTXOPtr &utxoInfo);
    void setUtxoInfo(const int index);
    void setUtxoInfo(const QString& tx_id);

    void RequestGetCoins();
    void RequestUpdateCoins();

    void GetCoinControlFromServer();

    QUTXOListModelPtr GetCoinByTagId(const int tag_id);
    QUTXOListModelPtr GetCoinByCollectionId(const int collection_id);

    //Wallet scope
    bool UpdateCoinTag(const nunchuk::CoinTag& tag, bool force = true);
    bool DeleteCoinTag(int tag_id, bool force = true);
    bool AddNewCoinTag(const QString& tag_name, const QString& tag_color, bool force = true);

    // Coin scope
    bool AddToCoinTag(int tag_id, const QString& tx_id, int vout, bool force = true);
    bool UpdateCoinMemo(const QString& tx_id, int vout, const QString& memo, bool force = true);

    bool LockCoin(const QString& tx_id, int vout, bool force = true);
    void LockCoins(const std::vector<nunchuk::UnspentOutput>& coins);
    bool UnlockCoin(const QString& tx_id, int vout, bool force = true);
    void UnlockCoins(const std::vector<nunchuk::UnspentOutput>& coins);

    bool RemoveFromCoinTag(int tag_id, const QString& tx_id, int vout, bool force = true);
    bool RemoveFromCoinCollection(int collection_id, const QString& tx_id, int vout, bool force = true);
    bool UpdateCoinCollection(const nunchuk::CoinCollection& collection, bool apply_to_existing_coins = true);
    bool CreateCoinCollection(const QMap<QString, QVariant> &maps);
    bool AddToCoinCollection(int collection_id, const QString &tx_id, int vout, bool force);
    bool DeleteCoinCollection(int collection_id, bool force = true);

    bool RequestCoinScreen(const QVariant &coinFlow);
    QUTXOListModel *utxoFilterTag() const;
    void setUtxoFilterTag(const int tag_id);
    QUTXOListModel *utxoFilterCollection() const;
    void setUtxoFilterCollection(const int collection_id);

    void RequestForAllCoins(const QVariant &act);
    QCoinCollectionsModel* coinCollections() const;
    QCoinTagsModel* coinTags() const;
    QCoinTagsModel* coinTagsFilter() const;

    QString searchText() const;
    void setSearchText(const QString &newSearchText);

    QUTXOListModel *utxoListLocked() const;
    QUTXOListModelPtr utxoListLockedPtr() const;
    void setUtxoListLocked(const QUTXOListModelPtr &utxoList);
    void RequestLockedCoins();
    void RequestForLockedAllCoins(const QVariant &act);

    void RequestGetCoinAncestry();
    QList<QUTXOListModel *> ancestryList() const;

    bool ImportCoinControlData(const QString& filePath);
    bool ExportCoinControlData(const QString& filePath);
    bool ExportBIP329(const QString& filePath);
    bool ImportBIP329(const QString& filePath);
    bool CreateDraftTransaction(int successEvtID, const QVariant &msg);
    bool UpdateDraftTransaction(const QVariant &msg);
    bool UpdateDraftRBFransaction(const QVariant &msg);
    void setReuse(bool newReuse);
    void RequestSyncSelectCoinForMakeTransaction(const QVariant &msg);

    QUTXOListModelPtr GetUtxoListSelected();
    bool AssignTagsToTxChange();
    QList<int> tagsInTxAssigned() const;

    bool sortByCoinAge();
    void setSortByCoinAge(const int data);
public slots:
    void requestForAllCoins(const QVariant &act);
    void requestForLockedAllCoins(const QVariant &act);
    void requestImportCoinControlData(const QString& filePath);
    void requestExportCoinControlData(const QString& filePath);
    void requestExportBIP329(const QString& filePath);
    void requestImportBIP329(const QString& filePath);
    void requestConsolidateMakeTransaction(const QVariant &msg);
    void requestSyncSelectCoinForMakeTransaction(const QVariant &msg);
private:
    void GetCoinControlUserWallet();
    void GetCoinControlGroupWallet();
    void UpdateCoinControlUserWallet();
    void UpdateCoinControlGroupWallet();
signals:
    void isViewCoinShowChanged();
    void coinFlowChanged();
    void utxoListChanged();
    void utxoInfoChanged();
    void utxoFilterTagChanged();
    void utxoFilterCollectionChanged();
    void coinCollectionsChanged();
    void coinTagsChanged();
    void searchTextChanged();
    void updateCollectionNameChanged(bool isError);
    void ancestryListChanged();
    void requestCreateTransaction(const QString& address);
    void updateTagNameChanged(bool isError);
    void coinTagsFilterChanged();
    void sortCoinByAgeChanged();

private:
    QUTXOListModelPtr m_utxoList;
    QUTXOListModelPtr m_utxoListLocked;
    QUTXOPtr          m_utxoInfo;
    QCoinCollectionsModelPtr m_coinCollections;
    QCoinTagsModelPtr m_coinTags;
    QCoinTagsModelPtr m_coinTagsFilter;
    bool m_isViewCoinShow {false};
    QString m_coinFlow {};
    QUTXOListModelPtr m_utxoFilterTag;
    QUTXOListModelPtr m_utxoFilterCollection;
    QString m_searchText {};
    QList<QUTXOListModel *> m_ancestryList;
    QString m_txid_before_enter_spent_coin {};
    bool m_reuse {false};
    QList<int> m_tagsInTxAssigned {};
    QString m_previousViewCollection {};

protected:
    bool m_sortByCoinAge {false};
};

#endif // COINWALLET_H
