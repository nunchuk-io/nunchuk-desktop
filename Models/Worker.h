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

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QJsonArray>
#include <QThread>
#include <QSharedPointer>
#include <map>
#include <string>
#include <vector>
#include "WalletModel.h"
#include "DeviceModel.h"
#include "SingleSignerModel.h"
#include "MasterSignerModel.h"
#include "TransactionModel.h"
#include "QEventProcessor.h"
#include "core/common/datatypes/NunchukMetaType.hpp"

struct WorkerWarningData {
    QString what;
    int type{0};
    int code{0};
};
Q_DECLARE_METATYPE(WorkerWarningData)

struct WorkerScanDevicesData {
    int stateId{0};
    bool isTopUpXpub{false};
    QString masterSignerId;
    QString masterFingerprint;
    QString signerName;
    bool isSignedIn{false};
    QString hwiPath;
};
Q_DECLARE_METATYPE(WorkerScanDevicesData)

struct WorkerScanDevicesResult {
    int stateId{0};
    bool isTopUpXpub{false};
    std::vector<nunchuk::Device> devices;
    WorkerWarningData warning;
};
Q_DECLARE_METATYPE(WorkerScanDevicesResult)

struct WorkerCreateMasterSignerData {
    QString name;
    QString xfp;
    bool isSignedIn{false};
    QString hwiPath;
};
Q_DECLARE_METATYPE(WorkerCreateMasterSignerData)

struct WorkerCreateMasterSignerResult {
    bool hasSigner{false};
    nunchuk::MasterSigner signer;
    QString signerMessage;
    int deviceIndex{-1};
    WorkerWarningData warning;
};
Q_DECLARE_METATYPE(WorkerCreateMasterSignerResult)

struct WorkerCreateRemoteSignerData {
    QString name;
    QString xpub;
    QString publicKey;
    QString derivationPath;
    QString masterFingerprint;
    nunchuk::SignerType type{};
    std::vector<nunchuk::SignerTag> tags;
    bool replace{false};
    int event{0};
};
Q_DECLARE_METATYPE(WorkerCreateRemoteSignerData)

struct WorkerCreateRemoteSignerResult {
    int event{0};
    nunchuk::SingleSigner signer;
    bool replace{false};
    bool syncReplacement{false};
    WorkerWarningData warning;
};
Q_DECLARE_METATYPE(WorkerCreateRemoteSignerResult)

struct WorkerDisplayAddressResult {
    bool result{false};
    WorkerWarningData warning;
};
Q_DECLARE_METATYPE(WorkerDisplayAddressResult)

struct WorkerCreateSoftwareSignerData {
    QString name;
    QString secret;
    QString passphrase;
    bool isPrimaryKey{false};
    bool replace{false};
};
Q_DECLARE_METATYPE(WorkerCreateSoftwareSignerData)

struct WorkerCreateSoftwareSignerResult {
    nunchuk::MasterSigner signer;
    WorkerWarningData warning;
    bool hasReplacementSigner{false};
    nunchuk::SingleSigner replacementSigner;
    bool signerExists{false};
    QString signerExistsFingerprint;
};
Q_DECLARE_METATYPE(WorkerCreateSoftwareSignerResult)

struct WorkerCreateWalletData {
    bool needBackup{false};
    QString filePath;
    QString name;
    int m{0};
    int n{0};
    std::vector<nunchuk::SingleSigner> signers;
    nunchuk::AddressType addressType{};
    nunchuk::WalletType walletType{};
    QString description;
    nunchuk::WalletTemplate walletTemplate{};
};
Q_DECLARE_METATYPE(WorkerCreateWalletData)

struct WorkerCreateMiniscriptWalletData {
    QString name;
    QString scriptTemplate;
    std::map<std::string, nunchuk::SingleSigner> signers;
    nunchuk::AddressType addressType{};
    QString description;
    bool allowUsedSigner{false};
};
Q_DECLARE_METATYPE(WorkerCreateMiniscriptWalletData)

struct WorkerHealthCheckRemoteSignerData {
    int stateId{0};
    QString xfp;
    int signerType{0};
    QString message;
    bool hasSingleSigner{false};
    nunchuk::SingleSigner singleSigner;
    QString signerMessage;
    QString signerSignature;
};
Q_DECLARE_METATYPE(WorkerHealthCheckRemoteSignerData)

