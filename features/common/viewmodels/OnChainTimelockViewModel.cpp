#include "OnChainTimelockViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"

namespace features::common::viewmodels {
using namespace core::viewmodels;
using namespace features::common::usecases;
const QString MAX_DATE = "01/19/2038";
const int DURATION = 5; // years

OnChainTimelockViewModel::OnChainTimelockViewModel(QObject *parent) 
: ActionViewModel(parent)
{
    setisInit(true);
    connect(this, &OnChainTimelockViewModel::valueDateChanged, this, &OnChainTimelockViewModel::slotTimeChanged);
    connect(this, &OnChainTimelockViewModel::valueTimeChanged, this, &OnChainTimelockViewModel::slotTimeChanged);
}

void OnChainTimelockViewModel::clearTimeLock(QJsonObject timelock) {
    DBG_INFO << timelock;
    if (timelock.isEmpty()) {
        QDateTime dt = QDateTime::currentDateTime();
        dt = dt.addYears(DURATION);
        QString formattedDate = dt.toString("MM/dd/yyyy");
        QString formattedTime = dt.toString("HH:mm");        
        setvalueTime(formattedTime);
        setbased("TIME_LOCK");
        setblockHeight(0);
        setvalueDate(formattedDate);
    } else {
        qint64 timestamp = timelock.value("value").toVariant().toULongLong();
        QString timezone = timelock.value("timezone").toString();
        QString based = timelock.value("based").toString();
        int64_t blockHeight = static_cast<int64_t>(timelock.value("block_height").toDouble());
        QDateTime dt = qUtils::convertTimestampToDateTime(timestamp, timezone);
        QString formattedDate = dt.date().toString("MM/dd/yyyy");
        QString formattedTime = dt.time().toString("HH:mm");        
        setvalueTime(formattedTime);
        setvalueTimezone(timezone);
        setbased(based);
        setblockHeight(blockHeight);
        setvalueDate(formattedDate);
    }
}

void OnChainTimelockViewModel::slotTimeChanged() {
    qint64 maxTimestamp = qUtils::convertDateTimeToTimestamp(MAX_DATE, "00:00", valueTimezone());
    qint64 selectedTimestamp = qUtils::convertDateTimeToTimestamp(valueDate(), valueTime(), valueTimezone());
    setisShowBlockHeight(selectedTimestamp >= maxTimestamp);
    int years = qUtils::fullYearsBetween(valueDate(), MAX_DATE);
    setxYearsLock(years);
    TimeLockConvertInput input;
    input.timezoneId = qUtils::extractTimeZoneId(valueTimezone());
    input.timestamp = qUtils::convertDateTimeToTimestamp(valueDate(), valueTime(), valueTimezone());
    input.isShowBlockHeight = isShowBlockHeight();
    m_timeLockConvertUC.executeAsync(input, [this](core::usecase::Result<TimeLockConvertResult> result) {
        if (result.isSuccess()) {
            auto timelock = result.value().timelock;
            clearTimeLock(timelock);
        }
        setisInit(false);
    });
}

} // namespace features::common::viewmodels
