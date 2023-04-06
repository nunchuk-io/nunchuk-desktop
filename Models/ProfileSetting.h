#ifndef PROFILESETTING_H
#define PROFILESETTING_H

#include <QObject>

class ProfileSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList currencies READ currencies NOTIFY currenciesChanged)
    Q_PROPERTY(int currencyIndex READ currencyIndex NOTIFY currencyIndexChanged)
public:
    explicit ProfileSetting(QObject *parent = nullptr);
    static ProfileSetting *instance();
    ProfileSetting(ProfileSetting &other) = delete;
    ProfileSetting(ProfileSetting const &other) = delete;
    void operator=(const ProfileSetting &other) = delete;

    QVariantList currencies() const;
    Q_INVOKABLE void createCurrencies();
    int findCurrency(const QString &currency);
    int currencyIndex() const;
    Q_INVOKABLE void setCurrency(const QString &currency);

signals:
    void currenciesChanged();
    void currencyIndexChanged();

private:
    QVariantList m_currencies;
    int m_currencyIndex = {0};
};

#endif // PROFILESETTING_H
