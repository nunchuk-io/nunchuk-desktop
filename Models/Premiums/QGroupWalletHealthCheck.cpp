#include "QGroupWalletHealthCheck.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "QGroupWalletDummyTx.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritancePlan.h"
#include "QThreadForwarder.h"

QGroupWalletHealthCheck::QGroupWalletHealthCheck(WalletId wallet_id)
    : QBasePremium(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QGroupWalletHealthCheck::~QGroupWalletHealthCheck()
{
}

QString QGroupWalletHealthCheck::groupId() const
{
    if (auto dashboard = dashBoardPtr()) {
        return dashboard->groupId();
    }
    return "";
}

void QGroupWalletHealthCheck::GetStatuses()
{
    if (auto dashboard = dashBoardPtr()) {
        QPointer<QGroupWalletHealthCheck> safeThis(this);
        runInThread([safeThis]() ->QJsonArray{
            SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
            QJsonObject output;
            QString errormsg = "";
            if (ptrLamda->isUserWallet()) {
                Draco::instance()->GetWalletHealthStatus(ptrLamda->wallet_id(), output, errormsg);
            }
            else {
                Byzantine::instance()->GetWalletHealthStatus(ptrLamda->groupId(), ptrLamda->wallet_id(), output, errormsg);
            }
            return output["statuses"].toArray();
        },[safeThis, dashboard](QJsonArray statuses) {
            QJsonArray result;
            SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
            for (auto obj : statuses) {
                QJsonObject status = obj.toObject();
                QJsonValue last = status["last_health_check_time_millis"];
                if(last.isNull()){
                    status["lastState"] = "NotCheckedYet";
                }
                else{
                    double last_health_check_time_millis = last.toDouble();
                    double time_distance = ptrLamda.data()->CurrentTimeToMillis() - last_health_check_time_millis;
                    if (time_distance < ptrLamda.data()->SixMonthToMillis()) {
                        // Last checked: Less than 6 months ago
                        status["lastState"] = "LessThan6months";
                    } else if (time_distance > ptrLamda.data()->SixMonthToMillis() && time_distance < ptrLamda.data()->YearToMillis()) {
                        // Last checked: More than 6 months ago
                        status["lastState"] = "MoreThan6months";
                    } else if (time_distance > ptrLamda.data()->YearToMillis()) {
                        // Last checked: More than 6 months ago
                        status["lastState"] = "MoreThan1year";
                    }
                }
                QString xfp = status["xfp"].toString();
                status["keyinfo"] = dashboard->GetSigner(xfp);
                auto reminder = ptrLamda.data()->GetReminder(xfp);
                if (reminder.isEmpty()) {
                    status["reminder"] = {};
                }
                else {
                    status["reminder"] = reminder;
                }
                result.append(status);
            }
            DBG_INFO << "safeThis" << ptrLamda;
            ptrLamda.data()->m_healthStatuses = result;
            emit ptrLamda.data()->healthStatusesChanged();
        });
    }
}

void QGroupWalletHealthCheck::HealthCheckForKey(const QString &xfp)
{
    if (auto dashboard = dashBoardPtr()) {
        QJsonObject output;
        QString errormsg = "";
        bool ret {false};
        if (isUserWallet()) {
            ret = Draco::instance()->HealthCheckForKey(dashboard->wallet_id(), xfp, output, errormsg);
        }
        else {
            ret = Byzantine::instance()->HealthCheckForKey(dashboard->groupId(), dashboard->wallet_id(), xfp, output, errormsg);
        }
        if(ret){
            QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
            if (auto dummy = groupDummyTxPtr()) {
                dummy->setCurrentXfp(xfp);
                dummy->setDummyTxData(dummy_transaction);
            }
        }
        else {

        }
    }
}

void QGroupWalletHealthCheck::RequestHealthCheckForKey(const QString &xfp)
{
    if (auto dashboard = dashBoardPtr()) {
        QJsonObject output;
        QString errormsg = "";
        bool ret = Byzantine::instance()->RequestHealthCheckForKey(dashboard->groupId(), dashboard->wallet_id(), xfp, output, errormsg);
        if(ret){
            if (auto dummy = groupDummyTxPtr()) {
                dummy->setCurrentXfp(xfp);
            }
            //Request sync statsus again if need
            emit requestHealthCheckSuccess();
        }
        else {
            //Show toast ?
        }
    }
}

bool QGroupWalletHealthCheck::HealthCheckPendingForTx(const QString &dummy_transaction_id)
{
    if (auto dashboard = dashBoardPtr()) {
        QJsonObject output;
        QString errormsg = "";
        bool ret {false};
        if (isUserWallet()) {
            ret = Draco::instance()->GetDummyTransaction(dashboard->wallet_id(), dummy_transaction_id, output, errormsg);
        }
        else {
            ret = Byzantine::instance()->GetDummyTransaction(dashboard->groupId(), dashboard->wallet_id(), dummy_transaction_id, output, errormsg);
        }
        DBG_INFO << ret << output;
        if(ret){
            QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
            bool is_draft = dummy_transaction["is_draft"].toBool();
            if (is_draft) {
                QJsonObject tmp;
                if (isUserWallet()) {
                    Draco::instance()->FinalizeDummyTransaction(dashboard->wallet_id(), dummy_transaction_id, tmp, errormsg);
                }
                else {
                    Byzantine::instance()->FinalizeDummyTransaction(dashboard->groupId(), dashboard->wallet_id(), dummy_transaction_id, tmp, errormsg);
                }
            }
            else {
            }
            dummy_transaction = output["dummy_transaction"].toObject();
            if (auto dummy = groupDummyTxPtr()) {
                dummy->setDummyTxData(dummy_transaction);
            }
        }
        return ret;
    }
    return false;
}

double QGroupWalletHealthCheck::SixMonthToMillis() const
{
    qint64 sixMonths = 6;
    qint64 daysInMonth = 30;
    qint64 hoursInDay = 24;
    qint64 minutesInHour = 60;
    qint64 secondsInMinute = 60;
    qint64 millisecondsInSecond = 1000;
    return (sixMonths * daysInMonth * hoursInDay * minutesInHour * secondsInMinute * millisecondsInSecond)*1.f;
}

double QGroupWalletHealthCheck::YearToMillis() const
{
    return SixMonthToMillis() * 2.f;
}

double QGroupWalletHealthCheck::CurrentTimeToMillis() const
{
    return qUtils::GetCurrentTimeSecond() * 1000.f;
}

QString QGroupWalletHealthCheck::dateToday() const
{
    long int current_time = qUtils::GetCurrentTimeSecond();
    return QDateTime::fromTime_t(current_time).date().toString("MM/dd/yyyy");
}

void QGroupWalletHealthCheck::viewHistoryClicked(const QString &xfp)
{
    auto signer = bridge::nunchukGetMasterSignerFingerprint(xfp);
    if (signer) {
        QEventProcessor::instance()->sendEvent(E::EVT_HOME_MASTER_SIGNER_INFO_REQUEST, xfp);
    } else {
        QEventProcessor::instance()->sendEvent(E::EVT_HOME_REMOTE_SIGNER_INFO_REQUEST, xfp);
    }
}

void QGroupWalletHealthCheck::transactionSignedClicked(const QVariant &payload)
{
    QMap<QString, QVariant> maps = payload.toMap();
    QString transaction_id = maps["transaction_id"].toString();
    QString wallet_id = maps["wallet_local_id"].toString();
    if (auto wallet = AppModel::instance()->walletListPtr()->getWalletById(wallet_id)) {
        AppModel::instance()->setWalletInfo(wallet);
        QEventProcessor::instance()->sendEvent(E::EVT_HOME_TRANSACTION_INFO_REQUEST, transaction_id);
    }
}

QJsonArray QGroupWalletHealthCheck::reminders() const
{
    return m_reminders;
}

QList<QVariant> QGroupWalletHealthCheck::reminderKeys() const
{
    return m_reminderKeys;
}

void QGroupWalletHealthCheck::setReminderKeys(const QList<QVariant> &newReminderKeys)
{
    if(m_reminderKeys == newReminderKeys)
        return;
    m_reminderKeys = newReminderKeys;
    for (auto key : m_reminderKeys) {
        DBG_INFO << key;
    }
    emit reminderKeysChanged();
}

void QGroupWalletHealthCheck::GetKeyHealthReminder()
{
    QPointer<QGroupWalletHealthCheck> safeThis(this);
    runInThread([safeThis]() ->QJsonArray{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QJsonObject output;
        QString errormsg = "";
        bool ret {false};
        if (ptrLamda->isUserWallet()) {
            ret = Draco::instance()->GetKeyHealthReminder(ptrLamda->wallet_id(), output, errormsg);
        }
        else {
            ret = Byzantine::instance()->GetKeyHealthReminder(ptrLamda->groupId(), ptrLamda->wallet_id(), output, errormsg);
        }
        DBG_INFO << output;
        return output["reminders"].toArray();
    },[safeThis](QJsonArray reminders) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        ptrLamda->m_reminders = reminders;
    });
}

