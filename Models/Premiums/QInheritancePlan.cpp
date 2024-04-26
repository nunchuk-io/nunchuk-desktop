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

QJsonObject QInheritancePlan::ConvertToDisplayQml(QJsonObject data)
{
    long int activation_time_milis = static_cast<long int>(data.value("activation_time_milis").toDouble()/1000);
    if (activation_time_milis > 0) {
        data["activation_date"] = QDateTime::fromTime_t(activation_time_milis).date().toString("MM/dd/yyyy");
    } else {
        data["activation_date"] = "";
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

bool QInheritancePlan::GetInheritancePlan()
{
    QWalletPtr w = walletInfoPtr();
    if (w.isNull()) return false;
    QJsonObject response;
    QString errormsg;
    bool ret = Draco::instance()->inheritanceGetPlan(w->id(), w->groupId(), response, errormsg);
    if(ret){
        QJsonObject inheritance = response["inheritance"].toObject();
        DBG_INFO << inheritance;
        m_planInfoCurrent = ConvertToDisplayQml(inheritance);
        setPlanInfo(ConvertToDisplayQml(inheritance));
    }
    return ret;
}

QJsonObject QInheritancePlan::JsBody()
{
    QJsonObject body;
    body["wallet"] = wallet_id();
    body["group_id"] = WalletsMng->isUserWallet(wallet_id()) ? "" : walletInfoPtr()->groupId();
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
            body["group_id"] = WalletsMng->isUserWallet(wallet_id()) ? "" : wallet->groupId();
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
    bodyCancel["group_id"] = WalletsMng->isUserWallet(wallet_id()) ? "" : wallet->groupId();

    QJsonObject body_data = isCancel ? bodyCancel : JsBody();
    QString errormsg = "";
    QJsonObject output;
    bool ret = Draco::instance()->inheritancePlanRequiredSignatures(body_data, output, errormsg);
    if (ret) {
        DBG_INFO << output;
        QJsonObject resultObj = output["result"].toObject();
        ServiceSetting::instance()->servicesTagPtr()->setReqiredSignatures(resultObj);
        info = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesInfo();
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
        body["group_id"] = WalletsMng->isUserWallet(wallet_id()) ? "" : w->groupId();
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
        QString wallet_id = wallet->id();
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
        QString wallet_id = wallet->id();
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
    DBG_INFO;
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
    QMap<QString,QVariant> _new = info.toMap();
    DBG_INFO << _new;
    QJsonObject change = m_planInfo;
    QJsonObject _old = m_planInfoCurrent;
    change["edit_isChanged"] = false;
    if (_new["note"].isValid() && _old["note"].toVariant() != _new["note"]) {
        change["note"] = _new["note"].toString();
    }
    if (_new["display_emails"].isValid() && _old["display_emails"].toVariant() != _new["display_emails"]) {
        change["display_emails"] = _new["display_emails"].toString();
    }
    if (_new["activation_date"].isValid() && _old["activation_date"].toVariant() != _new["activation_date"]) {
        change["activation_date"] = _new["activation_date"].toString();
    }
    if (_new["notify_today"].isValid() && _old["notify_today"].toVariant() != _new["notify_today"]) {
        change["notify_today"] = _new["notify_today"].toBool();
    }
    if (_new["buffer_period"].isValid()) {
        QJsonObject buffer_period = _old["buffer_period"].toObject();
        QMap<QString,QVariant> new_buffer_period = _new["buffer_period"].toMap();
        if (new_buffer_period["id"].isValid() && buffer_period["id"].toVariant() != new_buffer_period["id"]) {
            buffer_period["id"] = new_buffer_period["id"].toString();
        }
        if (new_buffer_period["display_name"].isValid() && buffer_period["display_name"].toVariant() != new_buffer_period["display_name"]) {
            buffer_period["display_name"] = new_buffer_period["display_name"].toString();
        }
        if (new_buffer_period["enabled"].isValid() && buffer_period["enabled"].toVariant() != new_buffer_period["enabled"]) {
            buffer_period["enabled"] = new_buffer_period["enabled"].toBool();
        }
        change["buffer_period"] = buffer_period;
    }
    if (change != _old) {
        change["edit_isChanged"] = true;
    }
    DBG_INFO << change;
    setPlanInfo(change);
}

QWalletServicesTagPtr QInheritancePlan::servicesTagPtr() const
{
    return ServiceSetting::instance()->servicesTagPtr();
}
