#ifndef BRIDGEINTERFACE_H
#define BRIDGEINTERFACE_H

#include <QObject>
#include <functional>
#include <string>
#include <vector>
#include "AppModel.h"
#include "AppSetting.h"
#include "DeviceModel.h"
#include "SingleSignerModel.h"
#include "TransactionModel.h"
#include "UTXOModel.h"
#include "WalletModel.h"
#include "nunchuckiface.h"
#include "qUtils.h"

using namespace std;

class ENUNCHUCK: public QObject
{
    Q_OBJECT
    Q_ENUMS(AddressType)
    Q_ENUMS(Chain)
    Q_ENUMS(BackendType)
    Q_ENUMS(WalletType)
    Q_ENUMS(HealthStatus)
    Q_ENUMS(ExportFormat)
    Q_ENUMS(TransactionStatus)
    Q_ENUMS(ConnectionStatus)
    Q_ENUMS(Unit)
    Q_ENUMS(IN_FLOW)
public:
    enum class AddressType {
        ANY,
        LEGACY,
        NESTED_SEGWIT,
        NATIVE_SEGWIT,
    };

    enum class Chain {
        MAIN,
        TESTNET,
        REGTEST,
    };

    enum class BackendType {
        ELECTRUM,
        CORERPC,
    };

    enum class WalletType {
        SINGLE_SIG,
        MULTI_SIG,
        ESCROW,
    };

    enum class HealthStatus {
        SUCCESS,
        FINGERPRINT_NOT_MATCHED,
        NO_SIGNATURE,
        SIGNATURE_INVALID,
        KEY_NOT_MATCHED,
    };

    enum class TransactionStatus {
        PENDING_SIGNATURES,
        READY_TO_BROADCAST,
        NETWORK_REJECTED,
        PENDING_CONFIRMATION,
        REPLACED,
        CONFIRMED,
    };

    enum class ConnectionStatus {
        OFFLINE,
        SYNCING,
        ONLINE,
    };

    enum class ExportFormat {
        DB,
        DESCRIPTOR,
        COLDCARD,
        UTXO_CSV,
        TRANSACTION_CSV,
        COBO,
        BSMS,
    };

    enum class Unit : int {
        BTC,
        SATOSHI,
    };

    enum IN_FLOW {
        FLOW_ADD_SIGNER = 0,
        FLOW_ADD_WALLET,
        FLOW_SEND_TX,
        FLOW_RECIEVE_TX
    };

    enum PASSPHRASE_TRIGER {
        PASSPHRASE_HEALTHCHECK = 0,
        PASSPHRASE_SIGNING_TX,
        PASSPHRASE_CACHE_XPUB
    };
};
Q_DECLARE_METATYPE(ENUNCHUCK::AddressType)
Q_DECLARE_METATYPE(ENUNCHUCK::Chain)
Q_DECLARE_METATYPE(ENUNCHUCK::WalletType)
Q_DECLARE_METATYPE(ENUNCHUCK::HealthStatus)
Q_DECLARE_METATYPE(ENUNCHUCK::TransactionStatus)
Q_DECLARE_METATYPE(ENUNCHUCK::ConnectionStatus)
Q_DECLARE_METATYPE(ENUNCHUCK::ExportFormat)
Q_DECLARE_METATYPE(ENUNCHUCK::Unit)

