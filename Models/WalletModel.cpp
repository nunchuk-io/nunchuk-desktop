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
#include "WalletModel.h"
#include "qUtils.h"
#include "AppSetting.h"
#include "ViewsEnums.h"
#include "AppModel.h"
#include <QQmlEngine>
#include "bridgeifaces.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QUserWalletDummyTx.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QRecurringPayment.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QSharedWallets.h"
#include "QThreadForwarder.h"

Wallet::Wallet() : CreatingWallet(nunchuk::Wallet(false))
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Wallet::Wallet(const nunchuk::Wallet &w) : CreatingWallet{w}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Wallet::~Wallet(){

}

WalletListModel::WalletListModel(){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

WalletListModel::~WalletListModel(){m_data.clear();}

int WalletListModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant WalletListModel::data(const QModelIndex &index, int role) const {
    return dataWallet(m_data[index.row()], role);
}

bool WalletListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == wallet_M_Role){
        m_data[index.row()]->setWalletM(value.toInt());
        emit dataChanged(index, index, { role } );
    }
    return true;
}

QHash<int, QByteArray> WalletListModel::roleNames() const{
    return roleWalletNames();
}

QHash<int,QByteArray> WalletListModel::roleWalletNames() {
    QHash<int, QByteArray> roles;
    roles[wallet_Id_Role]                   = "wallet_id";
    roles[wallet_Name_Role]                 = "wallet_name";
    roles[wallet_M_Role]                    = "wallet_M";
    roles[wallet_N_Role]                    = "wallet_N";
    roles[wallet_AddressType_Role]          = "wallet_AddressType";
    roles[wallet_Balance_Role]              = "wallet_Balance";
    roles[wallet_BalanceBTC_Role]           = "wallet_Balance_BTC";
    roles[wallet_BalanceCurrency_Role]      = "wallet_Balance_Currency";
    roles[wallet_createDate_Role]           = "wallet_CreateDate";
    roles[wallet_Escrow_Role]               = "wallet_Escrow";
    roles[wallet_SingleSignerList_Role]     = "wallet_singleSignersAssigned";
    roles[wallet_Address_Role]              = "wallet_Address";
    roles[wallet_usedAddressList_Role]      = "wallet_usedAddressList";
    roles[wallet_unUsedAddressList_Role]    = "wallet_unUsedAddressList";
    roles[wallet_isSharedWallet_Role]       = "wallet_isSharedWallet";
    roles[wallet_isAssistedWallet_Role]     = "wallet_isAssistedWallet";
    roles[wallet_groupid_Role]              = "wallet_group_id";
    roles[wallet_dashboard_Role]            = "wallet_dashboard";
    roles[wallet_myRole_Role]               = "wallet_role";
    roles[wallet_hasOwner_Role]             = "wallet_hasOwner";
    roles[wallet_primaryOwner_Role]         = "wallet_primaryOwner";
    roles[wallet_isHotWallet_Role]          = "wallet_isHotWallet";
    roles[wallet_slug_Role]                 = "wallet_slug";
    roles[wallet_isLocked_Role]             = "wallet_isLocked";
    roles[wallet_isReplaced_Role]           = "wallet_isReplaced";
    roles[wallet_isSanboxWallet_Role]       = "wallet_isSanboxWallet";
    roles[wallet_conversation_Role]         = "wallet_conversation";
    roles[wallet_unreadMessage_Role]        = "wallet_unreadMessage";
    roles[wallet_numberOnline_Role]         = "wallet_numberOnline";
    roles[wallet_numberSigner_Role]         = "wallet_numberSigner";
    roles[wallet_archived_Role]             = "wallet_isArchived";
    roles[wallet_walletType_Role]           = "wallet_walletType";
    return roles;
}

