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
#include <QtCore/qreadwritelock.h>
#include "Commons/ReplaceKeyFreeUser.h"
#include "Commons/CoinsControl.h"
#include "Chats/QGroupWalletRoomModel.h"
#include "Premiums/GroupSandboxModel.h"
#include "UTXOModel.h"

class Wallet : public QStateFlow,
               public Slugs,
               public ReplaceKeyFreeUser,
               public CoinsControl
{
    Q_OBJECT
    Q_PROPERTY(QString      walletId                                READ walletId                   WRITE setWalletId           NOTIFY walletChanged)
    Q_PROPERTY(int          walletM                                 READ walletM                    WRITE setWalletM            NOTIFY walletChanged)
    Q_PROPERTY(int          walletN                                 READ walletN                    WRITE setWalletN            NOTIFY walletChanged)
    Q_PROPERTY(int          collabWalletN                           READ collabWalletN              WRITE setCollabWalletN      NOTIFY walletChanged)
    // Name
    Q_PROPERTY(QString      walletName                              READ walletNameDisplay          WRITE setWalletName         NOTIFY walletChanged)
    Q_PROPERTY(QString      walletOriginName                        READ walletName                                             NOTIFY walletChanged)
    Q_PROPERTY(QString      walletAliasName                         READ walletAliasName                                        NOTIFY walletChanged)
    // Type
    Q_PROPERTY(int          walletAddressType                       READ walletAddressType          WRITE setWalletAddressType  NOTIFY walletChanged)
    Q_PROPERTY(int          walletType                              READ walletType                 WRITE setWalletType         NOTIFY walletChanged)
    Q_PROPERTY(bool         walletEscrow                            READ walletEscrow                                           NOTIFY walletChanged)
    // Template
    Q_PROPERTY(int          walletTemplate                          READ walletTemplate             WRITE setWalletTemplate     NOTIFY walletChanged)
    Q_PROPERTY(bool         enableValuekeyset                       READ enableValuekeyset          WRITE setEnableValuekeyset  NOTIFY walletChanged)

    // Ballance
    Q_PROPERTY(QString      walletBalance                           READ balanceDisplay                                         NOTIFY walletChanged)
    Q_PROPERTY(QString      walletBalanceBTC                        READ balanceBTC                                             NOTIFY walletChanged)
    Q_PROPERTY(qint64       walletBalanceSats                       READ balanceSats                                            NOTIFY walletChanged)
    Q_PROPERTY(QString      walletBalanceCurrency                   READ balanceCurrency                                        NOTIFY walletChanged)
    // Create date
    Q_PROPERTY(QString      walletCreateDate                        READ walletCreateDateDisp                                   NOTIFY walletChanged)
    // Signer/Keys/Keyset
    Q_PROPERTY(SingleSignerListModel* walletSingleSignerAssigned    READ singleSignersAssigned                                  NOTIFY walletSignersChanged)
    // Tx history
    Q_PROPERTY(TransactionListModel*  transactionHistory            READ transactionHistory                                     NOTIFY transactionHistoryChanged)
    // Others
    Q_PROPERTY(int          walletGapLimit                 			READ walletGapLimit                                         NOTIFY walletChanged)
    Q_PROPERTY(QString      walletDescription                       READ walletDescription                                      NOTIFY walletChanged)
    Q_PROPERTY(QString      walletDescriptior                       READ descriptior                                            NOTIFY walletChanged)
    // Address
    Q_PROPERTY(QString      unUseAddress                            READ unUseAddress                                           CONSTANT)
    Q_PROPERTY(QString      walletAddress                           READ address                    WRITE setAddress            NOTIFY addressChanged)
    Q_PROPERTY(QStringList  walletusedAddressList                   READ usedAddressList                                        NOTIFY usedAddressChanged)
    Q_PROPERTY(QStringList  walletunUsedAddressList                 READ unUsedAddressList                                      NOTIFY unUsedAddressChanged)
    Q_PROPERTY(QStringList  walletusedChangedAddressList            READ usedChangeAddressList                                  NOTIFY usedChangeAddressChanged)
    Q_PROPERTY(QStringList  walletunUsedChangedAddressList          READ unUsedChangeddAddressList                              NOTIFY unUsedChangeAddressChanged)

    Q_PROPERTY(bool         capableCreate                           READ capableCreate                                          NOTIFY capableCreateChanged)
    Q_PROPERTY(int          creationMode                            READ getCreationMode                                        NOTIFY creationModeChanged)
    Q_PROPERTY(int          containsHWSigner                        READ getContainsHWSigner                                    NOTIFY containsHWSignerChanged)
    Q_PROPERTY(bool         isSharedWallet                          READ isSharedWallet                                         NOTIFY isSharedWalletChanged)
    Q_PROPERTY(bool         isAssistedWallet                        READ isAssistedWallet                                       NOTIFY isAssistedWalletChanged)
    Q_PROPERTY(bool         isArchived                              READ isArchived                 WRITE  setArchived          NOTIFY walletChanged)

    // Paid wallet
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
    Q_PROPERTY(bool         needBackup                              READ needBackup                 WRITE setNeedBackup         NOTIFY needBackupChanged)
    Q_PROPERTY(bool         keyNeedBackup                           READ keyNeedBackup              WRITE setKeyNeedBackup      NOTIFY walletChanged)
    Q_PROPERTY(QString      slug                                    READ slug                                                   CONSTANT)
    Q_PROPERTY(bool         enableCreateChat                        READ enableCreateChat                                       CONSTANT)
    Q_PROPERTY(bool         isReplaced                              READ isReplaced                                             NOTIFY groupInfoChanged)
    Q_PROPERTY(bool         isLocked                                READ isLocked                                               NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariantList signerExistList                         READ signerExistList                                        NOTIFY signerExistListChanged)
    Q_PROPERTY(QString      replaceFlow                             READ replaceFlow                                            NOTIFY replaceFlowChanged)
    Q_PROPERTY(DeviceListModel*     deviceList                      READ deviceList                                             NOTIFY deviceListChanged)
    Q_PROPERTY(QString      deviceType                              READ deviceType                                             NOTIFY deviceTypeChanged)
    Q_PROPERTY(bool         tranReplace                             READ tranReplace                                            NOTIFY tranReplaceChanged)
    Q_PROPERTY(bool         isHoneyBadger                           READ isHoneyBadger                                          CONSTANT)
    Q_PROPERTY(bool         isIronHand                              READ isIronHand                                             CONSTANT)
    Q_PROPERTY(bool         isViewCoinShow                          READ isViewCoinShow             WRITE setIsViewCoinShow     NOTIFY isViewCoinShowChanged)
    Q_PROPERTY(QString      coinFlow                                READ coinFlow                   WRITE setCoinFlow           NOTIFY coinFlowChanged)
    Q_PROPERTY(QUTXOListModel*      utxoList                        READ utxoList                                               NOTIFY utxoListChanged)
    Q_PROPERTY(UTXO*                utxoInfo                        READ utxoInfo                                               NOTIFY utxoInfoChanged)
    Q_PROPERTY(QUTXOListModel*      utxoFilterTag                   READ utxoFilterTag                                          NOTIFY utxoFilterTagChanged)
    Q_PROPERTY(QUTXOListModel*      utxoFilterCollection            READ utxoFilterCollection                                   NOTIFY utxoFilterCollectionChanged)
    Q_PROPERTY(QCoinCollectionsModel* coinCollections               READ coinCollections                                        NOTIFY coinCollectionsChanged)
    Q_PROPERTY(QCoinTagsModel*      coinTags                        READ coinTags                                               NOTIFY coinTagsChanged)
    Q_PROPERTY(QString              searchText                      READ searchText                 WRITE setSearchText         NOTIFY searchTextChanged)
    Q_PROPERTY(QUTXOListModel*      utxoListLocked                  READ utxoListLocked                                         NOTIFY utxoListChanged)
    Q_PROPERTY(QList<QUTXOListModel *> ancestryList                 READ ancestryList                                           NOTIFY ancestryListChanged)
    Q_PROPERTY(QCoinTagsModel*      coinTagsFilter                  READ coinTagsFilter                                         NOTIFY coinTagsFilterChanged)
    Q_PROPERTY(int                  walletOptType                   READ walletOptType              WRITE setWalletOptType      NOTIFY walletOptTypeChanged)

    // Group wallet (global)
    Q_PROPERTY(bool                 isGlobalGroupWallet             READ isGlobalGroupWallet                                    NOTIFY globalGroupWalletChanged)
    Q_PROPERTY(QGroupSandbox*       sandbox                         READ groupSandbox                                           NOTIFY groupSandboxChanged)
    Q_PROPERTY(QGroupMessageModel*  conversations                   READ conversations                                          NOTIFY walletChanged)
    Q_PROPERTY(int                  retentionDays                   READ retentionDays              WRITE setRetentionDays      NOTIFY walletConfigChanged)
    Q_PROPERTY(int                  unreadMessage                   READ unreadMessage                                          NOTIFY unreadMessageChanged)
    Q_PROPERTY(int                  numberOnline                    READ numberOnline                                           NOTIFY numberOnlineChanged)
    Q_PROPERTY(bool                 showbubbleChat                  READ showbubbleChat             WRITE setShowbubbleChat     NOTIFY showbubbleChatChanged)
    Q_PROPERTY(SingleSignerListModel* assignAvailableSigners        READ assignAvailableSigners                                 NOTIFY assignAvailableSignersChanged)
    Q_PROPERTY(int                  limitKeySet                     READ limitKeySet                                            NOTIFY limitKeySetChanged)
    Q_PROPERTY(bool                 isReplaceGroupWallet            READ isReplaceGroupWallet                                   CONSTANT)
    Q_PROPERTY(QVariantList         replaceGroups                   READ replaceGroups                                          NOTIFY replaceGroupsChanged)

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
    void convert(const nunchuk::Wallet w);

    QString walletId() const;
    void    setWalletId(const QString &data);

    int     walletM();
    void    setWalletM(const int data);

    int     walletN();
    void    setWalletN(const int data);

    int     collabWalletN() const;
    void    setCollabWalletN(int data);

    QString walletNameDisplay();
    QString walletName();
    QString walletAliasName() const;
    void    setWalletName(const QString &data);

    int     walletAddressType();
    void    setWalletAddressType(const int data);

    int     walletType();
    void    setWalletType(const int data);
    bool    walletEscrow() const;

    bool    enableValuekeyset();
    void    setEnableValuekeyset(bool data);
    int     walletTemplate();
    void    setWalletTemplate(const int data);

    qint64  balanceSats() const;
    QString balanceBTC() const;
    QString balanceDisplay() const;
    QString balanceCurrency() const;
    void    setWalletBalance(const qint64 data);

    QString walletCreateDateDisp() const;
    QDateTime walletCreateDate() const;

    int walletGapLimit() const;
    void setWalletGapLimit(const int data);

    void setSigners(const QSingleSignerListModelPtr &d);
    SingleSignerListModel *singleSignersAssigned();

    QString walletDescription();
    void setWalletDescription(const QString& data);

    bool isGlobalGroupWallet() const;

    QString address() const;
    void setAddress(const QString& d);
    QStringList usedAddressList() const;
    void setUsedAddressList(const QStringList& d);
    QStringList unUsedAddressList() const;
    void setunUsedAddressList(const QStringList& d);
    bool capableCreate() const;
    void setCapableCreate(bool capableCreate);
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

    bool needBackup();
    void setNeedBackup(const bool data);

    bool keyNeedBackup() const; // Hotkey ??
    void setKeyNeedBackup(const bool);

    int limitKeySet() const;

    int getCreationMode() const;
    void setCreationMode(int creationMode);
    bool getContainsHWSigner() const;
    bool isSharedWallet() const;
    void setIsSharedWallet(bool isShared);
    QString roomId() const;
    void setRoomId(const QString &roomId);
    QString initEventId() const;
    void setInitEventId(const QString &initEventId);    
    bool isAssistedWallet() const;
    bool containsColdcard();
    bool isArchived() const;
    void setArchived(bool archived);

    nunchuk::Wallet nunchukWallet() const;
    void setNunchukWallet(const nunchuk::Wallet &data);

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

    int walletOptType() const;
    void setWalletOptType(int optType);

    bool isReplaceGroupWallet() const;

    QGroupSandbox* groupSandbox();
    QGroupSandboxPtr groupSandboxPtr();
    void updateGroupSandbox(const nunchuk::GroupSandbox &value);
    void setGroupSandbox(QGroupSandboxPtr sandbox);

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
    QWalletPtr clone() const;

    // Groupwallet
    std::vector<nunchuk::SingleSigner> localSigners();
    void GetGroupWalletConfig();
    int  numberOnline();
    void setNumberOnline(int number);
    int  unreadMessage();
    void setUnreadMessage(int number);
    int  retentionDays();
    void setRetentionDays(const int days);
    QGroupMessageModel *conversations();
    bool showbubbleChat() const;
    void setShowbubbleChat(bool data);
    void markAsReadMessage(const QString&msg_id);
    Q_INVOKABLE void markFiveMessagesAsRead();
    Q_INVOKABLE void markAllMessagesAsRead();

    void CreateSignerListReviewWallet(const std::vector<nunchuk::SingleSigner> &signers);
    void CreateAssignAvailableSigners();
    SingleSignerListModel *assignAvailableSigners();
    void GetReplaceGroups();
    QVariantList replaceGroups();

private:
    QWalletDummyTxPtr dummyTxPtr() const;

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
    mutable QString m_id {""};
    mutable QString m_walletName {""};
    mutable QString m_walletDescription {""};
    mutable int     m_walletM {0};
    mutable int     m_walletN {0};
    mutable int     m_walletAddressType {(int)nunchuk::AddressType::ANY};
    mutable int     m_walletTemplate {(int)nunchuk::WalletTemplate::DEFAULT};
    mutable int     m_walletType {-1};
    mutable int     m_collabWalletN {0};
    mutable int     m_numberOnline {0};
    mutable int     m_unreadMessage {0};
    mutable bool    m_showbubbleChat{false};
    mutable bool    m_enableValuekeyset{false};

    QSingleSignerListModelPtr m_signers;
    QTransactionListModelPtr  m_transactionHistory;
    QSingleSignerListModelPtr m_assignAvailableSigners;
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
    QString m_descriptior {};
    int m_creationMode {};
    bool m_isSharedWallet {};
    QString m_roomId {};
    QString m_initEventId {};
    mutable nunchuk::Wallet m_nunchukWallet {false};
    QList<DracoUser> m_roomMembers;
    static int m_flow;
    static int m_walletOptionType;

    // Groupwallet
    QGroupSandboxPtr            m_sandbox;
    nunchuk::GroupWalletConfig  m_nunchukConfig;
    QGroupMessageModelPtr       m_conversations;
    int                         m_limitKeySet {0};
    QJsonArray                  m_replaceGroups {};
    mutable QMutex              m_mutex;
    mutable std::once_flag      m_idOnceFlag;

signals:
    void walletChanged();
    void walletSignersChanged();
    void walletConfigChanged();
    void addressChanged();
    void usedAddressChanged();
    void unUsedAddressChanged();
    void usedChangeAddressChanged();
    void unUsedChangeAddressChanged();
    void capableCreateChanged();
    void descriptiowalletNChanged();
    void transactionHistoryChanged();
    void creationModeChanged();
    void containsHWSignerChanged();
    void isSharedWalletChanged();
    void roomIdChanged();
    void initEventIdChanged();
    void isAssistedWalletChanged();
    void signalSyncCollabKeyname(QList<DracoUser> users);
    void groupInfoChanged();
    void flowChanged();
    void isDeletingChanged();
    void aliasMembersChanged();
    void signMessageChanged();
    void needBackupChanged();
    void signerExistListChanged() override;
    void replaceFlowChanged() override;
    void deviceListChanged() override;
    void deviceTypeChanged() override;
    void tranReplaceChanged() override;
    void isViewCoinShowChanged() override;
    void coinFlowChanged() override;
    void rollOverProcess(const QString& address);
    void utxoListChanged() override;
    void utxoInfoChanged() override;
    void utxoFilterTagChanged() override;
    void utxoFilterCollectionChanged() override;
    void coinCollectionsChanged() override;
    void coinTagsChanged() override;
    void searchTextChanged() override;
    void updateCollectionNameChanged(bool isError) override;
    void ancestryListChanged() override;
    void requestCreateTransaction(const QString& address) override;
    void updateTagNameChanged(bool isError) override;
    void coinTagsFilterChanged() override;
    void walletOptTypeChanged();
    void globalGroupWalletChanged();
    void unreadMessageChanged();
    void numberOnlineChanged();
    void groupSandboxChanged();
    void limitKeySetChanged();
    void showbubbleChatChanged();
    void assignAvailableSignersChanged();
    void replaceGroupsChanged();
public slots:
    void setValueKeyset(int index);
    void slotSyncCollabKeyname(QList<DracoUser> users);
    bool isValidAddress(const QString& address);
    int  reuseKeyGetCurrentIndex(const QString &xfp) override;
    QString bip32path(const QString &xfp, int index) override;
    bool updateKeyReplace(const QString &xfp, const int index) override;
    bool removeKeyReplaced(const int index);
    void requestForAllCoins(const QVariant &act);
    void requestForLockedAllCoins(const QVariant &act);
    void requestImportCoinControlData(const QString& filePath);
    void requestExportCoinControlData(const QString& filePath);
    void requestExportBIP329(const QString& filePath);
    void requestImportBIP329(const QString& filePath);
    void requestConsolidateMakeTransaction(const QVariant &msg);
    void requestSyncSelectCoinForMakeTransaction(const QVariant &msg);
    QString addressPath(const QString &address);
    QString addressBalance(const QString &address);
    bool    markAddressUsed(const QString &address);

    // Group message
    void startGetUnreadMessage();
    void startDownloadConversation();
    void startSendGroupMessage(const QString &message);

    void requestExportWalletViaBSMS(const QString &file_path);
    void requestExportWalletViaQRBCUR2Legacy();
    void requestExportWalletViaQRBCUR2();
    void requestAcceptReplaceGroup(const QString &sandbox_id);
    void requestDeclineReplaceGroup(const QString &sandbox_id);
};
typedef OurSharedPointer<Wallet> QWalletPtr;

