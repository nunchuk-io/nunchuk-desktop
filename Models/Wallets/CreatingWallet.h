#ifndef CREATINGWALLET_H
#define CREATINGWALLET_H

#include <QObject>
#include "MiniscriptWallet.h"

class CreatingWallet : public MiniscriptWallet
{
    Q_OBJECT

    Q_PROPERTY(int                  walletOptType                   READ walletOptType              WRITE setWalletOptType      NOTIFY walletOptTypeChanged)
    Q_PROPERTY(SingleSignerListModel* assignAvailableSigners        READ assignAvailableSigners                                 NOTIFY assignAvailableSignersChanged)
    Q_PROPERTY(bool         needBackup                              READ needBackup                 WRITE setNeedBackup         NOTIFY needBackupChanged)
    Q_PROPERTY(bool         needRegistered                          READ needRegistered             WRITE setNeedRegistered     NOTIFY needRegisteredChanged)
public:
    CreatingWallet(const nunchuk::Wallet &w);
    ~CreatingWallet() override = default;

    int walletOptType() const;
    void setWalletOptType(int optType);

    bool needBackup() const;
    void setNeedBackup(const bool data);

    bool needRegistered() const;
    void setNeedRegistered(const bool data);

    void CreateSignerListReviewWallet(const std::vector<nunchuk::SingleSigner> &signers, const QList<QString>& needTopUps = {});
    void CreateAssignAvailableSigners(const std::set<nunchuk::SignerType>& types = {}, const std::set<nunchuk::SignerTag>& tags = {});
    QSingleSignerListModelPtr CreateSupportedSigners(const std::set<nunchuk::SignerType>& types = {}, const std::set<nunchuk::SignerTag>& tags = {});
    SingleSignerListModel *assignAvailableSigners();
public slots:
    bool confirmAndAssignKeysToWallet();
    void clearPassphrase(const QString& masterFingerPrint);
    void startGetXpubs(int indexSinger);
    // void editBIP32Path(const QString &master_id, const QString &path);
signals:
    void walletOptTypeChanged();
    void assignAvailableSignersChanged();
    void needBackupChanged();
    void needRegisteredChanged();
private:
    static int m_walletOptionType;
    QSingleSignerListModelPtr m_assignAvailableSigners;
};

#endif // CREATINGWALLET_H
