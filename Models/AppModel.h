#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QSharedPointer>
#include "WalletModel.h"
#include "DeviceModel.h"
#include "SingleSignerModel.h"
#include "MasterSignerModel.h"
#include "TransactionModel.h"
#include "UTXOModel.h"
#include "QQuickViewer.h"
#include "Worker.h"

class AppModel final : public Controller
{
    Q_OBJECT
    Q_PROPERTY(int                      chainTip            READ chainTip           NOTIFY chainTipChanged)
    Q_PROPERTY(WalletListModel*         walletList          READ walletList         NOTIFY walletListChanged)
    Q_PROPERTY(Wallet*                  walletInfo          READ walletInfo         NOTIFY walletInfoChanged)
    Q_PROPERTY(MasterSignerListModel*   masterSignerList    READ masterSignerList   NOTIFY masterSignerListChanged)
    Q_PROPERTY(SingleSignerListModel*   remoteSignerList    READ remoteSignerList   NOTIFY remoteSignerListChanged)
    Q_PROPERTY(DeviceListModel*         deviceList          READ deviceList         NOTIFY deviceListChanged)
    Q_PROPERTY(MasterSigner*            masterSignerInfo    READ masterSignerInfo   NOTIFY masterSignerInfoChanged)
    Q_PROPERTY(SingleSigner*            singleSignerInfo    READ singleSignerInfo   NOTIFY singleSignerInfoChanged)
    Q_PROPERTY(QStringList              walletsUsingSigner  READ walletsUsingSigner NOTIFY walletsUsingSignerChanged)
    Q_PROPERTY(Wallet*                  newWalletInfo       READ newWalletInfo      NOTIFY newWalletInfoChanged)
    Q_PROPERTY(TransactionListModel*    transactionHistory  READ transactionHistory NOTIFY transactionHistoryChanged)
    Q_PROPERTY(UTXOListModel*           utxoList            READ utxoList           NOTIFY utxoListChanged)
    Q_PROPERTY(UTXO*                    utxoInfo            READ utxoInfo           NOTIFY utxoInfoChanged)
    Q_PROPERTY(Transaction*             transactionInfo     READ transactionInfo    NOTIFY transactionInfoChanged)
    Q_PROPERTY(DestinationListModel*    destinationList     READ destinationList    NOTIFY destinationListChanged)
    Q_PROPERTY(int walletListCurrentIndex  READ walletListCurrentIndex  	WRITE setWalletListCurrentIndex  NOTIFY walletListCurrentIndexChanged)
    Q_PROPERTY(int   addSignerStep         READ getAddSignerStep            NOTIFY addSignerStepChanged)
    Q_PROPERTY(int   cacheXpubsPercentage  READ getCacheXpubsPercentage    	NOTIFY cacheXpubsPercentageChanged)
    Q_PROPERTY(int   addSignerPercentage   READ getAddSignerPercentage    	NOTIFY addSignerPercentageChanged)
    Q_PROPERTY(QString    estimateFeeEconomical READ getEstimateFeeEconomical  NOTIFY estimateFeeEconomicalChanged)
    Q_PROPERTY(QString    estimateFeeStandard   READ getEstimateFeeStandard    NOTIFY estimateFeeStandardChanged)
    Q_PROPERTY(QString    estimateFeePriority   READ getEstimateFeePriority    NOTIFY estimateFeePriorityChanged)
    Q_PROPERTY(QString    addressBalance        READ addressBalance            NOTIFY addressBalanceChanged)
    Q_PROPERTY(TransactionListModel*    transactionHistoryShort                READ transactionHistoryShort NOTIFY transactionHistoryChanged)
    Q_PROPERTY(QStringList      qrExported          READ qrExported            NOTIFY qrExportedChanged)
    Q_PROPERTY(QString          mnemonic            READ getMnemonic           WRITE setMnemonic NOTIFY mnemonicChanged)
    Q_PROPERTY(DeviceListModel* softwareSignerDeviceList                       READ softwareSignerDeviceList         NOTIFY softwareSignerDeviceListChanged)
    Q_PROPERTY(QStringList      suggestMnemonics    READ getSuggestMnemonics   NOTIFY suggestMnemonicsChanged)
    Q_PROPERTY(bool             limitSoftwareSigner READ limitSoftwareSigner   CONSTANT)

public:
    static AppModel *instance();
    AppModel(AppModel &other) = delete;
    AppModel(AppModel const &other) = delete;
    void operator=(const AppModel &other) = delete;

