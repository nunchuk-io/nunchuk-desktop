#include "TimeLockUseCase.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "qUtils.h"
#include "Premiums/QGroupDashboard.h"

void TimeLockUseCase::SetupTimeLock(const QVariant &datetime, bool isPutServer, std::function<void()> callback) {
    if (isPutServer) {
        QString date = m_timeLockSet.value("valueDate").toString();
        QString time = m_timeLockSet.value("valueTime").toString();
        QString timezone = m_timeLockSet.value("valueTimezone").toString();
        QString timezoneId = qUtils::extractTimeZoneId(timezone);
        QDateTime dt = QDateTime::fromString(date + " " + time, "MM/dd/yyyy HH:mm");
        QTimeZone tz(timezoneId.toUtf8());
        if (!tz.isValid()) {
            // Fallback to UTC
            dt.setTimeSpec(Qt::UTC);
        } else {
            dt.setTimeZone(tz);
        }
        qint64 timestamp = dt.toSecsSinceEpoch();
        QPointer<QGroupDashboard> safeThis(m_dashboard.data());
        runInConcurrent([safeThis, timestamp, timezoneId]() ->bool{
            SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
            QJsonObject output;
            QString error_msg = "";
            bool ret {false};
            if (ptrLamda->isUserWallet() || ptrLamda->isUserDraftWallet()) {
                ret = Draco::instance()->DraftWalletUpdateTimelock(timezoneId, timestamp, output, error_msg);
            } else {
                ret = Byzantine::instance()->DraftWalletUpdateTimelock(ptrLamda->groupId(), timezoneId, timestamp, output, error_msg);
            }
            return ret;
        },[safeThis](bool ret) {
            SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
            if (ret) {
                ptrLamda->GetDraftWalletInfo();
            }
        });
    } else {
        auto maps = datetime.toMap();
        if (!maps.isEmpty()) {
            m_timeLockSet["valueDate"] = maps["valueDate"].toString();
            m_timeLockSet["valueTime"] = maps["valueTime"].toString();
            m_timeLockSet["valueTimezone"] = maps["valueTimezone"].toString();
            if (callback) callback();
        }             
    }
}

QJsonObject TimeLockUseCase::timeLockInfo() {
    if (m_dashboard.isNull()) {
        return QJsonObject();
    }
    auto walletInfoJs = m_dashboard->walletJson();
    auto draftWalletInfoJs = m_dashboard->walletDraftJson();
    auto info = walletInfoJs.isEmpty() ? draftWalletInfoJs : walletInfoJs;
    QJsonObject timelock = info.value("timelock").toObject();
    return timelock;
}

void TimeLockUseCase::clearTimeLock(std::function<void()> callback) {
    auto wallet = m_dashboard->walletInfoPtr();
    if (!wallet) return;
    QJsonObject timelock = timeLockInfo();
    DBG_INFO << timelock;
    if (timelock.isEmpty()) {
        QDateTime dt = QDateTime::currentDateTime();
        dt = dt.addYears(5);
        QString formattedDate = dt.toString("MM/dd/yyyy");
        QString formattedTime = dt.toString("HH:mm");
        QJsonObject formatted;
        formatted["valueDate"] = formattedDate;
        formatted["valueTime"] = formattedTime;
        formatted["valueTimezone"] = wallet->timezones()->selectedTimezone();
        m_timeLockSet = formatted;
        if (callback) callback();
    } else {
        qint64 timestamp = timelock.value("value").toVariant().toULongLong();
        QString timezone = timelock.value("timezone").toString();
        QDateTime dt = qUtils::convertTimestampToDateTime(timestamp, timezone);
        wallet->timezones()->setSelectedTimezone(timezone);
        QString formattedDate = dt.date().toString("MM/dd/yyyy");
        QString formattedTime = dt.time().toString("HH:mm");
        QJsonObject formatted;
        formatted["valueDate"] = formattedDate;
        formatted["valueTime"] = formattedTime;
        formatted["valueTimezone"] = timezone;
        m_timeLockSet = formatted;
        if (callback) callback();
    }
}

QJsonObject TimeLockUseCase::timeLockSet() const {
    return m_timeLockSet;
}