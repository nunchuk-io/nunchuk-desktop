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
#include "AppModel.h"
#include <QQmlEngine>
#include "bridgeifaces.h"

Wallet::Wallet() :
    m_id(""),
    m_m(0),
    n_n(0),
    m_nShared(0),
    m_name(""),
    m_addressType(QString::number((int)ENUNCHUCK::AddressType::NATIVE_SEGWIT)), // Default is NATIVE_SEGWIT
    m_balance(0),
    m_createDate(QDateTime::currentDateTime()),
    m_escrow(false),
    m_signers(QSingleSignerListModelPtr(new SingleSignerListModel())),
    m_transactionHistory(QTransactionListModelPtr(new TransactionListModel())),
    m_capableCreate(true),
    m_description(""),
    m_descriptior(""),
    m_creationMode((int)CreationMode::CREATE_NEW_WALLET),
    m_isSharedWallet(false),
    m_roomId(""),
    m_initEventId("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Wallet::Wallet(const QString &pr_id,
               const int pr_m,
               const int pr_n,
               const QString &pr_name,
               const QString &pr_addrType,
               const qint64 pr_balance,
               const QDateTime &pr_createDate,
               const bool pr_escrow,
               const QSingleSignerListModelPtr& pr_signers,
               const QString &pr_description) :
    m_id(pr_id),
    m_m(pr_m),
    n_n(pr_n),
    m_nShared(0),
    m_name(pr_name),
    m_addressType(pr_addrType),
    m_balance(pr_balance),
    m_createDate(pr_createDate),
    m_escrow(pr_escrow),
    m_signers(pr_signers),
    m_transactionHistory(QTransactionListModelPtr(new TransactionListModel())),
    m_address("There is no unused address"),
    m_capableCreate(true),
    m_description(pr_description),
    m_descriptior(""),
    m_creationMode((int)CreationMode::CREATE_NEW_WALLET),
    m_isSharedWallet(false),
    m_roomId(""),
    m_initEventId("")
{
    m_unUsedAddressList.clear();
    m_usedAddressList.clear();
    m_usedChangeAddressList.clear();
    m_unUsedChangedAddressList.clear();
}

Wallet::~Wallet(){

}

QString Wallet::id() const {return m_id;}

int Wallet::m() const { return m_m;}

int Wallet::n() {
    n_n = m_signers.data()->rowCount();
    return n_n;
}

QString Wallet::name() const {return m_name;}

QString Wallet::addressType() const {return m_addressType;}

qint64 Wallet::balanceSats() const
{
    //2100000000000000 Max
    return m_balance;
}

QString Wallet::balanceBTC() const
{
    return qUtils::QValueFromAmount(m_balance);
}

QString Wallet::balanceDisplay() const {
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(balanceSats());
    }
    else{
        return balanceBTC();
    }
}

QString Wallet::balanceCurrency() const
{
    return qUtils::currencyLocale(balanceSats());
}

QString Wallet::createDate() const {
    if(0 == m_createDate.toTime_t()){
        return "--/--/----"; // There is no time
    }
//    return createDate_.toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate);
    return m_createDate.toString( "MM/dd/yyyy hh:mm AP" );
}

QDateTime Wallet::createDateDateTime() const {return m_createDate;}

bool Wallet::escrow() const {return m_escrow;}

void Wallet::setId(const QString &d)
{
    if(d != m_id){
        m_id = d;
        emit idChanged();
    }
}

void Wallet::setM(const int d) {
    if(d != m_m){
        m_m = qMax(0, d);
        emit mChanged();
    }
}

void Wallet::setN(const int d) {
    if(d != n_n){
        n_n = qMax(0, d);
        emit nChanged();
    }
}

void Wallet::setName(const QString &d) {
    if(d != m_name){
        m_name = d;
        emit nameChanged();
    }
}

void Wallet::setAddressType(const QString &d) {
    if(d != m_addressType){
        m_addressType = d;
        emit addressTypeChanged();
    }
}

void Wallet::setBalance(const qint64 d) {
    if(d != m_balance){
        m_balance = d;
        emit balanceChanged();
    }
}

void Wallet::setCreateDate(const QDateTime &d) {
    if(d != m_createDate){
        m_createDate = d;
        emit createDateChanged();
    }
}

