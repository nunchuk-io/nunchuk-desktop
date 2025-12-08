#include "QInheritancePlan.h"
#include "QServerKey.h"
#include <QQmlEngine>
#include <QJsonArray>
#include <QJsonObject>
#include "qUtils.h"
#include "WalletModel.h"
#include "QGroupDashboard.h"
#include "AppModel.h"
#include "WalletModel.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "localization/STR_CPP.h"
#include "ServiceSetting.h"
#include "QWalletServicesTag.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QUserWalletDummyTx.h"

QInheritancePlan::QInheritancePlan(const QString &wallet_id)
    : QBasePremium(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

int QInheritancePlan::actionPlan() const
{
    return m_actionPlan;
}

void QInheritancePlan::setActionPlan(int actionPlan)
{
    if (m_actionPlan == actionPlan)
        return;

    m_actionPlan = actionPlan;
    emit actionPlanChanged();
}

QString QInheritancePlan::secret() const
{
    return m_secret;
}

void QInheritancePlan::setSecret(QString secret)
{
    if (m_secret == secret)
        return;

    m_secret = secret;
    emit secretChanged();
}

QVariantList QInheritancePlan::periods() const
{
    return m_periods.toVariantList();
}

void QInheritancePlan::setPeriods(QJsonArray periods)
{
    if (m_periods == periods)
        return;

    m_periods = periods;
    emit periodsChanged();
}

QVariant QInheritancePlan::planInfo() const
{
    return QVariant::fromValue(m_planInfo);
}

void QInheritancePlan::setPlanInfo(QJsonObject plan_info)
{
    if (m_planInfo == plan_info)
        return;

    m_planInfo = plan_info;
    emit planInfoChanged();
}


QVariant QInheritancePlan::planInfoNew() const
{
    return QVariant::fromValue(m_planInfoNew);
}

void QInheritancePlan::setPlanInfoNew(const QJsonObject &planInfoNew)
{
    if (m_planInfoNew == planInfoNew)
        return;

    m_planInfoNew = planInfoNew;
    emit planInfoNewChanged();
}

QVariant QInheritancePlan::planInfoUpdate() const
{
    QJsonObject _old = m_planInfoOld;
    QJsonObject _new = m_planInfoNew;
    QJsonObject change = m_planInfoNew;
    if (_old["note"] != _new["note"]) {
        change["note_isChanged"] = true;
    }
    if (_old["display_emails"] != _new["display_emails"]) {
        change["display_emails_isChanged"] = true;
    }
    if (_old["activation_date"] != _new["activation_date"]) {
        change["activation_date_isChanged"] = true;
    }
    if (_old["buffer_period_id"] != _new["buffer_period_id"]) {
        change["buffer_period_isChanged"] = true;
    }
    if (_old["notify_today"] != _new["notify_today"]) {
        change["notify_today_isChanged"] = true;
    }
    return QVariant::fromValue(change);
}

bool QInheritancePlan::isSetup() const
{
    QJsonObject accountSetup {};
    if (auto dash = dashBoardPtr()) {
        QJsonArray members = dash->groupInfo()["members"].toArray();
        QString owner_id = m_planInfo["owner_id"].toString();
        for (auto member : members) {
            QJsonObject user = member.toObject()["user"].toObject();
            QString id = user["id"].toString();
            if (qUtils::strCompare(owner_id, id)) {
                accountSetup = user;
            }
        }
    }
    QString email = accountSetup["email"].toString();
    DracoUser me = ClientController::instance()->getMe();
    return qUtils::strCompare(email, me.email);
}

QString QInheritancePlan::formatDateTime() const {
    if (auto walletInfo = walletInfoPtr()) {
        if (walletInfo->walletType() == (int)nunchuk::WalletType::MINISCRIPT) {
            return QString("MM/dd/yyyy hh:mm");
        } else {
            return QString("MM/dd/yyyy");
        }
    }
    return QString();
}

QJsonObject QInheritancePlan::ConvertToDisplayQml(QJsonObject data)
{
    auto wallet = walletInfoPtr();
    if (wallet.isNull()) return data;
    auto dashBoard = dashBoardPtr();
    if (dashBoard.isNull()) return data;
    nunchuk::WalletType wallet_type = wallet->nunchukWallet().get_wallet_type();
    if (wallet_type == nunchuk::WalletType::MINISCRIPT) {
        data["activation_date"] = dashBoard->timeLock();
        data["activation_timezone"] = wallet->timezones()->selectedTimezone();
        data["activation_timezone_local"] = wallet->timezones()->localTimezone();
    }
    else {
        long int activation_time_milis = static_cast<long int>(data.value("activation_time_milis").toDouble()/1000);
        if (activation_time_milis > 0) {
            data["activation_date"] = QDateTime::fromTime_t(activation_time_milis).toString(formatDateTime());
        }
        else {
            data["activation_date"] = "";
        }
        data["activation_timezone"] = qUtils::formatTimeZoneString(data["timezone"].toString());
        data["activation_timezone_local"] = wallet->timezones()->localTimezone();
        wallet->timezones()->setSelectedTimezone(data["activation_timezone"].toString());
    }
    
    QJsonArray emails = data.value("notification_emails").toArray();
    if (emails.size() > 0) {
        QStringList emailList;
        for (QJsonValue js : emails) {
            emailList.append(js.toString());
        }
        QString email = emailList.join(",");
        data["display_emails"] = email;
    } else {
        data["display_emails"] = "";
    }
    
    QJsonObject buffer_period = data["buffer_period"].toObject();
    if (buffer_period.isEmpty()) {
        buffer_period["id"] = "";
        buffer_period["interval"] = "";
        buffer_period["interval_count"] = 0;
        buffer_period["enabled"] = false;
        buffer_period["is_recommended"] = false;
        buffer_period["display_name"] = "";
        data["buffer_period"] = buffer_period;
    }

    data["note_isChanged"] = false;
    data["display_emails_isChanged"] = false;
    data["activation_date_isChanged"] = false;
    data["buffer_period_isChanged"] = false;
    data["notify_today_isChanged"] = false;
    data["edit_isChanged"] = false;

    return data;
}

void QInheritancePlan::UpdateFromDummyTx(QJsonObject data)
{
    setPlanInfoOld(ConvertToDisplayQml(data["payload"].toObject()["old_data"].toObject()));
    setPlanInfoNew(ConvertToDisplayQml(data["payload"].toObject()["new_data"].toObject()));
    emit planInfoUpdateChanged();
}

QVariant QInheritancePlan::planInfoOld() const
{
    return QVariant::fromValue(m_planInfoOld);
}

void QInheritancePlan::setPlanInfoOld(const QJsonObject &planInfoOld)
{
    if (m_planInfoOld == planInfoOld)
        return;

    m_planInfoOld = planInfoOld;
    emit planInfoOldChanged();
}

void QInheritancePlan::GetInheritancePlan()
{
    QPointer<QInheritancePlan> safeThis(this);
    runInThread([safeThis]() ->QJsonObject{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QWalletPtr w = ptrLamda->walletInfoPtr();
        if (w.isNull() || (w.data()->status() == "REPLACED")) return {};
        QJsonObject response;
        QString errormsg;
        Draco::instance()->inheritanceGetPlan(w->walletId(), w->groupId(), response, errormsg);
        return response["inheritance"].toObject();
    },[safeThis](QJsonObject inheritance) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        if (!inheritance.isEmpty()) {
            DBG_INFO << inheritance;
            ptrLamda->m_planInfoCurrent = ptrLamda->ConvertToDisplayQml(inheritance);
            ptrLamda->setPlanInfo(ptrLamda->ConvertToDisplayQml(inheritance));
            if (auto dash = ptrLamda->dashBoardPtr()) {
                if (auto tag = dash->servicesTagPtr()) {
                    tag->setListInheritantPlans();
                }
            }
        }
    });
}

