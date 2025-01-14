#include "QGroupWallets.h"
#include "Chats/ClientController.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "Premiums/QUserWallets.h"

QGroupWallets::QGroupWallets()
    : QAssistedDraftWallets(GROUP_WALLET)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(this, &QGroupWallets::dashboardList, this, &QGroupWallets::MakePendingDashboardList, Qt::QueuedConnection);
    connect(this, &QGroupWallets::acceptChanged, this, &QGroupWallets::slotAcceptChanged, Qt::QueuedConnection);
}

QGroupWallets::~QGroupWallets()
{
    mPendingWallets.clear();
}

QGroupWallets *QGroupWallets::instance()
{
    static QGroupWallets mInstance;
    return &mInstance;
}

QGroupDashboard* QGroupWallets::dashboardInfo()
{
    return dashboardInfoPtr().data();
}

QGroupDashboardPtr QGroupWallets::dashboardInfoPtr()
{
    if (mDashboard) {
        return mDashboard;
    } else {
        mDashboard = AppModel::instance()->walletInfoPtr() ? AppModel::instance()->walletInfoPtr()->dashboard() : nullptr;
    }
    if (mDashboard) {
        QString myRole = mDashboard->myRole();
        if(qUtils::strCompare(myRole, "KEYHOLDER_LIMITED")){
            mDashboard->setShowDashBoard(true);
        }
    }
    return mDashboard;
}

QList<QObject*> QGroupWallets::dashboards() const
{
    QList<QObject*> list;
    for (auto d : mPendingWallets) {
        list.append(d.data());
    }
    return list;
}

void QGroupWallets::GetAllGroups()
{
    if(!CLIENT_INSTANCE->isNunchukLoggedIn()){
        return;
    }
    QUserWallets::instance()->GetDraftWallet();
    QtConcurrent::run([=, this]() {
        QJsonObject output;
        QString error_msg = "";
        bool ret = Byzantine::instance()->GetAllGroupWallets(output, error_msg);
        QJsonArray groupList;
        auto draft = QUserWallets::instance()->dashboardInfoPtr();
        if (draft) {
            groupList.append(draft->groupInfo());
        }
        if (ret) {
            QJsonArray groups = output["groups"].toArray();
            for (auto v : groups) {
                QJsonObject group = v.toObject();
                groupList.append(group);
                DBG_INFO << group["id"].toString() << group["status"].toString();
            }
            GetListAllRequestAddKey(groups);
            emit dashboardList(groupList);
        } else {
            //Show error
            DBG_INFO << error_msg;
        }
    });
}

bool QGroupWallets::AcceptGroupWallet()
{
    if (!mDashboard) return false;
    QJsonObject output;
    QString error_msg = "";
    bool ret = Byzantine::instance()->AcceptGroupWallet(mDashboard->groupId(), output, error_msg);
    if(ret){
        DBG_INFO;
    }
    else{
        //Show error
    }
    return ret;
}

void QGroupWallets::DenyGroupWallet()
{
    if (!mDashboard) return;
    QJsonObject output;
    QString error_msg = "";
    bool ret = Byzantine::instance()->DenyGroupWallet(mDashboard->groupId(), output, error_msg);
    if(ret){
        GetAllGroups();
    }
    else{
        //Show error
    }
}

void QGroupWallets::ResetGroupWallet()
{
    if (!mDashboard) return;
    QJsonObject output;
    QString error_msg = "";
    bool ret {false};
    if (mDashboard->isUserDraftWallet()) {
        ret = Draco::instance()->DraftWalletResetCurrent(output, error_msg);
    } else {
        ret = Byzantine::instance()->ResetGroupWallet(mDashboard->groupId(), output, error_msg);
    }
    if(ret){
        GetAllGroups();
        QString msg_name = QString("Wallet has been canceled");
        AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
    }
}

