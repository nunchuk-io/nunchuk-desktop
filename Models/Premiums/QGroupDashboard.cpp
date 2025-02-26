#include "QGroupDashboard.h"
#include "ViewsEnums.h"
#include "Chats/ClientController.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "QGroupWalletHealthCheck.h"
#include "QGroupWalletDummyTx.h"
#include "qUtils.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritancePlan.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QKeyRecovery.h"
#include "Premiums/QRecurringPayment.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QSignerManagement.h"

int StringToInt(const QString &type) {
    const QMetaObject &mo = AlertEnum::staticMetaObject;
    int enum_index        = mo.indexOfEnumerator("E_Alert_t");
    QMetaEnum metaEnum    = mo.enumerator(enum_index);
    return metaEnum.keyToValue(type.toStdString().c_str());
}

QString IntToString(const int type) {
    const QMetaObject &mo = AlertEnum::staticMetaObject;
    int enum_index        = mo.indexOfEnumerator("E_Alert_t");
    QMetaEnum metaEnum    = mo.enumerator(enum_index);
    return QString(metaEnum.valueToKey(type));
}

QGroupDashboard::QGroupDashboard(const QString& wallet_id)
    : QBasePremium(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    connect(CLIENT_INSTANCE, &ClientController::byzantineRoomCreated,   this, &QGroupDashboard::byzantineRoomCreated);
    connect(CLIENT_INSTANCE, &ClientController::byzantineRoomDeleted,   this, &QGroupDashboard::byzantineRoomDeleted);
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &QGroupDashboard::updateProgress);
}

QGroupDashboard::~QGroupDashboard()
{
}

GroupId QGroupDashboard::groupId() const
{
    GroupId group_id = WalletsMng->groupId(wallet_id());
    return group_id.isEmpty() ? groupInfo()["id"].toString() : group_id;
}

QJsonObject QGroupDashboard::groupInfo() const
{
    return m_groupInfo;
}

void QGroupDashboard::setGroupInfo(const QJsonObject &groupInfo)
{
    QJsonArray members_old = m_groupInfo["members"].toArray();
    QJsonArray members_new = groupInfo["members"].toArray();
    if (!groupInfo["id"].toString().isEmpty() && (members_old.size() == 0 || members_new.size() != 0)) {
        m_groupInfo = groupInfo;
        QJsonArray members = members_new;
        for (QJsonValue js : members) {
            QJsonObject member = js.toObject();
            QString inviter_user_id = member["inviter_user_id"].toString();
            if (inviter_user_id == "") {
                setInviterInfo(member);
            }
            DracoUser   me   = ClientController::instance()->getMe();
            QString user_id  = member["user"].toObject()["id"].toString();
            if(qUtils::strCompare(user_id, me.id)){
                setMyInfo(member);
            }
        }
        emit groupInfoChanged();
    }
}

QString QGroupDashboard::groupStatus() const
{
    DBG_INFO << groupInfo()["status"].toString();
    return groupInfo()["status"].toString();
}

QString QGroupDashboard::myRole() const
{
    // return "FACILITATOR_ADMIN"; //FIXME TBD
    // DBG_INFO << myInfo()["role"].toString();
    return myInfo()["role"].toString();
}

QString QGroupDashboard::walletname() const
{
    return walletJson()["name"].toString();
}

QString QGroupDashboard::walletStatus() const
{
    return walletJson()["status"].toString();
}

bool QGroupDashboard::isReplaced()
{
    return qUtils::strCompare("REPLACED", walletStatus());
}

bool QGroupDashboard::accepted() const
{
    bool ret {false};
    if (isUserWallet()) {
        ret = true;
    }
    else {
        ret = qUtils::strCompare(myInfo()["status"].toString(), "ACTIVE");
    }
    return ret;
}

QString QGroupDashboard::userName() const
{
    return inviterInfo()["user"].toObject()["name"].toString();
}

QString QGroupDashboard::userEmail() const
{
    return inviterInfo()["user"].toObject()["email"].toString();
}

QVariantList QGroupDashboard::members() const
{
    if (isUserDraftWallet()) {
        return {};
    }
    return groupInfo()["members"].toArray().toVariantList();
}

QVariantList QGroupDashboard::memberSignatures()
{
    QJsonArray origin = groupInfo()["members"].toArray();
    QJsonArray result;
    QString my_membership_id = myInfo()["membership_id"].toString();

    for (auto member : origin) {
        QJsonObject it = member.toObject();
        QString membership_id = it["membership_id"].toString();
        QString status = it["status"].toString();
        QString role   = it["role"].toString(); //MASTER, ADMIN, KEYHOLDER, KEYHOLDER_LIMITED, OBSERVER, CUSTOMIZE

        bool isNotMe = !qUtils::strCompare(my_membership_id, membership_id);
        bool isActived = qUtils::strCompare(status, "ACTIVE");
        bool role_allowed = !qUtils::strCompare(role, "OBSERVER");
        if(isNotMe && isActived && role_allowed){
            result.push_back(member);
        }
    }
    return result.toVariantList();
}

QVariant QGroupDashboard::inviter() const
{
    return QVariant::fromValue(inviterInfo());
}

QJsonObject QGroupDashboard::walletJson() const
{
    return m_walletInfo;
}

void QGroupDashboard::GetMemberInfo()
{
    if (isUserWallet() || isReplaced() || isUserDraftWallet()) return;
    QPointer<QGroupDashboard> safeThis(this);
    runInThread<QJsonObject>([safeThis]() ->QJsonObject{
        QJsonObject output;
        QString error_msg = "";
        Byzantine::instance()->GetOneGroupWallets(safeThis->groupId(), output, error_msg);
        return output["group"].toObject();
    },[safeThis](QJsonObject group) {
        if(!group.isEmpty()) {
            safeThis->setGroupInfo(group);
        }
    });
}