QJsonObject QInheritancePlan::JsBody()
{
    QJsonObject body;
    body["wallet"] = wallet_id();
    body["group_id"] = isUserWallet() ? "" : walletInfoPtr()->groupId();
    body["note"] = m_planInfo["note"].toString();
    body["buffer_period_id"] = m_planInfo["buffer_period"].toObject()["id"].toString();
    QString email = m_planInfo["display_emails"].toString();
    QStringList emails = email.split(",");
    QJsonArray arrays;
    for (QString e : emails) {
        arrays.append(e);
    }
    body["notification_emails"] = arrays;
    body["notify_today"] = m_planInfo["buffer_period"].toObject()["enabled"].toBool();
    QString activation_date = m_planInfo["activation_date"].toString();
    long int t = qUtils::GetTimeSecond(activation_date);
    double activation_time_milis = m_planInfoCurrent["activation_time_milis"].toDouble();
    long int old_activeTime = static_cast<long int>(activation_time_milis/1000);
    if (QDateTime::fromTime_t(old_activeTime).date() == QDateTime::fromTime_t(t).date()) {
        body["activation_time_milis"] = activation_time_milis;
    }
    else {
        body["activation_time_milis"] = (double)t*1000;
    }
    body["notification_preferences"] = m_planInfo["notification_preferences"].toObject();
    return body;
}

