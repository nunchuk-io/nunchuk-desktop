#include "AssistedWallet.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QUserWalletDummyTx.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QRecurringPayment.h"
#include "QThreadForwarder.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QWalletServicesTag.h"
#include "features/transactions/usecases/SyncTransactionFlowUseCase.h"
#include "features/transactions/usecases/CancelTransactionUseCase.h"
#include "features/transactions/usecases/CreateTransactionUseCase.h"
#include "features/transactions/usecases/UpdateTransactionUseCase.h"
#include "features/transactions/usecases/FetchTransactionListUseCase.h"
#include "features/transactions/usecases/SignTransactionUseCase.h"
#include "features/transactions/usecases/RbfTransactionUseCase.h"
#include "features/transactions/usecases/FetchTransactionUseCase.h"
#include "features/transactions/usecases/FetchCancelTransactionUseCase.h"
#include "features/wallets/usecases/DeleteWalletUseCase.h"
#include "features/wallets/usecases/UpdateWalletUseCase.h"

AssistedWallet::AssistedWallet(const nunchuk::Wallet &w) :
    SharedWallet{w}
{}

void AssistedWallet::convert(const nunchuk::Wallet w)
{
    SharedWallet::convert(w);
    serverKeyPtr();
    inheritancePlanPtr();
    QGroupDashboardPtr dash = dashboard();
    if (dash && dash->myInfo().isEmpty()) {
        dash->GetMemberInfo();
        dash->GetAlertsInfo();
        dash->GetHealthCheckInfo();
        dash->GetWalletInfo();
    }
}

bool AssistedWallet::isReplaced() const
{
    return status() == "REPLACED" || SharedWallet::isReplaced();
}

bool AssistedWallet::isAssistedWallet() const
{
    return (isUserWallet() || isGroupWallet()) && !isReplaced();
}

QString AssistedWallet::groupId() const
{
    QString group_id = WalletsMng->groupId(walletId());
    if(group_id == ""){
        QWalletCached data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            group_id = data.groupId;
        }
    }
    return group_id;
}

QVariant AssistedWallet::dashboardInfo() const
{
    return QVariant::fromValue(dashboard().data());
}

QVariant AssistedWallet::serverKeyInfo() const
{
    return QVariant::fromValue(serverKeyPtr().data());
}

void AssistedWallet::GetAsisstedTx(const QString &txid)
{
    DBG_INFO << "txid:" << txid << "wallet status:" << status();
    if(isReplaced()){
        return;
    }

    features::transactions::usecases::FetchTransactionInput input;
    input.wallet_id = walletId();
    input.txid = txid;
    input.group_id = groupId();
    input.isClaimed = isClaimed();
    features::transactions::usecases::FetchTransactionUseCase  fetchTransactionUseCase;
    auto result = fetchTransactionUseCase.execute(input);
    if (result.isSuccess()) {
        auto transaction = result.value().transaction;
        auto tranPtr = transactionHistory()->getTransactionByTxid(txid);
        if(!transaction.isEmpty() && tranPtr){
            tranPtr->setServerKeyMessage(transaction);
        }
    }
    return;
}

QVariant AssistedWallet::inheritancePlanInfo() const
{
    return QVariant::fromValue(inheritancePlanPtr().data());
}

QServerKeyPtr AssistedWallet::serverKeyPtr() const
{
    return QServerKey::information<QServerKeyPtr>(walletId());
}

QInheritancePlanPtr AssistedWallet::inheritancePlanPtr() const
{
    return QInheritancePlan::information<QInheritancePlanPtr>(walletId());
}

QVariant AssistedWallet::recurringPayment() const
{
    return QVariant::fromValue(recurringPaymentPtr().data());
}

QRecurringPaymentPtr AssistedWallet::recurringPaymentPtr() const
{
    return QRecurringPayment::information<QRecurringPaymentPtr>(walletId());
}

QGroupWalletHealthCheckPtr AssistedWallet::healthPtr() const
{
    return QGroupWalletHealthCheck::information<QGroupWalletHealthCheckPtr>(walletId());
}

QGroupWalletDummyTxPtr AssistedWallet::groupDummyTxPtr() const
{
    if (auto dummy = dummyTxPtr()) {
        return dummy->get<QGroupWalletDummyTxPtr>();
    }
    return {};
}

bool AssistedWallet::isLocked() const
{
    if (auto dash = dashboard()) {
        QString status = dash->walletJson()["status"].toString();
        return qUtils::strCompare(status, "LOCKED");
    }
    return false;
}

QWalletServicesTagPtr AssistedWallet::servicesTagPtr() const
{
    return QWalletServicesTag::instance();
}

QString AssistedWallet::walletAliasName() const
{
    if (auto dash = dashboard()) {
        QString alias = dash->walletJson()["alias"].toString();
        QRegularExpression re("\\p{So}");
        alias.remove(re);
        return alias;
    }
    return "";
}

