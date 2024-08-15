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

int Wallet::m_flow = 0;
Wallet::Wallet() :
    m_addressType(QString::number((int)ENUNCHUCK::AddressType::NATIVE_SEGWIT)), // Default is NATIVE_SEGWIT
    m_createDate(QDateTime::currentDateTime()),
    m_signers(QSingleSignerListModelPtr(new SingleSignerListModel())),
    m_transactionHistory(QTransactionListModelPtr(new TransactionListModel())),
    m_creationMode((int)CreationMode::CREATE_NEW_WALLET)
{
    m_roomMembers.clear();
    init();
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Wallet::Wallet(const nunchuk::Wallet &w)
    : Wallet()
{
    m_wallet = w;
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Wallet::~Wallet(){

}

void Wallet::init()
{
    QObject::connect(this, &Wallet::signalSyncCollabKeyname, this, &Wallet::slotSyncCollabKeyname, Qt::QueuedConnection);
}

void Wallet::convert(const Wallet *w)
{
    if (w) {
        m_wallet = w->wallet();
    }
}

void Wallet::convert(const nunchuk::Wallet &w)
{
    m_wallet = w;
    setId(QString::fromStdString(w.get_id()));
    setM(w.get_m());
    setN(w.get_n());
    setName(QString::fromStdString(w.get_name()));
    setAddressType(QString::number((int)w.get_address_type()));
    setBalance(w.get_unconfirmed_balance());
    setCreateDate(QDateTime::fromTime_t(w.get_create_date()));
    setEscrow(w.is_escrow());
    setGapLimit(w.get_gap_limit());
    setDescription(QString::fromStdString(w.get_description()));
    serverKeyPtr();
    inheritancePlanPtr();
    QGroupDashboardPtr dash = dashboard();
    if (dash && dash->myInfo().isEmpty()) {
        dash->GetMemberInfo();
        QtConcurrent::run([dash]() {
            dash->GetAlertsInfo();
            dash->GetWalletInfo();
            dash->GetHealthCheckInfo();
        });
    }

    m_signers->cleardata();
    for (nunchuk::SingleSigner signer : w.get_signers()) {
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
        m_signers->addSingleSigner(ret);
    }
    if(m_signers){
        m_signers.data()->requestSort();
    }
}

QString Wallet::id() const {return m_id;}

int Wallet::m() const { return m_m;}

int Wallet::n() {
    n_n = m_signers.data()->rowCount();
    return n_n;
}

QString Wallet::name() {
    if(isByzantineWallet() && "" != aliasName()){
        return aliasName();
    }
    QRegularExpression re("\\p{So}");
    m_name.remove(re);
    return  m_name;
}

QString Wallet::walletOriginName() const
{
    return m_name;
}

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

SingleSignerListModel* Wallet::singleSignersAssigned() {
    if(m_signers){
        m_signers->requestSort();
        return m_signers.data();
    }
    return NULL;
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

QString Wallet::unUseAddress() const
{
    QString address;
    QStringList addrs = bridge::nunchukGetUnusedAddresses(id(), false);
    if (addrs.size() > 0) {
        address = addrs.first();
    } else {
        address = bridge::nunchukGenNewAddresses(id(),false);
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

bool Wallet::needBackup() const
{
    return m_wallet.need_backup();
}

void Wallet::setNeedBackup(const bool bVal)
{
    if (bVal != needBackup()) {
        m_wallet.set_need_backup(bVal);
        emit needBackupChanged();
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
    return isUserWallet() || isGroupWallet();
}

bool Wallet::containsColdcard()
{
    if(m_signers){
        return m_signers.data()->containsColdcard();
    }
    return false;
}

int Wallet::gapLimit() const
{
    return m_gapLimit;
}

void Wallet::setGapLimit(int gap_limit)
{
    if(m_gapLimit != gap_limit){
        m_gapLimit = gap_limit;
        emit gapLimitChanged();
    }
}

nunchuk::Wallet Wallet::wallet() const
{
    return m_wallet;
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
    return bridge::convertTransaction(tx, id());
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
    DBG_INFO << "tx_id:" << txid << " psbt:" << psbt << "wallet status:" << status();
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
    DBG_INFO << "tx_id:" << tx_id << " psbt:" << psbt << "wallet status:" << status();
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
    DBG_INFO << "tx_id:" << tx_id << " psbt:" << psbt << "wallet status:" << status();
    if(isReplaced()){
        return false;
    }
    QJsonObject data;
    QString errormsg = "";
    QString wallet_id = id();
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
    setName(name);
    setDescription(description);
    DBG_INFO << isGroupWallet() << isUserWallet();
    if(isGroupWallet()){
        UpdateGroupWallet(name, description);
    }
    else if(isUserWallet()){
        UpdateUserWallet(name, description);
    }
    else{
        QWarningMessage msg;
        m_wallet.set_name(name.toStdString());
        bridge::UpdateWallet(m_wallet, msg);
    }
}

QJsonObject Wallet::GetServerKeyInfo(const QString &txid)
{
    DBG_INFO << "txid:" << txid << "wallet status:" << status();
    if(isReplaced()){
        return QJsonObject();
    }
    QString wallet_id = id();
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
    if(isReplaced()){
        return false;
    }
    auto tag = servicesTagPtr();
    QString passwordToken = tag->passwordToken();
    QJsonObject output;
    QString errormsg = "";
    bool ret {false};
    if(isGroupWallet()){
        ret = Byzantine::instance()->DeleteGroupWallet(id(), groupId(), {}, passwordToken, tag->secQuesToken(), output, errormsg);
    }
    else if(isUserWallet()){
        ret = Draco::instance()->DeleteAssistedWallet(id(), {}, passwordToken, tag->secQuesToken(), output, errormsg);
    }
    if (ret) {
        setIsDeleting(false);
    }
    return ret;
}

bool Wallet::DeleteWalletRequiredSignatures()
{
    DBG_INFO << "wallet status:" << status();
    if(isReplaced()){
        return false;
    }
    auto tag = servicesTagPtr();
    ReqiredSignaturesInfo info;
    QString errormsg = "";
    QJsonObject output;
    bool ret {false};
    if(isGroupWallet()){
        ret = Byzantine::instance()->DeleteGroupWalletRequiredSignatures(id(), groupId(), output, errormsg);
    }
    else if(isUserWallet()){
        ret = Draco::instance()->DeleteWalletRequiredSignatures(id(), output, errormsg);
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

QString Wallet::aliasName() const
{
    if (auto dash = dashboard()) {
        QString alias = dash->walletJson()["alias"].toString();
        QRegularExpression re("\\p{So}");
        alias.remove(re);
        return alias;
    }
    return "";
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
    bool ret = Byzantine::instance()->GetWalletAlias(groupId(), id(), output, errormsg);
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
    bool ret = Byzantine::instance()->UpdateWalletAlias(groupId(), id(), nameWallet, output, errormsg);
    if(ret){
        QGroupDashboardPtr dash = dashboard();
        if (dash) {
            QtConcurrent::run([this, dash]() {
                dash->GetWalletInfo();
                if(AppModel::instance()->walletList()){
                    AppModel::instance()->walletList()->dataUpdated(id());
                }
                emit aliasNameChanged();
                emit nameChanged();
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
    bool ret = Byzantine::instance()->DeleteWalletAlias(groupId(), id(), output, errormsg);
    if(ret){
        QGroupDashboardPtr dash = dashboard();
        if (dash) {
            QtConcurrent::run([this, dash]() {
                dash->GetWalletInfo();
                if(AppModel::instance()->walletList()){
                    AppModel::instance()->walletList()->dataUpdated(id());
                }
                emit aliasNameChanged();
                emit nameChanged();
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
    bool ret = Byzantine::instance()->UpdateWalletPrimaryOwner(groupId(), id(), membership_id, output, errormsg);
    if(ret){
        QGroupDashboardPtr dash = dashboard();
        if (dash) {
            QtConcurrent::run([this, dash]() {
                dash->GetWalletInfo();
                emit groupInfoChanged();
            });
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

void Wallet::GetUserTxs()
{
    if(isUserWallet()){
        QString wallet_id = id();
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
        QString wallet_id = id();
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
        QString wallet_id = id();
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
        QString wallet_id = id();
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
    QString wallet_id = id();
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
        Draco::instance()->assistedWalletUpdateTx(id(),txid, memo);
    }
}

void Wallet::CancelUserTxs(const QString &txid)
{
    if(isUserWallet()){
        bool ret = Draco::instance()->assistedWalletCancelTx(id(), txid);
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
        bool ret = Draco::instance()->assistedWalletCreateTx(id(), psbt, memo);
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
        QString wallet_id = id();
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
        QString wallet_id = id();
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
        QString wallet_id = id();
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
        QString wallet_id = id();
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
        QString wallet_id = id();
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
        QString wallet_id = id();
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
    QString wallet_id = id();
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
        bool ret = Byzantine::instance()->UpdateTransaction(groupId(), id(), txid, memo, output, errormsg);
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
        bool ret = Byzantine::instance()->CancelTransaction(groupId(), id(), txid, output, errormsg);
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
        bool ret = Byzantine::instance()->CreateTransaction(groupId(), id(), psbt, memo, output, errormsg);
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
        QString wallet_id = id();
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
        QString wallet_id = id();
        QString group_id  = groupId();
        bool ret = Byzantine::instance()->UpdateWallet(group_id, wallet_id, name, description, data, error_msg);
        if(!ret){
            //TBD
        }
    }
}

QVariant Wallet::serverKeyInfo() const
{
    return QVariant::fromValue(serverKeyPtr().data());
}

QServerKeyPtr Wallet::serverKeyPtr() const
{
    return QServerKey::information<QServerKeyPtr>(id());
}

QVariant Wallet::inheritancePlanInfo() const
{
    return QVariant::fromValue(inheritancePlanPtr().data());
}

QInheritancePlanPtr Wallet::inheritancePlanPtr() const
{
    return QInheritancePlan::information<QInheritancePlanPtr>(id());
}

QVariant Wallet::recurringPayment() const
{
    return QVariant::fromValue(recurringPaymentPtr().data());
}

QRecurringPaymentPtr Wallet::recurringPaymentPtr() const
{
    return QRecurringPayment::information<QRecurringPaymentPtr>(id());
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
    return QGroupWalletHealthCheck::information<QGroupWalletHealthCheckPtr>(id());
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
        st << signMessage << endl;
        st.flush();
        file.close();
    }
}

bool Wallet::isReplaced() const
{
    return status() == "REPLACED";
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
    return bridge::convertWallet(wallet());
}

QWalletDummyTxPtr Wallet::dummyTxPtr() const
{
    return QWalletDummyTx::information<QWalletDummyTxPtr>(id());
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

QString Wallet::groupId() const
{
    QString group_id = WalletsMng->groupId(m_id);
    if(group_id == ""){
        QWalletCached<QString, QString, QString, QString> data;
        bool ret = AppSetting::instance()->getwalletCached(id(), data);
        if(ret){
            group_id = data.first;
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
    QString slug = WalletsMng->slugInfo(m_id);
    if(slug == ""){
        QWalletCached<QString, QString, QString, QString> data;
        bool ret = AppSetting::instance()->getwalletCached(id(), data);
        if(ret){
            slug = data.second;
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
        QWalletCached<QString, QString, QString, QString> data;
        bool ret = AppSetting::instance()->getwalletCached(id(), data);
        if(ret){
            role = data.third;
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
        QWalletCached<QString, QString, QString, QString> data;
        bool ret = AppSetting::instance()->getwalletCached(id(), data);
        if(ret){
            status = data.fourth;
        }
    }
    return status;
}

QGroupDashboardPtr Wallet::dashboard() const
{
    return QGroupDashboard::information<QGroupDashboardPtr>(id());
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
        case wallet_Address_Role:{
            if(d_[index.row()]->address().isEmpty()){
                return d_[index.row()]->unUseAddress();
            }
            else{
                return d_[index.row()]->address();
            }
        }
        case wallet_usedAddressList_Role:
            return d_[index.row()]->usedAddressList();
        case wallet_unUsedAddressList_Role:
            return d_[index.row()]->unUsedAddressList();
        case wallet_isSharedWallet_Role:
            return d_[index.row()]->isSharedWallet();
        case wallet_isAssistedWallet_Role:
            return d_[index.row()]->isAssistedWallet();
        case wallet_groupid_Role:
            return d_[index.row()]->groupId();
        case wallet_dashboard_Role:
            return d_[index.row()]->dashboardInfo();
        case wallet_myRole_Role:
            return d_[index.row()]->myRole();
        case wallet_hasOwner_Role:
            return d_[index.row()]->ownerPrimary().isValid();
        case wallet_primaryOwner_Role:
            return d_[index.row()]->ownerPrimary();
        case wallet_isHotWallet_Role:
            return d_[index.row()]->needBackup();
        case wallet_slug_Role:
            return d_[index.row()]->slug();
        case wallet_isLocked_Role:
            return d_[index.row()]->isLocked();
        case wallet_isReplaced_Role:
            return d_[index.row()]->isReplaced();
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
    roles[wallet_groupid_Role]              = "wallet_group_id";
    roles[wallet_dashboard_Role]            = "wallet_dashboard";
    roles[wallet_myRole_Role]               = "wallet_role";
    roles[wallet_hasOwner_Role]             = "wallet_hasOwner";
    roles[wallet_primaryOwner_Role]         = "wallet_primaryOwner";
    roles[wallet_isHotWallet_Role]          = "wallet_isHotWallet";
    roles[wallet_slug_Role]                 = "wallet_slug";
    roles[wallet_isLocked_Role]             = "wallet_isLocked";
    roles[wallet_isReplaced_Role]           = "wallet_isReplaced";
    return roles;
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

void WalletListModel::updateName(const QString &walletId, const QString &value)
{
    for (int i = 0; i < d_.count(); i++) {
        if(d_.at(i).data() && 0 == QString::compare(walletId, d_.at(i)->id(), Qt::CaseInsensitive)){
            d_.at(i)->setName(value);
            emit dataChanged(index(i),index(i));
        }
    }
}

void WalletListModel::dataUpdated(const QString &walletId)
{
    for (int i = 0; i < d_.count(); i++) {
        if(d_.at(i).data() && 0 == QString::compare(walletId, d_.at(i)->id(), Qt::CaseInsensitive)){
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

QVariantList WalletListModel::walletListByMasterSigner(const QString& masterSignerId){
    QVariantList ret;
    foreach (QWalletPtr i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            bool exist = i.data()->singleSignersAssigned()->containsMasterSignerId(masterSignerId);
            if(true == exist){
                QJsonObject json;
                json["name"] = i.data()->name();
                json["walletM"] = i.data()->m();
                json["walletN"] = i.data()->n();
                ret << QVariant::fromValue(json);
            }
        }
    }
    return ret;
}

QVariantList WalletListModel::walletListByFingerPrint(const QString &masterFingerPrint)
{
    QVariantList ret;
    foreach (QWalletPtr i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            bool exist = i.data()->singleSignersAssigned()->containsFingerPrint(masterFingerPrint);
            if(true == exist){
                QJsonObject json;
                json["name"] = i.data()->name();
                json["walletM"] = i.data()->m();
                json["walletN"] = i.data()->n();
                ret << QVariant::fromValue(json);
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

void WalletListModel::updateSignerHealthStatus(const QString &xfp, const int status, const time_t time)
{
    beginResetModel();
    foreach (QWalletPtr i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            i.data()->singleSignersAssigned()->updateSignerHealthStatus(xfp, status, time);
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

void WalletListModel::refresh()
{
    beginResetModel();
    endResetModel();
}

void WalletListModel::requestSort(int role, int order)
{
    beginResetModel();
    if(d_.count() > 1){
        switch (role) {
        case wallet_createDate_Role:
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

bool WalletListModel::existProWallet()
{
    foreach (QWalletPtr it , d_ ){
        if (it->isPro()) return true;
    }
    return false;
}

bool WalletListModel::existGroupWallet()
{
    foreach (QWalletPtr it , d_ ){
        if (it->hasGroup()) return true;
    }
    return false;
}

bool WalletListModel::isContainsPremier()
{
    foreach (QWalletPtr it , d_ ){
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

QVariant WalletListModel::removeOrNot(const QString &masterFingerPrint, const QString& derivation_path)
{
    bool used_in_assisted_wallet {false};
    bool used_in_free_wallet {false};
    foreach (QWalletPtr i , d_ ){
        if(NULL != i.data()->singleSignersAssigned()){
            bool exist = i.data()->singleSignersAssigned()->containsSigner(masterFingerPrint, derivation_path);
            if (exist && i->isAssistedWallet()) used_in_assisted_wallet = true;
            else if (exist && !i->isAssistedWallet()) used_in_free_wallet = true;
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
    foreach (QWalletPtr it , d_ ){
        if (it->isAssistedWallet()) return true;
    }
    return false;
}

bool sortWalletByNameAscending(const QWalletPtr &v1, const QWalletPtr &v2)
{
    return v1.data()->createDateDateTime() < v2.data()->createDateDateTime();
}

bool sortWalletByNameDescending(const QWalletPtr &v1, const QWalletPtr &v2)
{
    return v1.data()->createDateDateTime() > v2.data()->createDateDateTime();
}