struct WorkerDeviceInputData {
    int stateId{0};
    nunchuk::Device device;
    QString input;
    bool shouldSend{false};
};
Q_DECLARE_METATYPE(WorkerDeviceInputData)

struct WorkerMultiDeviceSyncData {
    bool state{false};
    bool canSync{false};
};
Q_DECLARE_METATYPE(WorkerMultiDeviceSyncData)


typedef std::function<void(void)> WorkerFunc;
Q_DECLARE_METATYPE(WorkerFunc)

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();
    static Worker *create(QThread *thread);
private:
    static Worker *mInstance;
    WorkerCreateMasterSignerResult createMasterSigner(const WorkerCreateMasterSignerData &data);
    WorkerScanDevicesResult scanDevices(const WorkerScanDevicesData &data);
public slots:
    void slotStartCreateMasterSigner(const WorkerCreateMasterSignerData &data);

    void slotStartCreateRemoteSigner(const WorkerCreateRemoteSignerData &data);

    void slotStartScanDevices(const WorkerScanDevicesData &data);

    void slotStartSigningTransaction(const QString &walletId,
                                     const QString &txid,
                                     const nunchuk::Device &device,
                                     const QString &masterSignerId,
                                     const QString &roomId,
                                     const QString &initEventId,
                                     const nunchuk::Transaction &transactionSnapshot,
                                     bool checkTaprootRoundOne,
                                     bool isSoftware);

    void slotStartHealthCheckMasterSigner(const int state_id,
                                          const QString& xfp,
                                          const QString& message);

    void slotStartTopXPUBsMasterSigner(const QVariant &data);

    void slotStartHealthCheckRemoteSigner(const WorkerHealthCheckRemoteSignerData &data);

    void slotStartDisplayAddress(const QString &wallet_id,
                                 const QString &address);

    void slotStartRescanBlockchain(int start, int stop);

    void slotStartCreateSoftwareSigner(const WorkerCreateSoftwareSignerData &data);

    void slotStartCreateSoftwareSignerXprv(const WorkerCreateSoftwareSignerData &data);

    void slotStartCreateWallet(const WorkerCreateWalletData &data);

    // for callback
    void slotStartBalanceChanged(const QString &id, qint64 balance);

    void slotStartTransactionChanged(const QString &tx_id,
                                     const int status,
                                     const QString &wallet_id);

    void slotStartBlockChanged(const int height,
                               const QString &hex_header);

    void slotStartGetUsedAddresses(const QString wallet_id);

    void slotStartGetUnusedAddresses(const QString wallet_id);

    void slotStartGetTransactionHistory(const QString wallet_id);

    void slotRequestGetEstimatedFee();

    void slotStartSendPinToDevice(const WorkerDeviceInputData &data);

    void slotStartSendPassphraseToDevice(const WorkerDeviceInputData &data);

    void slotStartRemoveAllWallets();

    void slotStartRemoveAllSigners();

    void slotStartMultiDeviceSync(const WorkerMultiDeviceSyncData &data);

    void slotStartReloadUserDb();

    void slotStartReloadWallets();

    void slotStartReloadMasterSigners();

    void slotStartReloadRemoteSigners();

    void slotStartSyncWalletDb(const QString &wallet_id);

    void slotCreateMiniscriptWallet(const WorkerCreateMiniscriptWalletData &data);