void QGroupDashboard::GetAlertsInfo()
{
    if (isReplaced()) return;
    QPointer<QGroupDashboard> safeThis(this);
    runInThread<QJsonArray>([safeThis]() ->QJsonArray{
        QJsonObject output;
        QString error_msg = "";
        bool ret {false};
        if (safeThis->isUserDraftWallet()) {
            ret = Draco::instance()->DraftWalletGetAlerts(output, error_msg);
        }
        else if (safeThis->isUserWallet()) {
            ret = Draco::instance()->GetAlerts(safeThis->wallet_id(), output, error_msg);
        }
        else {
            ret = Byzantine::instance()->GetGroupAlerts(safeThis->groupId(), output, error_msg);
        }
        DBG_INFO << ret;
        return output["alerts"].toArray();
    },[safeThis](QJsonArray alerts) {
        if(!alerts.isEmpty()) {
            QJsonArray v_alerts;
            for (auto js : alerts) {
                QJsonObject alert = js.toObject();
                long int created_time_millis = static_cast<long int>(alert.value("created_time_millis").toDouble()/1000);
                QDateTime date_time = QDateTime::fromTime_t(created_time_millis);
                alert["created_time_millis"] = QString("%1 at %2")
                                                   .arg(date_time.date().toString("MM/dd/yyyy"))
                                                   .arg(date_time.time().toString("hh:mm AP"));
                QString type = alert["type"].toString();
                alert["type"] = StringToInt(type);
                v_alerts.append(alert);
            }
            safeThis->m_alertInfo["alerts"] = v_alerts;
            emit safeThis->alertInfoChanged();
            DBG_INFO << safeThis->m_alertInfo;
            safeThis->setAlertId(safeThis->alertId());
        }
    });
}

bool QGroupDashboard::MarkAlertAsRead(const QString &alert_id)
{
    if (alert_id.isEmpty()) return false;
    QPointer<QGroupDashboard> safeThis(this);
    runInThread<bool>([safeThis, alert_id]() ->bool{
        QJsonObject output;
        QString error_msg = "";
        bool ret {false};
        if (safeThis->isUserDraftWallet()) {
            ret = Draco::instance()->DraftWalletMarkAnAlertAsRead(alert_id, output, error_msg);
        }
        else if (safeThis->isUserWallet()) {
            ret = Draco::instance()->MarkAlertAsRead(safeThis->wallet_id(), alert_id, output, error_msg);
        }
        else {
            ret = Byzantine::instance()->MarkGroupAlertAsRead(safeThis->groupId(), alert_id, output, error_msg);
        }
        DBG_INFO << ret << error_msg;
        return ret;
    },[safeThis](bool ret) {
        if(ret) {
            safeThis->GetAlertsInfo();
        }
    });
    return true;
}

bool QGroupDashboard::DismissAlert(const QString &alert_id)
{
    QPointer<QGroupDashboard> safeThis(this);
    runInThread<bool>([safeThis, alert_id]() ->bool{
        QJsonObject output;
        QString error_msg = "";
        bool ret {false};
        if (safeThis->isUserDraftWallet()) {
            ret = Draco::instance()->DraftWalletDismissAnAlert(alert_id, output, error_msg);
        }
        else if (safeThis->isUserWallet()) {
            ret = Draco::instance()->DismissAlert(safeThis->wallet_id(), alert_id, output, error_msg);
        }
        else {
            ret = Byzantine::instance()->DismissGroupAlert(safeThis->groupId(), alert_id, output, error_msg);
        }
        DBG_INFO << ret << error_msg;
        return ret;
    },[safeThis](bool ret) {
        if(ret) {
            safeThis->GetAlertsInfo();
        }
    });
    return false;
}

bool QGroupDashboard::DismissAlert()
{
    return DismissAlert(alertId());
}

void QGroupDashboard::GetWalletInfo()
{
    QPointer<QGroupDashboard> safeThis(this);
    runInThread<QJsonObject>([safeThis]() ->QJsonObject{
        QJsonObject output;
        QString error_msg = "";
        bool ret {false};
        if (safeThis->isUserDraftWallet()) {

        } else if (safeThis->isUserWallet()) {
            ret = Draco::instance()->assistedWalletGetInfo(safeThis->wallet_id(), output, error_msg);
        } else {
            ret = Byzantine::instance()->GetCurrentGroupWallet(safeThis->groupId(), output, error_msg);
        }
        return output["wallet"].toObject();
    },[safeThis](QJsonObject wallet) {
        if (wallet.isEmpty()) return;
        safeThis->m_walletInfo = wallet;
        DBG_INFO << wallet;
        QJsonArray signers = wallet["signers"].toArray();
        safeThis->m_signerInfo = signers;
        safeThis->checkInheritanceWallet();
        emit safeThis->groupInfoChanged();
        if (safeThis->walletInfoPtr() && safeThis->walletInfoPtr()->serverKeyPtr()) {
            safeThis->walletInfoPtr()->serverKeyPtr()->UpdateFromWallet(wallet);
        }
    });
}

void QGroupDashboard::checkInheritanceWallet()
{
    int size = 0;
    for(auto js : m_signerInfo) {
        QJsonObject signer = js.toObject();
        QJsonArray tags = signer["tags"].toArray();
        if (tags.contains("INHERITANCE")) {
            size++;
        }
    }
    setInheritanceCount(size);
}

void QGroupDashboard::GetDraftWalletInfo()
{
    QPointer<QGroupDashboard> safeThis(this);
    runInThread<QJsonObject>([safeThis]() ->QJsonObject{
        QJsonObject output;
        QString error_msg = "";
        bool ret {false};
        if (safeThis->isUserDraftWallet()) {
            ret = Draco::instance()->DraftWalletGetCurrent(output, error_msg);
        } else if (!safeThis->isUserWallet()){
            ret = Byzantine::instance()->GetCurrentGroupDraftWallet(safeThis->groupId(), output, error_msg);
        }
        return output["draft_wallet"].toObject();
    },[safeThis](QJsonObject draft_wallet) {
        if (draft_wallet.isEmpty()) return;
        DBG_INFO << draft_wallet;
        safeThis->m_walletDraftInfo = draft_wallet;
        safeThis->UpdateKeys(draft_wallet);
    });
}

void QGroupDashboard::GetHealthCheckInfo()
{
    if (!healthPtr() || wallet_id().isEmpty() || isReplaced()) return;
    healthPtr()->GetKeyHealthReminder();
    healthPtr()->GetStatuses();
}

