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

#ifndef BRIDGEINTERFACE_H
#define BRIDGEINTERFACE_H

#include <QObject>
#include <QtConcurrent>
#include <functional>
#include <string>
#include <vector>
#include "QWarningMessage.h"
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
#define MAX_UNUSED_ADDR 20

class ENUNCHUCK: public QObject
{
    Q_OBJECT
    Q_ENUMS(AddHardware)
    Q_ENUMS(TabSelection)
    Q_ENUMS(AddressType)
    Q_ENUMS(Chain)
    Q_ENUMS(BackendType)
    Q_ENUMS(WalletType)
    Q_ENUMS(HealthStatus)
    Q_ENUMS(ExportFormat)
    Q_ENUMS(TransactionStatus)
    Q_ENUMS(ConnectionStatus)
    Q_ENUMS(Unit)
    Q_ENUMS(SignerType)
    Q_ENUMS(RoomType)
    Q_ENUMS(IN_FLOW)
    Q_ENUMS(ROOM_EVT)
public:
    enum class AddHardware : int {
        ADD_LEDGER = (int)nunchuk::SignerTag::LEDGER,
        ADD_TREZOR = (int)nunchuk::SignerTag::TREZOR,
        ADD_COLDCARD = (int)nunchuk::SignerTag::COLDCARD,
        ADD_BITBOX = (int)nunchuk::SignerTag::BITBOX,
    };

    enum class TabSelection {
        WALLET_TAB,
        SERVICE_TAB,
        CHAT_TAB,
        SETTING_TAB,
        CURRENT_TAB,
    };

    enum class AddressType {
        ANY,
        LEGACY,
        NESTED_SEGWIT,
        NATIVE_SEGWIT,
        TAPROOT,
    };

    enum class Chain {
        MAIN,
        TESTNET,
        SIGNET,
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
        QRCODE,
        BSMS,
    };

    enum class Unit : int {
        BTC,
        SATOSHI,
    };

    enum class SignerType {
        UNKNOWN = -1,
        HARDWARE,
        AIRGAP,
        SOFTWARE,
        FOREIGN_SOFTWARE,
        NFC,
        COLDCARD_NFC,
        SERVER,
    };

    enum class RoomType {
        SUPPORT_ROOM = 0,
        SERVICE_ROOM,
        NUNSYNC_ROOM,
        GRPCHAT_ROOM
    };

    enum IN_FLOW {
        FLOW_NONE = 0,
        FLOW_PRIMARY_KEY,
        FLOW_REPLACE_PRIMARY_KEY,
        FLOW_ADD_SIGNER,
        FLOW_ADD_WALLET,
        FLOW_SEND_TX,
        FLOW_RECIEVE_TX,
        FLOW_ONLINE_ADD_SIGNER,
        FLOW_ONLINE_ADD_WALLET,
        FLOW_ONLINE_SEND_TX,
        FLOW_ONLINE_RECIEVE_TX
    };

    enum PASSPHRASE_TRIGER {
        PASSPHRASE_HEALTHCHECK = 0,
        PASSPHRASE_SIGNING_TX,
        PASSPHRASE_CACHE_XPUB
    };