void QGroupWalletHealthCheck::AddOrUpdateKeyHealthReminder(const QStringList xfps, const QString frequency, const QString start_date_millis)
{
    if (auto dashboard = dashBoardPtr()) {
        QPointer<QGroupWalletHealthCheck> safeThis(this);
        runInConcurrent([safeThis, xfps, frequency, start_date_millis]() ->bool{
            SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
            QJsonObject requestBody;
            QStringList xfps_list = xfps;
            xfps_list.removeAll("");
            requestBody["xfps"] = QJsonArray::fromStringList(xfps_list);
            requestBody["frequency"] = frequency;
            long int end = qUtils::GetTimeSecond(start_date_millis);
            if (end > 0) {
                requestBody["start_date_millis"] = (double)end*1000;
            }
            else {
                requestBody["start_date_millis"] = (double)qUtils::GetCurrentTimeSecond() * 1000;
            }
            DBG_INFO << requestBody;
            QJsonObject output;
            QString errormsg = "";
            bool ret {false};
            if (ptrLamda->isUserWallet()) {
                ret = Draco::instance()->AddOrUpdateKeyHealthReminder(ptrLamda->wallet_id(), requestBody, output, errormsg);
            }
            else {
                ret = Byzantine::instance()->AddOrUpdateKeyHealthReminder(ptrLamda->groupId(), ptrLamda->wallet_id(), requestBody, output, errormsg);
            }
            return ret;
        },[safeThis, dashboard](bool ret) {
            SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
            if (ret) {
                ptrLamda->GetKeyHealthReminder();
                ptrLamda->GetStatuses();
                emit ptrLamda->isAllReminderChanged();
                dashboard->setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_REMINDER_POPULATED);
                AppModel::instance()->showToast(0, "Reminders updated", EWARNING::WarningType::SUCCESS_MSG);
            }
        });
    }
}

