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

int Wallet::m_flow = 0;
int Wallet::m_walletOptionType = 0;
Wallet::Wallet() :
    m_signers(QSingleSignerListModelPtr(new SingleSignerListModel())),
    m_transactionHistory(QTransactionListModelPtr(new TransactionListModel())),
    m_assignAvailableSigners(QSingleSignerListModelPtr(new SingleSignerListModel())),
    m_creationMode((int)CreationMode::CREATE_NEW_WALLET),
    m_sandbox(QGroupSandboxPtr(new QGroupSandbox())),
    m_conversations(QGroupMessageModelPtr(new QGroupMessageModel()))
{
    m_roomMembers.clear();
    Wallet::init();
    CoinsControl::init();
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Wallet::Wallet(const nunchuk::Wallet &w) : Wallet()
{
    setNunchukWallet(w);
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Wallet::~Wallet(){

}

void Wallet::init()
{
    QObject::connect(this, &Wallet::signalSyncCollabKeyname, this, &Wallet::slotSyncCollabKeyname, Qt::QueuedConnection);
}

void Wallet::convert(const nunchuk::Wallet w)
{
    setNunchukWallet(w);
    serverKeyPtr();
    inheritancePlanPtr();
    QGroupDashboardPtr dash = dashboard();
    if (dash && dash->myInfo().isEmpty()) {
        dash->GetMemberInfo();
        QtConcurrent::run([dash]() {
            if (dash) {
                dash->GetAlertsInfo();
                dash->GetHealthCheckInfo();
            }
        });
        dash->GetWalletInfo();
    }
}

QString Wallet::walletId() const {
    std::string id = nunchukWallet().get_id();
    return id.empty() ? m_id: QString::fromStdString(id);
}

void Wallet::setWalletId(const QString &data)
{
    if(data != m_id){
        m_id = data;
        emit walletChanged();
    }
}

int Wallet::walletM() {
    if(m_walletM == 0){
        m_walletM = qMax(0, nunchukWallet().get_m());
    }
    return m_walletM;
}

void Wallet::setWalletM(const int data) {
    int value = qMax(0, data);
    if(value != m_walletM){
        m_walletM = value;
        m_nunchukWallet.set_m(value);
        emit walletChanged();
    }
}

int Wallet::walletN() {
    if(m_walletN == 0){
        m_walletN = qMax(0, nunchukWallet().get_n());
    }
    return m_walletN;
}

void Wallet::setWalletN(const int data) {
    int value = qMax(0, data);
    if(value != m_walletN){
        m_walletN = value;
        m_nunchukWallet.set_n(value);
        emit walletChanged();
    }
}

int Wallet::collabWalletN() const
{
    return m_collabWalletN;
}

void Wallet::setCollabWalletN(int data)
{
    if(data != m_collabWalletN){
        m_collabWalletN = qMax(0, data);
        emit walletChanged();
    }
}

QString Wallet::walletNameDisplay() {
    QRegularExpression re("\\p{So}");
    QString originname = walletName();
    originname.remove(re);

    QString ret = "";
    if(isByzantineWallet()){
        ret = walletAliasName();
    }
    if(ret == ""){
        ret = originname;
    }

    return ret;
}

QString Wallet::walletName()
{
    QString data_name = QString::fromStdString(nunchukWallet().get_name());
    if(data_name != ""){
        m_walletName = QString::fromStdString(nunchukWallet().get_name());
        if (isReplaced() && !data_name.contains("DEPRECATED")) {
            m_walletName = QString("[DEPRECATED] %1").arg(m_walletName);
        }
    }
    return m_walletName;
}

QString Wallet::walletAliasName() const
{
    if (auto dash = dashboard()) {
        QString alias = dash->walletJson()["alias"].toString();
        QRegularExpression re("\\p{So}");
        alias.remove(re);
        return alias;
    }
    return "";
}

void Wallet::setWalletName(const QString &data) {
    if(!qUtils::strCompare(data, walletName())){
        m_walletName = data;
        m_nunchukWallet.set_name(data.toStdString());
        emit walletChanged();
    }
}

int Wallet::walletAddressType() {
    if(m_walletAddressType == (int)nunchuk::AddressType::ANY){
        int cdata = (int)nunchukWallet().get_address_type();
        std::set<int> valid_numbers = {(int)nunchuk::AddressType::LEGACY, (int)nunchuk::AddressType::NESTED_SEGWIT, (int)nunchuk::AddressType::NATIVE_SEGWIT, (int)nunchuk::AddressType::TAPROOT};
        if (valid_numbers.find(cdata) != valid_numbers.end()) {
            m_walletAddressType = cdata;
        }
    }
    return m_walletAddressType;
}

void Wallet::setWalletAddressType(const int data) {
    if(data != m_walletAddressType){
        m_walletAddressType = data;
        m_nunchukWallet.set_address_type((nunchuk::AddressType)data);
        emit walletChanged();
    }
}

int Wallet::walletType()
{
    if(m_walletType == -1){
        int cdata = (int)nunchukWallet().get_wallet_type();
        std::set<int> valid_numbers = {(int)nunchuk::WalletType::SINGLE_SIG, (int)nunchuk::WalletType::MULTI_SIG, (int)nunchuk::WalletType::ESCROW};
        if (valid_numbers.find(cdata) != valid_numbers.end()) {
            m_walletType = cdata;
        }
    }
    return m_walletType;
}

void Wallet::setWalletType(const int data)
{
    if(data != m_walletType){
        m_walletType = data;
        m_nunchukWallet.set_wallet_type((nunchuk::WalletType)data);
        emit walletChanged();
    }
}

bool Wallet::walletEscrow() const {
    return ((nunchuk::WalletType::ESCROW == nunchukWallet().get_wallet_type()) ? true : false);
}

bool Wallet::enableValuekeyset()
{
    if((int)nunchuk::AddressType::TAPROOT == walletAddressType() && (int)nunchuk::WalletTemplate::DEFAULT == walletTemplate()){
        m_enableValuekeyset = true;
    }
    return m_enableValuekeyset;
}

void Wallet::setEnableValuekeyset(bool data)
{
    if (m_enableValuekeyset != data){
        m_enableValuekeyset = data;
        emit walletChanged();
    }
    int template_type = (int)nunchuk::WalletTemplate::DISABLE_KEY_PATH;
    if(m_enableValuekeyset){
        template_type = (int)nunchuk::WalletTemplate::DEFAULT;
    }
    setWalletTemplate(template_type);
}

int Wallet::walletTemplate()
{
    if(!walletId().isEmpty()){
        int cdata = (int)nunchukWallet().get_wallet_template();
        std::set<int> valid_numbers = {(int)nunchuk::WalletTemplate::DEFAULT, (int)nunchuk::WalletTemplate::DISABLE_KEY_PATH};
        if (valid_numbers.find(cdata) != valid_numbers.end()) {
            m_walletTemplate = cdata;
        }
    }
    return m_walletTemplate;
}

void Wallet::setWalletTemplate(const int data)
{
    if(data != m_walletTemplate){
        m_walletTemplate = data;
        m_nunchukWallet.set_wallet_template((nunchuk::WalletTemplate)data);
        emit walletChanged();
    }
}

qint64 Wallet::balanceSats() const
{
    //2,100,000,000,000,000 Max
    return (nunchukWallet().get_balance());
}

QString Wallet::balanceBTC() const
{
    return qUtils::QValueFromAmount(balanceSats());
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

void Wallet::setWalletBalance(const qint64 data) {
    if(data != balanceSats()){
        m_nunchukWallet.set_balance(data);
        emit walletChanged();
    }
}

QString Wallet::walletCreateDateDisp() const {
    if(0 == walletCreateDate().toTime_t()){
        return "--/--/----"; // There is no time
    }
    else {
        //    return createDate_.toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate);
        return walletCreateDate().toString("MM/dd/yyyy hh:mm AP");
    }
}

QDateTime Wallet::walletCreateDate() const
{
    time_t date = nunchukWallet().get_create_date();
    return QDateTime::fromTime_t(date);
}

int Wallet::walletGapLimit() const
{
    return (nunchukWallet().get_gap_limit());
}

void Wallet::setWalletGapLimit(const int data)
{
    if(data != walletGapLimit()){
        m_nunchukWallet.set_gap_limit(data);
        emit walletChanged();
    }
}

void Wallet::setSigners(const QSingleSignerListModelPtr &d){
    m_signers = d;
    emit walletChanged();
    emit walletSignersChanged();
}

QString Wallet::descriptior() const
{
    return m_descriptior;
}

void Wallet::setDescriptior(const QString &descriptior)
{
    m_descriptior = descriptior;
}

SingleSignerListModel* Wallet::singleSignersAssigned() {
    if(!m_signers){
        m_signers = QSingleSignerListModelPtr(new SingleSignerListModel());
    }
    if(m_signers.data()->signerCount() == 0){
        m_signers->cleardata();
        int m = walletM();
        std::vector<nunchuk::SingleSigner> signers = nunchukWallet().get_signers();
        for (int i = 0; i < signers.size(); i++) {
            nunchuk::SingleSigner signer = signers.at(i);
            QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
            if(enableValuekeyset()){
                if(i < m){
                    ret.data()->setValueKey(true);
                }
            }
            m_signers->addSingleSigner(ret);
        }
    }
    if(m_signers.data()->signerCount() > 1){
        m_signers->requestSort();
    }
    return m_signers.data();
}


QString Wallet::walletDescription()
{
    QString data_description = QString::fromStdString(nunchukWallet().get_description());
    if(data_description != ""){
        m_walletDescription = data_description;
    }
    return m_walletDescription;
}

void Wallet::setWalletDescription(const QString &data)
{
    if(!qUtils::strCompare(data, walletDescription())){
        m_walletDescription = data;
        m_nunchukWallet.set_description(data.toStdString());
        emit walletChanged();
    }
}

bool Wallet::isGlobalGroupWallet() const
{
    if(!AppModel::instance()->groupWalletListPtr()){
        return false;
    }
    else {
        return AppModel::instance()->groupWalletListPtr()->containsId(walletId());
    }
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

QString Wallet::unUseAddress() const
{
    QString wallet_id = walletId();
    QString address;
    QStringList addrs = bridge::nunchukGetUnusedAddresses(wallet_id, false);
    if (addrs.size() > 0) {
        address = addrs.first();
    }
    else {
        address = bridge::nunchukGenNewAddresses(wallet_id,false);
    }
    return address;
}

bool Wallet::isDeleting() const
{
    return m_isDeleting;
}

void Wallet::setIsDeleting(const bool val)
{
    if (m_isDeleting == val)
        return;
    m_isDeleting = val;
    emit isDeletingChanged();
}

bool Wallet::needBackup()
{
    if(isGlobalGroupWallet()){
        bool needBackup = true;
        QWalletCached<QString, QString, QString, QString, bool> data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            needBackup = !data.backedup;
        }
        else {
            setNeedBackup(needBackup);
        }
        return needBackup;
    }
    else {
        return false;
    }
}

void Wallet::setNeedBackup(const bool data)
{
    if(isGlobalGroupWallet()){
        QWalletCached<QString, QString, QString, QString, bool> cache;
        cache.groupId   = groupId();
        cache.slug      = slug();
        cache.myRole    = myRole();
        cache.status    = status();
        cache.backedup  = !data;
        AppSetting::instance()->setWalletCached(walletId(), cache);
        emit needBackupChanged();
    }
}

bool Wallet::keyNeedBackup() const
{
    return m_nunchukWallet.need_backup();
}

void Wallet::setKeyNeedBackup(const bool bVal)
{
    if (bVal != keyNeedBackup()) {
        m_nunchukWallet.set_need_backup(bVal);
        emit walletChanged();
    }
}

int Wallet::limitKeySet() const
{
    return m_limitKeySet;
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
    return isUserWallet() || isGroupWallet();
}

bool Wallet::containsColdcard()
{
    if(m_signers){
        return m_signers.data()->containsColdcard();
    }
    return false;
}

bool Wallet::isArchived() const
{
    return m_nunchukWallet.is_archived();
}

void Wallet::setArchived(bool archived)
{
    m_nunchukWallet.set_archived(archived);
    QWarningMessage msg;
    bridge::UpdateWallet(m_nunchukWallet, msg);
    if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
        QString msg_content = archived ? "Archived wallet" : "Unarchived wallet";
        AppModel::instance()->showToast(0, msg_content, EWARNING::WarningType::SUCCESS_MSG);
    }
    emit walletChanged();
}

nunchuk::Wallet Wallet::nunchukWallet() const
{
    return m_nunchukWallet;
}

void Wallet::setNunchukWallet(const nunchuk::Wallet &data)
{
    m_nunchukWallet = data;
    emit walletChanged();
}

void Wallet::GetAssistedTxs()
{
    DBG_INFO << "FIXME user:" << isUserWallet() << " group:" << isGroupWallet() << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    if(isGroupWallet()){
        GetGroupTxs();
    }
    else if(isUserWallet()){
        GetUserTxs();
    }
    else{}
}

void Wallet::GetAssistedCancelledTxs()
{
    DBG_INFO << "FIXME user:" << isUserWallet() << " group:" << isGroupWallet() << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    if(isGroupWallet()){
        GetGroupCancelledTxs();
    }
    else if(isUserWallet()){
        GetUserCancelledTxs();
    }
    else{}
}

QTransactionPtr Wallet::SyncAssistedTxs(const nunchuk::Transaction &tx)
{
    DBG_INFO << "FIXME user:" << isUserWallet() << " group:" << isGroupWallet() << "wallet status:" << status();
    if(isReplaced()){
        return NULL;
    }
    if(isGroupWallet()){
        return SyncGroupTxs(tx);
    }
    else if(isUserWallet()){
        return SyncUserTxs(tx);
    }
    else{
        //TBD
    }
    return bridge::convertTransaction(tx, walletId());
}

void Wallet::UpdateAssistedTxs(const QString &txid, const QString &memo)
{
    DBG_INFO << "tx_id:" << txid << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    if(isGroupWallet()){
        UpdateGroupTxs(txid, memo);
    }
    else if(isUserWallet()){
        UpdateUserTxs(txid, memo);
    }
    else{
        //TBD
    }
}

void Wallet::CancelAssistedTxs(const QString &txid)
{
    DBG_INFO << "tx_id:" << txid << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    if(isGroupWallet()){
        CancelGroupTxs(txid);
    }
    else if(isUserWallet()){
        CancelUserTxs(txid);
    }
    else{
        //TBD
    }
}

void Wallet::CreateAsisstedTxs(const QString &txid, const QString &psbt, const QString &memo)
{
    DBG_INFO << "tx_id:" << txid << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    if(isGroupWallet()){
        CreateGroupTxs(txid, psbt, memo);
    }
    else if(isUserWallet()){
        CreateUserTxs(txid, psbt, memo);
    }
    else{
        //TBD
    }
}

void Wallet::SignAsisstedTxs(const QString &tx_id, const QString &psbt, const QString &memo)
{
    DBG_INFO << "tx_id:" << tx_id << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    if(isGroupWallet()){
        SignGroupTxs(tx_id, psbt, memo);
    }
    else if(isUserWallet()){
        SignUserTxs(tx_id, psbt, memo);
    }
    else{
        //TBD
    }
}

bool Wallet::RbfAsisstedTxs(const QString &tx_id, const QString &psbt)
{
    DBG_INFO << "tx_id:" << tx_id << "wallet status:" << status();
    if(isReplaced()){
        return false;
    }
    QJsonObject data;
    QString errormsg = "";
    QString wallet_id = walletId();
    if(isGroupWallet()){
        QString group_id = groupId();
        bool ret = Byzantine::instance()->RbfTransaction(group_id, wallet_id, tx_id, psbt, data, errormsg);
        return ret;
    }
    else if(isUserWallet()){
        bool ret = Draco::instance()->assistedRbfTx(wallet_id, tx_id, psbt, data, errormsg);
        return ret;
    }
    else{
        //TBD
    }
    return false;
}

void Wallet::UpdateWallet(const QString &name, const QString &description)
{
    DBG_INFO << name << " description:" << description << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    setWalletName(name);
    setWalletDescription(description);
    DBG_INFO << isGroupWallet() << isUserWallet();
    if(isGroupWallet()){
        UpdateGroupWallet(name, description);
    }
    else if(isUserWallet()){
        UpdateUserWallet(name, description);
    }
    else{
        QWarningMessage msg;
        m_nunchukWallet.set_name(name.toStdString());
        bridge::UpdateWallet(m_nunchukWallet, msg);
        emit walletChanged();
    }
}

QJsonObject Wallet::GetServerKeyInfo(const QString &txid)
{
    DBG_INFO << "txid:" << txid << "wallet status:" << status();
    if(isReplaced()){
        return QJsonObject();
    }
    QString wallet_id = walletId();
    QString group_id = groupId();
    if(isGroupWallet()){
        QJsonObject output;
        QString errormsg = "";
        bool ret = Byzantine::instance()->GetOneTransaction(group_id, wallet_id, txid, output, errormsg);
        if(ret){
            return output;
        }
        else{
            return QJsonObject();
        }
    }
    else if(isUserWallet()){
        return Draco::instance()->assistedWalletGetTx(wallet_id, txid);
    }
    else{
        return QJsonObject();
    }
}

bool Wallet::DeleteAssistedWallet()
{
    DBG_INFO << "wallet status:" << status();
    auto tag = servicesTagPtr();
    QString passwordToken = tag->passwordToken();
    QJsonObject output;
    QString errormsg = "";
    bool ret {false};
    if(isGroupWallet()){
        ret = Byzantine::instance()->DeleteGroupWallet(walletId(), groupId(), {}, passwordToken, tag->secQuesToken(), output, errormsg);
    }
    else if(isUserWallet()){
        ret = Draco::instance()->DeleteAssistedWallet(walletId(), {}, passwordToken, tag->secQuesToken(), output, errormsg);
    }
    if (ret) {
        setIsDeleting(false);
    }
    return ret;
}

bool Wallet::DeleteWalletRequiredSignatures()
{
    DBG_INFO << "wallet status:" << status();
    auto tag = servicesTagPtr();
    ReqiredSignaturesInfo info;
    QString errormsg = "";
    QJsonObject output;
    bool ret {false};
    if(isGroupWallet()){
        ret = Byzantine::instance()->DeleteGroupWalletRequiredSignatures(walletId(), groupId(), output, errormsg);
    }
    else if(isUserWallet()){
        ret = Draco::instance()->DeleteWalletRequiredSignatures(walletId(), output, errormsg);
    }
    if (ret) {
        DBG_INFO << output;
        QJsonObject resultObj = output["result"].toObject();
        tag->setReqiredSignatures(resultObj);
        info = tag->reqiredSignaturesInfo();
        if (info.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            if (tag->CreateSecurityQuestionsAnswered()) {
                DBG_INFO;
                setIsDeleting(true);
//                AppModel::instance()->walletInfoPtr()->setIsDeleting(true);
            }
        }
    }
    return ret;
}

void Wallet::getChatInfo()
{
    if(isReplaced()){
        return;
    }
    if(isGroupWallet() && dashboard()){
        dashboard()->getChatInfo();
    }
}

QVariantList Wallet::aliasMembers() const
{
    return m_aliasMembers.toVariantList();
}

QVariantList Wallet::ownerMembers() const
{
    if (auto dash = dashboard()) {
        QJsonArray origin = dash->groupInfo()["members"].toArray();
        QJsonArray arrs;
        for (auto member : origin) {
            QJsonObject it = member.toObject();
            QJsonObject user = it["user"].toObject();
            QString role = it["role"].toString();
            if (role == "KEYHOLDER" || role == "MASTER" || role == "ADMIN" || role == "FACILITATOR_ADMIN") {
                if (!user.isEmpty()) {
                    arrs.append(member);
                }
            }
        }
        return arrs.toVariantList();
    }
    return {};
}

QVariant Wallet::ownerPrimary() const
{
    if (auto dash = dashboard()) {
        QString primary_membership_id =  dash->walletJson()["primary_membership_id"].toString();
        QJsonArray origin = dash->groupInfo()["members"].toArray();
        for (auto member : origin) {
            QJsonObject it = member.toObject();
            QJsonObject user = it["user"].toObject();
            QString membership_id = it["membership_id"].toString();
            if (qUtils::strCompare(primary_membership_id, membership_id)) {
                return QVariant::fromValue(user);
            }
        }
    }
    return {};
}

bool Wallet::GetWalletAlias()
{
    QJsonObject output;
    QString errormsg;
    bool ret = Byzantine::instance()->GetWalletAlias(groupId(), walletId(), output, errormsg);
    DBG_INFO << output;
    if (ret) {
        auto findMember = [this](const QString& id) -> QJsonObject {
            if (auto dash = dashboard()) {
                QJsonArray members = dash->groupInfo()["members"].toArray();
                QString my_membership_id = dash->myInfo()["membership_id"].toString();
                for (auto member : members) {
                    QJsonObject it = member.toObject();
                    QString membership_id = it["membership_id"].toString();
                    if(qUtils::strCompare(id, membership_id) && !qUtils::strCompare(id, my_membership_id)) {
                        return it["user"].toObject();
                    }
                }
            }
            return {};
        };
        QJsonArray aliases = output["aliases"].toArray();
        QJsonArray arrs;
        for (auto js : aliases) {
            QJsonObject it = js.toObject();
            QString membership_id = it["membership_id"].toString();
            QJsonObject membership = findMember(membership_id);
            if (!membership.isEmpty()) {
                it["user"] = membership;
                arrs.append(it);
            }
        }
        m_aliasMembers = arrs;
        emit aliasMembersChanged();
    }
    return ret;
}

bool Wallet::updateWalletAlias(const QString &nameWallet)
{
    QJsonObject output;
    QString errormsg;
    bool ret = Byzantine::instance()->UpdateWalletAlias(groupId(), walletId(), nameWallet, output, errormsg);
    if(ret){
        QGroupDashboardPtr dash = dashboard();
        if (dash) {
            dash->GetWalletInfo();
            QtConcurrent::run([this, dash]() {
                if(AppModel::instance()->walletList()){
                    AppModel::instance()->walletList()->dataUpdated(walletId());
                }
                emit walletChanged();
            });
        }
    }
    DBG_INFO << ret << output;
    return ret;
}

bool Wallet::deleteWalletAlias()
{
    QJsonObject output;
    QString errormsg;
    bool ret = Byzantine::instance()->DeleteWalletAlias(groupId(), walletId(), output, errormsg);
    if(ret){
        QGroupDashboardPtr dash = dashboard();
        if (dash) {
            dash->GetWalletInfo();
            QtConcurrent::run([this, dash]() {
                if(AppModel::instance()->walletList()){
                    AppModel::instance()->walletList()->dataUpdated(walletId());
                }
                emit walletChanged();
            });
        }
    }
    DBG_INFO << output;
    return ret;
}

bool Wallet::updateWalletPrimaryOwner(const QString &membership_id)
{
    QJsonObject output;
    QString errormsg;
    bool ret = Byzantine::instance()->UpdateWalletPrimaryOwner(groupId(), walletId(), membership_id, output, errormsg);
    if(ret){
        QGroupDashboardPtr dash = dashboard();
        if (dash) {
            dash->GetWalletInfo();
        }
    }
    DBG_INFO << output;
    return ret;
}

bool Wallet::isContainKey(const QString &xfp)
{
    for (auto signer : m_signers->fullList()) {
        if (qUtils::strCompare(signer->masterFingerPrint(), xfp) && isAssistedWallet()) {
            return true;
        }
    }
    return false;
}

QVariant Wallet::dummyTx() const
{
    return QVariant::fromValue(groupDummyTxPtr().data());
}

bool Wallet::isPro()
{
    if (auto dash = dashboard()) {
        return dash->inheritanceCount() > 0;
    }
    return false;
}

bool Wallet::hasGroup()
{
    return !groupId().isEmpty();
}

int Wallet::flow() const
{
    return m_flow;
}

void Wallet::setFlow(int flow)
{
    m_flow = flow;
    emit flowChanged();
}

int Wallet::walletOptType() const
{
    return m_walletOptionType;
}

void Wallet::setWalletOptType(int optType)
{
    m_walletOptionType = optType;
    emit walletOptTypeChanged();
}

bool Wallet::isReplaceGroupWallet() const
{
    if (m_sandbox) {
        return !m_sandbox->sandbox().get_replace_wallet_id().empty();
    }
    return false;
}

QGroupSandboxPtr Wallet::groupSandboxPtr()
{
    return m_sandbox;
}

void Wallet::updateGroupSandbox(const nunchuk::GroupSandbox &value)
{
    if(m_sandbox){
        m_sandbox.data()->setSandbox(value);
        emit groupSandboxChanged();
    }
}

void Wallet::setGroupSandbox(QGroupSandboxPtr sandbox)
{
    m_sandbox = sandbox;
    emit groupSandboxChanged();
}

QGroupSandbox *Wallet::groupSandbox()
{
    return groupSandboxPtr().data();
}

void Wallet::GetUserTxs()
{
    if(isUserWallet()){
        QString wallet_id = walletId();
        QJsonObject data = Draco::instance()->assistedWalletGetListTx(wallet_id);
        QJsonArray transactions = data["transactions"].toArray();
        for(QJsonValue js_value : transactions){
            QJsonObject transaction = js_value.toObject();
            QString status = transaction.value("status").toString();
            QString psbt = transaction.value("psbt").toString();
            QString memo = transaction.value("note").toString();;
            QString type = transaction.value("type").toString();
            QString transaction_id = transaction.value("transaction_id").toString();
            if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES" ) {
                QWarningMessage _msg;
                QTransactionPtr tran = bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
                if(tran && (int)EWARNING::WarningType::NONE_MSG == _msg.type()){
                    if(transactionHistory() && transactionHistory()->contains(transaction_id)){
                        transactionHistory()->updateTransactionMemo(transaction_id, memo);
                    }
                    else {
                        bridge::nunchukUpdateTransactionMemo(wallet_id, transaction_id, memo);
                    }
                    long int broadcast_time_milis = static_cast<long int>(transaction.value("broadcast_time_milis").toDouble());
                    // honey badger feature: schedule broadcast
                    long int current_time_stamp_milis = static_cast<long int>(std::time(nullptr)) * 1000;
                    if(type == "SCHEDULED" && broadcast_time_milis > current_time_stamp_milis) {
                        bridge::nunchukUpdateTransactionSchedule(wallet_id, transaction_id, broadcast_time_milis/1000,_msg);
                    }
                }
            }
        }
        //Remove cancelled txs
        GetUserCancelledTxs();
        GetUserTxNotes();
    }
}

void Wallet::GetUserCancelledTxs()
{
    if(isUserWallet()){
        int offset = 0;
        const int limit = 10;
        QString wallet_id = walletId();
        while (true) {
            QJsonObject data = Draco::instance()->assistedWalletDeleteListTx(wallet_id, offset, limit);
            QJsonArray transactions = data.value("transactions").toArray();
            for (QJsonValue js_value : transactions) {
                QJsonObject transaction = js_value.toObject();
                QString wallet_local_id = transaction.value("wallet_local_id").toString();
                QString transaction_id = transaction.value("transaction_id").toString();
                if(transactionHistory() && transactionHistory()->contains(transaction_id)){
                    bridge::nunchukDeleteTransaction(wallet_local_id, transaction_id);
                }
            }
            if (transactions.size() == 0 || transactions.size() < limit) {
                return; // exit while loop
            }
            offset += transactions.size();
        }
    }
}

void Wallet::GetUserTxNotes()
{
    if(isUserWallet()){
        QString wallet_id = walletId();
        QJsonObject output;
        QString errormsg = "";
        bool ret = Draco::instance()->assistedWalletGetTxNotes(wallet_id, output, errormsg);
        if(ret && transactionHistory() && output.contains("notes")){
            QJsonArray notes = output["notes"].toArray();
            for (auto i : notes) {
                QJsonObject note = i.toObject();
                transactionHistory()->updateTransactionMemo(note["transaction_id"].toString(), note["note"].toString());
            }
        }
    }
}

QString Wallet::GetUserTxNote(const QString &txid)
{
    if(isUserWallet()){
        QString wallet_id = walletId();
        QJsonObject output;
        QString errormsg = "";
        bool ret = Draco::instance()->assistedWalletGetTxNotes(wallet_id, output, errormsg);
        if(ret && transactionHistory() && output.contains("notes")){
            QJsonArray notes = output["notes"].toArray();
            for (auto i : notes) {
                QJsonObject note = i.toObject();
                if(qUtils::strCompare(note["transaction_id"].toString(), txid)){
                    return note["note"].toString();
                }
            }
        }
    }
    return "";
}

QTransactionPtr Wallet::SyncUserTxs(const nunchuk::Transaction &tx)
{
    QString wallet_id = walletId();
    if(isUserWallet()){
        QWarningMessage msg;
        if (tx.get_status() == nunchuk::TransactionStatus::PENDING_SIGNATURES || tx.get_status() == nunchuk::TransactionStatus::READY_TO_BROADCAST)
        {
            QJsonObject data = Draco::instance()->assistedWalletGetTx(wallet_id,QString::fromStdString(tx.get_txid()));
            QJsonObject transaction = data.value("transaction").toObject();
            if (!transaction.isEmpty()) {
                QString type = transaction.value("type").toString();
                QString status = transaction.value("status").toString();
                QString psbt = transaction.value("psbt").toString();
                QString transaction_id = transaction.value("transaction_id").toString();
                QString hex = transaction.value("hex").toString();
                QString reject_msg = transaction.value("reject_msg").toString();
                QString note = transaction.value("note").toString();
                QString replace_txid = transaction.value("replace_txid").toString();
                long int broadcast_time_milis = static_cast<long int>(transaction.value("broadcast_time_milis").toDouble());
                // honey badger feature: schedule broadcast
                long int current_time_stamp_milis = static_cast<long int>(std::time(nullptr)) * 1000;
                if(type == "SCHEDULED" && broadcast_time_milis > current_time_stamp_milis &&
                    broadcast_time_milis / 1000 != tx.get_schedule_time()) {
                    bridge::nunchukUpdateTransactionSchedule(wallet_id, QString::fromStdString(tx.get_txid()), broadcast_time_milis/1000,msg);
                }
                else if (type != "SCHEDULED" && tx.get_schedule_time() != -1) {
                    bridge::nunchukUpdateTransactionSchedule(wallet_id, QString::fromStdString(tx.get_txid()), -1,msg);
                }
                else{}
                if (status == "PENDING_CONFIRMATION" || status == "CONFIRMED" || status == "NETWORK_REJECTED") {
                    msg.resetWarningMessage();
                    bridge::nunchukImportPsbt(wallet_id, psbt,msg);
                    msg.resetWarningMessage();
                    bridge::nunchukUpdateTransaction(wallet_id, QString::fromStdString(tx.get_txid()), transaction_id, hex, reject_msg, msg);
                }
                else if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES") {
                    msg.resetWarningMessage();
                    auto trans = bridge::nunchukImportPsbt(wallet_id, psbt,msg);
                    if(trans && (int)EWARNING::WarningType::NONE_MSG == msg.type()){
                        if (trans->psbt() != psbt) {
                            Draco::instance()->assistedSyncTx(wallet_id, transaction_id, psbt, note);
                        }
                        if (replace_txid != "" && !qUtils::strCompare(QString::fromStdString(tx.get_replace_txid()), replace_txid)) {
                            bridge::nunchukReplaceTransactionId(wallet_id, transaction_id, replace_txid, msg);
                        }
                    }
                }
                else{}
                QTransactionPtr trans = bridge::nunchukGetTransaction(wallet_id, transaction_id);
                if(trans){
                    if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES" ) {
                        trans.data()->setMemo(note);
                    }
                }
                return trans;
            }
        }
        else {
            QString note = GetUserTxNote(QString::fromStdString(tx.get_txid()));
            QTransactionPtr trans = bridge::convertTransaction(tx, wallet_id);
            if(trans){
                trans.data()->setMemo(note);
            }
            return trans;
        }
    }
    return bridge::convertTransaction(tx, wallet_id);
}