void Wallet::setEscrow(const bool d) {
    if(d != m_escrow){
        m_escrow = d;
        emit escrowChanged();
    }
}

void Wallet::setSigners(const QSingleSignerListModelPtr &d){
    m_signers = d;
    emit nChanged();
    emit singleSignersAssignedChanged();
}

QString Wallet::descriptior() const
{
    return m_descriptior;
}

void Wallet::setDescriptior(const QString &descriptior)
{
    m_descriptior = descriptior;
}

SingleSignerListModel* Wallet::singleSignersAssigned() const {
    return m_signers.data();
}

QSingleSignerListModelPtr Wallet::singleSignersAssignedPtr() const
{
    return m_signers;
}

QString Wallet::address() const {
    return m_address;
}

void Wallet::setAddress(const QString &d){
    if(d != m_address){
        m_address = d;
        emit addressChanged();
    }
}

QStringList Wallet::usedAddressList() const {
    return m_usedAddressList;
}

void Wallet::setUsedAddressList(const QStringList &d){
    if(d != m_usedAddressList){
        m_usedAddressList = d;
        emit usedAddressChanged();
    }
}

QStringList Wallet::unUsedAddressList() const {
    return m_unUsedAddressList;
}

void Wallet::setunUsedAddressList(const QStringList &d){
    m_unUsedAddressList = d;
    emit unUsedAddressChanged();
    if(m_unUsedAddressList.isEmpty()){
        setAddress("There is no avaialable address");
    }
    else{
        setAddress(m_unUsedAddressList.first());
    }
}

bool Wallet::capableCreate() const
{
    return m_capableCreate;
}

void Wallet::setCapableCreate(bool capableCreate)
{
    if(m_capableCreate != capableCreate){
        m_capableCreate = capableCreate;
        emit capableCreateChanged();
    }
}

QString Wallet::description() const
{
    return m_description;
}

void Wallet::setDescription(const QString &description)
{
    if(m_description != description){
        m_description = description;
        emit descriptionChanged();
    }
}

QStringList Wallet::usedChangeAddressList() const
{
    return m_usedChangeAddressList;
}

void Wallet::setUsedChangeAddressList(const QStringList &usedChangeAddressList)
{
    if(m_usedChangeAddressList != usedChangeAddressList){
        m_usedChangeAddressList = usedChangeAddressList;
        emit usedChangeAddressChanged();
    }
}

QStringList Wallet::unUsedChangeddAddressList() const
{
    return m_unUsedChangedAddressList;
}

void Wallet::setUnUsedChangeddAddressList(const QStringList &unUseChangeddAddressList)
{
    if(m_unUsedChangedAddressList != unUseChangeddAddressList){
        m_unUsedChangedAddressList = unUseChangeddAddressList;
        emit unUsedChangeAddressChanged();
    }
}

TransactionListModel *Wallet::transactionHistory() const{
    return m_transactionHistory.data();
}

QTransactionPtr Wallet::getTransactionByIndex(const int index) const
{
    if(m_transactionHistory.data()){
         return m_transactionHistory.data()->getTransactionByIndex(index);
    }
    return NULL;
}

QTransactionPtr Wallet::getTransactionByTxid(const QString &txid) const
{
    if(m_transactionHistory.data()){
         return m_transactionHistory.data()->getTransactionByTxid(txid);
    }
    return NULL;
}

void Wallet::setTransactionHistory(const QTransactionListModelPtr &d) {
    m_transactionHistory = d;
    if(m_transactionHistory){
        m_transactionHistory->requestSort(TransactionListModel::TransactionRoles::transaction_blocktime_role, Qt::DescendingOrder);
    }
    emit transactionHistoryChanged();
}

void Wallet::updateTransaction(const QString txid, const QTransactionPtr &tx)
{
    if(m_transactionHistory){
         m_transactionHistory.data()->updateTransaction(txid, tx);
    }
}

int Wallet::getCreationMode() const
{
    return m_creationMode;
}

void Wallet::setCreationMode(int creationMode)
{
    m_creationMode = creationMode;
}

bool Wallet::getContainsHWSigner() const
{
    if(m_signers){
        return m_signers.data()->containsHardwareKey();
    }
    return false;
}

