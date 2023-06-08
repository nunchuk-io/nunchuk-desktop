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

#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QSharedPointer>
#include "QWarningMessage.h"
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
    Q_PROPERTY(Wallet*                  walletInfo          READ walletInfo         NOTIFY walletInfoChanged)
    Q_PROPERTY(Wallet*                  newWalletInfo       READ newWalletInfo      NOTIFY newWalletInfoChanged)
    Q_PROPERTY(WalletListModel*         walletList          READ walletList         NOTIFY walletListChanged)
    Q_PROPERTY(MasterSignerListModel*   masterSignerList    READ masterSignerList   NOTIFY masterSignerListChanged)
    Q_PROPERTY(SingleSignerListModel*   remoteSignerList    READ remoteSignerList   NOTIFY remoteSignerListChanged)
    Q_PROPERTY(DeviceListModel*         deviceList          READ deviceList         NOTIFY deviceListChanged)
    Q_PROPERTY(QMasterSigner*           masterSignerInfo    READ masterSignerInfo   NOTIFY masterSignerInfoChanged)
    Q_PROPERTY(QSingleSigner*           singleSignerInfo    READ singleSignerInfo   NOTIFY singleSignerInfoChanged)
    Q_PROPERTY(QStringList              walletsUsingSigner  READ walletsUsingSigner NOTIFY walletsUsingSignerChanged)
    Q_PROPERTY(Transaction*             transactionInfo     READ transactionInfo    NOTIFY transactionInfoChanged)
    Q_PROPERTY(UTXOListModel*           utxoList            READ utxoList           NOTIFY utxoListChanged)
    Q_PROPERTY(UTXO*                    utxoInfo            READ utxoInfo           NOTIFY utxoInfoChanged)
    Q_PROPERTY(DestinationListModel*    destinationList     READ destinationList    NOTIFY destinationListChanged)
    Q_PROPERTY(int walletListCurrentIndex  READ walletListCurrentIndex  	WRITE setWalletListCurrentIndex  NOTIFY walletListCurrentIndexChanged)
    Q_PROPERTY(int   addSignerStep         READ getAddSignerStep            NOTIFY addSignerStepChanged)
    Q_PROPERTY(int   addSignerPercentage   READ getAddSignerPercentage    	NOTIFY addSignerPercentageChanged)
    Q_PROPERTY(QString   fastestFee        READ fastestFee          NOTIFY fastestFeeChanged)
    Q_PROPERTY(QString   fastestFeeCurrency     READ fastestFeeCurrency       NOTIFY fastestFeeChanged)
    Q_PROPERTY(QString   halfHourFee       READ halfHourFee         NOTIFY halfHourFeeChanged)
    Q_PROPERTY(QString   halfHourFeeCurrency    READ halfHourFeeCurrency      NOTIFY halfHourFeeChanged)
    Q_PROPERTY(QString   hourFee           READ hourFee             NOTIFY hourFeeChanged)
    Q_PROPERTY(QString   hourFeeCurrency        READ hourFeeCurrency          NOTIFY hourFeeChanged)
    Q_PROPERTY(QString   minFee            READ minFee              NOTIFY minFeeChanged)
    Q_PROPERTY(QString   minFeeCurrency         READ minFeeCurrency           NOTIFY minFeeChanged)
    Q_PROPERTY(QString   addressBalance        READ addressBalance            NOTIFY addressBalanceChanged)
    Q_PROPERTY(QStringList      qrExported          READ qrExported            WRITE setQrExported NOTIFY qrExportedChanged)
    Q_PROPERTY(QString          mnemonic            READ getMnemonic           WRITE setMnemonic   NOTIFY mnemonicChanged)
    Q_PROPERTY(DeviceListModel* softwareSignerDeviceList                       READ softwareSignerDeviceList         NOTIFY softwareSignerDeviceListChanged)
    Q_PROPERTY(QStringList      suggestMnemonics    READ getSuggestMnemonics   NOTIFY suggestMnemonicsChanged)
    Q_PROPERTY(bool             limitSoftwareSigner READ limitSoftwareSigner   CONSTANT)
    Q_PROPERTY(int              nunchukMode         READ nunchukMode           NOTIFY nunchukModeChanged)
    Q_PROPERTY(int              tabIndex            READ tabIndex              WRITE setTabIndex NOTIFY tabIndexChanged)
    Q_PROPERTY(QWarningMessage* warningMessage      READ warningMessage        NOTIFY warningMessageChanged)
    Q_PROPERTY(QString          lasttimeCheckEstimatedFee                      READ lasttimeCheckEstimatedFee     NOTIFY lasttimeCheckEstimatedFeeChanged)
    Q_PROPERTY(QMasterSigner*    primaryKey         READ originPrimaryKey       NOTIFY primaryKeyChanged)
    Q_PROPERTY(QString    newKeySignMessage         READ newKeySignMessage       NOTIFY newKeySignMessageChanged)
    Q_PROPERTY(QString    newKeySignMessageSHA256   READ newKeySignMessageSHA256 NOTIFY newKeySignMessageChanged)
    Q_PROPERTY(int addSignerWizard READ addSignerWizard WRITE setAddSignerWizard NOTIFY addSignerWizardChanged)