    WalletListModel *walletList() const;
    QSharedPointer<WalletListModel> walletListPtr() const;
    void setWalletList(const QSharedPointer<WalletListModel> &d);

    MasterSignerListModel *masterSignerList() const;
    QSharedPointer<MasterSignerListModel> masterSignerListPtr() const;
    void setMasterSignerList(const QSharedPointer<MasterSignerListModel> &d);

    SingleSignerListModel* remoteSignerList() const;
    QSharedPointer<SingleSignerListModel> remoteSignerListPtr() const;
    void setRemoteSignerList(const QSharedPointer<SingleSignerListModel> &d);

    DeviceListModel *deviceList() const ;
    QSharedPointer<DeviceListModel> deviceListPtr() const;
    void setDeviceList(const QSharedPointer<DeviceListModel> &d);

    MasterSigner *masterSignerInfo() const;
    QSharedPointer<MasterSigner> masterSignerInfoPtr() const;
    void setMasterSignerInfo(const QSharedPointer<MasterSigner> &d);
    void setMasterSignerInfo(const int index);
    void updateMasterSignerInfoName(const QString &name);
    void updateSingleSignerInfoName(const QString &name);

    SingleSigner *singleSignerInfo();
    QSharedPointer<SingleSigner> singleSignerInfoPtr() const;
    void setSingleSignerInfo(const QSharedPointer<SingleSigner> &d);

    QStringList walletsUsingSigner() const;
    void setWalletsUsingSigner(QStringList d);

    bool removeMasterSigner(const QString& masterSignerId);
    void removeWallet(const QString& wallet_id);

    // New wallet
    Wallet *newWalletInfo() const;
    QSharedPointer<Wallet> newWalletInfoPtr() const;
    void setNewWalletInfo(const QSharedPointer<Wallet> &d);

    // Wallet selection
    Wallet *walletInfo() const;
    QSharedPointer<Wallet> walletInfoPtr() const;
    void setWalletInfo(const QSharedPointer<Wallet> &d);
    void setWalletInfo(const int index);

    // For new transaction
    Transaction *transactionInfo() const;
    QSharedPointer<Transaction> transactionInfoPtr() const;
    void setTransactionInfo(const QSharedPointer<Transaction>& d);

    // Transition history
    TransactionListModel *transactionHistory() const;
    TransactionListModel *transactionHistoryShort() const;
    QSharedPointer<Transaction> getTransactionByIndex(const int index) const;
    QSharedPointer<Transaction> getTransactionByTxid(const QString& txid) const;
    QSharedPointer<TransactionListModel> transactionHistoryPtr() const;
    void setTransactionHistory(const QSharedPointer<TransactionListModel> &d);
    void setTransactionHistoryShort(const QSharedPointer<TransactionListModel> &d);

    UTXOListModel *utxoList() const;
    QSharedPointer<UTXOListModel> utxoListPtr() const;
    void setUtxoList(const QSharedPointer<UTXOListModel> &utxoList);

    DestinationListModel* destinationList() const;
    QSharedPointer<DestinationListModel> destinationListPtr() const;
    void setDestinationList(const QSharedPointer<DestinationListModel> &destinationList);

    int walletListCurrentIndex() const;
    void setWalletListCurrentIndex(int walletListCurrentIndex);

    int chainTip() const;
    void setChainTip(int chainTip);

    UTXO* utxoInfo() const;
    QSharedPointer<UTXO> utxoInfoPtr() const;
    void setUtxoInfo(const QSharedPointer<UTXO> &utxoInfo);

    QSharedPointer<Transaction> getTransactionReplaceInfo() const;
    void setTransactionReplaceInfo(const QSharedPointer<Transaction> &transactionReplaceInfo);

    int getAddSignerStep() const;
    void setAddSignerStep(int addSignerStep);

    int getCacheXpubsPercentage() const;
    void setCacheXpubsPercentage(int cacheXpubsPercentage);