signals:
    void finishCreateMasterSigner(const WorkerCreateMasterSignerResult &result);

    void finishCreateRemoteSigner(const WorkerCreateRemoteSignerResult &result);

    void finishScanDevices(const WorkerScanDevicesResult &result);

    void beginCreateMasterSigner();
    void beginCreateSoftwareSigner();
    void scanDevicesStarted(bool isTopUpXpub);
    void displayAddressStarted();
    void updateNewKeySignMessage(const QString &message);
    void updateAddSignerStep(int step);
    void completeAddSignerProgress();
    void createMasterSignerFailed(const WorkerWarningData &warning);
    void applyUserDraftWallet(bool fetched, const QJsonObject &output);

    void finishSigningTransaction(const QString &walletId,
                                  nunchuk::Transaction result,
                                  QString what,
                                  int type,
                                  int code,
                                  QString masterSignerId,
                                  bool showTaprootRoundOneToast,
                                  bool isSoftware);

    void matrixSigningFinished();

    void finishHealthCheckMasterSigner(const int state_id,
                                       const int status,
                                       const QString &message,
                                       const QString &signature,
                                       const QString &xfp,
                                       QString what,
                                       int type,
                                       int code);

    void finishTopXPUBsMasterSigner(const QVariant &signer);

    void finishHealthCheckRemoteSigner(const int status,
                                       QString what,
                                       int type,
                                       int code);

    void finishDisplayAddress(const WorkerDisplayAddressResult &result);

    void finishRescanBlockchain();

    void finishCreateSoftwareSigner(const WorkerCreateSoftwareSignerResult &result);

    void finishCreateSoftwareSignerXprv(const WorkerCreateSoftwareSignerResult &result);

    void finishCreateWallet(nunchuk::Wallet ret,
                            QString what,
                            int type,
                            int code);

    void finishBackupWallet(QString what,
                            int type,
                            int code);

    // for callback
    void finishBalanceChanged(const QString& id,
                              const qint64 balance);

    void finishTransactionChanged(const QString &tx_id,
                                  const int status,
                                  const QString &wallet_id,
                                  nunchuk::Transaction tx);

    void finishBlockChanged(const int height,
                            const QString &hex_header);

    void finishGetUsedAddresses(const QString& wallet_id,
                                const QStringList &addr,
                                const QStringList &changeAddr);

    void finishGetUnusedAddresses(const QString& wallet_id,
                                  const QStringList &addr,
                                  const QStringList &changeAddr);

    void finishGetTransactionHistory(const QString wallet_id,
                                     std::vector<nunchuk::Transaction> ret);

    void finishSendPinToDevice(const int state_id,
                               QString what,
                               int type,
                               int code);

    void finishSendPassphraseToDevice(const int state_id,
                                      QString what,
                                      int type,
                                      int code);
    void finishRemoveAllWallets();

    void finishRemoveAllSigners();

    void finishReloadMasterSigners(std::vector<nunchuk::MasterSigner> masterSigners);

    void finishReloadRemoteSigners(std::vector<nunchuk::SingleSigner> remoteSigners);

    void finishReloadWallets(std::vector<nunchuk::Wallet> wallets);

    void finishReloadGroupWallets(std::vector<nunchuk::Wallet> wallets);

    void prepareSyncWalletDb(const QString &wallet_id,
                             nunchuk::Wallet wallet,
                             bool walletLoaded);

    void finishSyncWalletDb(const QString &wallet_id,
                            const QStringList &usedAddresses,
                            const QStringList &usedChangeAddresses,
                            const QStringList &unusedAddresses,
                            const QStringList &unusedChangeAddresses);

    void finishCreateMiniscriptWallet(nunchuk::Wallet ret,
        QString what,
        int type,
        int code);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller();
    virtual ~Controller();
    bool scanDevicesSync();
