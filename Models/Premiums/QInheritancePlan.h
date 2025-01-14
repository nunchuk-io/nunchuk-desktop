#ifndef QINHERITANCEPLAN_H
#define QINHERITANCEPLAN_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "TypeDefine.h"
#include <nunchuk.h>
#include "QBasePremium.h"
#include "Servers/DracoDefines.h"

class QInheritancePlan : public QBasePremium
{
    Q_OBJECT
    Q_PROPERTY(int actionPlan               READ actionPlan             WRITE setActionPlan         NOTIFY actionPlanChanged)
    Q_PROPERTY(QString secret               READ secret                 WRITE setSecret             NOTIFY secretChanged)

    Q_PROPERTY(QVariantList periods         READ periods                     NOTIFY periodsChanged)
    Q_PROPERTY(QVariant planInfo            READ planInfo                    NOTIFY planInfoChanged)
    Q_PROPERTY(QVariant planInfoOld         READ planInfoOld                 NOTIFY planInfoOldChanged)
    Q_PROPERTY(QVariant planInfoNew         READ planInfoNew                 NOTIFY planInfoNewChanged)
    Q_PROPERTY(QVariant planInfoUpdate      READ planInfoUpdate              NOTIFY planInfoUpdateChanged)
    Q_PROPERTY(bool isActived               READ IsActived                   NOTIFY planInfoChanged)
    Q_PROPERTY(bool isSetup                 READ isSetup                     NOTIFY planInfoChanged)

public:
    explicit QInheritancePlan(const QString &wallet_id);

    int actionPlan() const;
    void setActionPlan(int actionPlan);

    QString secret() const;
    void setSecret(QString secret);

    QVariantList periods() const;
    void setPeriods(QJsonArray periods);

    QVariant planInfo() const;
    void setPlanInfo(QJsonObject plan_info);

    QVariant planInfoOld() const;
    void setPlanInfoOld(const QJsonObject &planInfoOld);

    QVariant planInfoNew() const;
    void setPlanInfoNew(const QJsonObject &planInfoNew);

    QVariant planInfoUpdate() const;

    bool isSetup() const;
private:
    QJsonObject ConvertToDisplayQml(QJsonObject data);
public:
    void UpdateFromDummyTx(QJsonObject data);
    void GetInheritancePlan();

    QJsonObject JsBody();
    bool inheritancePlanRequiredSignatures(ReqiredSignaturesInfo &info, bool isCancel = false);
    bool RequestInheritancePlanUpdate();
    bool RequestInheritancePlanCreate();
    bool RequestInheritancePlanCancel();
    bool InheritancePlanUpdateSucceed();
    bool InheritancePlanCreateSucceed();
    bool InheritancePlanCancelSucceed();
    bool InheritancePlanCancel();
    bool IsActived() const;
    bool InheritancePlanningRequestApprove();
    bool InheritancePlanningRequestDeny();
public slots:
    bool createPeriods();
    void editPlanInfo(const QVariant& info);
signals:
    void actionPlanChanged();
    void secretChanged();
    void periodsChanged();
    void planInfoChanged();
    void planInfoNewChanged();
    void planInfoOldChanged();
    void planInfoUpdateChanged();

    void securityQuestionClosed();
    void inheritanceDummyTransactionAlert();
private:
    int m_actionPlan;
    QString m_secret;

    QJsonArray m_periods;
    double m_balance;
    QJsonObject m_planInfo;
    QJsonObject m_planInfoCurrent;
    QJsonObject m_planInfoOld;
    QJsonObject m_planInfoNew;
};

#endif // QINHERITANCEPLAN_H
