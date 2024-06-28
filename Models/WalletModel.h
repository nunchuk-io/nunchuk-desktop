/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

#ifndef WALLETLISTMODEL_H
#define WALLETLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "TransactionModel.h"
#include "SingleSignerModel.h"
#include "QOutlog.h"
#include "TypeDefine.h"
#include "Commons/Slugs.h"
#include <QJsonArray>

class Wallet : public QObject, public Slugs
{
    Q_OBJECT
    Q_PROPERTY(QString      walletId                                READ id                     WRITE setId             NOTIFY idChanged)
    Q_PROPERTY(int          walletM                                 READ m                      WRITE setM              NOTIFY mChanged)
    Q_PROPERTY(int          walletN                                 READ n                      WRITE setN              NOTIFY nChanged)
    Q_PROPERTY(int          walletNShared                           READ nShared                WRITE setNShared        NOTIFY nSharedChanged)
    Q_PROPERTY(QString      walletName                              READ name                   WRITE setName           NOTIFY nameChanged)
    Q_PROPERTY(QString      walletOriginName                        READ walletOriginName                               NOTIFY nameChanged)
    Q_PROPERTY(QString      walletAddressType                       READ addressType            WRITE setAddressType    NOTIFY addressTypeChanged)
    Q_PROPERTY(QString      walletBalance                           READ balanceDisplay                                 NOTIFY balanceChanged)
    Q_PROPERTY(QString      walletBalanceBTC                        READ balanceBTC                                     NOTIFY balanceChanged)
    Q_PROPERTY(QString      walletBalanceCurrency                   READ balanceCurrency                                NOTIFY balanceChanged)
    Q_PROPERTY(QString      walletCreateDate                        READ createDate                                     NOTIFY createDateChanged)
    Q_PROPERTY(bool         walletEscrow                            READ escrow                 WRITE setEscrow         NOTIFY escrowChanged)
    Q_PROPERTY(SingleSignerListModel* walletSingleSignerAssigned    READ singleSignersAssigned                          NOTIFY singleSignersAssignedChanged)
    Q_PROPERTY(TransactionListModel*  transactionHistory            READ transactionHistory                             NOTIFY transactionHistoryChanged)
    Q_PROPERTY(QString      walletAddress                           READ address                WRITE setAddress        NOTIFY addressChanged)
    Q_PROPERTY(QStringList  walletusedAddressList                   READ usedAddressList                                NOTIFY usedAddressChanged)
    Q_PROPERTY(QStringList  walletunUsedAddressList                 READ unUsedAddressList                              NOTIFY unUsedAddressChanged)
    Q_PROPERTY(QStringList  walletusedChangedAddressList            READ usedChangeAddressList                          NOTIFY usedChangeAddressChanged)
    Q_PROPERTY(QStringList  walletunUsedChangedAddressList          READ unUsedChangeddAddressList                      NOTIFY unUsedChangeAddressChanged)
    Q_PROPERTY(bool         capableCreate                           READ capableCreate                                  NOTIFY capableCreateChanged)
    Q_PROPERTY(QString      walletDescription                       READ description            WRITE setDescription    NOTIFY descriptionChanged)
    Q_PROPERTY(QString      walletDescriptior                       READ descriptior                                    NOTIFY descriptiorChanged)
    Q_PROPERTY(int          creationMode                            READ getCreationMode                                NOTIFY creationModeChanged)
    Q_PROPERTY(int          containsHWSigner                        READ getContainsHWSigner                            NOTIFY containsHWSignerChanged)
    Q_PROPERTY(bool         isSharedWallet                          READ isSharedWallet                                 NOTIFY isSharedWalletChanged)
    Q_PROPERTY(bool         isAssistedWallet                        READ isAssistedWallet                               NOTIFY isAssistedWalletChanged)
    Q_PROPERTY(int          gapLimit                 			    READ gapLimit                                       NOTIFY gapLimitChanged)
    Q_PROPERTY(QString      groupId                 			    READ groupId                                        NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariant     dashboardInfo                 			READ dashboardInfo                                  NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariant     serverKeyInfo                 			READ serverKeyInfo                                  NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariant     inheritancePlanInfo                 	READ inheritancePlanInfo                            NOTIFY groupInfoChanged)
    Q_PROPERTY(int          flow                                    READ flow                                           NOTIFY flowChanged)
    Q_PROPERTY(QVariant     dummyTx                                 READ dummyTx                                        CONSTANT)
    Q_PROPERTY(QVariant     recurringPayment                        READ recurringPayment                               CONSTANT)
    Q_PROPERTY(bool         containsColdcard                        READ containsColdcard                               CONSTANT)
    Q_PROPERTY(bool         isDeleting                              READ isDeleting             WRITE setIsDeleting     NOTIFY isDeletingChanged)
    Q_PROPERTY(bool         isPro                                   READ isPro                                          CONSTANT)
    Q_PROPERTY(QString      myRole                                  READ myRole                                         NOTIFY groupInfoChanged)
    Q_PROPERTY(QString      unUseAddress                            READ unUseAddress                                   CONSTANT)
    Q_PROPERTY(QVariantList aliasMembers                            READ aliasMembers                                   NOTIFY aliasMembersChanged)
    Q_PROPERTY(QString      aliasName                               READ aliasName                                      NOTIFY aliasNameChanged)
    Q_PROPERTY(bool         isByzantineWallet                       READ isByzantineWallet                              CONSTANT)
    Q_PROPERTY(bool         isUserWallet                            READ isUserWallet                                   CONSTANT)
    Q_PROPERTY(bool         isGroupWallet                           READ isGroupWallet                                  CONSTANT)
    Q_PROPERTY(QVariantList ownerMembers                            READ ownerMembers                                   CONSTANT)
    Q_PROPERTY(QVariant     ownerPrimary                            READ ownerPrimary                                   CONSTANT)
    Q_PROPERTY(bool         needBackup                              READ needBackup             WRITE setNeedBackup     NOTIFY needBackupChanged)
    Q_PROPERTY(QString      slug                                    READ slug                                           CONSTANT)
    Q_PROPERTY(bool         enableCreateChat                        READ enableCreateChat                               CONSTANT)
    Q_PROPERTY(bool         isReplaced                              READ isReplaced                                     NOTIFY groupInfoChanged)
    Q_PROPERTY(bool         isLocked                                READ isLocked                                       NOTIFY groupInfoChanged)
public:
    Wallet();
    Wallet(const nunchuk::Wallet &w);
    ~Wallet();
    enum class CreationMode : int {
        CREATE_NEW_WALLET,
        CREATE_BY_IMPORT_DB,
        CREATE_BY_IMPORT_DESCRIPTOR,
        CREATE_BY_IMPORT_CONFIGURATION,
        CREATE_BY_IMPORT_QRCODE
    };
    void init();
    void convert(const Wallet *w);
    void convert(const nunchuk::Wallet &w);