bool QInheritancePlan::RequestInheritancePlanUpdate()
{
    ReqiredSignaturesInfo info{};
    bool ret = inheritancePlanRequiredSignatures(info);
    if(ret){
        if (info.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            servicesTagPtr()->CreateSecurityQuestionsAnswered();
        }
        else if (info.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
            QJsonObject output;
            QString errormsg = "";
            QJsonObject data;
            data["nonce"] = Draco::instance()->randomNonce();
            data["body"]  = JsBody();
            QStringList authorizations;
            bool ret = Draco::instance()->inheritancePlanUpdate(servicesTagPtr()->passwordToken(),
                                                                servicesTagPtr()->secQuesToken(),
                                                                authorizations,
                                                                data,
                                                                true,
                                                                output,
                                                                errormsg);
            DBG_INFO << ret << output;
            if(ret){
                QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
                DBG_INFO << dummy_transaction;
                if (auto dummy = groupDummyTxPtr()) {
                    dummy->setDummyTxData(dummy_transaction);
                }
            }
            //Show popup support in moble
            emit inheritanceDummyTransactionAlert();
        }
        else {}
    }
    return ret;
}

bool QInheritancePlan::RequestInheritancePlanCreate()
{
    ReqiredSignaturesInfo info{};
    bool ret = inheritancePlanRequiredSignatures(info);
    if(ret){
        if (info.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            servicesTagPtr()->CreateSecurityQuestionsAnswered();
        }
        else if (info.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
            QJsonObject output;
            QString errormsg = "";
            QJsonObject data;
            data["nonce"] = Draco::instance()->randomNonce();
            data["body"]  = JsBody();
            QStringList authorizations;
            bool ret = Draco::instance()->inheritancePlanCreate(authorizations,
                                                                data,
                                                                true,
                                                                output,
                                                                errormsg);
            DBG_INFO << ret << output;
            if(ret){
                QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
                DBG_INFO << dummy_transaction;
                if (auto dummy = groupDummyTxPtr()) {
                    dummy->setDummyTxData(dummy_transaction);
                }
            }
            //Show popup support in moble
            emit inheritanceDummyTransactionAlert();
        }
        else {}
    }
    return ret;
}

bool QInheritancePlan::RequestInheritancePlanCancel()
{
    QWalletPtr wallet = walletInfoPtr();
    if(!wallet){
        return false;
    }
    ReqiredSignaturesInfo info{};
    bool ret = inheritancePlanRequiredSignatures(info, true);
    if(ret){
        if (info.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            servicesTagPtr()->CreateSecurityQuestionsAnswered();
        }
        else if (info.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
            QJsonObject output;
            QString errormsg = "";
            QJsonObject data;
            data["nonce"] = Draco::instance()->randomNonce();

            QJsonObject body;
            body["wallet"] = wallet_id();
            body["group_id"] = isUserWallet() ? "" : wallet->groupId();
            data["body"]  = body;

            QStringList authorizations;
            bool ret = Draco::instance()->inheritancePlanCancel(servicesTagPtr()->passwordToken(),
                                                                servicesTagPtr()->secQuesToken(),
                                                                authorizations,
                                                                data,
                                                                true,
                                                                output,
                                                                errormsg);
            if(ret){
                QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
                if (auto dummy = groupDummyTxPtr()) {
                    dummy->setDummyTxData(dummy_transaction);
                }
            }
            //Show popup support in moble
            emit inheritanceDummyTransactionAlert();
        }
        else {}
    }
    return ret;
}

