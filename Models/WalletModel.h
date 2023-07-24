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

class Wallet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  walletId                                    READ id                     WRITE setId             NOTIFY idChanged)
    Q_PROPERTY(int      walletM                                     READ m                      WRITE setM              NOTIFY mChanged)
    Q_PROPERTY(int      walletN                                     READ n                      WRITE setN              NOTIFY nChanged)
    Q_PROPERTY(int      walletNShared                               READ nShared                WRITE setNShared        NOTIFY nSharedChanged)
    Q_PROPERTY(QString  walletName                                  READ name                   WRITE setName           NOTIFY nameChanged)
    Q_PROPERTY(QString  walletAddressType                           READ addressType            WRITE setAddressType    NOTIFY addressTypeChanged)
    Q_PROPERTY(QString  walletBalance                               READ balanceDisplay                                 NOTIFY balanceChanged)
    Q_PROPERTY(QString  walletBalanceBTC                            READ balanceBTC                                     NOTIFY balanceChanged)
    Q_PROPERTY(QString  walletBalanceCurrency                       READ balanceCurrency                                NOTIFY balanceChanged)
    Q_PROPERTY(QString  walletCreateDate                            READ createDate                                     NOTIFY createDateChanged)
    Q_PROPERTY(bool     walletEscrow                                READ escrow                 WRITE setEscrow         NOTIFY escrowChanged)
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
    Q_PROPERTY(int gapLimit                 			      READ gapLimit            NOTIFY gapLimitChanged)
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

    void convert(const Wallet *w);
    void convert(const nunchuk::Wallet &w);

    QString id() const;
    int m() const;
    int n() ;
    QString name() const;
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
    SingleSignerListModel* singleSignersAssigned() const;
    QSingleSignerListModelPtr singleSignersAssignedPtr() const;
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
    void updateTransaction(const QString txid, const QTransactionPtr& tx);

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
    int gapLimit() const;
    void setGapLimit(int gap_limit);
    nunchuk::Wallet wallet() const;

    //Assisted
    void syncAissistedTxs();
    void syncAissistedCancelledTxs();
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
    // Additional member
    QString m_address;
    QStringList m_usedAddressList {};
    QStringList m_unUsedAddressList {};
    QStringList m_usedChangeAddressList {};
    QStringList m_unUsedChangedAddressList {};
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
};
typedef OurSharedPointer<Wallet> QWalletPtr;

bool sortWalletByNameAscending(const QWalletPtr &v1, const QWalletPtr &v2);
bool sortWalletByNameDescending(const QWalletPtr &v1, const QWalletPtr &v2);

class WalletListModel : public QAbstractListModel
{
    Q_OBJECT
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
    void updateAddress(const QString &walletId, const QStringList &used, const QStringList &unused);
    void updateName(const QString &walletId, const QString &value);
    void updateDescription(const QString &walletId, const QString &value);
    QStringList walletListByMasterSigner(const QString& masterSignerId);
    QStringList walletListByFingerPrint(const QString& masterFingerPrint);
    QWalletPtr getWalletByIndex(const int index);
    QWalletPtr getWalletById(const QString& walletId);
    bool removeWallet(const QWalletPtr it);
    void removeWallet(const QString& walletId);
    void notifyUnitChanged();
    void updateSignerHealthStatus(const QString& masterSignerId, const int status, const time_t time);
    void notifyMasterSignerDeleted(const QString& masterSignerId);
    int getWalletIndexById(const QString& walletId);
    void updateHealthCheckTime();
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
        wallet_isAssistedWallet_Role
    };
    QList<QWalletPtr> fullList() const;
    void cleardata();
private:
    QList<QWalletPtr> d_;
};
typedef OurSharedPointer<WalletListModel> QWalletListModelPtr;

#endif // WALLETLISTMODEL_H
