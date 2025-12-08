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
#include "Signers/QSignerManagement.h"

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
    : QBasePremium(wallet_id),
        m_timeLockUseCase(this)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    connect(CLIENT_INSTANCE, &ClientController::byzantineRoomCreated,   this, &QGroupDashboard::byzantineRoomCreated);
    connect(CLIENT_INSTANCE, &ClientController::byzantineRoomDeleted,   this, &QGroupDashboard::byzantineRoomDeleted);
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &QGroupDashboard::updateProgress);
    clearTimeLock();
}

QGroupDashboard::~QGroupDashboard()
{
}

GroupId QGroupDashboard::groupId() const
{
    GroupId group_id = WalletsMng->groupId(wallet_id());
    return group_id.isEmpty() ? groupInfo()["id"].toString() : group_id;
}

QString QGroupDashboard::walletType() const
{
    return m_walletInfo.isEmpty() ? m_walletDraftInfo["wallet_type"].toString() : m_walletInfo["wallet_type"].toString();
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

QJsonObject QGroupDashboard::walletDraftJson() const
{
    return m_walletDraftInfo;
}

void QGroupDashboard::GetMemberInfo()
{
    if (isUserWallet() || isReplaced() || isUserDraftWallet()) return;
    QPointer<QGroupDashboard> safeThis(this);
    runInThread([safeThis]() ->QJsonObject{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QJsonObject output;
        QString error_msg = "";
        Byzantine::instance()->GetOneGroupWallets(ptrLamda->groupId(), output, error_msg);
        return output["group"].toObject();
    },[safeThis](QJsonObject group) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        if(!group.isEmpty()) {
            ptrLamda->setGroupInfo(group);
        }
    });
}

void QGroupDashboard::GetAlertsInfo()
{
    if (isReplaced()) return;
    QPointer<QGroupDashboard> safeThis(this);
    runInThread([safeThis]() -> QJsonArray {
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QJsonObject output;
        QString error_msg = "";
        bool ret {false};
        if (ptrLamda->isUserDraftWallet()) {
            ret = Draco::instance()->DraftWalletGetAlerts(output, error_msg);
        }
        else if (ptrLamda->isUserWallet()) {
            ret = Draco::instance()->GetAlerts(ptrLamda->wallet_id(), output, error_msg);
        }
        else {
            ret = Byzantine::instance()->GetGroupAlerts(ptrLamda->groupId(), output, error_msg);
        }
        DBG_INFO << ret;
        return output["alerts"].toArray();
    }, [safeThis](QJsonArray alerts) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        QJsonArray v_alerts;
        for (auto js : alerts) {
            QJsonObject alert = js.toObject();
            long int created_time_millis = static_cast<long int>(alert.value("created_time_millis").toDouble() / 1000);
            QDateTime date_time = QDateTime::fromTime_t(created_time_millis);
            alert["created_time_millis"] = QString("%1 at %2")
                                               .arg(date_time.date().toString("MM/dd/yyyy"))
                                               .arg(date_time.time().toString("hh:mm AP"));
            QString type = alert["type"].toString();
            alert["type"] = StringToInt(type);
            v_alerts.append(alert);
        }
        // Optional hard-coded alert (fixed syntax)
        QJsonObject alertHardCode;
        alertHardCode["body"] = "IMPORTANT: You might need to register the wallet with your device.";
        alertHardCode["created_time_millis"] = "11/15/2025 at 09:59 PM";
        alertHardCode["id"] = "644914284035313664";
        QJsonObject payload;
        payload["claim_key"] = false;
        payload["group_id"] = "644817879908225024";
        QJsonArray register_key_xfps;
        register_key_xfps.append("25c523fb");
        register_key_xfps.append("c0452228");
        payload["register_key_xfps"] = register_key_xfps;
        payload["wallet_id"] = "644914281829109760";
        payload["wallet_local_id"] = "5ut70w3v";
        alertHardCode["payload"] = payload;
        alertHardCode["status"] = "READ";
        alertHardCode["title"] = "A group wallet has been set up";
        alertHardCode["type"] = 1;
        alertHardCode["viewable"] = true;

        // Append the hard-coded alert if needed
        // v_alerts.append(alertHardCode);

        ptrLamda->m_alertInfo["alerts"] = v_alerts;
        emit ptrLamda->alertInfoChanged();
        DBG_INFO << ptrLamda->m_alertInfo;
        ptrLamda->setAlertId(ptrLamda->alertId());
    });
}

bool QGroupDashboard::MarkAlertAsRead(const QString &alert_id)
{
    DBG_INFO << alert_id;
    if (alert_id.isEmpty()) return false;
    QPointer<QGroupDashboard> safeThis(this);
    runInConcurrent([safeThis, alert_id]() ->bool{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QJsonObject output;
        QString error_msg = "";
        bool ret {false};
        if (ptrLamda->isUserDraftWallet()) {
            ret = Draco::instance()->DraftWalletMarkAnAlertAsRead(alert_id, output, error_msg);
        }
        else if (ptrLamda->isUserWallet()) {
            ret = Draco::instance()->MarkAlertAsRead(ptrLamda->wallet_id(), alert_id, output, error_msg);
        }
        else {
            ret = Byzantine::instance()->MarkGroupAlertAsRead(ptrLamda->groupId(), alert_id, output, error_msg);
        }
        DBG_INFO << ret << error_msg;
        return ret;
    },[safeThis](bool ret) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        if(ret) {
            ptrLamda->GetAlertsInfo();
        }
    });
    return true;
}