QString AssistedWallet::walletNameDisplay()
{
    QString ret = "";
    if(isByzantineWallet()){
        ret = walletAliasName();
    }
    if(ret == ""){
        ret = SharedWallet::walletNameDisplay();
    }
    return ret;
}

bool AssistedWallet::isByzantineGuardian()
{
    QWalletCached data;
    bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
    if(ret){
        return data.hideFiatCurrency;
    }
    return false;
}

bool AssistedWallet::isClaimed() const
{
    QWalletCached data;
    bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
    if(ret){
        return data.isClaimed;
    }
    return false;
}

QStringList AssistedWallet::slugs() const
{
    return QStringList{slug()};
}

QString AssistedWallet::slug() const
{
    QString slug = WalletsMng->slugInfo(walletId());
    if(slug == ""){
        QWalletCached data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            slug = data.slug;
        }
    }
    return slug;
}

QString AssistedWallet::myRole() const
{
    QString role = "";
    if(dashboard()){
        role = dashboard().data()->myRole();
    }
    if (role == "") {
        QWalletCached data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            role = data.myRole;
        }
    }
    return role;
}


QString AssistedWallet::status() const
{
    QString status = "";
    if(dashboard()){
        status = dashboard().data()->walletStatus();
    }
    if(status == ""){
        QWalletCached data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            status = data.status;
        }
    }
    return status;
}

QGroupDashboardPtr AssistedWallet::dashboard() const
{
    return QGroupDashboard::information<QGroupDashboardPtr>(walletId());
}

bool AssistedWallet::enableCreateChat()
{
    // disable when wallet is free/iron hand/horneybadger
    QString wallet_slug = slug();
    if(wallet_slug == ""){
        return false;
    }
    else {
        bool ret_member = dashboard() ? (dashboard()->members().count() > 1) : false;
        bool enable = !isUserWallet() && ret_member;
        return enable;
    }
}

bool AssistedWallet::isByzantineWallet()
{
    bool ret =  isByzantineStandard() || isFinneyStandard();
    if(ret) {
        return true;
    }
    return false;
}


void AssistedWallet::GetAssistedTxs()
{
    DBG_INFO << "FIXME user:" << isUserWallet() << " group:" << isGroupWallet() << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    features::transactions::usecases::FetchTransactionListInput  input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.isClaimed = isClaimed();
    features::transactions::usecases::FetchTransactionListUseCase  fetchTransactionListUseCase;
    fetchTransactionListUseCase.execute(input);

    GetAssistedCancelledTxs();
    if(isGroupWallet()){
        GetGroupTxNotes();
    }
    else if(isUserWallet()){
        GetUserTxNotes();
    }
    else{}
}

void AssistedWallet::GetAssistedCancelledTxs()
{
    DBG_INFO << "FIXME user:" << isUserWallet() << " group:" << isGroupWallet() << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    features::transactions::usecases::FetchCancelTransactionInput  input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.isClaimed = isClaimed();
    features::transactions::usecases::FetchCancelTransactionUseCase  fetchCancelTransactionUseCase;
    fetchCancelTransactionUseCase.execute(input);
}


QTransactionPtr AssistedWallet::SyncAssistedTxs(const nunchuk::Transaction &tx)
{
    DBG_INFO << "FIXME user:" << walletId() << " group:" << groupId() << "wallet status:" << status();
    if(isReplaced()){
        return NULL;
    }
    features::transactions::usecases::SyncTransactionFlowInput  input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.tx = tx;
    input.isClaimed = isClaimed();
    features::transactions::usecases::SyncTransactionFlowUseCase  syncTransactionFlowUseCase;
    auto result = syncTransactionFlowUseCase.execute(input);
    if (result.isSuccess()) {
        auto transaction = result.value().transaction;
        auto tranPtr = transactionHistory()->getTransactionByTxid(QString::fromStdString(tx.get_txid()));
        if(!transaction.isEmpty() && tranPtr){
            tranPtr->setServerKeyMessage(transaction);
        }
    }
    return bridge::convertTransaction(result.value().tx, walletId());
}

void AssistedWallet::UpdateAssistedTxs(const QString &txid, const QString &memo)
{
    DBG_INFO << "tx_id:" << txid << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    features::transactions::usecases::UpdateTransactionInput input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.txid = txid;
    input.note = memo;
    input.isClaimed = isClaimed();
    features::transactions::usecases::UpdateTransactionUseCase updateTransactionUseCase;
    updateTransactionUseCase.execute(input);
}

void AssistedWallet::CancelAssistedTxs(const QString &txid)
{
    DBG_INFO << "tx_id:" << txid << "wallet status:" << status();
    if(isReplaced()){
        return;
    }

    features::transactions::usecases::CancelTransactionInput  input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.txid = txid;
    input.isClaimed = isClaimed();
    features::transactions::usecases::CancelTransactionUseCase cancelTransactionUseCase;
    cancelTransactionUseCase.execute(input);
}

