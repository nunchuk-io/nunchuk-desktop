#ifndef WALLETLISTMODEL_H
#define WALLETLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "TransactionModel.h"
#include "SingleSignerModel.h"
#include "QWarningMessage.h"
#include "QOutlog.h"

class Wallet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString walletId READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int walletM      READ m  WRITE setM  NOTIFY mChanged)
    Q_PROPERTY(int walletN      READ n  WRITE setN  NOTIFY nChanged)
    Q_PROPERTY(QString walletName           READ name           WRITE setName               NOTIFY nameChanged)
    Q_PROPERTY(QString walletAddressType    READ addressType    WRITE setAddressType        NOTIFY addressTypeChanged)
    Q_PROPERTY(QString walletBalance        READ balanceDisplay                             NOTIFY balanceChanged)
    Q_PROPERTY(QString walletCreateDate     READ createDate                                 NOTIFY createDateChanged)
    Q_PROPERTY(bool walletEscrow            READ escrow         WRITE setEscrow             NOTIFY escrowChanged)
    Q_PROPERTY(SingleSignerListModel*   walletSingleSignerAssigned  READ singleSignersAssigned  NOTIFY singleSignersAssignedChanged)
    Q_PROPERTY(TransactionListModel*    transactionHistory          READ transactionHistory     NOTIFY transactionHistoryChanged)
    Q_PROPERTY(QString walletAddress        READ address        WRITE setAddress        NOTIFY addressChanged)
    Q_PROPERTY(QStringList walletusedAddressList    READ usedAddressList    NOTIFY usedAddressChanged)
    Q_PROPERTY(QStringList walletunUsedAddressList  READ unUsedAddressList  NOTIFY unUsedAddressChanged)
    Q_PROPERTY(QStringList walletusedChangedAddressList    READ usedChangeAddressList       NOTIFY usedChangeAddressChanged)
    Q_PROPERTY(QStringList walletunUsedChangedAddressList  READ unUsedChangeddAddressList   NOTIFY unUsedChangeAddressChanged)
    Q_PROPERTY(bool capableCreate           READ capableCreate  WRITE setCapableCreate  NOTIFY capableCreateChanged)
    Q_PROPERTY(QString walletDescription    READ description    WRITE setDescription    NOTIFY descriptionChanged)
    Q_PROPERTY(QString walletDescriptior    READ descriptior                            NOTIFY descriptiorChanged)
    Q_PROPERTY(QWarningMessage* warningMessage  READ warningMessage NOTIFY warningMessageChanged)
    Q_PROPERTY(int creationMode             READ getCreationMode    NOTIFY creationModeChanged)
    Q_PROPERTY(int containsHWSigner         READ getContainsHWSigner    NOTIFY containsHWSignerChanged)

public:
    Wallet();
    Wallet(const QString &pr_id,
           const int pr_m,
           const int pr_n,
           const QString &pr_name,
           const QString &pr_addrType,
           const qint64 pr_balance,
           const QDateTime &pr_createDate,
           const bool pr_escrow,
           const QSharedPointer<SingleSignerListModel> &pr_signers,
           const QString &pr_description);
    ~Wallet();

    QString id() const;
    int m() const;
    int n() ;
    QString name() const;
    QString addressType() const;
    qint64 balanceSats() const;
    QString balanceBTC() const;
    QString balanceDisplay() const;

    QString createDate() const;
    QDateTime createDateDateTime() const;
    bool escrow() const;
    void setEscrow(const bool d);

    void setId(const QString &d);
    void setM(const int d);
    void setN(const int d = -1);
    void setName(const QString &d);
    void setAddressType(const QString &d);
    void setBalance(const qint64 d);
    void setCreateDate(const QDateTime &d);
    void setSigners(const QSharedPointer<SingleSignerListModel> &d);

    SingleSignerListModel* singleSignersAssigned() const;
    QSharedPointer<SingleSignerListModel> singleSignersAssignedPtr() const;

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

    QWarningMessage* warningMessage() const;
    QSharedPointer<QWarningMessage> warningMessagePtr() const;
    void setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage);

    QString descriptior() const;
    void setDescriptior(const QString &descriptior);

    TransactionListModel *transactionHistory() const;
    QSharedPointer<Transaction> getTransactionByIndex(const int index) const;
    QSharedPointer<Transaction> getTransactionByTxid(const QString& txid) const;
    QSharedPointer<TransactionListModel> transactionHistoryPtr() const;
    void setTransactionHistory(const QSharedPointer<TransactionListModel> &d);

    int getCreationMode() const;
    void setCreationMode(int creationMode);

    enum class CreationMode : int {
        CREATE_NEW_WALLET,
        CREATE_BY_IMPORT_DB,
        CREATE_BY_IMPORT_DESCRIPTOR,
        CREATE_BY_IMPORT_CONFIGURATION,
        CREATE_BY_IMPORT_QRCODE
    };

    bool getContainsHWSigner() const;
    void setContainsHWSigner(bool containsHWSigner);