public slots:
    void slotPrepareCreateMasterSigner(const QString &name, const QString &xfp);
    void slotFinishCreateMasterSigner(const WorkerCreateMasterSignerResult &result);

    void slotPrepareCreateRemoteSigner(const QString &name,
                                       const QString &xpub,
                                       const QString &public_key,
                                       const QString &derivation_path,
                                       const QString &master_fingerprint,
                                       nunchuk::SignerType type,
                                       std::vector<nunchuk::SignerTag> tags,
                                       bool replace,
                                       int event);
    void slotFinishCreateRemoteSigner(const WorkerCreateRemoteSignerResult &result);

    void slotPrepareScanDevices(const QVariant &data);
    void slotFinishScanDevices(const WorkerScanDevicesResult &result);

    void slotFinishSigningTransaction(const QString &walletId,
                                      nunchuk::Transaction result,
                                      QString what,
                                      int type,
                                      int code,
                                      QString masterSignerId,
                                      bool showTaprootRoundOneToast,
                                      bool isSoftware);

    void slotPrepareSigningTransaction(const QString &walletId,
                                       const QString &txid,
                                       const QString &deviceXfp,
                                       bool isSoftware);

    void slotPrepareHealthCheckRemoteSigner(const int state_id,
                                            const QString &xfp,
                                            const int signer_type,
                                            const QString &message);

    void slotPrepareDisplayAddress(const QString &wallet_id,
                                   const QString &address);

    void slotPrepareCreateWallet(bool need_backup, QString file_path);
    void slotPrepareCreateMiniscriptWallet();

    void slotPrepareSendPinToDevice(const int state_id,
                                    const int device_index,
                                    const QString &pin);

    void slotPrepareSendPassphraseToDevice(const int state_id,
                                           const int device_index,
                                           const QString &passphrase);

    void slotPrepareMultiDeviceSync(const bool state);

    void slotFinishHealthCheckMasterSigner(const int state_id,
                                           const int status,
                                           const QString &message,
                                           const QString& signature,
                                           const QString & xfp,
                                           QString what,
                                           int type,
                                           int code);

    void slotFinishTopXPUBsMasterSigner(const QVariant &data);


    void slotFinishHealthCheckRemoteSigner(const int status,
                                           QString what,
                                           int type,
                                           int code);
    void slotFinishDisplayAddress(const WorkerDisplayAddressResult &result);
    void slotFinishRescanBlockchain();

    void slotPrepareCreateSoftwareSigner(const QString &name,
                                         const QString &mnemonic,
                                         const QString &passphrase,
                                         bool replace);
    void slotFinishCreateSoftwareSigner(const WorkerCreateSoftwareSignerResult &result);

    void slotPrepareCreateSoftwareSignerXprv(const QString &name,
                                             const QString &xprv,
                                             bool replace);
    void slotFinishCreateSoftwareSignerXprv(const WorkerCreateSoftwareSignerResult &result);

    void slotFinishCreateWallet(nunchuk::Wallet ret,
                                QString what,
                                int type,
                                int code);
    void slotFinishBackupWallet(QString what,
                                int type,
                                int code);

    // For callback
    void slotFinishBalanceChanged(const QString &id,
                                  qint64 balance);
    void slotFinishTransactionChanged(const QString &tx_id,
                                      const int status,
                                      const QString &wallet_id,
                                      nunchuk::Transaction tx);
    void slotFinishBlockChanged(const int height,
                                const QString &hex_header);

    void slotFinishGetUsedAddresses(const QString& wallet_id,
                                    const QStringList &addr,
                                    const QStringList &changeAddr);

    void slotFinishGetUnusedAddresses(const QString &wallet_id,
                                      const QStringList &addr,
                                      const QStringList &changeAddr);

    void slotFinishGetTransactionHistory(const QString wallet_id,
                                         std::vector<nunchuk::Transaction> ret);

    void slotCheckAndUnlockDevice(const int state_id);

    void slotFinishSendPinToDevice(const int state_id,
                                   QString what,
                                   int type,
                                   int code);

    void slotFinishSendPassphraseToDevice(const int state_id,
                                          QString what,
                                          int type,
                                          int code);

    void slotFinishRemoveAllWallets();

    void slotFinishRemoveAllSigners();

    void slotFinishReloadMasterSigners(std::vector<nunchuk::MasterSigner> masterSigners);

    void slotFinishReloadRemoteSigners(std::vector<nunchuk::SingleSigner> remoteSigners);

    void slotFinishReloadWallets(std::vector<nunchuk::Wallet> wallets);

    void slotFinishReloadGroupWallets(std::vector<nunchuk::Wallet> wallets);

    void slotApplySyncWalletSnapshot(const QString &wallet_id,
                                     nunchuk::Wallet wallet,
                                     bool walletLoaded);

    void slotFinishSyncWalletDb(const QString &wallet_id,
                                const QStringList &usedAddresses,
                                const QStringList &usedChangeAddresses,
                                const QStringList &unusedAddresses,
                                const QStringList &unusedChangeAddresses);

    void slotFinishCreateMiniscriptWallet(nunchuk::Wallet ret,
                                          QString what,
                                          int type,
                                          int code);
