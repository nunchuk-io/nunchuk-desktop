#ifndef QGROUPWALLETHEALTHCHECK_H
#define QGROUPWALLETHEALTHCHECK_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "TypeDefine.h"
#include "Premiums/QBasePremium.h"

class QGroupWalletHealthCheck : public QBasePremium
{
    Q_OBJECT
    Q_PROPERTY(QVariantList healthStatuses  READ healthStatuses NOTIFY healthStatusesChanged)
public:
    QGroupWalletHealthCheck(WalletId wallet_id);
    virtual ~QGroupWalletHealthCheck();

    QVariantList healthStatuses() const;

    void GetStatuses();
    void HealthCheckForKey(const QString& xfp);
    void RequestHealthCheckForKey(const QString& xfp);
    bool HealthCheckPendingForTx(const QString& dummy_transaction_id);

    bool CancelHealthCheckPending();
private:
    double SixMonthToMillis() const;
    double YearToMillis() const;
    double CurrentTimeToMillis() const;
signals:
    void healthStatusesChanged();
    void requestHealthCheckSuccess();
private:
    QString                     m_groupId {};
    QString                     m_walletId {};
    QJsonArray                  m_healthStatuses {};
};
#endif // QGROUPWALLETHEALTHCHECK_H