int Wallet::nShared() const
{
    return m_nShared;
}

void Wallet::setNShared(int d)
{
    if(d != m_nShared){
        m_nShared = qMax(0, d);
        emit nSharedChanged();
    }
}

bool Wallet::isSharedWallet() const
{
    return m_isSharedWallet;
}

void Wallet::setIsSharedWallet(bool isShared)
{
    if(m_isSharedWallet != isShared){
        m_isSharedWallet = isShared;
        emit isSharedWalletChanged();
    }
}

QString Wallet::roomId() const
{
    return m_roomId;
}

void Wallet::setRoomId(const QString &roomId)
{
    if(m_roomId != roomId){
        m_roomId = roomId;
        emit roomIdChanged();
    }
}

QString Wallet::initEventId() const
{
    return m_initEventId;
}

void Wallet::setInitEventId(const QString &initEventId)
{
    if(m_initEventId != initEventId){
        m_initEventId = initEventId;
        emit initEventIdChanged();
    }
}

bool Wallet::isAssistedWallet() const
{
    return AppModel::instance()->getUserWallets().contains(m_id);
}

WalletListModel::WalletListModel(){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

WalletListModel::~WalletListModel(){d_.clear();}

int WalletListModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return d_.count();
}

QVariant WalletListModel::data(const QModelIndex &index, int role) const {
    if(d_[index.row()]){
        switch (role) {
        case wallet_Id_Role:
            return d_[index.row()]->id();
        case wallet_Name_Role:
            return d_[index.row()]->name();
        case wallet_M_Role:
            return d_[index.row()]->m();
        case wallet_N_Role:
            return d_[index.row()]->n();
        case wallet_AddressType_Role:
            return d_[index.row()]->addressType();
        case wallet_createDate_Role:
            return d_[index.row()]->createDate();
        case wallet_Balance_Role:
            return qVariantFromValue(d_[index.row()]->balanceDisplay());
        case wallet_BalanceBTC_Role:
            return qVariantFromValue(d_[index.row()]->balanceBTC());
        case wallet_BalanceCurrency_Role:
            return qVariantFromValue(d_[index.row()]->balanceCurrency());
        case wallet_Escrow_Role:
            return d_[index.row()]->escrow();
        case wallet_SingleSignerList_Role:
            return qVariantFromValue((SingleSignerListModel*)d_[index.row()]->singleSignersAssigned());
        case wallet_Address_Role:
            return d_[index.row()]->address();
        case wallet_usedAddressList_Role:
            return d_[index.row()]->usedAddressList();
        case wallet_unUsedAddressList_Role:
            return d_[index.row()]->unUsedAddressList();
        case wallet_isSharedWallet_Role:
            return d_[index.row()]->isSharedWallet();
        case wallet_isAssistedWallet_Role:
            return d_[index.row()]->isAssistedWallet();
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool WalletListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == wallet_M_Role){
        d_[index.row()]->setM(value.toInt());
        emit dataChanged(index, index, { role } );
    }
    return true;
}

QHash<int, QByteArray> WalletListModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[wallet_Id_Role]   = "wallet_id";
    roles[wallet_Name_Role] = "wallet_name";
    roles[wallet_M_Role]    = "wallet_M";
    roles[wallet_N_Role]    = "wallet_N";
    roles[wallet_AddressType_Role]  = "wallet_AddressType";
    roles[wallet_Balance_Role]      = "wallet_Balance";
    roles[wallet_BalanceBTC_Role]   = "wallet_Balance_BTC";
    roles[wallet_BalanceCurrency_Role]   = "wallet_Balance_Currency";
    roles[wallet_createDate_Role]   = "wallet_CreateDate";
    roles[wallet_Escrow_Role]       = "wallet_Escrow";
    roles[wallet_SingleSignerList_Role]     = "wallet_singleSignersAssigned";
    roles[wallet_Address_Role]              = "wallet_Address";
    roles[wallet_usedAddressList_Role]      = "wallet_usedAddressList";
    roles[wallet_unUsedAddressList_Role]    = "wallet_unUsedAddressList";
    roles[wallet_isSharedWallet_Role]       = "wallet_isSharedWallet";
    roles[wallet_isAssistedWallet_Role]     = "wallet_isAssistedWallet";
    return roles;
}

