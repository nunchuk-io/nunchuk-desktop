#include "CoinsControl.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"
#include "Servers/Byzantine.h"
#include "Servers/Draco.h"
#include "QEventProcessor.h"

void CoinsControl::init()
{
    m_utxoList = QUTXOListModelPtr(new QUTXOListModel((dynamic_cast<Wallet*>(this))->id()));
    m_utxoInfo = QUTXOPtr(new UTXO((dynamic_cast<Wallet*>(this))->id()));
    m_coinCollections = QCoinCollectionsModelPtr(new QCoinCollectionsModel());
    m_coinTags = QCoinTagsModelPtr(new QCoinTagsModel());
    m_coinTagsFilter = QCoinTagsModelPtr(new QCoinTagsModel());
}

bool CoinsControl::isViewCoinShow() const
{
    return m_isViewCoinShow;
}

void CoinsControl::setIsViewCoinShow(bool newIsViewCoinShow)
{
    if(m_isViewCoinShow == newIsViewCoinShow){
        return;
    }
    m_isViewCoinShow = newIsViewCoinShow;
    if (m_isViewCoinShow) {
        RequestGetCoins();
        utxoList()->selectAll(false);
    }
    emit isViewCoinShowChanged();
}

QString CoinsControl::coinFlow() const
{
    return m_coinFlow;
}

void CoinsControl::setCoinFlow(const QString &newCoinFlow)
{
    m_coinFlow = newCoinFlow;
    emit coinFlowChanged();
}

QUTXOListModel *CoinsControl::utxoList() const
{
    return m_utxoList.data();
}

QUTXOListModelPtr CoinsControl::utxoListPtr() const
{
    return m_utxoList;
}

void CoinsControl::setUtxoList(const QUTXOListModelPtr &utxoList)
{
    m_utxoList = utxoList;
    if(m_utxoList.data()){
        m_utxoList.data()->requestSort(QUTXOListModel::UTXORoles::utxo_amount_role,  Qt::DescendingOrder);
    }
    emit utxoListChanged();
}

UTXO *CoinsControl::utxoInfo() const
{
    return m_utxoInfo.data();
}

QUTXOPtr CoinsControl::utxoInfoPtr() const
{
    return m_utxoInfo;
}

void CoinsControl::setUtxoInfo(const QUTXOPtr &utxoInfo)
{
    m_utxoInfo = utxoInfo;
    emit utxoInfoChanged();
}

void CoinsControl::setUtxoInfo(const int index)
{
    QUTXOPtr info = utxoList()->getUTXOByIndex(index);
    if(info){
        setUtxoInfo(info);
    }
}

void CoinsControl::setUtxoInfo(const QString &tx_id)
{
    QUTXOPtr info = utxoList()->getUTXOByTxid(tx_id);
    if(info){
        setUtxoInfo(info);
    }
}

void CoinsControl::RequestGetCoins()
{
    // dynamic cast this to Wallet
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QUTXOListModelPtr utxos = bridge::nunchukGetUnspentOutputs(wallet_id);
    if(utxos){
        if (m_reuse) {
            for (auto old_utxo : m_utxoList->fullList()) {
                for (auto new_utxo : utxos->fullList()) {
                    if (old_utxo->txid() == new_utxo->txid()) {
                        new_utxo->setSelected(old_utxo->selected());
                    }
                }
            }
            setReuse(false);
        }
        setUtxoList(utxos);
        if (!utxoInfoPtr().isNull()) {
            setUtxoInfo(utxoInfoPtr()->txid());
        }
    }
    if (!m_coinCollections.isNull()) {
        m_coinCollections->refreshCollections({}, wallet_id, false);
        emit coinCollectionsChanged();
    }
    if (!m_coinTags.isNull()) {
        m_coinTags->refreshTags({}, wallet_id, false);
        emit coinTagsChanged();
    }
}

void CoinsControl::RequestUpdateCoins()
{
    bool isReplaced = (dynamic_cast<Wallet*>(this))->isReplaced();
    bool isGroupWallet = (dynamic_cast<Wallet*>(this))->isGroupWallet();
    bool isUserWallet = (dynamic_cast<Wallet*>(this))->isUserWallet();

    DBG_INFO << "FIXME user:" << isUserWallet << " group:" << isGroupWallet;
    if(isReplaced){
        return;
    }
    if(isGroupWallet){
        UpdateCoinControlGroupWallet();
    }
    else if(isUserWallet){
        UpdateCoinControlUserWallet();
    }
    else{}
}

void CoinsControl::GetCoinControlFromServer()
{
    bool isReplaced = (dynamic_cast<Wallet*>(this))->isReplaced();
    bool isGroupWallet = (dynamic_cast<Wallet*>(this))->isGroupWallet();
    bool isUserWallet = (dynamic_cast<Wallet*>(this))->isUserWallet();

    DBG_INFO << "FIXME user:" << isUserWallet << " group:" << isGroupWallet;
    if(isReplaced){
        return;
    }
    if(isGroupWallet){
        GetCoinControlGroupWallet();
    }
    else if(isUserWallet){
        GetCoinControlUserWallet();
    }
    else{}
}

QUTXOListModelPtr CoinsControl::GetCoinByTagId(const int tag_id)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    return bridge::nunckGetCoinByTag(wallet_id, tag_id);
}

QUTXOListModelPtr CoinsControl::GetCoinByCollectionId(const int collection_id)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    return bridge::nunchukGetCoinInCollection(wallet_id, collection_id);
}

