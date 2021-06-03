#include "WalletModel.h"
#include "qUtils.h"
#include "AppSetting.h"
#include <QQmlEngine>

Wallet::Wallet() :
    id_(""),
    m_(0),
    n_(0),
    name_(""),
    addressType_("2"), // Default is NESTED_SEGWIT
    balance_(0),
    createDate_(QDateTime::currentDateTime()),
    escrow_(false),
    singleSignersAssigned_(QSharedPointer<SingleSignerListModel>(new SingleSignerListModel())),
    transactionHistory_(QSharedPointer<TransactionListModel>(new TransactionListModel())),
    capableCreate_(true),
    description_(""),
    descriptior_(""),
    warningMessage_(QSharedPointer<QWarningMessage>(new QWarningMessage())),
    creationMode_((int)CreationMode::CREATE_NEW_WALLET),
    containsHWSigner_(false)
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
               const QSharedPointer<SingleSignerListModel>& pr_signers,
               const QString &pr_description) :
    id_(pr_id),
    m_(pr_m),
    n_(pr_n),
    name_(pr_name),
    addressType_(pr_addrType),
    balance_(pr_balance),
    createDate_(pr_createDate),
    escrow_(pr_escrow),
    singleSignersAssigned_(pr_signers),
    address_("There is no unused address"),
    capableCreate_(true),
    description_(pr_description),
    descriptior_(""),
    warningMessage_(QSharedPointer<QWarningMessage>(new QWarningMessage())),
    creationMode_((int)CreationMode::CREATE_NEW_WALLET)
{
    unUsedAddressList_.clear();
    usedAddressList_.clear();
    usedChangeAddressList_.clear();
    unUsedChangeddAddressList_.clear();
}

Wallet::~Wallet(){
    singleSignersAssigned_.clear();
    transactionHistory_.clear();
    warningMessage_.clear();
}

QString Wallet::id() const {return id_;}

int Wallet::m() const { return m_;}

int Wallet::n() {
    n_ = singleSignersAssigned_.data()->rowCount();
    return n_;
}

QString Wallet::name() const {return name_;}

QString Wallet::addressType() const {return addressType_;}

qint64 Wallet::balanceSats() const
{
    return balance_;
}

QString Wallet::balanceBTC() const
{
    return qUtils::QValueFromAmount(balance_);
}

QString Wallet::balanceDisplay() const {
    if(1 == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(balanceSats());
    }
    else{
        return balanceBTC();
    }
}

QString Wallet::createDate() const {
    if(0 == createDate_.toTime_t()){
        return "--/--/----"; // There is no time
    }
//    return createDate_.toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate);
    return createDate_.toString( "MM/dd/yyyy hh:mm AP" );
}

QDateTime Wallet::createDateDateTime() const {return createDate_;}

bool Wallet::escrow() const {return escrow_;}

void Wallet::setId(const QString &d)
{
    if(d != id_){
        id_ = d;
        emit idChanged();
    }
}

void Wallet::setM(const int d) {
    if(d != m_){
        m_ = qMax(0, d);
        emit mChanged();
    }
}

void Wallet::setN(const int d) {
    if(d != n_){
        n_ = qMax(0, d);
        emit nChanged();
    }
}

void Wallet::setName(const QString &d) {
    if(d != name_){
        name_ = d;
        emit nameChanged();
    }
}

void Wallet::setAddressType(const QString &d) {
    if(d != addressType_){
        addressType_ = d;
        emit addressTypeChanged();
    }
}

void Wallet::setBalance(const qint64 d) {
    if(d != balance_){
        balance_ = d;
        emit balanceChanged();
    }
}

void Wallet::setCreateDate(const QDateTime &d) {
    if(d != createDate_){
        createDate_ = d;
        emit createDateChanged();
    }
}

void Wallet::setEscrow(const bool d) {
    if(d != escrow_){
        escrow_ = d;
        emit escrowChanged();
    }
}

void Wallet::setSigners(const QSharedPointer<SingleSignerListModel> &d){
    singleSignersAssigned_ = d;
    emit nChanged();
    emit singleSignersAssignedChanged();
}

QSharedPointer<QWarningMessage> Wallet::warningMessagePtr() const
{
    return warningMessage_;
}

QWarningMessage* Wallet::warningMessage() const
{
    return warningMessage_.data();
}

void Wallet::setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage)
{
    warningMessage_ = warningMessage;
    emit warningMessageChanged();
}

QString Wallet::descriptior() const
{
    return descriptior_;
}

void Wallet::setDescriptior(const QString &descriptior)
{
    descriptior_ = descriptior;
}

SingleSignerListModel* Wallet::singleSignersAssigned() const {
    return singleSignersAssigned_.data();
}

QSharedPointer<SingleSignerListModel> Wallet::singleSignersAssignedPtr() const
{
    return singleSignersAssigned_;
}