bool sortWalletByNameAscending(const QWalletPtr &v1, const QWalletPtr &v2);
bool sortWalletByNameDescending(const QWalletPtr &v1, const QWalletPtr &v2);

class WalletListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool isContainsPremier   READ isContainsPremier                              NOTIFY containsGroupChanged)
    Q_PROPERTY(bool isContainsGroup     READ existGroupWallet                               NOTIFY containsGroupChanged)
    Q_PROPERTY(int  count               READ count                                          NOTIFY containsGroupChanged)
    Q_PROPERTY(int  unReadMessageCount  READ unReadMessageCount                             NOTIFY unReadMessageCountChanged)
    Q_PROPERTY(int  currentIndex        READ currentIndex           WRITE setCurrentIndex   NOTIFY currentIndexChanged)
    Q_PROPERTY(Wallet* currentWallet    READ currentWallet                                  NOTIFY currentIndexChanged)
public:
    WalletListModel();
    ~WalletListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QHash<int,QByteArray> roleNames() const;
    int count() const;
    int unReadMessageCount();
    Q_INVOKABLE  QVariant get(int row);
    void addWallet(const QWalletPtr &wallet);
    void replaceWallet(const QWalletPtr &wallet);
    void addSharedWallet(const QWalletPtr &wallet);
    void updateBalance(const QString &walletId, const qint64 balance);
    void updateName(const QString &walletId, const QString &value);
    void updateNunchukWallet(const QString &walletId, const nunchuk::Wallet wallet);
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
    void updateGroupMessage(const QString &wallet_id, const nunchuk::GroupMessage data);
    void updateGroupSandbox(const QString &wallet_id, const nunchuk::GroupSandbox data);
    void updateUnreadMessage(const QString &wallet_id, int number);

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
        wallet_isReplaced_Role,
        wallet_isSanboxWallet_Role,
        wallet_conversation_Role,
        wallet_unreadMessage_Role,
        wallet_numberOnline_Role,
        wallet_numberSigner_Role,
    };
    QList<QWalletPtr> fullList() const;
    void cleardata();
    bool existProWallet();
    bool existGroupWallet();
    bool isContainsPremier();
    void checkContainsGroup();

    int currentIndex() const;
    void setCurrentIndex(int newCurrentIndex);

    void startAllConversation();

    Wallet *currentWallet() const;
    QWalletPtr currentWalletPtr() const;
    void setCurrentWallet(const QWalletPtr&newCurrentWallet);

public slots:
    QVariant removeOrNot(const QString& masterFingerPrint);
    bool hasAssistedWallet() const;

private:
    QList<QWalletPtr> m_data;
    int m_currentIndex {0};
    mutable QReadWriteLock m_lock; // Can be used for multithread read

    QWalletPtr m_currentWallet {nullptr};

signals:
    void containsGroupChanged();
    void currentIndexChanged();
    void unReadMessageCountChanged();
};
typedef OurSharedPointer<WalletListModel> QWalletListModelPtr;

#endif // WALLETLISTMODEL_H
