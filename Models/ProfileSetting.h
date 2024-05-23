#ifndef PROFILESETTING_H
#define PROFILESETTING_H

#include <QObject>
#include <QJsonArray>

class ProfileSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList currencies READ currencies NOTIFY currenciesChanged)
    Q_PROPERTY(int currencyIndex READ currencyIndex NOTIFY currencyIndexChanged)
    Q_PROPERTY(int optionIndex  READ optionIndex    WRITE setOptionIndex    NOTIFY optionIndexChanged)
    Q_PROPERTY(QVariantList mainnetServers READ mainnetServers              NOTIFY mainnetServersChanged)
    Q_PROPERTY(int mainnetIndex READ mainnetIndex WRITE setMainnetIndex     NOTIFY mainnetIndexChanged)
    Q_PROPERTY(QString newEmail READ newEmail WRITE setNewEmail                 NOTIFY newEmailChanged)
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

    int optionIndex() const;
    void setOptionIndex(int optionIndex);

    void GetMainNetServer();

    QVariantList mainnetServers() const;
    void updateMainnetServers();
    void resetDefaultMainnetServers();

    int mainnetIndex() const;
    void setMainnetIndex(int mainnetIndex);

    // For change Email
    bool CalculateRequireSignaturesForChangingEmail();
    bool ChangeEmail();
    bool ChangeEmailNone();
    QString newEmail() const;
    void setNewEmail(const QString &newNew_email);

public slots:
    bool seccurityQuestion();
    bool addMainnetServer(const QVariant &server);
    void removeMainnetServer(const QVariant &server);
    void saveMainnetServers();
    void loadMainnetServers();
signals:
    void currenciesChanged();
    void currencyIndexChanged();
    void optionIndexChanged();
    void mainnetServersChanged();
    void mainnetIndexChanged();
    void newEmailChanged();
private:
    QVariantList m_currencies;
    int m_currencyIndex {0};
    int m_optionIndex {0};
    int m_mainnetIndex {0};
    QJsonArray m_mainnetList;
    QJsonArray m_mainnetServers;
    QJsonArray m_storeMainnetServers;
    QString    m_newEmail;
};

#endif // PROFILESETTING_H
