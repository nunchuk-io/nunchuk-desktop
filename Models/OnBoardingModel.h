#ifndef ONBOARDINGMODEL_H
#define ONBOARDINGMODEL_H

#include <QObject>
#include <QJsonArray>
#include <QVariant>
#include <QJsonObject>
#include <QVariantList>
#include "TypeDefine.h"

class OnBoardingModel final: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList countries READ countries NOTIFY countryListChanged)
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)
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
    QString state() const;
    void setState(const QString &newState);
    QWalletPtr ImportWalletDescriptor(const QString &file_path);
    QWalletPtr ImportWalletDB(const QString &file_path);
public slots:
    void requestBackupHotWallet();
    bool importQrHotWallet(const QStringList qrtags);
signals:
    void countryListChanged();
    void stateChanged();
private:
    OnBoardingModel();
    ~OnBoardingModel();
    QJsonArray m_countries {};
    QString    m_state {"onboarding"};
};

#endif // ONBOARDINGMODEL_H
