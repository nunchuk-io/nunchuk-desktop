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
#include <QThread>
#include <QSharedPointer>
#include "WalletModel.h"
#include "DeviceModel.h"
#include "SingleSignerModel.h"
#include "MasterSignerModel.h"
#include "TransactionModel.h"
#include "QEventProcessor.h"
#include "nunchuk.h"

Q_DECLARE_METATYPE(nunchuk::Transaction)
Q_DECLARE_METATYPE(nunchuk::Wallet)
Q_DECLARE_METATYPE(nunchuk::SingleSigner)
Q_DECLARE_METATYPE(nunchuk::MasterSigner)
Q_DECLARE_METATYPE(nunchuk::Device)
Q_DECLARE_METATYPE(std::vector<nunchuk::Device>)
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
public slots:
    void slotStartCreateMasterSigner(const QString &name,
                                     const int deviceIndex);

    void slotStartCreateRemoteSigner(const QString &name,
                                     const QString &xpub,
                                     const QString &public_key,
                                     const QString &derivation_path,
                                     const QString &master_fingerprint,
                                     const nunchuk::SignerType type,
                                     const std::vector<nunchuk::SignerTag> tags,
                                     const bool replace,
                                     const int event);

    void slotStartScanDevices(const int state_id);

    void slotStartSigningTransaction(const QString &walletId,
                                     const QString &txid,
                                     const QString& deviceXfp,
                                     bool isSoftware);

    void slotStartHealthCheckMasterSigner(const int state_id,
                                          const QString& xfp,
                                          const QString& message);

    void slotStartTopXPUBsMasterSigner(const QVariant &data);

    void slotStartHealthCheckRemoteSigner(const int state_id,
                                          const QString& xfp,
                                          const int signer_type,
                                          const QString& message);

    void slotStartDisplayAddress(const QString &wallet_id,
                                 const QString &address);

    void slotStartRescanBlockchain(int start, int stop);

    void slotStartCreateSoftwareSigner(const QString name,
                                       const QString mnemonic,
                                       const QString passphrase,
                                       bool replace);

    void slotStartCreateSoftwareSignerXprv(const QString name,
                                           const QString xprv,
                                           bool replace);

    void slotStartCreateWallet(bool need_backup, QString file_path);

    // for callback
    void slotStartBalanceChanged(const QString& id,
                                 const qint64 balance);

    void slotStartTransactionChanged(const QString &tx_id,
                                     const int status,
                                     const QString &wallet_id);

    void slotStartBlockChanged(const int height,
                               const QString &hex_header);

    void slotStartGetUsedAddresses(const QString wallet_id);

    void slotStartGetUnusedAddresses(const QString wallet_id);

    void slotStartGetTransactionHistory(const QString wallet_id);

    void slotStartGetEstimatedFee();

    void slotStartSendPinToDevice(const int state_id,
                                  const int device_idx,
                                  const QString &pin);

    void slotStartSendPassphraseToDevice(const int state_id,
                                         const int device_idx,
                                         const QString &pprase);

    void slotStartRemoveAllWallets();

    void slotStartRemoveAllSigners();

    void slotStartMultiDeviceSync(const bool state);

    void slotStartReloadUserDb();

    void slotStartReloadWallets();

    void slotStartReloadMasterSigners();

    void slotStartReloadRemoteSigners();

    void slotStartSyncWalletDb(const QString &wallet_id);

    void slotCreateMiniscriptWallet();
