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
#include "Chats/QGroupWalletRoomModel.h"
#include "Premiums/GroupSandboxModel.h"
#include "UTXOModel.h"
#include "Wallets/CreatingWallet.h"

class Wallet : public CreatingWallet
{
    Q_OBJECT

public:
    Wallet();
    Wallet(const nunchuk::Wallet &w);
    ~Wallet();
};
typedef OurSharedPointer<Wallet> QWalletPtr;

bool sortWalletByNameAscending(const QWalletPtr &v1, const QWalletPtr &v2);
bool sortWalletByNameDescending(const QWalletPtr &v1, const QWalletPtr &v2);
bool sortLastTimestamp(const QWalletPtr &v1, const QWalletPtr &v2);

class WalletListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool isContainsPremier   READ isContainsPremier                              NOTIFY containsGroupChanged)
    Q_PROPERTY(bool isContainsGroup     READ existGroupWallet                               NOTIFY containsGroupChanged)
    Q_PROPERTY(int  count               READ count                                          NOTIFY walletCountChanged)
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
    static QHash<int,QByteArray> roleWalletNames();
    static QVariant dataWallet(const QWalletPtr &data, int role);
    static QVariant useQml(const QWalletPtr &data);
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
    Q_INVOKABLE int getWalletIndexById(const QString& walletId);
    void updateHealthCheckTime();
    void refresh();
    void requestSort();
    void requestSortLastTimestamp();
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
        wallet_archived_Role,
        wallet_walletType_Role,
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

    QStringList getOrderedWalletIds() const;
    void saveOrderWalletIds();

public slots:
    QVariant removeOrNot(const QString& masterFingerPrint);
    bool hasAssistedWallet() const;
    bool swapWallets(const int from, const int to);
    void slotsMoveFinished(const QString &oldWalletId);
private:
    QList<QWalletPtr> m_data;
    int m_currentIndex {0};
    mutable QReadWriteLock m_lock; // Can be used for multithread read

    QWalletPtr m_currentWallet {nullptr};

signals:
    void walletCountChanged();
    void containsGroupChanged();
    void currentIndexChanged();
    void unReadMessageCountChanged();
    void refreshWalletList();
};
typedef OurSharedPointer<WalletListModel> QWalletListModelPtr;

#endif // WALLETLISTMODEL_H