namespace bridge {

bool nunchukMakeInstance(const QString &passphrase, QWarningMessage &msg);

QSharedPointer<Transaction> convertTransaction(nunchuk::Transaction in);

bool nunchukSetPassphrase(const QString &passphrase, QWarningMessage &msg);

QSharedPointer<WalletListModel> nunchukGetWallets();

QSharedPointer<Wallet> nunchukGetWallet(const QString &wallet_id);

bool nunchukDeleteWallet(const QString& wallet_id);

bool nunchukUpdateWalletName(const QString& wallet_id, const QString &name);

bool nunchukUpdateWalletDescription(const QString &wallet_id, const QString &description);

QSharedPointer<MasterSignerListModel> nunchukGetMasterSigners();

QSharedPointer<DeviceListModel> nunchukGetDevices();

QSharedPointer<MasterSigner> nunchukCreateMasterSigner(const QString &name,
                                                       const int deviceIndex,
                                                       QWarningMessage &msg);

QString nunchukGetHealthCheckPath();

nunchuk::HealthStatus nunchukHealthCheckMasterSigner(const QString &signerId,
                                                     const QString &message,
                                                     QString &signature,
                                                     QString &path,
                                                     QWarningMessage &msg);

nunchuk::HealthStatus nunchukHealthCheckSingleSigner(const QSharedPointer<SingleSigner> &signer,
                                                     QWarningMessage &msg);

int nunchukGetCurrentIndexFromMasterSigner(const QString& mastersigner_id,
                                           const ENUNCHUCK::WalletType& wallet_type,
                                           const ENUNCHUCK::AddressType& address_type,
                                           QWarningMessage &msg);

QSharedPointer<SingleSigner> nunchukGetSignerFromMasterSigner(const QString& mastersigner_id,
                                                              const ENUNCHUCK::WalletType& wallet_type,
                                                              const ENUNCHUCK::AddressType& address_type,
                                                              int index);

QSharedPointer<SingleSigner> nunchukGetUnusedSignerFromMasterSigner(const QString& mastersigner_id,
                                                                    const ENUNCHUCK::WalletType& wallet_type,
                                                                    const ENUNCHUCK::AddressType& address_type);

bool nunchukDeleteMasterSigner(const QString& mastersigner_id);

bool nunchukDeleteRemoteSigner(const QString& master_fingerprint, const QString& derivation_path);

bool nunchukUpdateMasterSigner(const QString& mastersignerId, const QString &name);

bool nunchukUpdateRemoteSigner(const QString &newname);

QSharedPointer<SingleSignerListModel> nunchukGetRemoteSigners();

QSharedPointer<SingleSigner> nunchukCreateSigner(const QString& name,
                                                 const QString& xpub,
                                                 const QString& public_key,
                                                 const QString& derivation_path,
                                                 const QString& master_fingerprint,
                                                 QWarningMessage &msg);

QSharedPointer<Wallet> nunchukCreateWallet(const QString& name,
                                           int m,
                                           int n,
                                           SingleSignerListModel* signers,
                                           ENUNCHUCK::AddressType address_type,
                                           bool is_escrow,
                                           const QString &desc,
                                           QWarningMessage &msg);

QString nunchukDraftWallet(const QString& name,
                           int m,
                           int n,
                           SingleSignerListModel* signers,
                           ENUNCHUCK::AddressType address_type,
                           bool is_escrow,
                           const QString &desc,
                           QWarningMessage &msg);

QSharedPointer<Wallet> nunchukImportWallet(const QString& dbFile);

QSharedPointer<Wallet> nunchukImportWalletDescriptor(const QString& dbFile,
                                                     const QString &name,
                                                     const QString &description);

QString nunchukGenNewAddresses(const QString& walletId, bool internal);

QStringList nunchukGetAddresses(const QString &wallet_id, bool used, bool internal);

QStringList nunchukGetUnusedAddresses(const QString& walletId, bool internal);

QStringList nunchukGetUsedAddresses(const QString& walletId, bool internal);

QSharedPointer<Transaction> nunchukCreateTransaction(const QString& wallet_id,
                                                     const QMap<QString, qint64> outputs,
                                                     const QString& memo,
                                                     const QSharedPointer<UTXOListModel> inputs,
                                                     const int fee_rate,
                                                     const bool subtract_fee_from_amount,
                                                     QWarningMessage &msg);

QSharedPointer<Transaction> nunchukDraftTransaction(const QString& wallet_id,
                                                    const QMap<QString, qint64> outputs,
                                                    const QSharedPointer<UTXOListModel> inputs,
                                                    const int fee_rate,
                                                    const bool subtract_fee_from_amount,
                                                    QWarningMessage &msg);

QSharedPointer<Transaction> nunchukReplaceTransaction(const QString &wallet_id,
                                                      const QString& tx_id,
                                                      const int new_fee_rate,
                                                      QWarningMessage &msg);

void nunchukUpdateTransactionMemo(const QString &wallet_id,
                                  const QString &tx_id,
                                  const QString &new_memo);

bool nunchukExportTransaction(const QString& wallet_id, const QString& tx_id, const QString& file_path);

QSharedPointer<Transaction> nunchukImportTransaction(const QString& wallet_id, const QString& file_path);

QSharedPointer<Transaction> nunchukSignTransaction(const QString& wallet_id,
                                                   const QString& tx_id,
                                                   const QSharedPointer<Device> &device,
                                                   QWarningMessage &msg);

nunchuk::Transaction nunchukSignTransactionThread(const QString &wallet_id,
                                                  const QString &tx_id,
                                                  const QSharedPointer<Device> &device,
                                                  QWarningMessage& msg);

QSharedPointer<Transaction> nunchukBroadcastTransaction(const QString& wallet_id,
                                                        const QString& tx_id,
                                                        QWarningMessage &msg);

QSharedPointer<Transaction> nunchukGetTransaction(const QString& wallet_id, const QString& tx_id);

bool nunchukDeleteTransaction(const QString& wallet_id, const QString& tx_id);

QSharedPointer<TransactionListModel> nunchukGetTransactionHistory(const QString& wallet_id, int count = 1000, int skip = 0);

QSharedPointer<UTXOListModel> nunchukGetUnspentOutputs(const QString& walletId);

int nunchukGetChainTip();

bool nunchukExportHealthCheckMessage(const QString& message, const QString& file_path);

QString nunchukImportHealthCheckSignature( const QString& file_path);

int nunchukEstimateFee(int conf_target);

bool nunchukExportUnspentOutputs( const QString& wallet_id,
                                  const QString& file_path,
                                  nunchuk::ExportFormat format);

bool nunchukExportTransactionHistory(const QString& wallet_id,
                                     const QString& file_path,
                                     nunchuk::ExportFormat format);

void nunchukBalanceChanged(const QString &walletId, const int value);

void nunchukDevicesChanged(const QString &fingerprint, const bool connected);

void nunchukTransactionChanged(const QString &tx_id, const int status);

void nunchukBlockChanged(const int height, const QString &hex_header);

void nunchukCacheMasterSignerXPub(const QString &mastersigner_id);

void nunchukUpdateAppSettings(QWarningMessage &msg);

bool nunchukExportWallet(const QString &wallet_id, const QString &file_path, const nunchuk::ExportFormat format);

bool nunchukDisplayAddressOnDevice( const QString& wallet_id,
                                    const QString& address,
                                    const QString& device_fingerprint);

qint64 nunchukGetAddressBalance(const QString &wallet_id,
                                const QString &address);

QString nunchukGetSelectedWallet();

bool nunchukSetSelectedWallet(const QString& wallet_id);

void nunchukPromtPinOnDevice(const QSharedPointer<Device> &device,
                             QWarningMessage& msg);

void nunchukSendPinToDevice(const QSharedPointer<Device> &device,
                            const QString& pin,
                            QWarningMessage& msg);

QSharedPointer<SingleSigner> nunchukCreateCoboSigner(const QString &name,
                                                     const QString &json_info,
                                                     QWarningMessage& msg);

QStringList nunchukExportCoboTransaction( const QString& wallet_id,
                                          const QString& tx_id,
                                          QWarningMessage& msg);

QSharedPointer<Transaction> nunchukImportCoboTransaction(const QString &wallet_id,
                                                         const QStringList &qr_data,
                                                         QWarningMessage& msg);

QStringList nunchukExportCoboWallet(const QString &wallet_id, QWarningMessage &msg);

QSharedPointer<Wallet> nunchukImportCoboWallet(const QStringList& qr_data,
                                               const QString& description);

QSharedPointer<Wallet> nunchukImportWalletConfigFile(const QString& file_path,
                                                     const QString& description);

void nunchukRescanBlockchain(int start_height, int stop_height = -1);

QSharedPointer<MasterSigner> nunchukCreateSoftwareSigner(const QString &name,
                                                         const QString &mnemonic,
                                                         const QString &passphrase,
                                                         QWarningMessage& msg);

void nunchukSendSignerPassphrase(const QString &mastersigner_id,
                                 const QString &passphrase,
                                 QWarningMessage &msg);

QStringList nunchuckGetBIP39WordList();

void nunchukClearSignerPassphrase(const QString& mastersigner_id,
                                  QWarningMessage& msg);

}

#endif // BRIDGEINTERFACE_H