bool QGroupWalletHealthCheck::DeleteKeyHealthReminder(const QStringList& xfps)
{
    if (auto dashboard = dashBoardPtr()) {
        QJsonObject output;
        QString errormsg = "";
        bool ret {false};
        if (isUserWallet()) {
            ret = Draco::instance()->DeleteKeyHealthReminder(wallet_id(), xfps, output, errormsg);
        }
        else {
            ret = Byzantine::instance()->DeleteKeyHealthReminder(dashboard->groupId(), wallet_id(), xfps, output, errormsg);
        }
        if (ret) {
            DBG_INFO << ret << output;
            GetKeyHealthReminder();
            GetStatuses();
            emit isAllReminderChanged();
        }
        return ret;
    }
    return false;
}

bool QGroupWalletHealthCheck::SkipKeyHealthReminder()
{
    if (auto dashboard = dashBoardPtr()) {
        QJsonObject payload = dashboard->alertJson()["payload"].toObject();
        QString xfp = payload["xfp"].toString();
        QJsonObject output;
        QString errormsg = "";
        bool ret {false};
        if (isUserWallet()) {
            ret = Draco::instance()->SkipKeyHealthReminder(wallet_id(), xfp, output, errormsg);
        }
        else {
            ret = Byzantine::instance()->SkipKeyHealthReminder(dashboard->groupId(), wallet_id(), xfp, output, errormsg);
        }
        DBG_INFO << ret << output;
        if (ret) {
            if (auto dash = dashBoardPtr()) {
                dash->GetAlertsInfo();
            }
        }
        return ret;
    }
    return false;
}

QJsonObject QGroupWalletHealthCheck::GetReminder(const QString &xfp) const
{
    QJsonArray all_reminders = reminders();
    for (auto one_reminder : all_reminders) {
        QJsonObject obj = one_reminder.toObject();
        if (obj["xfp"].toString() == xfp) {
            return obj;
        }
    }
    return {};
}

bool QGroupWalletHealthCheck::CancelHealthCheckPending()
{
    if (auto dummy = groupDummyTxPtr()) {
        return dummy->CancelDummyTransaction();
    }
    return false;
}

QVariantList QGroupWalletHealthCheck::healthStatuses() const
{
    if (qUtils::strCompare(dashBoardPtr()->myRole(), "FACILITATOR_ADMIN")) {
        return m_healthStatuses.toVariantList();
    } else {
        QJsonArray list;
        for (auto js : m_healthStatuses) {
            auto key = js.toObject()["keyinfo"].toObject();
            if (key["ourAccount"].toBool()) {
                list.append(js);
            }
        }
        return list.toVariantList();
    }
}

QVariant QGroupWalletHealthCheck::aKeyStatus() const
{
    if (auto dashboard = dashBoardPtr()) {
        for (auto status : m_healthStatuses) {
            if (status.toObject()["xfp"].toString() == m_keyXfp) {
                return status;
            }
        }
        QJsonObject obj;
        obj["xfp"] = m_keyXfp;
        obj["lastState"] = "NotCheckedYet";
        obj["keyinfo"] = dashboard->GetSigner(m_keyXfp);
        auto reminder = GetReminder(m_keyXfp);
        if (reminder.isEmpty()) {
            obj["reminder"] = {};
        }
        else {
            obj["reminder"] = reminder;
        }
        return QVariant::fromValue(obj);
    }
    return QVariant();
}

