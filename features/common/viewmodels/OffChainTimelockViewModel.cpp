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
        QString timezone_formated = qUtils::formatTimeZoneString(m_timezones->selectedTimezone());
        setvalueTimezone(timezone_formated);
        setisCurrentTime(true);
    } else {
        qint64 timestamp = timelock.value("value").toVariant().toULongLong();
        QString timezone = timelock.value("timezone").toString();
        QDateTime dt = qUtils::convertTimestampToDateTime(timestamp, timezone);
        QString formattedDate = dt.date().toString("MM/dd/yyyy");
        QString formattedTime = dt.time().toString("HH:mm");        
        setvalueTime(formattedTime);
        QString timezone_formated = qUtils::formatTimeZoneString(timezone);
        setvalueTimezone(timezone_formated);
        setvalueDate(formattedDate);
    }
}

} // namespace features::common::viewmodels