void Wallet::UpdateUserTxs(const QString &txid, const QString &memo)
{
    if(isUserWallet()){
        Draco::instance()->assistedWalletUpdateTx(walletId(),txid, memo);
    }
}

void Wallet::CancelUserTxs(const QString &txid)
{
    if(isUserWallet()){
        bool ret = Draco::instance()->assistedWalletCancelTx(walletId(), txid);
        if(ret){
            //TDB
        }
        else{
            //TDB
        }
    }
}

void Wallet::CreateUserTxs(const QString &txid, const QString &psbt, const QString &memo)
{
    if(isUserWallet()){
        bool ret = Draco::instance()->assistedWalletCreateTx(walletId(), psbt, memo);
        if(ret){
//            QJsonObject serverKeyData = GetServerKeyInfo(txid);
//            if(!serverKeyData.isEmpty() && AppModel::instance()->transactionInfo()){
//                AppModel::instance()->transactionInfo()->setServerKeyMessage(serverKeyData);
//            }
        }
    }
}

void Wallet::SignUserTxs(const QString &tx_id, const QString &psbt, const QString &memo)
{
    if(isUserWallet()){
        QString wallet_id = walletId();
        QJsonObject data =  Draco::instance()->assistedWalletSignTx(wallet_id, tx_id, psbt, memo);
        QJsonObject transaction = data.value("transaction").toObject();
        QString status = transaction.value("status").toString();
        QString psbt = transaction.value("psbt").toString();
        if (qUtils::strCompare(status, "PENDING_CONFIRMATION") || qUtils::strCompare(status, "CONFIRMED") || qUtils::strCompare(status, "NETWORK_REJECTED")){
            QWarningMessage _msg;
            bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
            QString id = transaction.value("id").toString();
            QString hex = transaction.value("hex").toString();
            QString reject_msg = transaction.value("reject_msg").toString();
            bridge::nunchukUpdateTransaction(wallet_id, tx_id, id, hex, reject_msg, _msg);
        }
        else if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES") {
            QWarningMessage _msg;
            bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
        }
        QJsonObject serverKeyData = GetServerKeyInfo(tx_id);
        if(!serverKeyData.isEmpty() && AppModel::instance()->transactionInfo()){
            AppModel::instance()->transactionInfo()->setServerKeyMessage(serverKeyData);
        }
        else{}
    }
}

