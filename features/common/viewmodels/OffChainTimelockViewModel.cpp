#include "OffChainTimelockViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"

namespace features::common::viewmodels {
using namespace core::viewmodels;

OffChainTimelockViewModel::OffChainTimelockViewModel(QObject *parent) 
: ActionViewModel(parent),
m_timezones(QWalletTimezoneModelPtr(new QWalletTimezoneModel()))
{
}

void OffChainTimelockViewModel::clearTimeLock(QJsonObject timelock) {
    DBG_INFO << timelock;
    if (timelock.isEmpty()) {
        QDateTime dt = QDateTime::currentDateTime();
        QString formattedDate = dt.toString("MM/dd/yyyy");
        QString formattedTime = dt.toString("HH:mm");        
        setvalueTime(formattedTime);
        setvalueDate(formattedDate);
        setvalueTimezone(m_timezones->selectedTimezone());
        setisCurrentTime(true);
    } else {
        qint64 timestamp = timelock.value("value").toVariant().toULongLong();
        QString timezone = timelock.value("timezone").toString();
        QDateTime dt = qUtils::convertTimestampToDateTime(timestamp, timezone);
        QString formattedDate = dt.date().toString("MM/dd/yyyy");
        QString formattedTime = dt.time().toString("HH:mm");        
        setvalueTime(formattedTime);
        setvalueTimezone(timezone);
        setvalueDate(formattedDate);
    }
}

} // namespace features::common::viewmodels
