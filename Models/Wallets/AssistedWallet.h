#ifndef ASSISTEDWALLET_H
#define ASSISTEDWALLET_H

#include <QObject>
#include "SharedWallet.h"

class AssistedWallet : public SharedWallet, public Slugs
{
    Q_OBJECT
    // Paid wallet
    Q_PROPERTY(bool         isAssistedWallet                        READ isAssistedWallet                                       NOTIFY isAssistedWalletChanged)
    Q_PROPERTY(QString      groupId                 			    READ groupId                                                NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariant     dashboardInfo                 			READ dashboardInfo                                          NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariant     serverKeyInfo                 			READ serverKeyInfo                                          NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariant     inheritancePlanInfo                 	READ inheritancePlanInfo                                    NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariant     dummyTx                                 READ dummyTx                                                CONSTANT)
    Q_PROPERTY(QVariant     recurringPayment                        READ recurringPayment                                       CONSTANT)
    Q_PROPERTY(QString      myRole                                  READ myRole                                                 NOTIFY groupInfoChanged)
    Q_PROPERTY(bool         containsColdcard                        READ containsColdcard                                       CONSTANT)
    Q_PROPERTY(int          flow                                    READ flow                                                   NOTIFY flowChanged)
    Q_PROPERTY(bool         isDeleting                              READ isDeleting                 WRITE setIsDeleting         NOTIFY isDeletingChanged)
    Q_PROPERTY(bool         isPro                                   READ isPro                                                  CONSTANT)
    Q_PROPERTY(QVariantList aliasMembers                            READ aliasMembers                                           NOTIFY aliasMembersChanged)
    Q_PROPERTY(bool         isByzantineWallet                       READ isByzantineWallet                                      CONSTANT)
    Q_PROPERTY(bool         isUserWallet                            READ isUserWallet                                           CONSTANT)
    Q_PROPERTY(bool         isGroupWallet                           READ isGroupWallet                                          CONSTANT)
    Q_PROPERTY(QVariantList ownerMembers                            READ ownerMembers                                           CONSTANT)
    Q_PROPERTY(QVariant     ownerPrimary                            READ ownerPrimary                                           CONSTANT)
    Q_PROPERTY(QString      slug                                    READ slug                                                   CONSTANT)
    Q_PROPERTY(bool         enableCreateChat                        READ enableCreateChat                                       CONSTANT)
    Q_PROPERTY(bool         isLocked                                READ isLocked                                               NOTIFY groupInfoChanged)
    Q_PROPERTY(QString      walletAliasName                         READ walletAliasName                                        NOTIFY walletChanged)
    Q_PROPERTY(bool         isHoneyBadger                           READ isHoneyBadger                                          CONSTANT)
    Q_PROPERTY(bool         isIronHand                              READ isIronHand                                             CONSTANT)

public:
    AssistedWallet(const nunchuk::Wallet &w);
    ~AssistedWallet() override = default;
    void convert(const nunchuk::Wallet w) override;
    bool isReplaced() const override;
    bool isAssistedWallet() const;
    QString groupId() const;

    QVariant dashboardInfo() const;

    QVariant serverKeyInfo() const;
    QJsonObject GetServerKeyInfo(const QString &txid);

    QVariant inheritancePlanInfo() const;

    QServerKeyPtr serverKeyPtr() const;
    QInheritancePlanPtr inheritancePlanPtr() const;
    QVariant recurringPayment() const;
    QRecurringPaymentPtr recurringPaymentPtr() const;
    QGroupWalletDummyTxPtr groupDummyTxPtr() const;
    QGroupWalletHealthCheckPtr healthPtr() const;

    bool isLocked() const;
    QWalletServicesTagPtr servicesTagPtr() const;
    QString walletAliasName() const;
    QString walletNameDisplay() override;
public:
    QStringList slugs() const final;
    QString slug() const;
    QString myRole() const;
    QString status() const;
    QGroupDashboardPtr dashboard() const;
    bool enableCreateChat();
    //Assisted
    bool isByzantineWallet();
    void GetAssistedTxs();
    void GetAssistedCancelledTxs();
    QTransactionPtr SyncAssistedTxs(const nunchuk::Transaction &tx);
    void UpdateAssistedTxs(const QString &txid, const QString &memo);
    void CancelAssistedTxs(const QString &txid);
    void CreateAsisstedTxs(const QString &txid, const QString &psbt, const QString &memo);
    void SignAsisstedTxs(const QString &tx_id, const QString &psbt, const QString &memo);
    bool RbfAsisstedTxs(const QString &tx_id, const QString &psbt);
    void UpdateWallet(const QString &name, const QString &description) override;

    bool DeleteAssistedWallet();
    bool DeleteWalletRequiredSignatures();
    void getChatInfo();

    QVariantList aliasMembers() const;
    QVariantList ownerMembers() const;
    QVariant ownerPrimary() const;

    bool GetWalletAlias();
    Q_INVOKABLE bool updateWalletAlias(const QString &nameWallet);
    Q_INVOKABLE bool deleteWalletAlias();
    Q_INVOKABLE bool updateWalletPrimaryOwner(const QString &membership_id);

    bool isPro();
    bool hasGroup();
    QVariant dummyTx() const;
protected:
    //User wallet
    void GetUserTxs();
    void GetUserCancelledTxs();
    void GetUserTxNotes();
    QString GetUserTxNote(const QString &txid);
    QTransactionPtr SyncUserTxs(const nunchuk::Transaction &tx);
    void UpdateUserTxs(const QString &txid, const QString &memo);
    void CancelUserTxs(const QString &txid);
    void CreateUserTxs(const QString &txid, const QString &psbt, const QString &memo);
    void SignUserTxs(const QString &tx_id, const QString &psbt, const QString &memo);
    void UpdateUserWallet(const QString &name, const QString &description);

    //Group Assisted wallet
    void GetGroupTxs();
    void GetGroupCancelledTxs();
    void GetGroupTxNotes();
    QString GetGroupTxNote(const QString &txid);
    QTransactionPtr SyncGroupTxs(const nunchuk::Transaction &tx);
    void UpdateGroupTxs(const QString &txid, const QString &memo);
    void CancelGroupTxs(const QString &txid);
    void CreateGroupTxs(const QString &txid, const QString &psbt, const QString &memo);
    void SignGroupTxs(const QString &tx_id, const QString &psbt, const QString &memo);
    void UpdateGroupWallet(const QString &name, const QString &description);
private:
    QWalletDummyTxPtr dummyTxPtr() const;
signals:
    void isAssistedWalletChanged();
    void groupInfoChanged();
    void aliasMembersChanged();
private:
    QJsonArray m_aliasMembers {};
};

#endif // ASSISTEDWALLET_H