bool QInheritancePlan::inheritancePlanRequiredSignatures(ReqiredSignaturesInfo &info, bool isCancel)
{
    QWalletPtr wallet = walletInfoPtr();
    if(!wallet){
        return false;
    }
    QString activation_date = m_planInfo["activation_date"].toString();
    long int activeTime = qUtils::GetTimeSecond(activation_date);
    long int old_activeTime = static_cast<long int>(m_planInfoCurrent["activation_time_milis"].toDouble()/1000);
    long int currentTime = qUtils::GetCurrentTimeSecond();
    if (QDateTime::fromTime_t(old_activeTime).date() != QDateTime::fromTime_t(activeTime).date() && activeTime < currentTime) {
        AppModel::instance()->showToast(0, STR_CPP_120, EWARNING::WarningType::ERROR_MSG);
        return false;
    }
    QJsonObject bodyCancel;
    bodyCancel["wallet"] = wallet_id();
    bodyCancel["group_id"] = isUserWallet() ? "" : wallet->groupId();

    QJsonObject body_data = isCancel ? bodyCancel : JsBody();
    QString errormsg = "";
    QJsonObject output;
    bool ret = Draco::instance()->inheritancePlanRequiredSignatures(body_data, output, errormsg);
    if (ret) {
        DBG_INFO << output;
        QJsonObject resultObj = output["result"].toObject();
        servicesTagPtr()->setReqiredSignatures(resultObj);
        info = servicesTagPtr()->reqiredSignaturesInfo();
    }
    return ret;
}

bool QInheritancePlan::InheritancePlanUpdateSucceed()
{
    QJsonObject data;
    QStringList authorizations;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"]  = JsBody();

    QJsonObject output;
    QString errormsg = "";
    DBG_INFO << servicesTagPtr()->passwordToken() << servicesTagPtr()->secQuesToken() << data;
    bool ret = Draco::instance()->inheritancePlanUpdate(servicesTagPtr()->passwordToken(), servicesTagPtr()->secQuesToken(), authorizations, data, false, output, errormsg);
    DBG_INFO << errormsg << output;
    if (ret) {
        emit securityQuestionClosed();
        AppModel::instance()->showToast(0, STR_CPP_119, EWARNING::WarningType::SUCCESS_MSG);
    }
    return ret;
}

bool QInheritancePlan::InheritancePlanCreateSucceed()
{
    QStringList authorizations;
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"]  = JsBody();

    QJsonObject output;
    QString errormsg = "";
    bool ret = Draco::instance()->inheritancePlanCreate(authorizations, data, false, output, errormsg);
    DBG_INFO << errormsg << output;
    if (ret) {
        AppModel::instance()->showToast(0, STR_CPP_119, EWARNING::WarningType::SUCCESS_MSG);
    }
    return ret;
}

bool QInheritancePlan::InheritancePlanCancelSucceed()
{
    if (auto w = walletInfoPtr()) {
        QStringList authorizations;
        QJsonObject data;
        data["nonce"] = Draco::instance()->randomNonce();
        QJsonObject body;
        body["wallet"] = wallet_id();
        body["group_id"] = isUserWallet() ? "" : w->groupId();
        data["body"]  = body;

        QJsonObject output;
        QString errormsg = "";

        DBG_INFO << servicesTagPtr()->passwordToken() << servicesTagPtr()->secQuesToken() << data;
        bool ret = Draco::instance()->inheritancePlanCancel(servicesTagPtr()->passwordToken(), servicesTagPtr()->secQuesToken(), authorizations, data, false, output, errormsg);
        DBG_INFO << errormsg << output;
        if (ret) {
            emit securityQuestionClosed();
            AppModel::instance()->showToast(0, STR_CPP_119, EWARNING::WarningType::SUCCESS_MSG);
        }
        return ret;
    }
    return false;
}

bool QInheritancePlan::InheritancePlanCancel()
{
    if (auto dummy = groupDummyTxPtr()) {
        return dummy->CancelDummyTransaction();
    }
    return false;
}

bool QInheritancePlan::IsActived() const
{
    QString status = m_planInfoCurrent.value("status").toString();
    if (status == "ACTIVE") {
        return true;
    }
    return false;
}

bool QInheritancePlan::InheritancePlanningRequestApprove()
{
    if (auto wallet = walletInfoPtr()) {
        QString wallet_id = wallet->walletId();
        if (auto dash = dashBoardPtr()) {
            QJsonObject alert = dash->alertJson();
            QJsonObject payload = alert["payload"].toObject();
            QString request_id = payload["request_id"].toString();
            QString group_id = dash->groupId();
            QString error_msg = "";
            return Draco::instance()->InheritancePlanningRequestApprove(request_id, wallet_id, group_id, error_msg);
        }
    }
    return false;
}

bool QInheritancePlan::InheritancePlanningRequestDeny()
{
    if (auto wallet = walletInfoPtr()) {
        QString wallet_id = wallet->walletId();
        if (auto dash = dashBoardPtr()) {
            QJsonObject alert = dash->alertJson();
            QJsonObject payload = alert["payload"].toObject();
            QString request_id = payload["request_id"].toString();
            QString group_id = dash->groupId();
            QString error_msg = "";
            return Draco::instance()->InheritancePlanningRequestDeny(request_id, wallet_id, group_id, error_msg);
        }
    }
    return false;
}

