#ifndef QGROUPWALLETHEALTHCHECK_H
#define QGROUPWALLETHEALTHCHECK_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <functional>
#include "TypeDefine.h"
#include "Premiums/QBasePremium.h"

class QGroupWalletHealthCheck : public QBasePremium
{
    Q_OBJECT
    Q_PROPERTY(QVariantList healthStatuses  READ healthStatuses NOTIFY healthStatusesChanged)
    Q_PROPERTY(QVariant aKeyStatus          READ aKeyStatus     NOTIFY healthStatusesChanged)
    Q_PROPERTY(QVariantList claimKeys       READ claimKeys      NOTIFY healthStatusesChanged)
    Q_PROPERTY(QList<QVariant> reminderKeys READ reminderKeys   NOTIFY reminderKeysChanged)
    Q_PROPERTY(bool isAllReminder           READ isAllReminder  NOTIFY isAllReminderChanged)
    Q_PROPERTY(QString editReminder         READ editReminder   WRITE setEditReminder NOTIFY editReminderChanged)
public:
    QGroupWalletHealthCheck(WalletId wallet_id);
    virtual ~QGroupWalletHealthCheck();
    QString groupId() const;
    QVariantList healthStatuses() const;
    QVariantList claimKeys() const;

    void GetStatuses();
    void HealthCheckForKey(const QString& xfp);
    void RequestHealthCheckForKey(const QString& xfp);
    bool HealthCheckPendingForTx(const QString& dummy_transaction_id, std::function<void(bool)> callback = nullptr);

    bool CancelHealthCheckPending();
    QVariant aKeyStatus() const;
    void setKeyXfp(const QString& xfp);
    QList<QVariant> reminderKeys() const;
    void setReminderKeys(const QList<QVariant> &newReminderKeys);

    // onComplete (optional) is called on the UI thread immediately after m_reminders
    // has been updated and healthStatusesChanged has been emitted.  Use this to
    // sequence work that must see the fresh reminder data (e.g. setFlow so the Loader
    // creates _keyInfo only after the badge data is ready).
    void GetKeyHealthReminder(std::function<void()> onComplete = nullptr);
    void AddOrUpdateKeyHealthReminder(const QStringList xfps, const QString frequency, const QString start_date_millis);
    bool DeleteKeyHealthReminder(const QStringList& xfps);
    bool SkipKeyHealthReminder();
    QJsonObject GetReminder(const QString& xfp) const;

    QJsonArray reminders() const;

    bool isAllReminder() const;
    bool hasReminder() const;

    bool HealthCheckAddReminderClicked(const QVariant& msg);
    QString editReminder() const;
    void setEditReminder(const QString &newEditReminder);

    void saveReminderState();
    bool currentReminderState();
    void refreshKeyInfo();
private:
    double SixMonthToMillis() const;
    double YearToMillis() const;
    double CurrentTimeToMillis() const;
public slots:
    void resetScreen();
    QString dateToday() const;
    void viewHistoryClicked(const QString& xfp);
    void transactionSignedClicked(const QVariant& payload);
signals:
    void healthStatusesChanged();
    void requestHealthCheckSuccess();
    void reminderKeysChanged();
    void isAllReminderChanged();
    void editReminderChanged();

private:
    QString                     m_groupId {};
    QString                     m_walletId {};
    QJsonArray                  m_healthStatuses {};
    QString                     m_keyXfp {};
    QList<QVariant>             m_reminderKeys {};
    QJsonArray                  m_reminders {};
    bool                        m_isAllReminder {false};
    QString                     m_editReminder {};
    QJsonArray                  m_reminderState {};
};
#endif // QGROUPWALLETHEALTHCHECK_H