    QString id() const;
    int m() const;
    int n();
    QString name();
    QString walletOriginName() const;
    QString addressType() const;
    qint64 balanceSats() const;
    QString balanceBTC() const;
    QString balanceDisplay() const;
    QString balanceCurrency() const;
    QString createDate() const;
    QDateTime createDateDateTime() const;
    bool escrow() const;
    void setEscrow(const bool d);
    void setId(const QString &d);
    void setM(const int d);
    void setN(const int d);
    void setName(const QString &d);
    void setAddressType(const QString &d);
    void setBalance(const qint64 d);
    void setCreateDate(const QDateTime &d);
    void setSigners(const QSingleSignerListModelPtr &d);
    SingleSignerListModel* singleSignersAssigned();
    QString address() const;
    void setAddress(const QString& d);
    QStringList usedAddressList() const;
    void setUsedAddressList(const QStringList& d);
    QStringList unUsedAddressList() const;
    void setunUsedAddressList(const QStringList& d);
    bool capableCreate() const;
    void setCapableCreate(bool capableCreate);
    QString description() const;
    void setDescription(const QString &description);
    QStringList usedChangeAddressList() const;
    void setUsedChangeAddressList(const QStringList &usedChangeAddressList);
    QStringList unUsedChangeddAddressList() const;
    void setUnUsedChangeddAddressList(const QStringList &unUsedChangeddAddressList);
    QString descriptior() const;
    void setDescriptior(const QString &descriptior);

    QTransactionPtr getTransactionByIndex(const int index) const;
    QTransactionPtr getTransactionByTxid(const QString& txid) const;

    TransactionListModel *transactionHistory() const;
    void setTransactionHistory(const QTransactionListModelPtr &d);

    QString unUseAddress() const;

    bool isDeleting() const;
    void setIsDeleting(const bool);

    bool needBackup() const;
    void setNeedBackup(const bool);

