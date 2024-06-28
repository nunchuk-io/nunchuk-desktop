#include "ISigner.h"
#include "Servers/Draco.h"

bool ISigner::GetHistorySignerList()
{
    QJsonObject output;
    QString errormsg = "";
    bool ret {false};
    ret = Draco::instance()->GetHistorySignerList(fingerPrint(), output, errormsg);
    DBG_INFO << ret << output;
    if(ret){
        QJsonArray histories = output["history"].toArray();
        QJsonArray v_histories;
        for (auto js : histories) {
            QJsonObject history = js.toObject();
            long int created_time_millis = static_cast<long int>(history.value("created_time_millis").toDouble()/1000);
            QDateTime date_time = QDateTime::fromTime_t(created_time_millis);
            history["created_time_millis"] = QString("%1")
                                                 .arg(date_time.date().toString("MMM dd,yyyy"));
            v_histories.append(history);
        }
        setHealthCheckHistory(v_histories);
    }
    return ret;
}

QVariantList ISigner::healthCheckHistory() const
{
    return m_healthCheckHistory.toVariantList();
}

void ISigner::setHealthCheckHistory(QJsonArray list)
{
    if (m_healthCheckHistory == list)
        return;
    m_healthCheckHistory = list;
    healthCheckHistoryChanged();
}

#include "QOutlog.h"