signals:
    void startCreateMasterSigner(const QString name,
                                 const QString xfp);

    void startCreateMasterSignerPrepared(const WorkerCreateMasterSignerData &data);

    void startCreateRemoteSigner(const QString &name,
                                 const QString &xpub,
                                 const QString &public_key,
                                 const QString &derivation_path,
                                 const QString &master_fingerprint,
                                 const nunchuk::SignerType type,
                                 const std::vector<nunchuk::SignerTag> tags,
                                 const bool replace,
                                 const int event);

    void startCreateRemoteSignerPrepared(const WorkerCreateRemoteSignerData &data);

    void startScanDevices(const QVariant &data);

    void startScanDevicesPrepared(const WorkerScanDevicesData &data);

    void startSigningTransaction(const QString &walletId,
                                 const QString &txid,
                                 const QString& deviceXfp,
                                 bool isSoftware);

    void startSigningTransactionPrepared(const QString &walletId,
                                         const QString &txid,
                                         const nunchuk::Device &device,
                                         const QString &masterSignerId,
                                         const QString &roomId,
                                         const QString &initEventId,
                                         const nunchuk::Transaction &transactionSnapshot,
                                         bool checkTaprootRoundOne,
                                         bool isSoftware);

    void startHealthCheckMasterSigner(const int state_id,
                                      const QString& xfp,
                                      const QString& message);

    void startTopXPUBsMasterSigner(const QVariant &data);

    void startHealthCheckRemoteSigner(const int state_id,
                                      const QString& xfp,
                                      const int signer_type,
                                      const QString& message);

    void startHealthCheckRemoteSignerPrepared(const WorkerHealthCheckRemoteSignerData &data);

    void startDisplayAddress(const QString &wallet_id,
                             const QString &address);

    void startDisplayAddressPrepared(const QString &wallet_id,
                                     const QString &address);

    void startRescanBlockchain(int start, int stop);

    void startCreateSoftwareSigner(const QString name,
                                   const QString mnemonic,
                                   const QString passphrase,
                                   bool replace = false);

    void startCreateSoftwareSignerPrepared(const WorkerCreateSoftwareSignerData &data);

    void startCreateSoftwareSignerXprv(const QString name,
                                       const QString xprv,
                                       bool replace = false);

    void startCreateSoftwareSignerXprvPrepared(const WorkerCreateSoftwareSignerData &data);

    void startCreateWallet(bool need_backup, QString file_path);

    void startCreateWalletPrepared(const WorkerCreateWalletData &data);

    // For callback
    void startBalanceChanged(const QString& id,
                             const qint64 balance);


    void startTransactionChanged(const QString &tx_id,
                                 const int status,
                                 const QString &wallet_id);

    void startBlockChanged(const int height,
                           const QString &hex_header);

    void startGetUsedAddresses(const QString wallet_id);

    void startGetUnusedAddresses(const QString wallet_id);

    void startGetTransactionHistory(const QString wallet_id);

    void startNunchukSync();

    void startGetEstimatedFee();


    void startSendPinToDevice(const int state_id, const int device_index, const QString &pin);

    void startSendPinToDevicePrepared(const WorkerDeviceInputData &data);

    void startSendPassphraseToDevice(const int state_id, const int device_index, const QString &pprase);

    void startSendPassphraseToDevicePrepared(const WorkerDeviceInputData &data);

    void startRemoveAllWallets();

    void startRemoveAllSigners();

    void startReloadUserDb();

    void startReloadWallets();

    void startReloadMasterSigners();

    void startReloadRemoteSigners();

    void startMultiDeviceSync(const bool state);

    void startMultiDeviceSyncPrepared(const WorkerMultiDeviceSyncData &data);

    void startSyncWalletDb(const QString &wallet_id);

    void startCreateMiniscriptWallet();

    void startCreateMiniscriptWalletPrepared(const WorkerCreateMiniscriptWalletData &data);
    // Qml
    void finishedScanDevices();
    void finishedSigningTransaction();
    void finishedHealthCheckMasterSigner(bool isResult);
    void finishedHealthCheckRemoteSigner(bool isResult);
    void finishedDisplayAddress(bool result);
    void finishedCreateWallet();
    void finishedCreateRemoteSigner();
    void finishedGetTransactionHistory();
    void checkAndUnlockDevice(const int state_id);
    void finishSendPinToDevice(int result);
    void finishSendPassphraseToDevice(int result);
    void openPromtNunchukSync();
    void closePromtNunchukSync();
    void finishedTopXPUBsMasterSigner();
};

#endif // WORKER_H