public:
    static AppModel *instance();
    AppModel(AppModel &other) = delete;
    AppModel(AppModel const &other) = delete;
    void operator=(const AppModel &other) = delete;
    void requestInitialData();
    void requestSyncWalletDb(const QString& wallet_id);
    void requestCreateUserWallets();
    void requestSyncSharedWallets();
    void requestClearData();

    WalletListModel *walletList() const;
    QWalletListModelPtr walletListPtr() const;
    void setWalletList(const QWalletListModelPtr &d);

    MasterSignerListModel *masterSignerList() const;
    QMasterSignerListModelPtr masterSignerListPtr() const;
    void setMasterSignerList(const QMasterSignerListModelPtr &d);

    SingleSignerListModel* remoteSignerList() const;
    QSingleSignerListModelPtr remoteSignerListPtr() const;
    void setRemoteSignerList(const QSingleSignerListModelPtr &d);

    DeviceListModel *deviceList() const ;
    QDeviceListModelPtr deviceListPtr() const;
    void setDeviceList(const QDeviceListModelPtr &d);

    QMasterSigner *originPrimaryKey() const;
    void setPrimaryKey(const QString &userName);
    QMasterSignerPtr getPrimaryKey() const;

    QMasterSigner *masterSignerInfo() const;
    QMasterSignerPtr masterSignerInfoPtr() const;
    void setMasterSignerInfo(const QMasterSignerPtr &d);
    void setMasterSignerInfo(const int index);

    QSingleSigner *singleSignerInfo();
    QSingleSignerPtr singleSignerInfoPtr() const;
    void setSingleSignerInfo(const QSingleSignerPtr &d);

    QStringList walletsUsingSigner() const;
    void setWalletsUsingSigner(QStringList d);

    bool removeMasterSigner(const QString& masterSignerId);
    void removeWallet(const QString& wallet_id);

    // New wallet
    Wallet *newWalletInfo() const;
    QWalletPtr newWalletInfoPtr() const;
    void setNewWalletInfo(const QWalletPtr &d);

    // Wallet selection
    Wallet *walletInfo() const;
    QWalletPtr walletInfoPtr() const;
    void setWalletInfo(const QWalletPtr &d);
    void setWalletInfoByIndex(const int index);

    // For new transaction
    Transaction *transactionInfo() const;
    QTransactionPtr transactionInfoPtr() const;
    void setTransactionInfo(const QTransactionPtr& d);

    UTXOListModel *utxoList() const;
    QUTXOListModelPtr utxoListPtr() const;
    void setUtxoList(const QUTXOListModelPtr &utxoList);

    DestinationListModel* destinationList() const;
    QDestinationListModelPtr destinationListPtr() const;
    void setDestinationList(const QDestinationListModelPtr &destinationList);

    int walletListCurrentIndex() const;
    void setWalletListCurrentIndex(int walletListCurrentIndex);

    int chainTip() const;
    void setChainTip(int chainTip);

    UTXO* utxoInfo() const;
    QUTXOPtr utxoInfoPtr() const;
    void setUtxoInfo(const QUTXOPtr &utxoInfo);

    QTransactionPtr getTransactionReplaceInfo() const;
    void setTransactionReplaceInfo(const QTransactionPtr &transactionReplaceInfo);

    int getAddSignerStep() const;
    void setAddSignerStep(int addSignerStep);

    int getAddSignerPercentage() const;
    void setAddSignerPercentage(int addSignerPercentage);

    QString getTxidReplacing() const;
    void setTxidReplacing(const QString &txidReplacing);

    QString fastestFeeCurrency() const;
    QString fastestFee() const;
    void setFastestFee(qint64 fee);

    QString halfHourFeeCurrency() const;
    QString halfHourFee() const;
    void setHalfHourFee(qint64 fee);

    QString hourFeeCurrency() const;
    QString hourFee() const;
    void setHourFee(qint64 fee);

    QString minFeeCurrency() const;
    QString minFee() const;
    void setMinFee(qint64 fee);

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
    QDeviceListModelPtr softwareSignerDeviceListPtr() const;
    void setSoftwareSignerDeviceList(const QDeviceListModelPtr &d);

    QStringList getSuggestMnemonics() const;
    void setSuggestMnemonics(const QStringList &suggestMnemonics);

    bool limitSoftwareSigner() const;

    bool inititalized() const;
    void setInititalized(bool inititalized);

    int nunchukMode() const;
    void setNunchukMode(int nunchukMode);

    int tabIndex() const;
    void setTabIndex(int tabIndex);

    QWarningMessage* warningMessage() const;
    QWarningMessagePtr warningMessagePtr() const;
    void setWarningMessage(const QWarningMessagePtr &warningMessage);
    Q_INVOKABLE void showToast(int code,
                               const QString &what,
                               EWARNING::WarningType type,
                               const QString &explain = "",
                               POPUP::PopupType popup = POPUP::PopupType::PopupCenter);
    Q_INVOKABLE void setToast(int code,
                              const QString &what,
                              EWARNING::WarningType type,
                              const QString &explain = "",
                              POPUP::PopupType popup = POPUP::PopupType::PopupCenter);
    double exchangeRates() const;
    void setExchangeRates(double exchangeRates);
    double btcRates() const;
    void setBtcRates(double btcRates);

    QString lasttimeCheckEstimatedFee() const;
    void setLasttimeCheckEstimatedFee(const QDateTime &lasttime_checkEstimatedFee);

    void startCheckAuthorize();
    void stopCheckAuthorize();

    bool makeInstanceForAccount(const QVariant msg, const QString &dbPassphrase);
    bool makeNunchukInstanceForAccount(const QVariant msg, const QString &dbPassphrase);
    void makeMatrixInstanceForAccount();
    bool makeNunchukInstance(const QVariant makeInstanceData, const QString &dbPassphrase);
    void loginNunchuk(QVariant msg);
    void create_account(QVariant msg);
    QString newKeySignMessage() const;
    QString newKeySignMessageSHA256() const;
    void setNewKeySignMessage(const QString &value);
    QStringList getUserWallets() const;

    const std::vector<nunchuk::PrimaryKey> &primaryKeys();
    nunchuk::PrimaryKey findPrimaryKey(const QString& fingerprint);
    void clearPrimaryKeyList();

    void setAddSignerWizard(int index);
    int addSignerWizard() const;

