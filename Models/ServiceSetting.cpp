#include "ServiceSetting.h"
#include "Draco.h"
#include "nunchuckiface.h"
#include "ProfileSetting.h"
#include "Chats/QUserWallets.h"

ServiceSetting::ServiceSetting(QObject *parent) : QObject(parent)
{
    QMap<QString,QVariant> maps = m_keyCoSigning.toMap();
    maps["auto_broadcast_transaction"] = true;
    maps["hours"] = 0;
    maps["minutes"] = 0;
    maps["interval"] = "DAILY";
    maps["currency"] = "USD";
    maps["limit"] = 0;
    m_keyCoSigning = QVariant::fromValue(maps);
}

ServiceSetting *ServiceSetting::instance() {
    static ServiceSetting mInstance;
    return &mInstance;
}

bool ServiceSetting::isSubscriber() const
{
    if (ClientController::instance()->isNunchukLoggedIn()) {
        return m_isSubscriber;
    } else {
        return false;
    }
}

void ServiceSetting::setIsSubscriber(bool isSubscriber)
{
    if (m_isSubscriber == isSubscriber)
        return;

    m_isSubscriber = isSubscriber;
    emit isSubscriberChanged();
}

int ServiceSetting::claimInheritanceFlow() const
{
    return m_claimInheritanceFlow;
}

void ServiceSetting::setClaimInheritanceFlow(int flow)
{
    if (m_claimInheritanceFlow == flow)
        return;

    m_claimInheritanceFlow = flow;
    emit claimInheritanceFlowChanged();
}

int ServiceSetting::claimInheritanceStatus() const
{
    return m_claimInheritanceStatus;
}

void ServiceSetting::setClaimInheritanceStatus(int status)
{
    if (m_claimInheritanceStatus == status)
        return;

    m_claimInheritanceStatus = status;
    emit claimInheritanceStatusChanged();
}

QString ServiceSetting::claimInheritancePeriod() const
{
    return m_claimInheritancePeriod;
}

void ServiceSetting::setClaimInheritancePeriod(QString period)
{
    if (m_claimInheritancePeriod == period)
        return;

    m_claimInheritancePeriod = period;
    emit claimInheritancePeriodChanged();
}

QVariant ServiceSetting::inheritance() const
{
    static const int64_t COIN = 100000000;
    QMap<QString,QVariant> maps;
    double balance = QUserWallets::instance()->inheritance().balance;//BTC
    int balanceSats = balance * COIN;
    QLocale locale(QLocale::English);
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        maps["balance"] = locale.toString(balanceSats);//sats
    }
    else{
        maps["balance"] = locale.toString(balance, 'f', qUtils::Precision(balance));
    }
    maps["balanceSats"] = balanceSats;
    maps["balanceCurrency"] = qUtils::currencyLocale(balanceSats);
    maps["note"] = QUserWallets::instance()->inheritance().note;
    return QVariant::fromValue(maps);
}

QVariant ServiceSetting::keyCoSigning() const
{
    return m_keyCoSigning;
}

void ServiceSetting::setKeyCoSigning(QVariant keyCoSigning)
{
    QMap<QString,QVariant> maps = keyCoSigning.toMap();
    QMap<QString,QVariant> result = m_keyCoSigning.toMap();
    for (auto key: maps.keys()) {
        result.insert(key, maps[key]);
    }
    qInfo() << result;
    m_keyCoSigning = QVariant::fromValue(result);
    emit keyCoSigningChanged();
}

QVariantList ServiceSetting::keyCoSigningIntervals()
{
    if (m_keyCoSigningIntervals.isEmpty()) {
        for (auto key : { "DAILY", "WEEKLY", "MONTHLY", "YEARLY" }) {
            QMap<QString,QVariant> maps;
            maps["displayName"] = key;
            m_keyCoSigningIntervals.append(QVariant::fromValue(maps));
        }
    }
    return m_keyCoSigningIntervals;
}

int ServiceSetting::assistedSize() const
{
    return AppModel::instance()->getUserWallets().size();
}

bool ServiceSetting::existKeyType(const QString &type)
{
    for (QMasterSignerPtr ptr : AppModel::instance()->masterSignerList()->fullList()) {
        if (ptr && ptr->deviceType() == type) return true;
    }
    return false;
}

void ServiceSetting::inheritanceDataChanged()
{
    QJsonObject body = QUserWallets::instance()->inheritancePlanBody();
    if (mInheritancePlanBody != body)
    {
        mInheritancePlanBody = body;
        setViewInheritanceIsEdit(true);
    }
}
