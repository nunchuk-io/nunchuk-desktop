#ifndef CREATINGWALLET_H
#define CREATINGWALLET_H

#include <QObject>
#include "SandboxWallet.h"

class CreatingWallet : public SandboxWallet
{
    Q_OBJECT

    Q_PROPERTY(int                  walletOptType                   READ walletOptType              WRITE setWalletOptType      NOTIFY walletOptTypeChanged)
    Q_PROPERTY(SingleSignerListModel* assignAvailableSigners        READ assignAvailableSigners                                 NOTIFY assignAvailableSignersChanged)
public:
    CreatingWallet(const nunchuk::Wallet &w);
    ~CreatingWallet() override = default;

    int walletOptType() const;
    void setWalletOptType(int optType);

    void CreateSignerListReviewWallet(const std::vector<nunchuk::SingleSigner> &signers, const QList<QString>& needTopUps = {});
    void CreateAssignAvailableSigners();
    SingleSignerListModel *assignAvailableSigners();
public slots:
    bool confirmAndAssignKeysToWallet();
    void clearPassphrase(const QString& masterFingerPrint);
    void startGetXpubs(int indexSinger);
signals:
    void walletOptTypeChanged();
    void assignAvailableSignersChanged();
private:
    static int m_walletOptionType;
    QSingleSignerListModelPtr m_assignAvailableSigners;
};

#endif // CREATINGWALLET_H
