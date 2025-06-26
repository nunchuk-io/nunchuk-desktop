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

#ifndef BASEWALLET_H
#define BASEWALLET_H

#include <QObject>
#include <QAbstractListModel>
#include "TransactionModel.h"
#include "SingleSignerModel.h"
#include "QOutlog.h"
#include "Models/DeviceModel.h"
#include "TypeDefine.h"
#include "Commons/Slugs.h"
#include <QJsonArray>
#include <QtCore/qreadwritelock.h>
#include "Chats/QGroupWalletRoomModel.h"
#include "Commons/QStateFlow.h"

#include "UTXOModel.h"

class BaseWallet : public QStateFlow
{
    Q_OBJECT
    Q_PROPERTY(QString      walletId                                READ walletId                   WRITE setWalletId           NOTIFY walletChanged)
    Q_PROPERTY(int          walletM                                 READ walletM                    WRITE setWalletM            NOTIFY walletChanged)
    Q_PROPERTY(int          walletN                                 READ walletN                    WRITE setWalletN            NOTIFY walletChanged)

    // Name
    Q_PROPERTY(QString      walletName                              READ walletNameDisplay          WRITE setWalletName         NOTIFY walletChanged)
    Q_PROPERTY(QString      walletOriginName                        READ walletName                                             NOTIFY walletChanged)

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
    Q_PROPERTY(bool         isArchived                              READ isArchived                 WRITE setArchived           NOTIFY walletChanged)
    Q_PROPERTY(QVariantList signerExistList                         READ signerExistList                                        NOTIFY signerExistListChanged)
    Q_PROPERTY(DeviceListModel* deviceList                          READ deviceList                                             NOTIFY deviceListChanged)
    Q_PROPERTY(QString      deviceType                              READ deviceType                                             NOTIFY deviceTypeChanged)
    Q_PROPERTY(int          limitKeySet                             READ limitKeySet                                            NOTIFY limitKeySetChanged)
    Q_PROPERTY(bool         isByzantineGuardian                     READ isByzantineGuardian                                    NOTIFY isByzantineGuardianChanged)

public:
    BaseWallet(const nunchuk::Wallet &w);
    ~BaseWallet() override = default;
    enum class CreationMode : int {
        CREATE_NEW_WALLET,
        CREATE_BY_IMPORT_DB,
        CREATE_BY_IMPORT_DESCRIPTOR,
        CREATE_BY_IMPORT_CONFIGURATION,
        CREATE_BY_IMPORT_QRCODE
    };

    virtual void convert(const nunchuk::Wallet w);

    QString walletId() const;
    void    setWalletId(const QString &data);

    int     walletM();
    void    setWalletM(const int data);

    int     walletN();
    void    setWalletN(const int data);

    virtual QString walletNameDisplay();
    virtual QString walletName();

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
    QSingleSignerListModelPtr signersPtr() const;

    QString walletDescription();
    void setWalletDescription(const QString& data);

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
    int limitKeySet() const;
    int getCreationMode() const;
    void setCreationMode(int creationMode);
    bool getContainsHWSigner() const;
    bool containsColdcard();
    bool isArchived() const;
    void setArchived(bool archived);

    nunchuk::Wallet nunchukWallet() const;
    void setNunchukWallet(const nunchuk::Wallet &data);

    int flow() const;
    void setFlow(int flow);

    Q_INVOKABLE void updateSignMessage(const QString &xfp, int wallet_type);
    Q_INVOKABLE void exportBitcoinSignedMessage(const QString &xfp, const QString &file_path, int wallet_type);

    QVariantList signerExistList() const;
    void setSignerExistList(QJsonArray signerExistList);

    DeviceListModel *deviceList() const ;
    void setDeviceList(const QDeviceListModelPtr &d);
    QString deviceType() const;
    QString deviceType(const QString &xfp);
    void setDeviceType(const QString &deviceType);
    virtual void UpdateWallet(const QString &name, const QString &description);
    virtual QWalletPtr clone() const;
    std::vector<nunchuk::SingleSigner> localSigners();
    Q_INVOKABLE bool isContainKey(const QString &xfp);

    bool isByzantineGuardian();
private:
    mutable QString m_id {""};
    mutable QString m_walletName {""};
    mutable QString m_walletDescription {""};
    mutable int     m_walletM {0};
    mutable int     m_walletN {0};
    mutable int     m_walletAddressType {(int)nunchuk::AddressType::ANY};
    mutable int     m_walletTemplate {(int)nunchuk::WalletTemplate::DEFAULT};
    mutable int     m_walletType {-1};
    mutable bool    m_enableValuekeyset{false};
    QSingleSignerListModelPtr m_signers;
    QTransactionListModelPtr  m_transactionHistory;
    QDeviceListModelPtr deviceList_;
    bool m_isDeleting {false};
    // Additional member
    QString m_address;
    QStringList m_usedAddressList {};
    QStringList m_unUsedAddressList {};
    QStringList m_usedChangeAddressList {};
    QStringList m_unUsedChangedAddressList {};

    // capable to create wallet
    bool m_capableCreate {true};
    QString m_descriptior {};
    int m_creationMode {};
    mutable nunchuk::Wallet m_nunchukWallet {false};
    QJsonArray m_signerExistList {};
    QString m_deviceType {};
    int m_limitKeySet {0};
    static int m_flow;

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
    void flowChanged();
    void isDeletingChanged();
    void signMessageChanged();
    void rollOverProcess(const QString& address);
    void limitKeySetChanged();
    void signerExistListChanged();
    void deviceListChanged();
    void deviceTypeChanged();
    void isByzantineGuardianChanged();

public slots:
    void setValueKeyset(int index);
    bool isValidAddress(const QString& address);
    QString addressPath(const QString &address);
    QString addressBalance(const QString &address);
    bool    markAddressUsed(const QString &address);
};
typedef OurSharedPointer<Wallet> QWalletPtr;
#endif // BASEWALLET_H