void Wallet::UpdateUserWallet(const QString &name, const QString &description)
{
    if(isUserWallet()){
        QJsonObject data;
        QString error_msg = "";
        QString wallet_id = walletId();
        bool ret = Draco::instance()->assistedWalletUpdate(wallet_id, name, description, data, error_msg);
        if(!ret){
            //TBD
        }
    }
}

void Wallet::GetGroupTxs()
{
    if(isGroupWallet()){
        QJsonObject output;
        QString errormsg = "";
        QString wallet_id = walletId();
        QString group_id = groupId();
        bool ret = Byzantine::instance()->GetAllTransaction(group_id, wallet_id, output, errormsg);
        if(ret){
            QJsonArray transactions = output["transactions"].toArray();
            for (auto i : transactions) {
                QJsonObject transaction = i.toObject();
                QString status = transaction.value("status").toString();
                QString psbt = transaction.value("psbt").toString();
                QString memo = transaction.value("note").toString();;
                QString type = transaction.value("type").toString();
                QString transaction_id = transaction.value("transaction_id").toString();
                if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES" ) {
                    QWarningMessage warningmsg;
                    QTransactionPtr tran = bridge::nunchukImportPsbt(wallet_id, psbt, warningmsg);
                    if(tran && (int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                        if(transactionHistory() && transactionHistory()->contains(transaction_id)){
                            transactionHistory()->updateTransactionMemo(transaction_id, memo);
                        }
                        else {
                            bridge::nunchukUpdateTransactionMemo(wallet_id, transaction_id, memo);
                        }
                        long int broadcast_time_milis = static_cast<long int>(transaction.value("broadcast_time_milis").toDouble());
                        // honey badger feature: schedule broadcast
                        long int current_time_stamp_milis = static_cast<long int>(std::time(nullptr)) * 1000;
                        if(type == "SCHEDULED" && broadcast_time_milis > current_time_stamp_milis) {
                            warningmsg.resetWarningMessage();
                            bridge::nunchukUpdateTransactionSchedule(wallet_id, transaction_id, broadcast_time_milis/1000, warningmsg);
                        }
                    }
                    warningmsg.resetWarningMessage();
                }
            }
        }
        //Remove cancelled txs
        GetGroupCancelledTxs();
        GetGroupTxNotes();
    }
}