void AssistedWallet::CreateAsisstedTxs(const QString &txid, const QString &psbt, const QString &memo)
{
    DBG_INFO << "tx_id:" << txid << "wallet status:" << status() << "replace: " << isReplaced();
    if(isReplaced()){
        return;
    }
    features::transactions::usecases::CreateTransactionInput  input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.psbt = psbt;
    input.note = memo;
    input.isClaimed = isClaimed();
    features::transactions::usecases::CreateTransactionUseCase createTransactionUseCase;
    createTransactionUseCase.execute(input);
}

void AssistedWallet::SignAsisstedTxs(const QString &tx_id, const QString &psbt, const QString &memo)
{
    DBG_INFO << "tx_id:" << tx_id << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    features::transactions::usecases::SignTransactionInput  input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.txid =  tx_id;
    input.psbt = psbt;
    input.note = memo;
    input.isClaimed = isClaimed();
    features::transactions::usecases::SignTransactionUseCase signTransactionUseCase;
    auto result = signTransactionUseCase.execute(input);
    if (result.isSuccess()) {
        auto transaction = result.value().transaction;
        auto tranPtr = transactionHistory()->getTransactionByTxid(tx_id);
        if(!transaction.isEmpty() && tranPtr){
            tranPtr->setServerKeyMessage(transaction);
        }
    }
}

bool AssistedWallet::RbfAsisstedTxs(const QString &tx_id, const QString &psbt)
{
    DBG_INFO << "tx_id:" << tx_id << "wallet status:" << status();
    if(isReplaced()){
        return false;
    }
    features::transactions::usecases::RbfTransactionInput  input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.txid =  tx_id;
    input.psbt = psbt;
    input.isClaimed = isClaimed();
    features::transactions::usecases::RbfTransactionUseCase rbfTransactionUseCase;
    rbfTransactionUseCase.execute(input);
    return false;
}

void AssistedWallet::UpdateWallet(const QString &name, const QString &description)
{
    DBG_INFO << name << " description:" << description << "wallet status:" << status();
    if(isReplaced()){
        return;
    }
    setWalletName(name);
    setWalletDescription(description);
    
    features::wallets::usecases::UpdateWalletInput  input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.name = name;
    input.description = description;
    input.isClaimed = isClaimed();
    features::wallets::usecases::UpdateWalletUseCase updateWalletUseCase;
    auto result = updateWalletUseCase.execute(input);
    if (!result.isSuccess()) {
        BaseWallet::UpdateWallet(name, description);
    }
}

bool AssistedWallet::DeleteAssistedWallet()
{
    auto tag = servicesTagPtr();
    features::wallets::usecases::DeleteWalletInput  input;
    input.wallet_id = walletId();
    input.group_id = groupId();
    input.passwordToken = tag->passwordToken();
    input.secQuesToken = tag->secQuesToken();
    input.isClaimed = isClaimed();
    features::wallets::usecases::DeleteWalletUseCase deleteWalletUseCase;
    auto result = deleteWalletUseCase.execute(input);
    if (result.isSuccess()) {
        setIsDeleting(false);
    }
    return result.isSuccess();
}

bool AssistedWallet::DeleteWalletRequiredSignatures()
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

void AssistedWallet::getChatInfo()
{
    if(isReplaced()){
        return;
    }
    if(isGroupWallet() && dashboard()){
        dashboard()->getChatInfo();
    }
}

QVariantList AssistedWallet::aliasMembers() const
{
    return m_aliasMembers.toVariantList();
}

QVariantList AssistedWallet::ownerMembers() const
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

QVariant AssistedWallet::ownerPrimary() const
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

bool AssistedWallet::isPro()
{
    if (auto dash = dashboard()) {
        return dash->inheritanceCount() > 0;
    }
    return false;
}

bool AssistedWallet::hasGroup()
{
    return !groupId().isEmpty();
}

bool AssistedWallet::GetWalletAlias()
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

bool AssistedWallet::updateWalletAlias(const QString &nameWallet)
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

bool AssistedWallet::deleteWalletAlias()
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

bool AssistedWallet::updateWalletPrimaryOwner(const QString &membership_id)
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

QVariant AssistedWallet::dummyTx() const
{
    return QVariant::fromValue(groupDummyTxPtr().data());
}

QWalletDummyTxPtr AssistedWallet::dummyTxPtr() const
{
    return QWalletDummyTx::information<QWalletDummyTxPtr>(walletId());
}

void AssistedWallet::GetUserTxNotes()
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

QString AssistedWallet::GetUserTxNote(const QString &txid)
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

void AssistedWallet::GetGroupTxNotes()
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

QString AssistedWallet::GetGroupTxNote(const QString &txid) {
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