QString Wallet::address() const {
    return address_;
}

void Wallet::setAddress(const QString &d){
    if(d != address_){
        address_ = d;
        emit addressChanged();
    }
}

QStringList Wallet::usedAddressList() const {
    return usedAddressList_;
}

void Wallet::setUsedAddressList(const QStringList &d){
    if(d != usedAddressList_){
        usedAddressList_ = d;
        emit usedAddressChanged();
    }
}

QStringList Wallet::unUsedAddressList() const {
    return unUsedAddressList_;
}

void Wallet::setunUsedAddressList(const QStringList &d){
    if(d != unUsedAddressList_){
        unUsedAddressList_ = d;
        emit unUsedAddressChanged();
    }
    if(!unUsedAddressList_.isEmpty()){
        setAddress(unUsedAddressList_.first());
    }
}

bool Wallet::capableCreate() const
{
    return capableCreate_;
}

void Wallet::setCapableCreate(bool capableCreate)
{
    if(capableCreate_ != capableCreate){
        capableCreate_ = capableCreate;
        emit capableCreateChanged();
    }
}

QString Wallet::description() const
{
    return description_;
}

void Wallet::setDescription(const QString &description)
{
    if(description_ != description){
        description_ = description;
        emit descriptionChanged();
    }
}

QStringList Wallet::usedChangeAddressList() const
{
    return usedChangeAddressList_;
}

void Wallet::setUsedChangeAddressList(const QStringList &usedChangeAddressList)
{
    if(usedChangeAddressList_ != usedChangeAddressList){
        usedChangeAddressList_ = usedChangeAddressList;
        emit usedChangeAddressChanged();
    }
}

QStringList Wallet::unUsedChangeddAddressList() const
{
    return unUsedChangeddAddressList_;
}

void Wallet::setUnUsedChangeddAddressList(const QStringList &unUseChangeddAddressList)
{
    if(unUsedChangeddAddressList_ != unUseChangeddAddressList){
        unUsedChangeddAddressList_ = unUseChangeddAddressList;
        emit unUsedChangeAddressChanged();
    }
}

TransactionListModel *Wallet::transactionHistory() const{
    if(transactionHistory_.data()){
        return transactionHistory_.data();
    }
    return NULL;
}

QSharedPointer<Transaction> Wallet::getTransactionByIndex(const int index) const
{
    if(transactionHistory_.data()){
         return transactionHistory_.data()->getTransactionByIndex(index);
    }
    return NULL;
}

QSharedPointer<Transaction> Wallet::getTransactionByTxid(const QString &txid) const
{
    if(transactionHistory_.data()){
         return transactionHistory_.data()->getTransactionByTxid(txid);
    }
    return NULL;
}

QSharedPointer<TransactionListModel> Wallet::transactionHistoryPtr() const {
    return transactionHistory_;
}

void Wallet::setTransactionHistory(const QSharedPointer<TransactionListModel> &d) {
    transactionHistory_ = d;
    if(transactionHistory_.data()){
        transactionHistory_.data()->requestSort(TransactionListModel::TransactionRoles::transaction_blocktime_role, Qt::DescendingOrder);
    }
    emit transactionHistoryChanged();
}

int Wallet::getCreationMode() const
{
    return creationMode_;
}

void Wallet::setCreationMode(int creationMode)
{
    creationMode_ = creationMode;
}

bool Wallet::getContainsHWSigner() const
{
    return containsHWSigner_;
}

void Wallet::setContainsHWSigner(bool containsHWSigner)
{
    if(containsHWSigner_ != containsHWSigner){
        containsHWSigner_ = containsHWSigner;
        emit containsHWSignerChanged();
    }
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
    roles[wallet_createDate_Role]   = "wallet_CreateDate";
    roles[wallet_Escrow_Role]       = "wallet_Escrow";
    roles[wallet_SingleSignerList_Role]     = "wallet_singleSignersAssigned";
    roles[wallet_Address_Role]              = "wallet_Address";
    roles[wallet_usedAddressList_Role]      = "wallet_usedAddressList";
    roles[wallet_unUsedAddressList_Role]    = "wallet_unUsedAddressList";
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
                                QSharedPointer<SingleSignerListModel> pr_signers,
                                const QString &pr_description)
{
    beginResetModel();
    d_.append(QSharedPointer<Wallet>(new Wallet(pr_id, pr_m, pr_n, pr_name, pr_addrType, pr_balance, pr_createDate, pr_escrow, pr_signers, pr_description)));
    endResetModel();
}

int WalletListModel::addWallet(const QSharedPointer<Wallet> &wl)
{
    beginResetModel();
    d_.append(wl);
    endResetModel();
    return d_.indexOf(wl);
}