bool QGroupDashboard::DismissAlert(const QString &alert_id)
{
    QPointer<QGroupDashboard> safeThis(this);
    runInConcurrent([safeThis, alert_id]() ->bool{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QJsonObject output;
        QString error_msg = "";
        bool ret {false};
        if (ptrLamda->isUserDraftWallet()) {
            ret = Draco::instance()->DraftWalletDismissAnAlert(alert_id, output, error_msg);
        }
        else if (ptrLamda->isUserWallet()) {
            ret = Draco::instance()->DismissAlert(ptrLamda->wallet_id(), alert_id, output, error_msg);
        }
        else {
            ret = Byzantine::instance()->DismissGroupAlert(ptrLamda->groupId(), alert_id, output, error_msg);
        }
        DBG_INFO << ret << error_msg;
        return ret;
    },[safeThis](bool ret) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        if(ret) {
            ptrLamda->GetAlertsInfo();
        }
    });
    return false;
}

bool QGroupDashboard::dismissAlert()
{
    return DismissAlert(alertId());
}

void QGroupDashboard::GetWalletInfo()
{
    QPointer<QGroupDashboard> safeThis(this);
    runInThread([safeThis]() ->QJsonObject{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QJsonObject output;
        QString error_msg = "";
        if (ptrLamda->isUserDraftWallet()) {

        } else if (ptrLamda->isUserWallet()) {
            Draco::instance()->assistedWalletGetInfo(ptrLamda->wallet_id(), output, error_msg);
        } else {
            Byzantine::instance()->GetCurrentGroupWallet(ptrLamda->groupId(), output, error_msg);
        }
        return output.value("wallet").toObject();
    },[safeThis](QJsonObject wallet) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        if (!wallet.isEmpty()) {
            ptrLamda->m_walletInfo = wallet;
            DBG_INFO << wallet;
            QJsonArray signers = wallet["signers"].toArray();
            ptrLamda->m_signerInfo = signers;
            ptrLamda->checkInheritanceWallet();
            emit ptrLamda->groupInfoChanged();
            if (ptrLamda->walletInfoPtr() && ptrLamda->walletInfoPtr()->serverKeyPtr()) {
                ptrLamda->walletInfoPtr()->serverKeyPtr()->UpdateFromWallet(wallet);
            }
            ptrLamda->clearTimeLock();
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
    runInThread([safeThis]() ->QJsonObject{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QJsonObject output;
        QString error_msg = "";
        bool ret {false};
        if (ptrLamda->isUserDraftWallet()) {
            ret = Draco::instance()->DraftWalletGetCurrent(output, error_msg);
        } else if (!ptrLamda->isUserWallet()){
            ret = Byzantine::instance()->GetCurrentGroupDraftWallet(ptrLamda->groupId(), output, error_msg);
        }
        return output["draft_wallet"].toObject();
    },[safeThis](QJsonObject draft_wallet) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        if (!draft_wallet.isEmpty()) {
            DBG_INFO << draft_wallet;
            QJsonArray signers = draft_wallet["signers"].toArray();
            ptrLamda->m_signerInfo = signers;
            ptrLamda->m_walletDraftInfo = draft_wallet;
            ptrLamda->UpdateKeys(draft_wallet);
            ptrLamda->clearTimeLock();
        }
    });
}

void QGroupDashboard::GetHealthCheckInfo()
{
    if (!healthPtr() || wallet_id().isEmpty() || isReplaced()) return;
    healthPtr()->GetKeyHealthReminder();
    healthPtr()->GetStatuses();
}

QString QGroupDashboard::getOurId() const {
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
    return our_id;
}

const QMap<int, QList<int>> mapIndexMultiSig = {
    {0, {0}},
    {1, {1}},
    {2, {2}},
    {3, {3}},
    {4, {4}},
    {5, {5}},
    {6, {6}}
};

const QMap<int, QList<int>> mapIndexMiniscript = {
    {0, {0*2, 0*2 + 1}},
    {1, {1*2, 1*2 + 1}},
    {2, {2*2, 2*2 + 1}},
    {3, {3*2, 3*2 + 1}},
    {4, {4*2, 4*2 + 1}},
    {5, {5*2, 5*2 + 1}},
    {6, {6*2, 6*2 + 1}}
};

const QMap<QString, int> mapTagkeys = {
    {"LEDGER",   (int)Key::ADD_LEDGER},
    {"TREZOR",   (int)Key::ADD_TREZOR},
    {"COLDCARD", (int)Key::ADD_COLDCARD},
    {"BITBOX",   (int)Key::ADD_BITBOX},
    {"JADE",     (int)Key::ADD_JADE},
};

QJsonObject QGroupDashboard::createOrUpdateSignerInfo(const QJsonObject &info, int index) {
    QString our_id = getOurId();
    QJsonObject signer = info;
    QString wallet_type = walletType();
    QString xfp = signer.value("xfp").toString();
    if (!xfp.isEmpty()) {
        signer["ourAccount"] = false;
        signer["signer_type"] = (int)qUtils::GetSignerType(signer.value("type").toString());
        QJsonObject tapsignerObj = signer.value("tapsigner").toObject();
        signer["card_id"] = tapsignerObj.isEmpty() ? "" : tapsignerObj.value("card_id").toString();
        QString added_by_user_id = signer.value("added_by_user_id").toString();
        if (our_id == added_by_user_id) {
            signer["ourAccount"] = true;
            WalletsMng->UpdateSigner(signer);
        }

        QJsonArray tags = signer.value("tags").toArray();
        signer["is_inheritance"] = tags.contains("INHERITANCE");
        for (auto tag : tags) {
            if (tag.toString() != "INHERITANCE") {
                signer["tag"] = tag.toString();
                signer["hwType"] = mapTagkeys.value(tag.toString(), -1);
            }
        }
    } else {
        signer["is_inheritance"] = false;
    }
    
    bool has = signer.value("has").toBool();
    if (!has) {
        signer["new_account_index"] = 0;
    } else {
        signer["new_account_index"] = 1;
    }    
    return signer;
}

QJsonObject QGroupDashboard::CreateAccountIndexAndKeyIndices(const QJsonArray &signers, int idx) {
    std::function<QJsonObject(const int, const QJsonArray&)> findSigner = [] (const int key_index, const QJsonArray& signers_tmp) {
        for (const auto& js : signers_tmp) {
            QJsonObject signer = js.toObject();
            int k_index = signer.value("key_index").toInt();
            QString xfp = signer.value("xfp").toString();
            if (k_index == key_index && !xfp.isEmpty()) {
                return signer;
            }
        }
        return QJsonObject();
    };
    QString wallet_type = walletType();
    if (wallet_type == "MULTI_SIG") {
        // For MULTI_SIG, just map index directly
        QList<int> indices = mapIndexMultiSig.value(idx);
        int key_index = indices.value(0, -1);
        QJsonObject signer = findSigner(key_index, signers);
        if (signer.isEmpty() ) {
            signer["key_index"] = key_index;
            signer["account_indexs"] = {};
            signer["account_index"] = 0;
            signer["has"] = false;
        } else {
            QString xfp = signer.value("xfp").toString();
            int index = qUtils::GetIndexFromPath(signer.value("derivation_path").toString());
            QList<QString> accountIndexs = {QString::number(index)};
            signer["account_indexs"] = QJsonArray::fromStringList(accountIndexs);
            signer["account_index"] = index;
            signer["has"] = true;
        }
        signer["key_indices"] = QJsonArray::fromVariantList({key_index});
        signer["wallet_type"] = wallet_type;
        signer["group_id"] = groupId();
        return signer;
    } else if (wallet_type == "MINISCRIPT") {
        // For MINISCRIPT, each logical index maps to two key indices
        QList<int> indices = mapIndexMiniscript.value(idx);
        const int key_index_first  = indices.value(0, -1);
        const int key_index_second = indices.value(1, -1);

        QJsonObject signer_first  = findSigner(key_index_first,  signers);
        QJsonObject signer_second = findSigner(key_index_second, signers);
        QJsonObject ret;

        if (signer_first.isEmpty() && signer_second.isEmpty()) {
            // No keys yet: mark both slots as placeholders X/Y
            ret["key_index"] = key_index_first;
            ret["account_indexs"] = QJsonArray::fromStringList({"X", "Y"});
            ret["account_index"] = 0;
        } else if (!signer_first.isEmpty() && !signer_second.isEmpty()) {
            // Both keys present: collect both indices
            const int index_first  = qUtils::GetIndexFromPath(signer_first.value("derivation_path").toString());
            const int index_second = qUtils::GetIndexFromPath(signer_second.value("derivation_path").toString());
            QList<QString> accountIndexs{QString::number(index_first), QString::number(index_second)};
            ret = signer_first;
            ret["account_indexs"] = QJsonArray::fromStringList(accountIndexs);
            // No single "account_index" is primary in this combined case
            ret["account_index"] = {};
            // Both present; no "next" key_index to set
            ret["key_index"] = key_index_second;
        } else if (!signer_first.isEmpty()) {
            // Only first present: second missing
            const int index_first = qUtils::GetIndexFromPath(signer_first.value("derivation_path").toString());
            QList<QString> accountIndexs{QString::number(index_first), "Y"};
            ret = signer_first;
            ret["account_indexs"] = QJsonArray::fromStringList(accountIndexs);
            // Next expected key_index should be the second slot
            ret["key_index"] = key_index_second;
            // Indicate next expected slot (second)
            ret["account_index"] = 1;
        } else { // only signer_second present
            const int index_second = qUtils::GetIndexFromPath(signer_second.value("derivation_path").toString());
            QList<QString> accountIndexs{"X", QString::number(index_second)};
            ret = signer_second;
            ret["account_indexs"] = QJsonArray::fromStringList(accountIndexs);
            // Next expected key_index should be the first slot
            ret["key_index"] = key_index_first;
            // Indicate current filled slot (first missing -> 0)
            ret["account_index"] = 0;
        }

        // Always provide both key indices for the logical slot
        ret["key_indices"] = QJsonArray::fromVariantList({key_index_first, key_index_second});

        // hasSecond denotes whether the second slot is already present
        const QJsonArray ret_accountIndexs = ret.value("account_indexs").toArray();
        if (!ret_accountIndexs.isEmpty()) {
            ret["has"] = !ret_accountIndexs.contains("X");
            ret["hasSecond"] = !ret_accountIndexs.contains("Y");
        }
        ret["wallet_type"] = wallet_type;
        ret["group_id"] = groupId();
        return ret;
    }
    return QJsonObject();
}

void QGroupDashboard::UpdateKeys(const QJsonObject &data)
{
    QJsonObject wallet_config = data["wallet_config"].toObject();
    QString wallet_type = data["wallet_type"].toString();
    bool allow_inheritance = wallet_config["allow_inheritance"].toBool();
    int n = m_nInfo = wallet_config["n"].toInt();
    int m = m_mInfo = wallet_config["m"].toInt();
    DBG_INFO << data;
    m_allow_inheritance = allow_inheritance;
    bool required_server_key = wallet_config["required_server_key"].toBool();

    QJsonArray signers = data["signers"].toArray();
    QString our_id = getOurId();
    QJsonArray signers_tmp = signers;
    signers = {};
    for(int i = 0; i < n; i++) {
        QJsonObject signer = CreateAccountIndexAndKeyIndices(signers_tmp, i);
        auto tmp = createOrUpdateSignerInfo(signer, i);
        signers.append(tmp);
    }
    emit WalletsMng->signalUpdateSigner();
    
    QList<QJsonObject> tmp_keys;

    bool is_wallet_2_of_4 = (m == 2 && n == 4);
    bool is_wallet_3_of_5 = (m == 3 && n == 5);
    int  last_index = required_server_key ? n - 1 : n;
    bool is_premier = CLIENT_INSTANCE->isByzantinePremier() || isPremierGroup();
    bool hasServer = !CLIENT_INSTANCE->isFinney();
    DBG_INFO << is_premier << allow_inheritance << m_mInfo << m_nInfo;
    // Improved logic for assigning inheritance and server key properties
    for (int i = 0; i < signers.size(); i++) {
        QJsonObject obj = signers.at(i).toObject();
        // Assign inheritance keys based on wallet type and conditions
        if (is_wallet_2_of_4 && i == 0 && allow_inheritance && !is_premier) {
            obj["is_inheritance"] = true;
        } else if (is_wallet_3_of_5 && (i == 0 || i == 1) && allow_inheritance && !is_premier) {
            obj["is_inheritance"] = true;
        }

        // Assign server key properties if required
        if (i == last_index && required_server_key && hasServer) {
            bool has = obj["has"].toBool();
            if (!has) {
                obj["type"] = "SERVER";
                obj["has"]  = false;
            }
        }

        DBG_INFO << obj;
        tmp_keys.insert(i, obj);
    }
    QJsonArray array;
    for (const QJsonObject &obj : tmp_keys) {
        array.append(obj);
    }
    m_keys = array;
    emit draftWalletChanged();

    ServiceSetting::instance()->CreateAssignAvailableSigners(wallet_type);
}

void QGroupDashboard::UpdateReplacementKeys(const QJsonObject &data)
{
    QJsonObject wallet_config = data["wallet_config"].toObject();
    QString wallet_type = data["wallet_type"].toString();
    bool allow_inheritance = wallet_config["allow_inheritance"].toBool();
    int n = m_nInfo = wallet_config["n"].toInt();
    int m = m_mInfo = wallet_config["m"].toInt();
    DBG_INFO << data;
    m_allow_inheritance = allow_inheritance;
    bool required_server_key = wallet_config["required_server_key"].toBool();

    QJsonArray signers = data["signers"].toArray();
    QString our_id = getOurId();
    QJsonArray signers_tmp = signers;
    signers = {};
    for(int i = 0; i < n; i++) {
        QJsonObject signer = CreateAccountIndexAndKeyIndices(signers_tmp, i);
        QString xfp = signer.value("xfp").toString();
        QJsonObject replacementSigner = GetSigerReplacement(xfp);
        if (!replacementSigner.isEmpty()) {
            QJsonArray replacements = replacementSigner["replacements"].toArray();
            QJsonObject new_signer = CreateAccountIndexAndKeyIndices(replacements, i);
            new_signer["replacements"] = replacements;
            auto tmp = createOrUpdateSignerInfo(new_signer, i);
            tmp["old_singer"] = signer;
            signers.append(tmp);
        } else {
            auto tmp = createOrUpdateSignerInfo(signer, i);
            tmp["replacements"] = QJsonArray();
            QJsonObject sub_singer = CreateAccountIndexAndKeyIndices(QJsonArray(), i);
            tmp["sub_singer"] = sub_singer;
            signers.append(tmp);
        }
    }
    m_replaceKeys = signers;
    emit replacementKeysChanged();
    DBG_INFO << m_replaceKeys;
    ServiceSetting::instance()->CreateAssignAvailableSigners(wallet_type);
}

bool QGroupDashboard::GetKeyReplacementStatus()
{
    QJsonObject output;
    QString errormsg;
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->GetKeyReplacementStatus(wallet_id(), output, errormsg);
    }
    else {
        ret = Byzantine::instance()->GetKeyReplacementStatus(groupId(), wallet_id(), output, errormsg);
    }
    if (ret) {
        DBG_INFO << output;
        m_keyReplacementInfo  = output["status"].toObject();
        auto timelock = m_keyReplacementInfo["timelock"].toObject();
        if (!timelock.isEmpty()) {
            setTimelockReplacement(timelock);
        } else {
            setTimelockReplacement(m_walletInfo.value("timelock").toObject());
            clearTimeLock();
        }
        
        preparingKeyTobeReplaced();
    }
    return ret;
}