private:
    AppModel();
    ~AppModel();
    bool                inititalized_[2];
    QWalletListModelPtr walletList_;
    QDeviceListModelPtr deviceList_;
    QMasterSignerListModelPtr masterSignerList_;
    QSingleSignerListModelPtr remoteSignerList_;
    QMasterSignerPtr    masterSignerInfo_;
    QSingleSignerPtr    singleSignerInfo_;
    QStringList         walletsUsingSigner_;
    QWalletPtr          newWalletInfo_;
    QWalletPtr          walletInfo_;
    QTransactionPtr     transactionInfo_;
    QTransactionPtr     transactionReplaceInfo_;
    QTransactionListModelPtr  transactionPending_;
    QUTXOListModelPtr   utxoList_;
    QUTXOPtr            utxoInfo_;
    QDestinationListModelPtr destinationList_;
    int                 walletListCurrentIndex_;
    int                 chainTip_;
    int                 addSignerStep_;
    int                 addSignerPercentage_;
    QString             txidReplacing_;
    qint64              fastestFee_;
    qint64              halfHourFee_;
    qint64              hourFee_;
    qint64              minFee_;
    qint64              addressBalance_;
    QTimer              timerRefreshHealthCheck_;
    QTimer              timerFeeRates_;
    QTimer              timerCheckAuthorized_;
    QStringList         qrExported_;
    QString             mnemonic_;
    QDeviceListModelPtr softwareSignerDeviceList_;
    QStringList         suggestMnemonics_;
    int                 nunchukMode_;
    int                 m_tabIndex;
    QWarningMessagePtr warningMessage_;
    double              exchangeRates_;
    double              btcRates_;
    QDateTime           lasttime_checkEstimatedFee_;
    QMasterSignerPtr    m_primaryKey;
    QString             newKeySignMessage_;
    QStringList         mUserWallets;
    std::vector<nunchuk::PrimaryKey> m_primaryKeys;
    int m_addSignerWizard {0};

signals:
    void signalShowToast();
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
    void transactionPendingChanged();
    void utxoListChanged();
    void destinationListChanged();
    void walletListCurrentIndexChanged();
    void chainTipChanged();
    void utxoInfoChanged();
    void addSignerStepChanged();
    void addSignerPercentageChanged();
    void fastestFeeChanged();
    void halfHourFeeChanged();
    void hourFeeChanged();
    void minFeeChanged();
    void addressBalanceChanged();
    void qrExportedChanged();
    bool displayAddressOnDevices();
    void mnemonicChanged();
    void softwareSignerDeviceListChanged();
    void suggestMnemonicsChanged();
    void nunchukModeChanged();
    void tabIndexChanged();
    void warningMessageChanged();
    void exchangeRatesChanged();
    void finishGenerateSigner();
    void lasttimeCheckEstimatedFeeChanged();
    void primaryKeyChanged();
    void newKeySignMessageChanged();
    void btcRatesChanged();
    void addSignerWizardChanged();

public slots:
    void timerHealthCheckTimeHandle();
    void timerFeeRatesHandle();
    void timerCheckAuthorizedHandle();
    QString parseKeystoneSigner(QString qr);
    bool    parseKeystoneWallet(const QString name,
                                const QString desc,
                                const QStringList qrtags);
    bool    parseKeystoneCollabWallet(const QStringList qrtags);
    bool    parseKeystoneTransaction(const QStringList qrtags);
    QString parsePassportSigners(QStringList qrtags);
    QString parseQRSigners(QStringList qrtags);
    QString parseJSONSigners(QString fileName);
    bool    parsePassportTransaction(const QStringList qrtags);
    bool    updateSettingRestartRequired();
    QString getFilePath(const QString in);
    bool    enableDatabaseEncryption(const QString in);
};

#endif // APPMODEL_H