    int getCreationMode() const;
    void setCreationMode(int creationMode);
    bool getContainsHWSigner() const;
    int nShared() const;
    void setNShared(int d);
    bool isSharedWallet() const;
    void setIsSharedWallet(bool isShared);
    QString roomId() const;
    void setRoomId(const QString &roomId);
    QString initEventId() const;
    void setInitEventId(const QString &initEventId);    
    bool isAssistedWallet() const;
    bool containsColdcard();
    int gapLimit() const;
    void setGapLimit(int gap_limit);
    nunchuk::Wallet wallet() const;

    //Collab wallet
    void syncCollabKeyname();
    QVariant dashboardInfo() const;
    QString groupId() const;
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
    void UpdateWallet(const QString &name, const QString &description);
    QJsonObject GetServerKeyInfo(const QString &txid);
    bool DeleteAssistedWallet();
    bool DeleteWalletRequiredSignatures();
    void getChatInfo();

    QVariantList aliasMembers() const;
    QString aliasName() const;
    QVariantList ownerMembers() const;
    QVariant ownerPrimary() const;

    bool GetWalletAlias();
    Q_INVOKABLE bool updateWalletAlias(const QString &nameWallet);
    Q_INVOKABLE bool deleteWalletAlias();
    Q_INVOKABLE bool updateWalletPrimaryOwner(const QString &membership_id);
    Q_INVOKABLE bool isContainKey(const QString &xfp);

    QVariant dummyTx() const;

    bool isPro();
    bool hasGroup();

    int flow() const;
    void setFlow(int flow);

    QVariant serverKeyInfo() const;
    QServerKeyPtr serverKeyPtr() const;

    QVariant inheritancePlanInfo() const;
    QInheritancePlanPtr inheritancePlanPtr() const;

    QVariant recurringPayment() const;
    QRecurringPaymentPtr recurringPaymentPtr() const;

    QGroupWalletDummyTxPtr groupDummyTxPtr() const;

    QGroupWalletHealthCheckPtr healthPtr() const;

    Q_INVOKABLE void updateSignMessage(const QString &xfp, int wallet_type);
    Q_INVOKABLE void exportBitcoinSignedMessage(const QString &xfp, const QString &file_path, int wallet_type);
    bool isReplaced() const;
    bool isLocked() const;
    QWalletServicesTagPtr servicesTagPtr() const;
private:
    QWalletDummyTxPtr dummyTxPtr() const;
protected:
    //User wallet
    void GetUserTxs();
    void GetUserCancelledTxs();
    QTransactionPtr SyncUserTxs(const nunchuk::Transaction &tx);
    void UpdateUserTxs(const QString &txid, const QString &memo);
    void CancelUserTxs(const QString &txid);
    void CreateUserTxs(const QString &txid, const QString &psbt, const QString &memo);
    void SignUserTxs(const QString &tx_id, const QString &psbt, const QString &memo);
    void UpdateUserWallet(const QString &name, const QString &description);

    //Group wallet
    void GetGroupTxs();
    void GetGroupCancelledTxs();
    QTransactionPtr SyncGroupTxs(const nunchuk::Transaction &tx);
    void UpdateGroupTxs(const QString &txid, const QString &memo);
    void CancelGroupTxs(const QString &txid);
    void CreateGroupTxs(const QString &txid, const QString &psbt, const QString &memo);
    void SignGroupTxs(const QString &tx_id, const QString &psbt, const QString &memo);
    void UpdateGroupWallet(const QString &name, const QString &description);

private:
    QString m_id {};
    int m_m {};
    int n_n {};
    int m_nShared {};
    QString m_name {};
    QString m_addressType {};
    qint64 m_balance {};
    QDateTime m_createDate {};
    bool m_escrow {};
    QSingleSignerListModelPtr m_signers;
    QTransactionListModelPtr  m_transactionHistory;
    bool m_isDeleting {false};
    // Additional member
    QString m_address;
    QStringList m_usedAddressList {};
    QStringList m_unUsedAddressList {};
    QStringList m_usedChangeAddressList {};
    QStringList m_unUsedChangedAddressList {};
    QJsonArray m_aliasMembers {};
    // capable to create wallet