QVariant WalletListModel::dataWallet(const QWalletPtr &data, int role) {
    if (data.isNull())
        return QVariant();

    switch (role) {
    case wallet_Id_Role:
        return data->walletId();
    case wallet_Name_Role:
        return data->walletNameDisplay();
    case wallet_M_Role:
        return data->walletM();
    case wallet_N_Role:
        return data->walletN();
    case wallet_AddressType_Role:
        return data->walletAddressType();
    case wallet_createDate_Role:
        return data->walletCreateDate();
    case wallet_Balance_Role:
        return QVariant::fromValue(data->balanceDisplay());
    case wallet_BalanceBTC_Role:
        return QVariant::fromValue(data->balanceBTC());
    case wallet_BalanceCurrency_Role:
        return QVariant::fromValue(data->balanceCurrency());
    case wallet_Escrow_Role:
        return data->walletEscrow();
    case wallet_SingleSignerList_Role:
        return QVariant::fromValue((SingleSignerListModel *)data->singleSignersAssigned());
    case wallet_Address_Role: {
        if (data->address().isEmpty()) {
            return data->unUseAddress();
        } else {
            return data->address();
        }
    }
    case wallet_usedAddressList_Role:
        return data->usedAddressList();
    case wallet_unUsedAddressList_Role:
        return data->unUsedAddressList();
    case wallet_isSharedWallet_Role:
        return data->isSharedWallet();
    case wallet_isAssistedWallet_Role:
        return data->isAssistedWallet();
    case wallet_groupid_Role:
        return data->groupId();
    case wallet_dashboard_Role:
        return data->dashboardInfo();
    case wallet_myRole_Role:
        return data->myRole();
    case wallet_hasOwner_Role:
        return data->ownerPrimary().isValid();
    case wallet_primaryOwner_Role:
        return data->ownerPrimary();
    case wallet_isHotWallet_Role:
        return data->keyNeedBackup();
    case wallet_slug_Role:
        return data->slug();
    case wallet_isLocked_Role:
        return data->isLocked();
    case wallet_isReplaced_Role:
        return data->isReplaced();
    case wallet_isSanboxWallet_Role:
        return data->isGlobalGroupWallet();
    case wallet_conversation_Role:
        return QVariant::fromValue((QGroupMessageModel *)data->conversations());
    case wallet_unreadMessage_Role:
        return data->unreadMessage();
    case wallet_numberOnline_Role:
        return data->numberOnline();
    case wallet_archived_Role:
        return data->isArchived();
    case wallet_walletType_Role:
        return data->walletType();
    default:
        return QVariant();
    }
}

QVariant WalletListModel::useQml(const QWalletPtr &data) {
    QHash<int,QByteArray> names = roleWalletNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    while (i.hasNext()) {
        i.next();
        QVariant tmp = dataWallet(data, i.key());
        res[i.value()] = tmp;
    }
    return QVariant::fromValue(res);
}

int WalletListModel::count() const
{
    return m_data.count();
}

int WalletListModel::unReadMessageCount()
{
    int count = 0;
    for (int i = 0; i < m_data.count(); i++) {
        count += m_data.at(i)->unreadMessage();
    }
    DBG_INFO << m_data.count() << count;
    return count;
}

QVariant WalletListModel::get(int row)
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

void WalletListModel::addWallet(const QWalletPtr &wallet)
{
    if(wallet && !containsId(wallet.data()->walletId())){
        beginResetModel();
        m_data.append(wallet);
        endResetModel();
    }
}

void WalletListModel::replaceWallet(const QWalletPtr &wallet)
{
    if(wallet){
        for(int i = 0; i < m_data.count(); i++){
            if(qUtils::strCompare(wallet.data()->walletId(), m_data.at(i)->walletId())){
                m_data.replace(i, wallet);
                emit dataChanged(index(i),index(i));
                break;
            }
        }
    }
}

void WalletListModel::addSharedWallet(const QWalletPtr &wallet)
{
    if(wallet){
        beginResetModel();
        if(containsId(wallet.data()->walletId())){
            replaceWallet(wallet);
        }
        else{
            addWallet(wallet);
        }
        endResetModel();
    }
}

void WalletListModel::updateBalance(const QString &walletId, const qint64 balance)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && qUtils::strCompare(walletId, m_data.at(i)->walletId())){
            m_data.at(i)->setWalletBalance(balance);
            emit dataChanged(index(i),index(i));
        }
    }
}