void QGroupWallets::MakePendingDashboardList(const QJsonArray &groups)
{
    DBG_INFO << groups.size();
    QStringList groupids;
    for (auto js : groups) {
        QJsonObject info = js.toObject();
        QString group_status = info["status"].toString();
        QString group_id = "";
        QGroupDashboardPtr dashboard = QGroupDashboardPtr(new QGroupDashboard(""));
        dashboard->setGroupInfo(info);
        if (qUtils::strCompare("PENDING_WALLET", group_status)) {
            group_id = info["id"].toString();
        }
        else if (qUtils::strCompare("ACTIVE", group_status)) {
            if(!dashboard->accepted()){
                group_id = info["id"].toString();
            }
        }
        else{ // status = "DELETED"
            continue;
        }
        auto draft = QUserWallets::instance()->dashboardInfoPtr();
        if (draft && draft->groupId() == group_id) {
            dashboard = draft;
        }
        if(group_id != ""){
            groupids.append(group_id);
            if (!Contains(group_id)) {
                mPendingWallets.append(dashboard);
            }
            else {
                GetDashboard(group_id)->setGroupInfo(info);
            }
        }
    }
    for (auto remove: mPendingWallets) {
        if (!groupids.contains(remove->groupId())) {
            mPendingWallets.removeOne(remove);
            if (mDashboard && qUtils::strCompare(mDashboard->groupId(), remove->groupId())) {
                mDashboard->setShowDashBoard(false);
                mDashboard.clear();
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
            }
        }
    }
    if (groups.size() == 0 && mPendingWallets.size() > 0) {
        mPendingWallets.clear();
    }
    if (mPendingWallets.size() > 0) {
        if (mDashboard.isNull()) {
            if (auto w = AppModel::instance()->walletInfoPtr()) {
                if (w->id().isEmpty()) { // There not wallet selected
                    dashboard(mPendingWallets.first()->groupId(), "");
                    mDashboard->setShowDashBoard(true);
                }
            }
        }
    } else if (AppModel::instance()->walletListPtr() && AppModel::instance()->walletListPtr()->rowCount() > 0){
        if (mDashboard && (mDashboard->hasWallet() == false)) {
            mDashboard->setShowDashBoard(false);
            mDashboard.clear();
        }
        if (mDashboard.isNull()) {
            if (auto w = AppModel::instance()->walletInfoPtr()) {
                if (w->id().isEmpty()) { // There not wallet selected
                    if (auto w_0 = AppModel::instance()->walletListPtr()->getWalletByIndex(0)) {
                        dashboard(w_0->groupId(), w_0->id());
                    }
                }
            }
            else if (auto w_0 = AppModel::instance()->walletListPtr()->getWalletByIndex(0)) {
                dashboard(w_0->groupId(), w_0->id());
            }
        }
    }
    clearDashBoard();
    emit dashboardListChanged();
}

void QGroupWallets::dashboard(const QString &group_id, const QString& wallet_id)
{
    setDashboardInfo(group_id, wallet_id);
    DBG_INFO << mDashboard;
    if (mDashboard) {
        mDashboard->setShowDashBoard(true);
        QtConcurrent::run([this]() {
            if(mDashboard){
                mDashboard->GetAlertsInfo();
                mDashboard->GetMemberInfo();
                mDashboard->GetWalletInfo();
                mDashboard->GetHealthCheckInfo();
            }
        });
        mDashboard->GetDraftWalletInfo();
    }
}

void QGroupWallets::accept(const QString &group_id)
{
    setDashboardInfo(group_id);
    if (!mDashboard) return;
    mDashboard->setShowDashBoard(true);
    QtConcurrent::run([this, group_id]() {
        if (AcceptGroupWallet()) {
            WalletsMng->GetListWallet(GROUP_WALLET); // active wallet
            emit acceptChanged(group_id);
        }
    });
}

void QGroupWallets::deny(const QString &group_id)
{
    setDashboardInfo(group_id);
    if (!mDashboard) return;
    mDashboard->setShowDashBoard(false);
    QtConcurrent::run([this]() {
        DenyGroupWallet();
    });
}

void QGroupWallets::reset(const QString &group_id)
{
    setDashboardInfo(group_id);
    if (!mDashboard) return;
    mDashboard->setShowDashBoard(false);
    QtConcurrent::run([this]() {
        ResetGroupWallet();
    });
}

void QGroupWallets::markRead(const QString &alert_id)
{
    DBG_INFO << alert_id;
    if (!mDashboard) return;
    mDashboard->setAlertId(alert_id);
    if (mDashboard->canEntryClickAlert()) {
        switch ((AlertEnum::E_Alert_t)mDashboard->flow()) {
        case AlertEnum::E_Alert_t::TRANSACTION_SIGNATURE_REQUEST:{
            QJsonObject payload = mDashboard->alertJson()["payload"].toObject();
            DBG_INFO << payload;
            QString transaction_id = payload["transaction_id"].toString();
            QEventProcessor::instance()->sendEvent(E::EVT_HOME_TRANSACTION_INFO_REQUEST, transaction_id);
            if(auto tx = AppModel::instance()->transactionInfoPtr()) {
                tx->setHasMoreBtn(false);
            }
            break;
        }
        default:
            QEventProcessor::instance()->sendEvent(E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST);
            break;
        }
    }
    QtConcurrent::run([this, alert_id]() {
        if (mDashboard->MarkAlertAsRead(alert_id)) {
            mDashboard->GetAlertsInfo();
        }
    });
}