    enum class ROOM_EVT {
        INITIALIZE = 0,     //0
        PLAIN_TEXT,         //1
        STATE_EVT,          //2
        WALLET_PAST,        //3
        WALLET_INIT,        //4
        WALLET_JOIN,        //5
        WALLET_LEAVE,       //6
        WALLET_READY,       //7
        WALLET_CREATE,      //8
        WALLET_CANCEL,      //9
        WALLET_BACKUP,      //10
        TX_CANCELED,        //11
        TX_INIT,            //12
        TX_SIGN,            //13
        TX_BROADCAST,       //14
        TX_READY,           //15
        TX_CANCEL,          //16
        TX_RECEIVE,         //17
        EXCEPTION,          //18
        FILE_IMAGE,         //19
        FILE_VIDEO,         //20
        FILE_OTHER,         //21
        INVALID,            //22
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
Q_DECLARE_METATYPE(ENUNCHUCK::SignerType)
Q_DECLARE_METATYPE(ENUNCHUCK::ROOM_EVT)

typedef void (*NunchukType1)(const QString &str);
Q_DECLARE_METATYPE(NunchukType1)

namespace bridge {
int nunchukCurrentMode();

void nunchukSetCurrentMode(int mode);

void nunchukMakeInstance(const QString &passphrase,
                         QWarningMessage &msg);

void nunchukMakeInstanceForAccount(const QString &account,
                                   const QString &passphrase,
                                   QWarningMessage &msg);

QTransactionPtr convertTransaction(const nunchuk::Transaction& in, const QString &wallet_id);

void nunchukSetPassphrase(const QString &passphrase, QWarningMessage &msg);

QWalletListModelPtr nunchukGetWallets();

std::vector<nunchuk::Wallet> nunchukGetOriginWallets(QWarningMessage &msg);

QWalletListModelPtr nunchukConvertWallets(std::vector<nunchuk::Wallet> list);

QWalletPtr convertWallet(const nunchuk::Wallet &wallet);

QWalletPtr nunchukGetWallet(const QString &wallet_id);

nunchuk::Wallet nunchukGetOriginWallet(const QString &wallet_id, QWarningMessage &msg);

bool nunchukHasWallet(const QString &wallet_id);

bool nunchukDeleteWallet(const QString& wallet_id, QWarningMessage &msg);

void nunchukDeleteAllWallet();

void nunchukUpdateWallet(const QString &wallet_id, const QString &name, const QString &description);

void nunchukUpdateWalletGapLimit(const QString& wallet_id, int gap_limit);

QMasterSignerListModelPtr nunchukGetMasterSigners();

std::vector<nunchuk::MasterSigner> nunchukGetOriginMasterSigners(QWarningMessage &msg);

nunchuk::MasterSigner nunchukGetOriginMasterSigner(const QString &id, QWarningMessage &msg);

QMasterSignerListModelPtr nunchukConvertMasterSigners(std::vector<nunchuk::MasterSigner> list);

QDeviceListModelPtr nunchukGetDevices(QWarningMessage& msg);

std::vector<nunchuk::Device> nunchukGetOriginDevices(QWarningMessage& msg);

QMasterSignerPtr nunchukCreateMasterSigner(const QString &name,
                                           const int deviceIndex,
                                           QWarningMessage &msg);

QString nunchukGetHealthCheckPath();

nunchuk::HealthStatus nunchukHealthCheckMasterSigner(const QString &xfp,
                                                     const QString &message,
                                                     QString &signature,
                                                     QString &path,
                                                     QWarningMessage &msg);

nunchuk::HealthStatus nunchukHealthCheckSingleSigner(const QSingleSignerPtr &signer,
                                                     QWarningMessage &msg);

int nunchukGetLastUsedSignerIndex(const QString& xfp,
                                  const ENUNCHUCK::WalletType& wallet_type,
                                  const ENUNCHUCK::AddressType& address_type,
                                  QWarningMessage &msg);

QSingleSignerPtr nunchukGetSignerFromMasterSigner(const QString& mastersigner_id,
                                                  const ENUNCHUCK::WalletType& wallet_type,
                                                  const ENUNCHUCK::AddressType& address_type,
                                                  const int index,
                                                  QWarningMessage &msg);

nunchuk::SingleSigner nunchukGetOriginSingleSigner(const QString& xfp,
                                                   const ENUNCHUCK::WalletType &wallet_type,
                                                   const ENUNCHUCK::AddressType &address_type,
                                                   const int index,
                                                   QWarningMessage& msg);

QSingleSignerPtr nunchukGetUnusedSignerFromMasterSigner(const QString& mastersigner_id,
                                                        const ENUNCHUCK::WalletType& wallet_type,
                                                        const ENUNCHUCK::AddressType& address_type,
                                                        QWarningMessage &msg);

QSingleSignerPtr nunchukGetDefaultSignerFromMasterSigner(const QString& mastersigner_id,
                                                        const ENUNCHUCK::WalletType& wallet_type,
                                                        const ENUNCHUCK::AddressType& address_type,
                                                        QWarningMessage &msg);

bool nunchukDeleteMasterSigner(const QString& mastersigner_id);

bool nunchukDeletePrimaryKey();

bool nunchukDeleteRemoteSigner(const QString& master_fingerprint, const QString& derivation_path);

void nunchukUpdateMasterSigner(const QMasterSignerPtr &signer, bool sync = true);

void nunchukUpdateRemoteSigner(const QSingleSignerPtr &signer, bool sync = true);

bool nunchukHasSinger(const nunchuk::SingleSigner& signer);

QSingleSignerListModelPtr nunchukGetRemoteSigners();

std::vector<nunchuk::SingleSigner> nunchukGetOriginRemoteSigners(QWarningMessage &msg);

QSingleSignerListModelPtr nunchukConvertRemoteSigners(std::vector<nunchuk::SingleSigner> list);

QSingleSignerPtr nunchukCreateSigner(const QString& name,
                                     const QString& xpub,
                                     const QString& public_key,
                                     const QString& derivation_path,
                                     const QString& master_fingerprint,
                                     const QString& type);

nunchuk::SingleSigner nunchukCreateOriginSigner(const QString& name,
                                                const QString& xpub,
                                                const QString& public_key,
                                                const QString& derivation_path,
                                                const QString& master_fingerprint,
                                                QWarningMessage &msg);

QWalletPtr nunchukCreateWallet(const nunchuk::Wallet& wallet,
                               bool allow_used_signer,
                               QWarningMessage &msg);

nunchuk::Wallet nunchukCreateOriginWallet(const QString& name,
                                          int m,
                                          int n,
                                          SingleSignerListModel* signers,
                                          ENUNCHUCK::AddressType address_type,
                                          bool is_escrow,
                                          const QString &desc,
                                          QWarningMessage &msg);

nunchuk::Wallet nunchukCreateOriginWallet(const QString& name,
                                          int m,
                                          int n,
                                          const std::vector<nunchuk::SingleSigner>& signers,
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

QWalletPtr nunchukImportWallet(const QString& dbFile,
                               QWarningMessage &msg);

QWalletPtr nunchukImportWalletDescriptor(const QString& dbFile,
                                         const QString &name,
                                         const QString &description,
                                         QWarningMessage &msg);

QString nunchukGenNewAddresses(const QString& walletId, bool internal);

QStringList nunchukGetAddresses(const QString &wallet_id, bool used, bool internal);

QStringList nunchukGetUnusedAddresses(const QString& walletId, bool internal);

QStringList nunchukGetUsedAddresses(const QString& walletId, bool internal);

QTransactionPtr nunchukCreateTransaction(const QString& wallet_id,
                                         const QMap<QString, qint64> outputs,
                                         const QString& memo,
                                         const QUTXOListModelPtr inputs,
                                         const int fee_rate,
                                         const bool subtract_fee_from_amount,
                                         const QString &replace_txid,
                                         QWarningMessage &msg);

QTransactionPtr nunchukCancelCreateTransaction(const QString &wallet_id,
                                               nunchuk::Transaction origin_tx,
                                               QString input_address,
                                               const QString &memo,
                                               const int fee_rate,
                                               const QString &replace_txid,
                                               QWarningMessage &msg);

QTransactionPtr nunchukDraftTransaction(const QString& wallet_id,
                                        const QMap<QString, qint64> outputs,
                                        const QUTXOListModelPtr inputs,
                                        const int fee_rate,
                                        const bool subtract_fee_from_amount,
                                        const QString &replace_txid,
                                        QWarningMessage &msg);

nunchuk::Transaction nunchukDraftOriginTransaction(const string &wallet_id,
                                                   std::vector<nunchuk::TxOutput> tx_outputs,
                                                   std::vector<nunchuk::TxInput> tx_inputs,
                                                   nunchuk::Amount fee_rate,
                                                   const bool subtract_fee_from_amount,
                                                   const string &replace_txid,
                                                   QWarningMessage &msg);


QTransactionPtr nunchukReplaceTransaction(const QString &wallet_id,
                                          const QString& tx_id,
                                          const int new_fee_rate,
                                          QWarningMessage &msg);

void nunchukUpdateTransactionMemo(const QString &wallet_id,
                                  const QString &tx_id,
                                  const QString &new_memo);

bool nunchukExportTransaction(const QString& wallet_id,
                              const QString& tx_id,
                              const QString& file_path,
                              QWarningMessage &msg);

QTransactionPtr nunchukImportTransaction(const QString& wallet_id,
                                         const QString& file_path,
                                         QWarningMessage &msg);

QTransactionPtr nunchukUpdateTransaction(const QString& wallet_id,
                                         const QString& tx_id,
                                         const QString& new_txid,
                                         const QString& raw_tx,
                                         const QString& reject_msg,
                                         QWarningMessage& msg);

QTransactionPtr nunchukImportPsbt(const QString& wallet_id,
                                        const QString& psbt,
                                        QWarningMessage &msg);

bool nunchukReplaceTransactionId(const QString& wallet_id,
                                 const QString &transaction_id,
                                 const QString &replace_txid,
                                 QWarningMessage &msg);

nunchuk::Transaction nunchukSignTransaction(const QString &wallet_id,
                                                  const QString &tx_id,
                                                  const QDevicePtr &device,
                                                  QWarningMessage& msg);

QTransactionPtr nunchukBroadcastTransaction(const QString& wallet_id,
                                            const QString& tx_id,
                                            QWarningMessage &msg);

QTransactionPtr nunchukGetTransaction(const QString& wallet_id,
                                      const QString& tx_id);

nunchuk::Transaction nunchukGetOriginTransaction(const QString &wallet_id,
                                                 const QString &tx_id,
                                                 QWarningMessage &msg);

bool nunchukDeleteTransaction(const QString& wallet_id, const QString& tx_id);

QTransactionListModelPtr nunchukGetTransactionHistory(const QString& wallet_id, int count = 1000, int skip = 0);

std::vector<nunchuk::Transaction> nunchukGetOriginTransactionHistory(const QString& wallet_id, int count = 1000, int skip = 0);

QUTXOListModelPtr nunchukGetUnspentOutputs(const QString& walletId);

int nunchukGetChainTip();

bool nunchukExportHealthCheckMessage(const QString& message, const QString& file_path);

QString nunchukImportHealthCheckSignature( const QString& file_path);

qint64 nunchukEstimateFee(int conf_target);

bool nunchukExportUnspentOutputs( const QString& wallet_id,
                                  const QString& file_path,
                                  nunchuk::ExportFormat format);

bool nunchukExportTransactionHistory(const QString& wallet_id,
                                     const QString& file_path,
                                     nunchuk::ExportFormat format);

void nunchukBalanceChanged(const QString &walletId, const qint64 balance);

void nunchukDevicesChanged(const QString &fingerprint, const bool connected);

void nunchukTransactionChanged(const QString &tx_id, const int status, const QString &wallet_id);

void nunchukBlockChanged(const int height, const QString &hex_header);

void nunchukCacheMasterSignerXPub(const QString &mastersigner_id,
                                  QWarningMessage &msg);

void nunchukUpdateAppSettings(QWarningMessage &msg);

bool nunchukExportWallet(const QString &wallet_id, const QString &file_path, const nunchuk::ExportFormat format);

bool nunchukDisplayAddressOnDevice( const QString& wallet_id,
                                    const QString& address,
                                    const QString& device_fingerprint);

qint64 nunchukGetAddressBalance(const QString &wallet_id,
                                const QString &address);

QString nunchukGetSelectedWallet();

bool nunchukSetSelectedWallet(const QString& wallet_id);

void nunchukPromtPinOnDevice(const QDevicePtr &device,
                             QWarningMessage& msg);

void nunchukSendPinToDevice(const QDevicePtr &device,
                            const QString& pin,
                            QWarningMessage& msg);

void nunchukSendPassphraseToDevice(const QDevicePtr &device,
                                   const QString &passphrase,
                                   QWarningMessage &msg);

QSingleSignerPtr nunchukCreateCoboSigner(const QString &name,
                                         const QString &json_info,
                                         QWarningMessage& msg);

QStringList nunchukExportCoboTransaction( const QString& wallet_id,
                                          const QString& tx_id,
                                          QWarningMessage& msg);

QTransactionPtr nunchukImportCoboTransaction(const QString &wallet_id,
                                             const QStringList &qr_data,
                                             QWarningMessage& msg);

QStringList nunchukExportCoboWallet(const QString &wallet_id,
                                    QWarningMessage &msg);

QWalletPtr nunchukImportCoboWallet(const QStringList& qr_data,
                                   const QString& description,
                                   QWarningMessage& msg);

QWalletPtr nunchukImportWalletConfigFile(const QString& file_path,
                                         const QString& description,
                                         QWarningMessage &msg);

void nunchukRescanBlockchain(int start_height, int stop_height = -1);

QMasterSignerPtr nunchukCreateSoftwareSigner(const QString &name,
                                             const QString &mnemonic,
                                             const QString &passphrase,
                                             QWarningMessage& msg,
                                             bool isPrimaryKey);

QString SignLoginMessage(const QString& mastersigner_id,
                         const QString& message);

void nunchukSendSignerPassphrase(const QString &mastersigner_id,
                                 const QString &passphrase,
                                 QWarningMessage &msg);

QStringList nunchuckGetBIP39WordList();

void nunchukClearSignerPassphrase(const QString& mastersigner_id,
                                  QWarningMessage& msg);

QString nunchukParseKeystoneSigner(const QString& qr_data);

QStringList nunchukExportKeystoneWallet(const QString& wallet_id,
                                        QWarningMessage& msg);

QStringList nunchukExportBCR2020010Wallet(const QString& wallet_id,
                                          QWarningMessage& msg);

QStringList nunchukExportKeystoneTransaction(const QString& wallet_id,
                                             const QString& tx_id,
                                             QWarningMessage& msg);

QTransactionPtr nunchukImportKeystoneTransaction(const QString& wallet_id,
                                                 const QList<QString>& qr_data,
                                                 QWarningMessage& msg);

QWalletPtr nunchukImportKeystoneWallet(const QList<QString>& qr_data,
                                       const QString& description,
                                       QWarningMessage& msg);

QString nunchukParsePassportSigner(const QStringList &qr_data);

QString nunchukParseQRSigners(const QStringList &qr_data);

QString nunchukParseJSONSigners(const QString &filePathName);

QSingleSignerPtr nunchukParseJSONSigners(const QString &filePathName, ENUNCHUCK::SignerType signer_type, QWarningMessage &msg);

QSingleSignerPtr nunchukParseJSONSigners(const QString &filePathName, ENUNCHUCK::SignerType signer_type, ENUNCHUCK::AddressType address_type, QWarningMessage &msg);

QStringList nunchukExportPassportWallet(const QString& wallet_id,
                                        QWarningMessage& msg);

QStringList nunchukExportPassportTransaction(const QString& wallet_id,
                                             const QString& tx_id,
                                             QWarningMessage& msg);

QTransactionPtr nunchukImportPassportTransaction(const QString& wallet_id,
                                                 const QStringList& qr_data,
                                                 QWarningMessage& msg);

void stopNunchuk();

void AddTapsigner(const QString& card_ident, const QString& xfp,
                  const QString& name, const QString& version,
                  int birth_height, bool is_testnet);

bool nunchukUpdateTransactionSchedule(const QString& wallet_id,
                                      const QString& tx_id,
                                      time_t ts,
                                      QWarningMessage& msg);

void ForceRefreshWallet(const QString& wallet_id, QWarningMessage &msg);

QString SignHealthCheckMessage(const QSingleSignerPtr& signer,
                               const QString& message,
                               QWarningMessage& msg);

QString SignHealthCheckMessage(const nunchuk::SingleSigner &signer,
                               const QString& message,
                               QWarningMessage& msg);

QMasterSignerPtr ImportTapsignerMasterSigner( const std::vector<unsigned char>& data,
                                              const QString& backup_key,
                                              const QString& name,
                                              bool is_primary,
                                              QWarningMessage& msg);

nunchuk::Transaction nunchukImportQRTransaction(const QString& wallet_id,
                                           const QList<QString>& qr_data,
                                           QWarningMessage& msg);

QStringList nunchukExportQRTransaction(const QString &wallet_id,
                                       const QString &tx_id,
                                       QWarningMessage &msg);

QString nunchukGetWalletExportData(const nunchuk::Wallet& wallet, nunchuk::ExportFormat format);

bool IsMyAddress(const QString& wallet_id, const QString& address, QWarningMessage &msg);

bool IsCPFP(const QString& wallet_id, const nunchuk::Transaction &tx, nunchuk::Amount &package_fee_rate, QWarningMessage& msg);

nunchuk::SingleSigner GetSignerFromMasterSigner(const QString& mastersigner_id,
                                                const QString& derivation_path,
                                                QWarningMessage& msg);

nunchuk::Transaction SignTransaction(const nunchuk::Wallet &wallet, const nunchuk::Transaction &tx, const nunchuk::Device &device, QWarningMessage &msg);

bool UpdateRemoteSigner(const nunchuk::SingleSigner &remotesigner, QWarningMessage &msg);

bool UpdateMasterSigner(const nunchuk::MasterSigner &mastersigner, QWarningMessage &msg);

bool UpdateWallet(const nunchuk::Wallet &wallet, QWarningMessage &msg);

nunchuk::TapsignerStatus GetTapsignerStatusFromMasterSigner(const QString& fingerPrint);

QString SignMessage(const nunchuk::SingleSigner& signer,
                    const QString& message);

QString GetSignerAddress(const nunchuk::SingleSigner& signer,
                         const nunchuk::AddressType& address_type);

QString GetHotWalletMnemonic(const QString& wallet_id, const QString& passphrase);

QWalletPtr nunchukCreateHotWallet(const QString &mnemonic, const QString& passphrase, bool need_backup, QWarningMessage &msg);
}

#endif // BRIDGEINTERFACE_H