void WalletListModel::updateName(const QString &walletId, const QString &value)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && qUtils::strCompare(walletId, m_data.at(i)->walletId())){
            m_data.at(i)->setWalletName(value);
            emit dataChanged(index(i),index(i));
        }
    }
}

void WalletListModel::updateNunchukWallet(const QString &walletId, const nunchuk::Wallet wallet)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && qUtils::strCompare(walletId, m_data.at(i)->walletId())){
            m_data.at(i)->setNunchukWallet(wallet);
            emit dataChanged(index(i),index(i));
        }
    }
}

void WalletListModel::dataUpdated(const QString &walletId)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && qUtils::strCompare(walletId, m_data.at(i)->walletId())){
            emit dataChanged(index(i),index(i));
        }
    }
}

void WalletListModel::updateDescription(const QString &walletId, const QString &value)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && qUtils::strCompare(walletId, m_data.at(i)->walletId())){
            m_data.at(i)->setWalletDescription(value);
            emit dataChanged(index(i),index(i));
        }
    }
}

QVariantList WalletListModel::walletListByMasterSigner(const QString& masterSignerId){
    QVariantList ret;
    foreach (QWalletPtr i , m_data ){
        if(NULL != i.data()->singleSignersAssigned()){
            bool exist = i.data()->singleSignersAssigned()->containsMasterSignerId(masterSignerId);
            if(true == exist){
                QJsonObject json;
                json["name"] = i.data()->walletNameDisplay();
                json["walletM"] = i.data()->walletM();
                json["walletN"] = i.data()->walletN();
                json["walletType"] = i.data()->walletType();
                ret << QVariant::fromValue(json);
            }
        }
    }
    return ret;
}

QVariantList WalletListModel::walletListByFingerPrint(const QString &masterFingerPrint)
{
    QVariantList ret;
    foreach (QWalletPtr i , m_data ){
        if(NULL != i.data()->singleSignersAssigned()){
            bool exist = i.data()->singleSignersAssigned()->containsFingerPrint(masterFingerPrint);
            if(true == exist){
                QJsonObject json;
                json["name"] = i.data()->walletNameDisplay();
                json["walletM"] = i.data()->walletM();
                json["walletN"] = i.data()->walletN();
                json["walletType"] = i.data()->walletType();
                ret << QVariant::fromValue(json);
            }
        }
    }
    return ret;
}

QWalletPtr WalletListModel::getWalletByIndex(const int index)
{
    if(index < 0 || index >= m_data.count()){
        DBG_INFO << "Index out of range";
        return NULL;
    }
    else {
        return m_data.at(index);
    }
}

QWalletPtr WalletListModel::getWalletById(const QString &walletId)
{
    QReadLocker locker(&m_lock);
    for (const QWalletPtr &walletPtr : std::as_const(m_data)) {
        if (walletPtr && qUtils::strCompare(walletId, walletPtr->walletId())) {
            return walletPtr;
        }
    }
    return QWalletPtr();
}

bool WalletListModel::removeWallet(const QWalletPtr it)
{
    beginResetModel();
    m_data.removeAll(it);
    endResetModel();
    return true;
}

void WalletListModel::removeWallet(const QString &walletId)
{
    beginResetModel();
    foreach (QWalletPtr it, m_data) {
        if(qUtils::strCompare(walletId, it.data()->walletId())){
            m_data.removeOne(it);
            break;
        }
    }
    endResetModel();
}

void WalletListModel::notifyUnitChanged()
{
    const QVector<int>& roles = {};
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data()){
            const auto idx = index(i);
            emit dataChanged(idx, idx, roles);
        }
    }
}

void WalletListModel::updateSignerHealthStatus(const QString &xfp, const int status, const time_t time)
{
    beginResetModel();
    foreach (QWalletPtr i , m_data ){
        if(NULL != i.data()->singleSignersAssigned()){
            i.data()->singleSignersAssigned()->updateSignerHealthStatus(xfp, status, time);
        }
    }
    endResetModel();
}