void QGroupDashboard::UpdateKeys(const QJsonObject &data)
{
    QJsonObject wallet_config = data["wallet_config"].toObject();
    bool allow_inheritance = wallet_config["allow_inheritance"].toBool();
    int n = wallet_config["n"].toInt();
    int m = wallet_config["m"].toInt();

    m_nInfo = n;
    m_mInfo = m;
    m_allow_inheritance = allow_inheritance;
    bool required_server_key = wallet_config["required_server_key"].toBool();

    QJsonArray signers = data["signers"].toArray();
    QJsonObject server;
    server["type"] = "SERVER";
    server["has"]  = false;
    QJsonArray origin = groupInfo()["members"].toArray();
    QString our_id = "";
    for (auto member : origin) {
        QJsonObject it = member.toObject();
        QString user_id = it["user"].toObject()["id"].toString();
        QString email_or_username = it["email_or_username"].toString();
        if (qUtils::strCompare(ClientController::instance()->getMe().email, email_or_username)) {
            our_id = user_id;
            break;
        }
    }
    QJsonArray signers_tmp = signers;
    signers = {};
    for (auto js : signers_tmp) {
        QJsonObject signer = js.toObject();
        signer["ourAccount"] = false;
        signer["signer_type"] = (int)qUtils::GetSignerType(signer["type"].toString());
        signer["account_index"] = qUtils::GetIndexFromPath(signer["derivation_path"].toString());
        QJsonObject tapsignerObj = signer["tapsigner"].toObject();
        signer["card_id"] = tapsignerObj.isEmpty() ? "" : tapsignerObj["card_id"].toString();
        QString type = signer["type"].toString();
        if (type == "SERVER") {
            server = signer;
            server["has"] = true;
        }
        QString added_by_user_id = signer["added_by_user_id"].toString();
        if (our_id == added_by_user_id) {
            signer["ourAccount"] = true;
            WalletsMng->UpdateSigner(signer);
        }
        signers.append(signer);
    }
    emit WalletsMng->signalUpdateSigner();
    auto find = [&](const int index) -> QJsonObject{
        for (QJsonValue js : signers) {
            QJsonObject signer = js.toObject();
            int key_index = signer["key_index"].toInt(-1);
            if (key_index == index) {
                signer["has"] = true;
                QJsonArray tags = signer["tags"].toArray();
                signer["is_inheritance"] = tags.contains("INHERITANCE");
                for (auto tag : tags) {
                    if (tag.toString() != "INHERITANCE") {
                        signer["tag"] = tag.toString();
                    }
                }
                signer["signer_type"] = (int)qUtils::GetSignerType(signer["type"].toString());
                signer["account_index"] = qUtils::GetIndexFromPath(signer["derivation_path"].toString());
                QJsonObject tapsignerObj = signer["tapsigner"].toObject();
                if (!tapsignerObj.isEmpty()) {
                    signer["card_id"] = tapsignerObj["card_id"].toString();
                }
                return signer;
            }
        }
        int id = index - 1;
        if (id >= 0 && id < signers.size()) {
            QJsonObject signer = signers.at(id).toObject();
            int key_index = signer["key_index"].toInt(-1);
            if (key_index == -1) {
                QJsonArray tags = signer["tags"].toArray();
                signer["is_inheritance"] = tags.contains("INHERITANCE");
                for (auto tag : tags) {
                    if (tag.toString() != "INHERITANCE") {
                        signer["tag"] = tag.toString();
                    }
                }
                signer["key_index"] = index;
                signer["has"] = true;
                signer["signer_type"] = (int)qUtils::GetSignerType(signer["type"].toString());
                signer["account_index"] = qUtils::GetIndexFromPath(signer["derivation_path"].toString());
                return signer;
            }
        }
        return {};
    };
    m_keys.clear();

    bool is_wallet_2_of_4 = (m == 2 && n == 4);
    bool is_wallet_3_of_5 = (m == 3 && n == 5);
    int  last_index = required_server_key ? n - 1 : n;
    bool is_premier = CLIENT_INSTANCE->isByzantinePremier() || isPremierGroup();
    bool hasServer = !CLIENT_INSTANCE->isFinney();
    DBG_INFO << is_premier << m_allow_inheritance << m_mInfo << m_nInfo;

    if (is_wallet_2_of_4) {
        for(int i = 0; i < last_index; i++) {
            QJsonObject obj = find(i);
            if (obj.isEmpty()) {
                obj["is_inheritance"] = false;
                obj["key_index"] = i;
                obj["has"] = false;
                if((i == 0) && m_allow_inheritance && !is_premier){
                    obj["is_inheritance"] = true;
                }
            }
            m_keys.insert(i, QVariant::fromValue(obj));
        }
        if (required_server_key && hasServer) {
            m_keys.insert(last_index, QVariant::fromValue(server));
        }
    }
    else if (is_wallet_3_of_5) {
        for(int i = 0; i < last_index; i++) {
            QJsonObject obj = find(i);
            if (obj.isEmpty()) {
                obj["is_inheritance"] = false;
                obj["key_index"] = i;
                obj["has"] = false;
                if((i == 0 || i == 1) && m_allow_inheritance && !is_premier){
                    obj["is_inheritance"] = true;
                }
            }
            DBG_INFO << obj;
            m_keys.insert(i, QVariant::fromValue(obj));
        }
        if (required_server_key && hasServer) {
            m_keys.insert(last_index, QVariant::fromValue(server));
        }
    }
    else {
        for(int i = 0; i < last_index; i++) {
            QJsonObject obj = find(i);
            if (obj.isEmpty()) {
                obj["key_index"] = i;
                obj["has"] = false;
            }
            DBG_INFO << obj;
            m_keys.insert(i, QVariant::fromValue(obj));
        }
        if (required_server_key && hasServer) {
            m_keys.insert(last_index, QVariant::fromValue(server));
        }
    }
    emit draftWalletChanged();
}

bool QGroupDashboard::GetKeyReplacementStatus()
{
    QJsonObject output;
    QString errormsg;
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->GetKeyReplacementStatus(wallet_id(), output, errormsg);
    } else {
        ret = Byzantine::instance()->GetKeyReplacementStatus(groupId(), wallet_id(), output, errormsg);
    }
    if (ret) {
        DBG_INFO << output;
    }
    return ret;
}

QMap<QString, QVariant> QGroupDashboard::requestBodyUploadBackupFile(const QString &xfp, const QString &filePath)
{
    QString file_path = qUtils::QGetFilePath(filePath);
    DBG_INFO << file_path;
    QMap<QString, QVariant> body;
    QSingleSignerPtr single = AppModel::instance()->remoteSignerListPtr()->getSingleSignerByFingerPrint(xfp);
    if (single.isNull()) {
        QMasterSignerPtr master = AppModel::instance()->masterSignerListPtr()->getMasterSignerByXfp(xfp);
        if (master.isNull()) {
            QJsonObject json = GetDraftSigner(xfp);
            if (json.isEmpty()) {
                json = GetSigner(xfp);
            }
            DBG_INFO << json;
            body["key_name"] = json["name"].toString();
            body["key_type"] = json["type"].toString();
            body["key_xfp"] = json["xfp"].toString();
            QJsonObject tapsigner = json["tapsigner"].toObject();
            body["card_id"] = tapsigner["card_id"].toString();
            body["file"] = file_path;
        } else {
            body["key_name"] = master->name();
            body["key_type"] = qUtils::GetSignerTypeString(master->originMasterSigner().get_type());;
            body["key_xfp"] = master->fingerPrint();
            body["card_id"] = master->devicePtr()->cardId();
            body["file"] = file_path;
        }
    } else {
        body["key_name"] = single->name();
        body["key_type"] = qUtils::GetSignerTypeString(single->originSingleSigner().get_type());;
        body["key_xfp"] = single->fingerPrint();
        body["card_id"] = single->cardId();
        body["file"] = file_path;
    }
    return body;
}

