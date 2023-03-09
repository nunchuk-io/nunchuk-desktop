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
#include "QQuickViewer.h"
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
    void slotStartCreateMasterSigner(const QString &id,
                                     const int deviceIndex);

    void slotStartCreateRemoteSigner(const QString &name,
                                     const QString &xpub,
                                     const QString &public_key,
                                     const QString &derivation_path,
                                     const QString &master_fingerprint,
                                     const int event);

    void slotStartScanDevices(const int state_id);

    void slotStartSigningTransaction(const QString &walletId,
                                     const QString &txid,
                                     const QString& deviceXfp,
                                     bool isSoftware);

    void slotStartHealthCheckMasterSigner(const int state_id,
                                          const QString& signerId,
                                          const QString& message);

    void slotStartTopXPUBsMasterSigner(const QVariant &data);

    void slotStartHealthCheckRemoteSigner();

    void slotStartDisplayAddress(const QString &wallet_id,
                                 const QString &address);

    void slotStartRescanBlockchain(int start, int stop);

    void slotStartCreateSoftwareSigner(const QString name,
                                       const QString mnemonic,
                                       const QString passphrase);

    void slotStartCreateWallet(bool backup, QString file_path);

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

    void slotStartDeviceClosePin(int device_index);

    void slotStartSendPinToDevice(const int state_id,
                                  const int device_idx,
                                  const QString &pin);

    void slotStartSendPassphraseToDevice(const int state_id,
                                         const int device_idx,
                                         const QString &pprase);

    void slotStartRemoveAllWallets();

    void slotStartRemoveAllSigners();

    void slotStartMultiDeviceSync(const bool state);

    void slotStartReloadWallets();

    void slotStartReloadMasterSigners();

    void slotStartReloadRemoteSigners();

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
                                       const QString &id,
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

    void finishCreateSoftwareSigner(const QMasterSignerPtr ret,
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
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller();
    virtual ~Controller();
    void scanDevicesSync();
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
                                           const QString & id,
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
    void slotFinishCreateSoftwareSigner(const QMasterSignerPtr ret,
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
signals:
    void startCreateMasterSigner(const QString id,
                                 const int deviceIndex);

    void startCreateRemoteSigner(const QString &name,
                                 const QString &xpub,
                                 const QString &public_key,
                                 const QString &derivation_path,
                                 const QString &master_fingerprint,
                                 const int event);

    void startScanDevices(const int state_id);

    void startSigningTransaction(const QString &walletId,
                                 const QString &txid,
                                 const QString& deviceXfp,
                                 bool isSoftware);

    void startHealthCheckMasterSigner(const int state_id,
                                      const QString& signerId,
                                      const QString& message);

    void startTopXPUBsMasterSigner(const QVariant &data);

    void startHealthCheckRemoteSigner();

    void startDisplayAddress(const QString &wallet_id,
                             const QString &address);

    void startRescanBlockchain(int start, int stop);

    void startCreateSoftwareSigner(const QString name,
                                   const QString mnemonic,
                                   const QString passphrase);

    void startCreateWallet(bool backup,
                           QString file_path);

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

    void startDeviceClosePin(int device_index);

    void startSendPinToDevice(const int state_id, const int device_index, const QString &pin);

    void startSendPassphraseToDevice(const int state_id, const int device_index, const QString &pprase);

    void startRemoveAllWallets();

    void startRemoveAllSigners();

    void startReloadWallets();

    void startReloadMasterSigners();

    void startReloadRemoteSigners();

    void startMultiDeviceSync(const bool state);

    // Qml
    void finishedScanDevices();
    void finishedSigningTransaction();
    void finishedHealthCheckMasterSigner();
    void finishedHealthCheckRemoteSigner();
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
