#ifndef WALLET_KEYS_H
#define WALLET_KEYS_H
#include <QJsonArray>
#include <nunchuk.h>

class WalletKeys
{
public:
    bool GetHistorySignerList();
    static bool SyncSignerToServer(const nunchuk::SingleSigner &signer);
    QVariantList healthCheckHistory() const;
    void setHealthCheckHistory(QJsonArray list);
    virtual QString fingerPrint() const = 0;
    virtual void healthCheckHistoryChanged() = 0;
private:
    QJsonArray m_healthCheckHistory {};
};
#endif // WALLET_KEYS_H
