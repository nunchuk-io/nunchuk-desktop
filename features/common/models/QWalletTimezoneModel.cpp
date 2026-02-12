#include "QWalletTimezoneModel.h"

namespace features::common::models {
QWalletTimezoneModel::QWalletTimezoneModel() {
    timezones();
}

QWalletTimezoneModel::~QWalletTimezoneModel() {}

int QWalletTimezoneModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_timezones.size();
}

QVariant QWalletTimezoneModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_timezones.size()) {
        return QVariant();
    }
    if (role == timezone_id_Role) {
        return m_timezones.at(index.row());
    } else if (role == timezone_name_Role) {
        QTimeZone tz(m_timezones.at(index.row()));
        return formatTimeZone(tz);
    } else if (role == timezone_offsetFromUtc_Role) {
        QTimeZone tz(m_timezones.at(index.row()));
        return tz.offsetFromUtc(QDateTime::currentDateTime());
    } else {
        return QVariant();
    }
}

QHash<int, QByteArray> QWalletTimezoneModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[timezone_id_Role] = "timezoneId";
    roles[timezone_name_Role] = "timezoneName";
    roles[timezone_offsetFromUtc_Role] = "timezoneOffsetFromUtc";
    return roles;
}

QString QWalletTimezoneModel::formatTimeZone(const QTimeZone &tz) const {
    if (!tz.isValid()) {
        return QStringLiteral("Unknown (GMT+00:00)");
    }

    const QString id = QString::fromUtf8(tz.id());

    // Special-case UTC to show "(UTC)" instead of "(GMT+00:00)"
    if (id == QLatin1String("Etc/UTC") || id == QLatin1String("UTC")) {
        // return QString("%1 (UTC)").arg(id);
        return QString("Etc/UTC (GMT+00:00)");
    }

    const QDateTime nowUtc = QDateTime::currentDateTimeUtc(); // use UTC to compute effective offset (incl. DST)
    const int offsetSecs = tz.offsetFromUtc(nowUtc);

    const int absSecs = qAbs(offsetSecs);
    const int hours = absSecs / 3600;
    const int minutes = (absSecs % 3600) / 60;

    const QChar sign = (offsetSecs >= 0) ? QLatin1Char('+') : QLatin1Char('-');
    const QString offsetStr = QString("%1%2:%3").arg(sign).arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0'));

    return QString("%1 (GMT%2)").arg(id, offsetStr);
}

qint64 QWalletTimezoneModel::offsetFromUtc(const QTimeZone &tz) const {
    QDateTime dt = QDateTime::currentDateTime(); // TBD get from inputs
    return static_cast<qint64>(tz.offsetFromUtc(dt));
}

void QWalletTimezoneModel::timezones() {
    if (!m_timezones.isEmpty()) {
        return; // đã có -> nhanh
    }
    m_timezones.clear();
    m_systemTimezone = QTimeZone::systemTimeZone().id();

    // compute region safely
    int slash = m_systemTimezone.indexOf('/');
    QByteArray region = (slash > 0) ? m_systemTimezone.left(slash) : QByteArray();
    QByteArray prefix;
    if (!region.isEmpty()) prefix = region + '/';

    // get all ids once
    const QList<QByteArray> allIds = QTimeZone::availableTimeZoneIds();

    // build vector of pairs (id, offset)
    struct ZoneItem { QByteArray id; int offset; };
    QVector<ZoneItem> items;
    items.reserve(allIds.size());

    QDateTime refDate = QDateTime::currentDateTime();

    for (const QByteArray &zoneId : allIds) {
        // filter by region or accept UTC/Etc
        if (!region.isEmpty()) {
            if (!(zoneId.startsWith(prefix) || zoneId == "UTC" || zoneId == "Etc/UTC" || zoneId == "Etc/GMT"))
                continue;
        }
        // compute offset once
        QTimeZone tz(zoneId);
        int offset = tz.standardTimeOffset(refDate); // costly, nhưng làm 1 lần
        items.append({zoneId, offset});
    }

    // sort by offset then id - comparator uses precomputed offsets (cheap)
    std::sort(items.begin(), items.end(), [](const ZoneItem &a, const ZoneItem &b) {
        if (a.offset == b.offset) return a.id < b.id;
        return a.offset < b.offset;
    });

    // move to m_timezones
    m_timezones.reserve(items.size());
    for (const ZoneItem &it : items) m_timezones.append(it.id);

    if (-1 == currentIndex()) {
        setSelectedTimezone(m_systemTimezone);
    }
}

