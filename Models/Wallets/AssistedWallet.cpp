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

AssistedWallet::AssistedWallet(const nunchuk::Wallet &w) :
    SharedWallet{w}
{}

void AssistedWallet::convert(const nunchuk::Wallet w)
{
    BaseWallet::convert(w);
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
        QWalletCached<QString, QString, QString, QString, bool, bool, bool> data;
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

QJsonObject AssistedWallet::GetServerKeyInfo(const QString &txid)
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
    QWalletCached<QString, QString, QString, QString, bool, bool, bool> data;
    bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
    if(ret){
        return data.hideFiatCurrency;
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
        QWalletCached<QString, QString, QString, QString, bool, bool, bool> data;
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
        QWalletCached<QString, QString, QString, QString, bool, bool, bool> data;
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
        QWalletCached<QString, QString, QString, QString, bool, bool, bool> data;
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
    if(isGroupWallet()){
        GetGroupTxs();
    }
    else if(isUserWallet()){
        GetUserTxs();
    }
    else{}
}

void AssistedWallet::GetAssistedCancelledTxs()
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


QTransactionPtr AssistedWallet::SyncAssistedTxs(const nunchuk::Transaction &tx)
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

void AssistedWallet::UpdateAssistedTxs(const QString &txid, const QString &memo)
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

void AssistedWallet::CancelAssistedTxs(const QString &txid)
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

void AssistedWallet::CreateAsisstedTxs(const QString &txid, const QString &psbt, const QString &memo)
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

void AssistedWallet::SignAsisstedTxs(const QString &tx_id, const QString &psbt, const QString &memo)
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

bool AssistedWallet::RbfAsisstedTxs(const QString &tx_id, const QString &psbt)
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

void AssistedWallet::UpdateWallet(const QString &name, const QString &description)
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
        BaseWallet::UpdateWallet(name, description);
    }
}

bool AssistedWallet::DeleteAssistedWallet()
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

void AssistedWallet::GetUserTxs()
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

void AssistedWallet::GetUserCancelledTxs()
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

QTransactionPtr AssistedWallet::SyncUserTxs(const nunchuk::Transaction &tx)
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

void AssistedWallet::UpdateUserTxs(const QString &txid, const QString &memo)
{
    if(isUserWallet()){
        Draco::instance()->assistedWalletUpdateTx(walletId(),txid, memo);
    }
}

void AssistedWallet::CancelUserTxs(const QString &txid)
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

void AssistedWallet::CreateUserTxs(const QString &txid, const QString &psbt, const QString &memo)
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

void AssistedWallet::SignUserTxs(const QString &tx_id, const QString &psbt, const QString &memo)
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

void AssistedWallet::UpdateUserWallet(const QString &name, const QString &description)
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

void AssistedWallet::GetGroupTxs()
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

void AssistedWallet::GetGroupCancelledTxs()
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

QTransactionPtr AssistedWallet::SyncGroupTxs(const nunchuk::Transaction &tx)
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

void AssistedWallet::UpdateGroupTxs(const QString &txid, const QString &memo)
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

void AssistedWallet::CancelGroupTxs(const QString &txid)
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

void AssistedWallet::CreateGroupTxs(const QString &txid, const QString &psbt, const QString &memo)
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

void AssistedWallet::SignGroupTxs(const QString &tx_id, const QString &psbt, const QString &memo)
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

void AssistedWallet::UpdateGroupWallet(const QString &name, const QString &description)
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