bool CoinsControl::UpdateCoinTag(const nunchuk::CoinTag &tag, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukUpdateCoinTag(wallet_id, tag);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::DeleteCoinTag(int tag_id, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukDeleteCoinTag(wallet_id, tag_id);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::AddNewCoinTag(const QString &tag_name, const QString &tag_color, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukCreateCoinTag(wallet_id, tag_name, tag_color);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::AddToCoinTag(int tag_id, const QString &tx_id, int vout, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukAddToCoinTag(wallet_id, tag_id, tx_id, vout);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::UpdateCoinMemo(const QString &tx_id, int vout, const QString &memo, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukUpdateCoinMemo(wallet_id, tx_id, vout, memo);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::LockCoin(const QString &tx_id, int vout, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukLockCoin(wallet_id, tx_id, vout);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
        AppModel::instance()->showToast(0, "Coin locked", EWARNING::WarningType::SUCCESS_MSG);
    }
    return ret;
}

void CoinsControl::LockCoins(const std::vector<nunchuk::UnspentOutput> &coins)
{
    // Lock one by one
    int count = 0;
    for (const auto &coin : coins) {
        if(!coin.is_locked()){
            bool ret = LockCoin(QString::fromStdString(coin.get_txid()), coin.get_vout(), false);
            if(ret) {
                count++;
            }
            else {
                DBG_INFO << "Could not lock coin:" << coin.get_txid() << coin.get_vout();
            }
        }
    }
    if(count > 0){
        RequestGetCoins();
        RequestUpdateCoins();
        QString message = QString("Coin%1 locked").arg(count > 1 ? "s" : "");
        AppModel::instance()->showToast(0, message, EWARNING::WarningType::SUCCESS_MSG);
    }
}

bool CoinsControl::UnlockCoin(const QString &tx_id, int vout, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukUnlockCoin(wallet_id, tx_id, vout);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
        AppModel::instance()->showToast(0, "Coin unlocked", EWARNING::WarningType::SUCCESS_MSG);
    }
    return ret;
}

void CoinsControl::UnlockCoins(const std::vector<nunchuk::UnspentOutput> &coins)
{
    // Unlock one by one
    int count = 0;
    for (const auto &coin : coins) {
        if(coin.is_locked()){
            bool ret = UnlockCoin(QString::fromStdString(coin.get_txid()), coin.get_vout(), false);
            if(ret){
                count++;
            }
            else{
                DBG_ERROR << "Could not unlock coin:" << coin.get_txid() << coin.get_vout();
            }
        }
    }
    if(count > 0){
        RequestGetCoins();
        RequestUpdateCoins();
        QString message = QString("Coin%1 unlocked").arg(count > 1 ? "s" : "");
        AppModel::instance()->showToast(0, message, EWARNING::WarningType::SUCCESS_MSG);
    }
}

bool CoinsControl::RemoveFromCoinTag(int tag_id, const QString &tx_id, int vout, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukRemoveFromCoinTag(wallet_id, tag_id, tx_id, vout);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::RemoveFromCoinCollection(int collection_id, const QString &tx_id, int vout, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukRemoveFromCoinCollection(wallet_id, collection_id, tx_id, vout);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::UpdateCoinCollection(const nunchuk::CoinCollection &collection, bool apply_to_existing_coins)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukUpdateCoinCollection(wallet_id, collection, apply_to_existing_coins);
    if(ret){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::CreateCoinCollection(const QMap<QString, QVariant> &maps)
{
    DBG_INFO << maps;
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QString coinName = maps["coinName"].toString();
    auto collection = bridge::nunchukCreateCoinCollection(wallet_id, coinName);
    if (collection.get_id() > 0) {
        bool isCheckedWithoutTags = maps["isCheckedCoinWithoutTags"].toBool();
        bool isCheckedWithTags = maps["isCheckedCoinWithTags"].toBool();
        bool isApplyExistingCoins = maps["isApplyExistingCoins"].toBool();
        bool isAutoLock = maps["isAutoLock"].toBool();
        if (isCheckedWithTags) {
            std:vector<int> tags;
            for (auto tag: m_coinTagsFilter->fullList()) {
                tags.push_back(tag.get_id());
            }
            collection.set_add_coins_with_tag(tags);
        }
        if (isCheckedWithoutTags) {
            for (auto utxo : m_utxoList->fullList()) {
                if (utxo->getTags().isEmpty()) {
                    AddToCoinCollection(collection.get_id(), utxo->txid(), utxo->vout(), false);
                }
            }
        }
        collection.set_add_new_coin(isCheckedWithoutTags);
        collection.set_auto_lock(isAutoLock);
        UpdateCoinCollection(collection, isApplyExistingCoins);
    }
    return collection.get_id() > 0;
}

bool CoinsControl::AddToCoinCollection(int collection_id, const QString &tx_id, int vout, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukAddToCoinCollection(wallet_id, collection_id, tx_id, vout);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::DeleteCoinCollection(int collection_id, bool force)
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    bool ret = bridge::nunchukDeleteCoinCollection(wallet_id, collection_id);
    if(ret && force){
        RequestGetCoins();
        RequestUpdateCoins();
    }
    return ret;
}

bool CoinsControl::RequestCoinScreen(const QVariant &msg)
{
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    if (type == "coin-details") {
        int index = maps["coin_index"].toInt();
        setUtxoInfo(index);
    }
    else if (type == "coin-filter-tag") {
        int tag_id = maps["coin_id"].toInt();
        m_coinTags->setTag(tag_id);
        setUtxoFilterTag(tag_id);
        DBG_INFO << "coin-filter-tag" << tag_id << utxoFilterTag()->count();
    }
    else if (type == "coin-filter-collection") {
        int collection_id = maps["coin_id"].toInt();
        m_coinCollections->setCollection(collection_id);
        setUtxoFilterCollection(collection_id);
        DBG_INFO << "coin-filter-collection" << collection_id << utxoFilterCollection()->count();
    }
    else if (type == "coin-lock-change") {
        if (utxoInfo()->isLocked()) {
            UnlockCoin(utxoInfo()->txid(), utxoInfo()->vout());
        } else {
            LockCoin(utxoInfo()->txid(), utxoInfo()->vout());
        }
        setUtxoInfo(utxoInfo()->txid());
        return true;
    }
    else if (type == "coin-tag-change-name") {
        int coinTagId = maps["coinTagId"].toInt();
        QString coinTagColor = maps["coinTagColor"].toString();
        QString coinName = maps["coinName"].toString();
        nunchuk::CoinTag tag(coinTagId, coinName.toStdString(), coinTagColor.toStdString());
        if (UpdateCoinTag(tag)) {
            m_coinTags->setTag(coinTagId);
            setUtxoFilterTag(coinTagId);
            emit updateTagNameChanged(false);
        }
        else {
            emit updateTagNameChanged(true);
        }
        return true;
    }
    else if (type == "coin-delete-all-from-a-tag") {
        int coin_id = maps["coin_id"].toInt();
        if (DeleteCoinTag(coin_id, true)) {
            m_coinTags->setTag(coin_id);
            setUtxoFilterTag(coin_id);
            AppModel::instance()->showToast(0, "Tag deleted", EWARNING::WarningType::SUCCESS_MSG);
            setCoinFlow("view-coin-tags");
        }
        return true;
    }
    else if (type == "coin-delete-selected-from-a-tag") {
        int coin_id = maps["coin_id"].toInt();
        bool ret = false;
        for (auto utxo : m_utxoFilterTag->fullList()) {
            if (utxo->selected()) {
                if(RemoveFromCoinTag(maps["coin_id"].toInt(), utxo->txid(), utxo->vout(), false)) {
                    ret = true;
                }
            }
        }
        if (ret) {
            RequestGetCoins();
            RequestUpdateCoins();
            m_coinTags->setTag(coin_id);
            setUtxoFilterTag(coin_id);
            AppModel::instance()->showToast(0, "Tag deleted", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "coin-delete-a-tag") {
        QString tx_id = maps["tx_id"].toString();
        int coin_id = m_coinTags->tag().toMap()["coin_tag_id"].toInt();
        QUTXOPtr info = utxoList()->getUTXOByTxid(tx_id);
        if (RemoveFromCoinTag(coin_id, info->txid(), info->vout())) {
            m_coinTags->setTag(coin_id);
            setUtxoFilterTag(coin_id);
            AppModel::instance()->showToast(0, "Tag deleted", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "coin-delete-a-collection") {
        QString tx_id = maps["tx_id"].toString();
        int coin_id = m_coinCollections->collection().toMap()["coin_collection_id"].toInt();
        QUTXOPtr info = utxoList()->getUTXOByTxid(tx_id);
        if (RemoveFromCoinCollection(coin_id, info->txid(), info->vout())) {
            m_coinCollections->setCollection(coin_id);
            setUtxoFilterCollection(coin_id);
            AppModel::instance()->showToast(0, "Collection deleted", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "coin-delete-all-collection") {
        int coin_id = maps["coin_id"].toInt();
        if (DeleteCoinCollection(coin_id, true)) {
            m_coinCollections->setCollection(coin_id);
            setUtxoFilterCollection(coin_id);
            AppModel::instance()->showToast(0, "Collection deleted", EWARNING::WarningType::SUCCESS_MSG);
            setCoinFlow("view-coin-collections");
        }
        return true;
    }
    else if (type == "coin-delete-selected-collection") {
        int coin_id = maps["coin_id"].toInt();
        bool ret = false;
        for (auto utxo : m_utxoFilterCollection->fullList()) {
            if (utxo->selected()) {
                if(RemoveFromCoinCollection(coin_id, utxo->txid(), utxo->vout(), false)) {
                    ret = true;
                }
            }
        }
        if (ret) {
            RequestGetCoins();
            RequestUpdateCoins();
            m_coinCollections->setCollection(coin_id);
            setUtxoFilterCollection(coin_id);
            AppModel::instance()->showToast(0, "Collection deleted", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "coin-update-collection") {
        int coin_id = maps["coin_id"].toInt();
        QString coinName = maps["coinName"].toString();
        bool isAutoLock = maps["isAutoLock"].toBool();
        bool isMoveAll = maps["isMoveAll"].toBool();

        nunchuk::CoinCollection collection(coin_id, coinName.toStdString());
        collection.set_auto_lock(isAutoLock);
        if (isMoveAll) {
            for (auto utxo : m_utxoList->fullList()) {
                AddToCoinCollection(collection.get_id(), utxo->txid(), utxo->vout(), false);
            }
        }
        if (UpdateCoinCollection(collection)) {
            m_coinCollections->setCollection(coin_id);
            setUtxoFilterCollection(coin_id);
            AppModel::instance()->showToast(0, "Collection updated", EWARNING::WarningType::SUCCESS_MSG);
            emit updateCollectionNameChanged(false);
        } else {
            emit updateCollectionNameChanged(true);
        }
        return true;
    }
    else if (type == "add-coin-tag-to-wallet") {
        QString coinName = maps["coinName"].toString();
        QString coinColor = maps["coinColor"].toString();
        DBG_INFO << "Coin tag added to wallet " << coinName << ": color : " << coinColor;
        if (!coinName.isEmpty() && !coinColor.isEmpty()) {
            bool ret = AddNewCoinTag(coinName, coinColor, true);
            if (ret) {
                emit updateTagNameChanged(false);
                AppModel::instance()->showToast(0, "Coin tag updated", EWARNING::WarningType::SUCCESS_MSG);
            }
            else {
                emit updateTagNameChanged(true);
            }
        }
        return true;
    }
    else if (type == "coin-tag-checked-unchecked") {
        int coin_id = maps["coin_id"].toInt();
        bool isChecked = maps["checked"].toBool();
        m_coinTags->setChecked(coin_id, isChecked);
        return true;
    }
    else if (type == "coin-collection-checked-unchecked") {
        int coin_id = maps["coin_id"].toInt();
        bool isChecked = maps["checked"].toBool();
        m_coinCollections->setChecked(coin_id, isChecked);
        return true;
    }
    else if (type == "add-coin-tags-to-wallet") {
        bool ret = false;
        for (auto coin: m_coinTags->fullList()) {
            if (m_coinTags->isChecked(coin.get_id())) {
                if(AddToCoinTag(coin.get_id(), utxoInfo()->txid(), utxoInfo()->vout(), false)) {
                    ret = true;
                }
            } else {
                if(RemoveFromCoinTag(coin.get_id(), utxoInfo()->txid(), utxoInfo()->vout(), false)) {
                    ret = true;
                }
            }
        }
        if (ret) {
            RequestGetCoins();
            RequestUpdateCoins();
            setUtxoInfo(utxoInfo()->txid());
            setCoinFlow("coin-details");
            AppModel::instance()->showToast(0, "Coin updated", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "add-multi-coin-tags-to-wallet") {
        bool ret = false;
        for (auto utxo : m_utxoList->fullList()) {
            if (utxo->selected()) {
                for (auto coin: m_coinTags->fullList()) {
                    if (m_coinTags->isChecked(coin.get_id())) {
                        if(AddToCoinTag(coin.get_id(), utxo->txid(), utxo->vout(), false)) {
                            ret = true;
                        }
                    }
                }
            }
        }
        if (ret) {
            setReuse(true);
            RequestGetCoins();
            RequestUpdateCoins();
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
            AppModel::instance()->showToast(0, "Coin updated", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "add-locked-multi-coin-tags-to-wallet") {
        bool ret = false;
        for (auto utxo : m_utxoListLocked->fullList()) {
            for (auto coin: m_coinTags->fullList()) {
                if (m_coinTags->isChecked(coin.get_id())) {
                    if(AddToCoinTag(coin.get_id(), utxo->txid(), utxo->vout(), false)) {
                        ret = true;
                    }
                } else {
                    if(RemoveFromCoinTag(coin.get_id(), utxo->txid(), utxo->vout(), false)) {
                        ret = true;
                    }
                }
            }
        }
        if (ret) {
            RequestGetCoins();
            RequestUpdateCoins();
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
            AppModel::instance()->showToast(0, "Coin updated", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "edit-view-coin-tags") {
        setUtxoInfo(utxoInfo()->txid());
        m_coinTags->setChecked(utxoInfo()->getTags());
    }
    else if (type == "edit-view-coin-collections") {
        setUtxoInfo(utxoInfo()->txid());
        m_coinCollections->setChecked(utxoInfo()->getCollections());
        m_previousViewCollection = type;
    }
    else if (type == "coin-create-collection") {
        setReuse(true);
        if (CreateCoinCollection(maps)) {
            AppModel::instance()->showToast(0, "Collection updated", EWARNING::WarningType::SUCCESS_MSG);
            emit updateCollectionNameChanged(false);
            setCoinFlow(m_previousViewCollection);
        } else {
            emit updateCollectionNameChanged(true);
        }
        return true;
    }
    else if (type == "add-coin-collections-to-wallet") {
        bool ret = false;
        for (auto coin: m_coinCollections->fullList()) {
            if (m_coinCollections->isChecked(coin.get_id())) {
                if(AddToCoinCollection(coin.get_id(), utxoInfo()->txid(), utxoInfo()->vout(), false)) {
                    ret = true;
                }
            } else {
                if(RemoveFromCoinCollection(coin.get_id(), utxoInfo()->txid(), utxoInfo()->vout(), false)) {
                    ret = true;
                }
            }
        }
        if (ret) {
            RequestGetCoins();
            RequestUpdateCoins();
            setUtxoInfo(utxoInfo()->txid());
            setCoinFlow("coin-details");
            AppModel::instance()->showToast(0, "Coin updated", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "add-multi-coin-collections-to-wallet") {
        bool ret = false;
        for (auto utxo : m_utxoList->fullList()) {
            if (utxo->selected()) {
                for (auto coin: m_coinCollections->fullList()) {
                    if (m_coinCollections->isChecked(coin.get_id())) {
                        if(AddToCoinCollection(coin.get_id(), utxo->txid(), utxo->vout(), false)) {
                            ret = true;
                        }
                    }
                }
            }
        }
        if (ret) {
            setReuse(true);
            RequestGetCoins();
            RequestUpdateCoins();
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
            AppModel::instance()->showToast(0, "Coin updated", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "add-locked-multi-coin-collections-to-wallet") {
        bool ret = false;
        for (auto utxo : m_utxoListLocked->fullList()) {
            for (auto coin: m_coinCollections->fullList()) {
                if (m_coinCollections->isChecked(coin.get_id())) {
                    if(AddToCoinCollection(coin.get_id(), utxo->txid(), utxo->vout(), false)) {
                        ret = true;
                    }
                } else {
                    if(RemoveFromCoinCollection(coin.get_id(), utxo->txid(), utxo->vout(), false)) {
                        ret = true;
                    }
                }
            }
        }
        if (ret) {
            RequestGetCoins();
            RequestUpdateCoins();
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
            AppModel::instance()->showToast(0, "Coin updated", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "coin-move-to-another-collection-save") {
        int new_coin_id = maps["coin_id"].toInt();
        int coin_id = m_coinCollections->collection().toMap()["coin_collection_id"].toInt();
        bool remove = false, add = false;
        for (auto utxo : m_utxoFilterCollection->fullList()) {
            if (utxo->selected()) {
                if(AddToCoinCollection(new_coin_id, utxo->txid(), utxo->vout(), false)) {
                    add = true;
                }
                if(RemoveFromCoinCollection(coin_id, utxo->txid(), utxo->vout(), false)) {
                    remove = true;
                }
            }
        }
        if (add && remove) {
            RequestGetCoins();
            m_coinCollections->setCollection(coin_id);
            setUtxoFilterCollection(coin_id);
            setCoinFlow("coin-filter-collection");
            AppModel::instance()->showToast(0, "Coin updated", EWARNING::WarningType::SUCCESS_MSG);
        }
        return true;
    }
    else if (type == "apply-filter-coin-collection") {
        m_utxoList->searchByFilter(msg);
        return true;
    }
    else if (type == "clear-all-filter-coin-collection") {
        m_coinTags->setChecked({});
        m_coinCollections->setChecked({});
        m_utxoList->clearAllFilter();
    }
    else if (type == "view-locked-coins") {
        RequestLockedCoins();
    }
    else if (type == "coin-ancestry") {
        RequestGetCoinAncestry();
    }
    else if (type == "spent-coin-details") {
        QString tx_id = maps["tx_id"].toString();
        m_txid_before_enter_spent_coin = m_utxoInfo->txid();
        for (auto list : m_ancestryList) {
            for (auto utxo: list->fullList()) {
                if (qUtils::strCompare(tx_id, utxo->txid())) {
                    setUtxoInfo(utxo);
                }
            }
        }
    }
    else if (type == "spent-coin-details-back") {
        setUtxoInfo(m_txid_before_enter_spent_coin);
        RequestGetCoinAncestry();
        setCoinFlow("coin-ancestry");
        return true;
    }
    else if (type == "select-coin-for-create-transaction") {
        if (auto trans = AppModel::instance()->transactionInfoPtr()) {
            trans->createFilteringCoinInCoinSelection();
        }
    }
    else if (type == "use-selected-coins-create-transaction") {
        timeoutHandler(1000,[=]{
            auto w = dynamic_cast<Wallet*>(this);
            QEventProcessor::instance()->sendEvent(E::EVT_CONSOLIDATE_COINS_MERGE_MAKE_TRANSACTION_REQUEST, {});
            w->UpdateDraftTransaction({});
            AppModel::instance()->showToast(0, "Coin selection updated", EWARNING::WarningType::SUCCESS_MSG);
        });
        return true;
    }
    else if (type == "filtering-text") {
        if (auto trans = AppModel::instance()->transactionInfoPtr()) {
            DBG_INFO << "searchText" << maps["searchText"].toString();
            trans->manualCoins()->searchByString(maps["searchText"].toString());
            emit trans->manualCoinsChanged();
        }
        return true;
    }
    else if (type == "apply-filter-coin-collection-for-create-transaction") {
        if (auto trans = AppModel::instance()->transactionInfoPtr()) {
            trans->manualCoins()->searchByFilter(msg);
        }
        return true;
    } else if (type == "create-a-new-collection") {
        m_coinCollections->setCollection(-1);
        m_coinTagsFilter->clear();
        for (auto coin: m_coinTags->fullList()) {
            m_coinTagsFilter->addTag(coin);
        }
        emit coinTagsFilterChanged();
    } else if (type == "add-multi-coin-tags-for-collection") {
        m_coinTagsFilter->clear();
        for (auto coin: m_coinTags->fullList()) {
            if (m_coinTags->isChecked(coin.get_id())) {
                m_coinTagsFilter->addTag(coin);
            }
        }
        emit coinTagsFilterChanged();
        setCoinFlow("create-a-new-collection");
        return true;
    } else if (type == "view-coin-collections" ||
               type == "edit-multi-coin-collections" ||
               type == "edit-locked-multi-coin-collections") {
        m_previousViewCollection = type;
    }
    setCoinFlow(type);
    DBG_INFO << "coin-flow" << coinFlow();
    return true;
}

void CoinsControl::GetCoinControlUserWallet()
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QJsonObject output;
    QString errormsg = "";

    bool ret = Draco::instance()->GetCoinControl(wallet_id, output, errormsg);
    if(ret){
        QString data = output["data"].toString();
        bool force = false; // Assume data from server is newer than local
        QWarningMessage msg;
        bool ret = bridge::nunchukImportCoinControlData(wallet_id, data, false, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            if(ret){
                DBG_INFO << "Success: > Request get new UTXOs";
                RequestGetCoins();
            }
            else{
                DBG_ERROR << "Data from server is older than local >  export CoinControl data and post to server";
                RequestUpdateCoins();
            }
        }
    }
    else{
        DBG_ERROR << "GetCoinControlUserWallet failed:" << errormsg;
    }
}

void CoinsControl::GetCoinControlGroupWallet()
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QString group_id = (dynamic_cast<Wallet*>(this))->groupId();
    QJsonObject output;
    QString errormsg = "";

    bool ret = Byzantine::instance()->GetCoinControl(group_id, wallet_id, output, errormsg);
    if(ret){
        QString data = output["data"].toString();
        bool force = false; // Assume data from server is newer than local
        QWarningMessage msg;
        bool ret = bridge::nunchukImportCoinControlData(wallet_id, data, false, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            if(ret){
                DBG_INFO << "Success: > Request get new UTXOs";
                RequestGetCoins();
            }
            else{
                DBG_ERROR << "Data from server is older than local >  export CoinControl data and post to server";
                RequestUpdateCoins();
            }
        }
    }
    else{
        DBG_ERROR << "GetCoinControlGroupWallet failed:" << errormsg;
    }
}

void CoinsControl::UpdateCoinControlUserWallet()
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QWarningMessage msg;
    QString data = bridge::nunchukExportCoinControlData(wallet_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && data != ""){
        QJsonObject updateOutput;
        QString errormsg = "";
        bool update = Draco::instance()->UpdateCoinControl(wallet_id, data, updateOutput, errormsg);
        if(update){
            DBG_INFO << "UpdateCoinControl success";
        }
        else{
            DBG_ERROR << "UpdateCoinControl failed:" << errormsg;
        }
    }
}

void CoinsControl::UpdateCoinControlGroupWallet()
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QString group_id = (dynamic_cast<Wallet*>(this))->groupId();
    QWarningMessage msg;
    QString data = bridge::nunchukExportCoinControlData(wallet_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && data != ""){
        QJsonObject updateOutput;
        QString errormsg = "";
        bool update = Byzantine::instance()->UpdateCoinControl(group_id, wallet_id, data, updateOutput, errormsg);
        if(update){
            DBG_INFO << "UpdateCoinControl success";
        }
        else{

        }
    }
}

QList<int> CoinsControl::tagsInTxAssigned() const
{
    return m_tagsInTxAssigned;
}

void CoinsControl::setReuse(bool newReuse)
{
    m_reuse = newReuse;
}

void CoinsControl::RequestSyncSelectCoinForMakeTransaction(const QVariant &msg)
{
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    if (type == "request-make-transaction-tag") {
        for (auto old_utxo : m_utxoFilterTag->fullList()) {
            for (auto new_utxo : m_utxoList->fullList()) {
                if (old_utxo->txid() == new_utxo->txid()) {
                    new_utxo->setSelected(old_utxo->selected());
                }
            }
        }
    }
    else if (type == "request-make-transaction-collection") {
        for (auto old_utxo : m_utxoFilterCollection->fullList()) {
            for (auto new_utxo : m_utxoList->fullList()) {
                if (old_utxo->txid() == new_utxo->txid()) {
                    new_utxo->setSelected(old_utxo->selected());
                }
            }
        }
    }
    else if (type == "request-unlock-coins-selected") {
        std::vector<nunchuk::UnspentOutput> utxos_selected;
        if(utxoList()){
            for (auto w : utxoList()->fullList()) {
                if(w.data()->selected()){
                    nunchuk::UnspentOutput utxo = w.data()->getUnspentOutput();
                    utxos_selected.push_back(utxo);
                }
            }
        }
        if(utxos_selected.size() > 0){
            setReuse(true);
            UnlockCoins(utxos_selected);
        }
    }
    else if (type == "request-unlock-coins-selected-for-transaction") {
        if (auto trans = AppModel::instance()->transactionInfoPtr()) {
            auto manualCoin = trans->manualCoins();
            std::vector<nunchuk::UnspentOutput> utxos_selected;
            if(manualCoin){
                for (auto w : manualCoin->fullList()) {
                    if(w.data()->selected()){
                        nunchuk::UnspentOutput utxo = w.data()->getUnspentOutput();
                        utxos_selected.push_back(utxo);
                    }
                }
            }
            if(utxos_selected.size() > 0){
                setReuse(true);
                UnlockCoins(utxos_selected);
                trans->createFilteringCoinInCoinSelection();
            }
        }
    }
    else if (type == "coin-tags-assign-tags-to-transaction") {
        if (auto trans = AppModel::instance()->transactionInfoPtr()) {
            if (auto parentCoin = trans->parentCoinsTag()) {
                m_tagsInTxAssigned = parentCoin->tagsSelected();
                AssignTagsToTxChange();
                AppModel::instance()->showToast(0, "Tags assigned", EWARNING::WarningType::SUCCESS_MSG);
            }
        }
    }
    else if (type == "coin-tag-checked-unchecked") {
        if (auto trans = AppModel::instance()->transactionInfoPtr()) {
            int coin_id = maps["coin_id"].toInt();
            bool isChecked = maps["checked"].toBool();
            if (auto parentCoin = trans->parentCoinsTag()) {
                parentCoin->setChecked(coin_id, isChecked);
            }
        }
    }
    else if (type == "edit-change-address" || type == "manage-coin-details") {
        if (auto trans = AppModel::instance()->transactionInfoPtr()) {
            setUtxoInfo(trans->txid());
            QEventProcessor::instance()->sendEvent(E::EVT_COIN_DETAILS_CONTROL_REQUEST, {});
            setCoinFlow("coin-details");
        }
    }
}

QUTXOListModelPtr CoinsControl::GetUtxoListSelected()
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QUTXOListModelPtr inputs = QUTXOListModelPtr(new QUTXOListModel(wallet_id));
    if(utxoList()){
        for (int i = 0; i < utxoList()->rowCount(); i++) {
            QUTXOPtr it = utxoList()->getUTXOByIndex(i);
            if(it.data() && it.data()->selected()){
                DBG_INFO << "UTXO Selected:" << it.data()->txid() << it.data()->amountSats();
                inputs->addUTXO(it.data()->getUnspentOutput());
            }
        }
    }
    return inputs;
}

bool CoinsControl::AssignTagsToTxChange()
{
    if (auto trans = AppModel::instance()->transactionInfoPtr()) {
        if (auto parentCoin = trans->parentCoinsTag()) {
            parentCoin->setChecked(m_tagsInTxAssigned);
            auto txInput = trans->changeInfo();
            for(auto tag: parentCoin->fullList()) {
                if (parentCoin->isChecked(tag.get_id())) {
                    AddToCoinTag(tag.get_id(), QString::fromStdString(txInput.first), txInput.second, false);
                } else {
                    RemoveFromCoinTag(tag.get_id(), QString::fromStdString(txInput.first), txInput.second, false);
                }
            }
        }
    }
    return false;
}

QUTXOListModel *CoinsControl::utxoListLocked() const
{
    return m_utxoListLocked.data();
}

QUTXOListModelPtr CoinsControl::utxoListLockedPtr() const
{
    return m_utxoListLocked;
}

void CoinsControl::setUtxoListLocked(const QUTXOListModelPtr &utxoList)
{
    m_utxoListLocked = utxoList;
    if(m_utxoListLocked.data()){
        m_utxoListLocked.data()->requestSort(QUTXOListModel::UTXORoles::utxo_amount_role,  Qt::DescendingOrder);
    }
    emit utxoListChanged();
}

void CoinsControl::RequestLockedCoins()
{
    // dynamic cast this to Wallet
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QUTXOListModelPtr utxos = bridge::nunchukLockedGetUnspentOutputs(wallet_id);
    if(utxos){
        setUtxoListLocked(utxos);
    }
}

void CoinsControl::RequestForLockedAllCoins(const QVariant &act)
{
    QMap<QString, QVariant> maps = act.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;

    std::vector<nunchuk::UnspentOutput> utxos_selected;

    if(utxoListLocked()){
        for (auto w : utxoListLocked()->fullList()) {
            if(w.data()->selected()){
                nunchuk::UnspentOutput utxo = w.data()->getUnspentOutput();
                utxos_selected.push_back(utxo);
            }
        }
    }

    if(utxos_selected.size() > 0){
        if (type == "lock-coins") {
            LockCoins(utxos_selected);
            RequestLockedCoins();
        }
        else if (type == "unlock-coins") {
            UnlockCoins(utxos_selected);
            RequestLockedCoins();
        }
        else {

        }
    }
}

void CoinsControl::RequestGetCoinAncestry()
{
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    if (m_utxoInfo.isNull()) {
        return;
    }

    m_ancestryList = bridge::nunchukGetCoinAncestry(wallet_id, m_utxoInfo->txid(), m_utxoInfo->vout());
    emit ancestryListChanged();
}

QList<QUTXOListModel *> CoinsControl::ancestryList() const
{
    return m_ancestryList;
}

bool CoinsControl::ImportCoinControlData(const QString &filePath)
{
    QWarningMessage msg;
    QString file_path = qUtils::QGetFilePath(filePath);
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QString data = qUtils::ImportDataViaFile(file_path);
    bool ret = bridge::nunchukImportCoinControlData(wallet_id, data, true, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        RequestGetCoins();
        AppModel::instance()->showToast(0, "Import completed", EWARNING::WarningType::SUCCESS_MSG);
    }
    else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return ret;
}

bool CoinsControl::ExportCoinControlData(const QString &filePath)
{
    QString file_path = qUtils::QGetFilePath(filePath);
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QWarningMessage msg;
    QString data = bridge::nunchukExportCoinControlData(wallet_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        qUtils::ExportDataViaFile(file_path, data);
        return true;
    }
    return false;
}

bool CoinsControl::ExportBIP329(const QString &filePath)
{
    QString file_path = qUtils::QGetFilePath(filePath);
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QWarningMessage msg;
    QString data = bridge::nunchukExportBIP329(wallet_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        qUtils::ExportDataViaFile(file_path, data);
        return true;
    }
    return false;
}

bool CoinsControl::ImportBIP329(const QString &filePath)
{
    QWarningMessage msg;
    QString file_path = qUtils::QGetFilePath(filePath);
    QString wallet_id = (dynamic_cast<Wallet*>(this))->id();
    QString data = qUtils::ImportDataViaFile(file_path);
    bridge::nunchukImportBIP329(wallet_id, data, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        RequestGetCoins();
        AppModel::instance()->showToast(0, "Import completed", EWARNING::WarningType::SUCCESS_MSG);
        return true;
    }
    else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return false;
}

bool CoinsControl::CreateDraftTransaction(int successScreenID, const QVariant &msg)
{
    m_draftTransactionInput = msg.toMap();
    QString memo = m_draftTransactionInput.value("destinationMemo").toString();
    QList<QVariant> destinationInputed = m_draftTransactionInput.value("destinationList").toList();
    QDestinationListModelPtr destinationList = QDestinationListModelPtr(new DestinationListModel());
    auto walletInfo = (dynamic_cast<Wallet*>(this));
    QString wallet_id = walletInfo->id();
    qint64 totalAmountTotal = 0;
    for(QVariant var: destinationInputed){
        qint64 toAmount = 0;
        QMap<QString, QVariant> destination = var.toMap();
        bool isCurrency = destination["onCurrency"].toBool();
        DBG_INFO << destination;
        if (isCurrency) {
            toAmount = qUtils::QAmountFromCurrency(destination["toAmount"].toString());
        } else {
            if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
                QString amountStr = destination["toAmount"].toString();
                amountStr.remove(",");
                toAmount = amountStr.toLongLong();
            }
            else{
                toAmount = qUtils::QAmountFromValue(destination["toAmount"].toString());
            }
        }
        destinationList.data()->addDestination(destination["toAddress"].toString(), toAmount);
        totalAmountTotal += toAmount;
    }
    DBG_INFO << "Total Amount Inputted : " << totalAmountTotal;

    AppModel::instance()->setDestinationList(destinationList);

    QUTXOListModelPtr inputs = GetUtxoListSelected();
    qint64 totalAmountInputSelected = inputs->amountSats();

    bool subtractFromAmount = false;
    if(walletInfo->escrow() || (totalAmountTotal >= walletInfo->balanceSats()) || totalAmountTotal >= totalAmountInputSelected ){
        subtractFromAmount = true;
    }
    QMap<QString, qint64> outputs;
    if(AppModel::instance()->destinationList()){
        outputs = AppModel::instance()->destinationList()->getOutputs();
        DBG_INFO << "Destination : " << outputs;
    }
    qint64 feerate = AppModel::instance()->hourFeeOrigin();

    DBG_INFO << "Fee rate" << feerate;
    DBG_INFO << "Subtract From Amount : " << subtractFromAmount;

    QWarningMessage msgwarning;
    QTransactionPtr trans = bridge::nunchukDraftTransaction(wallet_id,
                                                            outputs,
                                                            inputs,
                                                            feerate,
                                                            subtractFromAmount,
                                                            "",
                                                            msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
        if(trans){
            AppModel::instance()->setTransactionInfo(trans);
            AppModel::instance()->transactionInfo()->setMemo(memo);
            QEventProcessor::instance()->sendEvent(successScreenID);
        }
    }
    else {
        if(((int)EWARNING::WarningType::EXCEPTION_MSG == msgwarning.type()) &&
            (msgwarning.code() == nunchuk::NunchukException::COIN_SELECTION_ERROR) &&
            subtractFromAmount == false)
        {
            DBG_INFO << "Retry make draft transaction with subtract true";
            msgwarning.resetWarningMessage();
            QTransactionPtr trans = bridge::nunchukDraftTransaction(wallet_id,
                                                                    outputs,
                                                                    inputs,
                                                                    feerate,
                                                                    true,
                                                                    "",
                                                                    msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                if(trans){
                    AppModel::instance()->setTransactionInfo(trans);
                    AppModel::instance()->transactionInfo()->setMemo(memo);
                    QEventProcessor::instance()->sendEvent(successScreenID);
                }
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
            }

        }
        else{
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }
    }
    return false;
}

bool CoinsControl::UpdateDraftTransaction(const QVariant &msg)
{
    if (!msg.toMap().isEmpty()) {
        m_draftTransactionInput = msg.toMap();
    }
    auto walletInfo = (dynamic_cast<Wallet*>(this));
    QString wallet_id = walletInfo->id();
    bool subtractFromFeeAmout = m_draftTransactionInput.value("subtractFromFeeAmout").toBool();
    int feeRate = m_draftTransactionInput.value("feeRate").toDouble()*1000; // Convert sats/Byte to sats/kB
    bool manualFee = m_draftTransactionInput.value("manualFee").toBool();
    bool manualOutput = m_draftTransactionInput.value("manualOutput").toBool();
    if(!manualFee) feeRate = -1;
    QString replace_txid = AppModel::instance()->getTxidReplacing();
    DBG_INFO << "subtract:" << subtractFromFeeAmout << "| manual Output:" << manualOutput << "| manual Fee:" << manualFee << "| free rate:" << feeRate;

    QUTXOListModelPtr inputs = NULL;
    if (auto trans = AppModel::instance()->transactionInfo()){
        inputs = trans->GetUtxoListSelected();
    }

    QMap<QString, qint64> outputs;
    if(AppModel::instance()->destinationList()){
        outputs = AppModel::instance()->destinationList()->getOutputs();
    }

    QWarningMessage msgwarning;
    QTransactionPtr trans = bridge::nunchukDraftTransaction(wallet_id,
                                                            outputs,
                                                            inputs,
                                                            feeRate,
                                                            subtractFromFeeAmout,
                                                            replace_txid,
                                                            msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
        if(trans){
            if(AppModel::instance()->transactionInfo()){
                trans.data()->setMemo(AppModel::instance()->transactionInfo()->memo());
                if(QEventProcessor::instance()->onsRequester() == E::STATE_ID_SCR_TRANSACTION_INFO){
                    DBG_INFO << "REPLACE BY FEE, KEEP ORIGIN FEE";
                    trans.data()->setFee(AppModel::instance()->transactionInfo()->feeSats());
                }
            }
            AppModel::instance()->setTransactionInfo(trans);
        }
    }
    else {
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
    }
    return false;
}

QString CoinsControl::searchText() const
{
    return m_searchText;
}

void CoinsControl::setSearchText(const QString &newSearchText)
{
    m_searchText = newSearchText;
    m_utxoList->searchByString(newSearchText);
    emit searchTextChanged();
    emit utxoListChanged();
}

QCoinTagsModel* CoinsControl::coinTags() const
{
    return m_coinTags.data();
}

QCoinTagsModel* CoinsControl::coinTagsFilter() const
{
    return m_coinTagsFilter.data();
}

QCoinCollectionsModel* CoinsControl::coinCollections() const
{
    return m_coinCollections.data();
}

QUTXOListModel *CoinsControl::utxoFilterTag() const
{
    return m_utxoFilterTag.data();
}

void CoinsControl::setUtxoFilterTag(const int tag_id)
{
    m_utxoFilterTag = GetCoinByTagId(tag_id);
    emit utxoFilterTagChanged();
}

QUTXOListModel *CoinsControl::utxoFilterCollection() const
{
    return m_utxoFilterCollection.data();
}

void CoinsControl::setUtxoFilterCollection(const int collection_id)
{
    m_utxoFilterCollection = GetCoinByCollectionId(collection_id);
    emit utxoFilterCollectionChanged();
}

void CoinsControl::RequestForAllCoins(const QVariant &act)
{
    QMap<QString, QVariant> maps = act.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;

    std::vector<nunchuk::UnspentOutput> utxos_selected;

    if(utxoList()){
        for (auto w : utxoList()->fullList()) {
            if(w.data()->selected()){
                nunchuk::UnspentOutput utxo = w.data()->getUnspentOutput();
                utxos_selected.push_back(utxo);
            }
        }
    }

    if(utxos_selected.size() > 0){
        if (type == "consolidate-coins") {
            QEventProcessor::instance()->sendEvent(E::EVT_UTXOS_CONSOLIDATE_REQUEST);
        }
        else if (type == "lock-coins") {
            LockCoins(utxos_selected);
            utxoList()->selectAll(false);
        }
        else if (type == "unlock-coins") {
            UnlockCoins(utxos_selected);
            utxoList()->selectAll(false);
        }
        else {

        }
    }
}