void QGroupWallets::dismiss(const QString &alert_id)
{
    DBG_INFO << alert_id;
    if (!mDashboard) return;
    if (mDashboard->DismissAlert(alert_id)) {
        mDashboard->GetAlertsInfo();
    }
}

void QGroupWallets::refresh()
{
    if (!mDashboard) return;
    mDashboard->GetDraftWalletInfo();
    QtConcurrent::run([this]() {
        if (mDashboard->hasWallet()) {
            mDashboard->GetWalletInfo();
        }
    });
}

void QGroupWallets::setDashboardInfo(const QString &group_id, const QString &wallet_id)
{
    if (wallet_id != "") {
        if (auto w = AppModel::instance()->walletListPtr()->getWalletById(wallet_id))
        {
            DBG_INFO << wallet_id << w->dashboard();
            if (auto dash = w->dashboard()) {
                mDashboard = dash;
            }
        }
        DBG_INFO << wallet_id << mDashboard;
        emit dashboardInfoChanged();
    }
    else if (group_id != "") {
        if (mDashboard) {
            if (qUtils::strCompare(group_id, mDashboard->groupId())){
                return;
            }
            else{
                mDashboard->setShowDashBoard(false);
            }
        }

        for (auto ptr : mPendingWallets) {
            if (qUtils::strCompare(group_id, ptr->groupId())){
                mDashboard = ptr;
            }
        }

        QString w_id = WalletsMng->walletId(group_id);
        if (auto w = AppModel::instance()->walletListPtr()->getWalletById(w_id))
        {
            if (auto dash = w->dashboard()) {
                mDashboard = dash;
            }
        }
        emit dashboardInfoChanged();
    }
    else {
        if (mDashboard) {
            mDashboard->setShowDashBoard(false);
            mDashboard.clear();
            emit dashboardInfoChanged();
        }
    }
}

void QGroupWallets::setDashboardInfo(const QWalletPtr &wallet)
{
    if (wallet) {
        if (auto dash = wallet->dashboard()) {
            mDashboard = dash;
        }
        else {
            if (mDashboard) {
                mDashboard->setShowDashBoard(false);
                if (!wallet->id().isEmpty()) {
                    mDashboard.clear();
                }
                emit dashboardInfoChanged();
            }
        }
        emit dashboardInfoChanged();
    }
}

void QGroupWallets::slotAcceptChanged(const QString &group_id)
{
    setDashboardInfo(group_id);
    if (mDashboard) {
        mDashboard->GetAlertsInfo();
        mDashboard->GetMemberInfo();
        mDashboard->GetWalletInfo();
        mDashboard->GetHealthCheckInfo();
        GetAllGroups(); // reset pending wallet
    }
}

void QGroupWallets::clearDashBoard()
{
    if (mPendingWallets.size() == 0 && WalletsMng->activeSize() == 0) {
        setDashboardInfo(""); //Clear DashBoard
    }
}

void QGroupWallets::requestManageGroupChatHistory()
{
    if (mDashboard) {
        mDashboard->GetGroupChat();
        GetHistoryPeriods();
        mDashboard->setFlow((int)AlertEnum::E_Alert_t::MANAGE_GROUP_CHAT_HISTORY);
        QEventProcessor::instance()->sendEvent(E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST);
    }
}

void QGroupWallets::reset()
{
    QAssistedDraftWallets::reset();
    if (mDashboard) {
        mDashboard->setShowDashBoard(false);
        mDashboard->setGroupInfo(QJsonObject());
    }
    if (mPendingWallets.size() > 0) {
        mPendingWallets.clear();
    }
    clearDashBoard();
}

bool QGroupWallets::AddOrUpdateAKeyToDraftWallet()
{
    return QAssistedDraftWallets::AddOrUpdateAKeyToDraftWallet();
}


bool QGroupWallets::requestKeyReplacement(QSingleSignerPtr signer)
{
    return QAssistedDraftWallets::requestKeyReplacement(signer);
}


bool QGroupWallets::Contains(const QString &group_id)
{
    for (auto ptr : mPendingWallets) {
        if (ptr->groupId() == group_id) {
            return true;
        }
    }
    return false;
}

QGroupDashboardPtr QGroupWallets::GetDashboard(const QString &group_id)
{
    for (auto ptr : mPendingWallets) {
        if (ptr->groupId() == group_id) {
            return ptr;
        }
    }
    return {};
}

