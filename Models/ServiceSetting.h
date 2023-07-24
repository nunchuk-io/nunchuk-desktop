#ifndef SERVICESETTING_H
#define SERVICESETTING_H

#include <QObject>
#include <QVariant>
#include "QCommonDefines.h"
#include <QJsonObject>

class ServiceSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isSubscriber                    READ isSubscriber                   WRITE setIsSubscriber           NOTIFY isSubscriberChanged)
    Q_PROPERTY(int claimInheritanceFlow             READ claimInheritanceFlow           WRITE setClaimInheritanceFlow   NOTIFY claimInheritanceFlowChanged)
    Q_PROPERTY(int claimInheritanceStatus           READ claimInheritanceStatus         WRITE setClaimInheritanceStatus NOTIFY claimInheritanceStatusChanged)
    Q_PROPERTY(QString claimInheritancePeriod       READ claimInheritancePeriod         WRITE setClaimInheritancePeriod NOTIFY claimInheritancePeriodChanged)
    Q_PROPERTY(QVariant inheritance                 READ inheritance                    NOTIFY inheritanceChanged)
    Q_PROPERTY(QVariant keyCoSigning                READ keyCoSigning                   WRITE setKeyCoSigning           NOTIFY keyCoSigningChanged)
    Q_PROPERTY(QVariantList keyCoSigningIntervals   READ keyCoSigningIntervals          CONSTANT)
    Q_PROPERTY(int assistedSize                     READ assistedSize                   CONSTANT)

    DECLARE_PROPERTY(QString, InheritanceWalletName, {}, {})
    DECLARE_PROPERTY(QString, InheritanceWalletId, {}, {})
    DECLARE_PROPERTY(int,     InheritancePlan, {}, {})
    DECLARE_PROPERTY(QString, InheritanceActivationDate, {inheritanceDataChanged();}, {})
    DECLARE_PROPERTY(QString, InheritanceNote, {inheritanceDataChanged();}, {})
    DECLARE_PROPERTY(QString, InheritanceMagic, {}, {})
    DECLARE_PROPERTY(QString, InheritancePeriod, {inheritanceDataChanged();}, {})
    DECLARE_PROPERTY(QString, InheritancePeriodId, {}, {})
    DECLARE_PROPERTY(bool,    InheritanceIsNotify, {}, {})
    DECLARE_PROPERTY(QString, InheritanceEmail, {inheritanceDataChanged();}, {})
    DECLARE_PROPERTY(QString, InheritanceSecret, {}, {})

    DECLARE_PROPERTY(int, RemainingAssistedWalletCount, {}, {})
    DECLARE_PROPERTY(QStringList, AssistedSetuped, {}, {})
    DECLARE_PROPERTY(bool, ViewInheritanceIsEdit, {}, {})
public:
    Q_ENUMS(CIWithDraw)
    enum class CIWithDraw {
        WITHDRAW_TO_NUNCHUK_WALLET,
        WITHDRAW_TO_ADDRESS
    };
    Q_ENUMS(CIStatus)
    enum class CIStatus {
        CI_NONE,
        CI_IS_PAID,
        CI_IS_NOT_PAID,
        CI_IS_EXPIRED,
        CI_IS_VALID,
        CI_IS_ERROR
    };
    Q_ENUMS(InheritanceEdit)
    enum class InheritanceEdit {
        IE_NONE,
        IE_ACTIVATION_DATE,
        IE_LEAVE_MESSAGE,
        IE_BUFFER_PERIOD,
        IE_NOTIFICATION
    };
    explicit ServiceSetting(QObject *parent = nullptr);
    static ServiceSetting *instance();
    ServiceSetting(ServiceSetting &other) = delete;
    ServiceSetting(ServiceSetting const &other) = delete;
    void operator=(const ServiceSetting &other) = delete;

    bool isSubscriber() const;
    void setIsSubscriber(bool isSubscriber);

    int claimInheritanceFlow() const;
    void setClaimInheritanceFlow(int flow);

    void checkInheritanceDownloadBackup();

    int claimInheritanceStatus() const;
    void setClaimInheritanceStatus(int status);

    QString claimInheritancePeriod() const;
    void setClaimInheritancePeriod(QString period);

    QVariant inheritance() const;

    QVariant keyCoSigning() const;
    void setKeyCoSigning(QVariant keyCoSigning);

    QVariantList keyCoSigningIntervals();

    int assistedSize() const;

    Q_INVOKABLE bool existKeyType(const QString& type);
    void inheritanceDataChanged();
signals:
    void isSubscriberChanged();
    void claimInheritanceFlowChanged();
    void claimInheritanceStatusChanged();
    void claimInheritancePeriodChanged();
    void inheritanceChanged();
    void keyCoSigningChanged();

private:
    bool m_isSubscriber {true};
    int m_claimInheritanceFlow {0};
    int m_claimInheritanceStatus {(int)CIStatus::CI_NONE};
    QString m_claimInheritancePeriod {};
    QVariant m_keyCoSigning;
    QVariantList m_keyCoSigningIntervals;
    QJsonObject mInheritancePlanBody {};
};

#endif // SERVICESETTING_H