void WalletListModel::notifyMasterSignerDeleted(const QString &masterSignerId)
{
    beginResetModel();
    foreach (QWalletPtr i , m_data ){
        if(NULL != i.data()->singleSignersAssigned()){
            i.data()->singleSignersAssigned()->notifyMasterSignerDeleted(masterSignerId);
        }
    }
    endResetModel();
}

int WalletListModel::getWalletIndexById(const QString &walletId)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(qUtils::strCompare(walletId, m_data.at(i).data()->walletId())){
            return i;
        }
    }
    return 0;
}

void WalletListModel::updateHealthCheckTime()
{
    beginResetModel();
    foreach (QWalletPtr i , m_data ){
        if (auto signers = i.data()->singleSignersAssigned()) {
            signers->updateHealthCheckTime();
        }
    }
    endResetModel();
}

void WalletListModel::refresh()
{
    beginResetModel();
    endResetModel();
}

void WalletListModel::requestSort()
{
    beginResetModel();
    if(m_data.count() > 1){
        std::sort(m_data.begin(), m_data.end(), sortWalletByNameDescending);
    }
    endResetModel();
}

void WalletListModel::requestSortLastTimestamp()
{
    if(m_data.count() > 1){
        QList<QWalletPtr> sortedData = m_data;
        std::sort(sortedData.begin(), sortedData.end(), sortLastTimestamp);
        bool needsSort = false;
        for (int i = 0; i < m_data.size(); ++i) {
            if (sortedData[i]->lastTime() != m_data[i]->lastTime()) {
                needsSort = true;
                break;
            }
        }
        beginResetModel();
        m_data = sortedData;
        endResetModel();
        emit refreshWalletList();
    }
}

bool WalletListModel::containsId(const QString &id)
{
    foreach (QWalletPtr i , m_data ){
        if(qUtils::strCompare(i.data()->walletId(), id)){
            return true;
        }
    }
    return false;
}

void WalletListModel::updateSharedWalletById(const QString &wallet_id, const QString &room_id, const QString &init_id,const QString &name)
{
    beginResetModel();
    foreach (QWalletPtr it , m_data ){
        if(qUtils::strCompare(it.data()->walletId(), wallet_id)){
            it.data()->setIsSharedWallet(true);
            it.data()->setRoomId(room_id);
            it.data()->setInitEventId(init_id);
            it.data()->setWalletName(name);
        }
    }
    endResetModel();
}

void WalletListModel::updateSignerOfRoomWallet(const QString &wallet_id, const SignerAssigned &signer)
{
    foreach (QWalletPtr it , m_data ){
        if(qUtils::strCompare(it.data()->walletId(), wallet_id)){
            if(it.data()->singleSignersAssigned()){
                it.data()->singleSignersAssigned()->updateSignerOfRoomWallet(signer);
            }
        }
    }
}

void WalletListModel::updateGroupMessage(const QString &wallet_id, const nunchuk::GroupMessage data)
{
    DBG_INFO << wallet_id << data.get_content() << AppModel::instance()->tabIndex();
    if((int)ENUNCHUCK::TabSelection::CHAT_TAB == AppModel::instance()->tabIndex()){
        auto wallet = currentWalletPtr();
        if (wallet && qUtils::strCompare(wallet->walletId(), wallet_id)) {
            wallet->markAsReadMessage(QString::fromStdString(data.get_id()));
            wallet->setUnreadMessage(0);
        } else if (auto otherWallet = getWalletById(wallet_id)) {
            int number = bridge::GetUnreadMessagesCount(wallet_id);
            otherWallet->setUnreadMessage(number);
        }
    }
    else if((int)ENUNCHUCK::TabSelection::WALLET_TAB == AppModel::instance()->tabIndex()){
        auto wallet = AppModel::instance()->walletInfo();
        if(wallet && qUtils::strCompare(wallet_id, wallet->walletId()) && wallet->showbubbleChat()){
            wallet->markAsReadMessage(QString::fromStdString(data.get_id()));
            wallet->setUnreadMessage(0);
            DBG_INFO << "1";
        } else if (auto otherWallet = getWalletById(wallet_id)) {
            int number = bridge::GetUnreadMessagesCount(wallet_id);
            otherWallet->setUnreadMessage(number);
        }
    }
    else if (auto otherWallet = getWalletById(wallet_id)) {
        int number = bridge::GetUnreadMessagesCount(wallet_id);
        otherWallet->setUnreadMessage(number);
    }

    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && qUtils::strCompare(wallet_id, m_data.at(i)->walletId())  && m_data.at(i)->conversations()){
            m_data.at(i)->conversations()->appendGroupMessage(data);
            emit m_data.at(i)->walletChanged();
            emit dataChanged(index(i),index(i));
            emit unReadMessageCountChanged();
        }
    }
}