void QGroupDashboard::CorrectCurrentSignerInfoForReplacementKey(const QString &xfp) {
    int index_of_xfp = -1;;
    for (int i = 0; i < m_replaceKeys.size(); i++) {
        QJsonObject signer = m_replaceKeys.at(i).toObject();
        QJsonArray replacements = signer["replacements"].toArray();
        QJsonObject old_singer = signer["old_singer"].toObject();
        if (replacements.size() == 0) {
            if (qUtils::strCompare(signer["xfp"].toString(), xfp)) {
                index_of_xfp = i;
                break; 
            }
        } else {
            if (qUtils::strCompare(old_singer["xfp"].toString(), xfp)) {
                index_of_xfp = i;
                break; 
            }
        }
    }
    DBG_INFO << index_of_xfp;
    startReplaceKeyAtIndex(index_of_xfp);
}

QJsonObject QGroupDashboard::GetSigerReplacement(const QString &xfp)
{
    auto signers = m_keyReplacementInfo["signers"].toArray();
    for (auto js : signers) {
        QJsonObject signer = js.toObject();
        if (qUtils::strCompare(signer["xfp"].toString(), xfp)) {
            return signer;
        }
    }
    return QJsonObject();
}

QMap<QString, QVariant> QGroupDashboard::requestBodyUploadBackupFile(const QString &xfp, const QString &filePath)
{
    QString file_path = qUtils::QGetFilePath(filePath);
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
    QMap<QString, QVariant> filteredMap;
    for (auto it = body.constBegin(); it != body.constEnd(); ++it) {
        if (it.value().isValid() && !it.value().toString().isEmpty()) {
            filteredMap.insert(it.key(), it.value());
        }
    }
    return filteredMap;
}

