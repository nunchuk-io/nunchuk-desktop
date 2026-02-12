#pragma once
#include "qUtils.h"
#include <QString>
#include <QDateTime>
#include <QTimeZone>

namespace qUtils {

    inline qint64 convertDateTimeToTimestamp(const QString &dateStr, const QString &timeStr, const QString &timezoneStr) {
        QString timezoneId = extractTimeZoneId(timezoneStr);
        QDateTime dt = QDateTime::fromString(dateStr + " " + timeStr, "MM/dd/yyyy HH:mm");
        QTimeZone tz(timezoneId.toUtf8());
        if (!tz.isValid()) {
            // Fallback to UTC
            dt.setTimeSpec(Qt::UTC);
        } else {
            dt.setTimeZone(tz);
        }
        return dt.toSecsSinceEpoch();
    }

    inline qint64 convertDateTimeToTimestamp(const QDateTime &dateTime, const QString &timezoneStr) {
        QString timezoneId = extractTimeZoneId(timezoneStr);
        QDateTime dt = dateTime;
        QTimeZone tz(timezoneId.toUtf8());
        if (!tz.isValid()) {
            // Fallback to UTC
            dt.setTimeSpec(Qt::UTC);
        } else {
            dt.setTimeZone(tz);
        }
        return dt.toSecsSinceEpoch();
    }

    inline int fullYearsBetween(const QString &fromDateStr, const QString &toDateStr, const QString &format = "MM/dd/yyyy") {
        QDate fromDate = QDate::fromString(fromDateStr, format);
        QDate toDate = QDate::currentDate();//fromString(toDateStr, format);
    
        if (!fromDate.isValid() || !toDate.isValid()) {
            return 0;
        }
        if (fromDate > toDate) {
            std::swap(fromDate, toDate);
        }
        int years = toDate.year() - fromDate.year();
        if (toDate.month() < fromDate.month() || (toDate.month() == fromDate.month() && toDate.day() < fromDate.day())) {
            years--;
        }
        return qMax(0, years);
    }
    
    inline QString increaseDateByYears(const QString &dateStr, int years, const QString &format = "MM/dd/yyyy") {
        QDate date = QDate::fromString(dateStr, format);
        if (!date.isValid()) {
            return QString();
        }
        QDate newDate = date.addYears(years);
        return newDate.toString(format);
    }
}