void WalletListModel::updateWalletList(const QSharedPointer<WalletListModel> &d)
{
    beginResetModel();
    d_.clear();
    for (int var = 0; var < d.data()->rowCount(); var++) {
        if(d.data()->getWalletByIndex(var)){
            d_.append(d.data()->getWalletByIndex(var));
        }
    }
    endResetModel();
}

void WalletListModel::updateBalance(const QString &id, const qint64 value)
{
    foreach (QSharedPointer<Wallet> it, d_) {
        if(it.data()->id() == id){
            beginResetModel();
            it.data()->setBalance(value);
            endResetModel();
        }
    }
}

void WalletListModel::updateAddress(const QString &id, const QStringList &used, const QStringList &unused)
{
    foreach (QSharedPointer<Wallet> it, d_) {
        if(it.data()->id() == id){
            beginResetModel();
            it.data()->setUsedAddressList(used);
            it.data()->setunUsedAddressList(unused);
            endResetModel();
        }
    }
}

void WalletListModel::updateName(const QString &id, const QString &value)
{
    foreach (QSharedPointer<Wallet> it, d_) {
        if(it.data()->id() == id){
            beginResetModel();
            it.data()->setName(value);
            endResetModel();
        }
    }
}

void WalletListModel::updateDescription(const QString &id, const QString &value)
{
    foreach (QSharedPointer<Wallet> it, d_) {
        if(it.data()->id() == id){
            beginResetModel();
            it.data()->setDescription(value);
            endResetModel();
        }
    }
}

QStringList WalletListModel::walletListByMasterSigner(const QString& masterSignerId){
    QStringList ret;
    foreach (QSharedPointer<Wallet> i , d_ ){
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
    foreach (QSharedPointer<Wallet> i , d_ ){
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

QSharedPointer<Wallet> WalletListModel::getWalletByIndex(const int index)
{
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
        return NULL;
    }
    else {
        return d_.at(index);
    }
}

QSharedPointer<Wallet> WalletListModel::getWalletById(const QString &id)
{
    foreach (QSharedPointer<Wallet> it, d_) {
        if(it.data()->id() == id){
            return it;
        }
    }
    return QSharedPointer<Wallet>(NULL);
}

bool WalletListModel::removeWallet(const QSharedPointer<Wallet> it)
{
    beginResetModel();
    d_.removeAll(it);
    endResetModel();
    return true;
}

void WalletListModel::removeWallet(const QString &walletID)
{
    beginResetModel();
    foreach (QSharedPointer<Wallet> it, d_) {
        if(it.data()->id() == walletID){
            d_.removeOne(it);
            break;
        }
    }
    endResetModel();
}

void WalletListModel::notifyUnitChanged()
{
    beginResetModel();
    foreach (QSharedPointer<Wallet> it, d_) {
        if(it.data()){
            it.data()->balanceChanged();
        }
    }
    endResetModel();
}

void WalletListModel::updateSignerHealthStatus(const QString &masterSignerId, const int status, const time_t time)
{
    beginResetModel();
    foreach (QSharedPointer<Wallet> i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            i.data()->singleSignersAssigned()->updateSignerHealthStatus(masterSignerId, status, time);
        }
    }
    endResetModel();
}

void WalletListModel::notifyMasterSignerDeleted(const QString &masterSignerId)
{
    beginResetModel();
    foreach (QSharedPointer<Wallet> i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            i.data()->singleSignersAssigned()->notifyMasterSignerDeleted(masterSignerId);
        }
    }
    endResetModel();
}

void WalletListModel::notifyMasterSignerRenamed(const QString &masterSignerId, const QString &newname)
{
    beginResetModel();
    foreach (QSharedPointer<Wallet> i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            i.data()->singleSignersAssigned()->notifyMasterSignerRenamed(masterSignerId, newname);
        }
    }
    endResetModel();
}

void WalletListModel::notifyRemoteSignerRenamed(const QString& fingerprint, const QString &newname)
{
    beginResetModel();
    foreach (QSharedPointer<Wallet> i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            i.data()->singleSignersAssigned()->notifyRemoteSignerRenamed(fingerprint, newname);
        }
    }
    endResetModel();
}

int WalletListModel::getWalletIndexById(const QString &walletId)
{
    int idx = 0;
    foreach (QSharedPointer<Wallet> i, d_ ){
        if(i.data()->id() == walletId){
            return idx;
        }
        idx++;
    }
    return -1;
}

void WalletListModel::updateHealthCheckTime()
{
    beginResetModel();
    foreach (QSharedPointer<Wallet> i , d_ ){
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

bool sortWalletByNameAscending(const QSharedPointer<Wallet> &v1, const QSharedPointer<Wallet> &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) < 0);
}

bool sortWalletByNameDescending(const QSharedPointer<Wallet> &v1, const QSharedPointer<Wallet> &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) > 0);
}