bool QGroupDashboard::ReplacementUploadBackupFile(const QString &xfp, const QString &filePath)
{
    if (mTimer != nullptr) {
        AppModel::instance()->setAddSignerPercentage(1);
        mTimer->start(100);
    }
    DBG_INFO << xfp << filePath;
    QJsonObject output;
    QString error_msg = "";
    QMap<QString, QVariant> body = requestBodyUploadBackupFile(xfp, filePath);
    DBG_INFO << wallet_id() << servicesTagPtr()->passwordToken() << body;
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
    DBG_INFO << xfp << filePath;
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
            dismissAlert();
        }
        else if (register_key_xfps.size() == 0) {
            dismissAlert();
        }
        return ret;
    }
    return false;
}

bool QGroupDashboard::registerKeyNext()
{
    QWalletPtr w = walletInfoPtr();
    if (w.isNull()) return false;
    nunchuk::WalletType wallet_type = w->nunchukWallet().get_wallet_type();
    QJsonObject payload = alertJson()["payload"].toObject();
    QStringList register_key_xfps = payload["register_key_xfps"].toVariant().toStringList();
    QStringList::iterator not_register = std::find_if_not(register_key_xfps.begin(), register_key_xfps.end(), [this](QString xfp) {return m_registered_key_xfps.contains(xfp);});
    if (not_register != register_key_xfps.end()) {
        m_registered_key_xfps.append(*not_register);
        if (wallet_type == nunchuk::WalletType::MINISCRIPT) {
            setConfigFlow("register-wallet-hardware");
            registerKeyDone();
            DBG_INFO << "GROUP_WALLET_SETUP " << "register-wallet-hardware" ;
            return true;
        } else {
            return xfpExport(*not_register);
        }
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
        if (hasWallet()) {
            GetWalletInfo();
        }
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
    case AlertEnum::E_Alert_t::CHANGE_TIMELOCK_TYPE:
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
    case AlertEnum::E_Alert_t::BACKUP_WALLET:
    case AlertEnum::E_Alert_t::HEALTH_CHECK_REQUEST:{
        if (hasWallet()) {
            GetWalletInfo();
        }
        return true;
    }
    case AlertEnum::E_Alert_t::TRANSACTION_SIGNATURE_REQUEST:{
        return true;
    }    
    case AlertEnum::E_Alert_t::KEY_REPLACEMENT_PENDING: {
        auto signer = GetSigner(xfp);
        payload["keyname"] = signer["name"].toString();
        QJsonArray tags = signer["tags"].toArray();
        bool is_inheritance = signer["tapsigner"].toObject()["is_inheritance"].toBool() || tags.contains("INHERITANCE");
        payload["is_inheritance"] = is_inheritance;
        payload["key_index"] = signer["key_index"].toInt();
        auto alert = alertJson();
        alert["payload"] = payload;
        setAlertId(alert);        
        GetKeyReplacementStatus();
        CorrectCurrentSignerInfoForReplacementKey(xfp);
        return true;
    }
    case AlertEnum::E_Alert_t::KEY_TIMELOCK_UPDATE_PENDING:{
        auto alert = alertJson();
        setAlertId(alert);
        if (canReplaceKey()) {
            QMap<QString, QVariant> data;
            data["state_id"] = E::STATE_ID_SCR_DASHBOARD_ALERT_INFO;
            data["wallet_id"] = wallet_id();
            QEventProcessor::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_REQUEST, data);
        }
        GetKeyReplacementStatus();
        return false;
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
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
        }
    }
    return ret;
}