void Wallet::GetGroupCancelledTxs()
{
    if(isGroupWallet()){
        QJsonObject output;
        QString errormsg = "";
        int offset = 0;
        const int limit = 10;
        QString wallet_id = walletId();
        QString group_id = groupId();
        while (true) {
            bool ret = Byzantine::instance()->GetAllCancelledTransaction(group_id, wallet_id, offset, limit, output, errormsg);
            if(ret){
                QJsonArray transactions = output.value("transactions").toArray();
                for (QJsonValue js_value : transactions) {
                    QJsonObject transaction = js_value.toObject();
                    QString wallet_local_id = transaction.value("wallet_local_id").toString();
                    QString transaction_id = transaction.value("transaction_id").toString();
                    if(transactionHistory() && transactionHistory()->contains(transaction_id)){
                        bridge::nunchukDeleteTransaction(wallet_local_id, transaction_id);
                    }
                }
                if (transactions.size() == 0 || transactions.size() < limit) {
                    return; // exit while loop
                }
                offset += transactions.size();
            }
            else{
                return;
            }
        }
    }
}

void Wallet::GetGroupTxNotes()
{
    if(isGroupWallet()){
        QString wallet_id = walletId();
        QString group_id = groupId();
        QJsonObject output;
        QString errormsg = "";
        bool ret = Byzantine::instance()->GetAllTransactionNotes(group_id, wallet_id, output, errormsg);
        if(ret && transactionHistory() && output.contains("notes")){
            QJsonArray notes = output["notes"].toArray();
            for (auto i : notes) {
                QJsonObject note = i.toObject();
                transactionHistory()->updateTransactionMemo(note["transaction_id"].toString(), note["note"].toString());
            }
        }
    }
}

