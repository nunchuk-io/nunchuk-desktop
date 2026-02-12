#pragma once
#include <QString>
#include <QAbstractListModel>
#include <QTimeZone>
#include "QOutlog.h"

namespace features::common::models {

class QWalletTimezoneModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int          currentIndex        READ currentIndex       WRITE setCurrentIndex       NOTIFY currentIndexChanged)
    Q_PROPERTY(QString      selectedTimezone    READ selectedTimezone                               NOTIFY currentIndexChanged)
public:
    QWalletTimezoneModel();
    ~QWalletTimezoneModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString formatTimeZone(const QTimeZone &tz) const;
    qint64 offsetFromUtc(const QTimeZone &tz) const;

    void timezones();
    void printAllTimezones() const;
    enum Timezones {
        timezone_id_Role,
        timezone_name_Role,
        timezone_offsetFromUtc_Role,
    };

    int currentIndex();
    void setCurrentIndex(int newCurrentIndex);

    QString selectedTimezone();
    QByteArray selectedTimezoneId();
    void setSelectedTimezone(const QByteArray &timezone);
    QByteArray convertFormattedToTimezoneId(const QString &formatted);
    Q_INVOKABLE void setSelectedTimezone(const QString &formatted);
    QString localTimezone();
signals:
    void currentIndexChanged();

private:
    QList<QByteArray>  m_timezones;
    QByteArray         m_systemTimezone;
    QByteArray         m_selectedTimezone;
    int                m_currentIndex {-1};
};
typedef OurSharedPointer<QWalletTimezoneModel> QWalletTimezoneModelPtr;
} // namespace features::common::models