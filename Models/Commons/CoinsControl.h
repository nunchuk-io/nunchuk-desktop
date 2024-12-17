#ifndef COINSCONTROL_H
#define COINSCONTROL_H
#include <QObject>
#include "nunchuk.h"
#include "UTXOModel.h"

class CoinsControl
{
public:
    void init();
    bool isViewCoinShow() const;
    void setIsViewCoinShow(bool newIsViewCoinShow);

    QString coinFlow() const;
    void setCoinFlow(const QString &newCoinFlow);

    QUTXOListModel *utxoList() const;
    QUTXOListModelPtr utxoListPtr() const;
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
    bool CreateDraftTransaction(int successScreenID, const QVariant &msg);
    bool UpdateDraftTransaction(const QVariant &msg);
    void setReuse(bool newReuse);
    void RequestSyncSelectCoinForMakeTransaction(const QVariant &msg);

    QUTXOListModelPtr GetUtxoListSelected();
    bool AssignTagsToTxChange();
    QList<int> tagsInTxAssigned() const;

private:
    void GetCoinControlUserWallet();
    void GetCoinControlGroupWallet();
    void UpdateCoinControlUserWallet();
    void UpdateCoinControlGroupWallet();

signals:
    virtual void isViewCoinShowChanged() = 0;
    virtual void coinFlowChanged() = 0;
    virtual void utxoListChanged() = 0;
    virtual void utxoInfoChanged() = 0;
    virtual void utxoFilterTagChanged() = 0;
    virtual void utxoFilterCollectionChanged() = 0;
    virtual void coinCollectionsChanged() = 0;
    virtual void coinTagsChanged() = 0;
    virtual void searchTextChanged() = 0;
    virtual void updateCollectionNameChanged(bool isError) = 0;
    virtual void ancestryListChanged() = 0;
    virtual void requestCreateTransaction(const QString& address) = 0;
    virtual void updateTagNameChanged(bool isError) = 0;
    virtual void coinTagsFilterChanged() = 0;
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
    QMap<QString, QVariant> m_draftTransactionInput;
    QList<int> m_tagsInTxAssigned {};
    QString m_previousViewCollection {};
};

#endif // COINSCONTROL_H