private:
    QString id_;
    int m_;
    int n_;
    QString name_;
    QString addressType_;
    qint64 balance_;
    QDateTime createDate_;
    bool escrow_;
    QSharedPointer<SingleSignerListModel> singleSignersAssigned_;
    QSharedPointer<TransactionListModel>  transactionHistory_;

    // Additional member
    QString address_;
    QStringList usedAddressList_;
    QStringList unUsedAddressList_;
    QStringList usedChangeAddressList_;
    QStringList unUsedChangeddAddressList_;

    // capable to create wallet
    bool capableCreate_;

    QString description_;
    QString descriptior_;
    QSharedPointer<QWarningMessage> warningMessage_;

    int creationMode_;
    bool containsHWSigner_;

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
    void warningMessageChanged();
    void transactionHistoryChanged();
    void creationModeChanged();
    void containsHWSignerChanged();
};

bool sortWalletByNameAscending(const QSharedPointer<Wallet> &v1, const QSharedPointer<Wallet> &v2);
bool sortWalletByNameDescending(const QSharedPointer<Wallet> &v1, const QSharedPointer<Wallet> &v2);

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
    void addWallet(const QString& pr_id,
                   const int pr_m,
                   const int pr_n,
                   const QString& pr_name,
                   const QString& pr_addrType,
                   const qint64 pr_balance,
                   const QDateTime& pr_createDate,
                   const bool pr_escrow,
                   QSharedPointer<SingleSignerListModel> pr_signers,
                   const QString &pr_description);
    int addWallet(const QSharedPointer<Wallet> &wl);
    void updateWalletList(const QSharedPointer<WalletListModel> &d);
    void updateBalance(const QString &id, const qint64 value);
    void updateAddress(const QString &id, const QStringList &used, const QStringList &unused);
    void updateName(const QString &id, const QString &value);
    void updateDescription(const QString &id, const QString &value);
    QStringList walletListByMasterSigner(const QString& masterSignerId);
    QStringList walletListByFingerPrint(const QString& masterFingerPrint);
    QSharedPointer<Wallet> getWalletByIndex(const int index);
    QSharedPointer<Wallet> getWalletById(const QString& id);
    bool removeWallet(const QSharedPointer<Wallet> it);
    void removeWallet(const QString& walletID);
    void notifyUnitChanged();
    void updateSignerHealthStatus(const QString& masterSignerId, const int status, const time_t time);
    void notifyMasterSignerDeleted(const QString& masterSignerId);
    void notifyMasterSignerRenamed(const QString& masterSignerId, const QString& newname);
    void notifyRemoteSignerRenamed(const QString&fingerprint, const QString &newname);
    int getWalletIndexById(const QString& walletId);
    void updateHealthCheckTime();
    enum WalletRoles {
        wallet_Id_Role,
        wallet_Name_Role,
        wallet_M_Role,
        wallet_N_Role,
        wallet_AddressType_Role,
        wallet_Balance_Role,
        wallet_createDate_Role,
        wallet_Escrow_Role,
        wallet_SingleSignerList_Role,
        wallet_Address_Role,
        wallet_usedAddressList_Role,
        wallet_unUsedAddressList_Role
    };
    void requestSort(int role, int order);
private:
    QList<QSharedPointer<Wallet>> d_;

signals:

};

#endif // WALLETLISTMODEL_H