QString Wallet::GetGroupTxNote(const QString &txid) {
    if(isGroupWallet()){
        QString wallet_id = walletId();
        QString group_id = groupId();
        QJsonObject output;
        QString errormsg = "";
        bool ret = Byzantine::instance()->GetAllTransactionNotes(group_id, wallet_id, output, errormsg);
        if(ret && transactionHistory() && output.contains("notes")){
            QJsonArray notes = output["notes"].toArray();
            for (auto i : notes) {
                QJsonObject note = i.toObject();
                if(qUtils::strCompare(note["transaction_id"].toString(), txid)){
                    return note["note"].toString();
                }
            }
        }
    }
    return "";
}

QTransactionPtr Wallet::SyncGroupTxs(const nunchuk::Transaction &tx)
{
    QString wallet_id = walletId();
    if(isGroupWallet()){
        QWarningMessage msg;
        if (tx.get_status() == nunchuk::TransactionStatus::PENDING_SIGNATURES || tx.get_status() == nunchuk::TransactionStatus::READY_TO_BROADCAST)
        {
            QString group_id = groupId();
            QJsonObject data;
            QString msgerror = "";
            bool ret = Byzantine::instance()->GetOneTransaction(group_id, wallet_id, QString::fromStdString(tx.get_txid()), data, msgerror);
            if(ret){
                QJsonObject transaction = data.value("transaction").toObject();
                QString type = transaction.value("type").toString();
                QString status = transaction.value("status").toString();
                QString psbt = transaction.value("psbt").toString();
                QString transaction_id = transaction.value("transaction_id").toString();
                QString hex = transaction.value("hex").toString();
                QString reject_msg = transaction.value("reject_msg").toString();
                QString note = transaction.value("note").toString();
                long int broadcast_time_milis = static_cast<long int>(transaction.value("broadcast_time_milis").toDouble());
                // honey badger feature: schedule broadcast
                long int current_time_stamp_milis = static_cast<long int>(std::time(nullptr)) * 1000;
                if(type == "SCHEDULED" && broadcast_time_milis > current_time_stamp_milis &&
                    broadcast_time_milis / 1000 != tx.get_schedule_time()) {
                    bridge::nunchukUpdateTransactionSchedule(wallet_id, QString::fromStdString(tx.get_txid()), broadcast_time_milis/1000,msg);
                }
                else if (type != "SCHEDULED" && tx.get_schedule_time() != -1) {
                    bridge::nunchukUpdateTransactionSchedule(wallet_id, QString::fromStdString(tx.get_txid()), -1,msg);
                }
                else{}

                if (status == "PENDING_CONFIRMATION" || status == "CONFIRMED" || status == "NETWORK_REJECTED") {
                    bridge::nunchukImportPsbt(wallet_id, psbt, msg);
                    msg.resetWarningMessage();
                    bridge::nunchukUpdateTransaction(wallet_id, QString::fromStdString(tx.get_txid()), transaction_id, hex, reject_msg, msg);
                    msg.resetWarningMessage();
                }
                else if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES") {
                    msg.resetWarningMessage();
                    auto tran = bridge::nunchukImportPsbt(wallet_id, psbt, msg);
                    if(tran && (int)EWARNING::WarningType::NONE_MSG == msg.type()){
                        if (tran->psbt() != psbt) {
                            QJsonObject dataSync;
                            QString syncError = "";
                            Byzantine::instance()->SyncTransaction(group_id, wallet_id, transaction_id, psbt, note, dataSync, syncError);
                        }
                    }
                }
                else{}
                QTransactionPtr trans = bridge::nunchukGetTransaction(wallet_id, transaction_id);
                if(trans ){
                    if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES" ) {
                        trans.data()->setMemo(note);
                    }
                }
                return trans;
            }
        }
        else {
            QString note = GetGroupTxNote(QString::fromStdString(tx.get_txid()));
            QTransactionPtr trans = bridge::convertTransaction(tx, wallet_id);
            if(trans){
                trans.data()->setMemo(note);
            }
            return trans;
        }
    }
    return bridge::convertTransaction(tx, wallet_id);
}

void Wallet::UpdateGroupTxs(const QString &txid, const QString &memo)
{
    if(isGroupWallet()){
        QJsonObject output;
        QString errormsg = "";
        bool ret = Byzantine::instance()->UpdateTransaction(groupId(), walletId(), txid, memo, output, errormsg);
        if(ret){
            //TDB
        }
        else{
            //TDB
        }
    }
}

void Wallet::CancelGroupTxs(const QString &txid)
{
    if(isGroupWallet()){
        QJsonObject output;
        QString errormsg = "";
        bool ret = Byzantine::instance()->CancelTransaction(groupId(), walletId(), txid, output, errormsg);
        if(ret){
            //TDB
        }
        else{
            //TDB
        }
    }
}

void Wallet::CreateGroupTxs(const QString &txid, const QString &psbt, const QString &memo)
{
    if(isGroupWallet()){
        QJsonObject output;
        QString errormsg = "";
        bool ret = Byzantine::instance()->CreateTransaction(groupId(), walletId(), psbt, memo, output, errormsg);
        if(ret){
//            QJsonObject serverKeyData = GetServerKeyInfo(txid);
//            if(!serverKeyData.isEmpty() && AppModel::instance()->transactionInfo()){
//                AppModel::instance()->transactionInfo()->setServerKeyMessage(serverKeyData);
//            }
        }
        else{
            //TDB
        }
    }
}