bool QInheritancePlan::createPeriods()
{
    QJsonArray periods;
    QString errormsg;
    bool ret = Draco::instance()->inheritancePlanBufferPeriod(periods, errormsg);
    if (!ret) {
        return false;
    } else {
        QJsonObject notNeed;
        notNeed["id"] = "";
        notNeed["display_name"] = "I don’t need a buffer period";
        notNeed["is_recommended"] = false;
        periods.append(notNeed);
        setPeriods(periods);
    }
    return true;
}

void QInheritancePlan::editPlanInfo(const QVariant &info)
{
    QMap<QString, QVariant> _new = info.toMap();
    QJsonObject original = m_planInfoCurrent; // baseline from server
    QJsonObject change   = m_planInfo;        // working copy (may already include previous edits)

    auto normalizeEmails = [](const QString &text) -> QString {
        QStringList parts = text.split(",", Qt::SkipEmptyParts);
        QStringList out;
        for (QString e : parts) {
            e = e.trimmed();
            if (e.isEmpty()) continue;
            bool dup = false;
            for (const QString &ex : out) {
                if (ex.compare(e, Qt::CaseInsensitive) == 0) { dup = true; break; }
            }
            if (!dup) out.append(e);
        }
        return out.join(",");
    };

    // note
    if (_new.contains("note")) {
        QString newNote = _new["note"].toString();
        QString oldNote = original.value("note").toString();
        if (newNote != oldNote) {
            change["note"] = newNote;
        }
    }

    // display_emails (normalize, trim, dedupe; compare canonically)
    if (_new.contains("display_emails")) {
        QString newEmails = normalizeEmails(_new["display_emails"].toString());
        QString oldEmails = normalizeEmails(original.value("display_emails").toString());
        if (newEmails != oldEmails) {
            change["display_emails"] = newEmails;
        }
    }

    // activation_date (accept only valid )
    if (_new.contains("activation_date")) {
        QString newDate = _new["activation_date"].toString().trimmed();
        QDate d = QDate::fromString(newDate, formatDateTime());
        if (d.isValid()) {
            QString oldDate = original.value("activation_date").toString();
            if (newDate != oldDate) {
                change["activation_date"] = newDate;
            }
        }
    }

    // notify_today (keep buffer_period.enabled in sync)
    if (_new.contains("notify_today")) {
        bool newNotify = _new["notify_today"].toBool();
        bool oldNotify = original.value("buffer_period").toObject().value("enabled").toBool();
        if (newNotify != oldNotify) {
            change["notify_today"] = newNotify;
            QJsonObject bp = change.value("buffer_period").toObject();
            bp["enabled"] = newNotify;
            change["buffer_period"] = bp;
        }
    }

    // buffer_period (merge any provided keys, keep notify_today in sync if enabled changed)
    if (_new.contains("buffer_period")) {
        QJsonObject oldBP = original.value("buffer_period").toObject();
        QJsonObject curBP = change.value("buffer_period").toObject();
        QMap<QString, QVariant> newBPMap = _new["buffer_period"].toMap();

        bool bpChanged = false;
        for (auto it = newBPMap.constBegin(); it != newBPMap.constEnd(); ++it) {
            QJsonValue newVal = QJsonValue::fromVariant(it.value());
            if (!oldBP.contains(it.key()) || oldBP.value(it.key()) != newVal) {
                curBP.insert(it.key(), newVal);
                bpChanged = true;
            }
        }

        // If enabled provided without explicit notify_today, mirror it
        if (!_new.contains("notify_today") && newBPMap.contains("enabled")) {
            bool newEnabled = newBPMap["enabled"].toBool();
            bool oldEnabled = oldBP.value("enabled").toBool();
            if (newEnabled != oldEnabled) {
                change["notify_today"] = newEnabled;
            }
        }

        if (bpChanged) {
            change["buffer_period"] = curBP;
        }
    }

    change["edit_isChanged"] = (change != original);
    setPlanInfo(change);
}