signals:
    void finishCreateMasterSigner(const QMasterSignerPtr ret,
                                  QString what,
                                  int type,
                                  int code);

    void finishCreateRemoteSigner(const int event,
                                  const nunchuk::SingleSigner ret,
                                  QString what,
                                  int type,
                                  int code);

    void finishScanDevices(const int state_id,
                           std::vector<nunchuk::Device> ret,
                           QString what,
                           int type,
                           int code);

    void finishSigningTransaction(const QString &walletId,
                                  nunchuk::Transaction result,
                                  QString what,
                                  int type,
                                  int code,
                                  QString masterSignerId,
                                  bool isSoftware);

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

    void finishDisplayAddress(bool result);

    void finishRescanBlockchain();

    void finishCreateSoftwareSigner(nunchuk::MasterSigner ret,
                                    QString what,
                                    int type,
                                    int code);

    void finishCreateSoftwareSignerXprv(nunchuk::MasterSigner ret,
                                        QString what,
                                        int type,
                                        int code);

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

    void finishSyncWalletDb(const QString &wallet_id);

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
    void slotFinishCreateMasterSigner(const QMasterSignerPtr ret,
                                      QString what,
                                      int type,
                                      int code);
    void slotFinishCreateRemoteSigner(const int event,
                                      const nunchuk::SingleSigner ret,
                                      QString what,
                                      int type,
                                      int code);
    void slotFinishScanDevices(const int state_id,
                               std::vector<nunchuk::Device> ret,
                               QString what,
                               int type,
                               int code);

    void slotFinishSigningTransaction(const QString &walletId,
                                      nunchuk::Transaction result,
                                      QString what,
                                      int type,
                                      int code,
                                      QString masterSignerId,
                                      bool isSoftware);
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
    void slotFinishDisplayAddress(bool result);
    void slotFinishRescanBlockchain();

    void slotFinishCreateSoftwareSigner(nunchuk::MasterSigner ret,
                                        QString what,
                                        int type,
                                        int code);

    void slotFinishCreateSoftwareSignerXprv(nunchuk::MasterSigner ret,
                                        QString what,
                                        int type,
                                        int code);

    void slotFinishCreateWallet(nunchuk::Wallet ret,
                                QString what,
                                int type,
                                int code);
    void slotFinishBackupWallet(QString what,
                                int type,
                                int code);

    // For callback
    void slotFinishBalanceChanged(const QString& id,
                                  const qint64 balance);
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

    void slotFinishSyncWalletDb(const QString &wallet_id);

    void slotFinishCreateMiniscriptWallet(nunchuk::Wallet ret,
                                          QString what,
                                          int type,
                                          int code);
signals:
    void startCreateMasterSigner(const QString id,
                                 const int deviceIndex);

    void startCreateRemoteSigner(const QString &name,
                                 const QString &xpub,
                                 const QString &public_key,
                                 const QString &derivation_path,
                                 const QString &master_fingerprint,
                                 const nunchuk::SignerType type,
                                 const std::vector<nunchuk::SignerTag> tags,
                                 const bool replace,
                                 const int event);

    void startScanDevices(const int state_id);

    void startSigningTransaction(const QString &walletId,
                                 const QString &txid,
                                 const QString& deviceXfp,
                                 bool isSoftware);

    void startHealthCheckMasterSigner(const int state_id,
                                      const QString& xfp,
                                      const QString& message);

    void startTopXPUBsMasterSigner(const QVariant &data);

    void startHealthCheckRemoteSigner(const int state_id,
                                      const QString& xfp,
                                      const int signer_type,
                                      const QString& message);

    void startDisplayAddress(const QString &wallet_id,
                             const QString &address);

    void startRescanBlockchain(int start, int stop);

    void startCreateSoftwareSigner(const QString name,
                                   const QString mnemonic,
                                   const QString passphrase,
                                   bool replace = false);

    void startCreateSoftwareSignerXprv(const QString name,
                                       const QString xprv,
                                       bool replace = false);

    void startCreateWallet(bool need_backup, QString file_path);

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

    void startSendPassphraseToDevice(const int state_id, const int device_index, const QString &pprase);

    void startRemoveAllWallets();

    void startRemoveAllSigners();

    void startReloadUserDb();

    void startReloadWallets();

    void startReloadMasterSigners();

    void startReloadRemoteSigners();

    void startMultiDeviceSync(const bool state);

    void startSyncWalletDb(const QString &wallet_id);

    void startCreateMiniscriptWallet();
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