void Wallet::SignGroupTxs(const QString &tx_id, const QString &psbt, const QString &memo)
{
    if(isGroupWallet()){
        QJsonObject data;
        QString errormsg = "";
        QString wallet_id = walletId();
        QString group_id = groupId();
        bool ret = Byzantine::instance()->SignTransaction(group_id, wallet_id, tx_id, psbt, memo, data, errormsg);
        if(ret){
            QJsonObject transaction = data.value("transaction").toObject();
            QString status = transaction.value("status").toString();
            QString psbt = transaction.value("psbt").toString();
            if (qUtils::strCompare(status, "PENDING_CONFIRMATION") || qUtils::strCompare(status, "CONFIRMED") || qUtils::strCompare(status, "NETWORK_REJECTED")){
                QWarningMessage _msg;
                bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
                QString id = transaction.value("id").toString();
                QString hex = transaction.value("hex").toString();
                QString reject_msg = transaction.value("reject_msg").toString();
                bridge::nunchukUpdateTransaction(wallet_id, tx_id, id, hex, reject_msg, _msg);
            }
            else if (qUtils::strCompare(status, "READY_TO_BROADCAST") || qUtils::strCompare(status, "PENDING_SIGNATURES")){
                QWarningMessage _msg;
                bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
            }
            QJsonObject serverKeyData = GetServerKeyInfo(tx_id);
            if(!serverKeyData.isEmpty() && AppModel::instance()->transactionInfo()){
                AppModel::instance()->transactionInfo()->setServerKeyMessage(serverKeyData);
            }
            else{}
        }
    }
}

void Wallet::UpdateGroupWallet(const QString &name, const QString &description)
{
    if(isGroupWallet()){
        QJsonObject data;
        QString error_msg = "";
        QString wallet_id = walletId();
        QString group_id  = groupId();
        bool ret = Byzantine::instance()->UpdateWallet(group_id, wallet_id, name, description, data, error_msg);
        if(!ret){
            //TBD
        }
    }
}

bool Wallet::showbubbleChat() const
{
    return m_showbubbleChat;
}

void Wallet::setShowbubbleChat(bool data)
{
    if (m_showbubbleChat == data)
        return;
    m_showbubbleChat = data;
    emit showbubbleChatChanged();
}

void Wallet::markAsReadMessage(const QString &msg_id)
{
    bridge::SetLastReadMessage(walletId(), msg_id);
}

void Wallet::markFiveMessagesAsRead()
{
    if(conversations()){
        conversations()->markFiveMessagesAsRead();
        int number = bridge::GetUnreadMessagesCount(walletId());
        if (auto list = AppModel::instance()->walletListPtr()) {
            list->updateUnreadMessage(walletId(),number);
        }
        if (auto list = AppModel::instance()->groupWalletListPtr()) {
            list->updateUnreadMessage(walletId(),number);
        }
    }
}

void Wallet::markAllMessagesAsRead()
{
    if(conversations()){
        conversations()->markAllMessagesAsRead();
        if (auto list = AppModel::instance()->walletListPtr()) {
            list->updateUnreadMessage(walletId(),0);
        }
        if (auto list = AppModel::instance()->groupWalletListPtr()) {
            list->updateUnreadMessage(walletId(),0);
        }
    }
}

void Wallet::CreateSignerListReviewWallet(const std::vector<nunchuk::SingleSigner> &signers)
{
    if (auto signersList = singleSignersAssigned()) {
        if (signersList->rowCount() == walletN()) return;
        signersList->cleardata();
        for (int i = 0; i < signers.size(); i++) {
            nunchuk::SingleSigner signer = signers.at(i);
            QWarningMessage msg;
            nunchuk::SingleSigner localSigner = bridge::nunchukGetOriginSingleSigner(signer, msg);
            if (localSigner.is_visible() && bridge::nunchukHasSinger(localSigner) && (int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(localSigner));
                signersList->addSingleSigner(ret);
            } else {
                QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
                ret->setName(QString("Key #%1").arg(i + 1));
                signersList->addSingleSigner(ret);
            }
        }
    }
}

void Wallet::CreateAssignAvailableSigners()
{
    if (m_assignAvailableSigners.isNull()) return;
    m_assignAvailableSigners->cleardata();
    auto masterList = AppModel::instance()->masterSignerList();
    auto singleList = AppModel::instance()->remoteSignerList();
    auto nw = AppModel::instance()->newWalletInfo();
    if (masterList && nw && singleList) {
        auto signers = nw->assignAvailableSigners();
        if (signers) {
            for (auto master : masterList->fullList()) {
                signers->addSingleSigner(master->cloneSingleSigner());
            }
            for (auto single : singleList->fullList()) {
                signers->addSingleSigner(single);
            }
            signers->signers();
        }
    }
    emit assignAvailableSignersChanged();
}

SingleSignerListModel *Wallet::assignAvailableSigners()
{
    return m_assignAvailableSigners.data();
}

void Wallet::GetReplaceGroups()
{
    QPointer<Wallet> safeThis(this);
    runInThread([safeThis]() ->QMap<QString, bool>{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QWarningMessage msg;
        return bridge::GetReplaceGroups(ptrLamda->walletId(), msg);
    },[safeThis](QMap<QString, bool> groups) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        QJsonArray array;
        for (auto it = groups.begin(); it != groups.end(); ++it) {
            QJsonObject js;
            js["group_id"] = it.key();
            js["accepted"] = it.value();
            array.append(js);
        }
        ptrLamda->m_replaceGroups = array;
        DBG_INFO << array;
        emit ptrLamda->replaceGroupsChanged();
    });
}

QVariantList Wallet::replaceGroups()
{
    return m_replaceGroups.toVariantList();
}

QVariant Wallet::serverKeyInfo() const
{
    return QVariant::fromValue(serverKeyPtr().data());
}

QServerKeyPtr Wallet::serverKeyPtr() const
{
    return QServerKey::information<QServerKeyPtr>(walletId());
}

QVariant Wallet::inheritancePlanInfo() const
{
    return QVariant::fromValue(inheritancePlanPtr().data());
}

QInheritancePlanPtr Wallet::inheritancePlanPtr() const
{
    return QInheritancePlan::information<QInheritancePlanPtr>(walletId());
}

QVariant Wallet::recurringPayment() const
{
    return QVariant::fromValue(recurringPaymentPtr().data());
}

QRecurringPaymentPtr Wallet::recurringPaymentPtr() const
{
    return QRecurringPayment::information<QRecurringPaymentPtr>(walletId());
}

QGroupWalletDummyTxPtr Wallet::groupDummyTxPtr() const
{
    if (auto dummy = dummyTxPtr()) {
        return dummy->get<QGroupWalletDummyTxPtr>();
    }
    return {};
}

QGroupWalletHealthCheckPtr Wallet::healthPtr() const
{
    return QGroupWalletHealthCheck::information<QGroupWalletHealthCheckPtr>(walletId());
}

void Wallet::updateSignMessage(const QString &xfp, int wallet_type)
{
    QtConcurrent::run([=, this]() {
        ENUNCHUCK::AddressType type = (ENUNCHUCK::AddressType)wallet_type;
        QWarningMessage msg;
        QSingleSignerPtr single = AppModel::instance()->remoteSignerListPtr()->getSingleSignerByFingerPrint(xfp);
        if (single) {
            QString address = bridge::GetSignerAddress(single->originSingleSigner(), (nunchuk::AddressType)type);
            QString signature = bridge::SignMessage(single->originSingleSigner(), single->message());
            single->setAddress(address);
            single->setSignature(signature);
            DBG_INFO << address << signature;
            if (!signature.isEmpty()) {
                AppModel::instance()->showToast(0, "The message has been signed", EWARNING::WarningType::SUCCESS_MSG );
            }
        }
        if (single.isNull()) {
            QMasterSignerPtr master = AppModel::instance()->masterSignerListPtr()->getMasterSignerByXfp(xfp);
            single = bridge::nunchukGetDefaultSignerFromMasterSigner(master->id(),
                                                                     ENUNCHUCK::WalletType::MULTI_SIG,
                                                                     type ,
                                                                     msg);
            if (single) {
                QString address = bridge::GetSignerAddress(single->originSingleSigner(), (nunchuk::AddressType)type);
                if (master) {
                    QString signature = bridge::SignMessage(single->originSingleSigner(), master->message());
                    master->setAddress(address);
                    master->setSignature(signature);
                    DBG_INFO << address << signature;
                    if (!signature.isEmpty()) {
                        AppModel::instance()->showToast(0, "The message has been signed", EWARNING::WarningType::SUCCESS_MSG );
                    }
                }
            }
        }
        emit signMessageChanged();
    });
}

void Wallet::exportBitcoinSignedMessage(const QString &xfp, const QString &file_path, int wallet_type)
{
    QString path = qUtils::QGetFilePath(file_path);
    QString address_type = qUtils::qAddressTypeToStr((nunchuk::AddressType)wallet_type);
    QSingleSignerPtr single = AppModel::instance()->remoteSignerListPtr()->getSingleSignerByFingerPrint(xfp);
    QString signMessage;
    if (single) {
        signMessage = qUtils::ExportBitcoinSignedMessage(single->message(), address_type, single->signature());
    }
    if (single.isNull()) {
        QMasterSignerPtr master = AppModel::instance()->masterSignerListPtr()->getMasterSignerByXfp(xfp);
        if (master) {
            signMessage = qUtils::ExportBitcoinSignedMessage(master->message(), address_type, master->signature());
        }
    }
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream st(&file);
        st.setCodec("UTF-8");
        st << signMessage << Qt::endl;
        st.flush();
        file.close();
    }
}

