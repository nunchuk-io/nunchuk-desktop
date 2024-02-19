#include "QGroupWalletHealthCheck.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "QGroupWalletDummyTx.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritancePlan.h"

QGroupWalletHealthCheck::QGroupWalletHealthCheck(WalletId wallet_id)
    : QBasePremium(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QGroupWalletHealthCheck::~QGroupWalletHealthCheck()
{
}

void QGroupWalletHealthCheck::GetStatuses()
{
    if (auto dashboard = dashBoardPtr()) {
        QJsonObject output;
        QString errormsg = "";
        bool ret = Byzantine::instance()->GetWalletHealthStatus(dashboard->groupId(), dashboard->wallet_id(), output, errormsg);
        if(ret){
            QJsonArray statuses = output["statuses"].toArray();
            QJsonArray result;
            for (auto obj : statuses) {
                QJsonObject status = obj.toObject();
                QJsonValue last = status["last_health_check_time_millis"];
                if(last.isNull()){
                    status["lastState"] = "NotCheckedYet";
                }
                else{
                    double last_health_check_time_millis = last.toDouble();
                    double time_distance = CurrentTimeToMillis() - last_health_check_time_millis;
                    if (time_distance < SixMonthToMillis()) {
                        // Last checked: Less than 6 months ago
                        status["lastState"] = "LessThan6months";
                    } else if (time_distance > SixMonthToMillis() && time_distance < YearToMillis()) {
                        // Last checked: More than 6 months ago
                        status["lastState"] = "MoreThan6months";
                    } else if (time_distance > YearToMillis()) {
                        // Last checked: More than 6 months ago
                        status["lastState"] = "MoreThan1year";
                    }
                }
                QString xfp = status["xfp"].toString();
                status["keyinfo"] = dashboard->GetSigner(xfp);
                result.append(status);
            }
            m_healthStatuses = result;
            emit healthStatusesChanged();
        }
        else {
            //TBD
        }
    }
}

void QGroupWalletHealthCheck::HealthCheckForKey(const QString &xfp)
{
    if (auto dashboard = dashBoardPtr()) {
        QJsonObject output;
        QString errormsg = "";
        bool ret = Byzantine::instance()->HealthCheckForKey(dashboard->groupId(), dashboard->wallet_id(), xfp, output, errormsg);
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
        bool ret = Byzantine::instance()->GetDummyTransaction(dashboard->groupId(), dashboard->wallet_id(), dummy_transaction_id, output, errormsg);
        DBG_INFO << ret << output;
        if(ret){
            QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
            bool is_draft = dummy_transaction["is_draft"].toBool();
            if (is_draft) {
                Byzantine::instance()->FinalizeDummyTransaction(dashboard->groupId(), dashboard->wallet_id(), dummy_transaction_id, output, errormsg);
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

bool QGroupWalletHealthCheck::CancelHealthCheckPending()
{
    if (auto dummy = groupDummyTxPtr()) {
        return dummy->CancelDummyTransaction();
    }
    return false;
}

QVariantList QGroupWalletHealthCheck::healthStatuses() const
{
    return m_healthStatuses.toVariantList();
}