void WalletListModel::updateGroupSandbox(const QString &wallet_id, const nunchuk::GroupSandbox data)
{
    DBG_INFO << wallet_id;
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && qUtils::strCompare(wallet_id, m_data.at(i)->walletId())){
            m_data.at(i)->updateGroupSandbox(data);
            emit dataChanged(index(i),index(i));
        }
    }
}

void WalletListModel::updateUnreadMessage(const QString &wallet_id, int number)
{
    DBG_INFO << wallet_id;
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).data() && qUtils::strCompare(wallet_id, m_data.at(i)->walletId())){
            m_data.at(i)->setUnreadMessage(number);
            emit dataChanged(index(i),index(i));
            emit unReadMessageCountChanged();
        }
    }
}

QList<QWalletPtr> WalletListModel::fullList() const
{
    return m_data;
}

void WalletListModel::cleardata()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

bool WalletListModel::existProWallet()
{
    foreach (QWalletPtr it , m_data ){
        if (it->isPro()) return true;
    }
    return false;
}

bool WalletListModel::existGroupWallet()
{
    foreach (QWalletPtr it , m_data ){
        if (it->hasGroup()) return true;
    }
    return false;
}

bool WalletListModel::isContainsPremier()
{
    foreach (QWalletPtr it , m_data ){
        if(it->isByzantinePremier()) {
            return true;
        }
    }
    return false;
}

void WalletListModel::checkContainsGroup()
{
    emit containsGroupChanged();
}

QVariant WalletListModel::removeOrNot(const QString &masterFingerPrint)
{
    struct UsedKeyInWallet {
        bool used_in_assisted_wallet;
        bool used_in_free_wallet;
        bool used_in_hot_wallet;
    };
    auto checkIsAssisted = [this](const QString &masterFingerPrint, const QString& derivation_path) -> UsedKeyInWallet {
        UsedKeyInWallet result{false, false, false};
        foreach (QWalletPtr i , m_data ){
            if(NULL != i.data()->singleSignersAssigned()){
                bool exist = i.data()->singleSignersAssigned()->containsSigner(masterFingerPrint, derivation_path);
                if (exist && i->isAssistedWallet()) { result.used_in_assisted_wallet = true; break; }
                else if (exist && i->keyNeedBackup()) { result.used_in_hot_wallet = true; break; }
                else if (exist) { result.used_in_free_wallet = true; break; }
            }
        }
        return result;
    };
    UsedKeyInWallet result{false, false, false};
    QMasterSignerPtr master = bridge::nunchukGetMasterSignerFingerprint(masterFingerPrint);
    if (!master.isNull()) {
        std::vector<nunchuk::SingleSigner> signers = bridge::GetSignersFromMasterSigner(masterFingerPrint);
        for(auto signer: signers) {
            auto tmp = checkIsAssisted(masterFingerPrint, QString::fromStdString(signer.get_derivation_path()));
            if (tmp.used_in_assisted_wallet || tmp.used_in_hot_wallet || tmp.used_in_free_wallet) {
                result = tmp;
                break;
            }
        }
    } else {
        QSingleSignerPtr single = bridge::nunchukGetRemoteSigner(masterFingerPrint);
        if (!single.isNull()) {
            result = checkIsAssisted(single->fingerPrint(), single->derivationPath());
        }
    }

    QJsonObject ret {
       {"used_in_assisted_wallet", result.used_in_assisted_wallet},
       {"used_in_hot_wallet", result.used_in_hot_wallet},
       {"used_in_free_wallet", result.used_in_free_wallet}
    };
    return QVariant::fromValue(ret);
}