bool Wallet::isReplaced() const
{
    bool isGlobalReplaced = QSharedWallets::instance()->deprecatedWallets().contains(walletId());
    return status() == "REPLACED" || isGlobalReplaced;
}

bool Wallet::isLocked() const
{
    if (auto dash = dashboard()) {
        QString status = dash->walletJson()["status"].toString();
        return qUtils::strCompare(status, "LOCKED");
    }
    return false;
}

QWalletServicesTagPtr Wallet::servicesTagPtr() const
{
    return isUserWallet() ? QUserWallets::instance()->servicesTagPtr() : QGroupWallets::instance()->servicesTagPtr();
}

QWalletPtr Wallet::clone() const
{
    QWalletPtr to = QWalletPtr(new Wallet());
    to.data()->setIsSharedWallet(isSharedWallet());
    to.data()->setRoomId(roomId());
    // to.da
    to.data()->convert(nunchukWallet());
    return to;
}

std::vector<nunchuk::SingleSigner> Wallet::localSigners()
{
    std::vector<nunchuk::SingleSigner> ret;
    if(m_signers){
        ret = m_signers.data()->localSigners();
    }
    return ret;
}

void Wallet::GetGroupWalletConfig()
{
    QWarningMessage msg;
    auto config = bridge::GetGroupWalletConfig(walletId().toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        m_nunchukConfig = config;
        emit walletConfigChanged();
    }
}

int Wallet::numberOnline()
{
    QtConcurrent::run([this]() {
        if(groupSandbox()){
            int number = bridge::GetGroupOnline(groupSandbox()->groupId());
            QThreadForwarder::instance()->forwardInQueuedConnection([number, this](){
                DBG_INFO << number;
                this->setNumberOnline(number);
            });
        }
    });
    return m_numberOnline;
}

void Wallet::setNumberOnline(int number)
{
    if((number >= 0) && (m_numberOnline != number)){
        m_numberOnline = number;
        emit numberOnlineChanged();
    }
}

int Wallet::unreadMessage()
{
    return m_unreadMessage;
}

void Wallet::setUnreadMessage(int number)
{
    if((number >= 0) && (m_unreadMessage != number)){
        m_unreadMessage = number;
        emit unreadMessageChanged();
    }
}

int Wallet::retentionDays()
{
    return m_nunchukConfig.get_chat_retention_days();
}

void Wallet::setRetentionDays(const int days)
{
    m_nunchukConfig.set_chat_retention_days(days);
    QWarningMessage msg;
    bridge::SetGroupWalletConfig(walletId().toStdString(), m_nunchukConfig, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        bridge::GetGroupWalletConfig(walletId().toStdString(), msg);
        emit walletConfigChanged();
    }
    else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

QGroupMessageModel *Wallet::conversations()
{
    if(!m_conversations){
        m_conversations = QGroupMessageModelPtr(new QGroupMessageModel());
    }
    return m_conversations.data();
}

QWalletDummyTxPtr Wallet::dummyTxPtr() const
{
    return QWalletDummyTx::information<QWalletDummyTxPtr>(walletId());
}

void Wallet::syncCollabKeyname()
{
    if(isSharedWallet()){
        QString room_id = roomId();
        if(room_id != ""){
            DBG_INFO << room_id;
            QList<DracoUser> users = Draco::instance()->getRoomMembers(room_id);
            if(users.count() > 0 && m_signers){
                emit signalSyncCollabKeyname(users);
            }
        }
    }
}

QVariant Wallet::dashboardInfo() const
{
    return QVariant::fromValue(dashboard().data());
}

void Wallet::setValueKeyset(int index)
{
    DBG_INFO << index;
    if (!singleSignersAssigned()) return;
    auto signers = singleSignersAssigned();
    auto list = singleSignersAssigned()->fullList();
    int count = 0;
    for (int i = 0; i < list.size(); i++) {
        auto signer = list.at(i);
        if (signer->valueKey()) {
            count++;
        }
        if (i == index) {
            if (count < walletM()) {
                signer->setValueKey(!signer->valueKey());
            } else {
                signer->setValueKey(false);
            }
            signers->setUserChecked(signer->valueKey(), i);
        }
    }
    singleSignersAssigned()->requestSort();
    m_limitKeySet = singleSignersAssigned()->signerSelectedCount();
    DBG_INFO << m_limitKeySet;
    emit limitKeySetChanged();
}

void Wallet::slotSyncCollabKeyname(QList<DracoUser> users){
    DBG_INFO << roomId();
    if(m_signers){
        m_signers.data()->syncNunchukEmail(users);
    }
}

bool Wallet::isValidAddress(const QString &address)
{
    return qUtils::IsValidAddress(address);
}

int Wallet::reuseKeyGetCurrentIndex(const QString &xfp) {
    return ReplaceKeyFreeUser::reuseKeyGetCurrentIndex(xfp);
}

QString Wallet::bip32path(const QString &xfp, int index) {
    return ReplaceKeyFreeUser::bip32path(xfp, index);
}

bool Wallet::updateKeyReplace(const QString &xfp, const int index)
{
    return ReplaceKeyFreeUser::updateKeyReplace(xfp, index);
}

bool Wallet::removeKeyReplaced(const int index)
{
    return ReplaceKeyFreeUser::removeKeyReplaced(index);
}

void Wallet::requestForAllCoins(const QVariant &act)
{
    CoinsControl::RequestForAllCoins(act);
}

void Wallet::requestForLockedAllCoins(const QVariant &act)
{
    CoinsControl::RequestForLockedAllCoins(act);
}

void Wallet::requestImportCoinControlData(const QString &filePath)
{
    CoinsControl::ImportCoinControlData(filePath);
}

void Wallet::requestExportCoinControlData(const QString &filePath)
{
    CoinsControl::ExportCoinControlData(filePath);
}

void Wallet::requestExportBIP329(const QString &filePath)
{
    CoinsControl::ExportBIP329(filePath);
}

void Wallet::requestImportBIP329(const QString &filePath)
{
    CoinsControl::ImportBIP329(filePath);
}

void Wallet::requestConsolidateMakeTransaction(const QVariant &msg)
{
    CoinsControl::CreateDraftTransaction(E::EVT_CONSOLIDATE_COINS_MERGE_MAKE_TRANSACTION_REQUEST, msg);
}

void Wallet::requestSyncSelectCoinForMakeTransaction(const QVariant &msg)
{
    CoinsControl::RequestSyncSelectCoinForMakeTransaction(msg);
}

QString Wallet::addressPath(const QString &address)
{
    return bridge::nunchukGetAddressPath(walletId(), address);
}

QString Wallet::addressBalance(const QString &address)
{
    qint64 amount = bridge::nunchukGetAddressBalance(walletId(), address);
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(amount);
    }
    else{
        return qUtils::QValueFromAmount(amount);
    }
}

bool Wallet::markAddressUsed(const QString &address)
{
    bool ret = bridge::nunchukMarkAddressUsed(walletId(), address);
    if(ret){
        QtConcurrent::run([=, this]() {
            QStringList used_addr = bridge::nunchukGetUsedAddresses(walletId(), false);
            QStringList unused_addr = bridge::nunchukGetUnusedAddresses(walletId(), false);
            this->setUsedAddressList(used_addr);
            this->setunUsedAddressList(unused_addr);
        });
    }
    return ret;
}

void Wallet::startGetUnreadMessage()
{
    QPointer<Wallet> safeThis(this);
    runInConcurrent([safeThis]() -> int {
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        return bridge::GetUnreadMessagesCount(ptrLamda->walletId());
    },[safeThis](int number) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        ptrLamda->setUnreadMessage(number);
        if (auto list = AppModel::instance()->groupWalletList()) {
            list->unReadMessageCountChanged();
        }
    });

}

void Wallet::startDownloadConversation()
{
    if(conversations() && isGlobalGroupWallet()){
        QString wallet_id = walletId();
        conversations()->startDownloadConversation(wallet_id);
        startGetUnreadMessage();
    }
}

