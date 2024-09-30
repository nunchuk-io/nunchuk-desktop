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
#include "QEventProcessor.h"
#include "Worker.h"
#include "Premiums/QWalletManagement.h"
#include "TypeDefine.h"
#include "Commons/SignInViaDummy.h"

class AppModel final : public Controller,
                       public SignInViaDummy
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
    Q_PROPERTY(QVariantList             walletsUsingSigner  READ walletsUsingSigner NOTIFY walletsUsingSignerChanged)
    Q_PROPERTY(Transaction*             transactionInfo     READ transactionInfo    NOTIFY transactionInfoChanged)
    Q_PROPERTY(UTXOListModel*           utxoList            READ utxoList           NOTIFY utxoListChanged)
    Q_PROPERTY(UTXO*                    utxoInfo            READ utxoInfo           NOTIFY utxoInfoChanged)
    Q_PROPERTY(DestinationListModel*    destinationList     READ destinationList    NOTIFY destinationListChanged)
    Q_PROPERTY(int walletListCurrentIndex  READ walletListCurrentIndex  	WRITE setWalletListCurrentIndex         NOTIFY walletListCurrentIndexChanged)
    Q_PROPERTY(int   addSignerStep         READ getAddSignerStep            NOTIFY addSignerStepChanged)
    Q_PROPERTY(int   addSignerPercentage   READ getAddSignerPercentage    	NOTIFY addSignerPercentageChanged)
    Q_PROPERTY(QString   fastestFee        READ fastestFee                  NOTIFY fastestFeeChanged)
    Q_PROPERTY(QString   fastestFeeCurrency     READ fastestFeeCurrency         NOTIFY fastestFeeChanged)
    Q_PROPERTY(QString   halfHourFee            READ halfHourFee                NOTIFY halfHourFeeChanged)
    Q_PROPERTY(QString   halfHourFeeCurrency    READ halfHourFeeCurrency        NOTIFY halfHourFeeChanged)
    Q_PROPERTY(QString   hourFee                READ hourFee                    NOTIFY hourFeeChanged)
    Q_PROPERTY(QString   hourFeeCurrency        READ hourFeeCurrency            NOTIFY hourFeeChanged)
    Q_PROPERTY(QString   minFee                 READ minFee                     NOTIFY minFeeChanged)
    Q_PROPERTY(QString   minFeeCurrency         READ minFeeCurrency             NOTIFY minFeeChanged)
    Q_PROPERTY(QString   addressBalance         READ addressBalance             NOTIFY addressBalanceChanged)
    Q_PROPERTY(QStringList      qrExported          READ qrExported             WRITE setQrExported                 NOTIFY qrExportedChanged)
    Q_PROPERTY(QString          mnemonic            READ getMnemonic            WRITE setMnemonic                   NOTIFY mnemonicChanged)
    Q_PROPERTY(DeviceListModel* softwareSignerDeviceList                        READ softwareSignerDeviceList       NOTIFY softwareSignerDeviceListChanged)
    Q_PROPERTY(QStringList      suggestMnemonics    READ getSuggestMnemonics    NOTIFY suggestMnemonicsChanged)
    Q_PROPERTY(bool             limitSoftwareSigner READ limitSoftwareSigner    CONSTANT)
    Q_PROPERTY(int              nunchukMode         READ nunchukMode            NOTIFY nunchukModeChanged)
    Q_PROPERTY(int              tabIndex            READ tabIndex               WRITE setTabIndex                   NOTIFY tabIndexChanged)
    Q_PROPERTY(QWarningMessage* warningMessage      READ warningMessage         NOTIFY warningMessageChanged)
    Q_PROPERTY(QVariantList    listMessage          READ listMessage            NOTIFY listMessagesChanged)
    Q_PROPERTY(QString          lasttimeCheckEstimatedFee                       READ lasttimeCheckEstimatedFee      NOTIFY lasttimeCheckEstimatedFeeChanged)
    Q_PROPERTY(QMasterSigner*    primaryKey         READ originPrimaryKey       NOTIFY primaryKeyChanged)
    Q_PROPERTY(QString    newKeySignMessage         READ newKeySignMessage       NOTIFY newKeySignMessageChanged)
    Q_PROPERTY(QString    newKeySignMessageSHA256   READ newKeySignMessageSHA256 NOTIFY newKeySignMessageChanged)
    Q_PROPERTY(int addSignerWizard                  READ addSignerWizard        WRITE setAddSignerWizard            NOTIFY addSignerWizardChanged)
    Q_PROPERTY(QString    txidReplacing             READ getTxidReplacing       NOTIFY txidReplacingChanged)

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
    void requestOnboarding();

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

    QVariantList walletsUsingSigner() const;
    void setWalletsUsingSigner(QVariantList d);

    bool removeMasterSigner(const QString& masterSignerId);
    void removeWallet(const QString& wallet_id);

    // New wallet
    Wallet *newWalletInfo() const;
    QWalletPtr newWalletInfoPtr() const;
    void setNewWalletInfo(const QWalletPtr &d);

    // Wallet selection
    Wallet *walletInfo() const;
    QWalletPtr walletInfoPtr() const;
    void setWalletInfo(const QWalletPtr &d, bool force = false);
    void setWalletInfoByIndex(const int index, bool force = false);

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
    void setWalletListCurrentIndex(int index, bool force = false);

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

    qint64  fastestFeeOrigin() const;
    QString fastestFeeCurrency() const;
    QString fastestFee() const;
    void setFastestFee(qint64 fee);

    qint64  halfHourFeeOrigin() const;
    QString halfHourFeeCurrency() const;
    QString halfHourFee() const;
    void setHalfHourFee(qint64 fee);

    qint64  hourFeeOrigin() const;
    QString hourFeeCurrency() const;
    QString hourFee() const;
    void setHourFee(qint64 fee);

    qint64  minFeeOrigin() const;
    QString minFeeCurrency() const;
    QString minFee() const;
    void setMinFee(qint64 fee);

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
    QVariantList listMessage() const;
    Q_INVOKABLE void showToast(int code, const QString &what, EWARNING::WarningType type);
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
    void checkAccountAvailability(QVariant msg);
    void resendTempoPassword(QVariant msg);
    QString newKeySignMessage() const;
    QString newKeySignMessageSHA256() const;
    void setNewKeySignMessage(const QString &value);

    const std::vector<nunchuk::PrimaryKey> &primaryKeys();
    nunchuk::PrimaryKey findPrimaryKey(const QString& fingerprint);
    void clearPrimaryKeyList();

    void setAddSignerWizard(int index);
    int addSignerWizard() const;

    bool checkAutoSignOut();
    void setTimeLogging(const QDateTime &newTimeLogging);

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
    QVariantList        walletsUsingSigner_;
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
    QVariantList        listMessage_;
    double              exchangeRates_;
    double              btcRates_;
    QDateTime           lasttime_checkEstimatedFee_;
    QMasterSignerPtr    m_primaryKey;
    QString             newKeySignMessage_;
    std::vector<nunchuk::PrimaryKey> m_primaryKeys;
    int m_addSignerWizard {0};
    QDateTime           timeLogging_;
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
    void forwardToast(int code, const QString &what, EWARNING::WarningType type);
    void listMessagesChanged();
    void txidReplacingChanged();
    void notifySignerExist(bool isSoftware, const QString fingerPrint);
    void syncingWalletFromServer(const QString fingerPrint);
    void syncingConfirmWalletRemoveKey(const QString fingerPrint);
    void signViaSingature();
public slots:
    void confirmSyncingWalletFromServer(bool yes, bool no);
    void timerHealthCheckTimeHandle();
    void timerFeeRatesHandle();
    void timerCheckAuthorizedHandle();
    void slotSignViaSingature();

    bool    parseQRWallet(const QString name, const QString desc, const QStringList qrtags);
    bool    parseQRCollabWallet(const QStringList qrtags);
    QString parseQRSigners(QStringList qrtags);
    QString parseJSONSigners(QString fileName);
    bool    isValidXPRV(const QString& xprv);
    bool    updateSettingRestartRequired();
    QString getFilePath(const QString in);
    bool    enableDatabaseEncryption(const QString in);
    void    recieveToast(int code, const QString &what, EWARNING::WarningType type);
};

#endif // APPMODEL_H