void WalletListModel::addWallet(const QString &pr_id,
                                const int pr_m,
                                const int pr_n,
                                const QString &pr_name,
                                const QString &pr_addrType,
                                const qint64 pr_balance,
                                const QDateTime &pr_createDate,
                                const bool pr_escrow,
                                QSingleSignerListModelPtr pr_signers,
                                const QString &pr_description)
{
    beginResetModel();
    if(!containsId(pr_id)){
        d_.append(QWalletPtr(new Wallet(pr_id, pr_m, pr_n, pr_name, pr_addrType, pr_balance, pr_createDate, pr_escrow, pr_signers, pr_description)));
    }
    endResetModel();
}

void WalletListModel::addWallet(const QWalletPtr &wallet)
{
    if(wallet && !containsId(wallet.data()->id())){
        beginResetModel();
        d_.append(wallet);
        endResetModel();
    }
}

void WalletListModel::replaceWallet(const QWalletPtr &wallet)
{
    if(wallet){
        for(int i = 0; i < d_.count(); i++){
            if(0 == QString::compare(wallet.data()->id(), d_.at(i)->id(), Qt::CaseInsensitive)){
                d_.replace(i, wallet);
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
        if(containsId(wallet.data()->id())){
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
    for (int i = 0; i < d_.count(); i++) {
        if(d_.at(i).data() && 0 == QString::compare(walletId, d_.at(i)->id(), Qt::CaseInsensitive)){
            d_.at(i)->setBalance(balance);
            emit dataChanged(index(i),index(i));
        }
    }
}

void WalletListModel::updateAddress(const QString &walletId, const QStringList &used, const QStringList &unused)
{
    beginResetModel();
    foreach (QWalletPtr it, d_) {
        if(0 == QString::compare(walletId, it.data()->id(), Qt::CaseInsensitive)){
            it.data()->setUsedAddressList(used);
            it.data()->setunUsedAddressList(unused);
        }
    }
    endResetModel();
}

void WalletListModel::updateName(const QString &walletId, const QString &value)
{
    for (int i = 0; i < d_.count(); i++) {
        if(d_.at(i).data() && 0 == QString::compare(walletId, d_.at(i)->id(), Qt::CaseInsensitive)){
            d_.at(i)->setName(value);
            emit dataChanged(index(i),index(i));
        }
    }
}

void WalletListModel::updateDescription(const QString &walletId, const QString &value)
{
    for (int i = 0; i < d_.count(); i++) {
        if(d_.at(i).data() && 0 == QString::compare(walletId, d_.at(i)->id(), Qt::CaseInsensitive)){
            d_.at(i)->setDescription(value);
            emit dataChanged(index(i),index(i));
        }
    }
}

QStringList WalletListModel::walletListByMasterSigner(const QString& masterSignerId){
    QStringList ret;
    foreach (QWalletPtr i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            bool exist = i.data()->singleSignersAssigned()->containsMasterSignerId(masterSignerId);
            if(true == exist){
                if(i.data()->escrow()){
                    ret << QString("%1 [%2/%3]").arg(i.data()->name()).arg(i.data()->m()).arg(i.data()->n());
                }
                else{
                    ret << QString("%1 [%2/%3]").arg(i.data()->name()).arg(i.data()->m()).arg(i.data()->n());
                }
            }
        }
    }
    return ret;
}

QStringList WalletListModel::walletListByFingerPrint(const QString &masterFingerPrint)
{
    QStringList ret;
    foreach (QWalletPtr i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            bool exist = i.data()->singleSignersAssigned()->contains(masterFingerPrint);
            if(true == exist){
                if(i.data()->escrow()){
                    ret << QString("%1 [%2/%3]").arg(i.data()->name()).arg(i.data()->m()).arg(i.data()->n());
                }
                else{
                    ret << QString("%1 [%2/%3]").arg(i.data()->name()).arg(i.data()->m()).arg(i.data()->n());
                }
            }
        }
    }
    return ret;
}

QWalletPtr WalletListModel::getWalletByIndex(const int index)
{
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
        return NULL;
    }
    else {
        return d_.at(index);
    }
}

QWalletPtr WalletListModel::getWalletById(const QString &walletId)
{
    foreach (QWalletPtr it, d_) {
        if(0 == QString::compare(walletId, it.data()->id(), Qt::CaseInsensitive)){
            return it;
        }
    }
    return QWalletPtr(NULL);
}

bool WalletListModel::removeWallet(const QWalletPtr it)
{
    beginResetModel();
    d_.removeAll(it);
    endResetModel();
    return true;
}

void WalletListModel::removeWallet(const QString &walletId)
{
    beginResetModel();
    foreach (QWalletPtr it, d_) {
        if(0 == QString::compare(walletId, it.data()->id(), Qt::CaseInsensitive)){
            d_.removeOne(it);
            break;
        }
    }
    endResetModel();
}

void WalletListModel::notifyUnitChanged()
{
    const QVector<int>& roles = {};
    for (int i = 0; i < d_.count(); i++) {
        if(d_.at(i).data()){
            const auto idx = index(i);
            emit dataChanged(idx, idx, roles);
        }
    }
}

void WalletListModel::updateSignerHealthStatus(const QString &masterSignerId, const int status, const time_t time)
{
    beginResetModel();
    foreach (QWalletPtr i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            i.data()->singleSignersAssigned()->updateSignerHealthStatus(masterSignerId, status, time);
        }
    }
    endResetModel();
}