void Wallet::startSendGroupMessage(const QString &message)
{
    QString wallet_id = walletId();
    QWarningMessage msg;
    bridge::SendGroupMessage(wallet_id.toStdString(), message.toStdString(), {}, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        DBG_INFO << "SENT>" << wallet_id << message ;
    }
    else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

void Wallet::requestExportWalletViaBSMS(const QString &file)
{
    DBG_INFO << walletId();
    QString file_path = qUtils::QGetFilePath(file);
    bridge::nunchukExportWallet(walletId(), file_path, nunchuk::ExportFormat::BSMS);
}

void Wallet::requestExportWalletViaQRBCUR2Legacy()
{
    QWarningMessage msgwarning;
    QStringList qrtags = bridge::nunchukExportKeystoneWallet(walletId(), msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
        AppModel::instance()->setQrExported(qrtags);
    }
    else{
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
    }
}

void Wallet::requestExportWalletViaQRBCUR2()
{
    QWarningMessage msgwarning;
    QStringList qrtags = bridge::nunchukExportBCR2020010Wallet(walletId(), msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
        AppModel::instance()->setQrExported(qrtags);
    }
    else{
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
    }
}

void Wallet::requestAcceptReplaceGroup(const QString &sandbox_id)
{
    if (m_sandbox) {
        if (m_sandbox->AcceptReplaceGroup(walletId(), sandbox_id)) {
            GetReplaceGroups();
            m_sandbox->setScreenFlow("setup-group-wallet");
            AppModel::instance()->setNewWalletInfo(this);
            QSharedWallets::instance()->GetAllGroups();
            timeoutHandler(500,[=]{
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST);
            });
        }
    }
}

void Wallet::requestDeclineReplaceGroup(const QString &sandbox_id)
{
    if (m_sandbox) {
        if (m_sandbox->DeclineReplaceGroup(walletId(), sandbox_id)) {
            GetReplaceGroups();
        }
    }
}

QString Wallet::groupId() const
{
    QString group_id = WalletsMng->groupId(walletId());
    if(group_id == ""){
        QWalletCached<QString, QString, QString, QString, bool> data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            group_id = data.groupId;
        }
    }
    return group_id;
}

QStringList Wallet::slugs() const
{
    return {slug()};
}

QString Wallet::slug() const
{
    QString slug = WalletsMng->slugInfo(walletId());
    if(slug == ""){
        QWalletCached<QString, QString, QString, QString, bool> data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            slug = data.slug;
        }
    }
    return slug;
}

QString Wallet::myRole() const
{
    QString role = "";
    if(dashboard()){
        role = dashboard().data()->myRole();
    }
    if (role == "") {
        QWalletCached<QString, QString, QString, QString, bool> data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            role = data.myRole;
        }
    }
    return role;
}

QString Wallet::status() const
{
    QString status = "";
    if(dashboard()){
        status = dashboard().data()->walletStatus();
    }
    if(status == ""){
        QWalletCached<QString, QString, QString, QString, bool> data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            status = data.status;
        }
    }
    return status;
}

QGroupDashboardPtr Wallet::dashboard() const
{
    return QGroupDashboard::information<QGroupDashboardPtr>(walletId());
}

bool Wallet::enableCreateChat()
{
    // disable when wallet is free/iron hand/horneybadger
    QString wallet_slug = slug();
    if(wallet_slug == ""){
        return false;
    }
    else {
        bool ret_role = wallet_slug.contains("iron_hand")
                        || wallet_slug.contains("iron_hand_testnet")
                        || wallet_slug.contains("honey_badger")
                        || wallet_slug.contains("honey_badger_testnet");
        bool ret_member = dashboard() ? (dashboard()->members().count() > 1) : false;
        bool enable = !ret_role && ret_member;
        return enable;
    }
}

bool Wallet::isByzantineWallet()
{
    QString wallet_slug = slug();
    bool ret =  wallet_slug.contains("byzantine")
               || wallet_slug.contains("finney");
    if(ret) {
        return true;
    }
    return false;
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
    if(m_data[index.row()]){
        switch (role) {
        case wallet_Id_Role:
            return m_data[index.row()]->walletId();
        case wallet_Name_Role:
            return m_data[index.row()]->walletNameDisplay();
        case wallet_M_Role:
            return m_data[index.row()]->walletM();
        case wallet_N_Role:
            return m_data[index.row()]->walletN();
        case wallet_AddressType_Role:
            return m_data[index.row()]->walletAddressType();
        case wallet_createDate_Role:
            return m_data[index.row()]->walletCreateDate();
        case wallet_Balance_Role:
            return QVariant::fromValue(m_data[index.row()]->balanceDisplay());
        case wallet_BalanceBTC_Role:
            return QVariant::fromValue(m_data[index.row()]->balanceBTC());
        case wallet_BalanceCurrency_Role:
            return QVariant::fromValue(m_data[index.row()]->balanceCurrency());
        case wallet_Escrow_Role:
            return m_data[index.row()]->walletEscrow();
        case wallet_SingleSignerList_Role:
            return QVariant::fromValue((SingleSignerListModel*)m_data[index.row()]->singleSignersAssigned());
        case wallet_Address_Role:{
            if(m_data[index.row()]->address().isEmpty()){
                return m_data[index.row()]->unUseAddress();
            }
            else{
                return m_data[index.row()]->address();
            }
        }
        case wallet_usedAddressList_Role:
            return m_data[index.row()]->usedAddressList();
        case wallet_unUsedAddressList_Role:
            return m_data[index.row()]->unUsedAddressList();
        case wallet_isSharedWallet_Role:
            return m_data[index.row()]->isSharedWallet();
        case wallet_isAssistedWallet_Role:
            return m_data[index.row()]->isAssistedWallet();
        case wallet_groupid_Role:
            return m_data[index.row()]->groupId();
        case wallet_dashboard_Role:
            return m_data[index.row()]->dashboardInfo();
        case wallet_myRole_Role:
            return m_data[index.row()]->myRole();
        case wallet_hasOwner_Role:
            return m_data[index.row()]->ownerPrimary().isValid();
        case wallet_primaryOwner_Role:
            return m_data[index.row()]->ownerPrimary();
        case wallet_isHotWallet_Role:
            return m_data[index.row()]->keyNeedBackup();
        case wallet_slug_Role:
            return m_data[index.row()]->slug();
        case wallet_isLocked_Role:
            return m_data[index.row()]->isLocked();
        case wallet_isReplaced_Role:
            return m_data[index.row()]->isReplaced();
        case wallet_isSanboxWallet_Role:
            return m_data[index.row()]->isGlobalGroupWallet();
        case wallet_conversation_Role:
            return QVariant::fromValue((QGroupMessageModel*)m_data[index.row()]->conversations());
        case wallet_unreadMessage_Role:
            return m_data[index.row()]->unreadMessage();
        case wallet_numberOnline_Role:
            return m_data[index.row()]->numberOnline();
        default:
            return QVariant();
        }
    }
    return QVariant();
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
    return roles;
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
    QReadLocker locker(&m_lock); // Multiple thread allowed
    if (!m_data.isEmpty()) {
        foreach (QWalletPtr it, m_data) {
            if(qUtils::strCompare(walletId, it.data()->walletId())){
                return it;
            }
        }
    }
    return QWalletPtr(NULL);
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

void WalletListModel::requestSort(int role, int order)
{
    beginResetModel();
    if(m_data.count() > 1){
        switch (role) {
        case wallet_createDate_Role:
        {
            if(Qt::DescendingOrder == order){
                std::sort(m_data.begin(), m_data.end(), sortWalletByNameDescending);
            }
            else{
                std::sort(m_data.begin(), m_data.end(), sortWalletByNameAscending);
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
        QString slug = it.data()->slug();
        bool ret =    qUtils::strCompare(slug, "byzantine_premier_testnet")
                   || qUtils::strCompare(slug, "byzantine_premier");
        if(ret) {
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
    auto checkIsAssisted = [this](const QString &masterFingerPrint, const QString& derivation_path) -> bool{
        foreach (QWalletPtr i , m_data ){
            if(NULL != i.data()->singleSignersAssigned()){
                bool exist = i.data()->singleSignersAssigned()->containsSigner(masterFingerPrint, derivation_path);
                if (exist && i->isAssistedWallet()) return true;
            }
        }
        return false;
    };
    bool used_in_assisted_wallet {false};
    bool used_in_free_wallet {false};
    QMasterSignerPtr master = bridge::nunchukGetMasterSignerFingerprint(masterFingerPrint);
    if (!master.isNull()) {
        std::vector<nunchuk::SingleSigner> signers = bridge::GetSignersFromMasterSigner(masterFingerPrint);
        for(auto signer: signers) {
            if (checkIsAssisted(masterFingerPrint, QString::fromStdString(signer.get_derivation_path()))) {
                used_in_assisted_wallet = true;
            }
        }
        used_in_free_wallet = !used_in_assisted_wallet;
    } else {
        QSingleSignerPtr single = bridge::nunchukGetRemoteSigner(masterFingerPrint);
        if (!single.isNull()) {
            used_in_assisted_wallet = checkIsAssisted(single->fingerPrint(), single->derivationPath());
            used_in_free_wallet = !used_in_assisted_wallet;
        }
    }

    QJsonObject ret {
       {"used_in_assisted_wallet", used_in_assisted_wallet},
       {"used_in_free_wallet", used_in_free_wallet}
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

bool sortWalletByNameAscending(const QWalletPtr &v1, const QWalletPtr &v2)
{
    return v1.data()->walletCreateDate() < v2.data()->walletCreateDate();
}

bool sortWalletByNameDescending(const QWalletPtr &v1, const QWalletPtr &v2)
{
    return v1.data()->walletCreateDate() > v2.data()->walletCreateDate();
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