    bool m_capableCreate {true};
    QString m_description {};
    QString m_descriptior {};
    int m_creationMode {};
    bool m_isSharedWallet {};
    QString m_roomId {};
    QString m_initEventId {};
    int m_gapLimit {0};
    nunchuk::Wallet m_wallet {false};
    QList<DracoUser> m_roomMembers;
    static int m_flow;
signals:
    void idChanged();
    void mChanged();
    void nChanged();
    void nameChanged();
    void addressTypeChanged();
    void balanceChanged();
    void createDateChanged();
    void escrowChanged();
    void singleSignersAssignedChanged();
    void addressChanged();
    void usedAddressChanged();
    void unUsedAddressChanged();
    void usedChangeAddressChanged();
    void unUsedChangeAddressChanged();
    void capableCreateChanged();
    void descriptionChanged();
    void descriptiorChanged();
    void transactionHistoryChanged();
    void creationModeChanged();
    void containsHWSignerChanged();
    void nSharedChanged();
    void isSharedWalletChanged();
    void roomIdChanged();
    void initEventIdChanged();
    void isAssistedWalletChanged();
    void gapLimitChanged();
    void signalSyncCollabKeyname(QList<DracoUser> users);
    void groupInfoChanged();
    void flowChanged();
    void isDeletingChanged();
    void aliasMembersChanged();
    void aliasNameChanged();
    void signMessageChanged();
    void needBackupChanged();
public slots:
    void slotSyncCollabKeyname(QList<DracoUser> users);
    bool isValidAddress(const QString& address);
};
typedef OurSharedPointer<Wallet> QWalletPtr;

bool sortWalletByNameAscending(const QWalletPtr &v1, const QWalletPtr &v2);
bool sortWalletByNameDescending(const QWalletPtr &v1, const QWalletPtr &v2);

class WalletListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool isContainsPremier   READ isContainsPremier  NOTIFY checkContainsGroup)
    Q_PROPERTY(bool isContainsGroup     READ existGroupWallet   NOTIFY checkContainsGroup)
public:
    WalletListModel();
    ~WalletListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QHash<int,QByteArray> roleNames() const;
    void addWallet(const QWalletPtr &wallet);
    void replaceWallet(const QWalletPtr &wallet);
    void addSharedWallet(const QWalletPtr &wallet);
    void updateBalance(const QString &walletId, const qint64 balance);
    void updateName(const QString &walletId, const QString &value);
    void dataUpdated(const QString &walletId);
    void updateDescription(const QString &walletId, const QString &value);
    QVariantList walletListByMasterSigner(const QString& masterSignerId);
    QVariantList walletListByFingerPrint(const QString& masterFingerPrint);
    QWalletPtr getWalletByIndex(const int index);
    QWalletPtr getWalletById(const QString& walletId);
    bool removeWallet(const QWalletPtr it);
    void removeWallet(const QString& walletId);
    void notifyUnitChanged();
    void updateSignerHealthStatus(const QString& xfp, const int status, const time_t time);
    void notifyMasterSignerDeleted(const QString& masterSignerId);
    int getWalletIndexById(const QString& walletId);
    void updateHealthCheckTime();
    void refresh();
    void requestSort(int role, int order);
    bool containsId(const QString& id);
    void updateSharedWalletById(const QString &wallet_id, const QString &room_id, const QString &init_id, const QString &name);
    void updateSignerOfRoomWallet(const QString &wallet_id, const SignerAssigned &signer);

    enum WalletRoles {
        wallet_Id_Role,
        wallet_Name_Role,
        wallet_M_Role,
        wallet_N_Role,
        wallet_AddressType_Role,
        wallet_Balance_Role,
        wallet_BalanceBTC_Role,
        wallet_BalanceCurrency_Role,
        wallet_createDate_Role,
        wallet_Escrow_Role,
        wallet_SingleSignerList_Role,
        wallet_Address_Role,
        wallet_usedAddressList_Role,
        wallet_unUsedAddressList_Role,
        wallet_isSharedWallet_Role,
        wallet_isAssistedWallet_Role,
        wallet_groupid_Role,
        wallet_dashboard_Role,
        wallet_myRole_Role,
        wallet_hasOwner_Role,
        wallet_primaryOwner_Role,
        wallet_isHotWallet_Role,
        wallet_slug_Role,
        wallet_isLocked_Role,
        wallet_isReplaced_Role
    };
    QList<QWalletPtr> fullList() const;
    void cleardata();
    bool existProWallet();
    bool existGroupWallet();
    bool isContainsPremier();
    void checkContainsGroup();
public slots:
    QVariant removeOrNot(const QString& masterFingerPrint, const QString& derivation_path = "");
    bool hasAssistedWallet() const;
private:
    QList<QWalletPtr> d_;

signals:
    void containsGroupChanged();
};
typedef OurSharedPointer<WalletListModel> QWalletListModelPtr;

#endif // WALLETLISTMODEL_H
