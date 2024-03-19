#ifndef QServerKey_H
#define QServerKey_H

#include <QObject>
#include "TypeDefine.h"
#include <nunchuk.h>
#include "QBasePremium.h"
#include "Servers/DracoDefines.h"

class QServerKey : public QBasePremium
{
    Q_OBJECT
    Q_PROPERTY(QVariant policies        READ policies       NOTIFY policiesChanged)
    Q_PROPERTY(QVariant policiesOld     READ policiesOld    NOTIFY policiesOldChanged)
    Q_PROPERTY(QVariant policiesNew     READ policiesNew    NOTIFY policiesNewChanged)
    Q_PROPERTY(QVariant broadcastDelay  READ broadcastDelay NOTIFY policiesNewChanged)
    Q_PROPERTY(QVariantList spendingLimitChange   READ spendingLimitChange   NOTIFY spendingLimitChangeChanged)
    Q_PROPERTY(QVariantList spendingLimitCurrent  READ spendingLimitCurrent  NOTIFY spendingLimitCurrentChanged)
    Q_PROPERTY(QVariant keyCoSigning              READ keyCoSigning          NOTIFY keyCoSigningChanged)
    Q_PROPERTY(QVariant hbSpendingLimitChange  READ hbSpendingLimitChange  NOTIFY spendingLimitCurrentChanged)
public:
    explicit QServerKey(const QString &wallet_id);
    // Update
    QVariant policies() const;
    void setPolicies(QJsonObject policies);

    QVariant policiesOld() const;
    void setPoliciesOld(QJsonObject policies);

    QVariant policiesNew() const;
    void setPoliciesNew(QJsonObject policies);

    QVariant broadcastDelay() const;

    void UpdateFromDummyTx(QJsonObject data);
    void UpdateFromWallet(QJsonObject data);
    void UpdateFromServerKey(QJsonObject data);
    QVariantList spendingLimitChange() const;

    QVariantList spendingLimitCurrent() const;
    QVariant hbSpendingLimitChange() const;

    QString groupId() const;
    nunchuk::SingleSigner ServerKey();
    // For server key
    // For co-signing policies
    bool serverKeyGetCurrentPolicies();
    QJsonObject serverKeyBody();
    bool ServerKeyRequiredSignature();
    bool ServerKeyUpdate(QJsonObject &output, QString &errormsg, bool isDraft);
    bool ServerKeyUpdatePoliciesChange();
    bool ServerKeyUpdatePoliciesSucceed();

    QWalletServicesTagPtr ServiceTag() const;

    QVariant keyCoSigning() const;
private:
    QJsonObject ConvertToDisplayQml(QJsonObject data);
public slots:
    void setKeyCoSigning(const QVariant& val);
    QString  secondsToString (qint64 total_seconds);
    QVariant secondsToTime(qint64 signing_delay_seconds);
    qint64 timeToSeconds(const QVariant& time);
private:
    QWalletServicesTagPtr servicesTagPtr() const;
signals:
    void policiesChanged();
    void policiesOldChanged();
    void policiesNewChanged();
    void spendingLimitChangeChanged();
    void spendingLimitCurrentChanged();

    // Notify Qml
    void securityQuestionClosed();
    void serverKeyDummyTransactionAlert();

    void keyCoSigningChanged();

private:
    nunchuk::SingleSigner m_server_key;
    QJsonObject m_policies;
    QJsonObject m_policiesOld;
    QJsonObject m_policiesNew;
    QJsonObject m_keyCoSigning;
};

#endif // QServerKey_H
