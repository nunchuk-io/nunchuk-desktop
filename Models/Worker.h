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

#ifdef USING_STUB_API
#include "nunchuckstub.h"
#else
#include <nunchuk.h>
#endif

Q_DECLARE_METATYPE(nunchuk::Transaction)

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();
public slots:
    void slotStartCreateMasterSigner(const QString &id, const int deviceIndex);
    void slotStartScanDevices(const QString &fingerprint = "");
    void slotStartCacheMasterSignerXPub(const QString &mastersigner_id);
    void slotStartSigningTransaction(const QString &walletId, const QString &txid, const int deviceIndex, bool isSoftware);
    void slotStartHealthCheckMasterSigner(const QString& signerId, const QString& message);
    void slotStartDisplayAddress(const QString &wallet_id, const QString &address, const QString &device_fingerprint);
    void slotStartRescanBlockchain(int start, int stop);
    void slotStartCreateSoftwareSigner(const QString name, const QString mnemonic, const QString passphrase);

    // for callback
    void slotStartBalanceChanged(const QString& id, const int value);
    void slotStartTransactionChanged(const QString &tx_id, const int status);
    void slotStartBlockChanged(const int height, const QString &hex_header);

signals:
    void finishCreateMasterSigner(const QSharedPointer<MasterSigner> ret);
    void finishScanDevices(QSharedPointer<DeviceListModel> ret, const QString &fingerprint = "");
    void finishCacheMasterSignerXPub(const QString &mastersigner_id);
    void finishSigningTransaction(nunchuk::Transaction result, QString what, int type, int code, QString masterSignerId, bool isSoftware);
    void finishHealthCheckMasterSigner(const int status, const QString &signature, const QString &id, QString what, int type, int code);
    void finishDisplayAddress(bool result);
    void finishRescanBlockchain();
    void finishCreateSoftwareSigner(const QSharedPointer<MasterSigner> ret);

    // for callback
    void finishBalanceChanged(const QString& id, const int value);
    void finishTransactionChanged(const QString &tx_id, const int status);
    void finishBlockChanged(const int height, const QString &hex_header);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller();
    virtual ~Controller();
public slots:
    void slotFinishCreateMasterSigner(const QSharedPointer<MasterSigner> ret);
    void slotFinishScanDevices(QSharedPointer<DeviceListModel> ret, const QString &fingerprint = "");
    void slotFinishCacheMasterSignerXPub(const QString &mastersigner_id);
    void slotFinishSigningTransaction(nunchuk::Transaction result, QString what, int type, int code, QString masterSignerId, bool isSoftware);
    void slotFinishHealthCheckMasterSigner(const int status, const QString& signature, const QString & id, QString what, int type, int code);
    void slotFinishDisplayAddress(bool result);
    void slotFinishRescanBlockchain();
    void slotFinishCreateSoftwareSigner(const QSharedPointer<MasterSigner> ret);

    // For callback
    void slotFinishBalanceChanged(const QString& id, const int value);
    void slotFinishTransactionChanged(const QString &tx_id, const int status);
    void slotFinishBlockChanged(const int height, const QString &hex_header);

signals:
    void startCreateMasterSigner(const QString id, const int deviceIndex);
    void startScanDevices(const QString &fingerprint = "");
    void startCacheMasterSignerXPub(const QString &mastersigner_id);
    void startSigningTransaction(const QString &walletId, const QString &txid, const int deviceIndex, bool isSoftware);
    void startHealthCheckMasterSigner(const QString& signerId, const QString& message);
    void startDisplayAddress(const QString &wallet_id, const QString &address, const QString &device_fingerprint);
    void startRescanBlockchain(int start, int stop);
    void startCreateSoftwareSigner(const QString name, const QString mnemonic, const QString passphrase);

    // For callback
    void startBalanceChanged(const QString& id, const int value);
    void startTransactionChanged(const QString &tx_id, const int status);
    void startBlockChanged(const int height, const QString &hex_header);

    // Qml
    void finishedScanDevices();
    void finishedSigningTransaction();
    void finishedHealthCheckMasterSigner();
    void finishedDisplayAddress(bool result);
};

#endif // WORKER_H