bool WalletListModel::hasAssistedWallet() const
{
    foreach (QWalletPtr it , m_data ){
        if (it->isAssistedWallet()) return true;
    }
    return false;
}

bool WalletListModel::swapWallets(const int from, const int to) {
    if (from >= 0 && to >= 0 && from < m_data.size() && to < m_data.size()) {
        std::swap(m_data[from], m_data[to]);
        return true;
    }
    return false;
}

void WalletListModel::slotsMoveFinished(const QString &oldWalletId)
{
    if(oldWalletId.isEmpty()) return;
    int index = getWalletIndexById(oldWalletId);
    AppModel::instance()->setWalletListCurrentIndex(index);
    // Save persistent data
    AppSetting::instance()->setOrderWalletList(getOrderedWalletIds());
    refresh();
}

int WalletListModel::currentIndex() const
{
    return m_currentIndex;
}

void WalletListModel::setCurrentIndex(int newCurrentIndex)
{
    m_currentIndex = newCurrentIndex;
    setCurrentWallet(getWalletByIndex(newCurrentIndex));
    emit currentIndexChanged();
}

void WalletListModel::startAllConversation()
{
    foreach (QWalletPtr it , m_data ){
        it->startDownloadConversation();
    }
}

Wallet *WalletListModel::currentWallet() const
{
    return m_currentWallet.data();
}

QWalletPtr WalletListModel::currentWalletPtr() const
{
    return m_currentWallet;
}

void WalletListModel::setCurrentWallet(const QWalletPtr& newCurrentWallet)
{
    m_currentWallet = newCurrentWallet;
    if(m_currentWallet && m_currentWallet.data()->conversations() && m_currentWallet->isGlobalGroupWallet()){
        QString wallet_id = m_currentWallet.data()->walletId();
        m_currentWallet.data()->conversations()->startDownloadConversation(wallet_id);
    }
    emit currentIndexChanged();
}

QStringList WalletListModel::getOrderedWalletIds() const {
    QStringList ids;
    for (const auto &wallet : m_data) {
        ids.append(wallet->walletId());
    }
    return ids;
}

void WalletListModel::saveOrderWalletIds() {
    AppSetting::instance()->setOrderWalletList(getOrderedWalletIds());
}

bool sortWalletByNameDescending(const QWalletPtr &v1, const QWalletPtr &v2)
{
    if (!v1 || !v2) {
        return false;
    }
    // Retrieve the specific wallet order
    QStringList walletIds = AppSetting::instance()->orderWalletList();

    // If a specific wallet order is set, sort according to it
    if (!walletIds.isEmpty()) {
        int index1 = walletIds.indexOf(v1->walletId());
        int index2 = walletIds.indexOf(v2->walletId());

        // Wallets found in the order list are sorted by their positions
        if (index1 != -1 && index2 != -1) {
            return index1 < index2;
        }

        // Wallets not found in the order list are placed after those in the list
        if (index1 != -1) {
            return false;
        }
        if (index2 != -1) {
            return true;
        }
    }

    // Assisted wallets are placed on top
    if (v1->isAssistedWallet() && !v2->isAssistedWallet()) {
        return true;
    }
    if (!v1->isAssistedWallet() && v2->isAssistedWallet()) {
        return false;
    }

    // Other wallets are ordered by creation time in descending order (newest first)
    return v1->walletCreateDate().toTime_t() > v2->walletCreateDate().toTime_t();
}

bool sortLastTimestamp(const QWalletPtr &v1, const QWalletPtr &v2)
{
    if (!v1 || !v2 || v1.isNull() || v2.isNull() || !v1.data() || !v2.data()) {
        return false; // Handle case where pointers are invalid or null
    }
    if (v1->lastTime() == 0 || v2->lastTime() == 0) {
        return false; // Handle case where timestamps are not set
    }
    return v1->lastTime() > v2->lastTime();
}