bool QGroupDashboard::ReplacementUploadBackupFile(const QString &xfp, const QString &filePath)
{
    if (mTimer != nullptr) {
        AppModel::instance()->setAddSignerPercentage(1);
        mTimer->start(100);
    }
    QJsonObject output;
    QString error_msg = "";
    QMap<QString, QVariant> body = requestBodyUploadBackupFile(xfp, filePath);
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->ReplacementUploadBackupFile(wallet_id(), servicesTagPtr()->passwordToken(), body, output, error_msg);
    } else {
        ret = Byzantine::instance()->ReplacementUploadBackupFile(groupId(), wallet_id(), servicesTagPtr()->passwordToken(), body, output, error_msg);
    }
    if (ret) {
        updateSuccess();
    } else {
        updateFail();
    }
    return ret;
}

bool QGroupDashboard::DraftWalletUploadBackupFile(const QString &xfp, const QString &filePath)
{
    if (mTimer != nullptr) {
        AppModel::instance()->setAddSignerPercentage(1);
        mTimer->start(100);
    }
    QJsonObject output;
    QString error_msg = "";
    QMap<QString, QVariant> body = requestBodyUploadBackupFile(xfp, filePath);
    bool ret {false};
    if (isUserWallet() || isUserDraftWallet()) {
        ret = Draco::instance()->DraftWalletUploadBackupFile(body, output, error_msg);
    } else {
        ret = Byzantine::instance()->DraftWalletUploadBackupFile(groupId(), body, output, error_msg);
    }
    if (ret) {
        updateSuccess();
    } else {
        updateFail();
    }
    return ret;
}

QJsonObject QGroupDashboard::myInfo() const
{
    return m_myInfo;
}

void QGroupDashboard::setMyInfo(const QJsonObject &myInfo)
{
    m_myInfo = myInfo;
    emit groupInfoChanged();
}

bool QGroupDashboard::register_wallet()
{
    if (auto dummy = groupDummyTxPtr()) {
        QString current_xfp = dummy->currentXfp();
        return xfpExport(current_xfp);
    }
    return false;
}

bool QGroupDashboard::isLocked() const
{
    return groupInfo().contains("is_locked") ? groupInfo()["is_locked"].toBool() : false;
}

bool QGroupDashboard::registerKeyDone()
{
    QJsonObject payload = alertJson()["payload"].toObject();
    if (payload.contains("register_key_xfps")) {
        QStringList register_key_xfps = payload["register_key_xfps"].toVariant().toStringList();
        bool ret = m_registered_key_xfps.size() == register_key_xfps.size() && m_registered_key_xfps.size() > 0;
        if (ret) {
            DismissAlert();
        }
        else if (register_key_xfps.size() == 0) {
            DismissAlert();
        }
        return ret;
    }
    return false;
}

bool QGroupDashboard::registerKeyNext()
{
    QJsonObject payload = alertJson()["payload"].toObject();
    QStringList register_key_xfps = payload["register_key_xfps"].toVariant().toStringList();
    QStringList::iterator not_register = std::find_if_not(register_key_xfps.begin(), register_key_xfps.end(), [this](QString xfp) {return m_registered_key_xfps.contains(xfp);});
    if (not_register != register_key_xfps.end()) {
        m_registered_key_xfps.append(*not_register);
        return xfpExport(*not_register);
    }
    return false;
}

QString QGroupDashboard::alertId() const
{
    return alertJson()["id"].toString();
}

bool QGroupDashboard::canEntryClickAlert()
{
    int flow = alertJson()["type"].toInt();
    QJsonObject payload = alertJson()["payload"].toObject();
    DBG_INFO << ".qml " << IntToString(flow) << alertJson();
    setFlow(flow);
    QString dummy_transaction_id = payload["dummy_transaction_id"].toString();
    QString xfp = payload["xfp"].toString();
    if (auto dummy = groupDummyTxPtr()) {
        dummy->setCurrentXfp(xfp);
    }
    if (!dummy_transaction_id.isEmpty()) {
        if (healthPtr()) {
            healthPtr()->HealthCheckPendingForTx(dummy_transaction_id);
        }
    }
    switch ((AlertEnum::E_Alert_t)flow) {
    case AlertEnum::E_Alert_t::GROUP_WALLET_SETUP:{
        QStringList register_key_xfps = payload["register_key_xfps"].toVariant().toStringList();
        bool isClaimkey = payload["claim_key"].toBool();
        bool isRegisterkey = register_key_xfps.size() > 0;
        if (isClaimkey) {
            this->setConfigFlow("register-claim");
            return true;
        }
        else if (isRegisterkey) {
            return registerKeyNext();
        }
        else{
            this->setConfigFlow("accessing-wallet-configuration");
            return true;
        }
    }
    case AlertEnum::E_Alert_t::WALLET_PENDING:
    case AlertEnum::E_Alert_t::GROUP_WALLET_PENDING:{
        GetDraftWalletInfo();
        if (hasWallet()) {
            GetWalletInfo();
        }
        return true;
    }
    case AlertEnum::E_Alert_t::KEY_RECOVERY_APPROVED:{
        QString key_xfp = payload["key_xfp"].toString();
        return servicesTagPtr()->keyRecoveryPtr()->UserKeysRecoveryKey(key_xfp);
    }
    case AlertEnum::E_Alert_t::RECURRING_PAYMENT_CANCELATION_PENDING: {
        if (auto payment = recurringPaymentPtr()) {
            payment->CancelPaymentFromAlert();
        }
        return true;
    }
    case AlertEnum::E_Alert_t::RECURRING_PAYMENT_REQUEST: {
        if (auto payment = recurringPaymentPtr()) {
            payment->CreatePaymentFromAlert();
        }
        return true;
    }
    case AlertEnum::E_Alert_t::CHANGE_EMAIL_REQUEST:
    case AlertEnum::E_Alert_t::HEALTH_CHECK_REMINDER:
    case AlertEnum::E_Alert_t::UPDATE_SECURITY_QUESTIONS:
    case AlertEnum::E_Alert_t::CREATE_INHERITANCE_PLAN_SUCCESS:
    case AlertEnum::E_Alert_t::KEY_RECOVERY_REQUEST:
    case AlertEnum::E_Alert_t::CREATE_INHERITANCE_PLAN:
    case AlertEnum::E_Alert_t::UPDATE_INHERITANCE_PLAN:
    case AlertEnum::E_Alert_t::CANCEL_INHERITANCE_PLAN:
    case AlertEnum::E_Alert_t::UPDATE_SERVER_KEY:
    case AlertEnum::E_Alert_t::REQUEST_INHERITANCE_PLANNING:
    case AlertEnum::E_Alert_t::HEALTH_CHECK_PENDING:
    case AlertEnum::E_Alert_t::HEALTH_CHECK_REQUEST:{
        return true;
    }
    case AlertEnum::E_Alert_t::TRANSACTION_SIGNATURE_REQUEST:{
        return true;
    }
    case AlertEnum::E_Alert_t::KEY_REPLACEMENT_PENDING: {
        auto signer = GetSigner(xfp);
        payload["keyname"] = signer["name"].toString();
        bool is_inheritance = signer["tapsigner"].toObject()["is_inheritance"].toBool();
        payload["is_inheritance"] = is_inheritance;
        payload["key_index"] = signer["key_index"].toInt();
        auto alert = alertJson();
        alert["payload"] = payload;
        setAlertId(alert);
        GetKeyReplacementStatus();
        return true;
    }
    default:
        break;
    }
    return false;
}