    int getAddSignerPercentage() const;
    void setAddSignerPercentage(int addSignerPercentage);

    QString getTxidReplacing() const;
    void setTxidReplacing(const QString &txidReplacing);

    QString getEstimateFeePriority() const;
    void setEstimateFeePriority(int estimateFeePriority);

    QString getEstimateFeeStandard() const;
    void setEstimateFeeStandard(int estimateFeeStandard);

    QString getEstimateFeeEconomical() const;
    void setEstimateFeeEconomical(int estimateFeeEconomical);

    void checkDeviceUsableToSign();
    void checkDeviceUsableToAdd();
    void resetSignersChecked();
    QString addressBalance() const;
    void setAddressBalance(const qint64 &addressBalance);

    QStringList qrExported() const;
    void setQrExported(const QStringList &qrExported);
    QString getMnemonic() const;
    void setMnemonic(const QString &mnemonic);

    DeviceListModel *softwareSignerDeviceList() const ;
    QSharedPointer<DeviceListModel> softwareSignerDeviceListPtr() const;
    void setSoftwareSignerDeviceList(const QSharedPointer<DeviceListModel> &d);

    QStringList getSuggestMnemonics() const;
    void setSuggestMnemonics(const QStringList &suggestMnemonics);

    bool limitSoftwareSigner() const;

private:
    AppModel();
    ~AppModel();
    QSharedPointer<WalletListModel> walletList_;
    QSharedPointer<DeviceListModel> deviceList_;
    QSharedPointer<MasterSignerListModel> masterSignerList_;
    QSharedPointer<SingleSignerListModel> remoteSignerList_;
    QSharedPointer<MasterSigner>    masterSignerInfo_;
    QSharedPointer<SingleSigner>    singleSignerInfo_;
    QStringList                     walletsUsingSigner_;
    QSharedPointer<Wallet>          newWalletInfo_;
    QSharedPointer<Wallet>          walletInfo_;
    QSharedPointer<Transaction>     transactionInfo_;
    QSharedPointer<Transaction>     transactionReplaceInfo_;
    QSharedPointer<TransactionListModel>  transactionHistory_;
    QSharedPointer<TransactionListModel>  transactionHistoryShort_;
    QSharedPointer<UTXOListModel>   utxoList_;
    QSharedPointer<UTXO>            utxoInfo_;
    QSharedPointer<DestinationListModel> destinationList_;
    int                             walletListCurrentIndex_;
    int                             chainTip_;

    int                             addSignerStep_;
    int                             cacheXpubsPercentage_;
    int                             addSignerPercentage_;
    QString                         txidReplacing_;

    int                             estimateFeePriority_;
    int                             estimateFeeStandard_;
    int                             estimateFeeEconomical_;
    qint64                          addressBalance_;

    QTimer timer_;
    QStringList                     qrExported_;
    QString                         mnemonic_;
    QSharedPointer<DeviceListModel> softwareSignerDeviceList_;
    QStringList                     suggestMnemonics_;
signals:
    void walletListChanged();
    void masterSignerListChanged();
    void remoteSignerListChanged();
    void deviceListChanged();
    void masterSignerInfoChanged();
    void singleSignerInfoChanged();
    void newSignerChanged();
    void walletsUsingSignerChanged();
    void newWalletInfoChanged();
    void walletInfoChanged();
    void transactionInfoChanged();
    void transactionHistoryChanged();
    void transactionHistoryShortChanged();
    void utxoListChanged();
    void destinationListChanged();
    void walletListCurrentIndexChanged();
    void chainTipChanged();
    void utxoInfoChanged();
    void addSignerStepChanged();
    void cacheXpubsPercentageChanged();
    void addSignerPercentageChanged();
    void estimateFeePriorityChanged();
    void estimateFeeStandardChanged();
    void estimateFeeEconomicalChanged();
    void addressBalanceChanged();
    void sentPINToDeviceResult(int result);
    void loginResult(bool succeed);
    void qrExportedChanged();
    void requireRestartApp();
    bool preCheckAddressOnDevice(bool valid);
    void mnemonicChanged();
    void softwareSignerDeviceListChanged();
    void suggestMnemonicsChanged();
public slots:
    void timer1sChanged();
};

#endif // APPMODEL_H
