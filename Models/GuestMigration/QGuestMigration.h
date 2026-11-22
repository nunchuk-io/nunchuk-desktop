#ifndef QGUESTMIGRATION_H
#define QGUESTMIGRATION_H

#include <QObject>
#include <QString>
#include <QList>
#include <nunchuk.h>

class QGuestMigration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int walletCount READ walletCount NOTIFY statusChanged)
    Q_PROPERTY(int keyCount READ keyCount NOTIFY statusChanged)
    Q_PROPERTY(bool canMigrate READ canMigrate NOTIFY statusChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
public:
    struct Result {
        bool success {false};
        bool invalidPassphrase {false};
        int code {0};
        QString message;
    };

    static QGuestMigration *instance();

    int walletCount() const;
    int keyCount() const;
    bool canMigrate() const;
    bool busy() const;

    void markNewAccount(const QString &email);
    void markFederatedSignin();
    void prepareAccountInstance(const QString &account);
    void onAccountInstanceReady();
    bool deferOnboarding();

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void requestPrompt();
    Q_INVOKABLE void dismissPrompt();
    Q_INVOKABLE void migrate(bool keep, const QString &passphrase);

signals:
    void statusChanged();
    void busyChanged();
    void promptRequested(int wallets, int keys);
    void passphraseRequired(bool keep);
    void passphraseInvalid(bool keep);
    void migrateFinished(bool success, bool keep, int wallets, int keys, const QString &reason, const QString &message);

private:
    explicit QGuestMigration(QObject *parent = nullptr);
    void setBusy(bool busy);
    void releasePrompt(bool migrated);
    bool isAccountReady() const;
    bool guestEncrypted() const;
    QString guestBasePath() const;
    QString accountDirName(const QString &account) const;
    QList<nunchuk::Chain> guestChainsWithData() const;
    static void restoreGuestPassphrase(const std::string &storage, const QList<nunchuk::Chain> &chains, const std::string &passphrase);
    static Result runMigration(const std::string &storage, const std::string &account, bool keep,
                               bool encrypted, const std::string &passphrase, const QList<nunchuk::Chain> &chains);
    static QString failReason(const Result &result);

    int m_walletCount {0};
    int m_keyCount {0};
    bool m_canMigrate {false};
    bool m_busy {false};
    bool m_promptActive {false};
    bool m_onboardingDeferred {false};
    bool m_federatedCandidate {false};
    QString m_instanceAccount;
};

#endif