void QGroupWallets::findPermissionAccount()
{
    QJsonObject first;
    first["role"] = "MASTER";
    first["hasGroupWallet"] = AppModel::instance()->walletListPtr()->existGroupWallet();
    first["hasPro"] = true;
    first["hasGroupPending"] = existGroupPending();
    setHighestPermissionAccount(first);

    QJsonObject currentMember;
    auto getMember = [&] (const QGroupDashboardPtr dash) -> QJsonObject {
        if (dash) {
            QString new_role = dash->role();
            QString cur_role = currentMember["role"].toString();
            if (new_role == "MASTER" || cur_role == "MASTER") {
                currentMember = new_role == "MASTER" ? dash->myInfo() : currentMember;
            } else if (new_role == "ADMIN" || cur_role == "ADMIN") {
                currentMember = new_role == "ADMIN" ? dash->myInfo() : currentMember;
            } else if (new_role == "KEYHOLDER" || cur_role == "KEYHOLDER") {
                currentMember = new_role == "KEYHOLDER" ? dash->myInfo() : currentMember;
            } else if (new_role == "KEYHOLDER_LIMITED" || cur_role == "KEYHOLDER_LIMITED") {
                currentMember = new_role == "KEYHOLDER_LIMITED" ? dash->myInfo() : currentMember;
            } else if (new_role == "FACILITATOR_ADMIN" || cur_role == "FACILITATOR_ADMIN") {
                currentMember = new_role == "FACILITATOR_ADMIN" ? dash->myInfo() : currentMember;
            } else if (new_role == "OBSERVER" || cur_role == "OBSERVER") {
                currentMember = new_role == "OBSERVER" ? dash->myInfo() : currentMember;
            } else if (new_role == "CUSTOMIZE" || cur_role == "CUSTOMIZE") {
                currentMember = new_role == "CUSTOMIZE" ? dash->myInfo() : currentMember;
            } else {
                DBG_INFO << "Unknow " << dash->myInfo();
            }
        }
        return currentMember;
    };

    if (AppModel::instance()->walletListPtr()->existProWallet()) {
        for (auto w : AppModel::instance()->walletListPtr()->fullList()) {
            auto dash = w->dashboard();
            if (w->isPro()) {
                getMember(dash);
            }
        }
        currentMember["hasPro"] = true;
    } else {
        for (auto w : AppModel::instance()->walletListPtr()->fullList()) {
            auto dash = w->dashboard();
            if (!w->isPro()) {
                getMember(dash);
            }
        }
        currentMember["hasPro"] = false;
    }
    currentMember["hasGroupWallet"] = AppModel::instance()->walletListPtr()->existGroupWallet();
    currentMember["hasGroupPending"] = existGroupPending();

    setHighestPermissionAccount(currentMember);

}

void QGroupWallets::setHighestPermissionAccount(const QJsonObject &highestPermissionAccount)
{
    QString role = highestPermissionAccount["role"].toString();
    if (role.isEmpty()) return;
    if (mHighestPermissionAccount == highestPermissionAccount)
        return;
    mHighestPermissionAccount = highestPermissionAccount;
    DBG_INFO << "High Permission Account:  " << mHighestPermissionAccount;
    emit highestPermissionAccountChanged();
}

QVariant QGroupWallets::highestPermissionAccount()
{
    return QVariant::fromValue(mHighestPermissionAccount);
}

bool QGroupWallets::existGroupPending() const
{
    return mPendingWallets.size() > 0;
}

void QGroupWallets::GetHistoryPeriods()
{
    QJsonObject output;
    QString error_msg = "";
    bool ret = Byzantine::instance()->GetHistoryPeriodsGroupChat(output, error_msg);
    if (ret) {
        DBG_INFO << output;
        QJsonArray history_periods = output["history_periods"].toArray();
        setHistoryPeriods(history_periods);
    } else {
        //Show error
        DBG_INFO << error_msg;
    }
}

QVariantList QGroupWallets::historyPeriods() const
{
    return mHistoryPeriods.toVariantList();
}

void QGroupWallets::setHistoryPeriods(QJsonArray historyPeriods)
{
#if 1
    QJsonArray filteredArray;
    for (const QJsonValue &value : historyPeriods) {
        QJsonObject obj = value.toObject();
        if (obj.contains("enabled") && obj["enabled"].toBool()) {
            filteredArray.append(obj);
        }
    }
    mHistoryPeriods = filteredArray;
#else // FIXME - TEST
    mHistoryPeriods = historyPeriods;
#endif
    emit historyPeriodsChanged();
}

void QGroupWallets::SignInDashBoard(QGroupDashboardPtr dashboard)
{
    mDashboard = dashboard;
}

void QGroupWallets::updateRequestKey()
{
    GetAllGroups();
}

int QGroupWallets::currentIndex() const
{
    return mCurrentIndex;
}

void QGroupWallets::setCurrentIndex(int currentIndex)
{
    mCurrentIndex = currentIndex;
    emit currentIndexChanged();
}
