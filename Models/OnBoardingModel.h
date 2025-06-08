#ifndef ONBOARDINGMODEL_H
#define ONBOARDINGMODEL_H

#include <QObject>
#include <QJsonArray>
#include <QVariant>
#include <QJsonObject>
#include <QVariantList>
#include "TypeDefine.h"
#include "Commons/QStateFlow.h"

class OnBoardingModel final: public QStateFlow
{
    Q_OBJECT
    Q_PROPERTY(QVariantList countries READ countries NOTIFY countryListChanged)
public:
    static OnBoardingModel *instance();


    OnBoardingModel(OnBoardingModel &other) = delete;
    OnBoardingModel(OnBoardingModel const &other) = delete;
    void operator=(const OnBoardingModel &other) = delete;

    QVariantList countries() const;
    void setCountries(const QJsonArray& countries);

    bool GetCountryCodeList();
    bool RequestOnboardingNoAdvisor(const QString &country_code, const QString &email, const QString &note);
    QWalletPtr CreateAHotWallet(const QString &mnemonic = "", bool need_backup = true);
    bool ImportWalletDescriptor(const QString &file_path);
    QWalletPtr ImportWalletDB(const QString &file_path);
public slots:
    void requestBackupHotWallet();
    bool importQrHotWallet(const QStringList qrtags);
signals:
    void countryListChanged();
private:
    OnBoardingModel();
    ~OnBoardingModel();
    QJsonArray m_countries {};
};

#endif // ONBOARDINGMODEL_H