bool QGroupDashboard::requestStartKeyReplacement(const QString &tag)
{
    return QSignerManagement::instance()->requestStartKeyReplacement(tag, registerAddKey);
}

bool QGroupDashboard::requestStartKeyCreate(const QString &tag, bool isFirst)
{
    DBG_INFO << "isFirst:" << isFirst << "tag:" << tag;
    if (!isFirst) {
        QSignerManagement::instance()->requestStartKeyCreate(tag, registerAddKey);
    } else {
        QSignerManagement::instance()->clearState();
        QSignerManagement::instance()->clearExecute();
        registerAddKey();
        qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
        runInConcurrent([]() ->bool{
            auto draftWallet = QAssistedDraftWallets::IsByzantine() ? dynamic_cast<QAssistedDraftWallets*>(QGroupWallets::instance()) : dynamic_cast<QAssistedDraftWallets*>(QUserWallets::instance());
            auto currentSignerInfo = QSignerManagement::instance()->currentSignerJs();
            QString xfp = currentSignerInfo.value("xfp").toString();
            QString account1Path = draftWallet->bip32path(xfp, 1);
            draftWallet->newAccountIndexCached(xfp, 1);
            if (!account1Path.isEmpty()) {                    
                bool ret = draftWallet->requestAddOrReplacementWithIndex(xfp, 1);
                QEventProcessor::instance()->sendEvent(E::EVT_ADD_HARDWARE_KEY_REQUEST);
            } else {
                draftWallet->resultAddOrUpdateAKeyToDraftWallet("eSCREEN_REFRESH_DEVICE", true);
                QEventProcessor::instance()->sendEvent(E::EVT_ADD_HARDWARE_KEY_REQUEST);
            }
            return true;
        },[](bool ret) {
            qApp->restoreOverrideCursor();
        });
    }
    return true;
}