bool QGroupDashboard::allowInheritance() const
{
    return m_allow_inheritance;
}

int QGroupDashboard::inheritanceCount() const
{
    return m_inheritanceCount;
}

void QGroupDashboard::setInheritanceCount(int count)
{
    if (m_inheritanceCount == count)
        return;
    m_inheritanceCount = count;
    emit inheritanceCountChanged();
}

void QGroupDashboard::GetGroupChat()
{
    QJsonObject output;
    QString error_msg = "";
    bool ret = Byzantine::instance()->GetGroupChat(groupId(), output, error_msg);
    if (ret) {
        DBG_INFO << output;
        QJsonObject chat = output["chat"].toObject();
        QJsonObject history_period = chat["history_period"].toObject();
        QString id = history_period["id"].toString();
        setHistoryPeriodId(id);
    } else {
        //Show error
        DBG_INFO << error_msg;
    }
}

void QGroupDashboard::UpdateGroupChat(const QString &history_period_id)
{
    QJsonObject output;
    QString error_msg = "";
    bool ret = Byzantine::instance()->UpdateGroupChat(groupId(), history_period_id, output, error_msg);
    if (ret) {
        DBG_INFO << output;
        QJsonObject chat = output["chat"].toObject();
        QJsonObject history_period = chat["history_period"].toObject();
        QString id = history_period["id"].toString();
        setHistoryPeriodId(id);
        QString room_id = chat["room_id"].toString();
        double duration_in_millis = history_period["duration_in_millis"].toDouble();
        ClientController::instance()->updateMessageMaxLifeTime(room_id, duration_in_millis);
        // show toast success
        AppModel::instance()->showToast(0, "Chat settings updated", EWARNING::WarningType::SUCCESS_MSG);
    } else {
        //Show error
        DBG_INFO << error_msg;
    }
}

QString QGroupDashboard::historyPeriodId() const
{
    return mHistoryPeriodId;
}

void QGroupDashboard::setHistoryPeriodId(const QString &newHistoryPeriodId)
{
    if (mHistoryPeriodId == newHistoryPeriodId)
        return;

    mHistoryPeriodId = newHistoryPeriodId;
    emit historyPeriodIdChanged();
}

void QGroupDashboard::getChatInfo()
{
    QtConcurrent::run([this](){
        QJsonObject output;
        QString error_msg = "";
        bool ret = Byzantine::instance()->GetCurrentGroupChat(groupId(), output, error_msg);
        if(ret){
            DBG_INFO << output;
            if(output.contains("chat")){
                QJsonObject chat = output["chat"].toObject();
                QString room_id = chat["room_id"].toString();
                DBG_INFO << room_id;
                ret = (room_id != "");
            }
        }
        setGroupChatExisted(ret);
    });
}

bool QGroupDashboard::groupChatExisted()
{
    return m_groupChatExisted;
}

void QGroupDashboard::setGroupChatExisted(bool existed)
{
    if (m_groupChatExisted != existed){
        m_groupChatExisted = existed;
        emit groupChatExistedChanged();
    }
}

bool QGroupDashboard::EditGroupMembers()
{
    QJsonObject output;
    QString error_msg = "";
    bool ret = Byzantine::instance()->EditGroupMembers(groupId(),
                                                       servicesTagPtr()->confirmCodeNonceBody(),
                                                        {},
                                                       servicesTagPtr()->passwordToken(),
                                                       servicesTagPtr()->secQuesToken(),
                                                       servicesTagPtr()->confirmToken(),
                                                       output,
                                                       error_msg);
    if(ret){
        DBG_INFO << output;
        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
        GetMemberInfo();
    }
    return ret;
}