void QInheritancePlan::editPlanInfoOnchain(const QVariant &info)
{
    QMap<QString, QVariant> req = info.toMap();
    QString setup_type = req.value("setup_type").toString();

    QJsonObject original = m_planInfoCurrent;  // baseline from server
    QJsonObject change   = m_planInfo;         // working copy

    auto ensurePrefs = [](QJsonObject prefs) {
        if (!prefs.contains("beneficiary_notifications"))
            prefs["beneficiary_notifications"] = QJsonArray{};
        if (!prefs.contains("email_me_wallet_config"))
            prefs["email_me_wallet_config"] = false;
        return prefs;
    };

    auto normalizeEmails = [](const QString &text) -> QStringList {
        QStringList parts = text.split(",", Qt::SkipEmptyParts);
        QStringList out;
        for (QString e : parts) {
            e = e.trimmed();
            if (e.isEmpty()) continue;
            bool dup = false;
            for (const QString &ex : out) {
                if (ex.compare(e, Qt::CaseInsensitive) == 0) { dup = true; break; }
            }
            if (!dup) out.append(e);
        }
        return out;
    };

    change["notification_preferences"] = ensurePrefs(change.value("notification_preferences").toObject());

    if (setup_type == "emails") {
        QStringList newEmailList = normalizeEmails(req.value("display_emails").toString());

        QJsonObject oldPrefs = original.value("notification_preferences").toObject();
        QJsonArray oldList = oldPrefs.value("beneficiary_notifications").toArray();

        QJsonArray newList;
        for (const QString &email : newEmailList) {
            bool found = false;
            QJsonObject entry;
            for (const QJsonValue &v : oldList) {
                QJsonObject obj = v.toObject();
                if (obj.value("email").toString().compare(email, Qt::CaseInsensitive) == 0) {
                    entry = obj;
                    found = true;
                    break;
                }
            }
            if (!found) {
                entry["email"] = email;
                entry["notify_timelock_expires"] = true;
                entry["notify_wallet_changes"] = true;
                entry["include_wallet_config"] = true;
            }
            newList.append(entry);
        }

        QJsonObject curPrefs = change.value("notification_preferences").toObject();
        QJsonArray curList = curPrefs.value("beneficiary_notifications").toArray();
        if (newList != curList) {
            curPrefs["beneficiary_notifications"] = newList;
            change["notification_preferences"] = curPrefs;
        }

        QString newDisplay = newEmailList.join(",");
        if (change.value("display_emails").toString() != newDisplay) {
            change["display_emails"] = newDisplay;
        }

        QJsonArray flatEmails;
        for (const QString &e : newEmailList) flatEmails.append(e);
        if (change.value("notification_emails").toArray() != flatEmails) {
            change["notification_emails"] = flatEmails;
        }
    }
    else if (setup_type == "preference") {
        QString email = req.value("setup_email").toString().trimmed();
        if (!email.isEmpty()) {
            QJsonObject prefs = change.value("notification_preferences").toObject();
            QJsonArray list = prefs.value("beneficiary_notifications").toArray();
            bool changedLocal = false;

            for (int i = 0; i < list.size(); ++i) {
                QJsonObject item = list[i].toObject();
                if (item.value("email").toString().compare(email, Qt::CaseInsensitive) == 0) {
                    bool nt = req.contains("notify_timelock_expires")
                              ? req.value("notify_timelock_expires").toBool()
                              : item.value("notify_timelock_expires").toBool();
                    bool nw = req.contains("notify_wallet_changes")
                              ? req.value("notify_wallet_changes").toBool()
                              : item.value("notify_wallet_changes").toBool();
                    bool ic = req.contains("include_wallet_config")
                              ? req.value("include_wallet_config").toBool()
                              : item.value("include_wallet_config").toBool();

                    if (item.value("notify_timelock_expires").toBool() != nt ||
                        item.value("notify_wallet_changes").toBool() != nw ||
                        item.value("include_wallet_config").toBool() != ic) {
                        item["notify_timelock_expires"] = nt;
                        item["notify_wallet_changes"] = nw;
                        item["include_wallet_config"] = ic;
                        list[i] = item;
                        changedLocal = true;
                    }
                    break;
                }
            }

            if (changedLocal) {
                prefs["beneficiary_notifications"] = list;
                change["notification_preferences"] = prefs;
            }
        }
    }
    else if (setup_type == "owner") {
        if (req.contains("email_me_wallet_config")) {
            QJsonObject prefs = change.value("notification_preferences").toObject();
            bool newVal = req.value("email_me_wallet_config").toBool();
            if (prefs.value("email_me_wallet_config").toBool() != newVal) {
                prefs["email_me_wallet_config"] = newVal;
                change["notification_preferences"] = prefs;
            }
        }
    } else {
        return;
    }

    change["edit_isChanged"] = (change != original);
    setPlanInfo(change);
}