void QGroupWalletHealthCheck::setKeyXfp(const QString &xfp)
{
    m_keyXfp = xfp;
}

bool QGroupWalletHealthCheck::isAllReminder() const
{
    for (auto status : m_healthStatuses) {
        QJsonObject obj = status.toObject();
        QJsonObject keyinfo = obj["keyinfo"].toObject();
        QString type = keyinfo["type"].toString();
        if (type == "SERVER") {
            continue;
        }
        if (obj["reminder"].toObject().isEmpty()) {
            return false;
        }
    }
    return true;
}

bool QGroupWalletHealthCheck::hasReminder() const
{
    for (auto status : m_healthStatuses) {
        QJsonObject obj = status.toObject();
        QJsonObject keyinfo = obj["keyinfo"].toObject();
        QString type = keyinfo["type"].toString();
        if (type == "SERVER") {
            continue;
        }
        if (!obj["reminder"].toObject().isEmpty()) {
            return true;
        }
    }
    return false;
}

static QStringList xfps = {};
bool QGroupWalletHealthCheck::HealthCheckAddReminderClicked(const QVariant &msg)
{
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    bool ret {true};
    if (auto dashboard = dashBoardPtr()) {
        if (type == "health-check-reminders") {
            if (reminders().size() > 0 || currentReminderState()) {
                dashboard->setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_REMINDER_POPULATED);
            } else {
                dashboard->setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_REMINDER_EMPTY_STATE);
                saveReminderState();
            }
        } else if (type == "health-check-add-reminders") {
            dashboard->setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_SELECT_KEY_WANT_TO_REMINDER);
        } else if (type == "health-check-add-reminders-continue") {
            QList<QVariant> reminderKeys = maps["checked"].toList();
            xfps.clear();
            xfps = maps["xfps"].toStringList();
            setEditReminder({});
            setReminderKeys(reminderKeys);
            dashboard->setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_FREQUENCY_REPEAT);
        } else if (type == "health-check-add-reminders-frequency-save") {
            QString frequency = maps["frequency"].toString();
            QString start_date_millis = maps["start_date"].toString();
            AddOrUpdateKeyHealthReminder(xfps, frequency, start_date_millis);
        } else if (type == "health-check-add-reminders-edit") {
            QString xfp = maps["xfp"].toString();
            DBG_INFO << xfp;
            xfps.clear();
            xfps.append(xfp);
            dashboard->setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_FREQUENCY_REPEAT);
        } else if (type == "health-check-add-reminders-no-reminder") {
            if (DeleteKeyHealthReminder(xfps)) {
                if (hasReminder()) {
                    dashboard->setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_REMINDER_POPULATED);
                    AppModel::instance()->showToast(0, "Reminders updated", EWARNING::WarningType::SUCCESS_MSG);
                } else {
                    dashboard->setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_STATUS);
                    AppModel::instance()->showToast(0, "Reminders updated", EWARNING::WarningType::SUCCESS_MSG);
                }
            }
        }
        else {
            ret = false;
        }
    } else {
        ret = false;
    }
    return ret;
}

void QGroupWalletHealthCheck::resetScreen()
{
    if (auto dashboard = dashBoardPtr()) {
        dashboard->setFlow((int)AlertEnum::E_Alert_t::MAX_ALERT);
    }
}

QString QGroupWalletHealthCheck::editReminder() const
{
    return m_editReminder;
}

void QGroupWalletHealthCheck::setEditReminder(const QString &newEditReminder)
{
    if (m_editReminder == newEditReminder)
        return;
    m_editReminder = newEditReminder;
    emit editReminderChanged();
}

void QGroupWalletHealthCheck::saveReminderState()
{
    QVariantList list = AppSetting::instance()->getReminderStates().toList();
    QJsonArray reminderState = QJsonArray::fromVariantList(list);
    QJsonObject obj;
    obj["wallet_id"] = wallet_id();
    obj["isEntry"] = true;
    reminderState.append(obj);
    AppSetting::instance()->setReminderStates(reminderState.toVariantList());
}

bool QGroupWalletHealthCheck::currentReminderState()
{
    QVariantList list = AppSetting::instance()->getReminderStates().toList();
    QJsonArray reminderState = QJsonArray::fromVariantList(list);
    for (auto state : reminderState) {
        QJsonObject obj = state.toObject();
        if (obj["wallet_id"].toString() == wallet_id()) {
            return obj["isEntry"].toBool();
        }
    }
    return false;
}