void registerAddKey() {
    auto masterFunc = []()->bool {
        auto draftWallet = QAssistedDraftWallets::IsByzantine()
                       ? dynamic_cast<QAssistedDraftWallets*>(QGroupWallets::instance())
                       : dynamic_cast<QAssistedDraftWallets*>(QUserWallets::instance());
        if (!draftWallet) {
            return false;
        }

        QJsonObject currentSignerInfo = QSignerManagement::instance()->currentSignerJs();
        const QString xfp = currentSignerInfo.value("xfp").toString();
        if (xfp.isEmpty()) {
            return false;
        }

        // Normal case: add/reuse at computed index
        return draftWallet->requestAddOrReplacementBothIndicesIfPossible(xfp);
    };

    auto masterFuncClaimInheritance = []()->bool {
        QJsonObject currentSignerInfo = QSignerManagement::instance()->currentSignerJs();
        const QString xfp = currentSignerInfo.value("xfp").toString();
        if (xfp.isEmpty()) {
            return false;
        }

        return ServiceSetting::instance()->servicesTagPtr()->requestDownloadWalletWithIndexAsync(xfp);
    };
    bool onlyUseForClaim = QSignerManagement::instance()->currentSignerJs().value("onlyUseForClaim").toBool();
    DBG_INFO << "onlyUseForClaim:" << onlyUseForClaim;
    QSignerManagement::instance()->registerCreateMasterSigner(onlyUseForClaim ? masterFuncClaimInheritance : masterFunc);
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
    if (m_keyReplacementInfo.isEmpty()) {
        setFlow((int)AlertEnum::E_Alert_t::WALLET_PENDING);
    } else {
        setFlow((int)AlertEnum::E_Alert_t::KEY_TIMELOCK_UPDATE_PENDING);
    }
    DBG_INFO << "requestShowLetAddYourKeys: " << m_keyReplacementInfo.isEmpty() << flow();
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

void QGroupDashboard::requestBackupColdcard(QVariant msg)
{
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << maps;
    if (type == "import-encrypted-backup") {
        QString currentFile = maps["currentFile"].toString();
        QString selectXfp = maps["fingerPrint"].toString();
        if (selectXfp.isEmpty()) {
            QJsonObject currentSignerInfo = QSignerManagement::instance()->currentSignerJs();
            selectXfp = currentSignerInfo.value("xfp").toString();
        }
        if (canReplaceKey()) {
            ReplacementUploadBackupFile(selectXfp, currentFile);
        } else {
            DraftWalletUploadBackupFile(selectXfp, currentFile);
        }
    } else if (type == "open-import-encrypted-backup") {
        AppModel::instance()->setAddSignerPercentage(0);
    }
}


void QGroupDashboard::startAddKeyAtIndex(int index) {
    if (index < 0 || index >= m_keys.size()) return;
    QJsonObject key = m_keys.at(index).toObject();
    QSignerManagement::instance()->setCurrentSigner(key);
}

void QGroupDashboard::startReplaceKeyAtIndex(int index) {
    if (index < 0 || index >= m_replaceKeys.size()) return;
    
    QJsonObject key = m_replaceKeys.at(index).toObject();
    QJsonArray replacements = key["replacements"].toArray();
    if (replacements.size() > 0) {
        QSignerManagement::instance()->setCurrentSigner(key);
        QJsonObject old_singer = key["old_singer"].toObject();
        QJsonObject  payload;
        payload["xfp"] = old_singer["xfp"].toString();
        auto alert = alertJson();
        alert["payload"] = payload;
        setAlertId(alert);
    } else {
        QJsonObject sub_singer = key["sub_singer"].toObject();
        sub_singer["notAllowDisplayKeySameXfp"] = true;
        sub_singer["xfp"] = key["xfp"].toString();
        QSignerManagement::instance()->setCurrentSigner(sub_singer); 
        QJsonObject  payload;
        payload["xfp"] = key["xfp"].toString();
        auto alert = alertJson();
        alert["payload"] = payload;
        setAlertId(alert);
    }    
}

bool QGroupDashboard::deviceExport(const QStringList tags, nunchuk::SignerType type)
{
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
    QWalletPtr w = walletInfoPtr();
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

bool QGroupDashboard::showDashBoard()
{
    if (qUtils::strCompare(myRole(), "KEYHOLDER_LIMITED")) {
        m_showDashBoard = true;
    }
    if (myRole().isEmpty()) {
        auto wallet = AppModel::instance()->walletInfoPtr();
        if (wallet && qUtils::strCompare(wallet->myRole(), "KEYHOLDER_LIMITED")) {
            m_showDashBoard = true;
        }
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
    QJsonObject output{};
    QJsonObject payload = alertJson()["payload"].toObject();
    QString xfp = payload["xfp"].toString();
    QString verifyToken = servicesTagPtr()->passwordToken();
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->ReplaceKey(wallet_id(), xfp, verifyToken, requestBody, output);
    }
    else {
        ret = Byzantine::instance()->ReplaceKey(groupId(), wallet_id(), xfp, verifyToken, requestBody, output);
    }
    DBG_INFO << "requestBody:" << requestBody << "output:" << output << ret;
    if(ret){
        // Handle preparing model here.
        GetAlertsInfo();
        GetHealthCheckInfo();
        GetKeyReplacementStatus();
    }
    else{
        //Show error
        QString error_msg = output.value("message").toString();
        AppModel::instance()->showToast(-1, error_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

bool QGroupDashboard::canReplaceKey()
{
    auto type = static_cast<AlertEnum::E_Alert_t>(alertJson()["type"].toInt());
    if (type == AlertEnum::E_Alert_t::KEY_REPLACEMENT_PENDING) {
        QJsonObject payload = alertJson()["payload"].toObject();
        return payload["can_replace"].toBool();
    } else if (type == AlertEnum::E_Alert_t::KEY_TIMELOCK_UPDATE_PENDING) {
        return true;
    }
    return false;
}

bool QGroupDashboard::isInheritance()
{
    auto type = static_cast<AlertEnum::E_Alert_t>(alertJson()["type"].toInt());
    if (type == AlertEnum::E_Alert_t::KEY_REPLACEMENT_PENDING ||
        type == AlertEnum::E_Alert_t::KEY_TIMELOCK_UPDATE_PENDING) {
        QJsonObject payload = alertJson()["payload"].toObject();
        return payload["is_inheritance"].toBool();
    } else {
         auto signer = QSignerManagement::instance()->currentSignerJs();
        return signer["is_inheritance"].toBool();
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
    maps["COLDCARD"] = "COLDCARD";
    maps["BITBOX"] = "BitBox";
    maps["LEDGER"] = "Ledger";
    maps["TREZOR"] = "Trezor";
    maps["JADE"] = "Jade";
    QStringList names = getNameSameTag(tag);
    QString name = maps[tag] + (index == 0 ? "" : QString(" #%1").arg(index + 1));
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
    return m_keys.toVariantList();
}

QVariantList QGroupDashboard::replaceKeys() const
{
    return m_replaceKeys.toVariantList();
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

QString QGroupDashboard::timeLock() const {
    auto info = m_walletInfo.isEmpty() ? m_walletDraftInfo : m_walletInfo;
    QJsonObject timelock = info.value("timelock").toObject();
    qint64 timestamp = timelock.value("value").toVariant().toULongLong();
    QString timezone = timelock.value("timezone").toString();
    QDateTime dt = qUtils::convertTimestampToDateTime(timestamp, timezone);
    QString formatted = dt.toString("MM/dd/yyyy HH:mm");
    DBG_INFO << formatted << timezone;
    return timestamp == 0 ? "" : formatted;
}

void QGroupDashboard::clearTimeLock() {
    auto info = m_walletInfo.isEmpty() ? m_walletDraftInfo : m_walletInfo;
    QJsonObject timelock = info.value("timelock").toObject();
    DBG_INFO << timelock;
    if (timelock.isEmpty()) {
        auto wallet = walletInfoPtr();
        if (!wallet) return;
        QDateTime dt = QDateTime::currentDateTime();
        dt = dt.addYears(5);
        QString formattedDate = dt.toString("MM/dd/yyyy");
        QString formattedTime = dt.toString("HH:mm");
        QJsonObject formatted;
        formatted["valueDate"] = formattedDate;
        formatted["valueTime"] = formattedTime;
        formatted["valueTimezone"] = wallet->timezones()->selectedTimezone();
        m_timeLockSet = formatted;
        emit timeLockSetChanged();
    } else {
        auto wallet = walletInfoPtr();
        if (!wallet) return;
        qint64 timestamp = timelock.value("value").toVariant().toULongLong();
        QString timezone = timelock.value("timezone").toString();
        QDateTime dt = qUtils::convertTimestampToDateTime(timestamp, timezone);
        wallet->timezones()->setSelectedTimezone(timezone);
        QString formattedDate = dt.date().toString("MM/dd/yyyy");
        QString formattedTime = dt.time().toString("HH:mm");
        QJsonObject formatted;
        formatted["valueDate"] = formattedDate;
        formatted["valueTime"] = formattedTime;
        formatted["valueTimezone"] = timezone;
        m_timeLockSet = formatted;
        emit timeLockSetChanged();
    }
}

QVariant QGroupDashboard::timeLockSet() const {    
    return QVariant::fromValue(m_timeLockSet);
}

QString QGroupDashboard::timelockReplacementDisp() const
{
    QJsonObject timelock = m_timeLockReplacement;
    qint64 timestamp = timelock.value("value").toVariant().toULongLong();
    QString timezone = timelock.value("timezone").toString();
    QDateTime dt = qUtils::convertTimestampToDateTime(timestamp, timezone);
    QString formatted = dt.toString("MM/dd/yyyy HH:mm");
    DBG_INFO << formatted << timezone;
    return timestamp == 0 ? "" : formatted;
}

void QGroupDashboard::setTimelockReplacement(const QJsonObject &timelock)
{
    auto wallet = walletInfoPtr();
    if (!wallet) return;
    m_timeLockReplacement = timelock;
    DBG_INFO << m_timeLockReplacement;
    qint64  timestamp = m_timeLockReplacement.value("value").toVariant().toULongLong();
    QString timezone = m_timeLockReplacement.value("timezone").toString();
    QDateTime dt = qUtils::convertTimestampToDateTime(timestamp, timezone);
    m_timeLockReplacement["valueDate"] = dt.toString("MM/dd/yyyy");
    m_timeLockReplacement["valueTime"] = dt.toString("HH:mm");
    m_timeLockReplacement["valueTimezone"] = timezone;
    wallet->timezones()->setSelectedTimezone(timezone);
    emit timelockReplacementChanged();
}

QVariant QGroupDashboard::timelockReplacement() const
{
    return QVariant::fromValue(m_timeLockReplacement);
}

void QGroupDashboard::draftWalletSetupTimeLock(const QVariant &datetime, bool isPutServer) {
    if (isPutServer) {
        QString date = m_timeLockSet.value("valueDate").toString();
        QString time = m_timeLockSet.value("valueTime").toString();
        QString timezone = m_timeLockSet.value("valueTimezone").toString();
        QString timezoneId = qUtils::extractTimeZoneId(timezone);
        QDateTime dt = QDateTime::fromString(date + " " + time, "MM/dd/yyyy HH:mm");
        QTimeZone tz(timezoneId.toUtf8());
        if (!tz.isValid()) {
            // Fallback to UTC
            dt.setTimeSpec(Qt::UTC);
        } else {
            dt.setTimeZone(tz);
        }
        qint64 timestamp = dt.toSecsSinceEpoch();
        QPointer<QGroupDashboard> safeThis(this);
        runInConcurrent([safeThis, timestamp, timezoneId]() ->bool{
            SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
            QJsonObject output;
            QString error_msg = "";
            bool ret {false};
            if (ptrLamda->isUserWallet() || ptrLamda->isUserDraftWallet()) {
                ret = Draco::instance()->DraftWalletUpdateTimelock(timezoneId, timestamp, output, error_msg);
            } else {
                ret = Byzantine::instance()->DraftWalletUpdateTimelock(ptrLamda->groupId(), timezoneId, timestamp, output, error_msg);
            }
            return ret;
        },[safeThis](bool ret) {
            SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
            if (ret) {
                ptrLamda->GetDraftWalletInfo();
            }
        });
    } else {
        auto maps = datetime.toMap();
        if (!maps.isEmpty()) {
            m_timeLockSet["valueDate"] = maps["valueDate"].toString();
            m_timeLockSet["valueTime"] = maps["valueTime"].toString();
            m_timeLockSet["valueTimezone"] = maps["valueTimezone"].toString();
            emit timeLockSetChanged(); 
        }             
    }
}

void QGroupDashboard::walletSetupTimeLock(const QVariant &datetime, bool isPutServer)
{
    if (isPutServer) {
        QString date = m_timeLockReplacement.value("valueDate").toString();
        QString time = m_timeLockReplacement.value("valueTime").toString();
        QString timezone = m_timeLockReplacement.value("valueTimezone").toString();
        QString timezoneId = qUtils::extractTimeZoneId(timezone);
        QString verifyToken = servicesTagPtr()->passwordToken();
        QDateTime dt = QDateTime::fromString(date + " " + time, "MM/dd/yyyy HH:mm");
        QTimeZone tz(timezoneId.toUtf8());
        if (!tz.isValid()) {
            // Fallback to UTC
            dt.setTimeSpec(Qt::UTC);
        } else {
            dt.setTimeZone(tz);
        }
        qint64 timestamp = dt.toSecsSinceEpoch();
        QPointer<QGroupDashboard> safeThis(this);
        runInConcurrent([safeThis, timestamp, timezoneId, verifyToken]() ->bool{
            SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
            QJsonObject output;
            bool ret {false};
            if (ptrLamda->isUserWallet() || ptrLamda->isUserDraftWallet()) {
                ret = Draco::instance()->walletChangeTimelock(ptrLamda->wallet_id(), timezoneId, timestamp, verifyToken, output);
            } else {
                ret = Byzantine::instance()->walletChangeTimelock(ptrLamda->wallet_id(), ptrLamda->groupId(), timezoneId, timestamp, verifyToken, output);
            }
            DBG_INFO << "output:" << output << "ret:" << ret;
            return ret;
        },[safeThis](bool ret) {
                            SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
                            if (ret) {
                                ptrLamda->GetDraftWalletInfo();
                                ptrLamda->GetKeyReplacementStatus();
                                emit ptrLamda->timelockReplacementChanged();
                            }
                        });
    } else {
        auto maps = datetime.toMap();
        if (!maps.isEmpty()) {
            m_timeLockReplacement["valueDate"] = maps["valueDate"].toString();
            m_timeLockReplacement["valueTime"] = maps["valueTime"].toString();
            m_timeLockReplacement["valueTimezone"] = maps["valueTimezone"].toString();
            emit timelockReplacementChanged();
        }
    }
}

bool QGroupDashboard::enoughKeyAdded(const QString& xfp) {
    QJsonArray signers = m_walletDraftInfo.value("signers").toArray();
    int count = 0;
    QString wallet_type = m_walletDraftInfo.value("wallet_type").toString();
    for (QJsonValue js : signers) {
        QJsonObject signer = js.toObject();
        if (signer.value("xfp").toString() == xfp) {
            count++;
        }
    }
    if (wallet_type == "MINISCRIPT") {
        return count == 2;
    } else {
        return count == 1;
    }
}

void QGroupDashboard::preparingKeyTobeReplaced()
{
    QJsonArray signers      = m_walletInfo["signers"].toArray();
    bool allow_inheritance = false;
    bool required_server_key = false;
    for (auto js : signers) {
        QJsonObject signer = js.toObject();
        QJsonArray tags = signer["tags"].toArray();
        if (tags.contains("INHERITANCE")) {
            allow_inheritance = true;
        }
        QString type = signer["type"].toString();
        if (type == "SERVER") {
            required_server_key = true;
        }
    }
    if (auto wallet = walletInfoPtr()) {
        int m = 0;
        int n = 0;
        if(walletInfo()->isHoneyBadger()){
            m = 2;
            n = 4;
        }
        else if(walletInfo()->isIronHand()){
            m = 2;
            n = 3;
        }
        else if(isGroupWallet()){
            QJsonObject output;
            QString error_msg = "";
            bool ret = Byzantine::instance()->GetOneGroupWallets(groupId(), output, error_msg);
            if(ret){
                QJsonObject group_info = output["group"].toObject();
                QJsonObject config     = group_info["wallet_config"].toObject();
                m = config["m"].toInt();
                n = config["n"].toInt();
                allow_inheritance = config["allow_inheritance"].toBool();
                required_server_key = config["required_server_key"].toBool();
            }
            m = output["m"].toInt();
        }
        else {
            return;
        }
        QJsonObject wallet_config;
        wallet_config["m"] = m;
        wallet_config["n"] = n;
        wallet_config["allow_inheritance"] = allow_inheritance;
        wallet_config["required_server_key"] = required_server_key;
        QJsonObject data;
        data["wallet_type"]     = m_walletInfo["wallet_type"].toString();
        data["signers"]         = signers;        
        data["wallet_config"]   = wallet_config;
        UpdateReplacementKeys(data);
    }
}