bool QGroupDashboard::CalculateRequireSignaturesForEditingMembers()
{
    QJsonObject output;
    QString error_msg = "";
    bool ret = Byzantine::instance()->CalculateRequireSignaturesForEditingMembers(groupId(), bodyEditMembers(), output, error_msg);
    if(ret){
        DBG_INFO << output;
        QJsonObject resultObj = output["result"].toObject();
        servicesTagPtr()->setReqiredSignatures(resultObj);
        auto info = servicesTagPtr()->reqiredSignaturesInfo();
        if (info.type == (int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE) {
            if (RequestConfirmationCodeEditMembers()) {
                emit editMembersSuccessChanged();
            }
        } else if (info.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            emit editMembersSuccessChanged();
        } else {
            QString message = QString("Not Support Dummy Transaction");
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return ret;
}

bool QGroupDashboard::requestStartKeyReplacement(const QString &tag)
{
    QSignerManagement::instance()->clearExecute();
    if (ServiceSetting::instance()->existHardware(tag)) {
        QEventProcessor::instance()->sendEvent(E::EVT_EXIST_HARDWARE_REQ);
    } else {
        QEventProcessor::instance()->sendEvent(E::EVT_ADD_HARDWARE_REQUEST);
    }
    return true;
}

bool QGroupDashboard::requestStartKeyCreate(const QString &tag)
{
    QSignerManagement::instance()->clearExecute();
    if (ServiceSetting::instance()->existHardware(tag)) {
        QEventProcessor::instance()->sendEvent(E::EVT_ADD_HARDWARE_KEY_EXIST_REQ);
    } else {
        QEventProcessor::instance()->sendEvent(E::EVT_ADD_HARDWARE_KEY_REQUEST);
    }
    return true;
}

void QGroupDashboard::requestHealthCheck(const QString &xfp){
    if (auto wallet = walletInfoPtr()) {
        setConfigFlow("health-check-procedure");
        setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_STATUS);
        AppModel::instance()->setWalletInfo(wallet);
        QGroupWallets::instance()->setDashboardInfo(wallet);
        QEventProcessor::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
        if (auto health = healthPtr()) {
            health->HealthCheckForKey(xfp);
        }
    }
}

bool QGroupDashboard::requestByzantineChat()
{
    if(!groupInfo().empty()){
        QStringList invitees_id;
        QStringList invitees_name;
        QString     group_id = groupId();
        QJsonArray members = groupInfo()["members"].toArray();
        DracoUser me = CLIENT_INSTANCE->getMe();
        for (QJsonValue js : members) {
            QJsonObject member = js.toObject();
            QString chat_id = member["user"].toObject()["chat_id"].toString();
            QString chat_name = member["user"].toObject()["name"].toString();
            if(!qUtils::strCompare(chat_id, me.chat_id)){
                QString chat_id = member["user"].toObject()["chat_id"].toString();
                QString chat_name = member["user"].toObject()["name"].toString();
                if(chat_id != ""){
                    invitees_id << chat_id;
                    invitees_name << chat_name;
                }
            }
        }
        if(hasWallet()){
            QString wallet_name = AppModel::instance()->walletInfo() ? AppModel::instance()->walletInfo()->walletName() : "";
            if(wallet_name != ""){
                invitees_name.clear();
                invitees_name.append(wallet_name);
            }
        }
        DBG_INFO << invitees_id << invitees_name << historyPeriodId();
        if(invitees_id.size() > 0){
            CLIENT_INSTANCE->createRoomByzantineChat(invitees_id, invitees_name, group_id, "");
        }
    }
    return true;
}

void QGroupDashboard::byzantineRoomCreated(QString room_id, QString group_id, bool existed)
{
    if (isUserWallet()) return;
    QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_CHAT_TAB);
    if(qUtils::strCompare(group_id, groupId()) && room_id != "" && !existed){
        DBG_INFO << room_id << existed;
        QJsonObject output;
        QString error_msg = "";
        bool ret = Byzantine::instance()->CreateGroupChat(groupId(), room_id, output, error_msg);
        if(!ret){
            //TBD
        }
        getChatInfo();
    }
}

void QGroupDashboard::byzantineRoomDeleted(QString room_id, QString group_id)
{
    if (isUserWallet()) return;
    if(qUtils::strCompare(group_id, groupId())){
        DBG_INFO << room_id << group_id;
        QJsonObject output;
        QString error_msg = "";
        bool ret = Byzantine::instance()->DeleteGroupChat(group_id, output, error_msg);
        if(!ret){
            DBG_INFO << output << error_msg;
        }
    }
}

void QGroupDashboard::requestShowLetAddYourKeys()
{
    if (isUserDraftWallet()) {
        setFlow((int)AlertEnum::E_Alert_t::WALLET_PENDING);
    }
    else {
        setFlow((int)AlertEnum::E_Alert_t::GROUP_WALLET_PENDING);
    }
    QEventProcessor::instance()->sendEvent(E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST);
}

void QGroupDashboard::requestShowReplacementKey()
{
    setFlow((int)AlertEnum::E_Alert_t::KEY_REPLACEMENT_PENDING);
    QEventProcessor::instance()->sendEvent(E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST);
}

bool QGroupDashboard::isDowngrade(QString email_or_username, QString roleNew)
{
    QString roleOld = "";
    for (auto js : groupInfo()["members"].toArray()) {
        auto email_or_username_tmp = js.toObject()["email_or_username"].toString();
        if (qUtils::strCompare(email_or_username, email_or_username_tmp)) {
            roleOld = js.toObject()["role"].toString();
        }
    }
    DBG_INFO << email_or_username << roleOld << roleNew;
    if (roleOld.isEmpty()) return false;

    static QMap<QString, int> maps = {
        {"MASTER", 1},
        {"ADMIN", 2},
        {"KEYHOLDER", 3},
        {"KEYHOLDER_LIMITED", 4},
        {"FACILITATOR_ADMIN", 5},
        {"OBSERVER", 6},
        {"CUSTOMIZE", 7}
    };
    if (qUtils::strCompare(roleOld, "FACILITATOR_ADMIN") && qUtils::strCompare(myRole(), "FACILITATOR_ADMIN")) {
        return false;
    }
    return maps[roleOld] < maps[roleNew] && qUtils::strCompare(myRole(), "FACILITATOR_ADMIN");
}

void QGroupDashboard::updateProgress() {
    int current = AppModel::instance()->getAddSignerPercentage();
    if (current + 20 < 100) {
        AppModel::instance()->setAddSignerPercentage(current + 10);
    }
}

void QGroupDashboard::updateSuccess()
{
    AppModel::instance()->setAddSignerPercentage(100);
    mTimer->stop();
}

void QGroupDashboard::updateFail()
{
    AppModel::instance()->setAddSignerPercentage(-1);
    mTimer->stop();
}

void QGroupDashboard::markRead()
{
    MarkAlertAsRead(alertId());
}

bool QGroupDashboard::deviceExport(const QStringList tags, nunchuk::SignerType type)
{
    DBG_INFO << tags << qUtils::GetSignerTypeString(type);
    bool ret {true};
    if (type == nunchuk::SignerType::COLDCARD_NFC || tags.contains("COLDCARD")) {
        setConfigFlow("register-COLDCARD");
    }
    else if (tags.contains("BITBOX")) {
        setConfigFlow("register-BitBox");
    }
    else if (type == nunchuk::SignerType::AIRGAP) {
        setConfigFlow("register-gapped-device");
    }
    else {
        ret = false;
        QString errormsg = "This device not support register wallet via QR and file";
        AppModel::instance()->showToast(0, errormsg, EWARNING::WarningType::ERROR_MSG);
    }
    return ret;
}

bool QGroupDashboard::xfpExport(const QString xfp)
{
    QWalletPtr w = AppModel::instance()->walletListPtr()->getWalletById(wallet_id());
    if (w.isNull()) return false;
    nunchuk::Wallet wallet = w->nunchukWallet();
    nunchuk::SingleSigner signer = *std::find_if(wallet.get_signers().begin(), wallet.get_signers().end(), [xfp](const nunchuk::SingleSigner &s) {
        return s.get_master_fingerprint() == xfp.toStdString();
    });
    QStringList tags;
    for (auto tag : signer.get_tags()) {
        tags.append(QString::fromStdString(SignerTagToStr(tag)));
    }
    return deviceExport(tags, signer.get_type());
}

QJsonObject QGroupDashboard::inviterInfo() const
{
    return m_inviterInfo;
}

void QGroupDashboard::setInviterInfo(const QJsonObject &inviterInfo)
{
    m_inviterInfo = inviterInfo;
    emit groupInfoChanged();
}

QVariant QGroupDashboard::myInfoModel() const
{
    return QVariant::fromValue(myInfo());
}

QString QGroupDashboard::role() const
{
    return myInfo()["role"].toString();
}

QString QGroupDashboard::walletunUsedAddress() const
{
    return m_walletunUsedAddress;
}

void QGroupDashboard::setWalletunUsedAddress(const QString &walletunUsedAddress)
{
    m_walletunUsedAddress = walletunUsedAddress;
    emit walletunUsedAddressChanged();
}

QString QGroupDashboard::configFlow() const
{
    return m_configFlow;
}

void QGroupDashboard::setConfigFlow(const QString &configFlow)
{
    m_configFlow = configFlow;
    emit configFlowChanged();
}

int QGroupDashboard::flow() const
{
    if (auto w = walletInfoPtr()) {
        return w->flow();
    }
    return 0;
}

void QGroupDashboard::setFlow(int flow)
{
    if (auto w = walletInfoPtr()) {
        w->setFlow(flow);
    }
}

bool QGroupDashboard::hasWallet() const
{
    return wallet_id() != "";
}

bool QGroupDashboard::showDashBoard() const
{
    if (qUtils::strCompare(myRole(), "KEYHOLDER_LIMITED")) {
        return true;
    }
    return m_showDashBoard;
}

void QGroupDashboard::setShowDashBoard(bool showDashBoard)
{
    if (m_showDashBoard == showDashBoard)
        return;
    m_showDashBoard = showDashBoard;
    emit showDashBoardChanged();
}

int QGroupDashboard::mInfo() const
{
    return m_mInfo;
}

int QGroupDashboard::nInfo() const
{
    return m_nInfo;
}

QJsonObject QGroupDashboard::GetDraftSigner(const QString &xfp) const
{
    QJsonArray signers = m_walletDraftInfo["signers"].toArray();
    for (QJsonValue js : signers) {
        QJsonObject signer = js.toObject();
        if (signer["xfp"].toString() == xfp) {
            return signer;
        }
    }
    return {};
}

QJsonObject QGroupDashboard::GetSigner(const QString &xfp) const
{
    QJsonArray origin = groupInfo()["members"].toArray();
    QString our_id = "";
    for (auto member : origin) {
        QJsonObject it = member.toObject();
        QString user_id = it["user"].toObject()["id"].toString();
        QString email_or_username = it["email_or_username"].toString();
        if (qUtils::strCompare(ClientController::instance()->getMe().email, email_or_username)) {
            our_id = user_id;
            break;
        }
    }
    for (QJsonValue js : m_signerInfo) {
        QJsonObject signer = js.toObject();
        if (signer["xfp"].toString() == xfp) {
            QJsonArray tags = signer["tags"].toArray();
            signer["tag"] = "";
            for (auto tag : tags) {
                signer["tag"] = tag.toString();
            }
            signer["account_index"] = qUtils::GetIndexFromPath(signer["derivation_path"].toString());
            signer["signer_type"] = (int)qUtils::GetSignerType(signer["type"].toString());
            auto key = AppModel::instance()->masterSignerListPtr()->getMasterSignerByXfp(xfp);
            if (!key.isNull()) {
                signer["signer_type"] = key->signerType();
            } else {
                auto key = AppModel::instance()->remoteSignerListPtr()->getSingleSignerByFingerPrint(xfp);
                if (!key.isNull()) {
                    signer["type"] = QString::fromStdString(SignerTypeToStr((nunchuk::SignerType)key->signerType()));
                    signer["signer_type"] = key->signerType();
                }
            }
            signer["signer_is_primary"] = qUtils::isPrimaryKey(xfp);
            QString added_by_user_id = signer["added_by_user_id"].toString();
            signer["ourAccount"] = false;
            if (our_id == added_by_user_id) {
                signer["ourAccount"] = true;
            }
            return signer;
        }
    }
    return {};
}

QVariant QGroupDashboard::health() const
{
    return QVariant::fromValue(healthPtr().data());
}

void QGroupDashboard::setAlertId(const QString &alertId)
{
    QJsonArray array = m_alertInfo["alerts"].toArray();
    for(auto js : array) {
        QJsonObject alert = js.toObject();
        if (alert["id"].toString() == alertId) {
            setAlertId(alert);
        }
    }
}

void QGroupDashboard::setAlertId(const QJsonObject &alert)
{
    QJsonObject payload = m_currentAlertInfo["payload"].toObject();
    QJsonObject new_payload = alert["payload"].toObject();
    for (auto it = new_payload.begin(); it != new_payload.end(); ++it) {
        payload[it.key()] = it.value();
    }
    QJsonObject tmp_alert = alert;
    tmp_alert["payload"] = payload;
    DBG_INFO << tmp_alert;
    m_currentAlertInfo = tmp_alert;
    emit alertInfoChanged();
}

void QGroupDashboard::setDummyTxAlert(const QJsonObject &dummy_transaction)
{
    QJsonObject alert = alertJson();
    QJsonObject payload = alert["payload"].toObject();
    payload["dummy_transaction_id"] = dummy_transaction["id"].toString();
    QJsonObject dummy_payload = dummy_transaction["payload"].toObject();
    for (auto it = dummy_payload.begin(); it != dummy_payload.end(); ++it) {
        payload[it.key()] = it.value();
    }
    payload["pending_signatures"] = dummy_transaction["pending_signatures"].toInt();
    payload["required_signatures"] = dummy_transaction["required_signatures"].toInt();
    alert["payload"] = payload;
    setAlertId(alert);
}

bool QGroupDashboard::CancelRecoveryKey()
{
    if (auto dummy = groupDummyTxPtr()) {
        return dummy->CancelDummyTransaction();
    }
    return false;
}

bool QGroupDashboard::CancelKeyReplacement()
{
    QJsonObject output;
    QString error_msg = "";
    QJsonObject payload = alertJson()["payload"].toObject();
    QString xfp = payload["xfp"].toString();
    bool can_cancel = payload["can_cancel"].toBool();
    if (can_cancel == false) return false;
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->CancelKeyReplacement(wallet_id(), xfp, output, error_msg);
    }
    else {
        ret = Byzantine::instance()->CancelKeyReplacement(groupId(), wallet_id(), xfp, output, error_msg);
    }
    DBG_INFO << ret << error_msg;
    if(ret){
        // Handle preparing model here.
    }
    else{
        //Show error
    }
    return ret;
}

