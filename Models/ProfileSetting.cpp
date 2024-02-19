#include "ProfileSetting.h"
#include "Servers/Draco.h"
#include "localization/STR_CPP.h"

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
    if (m_currencies.size() > 0){
        setCurrency(AppSetting::instance()->currency());
    }
    else {
        QtConcurrent::run([=]() {
            QJsonObject currencies;
            QString errormsg;
            bool ret = Draco::instance()->getCurrencies(currencies,errormsg);
            if (ret) {
                QMap<QString, QVariant> cs = currencies.toVariantMap();
                for (QMap<QString, QVariant>::const_iterator it = cs.constBegin(); it != cs.constEnd(); ++it) {
                    QMap<QString,QVariant> currency;
                    currency["currency"] = it.key();
                    currency["displayName"] = QString("%1 (%2)").arg(it.value().toString()).arg(it.key());
                    m_currencies.append(QVariant::fromValue(currency));
                }
                setCurrency(AppSetting::instance()->currency());
                emit currenciesChanged();
            }
            else {
                QMap<QString,QVariant> currency;
                currency["currency"] = "USD";
                currency["displayName"] = "United States Dollar (USD)";
                m_currencies.append(QVariant::fromValue(currency));
                setCurrency("USD");
                emit currenciesChanged();
            }
        });
    }
}

int ProfileSetting::findCurrency(const QString &currency)
{
    int i = 0;
    for (auto var : m_currencies) {
        QMap<QString,QVariant> maps = var.toMap();
        if (maps["currency"].toString() == currency) {
            return i;
        }
        i++;
    }
    return i;
}

int ProfileSetting::currencyIndex() const
{
    return m_currencyIndex;
}

void ProfileSetting::setCurrency(const QString &currency)
{
    AppSetting::instance()->setCurrency(currency);
    m_currencyIndex = findCurrency(currency);
    emit currencyIndexChanged();
    QtConcurrent::run([=]() {
        Draco::instance()->exchangeRates(currency);
    });
}

int ProfileSetting::optionIndex() const
{
    return m_optionIndex;
}

void ProfileSetting::setOptionIndex(int optionIndex)
{
    if (m_optionIndex == optionIndex)
        return;

    m_optionIndex = optionIndex;
    emit optionIndexChanged();
}

