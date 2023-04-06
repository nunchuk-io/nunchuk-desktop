#include "ProfileSetting.h"
#include "Draco.h"

ProfileSetting::ProfileSetting(QObject *parent) : QObject(parent)
{
    m_currencies.clear();
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

ProfileSetting *ProfileSetting::instance() {
    static ProfileSetting mInstance;
    return &mInstance;
}

QVariantList ProfileSetting::currencies() const
{
    return m_currencies;
}

void ProfileSetting::createCurrencies()
{
    if (m_currencies.size() > 0) return;
    QJsonObject currencies;
    QString errormsg;
    bool ret = Draco::instance()->getCurrencies(currencies,errormsg);
    if (!ret) {
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        errormsg);
    } else {
        QMap<QString,QVariant> cs = currencies.toVariantMap();
        QMap<QString,QVariant>::const_iterator i = cs.constBegin();
        m_currencies.clear();
        while (i != cs.constEnd()) {
            QMap<QString,QVariant> maps;
            maps["currency"] = i.key();
            maps["displayName"] = QString("%1 (%2)").arg(i.value().toString()).arg(i.key());
            m_currencies.append(QVariant::fromValue(maps));
            ++i;
        }
        emit currenciesChanged();
    }
    DBG_INFO << m_currencies.size();
}

int ProfileSetting::findCurrency(const QString &currency)
{
    int i = 0;
    for (auto var : m_currencies) {
        QMap<QString,QVariant> maps = var.toMap();
        if (maps["currency"].toString() == currency) {
            return i;
        }
        ++i;
    }
    return i;
}

int ProfileSetting::currencyIndex() const
{
    return m_currencyIndex;
}

void ProfileSetting::setCurrency(const QString &currency)
{
    DBG_INFO << currency;
    AppSetting::instance()->setCurrency(currency);
    m_currencyIndex = findCurrency(currency);
    emit currencyIndexChanged();
    Draco::instance()->exchangeRates(currency);
}