void QWalletTimezoneModel::printAllTimezones() const {
    // Print all available timezones by role name
    for (const auto &id : m_timezones) {
        QTimeZone tz(id);
        QString formatted = formatTimeZone(tz);
        DBG_INFO << "Timezone ID:" << QString(id) << ", Formatted:" << formatted;
    }
}

int QWalletTimezoneModel::currentIndex() {
    if (-1 == m_currentIndex) {
        m_currentIndex = m_timezones.indexOf(m_selectedTimezone);
    }
    if (-1 == m_currentIndex) {
        m_currentIndex = m_timezones.indexOf(m_systemTimezone);
    }
    return m_currentIndex;
}

void QWalletTimezoneModel::setCurrentIndex(int newCurrentIndex) {
    if (m_currentIndex == newCurrentIndex || newCurrentIndex < 0 || newCurrentIndex >= m_timezones.size()) {
        return;
    }
    m_currentIndex = newCurrentIndex;
    if (m_currentIndex >= 0 && m_currentIndex < m_timezones.size()) {
        setSelectedTimezone(m_timezones.at(m_currentIndex));
    }
    emit currentIndexChanged();
}

QString QWalletTimezoneModel::selectedTimezone() {
    return formatTimeZone(QTimeZone(m_selectedTimezone));
}

QByteArray QWalletTimezoneModel::selectedTimezoneId() {
    return m_selectedTimezone;
}

void QWalletTimezoneModel::setSelectedTimezone(const QByteArray &timezone) {
    if (m_selectedTimezone == timezone)
        return;
    m_selectedTimezone = timezone;
    m_currentIndex = m_timezones.indexOf(m_selectedTimezone);
    DBG_INFO << QString(timezone) << m_currentIndex;
    emit currentIndexChanged();
}

QByteArray QWalletTimezoneModel::convertFormattedToTimezoneId(const QString &formatted) {
    if (formatted.isEmpty())
        return {};

    QString text = formatted.trimmed();

    // Try extract substring inside parentheses: e.g. "(UTC)" or "(GMT+00:00)"
    int open = text.indexOf('(');
    int close = text.indexOf(')');
    QString inside, outside;

    if (open >= 0 && close > open) {
        inside = text.mid(open + 1, close - open - 1).trimmed(); // e.g. "UTC" or "GMT+00:00"
        outside = text.left(open).trimmed();                     // e.g. "Etc/UTC"
    } else {
        outside = text;
    }

    // 1) If inside is an IANA timezone ID → use it
    if (!inside.isEmpty()) {
        QTimeZone tz(inside.toUtf8());
        if (tz.isValid()) {
            return tz.id();
        }
        // If inside is GMT offset such as GMT+00:00 → ignore; fall back to outside
    }

    // 2) If outside is a timezone ID → use it
    if (!outside.isEmpty()) {
        QTimeZone tz(outside.toUtf8());
        if (tz.isValid())
            return tz.id();
    }

    // 3) Special UTC normalization
    if (outside == "UTC" || outside == "Etc/UTC" || inside == "UTC") {
        if (m_timezones.contains("UTC"))
            return QByteArray("UTC");
        if (m_timezones.contains("Etc/UTC"))
            return QByteArray("Etc/UTC");
        return QByteArray("UTC");
    }

    // 4) Fallback
    return {};
}

void QWalletTimezoneModel::setSelectedTimezone(const QString &formatted) {
    QByteArray timezoneId = convertFormattedToTimezoneId(formatted);
    DBG_INFO << "formatted" << formatted << "timezoneId:" << QString(timezoneId);
    setSelectedTimezone(timezoneId);
}

QString QWalletTimezoneModel::localTimezone() {
    return formatTimeZone(QTimeZone::systemTimeZone());
}

} // namespace core::common::models