bool QGroupDashboard::FinishKeyReplacement(const QJsonObject &requestBody)
{
    QString error_msg = "";
    QJsonObject payload = alertJson()["payload"].toObject();
    QString xfp = payload["xfp"].toString();
    QString verifyToken = servicesTagPtr()->passwordToken();
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->ReplaceKey(wallet_id(), xfp, verifyToken, requestBody, error_msg);
    }
    else {
        ret = Byzantine::instance()->ReplaceKey(groupId(), wallet_id(), xfp, verifyToken, requestBody, error_msg);
    }
    DBG_INFO << ret << error_msg << requestBody;
    if(ret){
        // Handle preparing model here.
        GetAlertsInfo();
        GetHealthCheckInfo();
    }
    else{
        //Show error
    }
    return ret;
}

bool QGroupDashboard::canReplaceKey()
{
    auto type = static_cast<AlertEnum::E_Alert_t>(alertJson()["type"].toInt());
    if (type == AlertEnum::E_Alert_t::KEY_REPLACEMENT_PENDING) {
        QJsonObject payload = alertJson()["payload"].toObject();
        return payload["can_replace"].toBool();
    }
    return false;
}

bool QGroupDashboard::isInheritance()
{
    auto type = static_cast<AlertEnum::E_Alert_t>(alertJson()["type"].toInt());
    if (type == AlertEnum::E_Alert_t::KEY_REPLACEMENT_PENDING) {
        QJsonObject payload = alertJson()["payload"].toObject();
        return payload["is_inheritance"].toBool();
    } else if (type == AlertEnum::E_Alert_t::WALLET_PENDING) {
        auto request = QGroupWallets::instance()->request();
        return request.mTags.contains("INHERITANCE");
    } else {
        auto request = DraftWallet()->request();
        return request.mTags.contains("INHERITANCE");
    }
}