void WalletListModel::notifyMasterSignerDeleted(const QString &masterSignerId)
{
    beginResetModel();
    foreach (QWalletPtr i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            i.data()->singleSignersAssigned()->notifyMasterSignerDeleted(masterSignerId);
        }
    }
    endResetModel();
}

int WalletListModel::getWalletIndexById(const QString &walletId)
{
    for (int i = 0; i < d_.count(); i++) {
        if(0 == QString::compare(walletId, d_.at(i).data()->id(), Qt::CaseInsensitive)){
            return i;
        }
    }
    return 0;
}

void WalletListModel::updateHealthCheckTime()
{
    beginResetModel();
    foreach (QWalletPtr i , d_ ){
        i.data()->singleSignersAssigned()->updateHealthCheckTime();
    }
    endResetModel();
}

void WalletListModel::requestSort(int role, int order)
{
    beginResetModel();
    if(d_.count() > 1){
        switch (role) {
        case wallet_Name_Role:
        {
            if(Qt::DescendingOrder == order){
                qSort(d_.begin(), d_.end(), sortWalletByNameDescending);
            }
            else{
                qSort(d_.begin(), d_.end(), sortWalletByNameAscending);
            }
        }
            break;
        default:
            break;
        }
    }
    endResetModel();
}

bool WalletListModel::containsId(const QString &id)
{
    foreach (QWalletPtr i , d_ ){
        if(0 == QString::compare(i.data()->id(), id, Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

void WalletListModel::updateSharedWalletById(const QString &wallet_id, const QString &room_id, const QString &init_id,const QString &name)
{
    beginResetModel();
    foreach (QWalletPtr it , d_ ){
        if(0 == QString::compare(it.data()->id(), wallet_id, Qt::CaseInsensitive)){
            it.data()->setIsSharedWallet(true);
            it.data()->setRoomId(room_id);
            it.data()->setInitEventId(init_id);
            it.data()->setName(name);
        }
    }
    endResetModel();
}

void WalletListModel::updateSignerOfRoomWallet(const QString &wallet_id, const SignerAssigned &signer)
{
    foreach (QWalletPtr it , d_ ){
        if(0 == QString::compare(it.data()->id(), wallet_id, Qt::CaseInsensitive)){
            if(it.data()->singleSignersAssigned()){
                it.data()->singleSignersAssigned()->updateSignerOfRoomWallet(signer);
            }
        }
    }
}

QList<QWalletPtr> WalletListModel::fullList() const
{
    return d_;
}

void WalletListModel::cleardata()
{
    beginResetModel();
    d_.clear();
    endResetModel();
}

bool sortWalletByNameAscending(const QWalletPtr &v1, const QWalletPtr &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) < 0);
}

bool sortWalletByNameDescending(const QWalletPtr &v1, const QWalletPtr &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) > 0);
}
