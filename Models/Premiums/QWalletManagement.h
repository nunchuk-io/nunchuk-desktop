#ifndef QWALLETMANAGEMENT_H
#define QWALLETMANAGEMENT_H

#include <QObject>
#include <QJsonArray>
#include "QSwitchAPI.h"
#include "TypeDefine.h"
#include <nunchuk.h>

#define WalletsMng  QWalletManagement::instance()
class QWalletManagement : public QObject
{
    Q_OBJECT
public:
    static QWalletManagement *instance();
    explicit QWalletManagement();
    virtual ~QWalletManagement();
    void GetListWallet(int mode);
    void UpdateSyncWalletFlows();
    QString UpdateSyncWalletFlows(bool yes, bool no);
    WalletIdList wallets() const;
    GroupIdList groupIds() const;
    GroupId groupId(WalletId wallet_id) const;
    WalletId walletId(GroupId group_id) const;
    QString slugInfo(WalletId wallet_id) const;
    QJsonObject walletInfo(WalletId wallet_id) const;
    void clear();

    template<typename T>
    bool contains(WalletId wallet_id);
    template<typename T>
    T data(WalletId wallet_id);
    template<typename T>
    void CreateData(WalletId wallet_id);

    void initSignInWallet(WalletId wallet_id);
    void FactoryWorker(WalletId wallet_id, GroupId group_id);
    int activeSize() const;
public slots:
    void slotGetListWalletFinish();
signals:
    void getListWalletFinish();
private:
    QMap<QString, QString>      mWallets;
    QMap<QString, QJsonObject>  mWalletsInfo;
    QList<QGroupDashboardPtr>   mActivedWallets;
    QList<QServerKeyPtr>        mServerKeys;
    QList<QInheritancePlanPtr>  mInheritancePlans;
    QList<QWalletDummyTxPtr>    mDummyTxs;
    QList<QGroupWalletHealthCheckPtr> mHealths;
    QList<QRecurringPaymentPtr> mRecurringPayments;
    QJsonArray                  walletArray;
    QJsonObject                 software_signer;
    nunchuk::SingleSigner       m_local_signer;
};

#endif // QWALLETMANAGEMENT_H
