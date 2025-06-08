#ifndef QSHAREDWALLETS_H
#define QSHAREDWALLETS_H

#include <QObject>
#include <QJsonArray>
#include <nunchuk.h>
#include <TypeDefine.h>
#include <Premiums/GroupSandboxModel.h>

#define SharedWalletsMng  QSharedWallets::instance()
class QSharedWallets : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int                  currentIndex    READ currentIndex   WRITE setCurrentIndex       NOTIFY currentIndexChanged)
    Q_PROPERTY(GroupSandboxModel*   sandboxList     READ sandboxList                                NOTIFY sandboxListChanged)

private:
    QSharedWallets();
    ~QSharedWallets();
public:
    static QSharedWallets *instance();
    QSharedWallets(QSharedWallets &other) = delete;
    QSharedWallets(QSharedWallets const &other) = delete;
    void operator=(const QSharedWallets &other) = delete;
    void clear();
    void GetAllGroups();
    void GetGroup(const QString &sandbox_id);
    bool CheckGroupConfig();

    QGroupSandboxModelPtr sandboxListPtr() const;
    GroupSandboxModel* sandboxList() const;

    int currentIndex() const;
    void setCurrentIndex(int currentIndex);
    nunchuk::GroupConfig config() const;

    QString uid();
    void GetGroupDeviceUID();

    bool RecoverSandboxWallet(const QString &file_path);
    nunchuk::GroupSandbox CreateSandboxFromRecoverWallet(const QWalletPtr& wallet);

    QStringList deprecatedWallets() const;
    void CreateDeprecatedWallets();
public slots:
    bool importQrSandboxWallet(const QStringList qrtags);
    void changeToWallet();
    bool checkSandboxWalletLimit();
signals:
    void sandboxListChanged();
    void currentIndexChanged();
    void groupWalletLimitChanged(bool isGuest);
private:
    int                     mCurrentIndex {-1};
    nunchuk::GroupConfig    m_config;
    QGroupSandboxModelPtr   m_sandboxList;
    QJsonArray              m_signerExistList;
    QString                 m_uid {""};
    QStringList             m_deprecatedWallets;
};

#endif // QSHAREDWALLETS_H