QStringList QGroupDashboard::getNameSameTag(const QString &tag)
{
    QStringList ret;
    for (QJsonValue js : m_signerInfo) {
        QJsonObject signer = js.toObject();
        QJsonArray tagsJs = signer["tags"].toArray();
        if (tagsJs.contains(tag)) {
            ret.append(signer["name"].toString());
        }
    }
    return ret;
}

QString QGroupDashboard::createName(const QString &tag, int &index)
{
    QMap<QString, QString> maps;
    maps["COLDCARD"] = "Coldcard";
    maps["BITBOX"] = "BitBox";
    maps["LEDGER"] = "Ledger";
    maps["TREZOR"] = "Trezor";
    maps["JADE"] = "Jade";
    QStringList names = getNameSameTag(tag);
    QString name = maps[tag] + (index == 0 ? "" : QString("#%1").arg(index));
    if (names.contains(name)) {
        index++;
        name = createName(tag, index);
    }
    return name;
}

QString QGroupDashboard::generateName(const QStringList &tags)
{
    QStringList tagList = tags;
    tagList.removeOne("INHERITANCE");
    QString tag = tagList.first();
    int index = 0;
    return createName(tag, index);
}

QVariantList QGroupDashboard::keys() const
{
    return m_keys;
}

QVariantList QGroupDashboard::alerts() const
{
    return m_alertInfo["alerts"].toArray().toVariantList();
}

QVariant QGroupDashboard::alert() const
{
    return QVariant::fromValue(alertJson());
}

QJsonObject QGroupDashboard::alertJson() const
{
    return m_currentAlertInfo;
}

QVariantList QGroupDashboard::editMembers() const
{
    return m_editMembers.toVariantList();
}

void QGroupDashboard::initMembers()
{
    m_editMembers = groupInfo()["members"].toArray();
    emit editMembersChanged();
}

bool QGroupDashboard::addMember(const QJsonObject &aEditMember)
{
    QJsonObject obj = aEditMember;
    obj["isNew"] = true;
    DBG_INFO << obj;
    m_editMembers.append(obj);
    emit editMembersChanged();
    return true;
}

bool QGroupDashboard::removeMember(const QJsonObject &aEditMember)
{
    QString email_or_username = aEditMember["email_or_username"].toString();
    for (int i = 0; i < m_editMembers.size(); i++) {
        auto js = m_editMembers.at(i);
        auto email_or_username_tmp = js.toObject()["email_or_username"].toString();
        if (qUtils::strCompare(email_or_username, email_or_username_tmp)) {
            m_editMembers.removeAt(i);
            emit editMembersChanged();
            return true;
        }
    }
    return false;
}

bool QGroupDashboard::editMembers(const QJsonObject &aEditMember, int index)
{
    QString email_or_username = aEditMember["email_or_username"].toString();
    auto old = m_editMembers.at(index).toObject();
    QString email_or_username_old = old["email_or_username"].toString();
    DBG_INFO << aEditMember << index;
    QJsonObject replace = old;
    for (auto it = aEditMember.begin(); it != aEditMember.end(); ++it) {
        replace[it.key()] = aEditMember[it.key()];
    }
    if (qUtils::strCompare(email_or_username, email_or_username_old)) {
        m_editMembers.replace(index, replace);
        emit editMembersChanged();
        return true;
    } else {
        if (containEditMeber(replace)) {
            return false;
        } else {
            m_editMembers.replace(index, replace);
            emit editMembersChanged();
            return true;
        }
    }
}

bool QGroupDashboard::containEditMeber(const QJsonObject &aEditMember)
{
    QString email_or_username = aEditMember["email_or_username"].toString();
    for (auto js : m_editMembers) {
        auto email_or_username_tmp = js.toObject()["email_or_username"].toString();
        if (qUtils::strCompare(email_or_username, email_or_username_tmp)) {
            return true;
        }
    }
    return false;
}

bool QGroupDashboard::RequestConfirmationCodeEditMembers()
{
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = bodyEditMembers();
    servicesTagPtr()->setConfirmCodeNonceBody(data);
    QString errormsg;
    QJsonObject output;
    bool ret = Draco::instance()->RequestConfirmationCode("EDIT_GROUP_MEMBERS", data, output, errormsg);
    if (ret) {
        QString code_id = output["code_id"].toString();
        servicesTagPtr()->setCode_id(code_id);
    }
    return ret;
}

QJsonObject QGroupDashboard::bodyEditMembers()
{
    QJsonArray arrays;
    for (auto js : m_editMembers) {
        auto member = js.toObject();
        QJsonObject newMember;
        newMember["email_or_username"] = member["email_or_username"];
        newMember["role"] = member["role"];
        newMember["permissions"] = member["permissions"];
        arrays.append(newMember);
    }
    QJsonObject requestBody;
    requestBody["members"] = arrays;
    return requestBody;
}
