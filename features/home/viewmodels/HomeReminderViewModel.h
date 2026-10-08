#pragma once

#include "features/home/usecases/GetHomeReminderUseCase.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QVariantList>

namespace features::home::viewmodels {

class HomeReminderViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString reminderId READ reminderId NOTIFY reminderChanged)
    Q_PROPERTY(QString title READ title NOTIFY reminderChanged)
    Q_PROPERTY(QString description READ description NOTIFY reminderChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl NOTIFY reminderChanged)
    Q_PROPERTY(QVariantList actions READ actions NOTIFY reminderChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

  public:
    explicit HomeReminderViewModel(QObject *parent = nullptr);

    QString reminderId() const;
    QString title() const;
    QString description() const;
    QString imageUrl() const;
    QVariantList actions() const;
    bool loading() const;

  public slots:
    void fetch();
    void triggerAction(const QVariantMap &actionSnapshot);

  signals:
    void reminderChanged();
    void loadingChanged();
    void reminderReady();

  private:
    static QString currentSessionKey();
    static bool isEligibleAudience();
    static bool isValidHttpsUrl(const QString &target);
    static QJsonObject normalizeAction(const QJsonValue &value, bool primary);
    bool syncSession();
    void applyReminder(const QJsonObject &reminder);
    void clearReminder();

    features::home::usecases::GetHomeReminderUseCase m_getHomeReminderUseCase;
    QJsonObject m_reminder;
    QJsonArray m_actions;
    QString m_sessionKey;
    quint64 m_sessionGeneration{0};
    quint64 m_requestGeneration{0};
    bool m_loading{false};
    bool m_initialFetchScheduled{false};
    bool m_initialFetchGateResolved{false};
};

} // namespace features::home::viewmodels
