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
    DBG_INFO << myInfo()["role"].toString();
    return myInfo()["role"].toString();
}

bool QGroupDashboard::accepted() const
{
    if (mode() == USER_WALLET) {
        return true;
    }
    else {
        return qUtils::strCompare(myInfo()["status"].toString(), "ACTIVE");
    }
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
    if (mode() == USER_WALLET) return;
    QJsonObject output;
    QString error_msg = "";
    bool ret = Byzantine::instance()->GetOneGroupWallets(groupId(), output, error_msg);
    if (ret) {
        setGroupInfo(output["group"].toObject());
    }
}

void QGroupDashboard::GetAlertsInfo()
{
    QJsonObject output;
    QString error_msg = "";
    bool ret {false};
    if (mode() == USER_WALLET) {
        ret = Draco::instance()->GetAlerts(wallet_id(), output, error_msg);
    }
    else {
        ret = Byzantine::instance()->GetGroupAlerts(groupId(), output, error_msg);
    }
    if(ret){
        DBG_INFO << output;
        // Handle preparing model here.
        QJsonArray   alerts  = output["alerts"].toArray();
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
        output["alerts"] = v_alerts;
        m_alertInfo = output;
        emit alertInfoChanged();
    }
    else{
        //Show error
    }
}

bool QGroupDashboard::MarkAlertAsRead(const QString &alert_id)
{
    QJsonObject output;
    QString error_msg = "";
    bool ret {false};
    if (mode() == USER_WALLET) {
        ret = Draco::instance()->MarkAlertAsRead(groupId(), alert_id, output, error_msg);
    }
    else {
        ret = Byzantine::instance()->MarkGroupAlertAsRead(groupId(), alert_id, output, error_msg);
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

bool QGroupDashboard::DismissAlert(const QString &alert_id)
{
    QJsonObject output;
    QString error_msg = "";
    bool ret {false};
    if (mode() == USER_WALLET) {
        ret = Draco::instance()->DismissAlert(wallet_id(), alert_id, output, error_msg);
    }
    else {
        ret = Byzantine::instance()->DismissGroupAlert(groupId(), alert_id, output, error_msg);
    }
    if(ret){
        // Handle preparing model here.
    }
    else{
        //Show error
    }
    return ret;
}

bool QGroupDashboard::DismissAlert()
{
    return DismissAlert(alertId());
}

void QGroupDashboard::GetWalletInfo()
{
    QJsonObject output;
    QString error_msg = "";
    bool ret {false};
    if (mode() == USER_WALLET) {
        ret = Draco::instance()->assistedWalletGetInfo(wallet_id(), output, error_msg);
    } else {
        ret = Byzantine::instance()->GetCurrentGroupWallet(groupId(), output, error_msg);
    }
    if (ret) {
        QJsonObject wallet = output["wallet"].toObject();
        if (wallet.isEmpty()) return;
        m_walletInfo = wallet;
        DBG_INFO << wallet;
        QJsonArray signers = wallet["signers"].toArray();
        m_signerInfo = signers;
        checkInheritanceWallet();
        emit groupInfoChanged();
        if (walletInfoPtr() && walletInfoPtr()->serverKeyPtr()) {
            walletInfoPtr()->serverKeyPtr()->UpdateFromWallet(wallet);
        }
    }
    else {
        DBG_INFO << ret << error_msg;
        //Show error
    }
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
    if (mode() == USER_WALLET) return;
    QJsonObject output;
    QString error_msg = "";
    bool ret = Byzantine::instance()->GetCurrentGroupDraftWallet(groupId(), output, error_msg);
    if (ret) {
        QJsonObject draft_wallet = output["draft_wallet"].toObject();
        UpdateKeys(draft_wallet);
    }
}

void QGroupDashboard::GetHealthCheckInfo()
{
    if (!healthPtr() || wallet_id().isEmpty()) return;
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

    QJsonArray signers = data["signers"].toArray();
    QJsonObject server;
    server["type"] = "SERVER";
    server["has"]  = false;

    for (QJsonValue js : signers) {
        QJsonObject signer = js.toObject();
        signer["signer_type"] = (int)qUtils::GetSignerType(signer["type"].toString());
        signer["account_index"] = qUtils::GetIndexFromPath(signer["derivation_path"].toString());
        QJsonObject tapsignerObj = signer["tapsigner"].toObject();
        signer["card_id"] = tapsignerObj.isEmpty() ? "" : tapsignerObj["card_id"].toString();
        QString type = signer["type"].toString();
        if (type == "SERVER") {
            server = signer;
            server["has"] = true;
        }
    }
    auto find = [&](const int index) -> QJsonObject{
        for (QJsonValue js : signers) {
            QJsonObject signer = js.toObject();
            int key_index = signer["key_index"].toInt(-1);
            if (key_index == index) {
                signer["has"] = true;
                QJsonArray tags = signer["tags"].toArray();
                for (auto tag : tags) {
                    signer["tag"] = tag.toString();
                }
                signer["signer_type"] = (int)qUtils::GetSignerType(signer["type"].toString());
                signer["account_index"] = qUtils::GetIndexFromPath(signer["derivation_path"].toString());
                QJsonObject tapsignerObj = signer["tapsigner"].toObject();
                QString type = signer["type"].toString();
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
                for (auto tag : tags) {
                    signer["tag"] = tag.toString();
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
    int  last_index = n-1;
    bool is_premier = CLIENT_INSTANCE->isByzantinePremier() || isPremierGroup();

    DBG_INFO << is_premier << m_allow_inheritance << m_mInfo << m_nInfo;

    if (is_wallet_2_of_4) {
        for(int i = 0; i < last_index; i++) {
            QJsonObject obj = find(i);
            if (obj.isEmpty()) {
                obj["key_index"] = i;
                obj["has"] = false;
                if((i == 0) && m_allow_inheritance && !is_premier){
                    obj["type"] = "NFC";
                }
            }
            m_keys.insert(i, QVariant::fromValue(obj));
        }
        m_keys.insert(last_index, QVariant::fromValue(server));
    }
    else if (is_wallet_3_of_5) {
        for(int i = 0; i < last_index; i++) {
            QJsonObject obj = find(i);
            if (obj.isEmpty()) {
                obj["key_index"] = i;
                obj["has"] = false;
                if((i == 0 || i == 1) && m_allow_inheritance && !is_premier){
                    obj["type"] = "NFC";
                }
            }
            m_keys.insert(i, QVariant::fromValue(obj));
        }
        m_keys.insert(last_index, QVariant::fromValue(server));
    }
    else {
        for(int i = 0; i < n; i++) {
            QJsonObject obj = find(i);
            if (obj.isEmpty()) {
                obj["key_index"] = i;
                obj["has"] = false;
            }
            m_keys.insert(i, QVariant::fromValue(obj));
        }
    }
    emit draftWalletChanged();
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
            QtConcurrent::run([this](){
                DismissAlert();
                GetAlertsInfo();
            });
        }
        else if (register_key_xfps.size() == 0) {
            QtConcurrent::run([this](){
                DismissAlert();
                GetAlertsInfo();
            });
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
    return m_alertId;
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
    case AlertEnum::E_Alert_t::GROUP_WALLET_PENDING:{
        QtConcurrent::run([this]() {
            GetDraftWalletInfo();
            if (hasWallet()) {
                GetWalletInfo();
            }
        });
        return true;
    }
    case AlertEnum::E_Alert_t::KEY_RECOVERY_APPROVED:{
        QString key_xfp = payload["key_xfp"].toString();
        return ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->UserKeysRecoveryKey(key_xfp);
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
    default:
        break;
    }
    return false;
}

QString QGroupDashboard::slug()
{
    return groupInfo()["slug"].toString();
}

bool QGroupDashboard::isPremierGroup()
{
    QString type = slug();
    bool ret =    qUtils::strCompare(type, "byzantine_premier_testnet")
               || qUtils::strCompare(type, "byzantine_premier");
    return ret;
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
                invitees_id << member["user"].toObject()["chat_id"].toString();
                invitees_name << member["user"].toObject()["name"].toString();
            }
        }
        if(hasWallet()){
            QString wallet_name = AppModel::instance()->walletInfo() ? AppModel::instance()->walletInfo()->name() : "";
            if(wallet_name != ""){
                invitees_name.clear();
                invitees_name.append(wallet_name);
            }
        }
        DBG_INFO << invitees_id << invitees_name;
        if(invitees_id.size() > 0){
            CLIENT_INSTANCE->createRoomByzantineChat(invitees_id, invitees_name, group_id, "");
        }
    }
    return true;
}

void QGroupDashboard::byzantineRoomCreated(QString room_id, bool existed)
{
    if (mode() == USER_WALLET) return;
    DBG_INFO << room_id << existed;
    QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_CHAT_TAB);
    if(room_id != "" && !existed){
        QJsonObject output;
        QString error_msg = "";
        bool ret = Byzantine::instance()->CreateGroupChat(groupId(), room_id, output, error_msg);
        if(!ret){
            //TBD
        }
    }
}

void QGroupDashboard::byzantineRoomDeleted(QString room_id, QString group_id)
{
    if (mode() == USER_WALLET) return;
    DBG_INFO << room_id << group_id;
    if(qUtils::strCompare(group_id, groupId())){
        QJsonObject output;
        QString error_msg = "";
        bool ret = Byzantine::instance()->DeleteGroupChat(group_id, output, error_msg);
        if(!ret){
            //TBD
        }
    }
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
    nunchuk::Wallet wallet = w->wallet();
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
    return !wallet_id().isEmpty() or WalletsMng->hasWallet(groupId());
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

QJsonObject QGroupDashboard::GetSigner(const QString &xfp) const
{
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
            signer["signer_is_primary"] = qUtils::isPrimaryKey(xfp);
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
    m_alertId = alertId;
    QJsonArray array = m_alertInfo["alerts"].toArray();
    for(auto js : array) {
        QJsonObject alert = js.toObject();
        if (alert["id"].toString() == m_alertId) {
            setAlertId(alert);
        }
    }
}

void QGroupDashboard::setAlertId(const QJsonObject &alert)
{
    m_currentAlertInfo = alert;
    DBG_INFO << alert;
    emit alertInfoChanged();
}

void QGroupDashboard::setDummyTxAlert(const QJsonObject &dummy_transaction)
{
    QJsonObject alert = alertJson();
    QJsonObject payload = alert["payload"].toObject();
    payload["dummy_transaction_id"] = dummy_transaction["id"].toString();
    payload["pending_signatures"] = dummy_transaction["pending_signatures"].toInt();
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
