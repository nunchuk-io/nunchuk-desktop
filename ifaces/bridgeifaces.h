﻿/**************************************************************************
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
    Q_ENUMS(WalletOptionType_t)
    Q_ENUMS(Fee_Setting)
    Q_ENUMS(WalletTemplate)
public:
    enum class AddHardware : int {
        ADD_INHERITANCE = (int)nunchuk::SignerTag::INHERITANCE,
        ADD_KEYSTONE = (int)nunchuk::SignerTag::KEYSTONE,
        ADD_JADE = (int)nunchuk::SignerTag::JADE,
        ADD_PASSPORT = (int)nunchuk::SignerTag::PASSPORT,
        ADD_SEEDSIGNER = (int)nunchuk::SignerTag::SEEDSIGNER,
        ADD_COLDCARD = (int)nunchuk::SignerTag::COLDCARD,
        ADD_TREZOR = (int)nunchuk::SignerTag::TREZOR,
        ADD_LEDGER = (int)nunchuk::SignerTag::LEDGER,
        ADD_BITBOX = (int)nunchuk::SignerTag::BITBOX,
        ADD_TAPSIGNER,
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
        MINISCRIPT,
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
        PENDING_NONCE,
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
        TRANSACTION_PDF,
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
        PORTAL_NFC,
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
        FLOW_ONLINE_RECIEVE_TX,
        FLOW_REPLACE_USER_FREE
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
    enum class WalletOptionType_t : int {
        E_PERSONAL_WALLET,
        E_GROUP_WALLET,
        E_ASSISTED_WALLET,
        E_HOT_WALLET,
        E_MINISCRIPT_WALLET,
    };

    enum class Fee_Setting : int {
        ECONOMY = 0,
        STANDARD,
        PRIORITY
    };

    enum class WalletTemplate : int {
        DEFAULT = 0,
        DISABLE_KEY_PATH,  // Taproot wallet only
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
Q_DECLARE_METATYPE(ENUNCHUCK::Fee_Setting)
Q_DECLARE_METATYPE(ENUNCHUCK::WalletTemplate)


typedef void (*NunchukType1)(const QString &str);
Q_DECLARE_METATYPE(NunchukType1)

namespace bridge {

QString hwiPath();

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

nunchuk::SingleSigner nunchukGetOriginSingleSigner(const nunchuk::SingleSigner& signer,
                                                   QWarningMessage& msg);

QSingleSignerPtr nunchukGetUnusedSignerFromMasterSigner(const QString& mastersigner_id,
                                                        const ENUNCHUCK::WalletType& wallet_type,
                                                        const ENUNCHUCK::AddressType& address_type,
                                                        QWarningMessage &msg);

QSingleSignerPtr nunchukGetDefaultSignerFromMasterSigner(const QString& mastersigner_id,
                                                        const ENUNCHUCK::WalletType& wallet_type,
                                                        const ENUNCHUCK::AddressType& address_type,
                                                        QWarningMessage &msg);

QSingleSignerPtr nunchukGetAvailableSignerFromMasterSigner(const QMasterSignerPtr &master,
                                                            const ENUNCHUCK::WalletType& wallet_type,
                                                            const ENUNCHUCK::AddressType& address_type,
                                                            QWarningMessage &msg);

QSingleSignerPtr nunchukGetSignerFromMasterSigner(const QString& mastersigner_id,
                                                            const QString& derivation_path,
                                                            QWarningMessage& msg);

bool nunchukDeleteMasterSigner(const QString& mastersigner_id);

bool nunchukDeletePrimaryKey();

bool nunchukDeleteRemoteSigner(const QString& master_fingerprint, const QString& derivation_path);

void nunchukUpdateMasterSigner(const QMasterSignerPtr &signer, bool sync = true);

void nunchukUpdateRemoteSigner(const QSingleSignerPtr &signer, bool sync = true);

bool nunchukHasSinger(const nunchuk::SingleSigner& signer);

QSingleSignerListModelPtr nunchukGetRemoteSigners();

QSingleSignerPtr nunchukGetRemoteSigner(const QString& master_fingerprint);

QMasterSignerPtr nunchukGetMasterSignerFingerprint(const QString& master_fingerprint);

std::vector<nunchuk::SingleSigner> nunchukGetOriginRemoteSigners(QWarningMessage &msg);

QSingleSignerListModelPtr nunchukConvertRemoteSigners(std::vector<nunchuk::SingleSigner> list);

QSingleSignerPtr nunchukCreateSigner(const QString& name,
                                     const QString& xpub,
                                     const QString& public_key,
                                     const QString& derivation_path,
                                     const QString& master_fingerprint,
                                     const nunchuk::SignerType type,
                                     const std::vector<nunchuk::SignerTag> tags,
                                     bool replace);

nunchuk::SingleSigner nunchukCreateOriginSigner(const QString& name,
                                                const QString& xpub,
                                                const QString& public_key,
                                                const QString& derivation_path,
                                                const QString& master_fingerprint,
                                                const nunchuk::SignerType type,
                                                const std::vector<nunchuk::SignerTag> tags,
                                                bool replace,
                                                QWarningMessage &msg);

QWalletPtr nunchukCreateWallet(const nunchuk::Wallet& wallet,
                               bool allow_used_signer,
                               QWarningMessage &msg);

nunchuk::Wallet nunchukCreateOriginWallet(const QString& name,
                                          int m,
                                          int n,
                                          SingleSignerListModel *signers,
                                          nunchuk::AddressType address_type,
                                          nunchuk::WalletType wallet_type,
                                          const QString& description,
                                          bool allow_used_signer,
                                          const QString& decoy_pin,
                                          nunchuk::WalletTemplate walletTemplate,
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
                                         bool anti_fee_sniping,
                                         bool use_script_path,
                                         const nunchuk::SigningPath& signing_path,
                                         QWarningMessage &msg);

QTransactionPtr nunchukCancelCreateTransaction(const QString &wallet_id,
                                               nunchuk::Transaction origin_tx,
                                               QString input_address,
                                               const QString &memo,
                                               const int fee_rate,
                                               const QString &replace_txid,
                                               bool anti_fee_sniping,
                                               bool use_script_path,
                                               const nunchuk::SigningPath& signing_path,
                                               QWarningMessage &msg);

QTransactionPtr nunchukDraftTransaction(const QString& wallet_id,
                                        const QMap<QString, qint64> outputs,
                                        const QUTXOListModelPtr inputs,
                                        const qint64 fee_rate,
                                        const bool subtract_fee_from_amount,
                                        const QString &replace_txid,
                                        bool use_script_path,
                                        const nunchuk::SigningPath& signing_path,
                                        QWarningMessage &msg);

nunchuk::Transaction nunchukDraftOriginTransaction(const string &wallet_id,
                                                   std::vector<nunchuk::TxOutput> tx_outputs,
                                                   std::vector<nunchuk::TxInput> tx_inputs,
                                                   nunchuk::Amount fee_rate,
                                                   const bool subtract_fee_from_amount,
                                                   const string &replace_txid,
                                                   bool use_script_path,
                                                   const nunchuk::SigningPath& signing_path,
                                                   QWarningMessage &msg);


QTransactionPtr nunchukReplaceTransaction(const QString &wallet_id,
                                          const QString& tx_id,
                                          const int new_fee_rate,
                                          bool anti_fee_sniping,
                                          bool use_script_path,
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

QUTXOListModelPtr nunchukGetUnspentOutputs(const QString& wallet_id);

QUTXOListModelPtr nunchukLockedGetUnspentOutputs(const QString& wallet_id);

int nunchukBlockHeight();

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

bool nunchukDisplayAddressOnDevice(const QString& wallet_id,
                                   const QString& address,
                                   const QString& device_fingerprint,
                                   QWarningMessage &msg);

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
                                             bool isPrimaryKey,
                                             bool replace,
                                             QWarningMessage& msg);

nunchuk::MasterSigner CreateSoftwareSignerFromMasterXprv(const QString &name,
                                                         const QString &xprv,
                                                         bool isPrimaryKey,
                                                         bool replace,
                                                         QWarningMessage& msg);

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

QString loadJsonFile(const QString &filePathName);

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
void stopAllNunchuk();

void AddTapsigner(const QString& card_ident, const QString& xfp,
                  const QString& name, const QString& version,
                  int birth_height, bool is_testnet, bool replace = false);

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

QWalletPtr nunchukCreateHotWallet(const QString &mnemonic, const QString& passphrase, bool need_backup, bool replace, QWarningMessage &msg);

std::vector<nunchuk::CoinCollection> nunchukGetCoinCollections(const std::string& wallet_id);

std::vector<nunchuk::CoinTag> nunchukGetCoinTags(const std::string& wallet_id);

bool nunchukImportCoinControlData(const QString& wallet_id,
                                  const QString& data,
                                  bool force,
                                  QWarningMessage &msg);

QString nunchukExportCoinControlData(const QString& wallet_id,
                                     QWarningMessage &msg);

QUTXOListModelPtr nunchukGetCoinInCollection(const QString& wallet_id,
                                             int collection_id);

QUTXOListModelPtr nunckGetCoinByTag(const QString& wallet_id,
                                    int tag_id);

bool nunchukCreateCoinTag(const QString& wallet_id,
                          const QString& name,
                          const QString& color);

bool nunchukUpdateCoinTag(const QString& wallet_id,
                          const nunchuk::CoinTag& tag);

bool nunchukUpdateCoinCollection(const QString& wallet_id,
                                const nunchuk::CoinCollection &collection,
                                bool apply_to_existing_coins);

nunchuk::CoinCollection nunchukCreateCoinCollection(const QString& wallet_id,
                                 const QString& name);

bool nunchukAddToCoinCollection(const QString& wallet_id,
                              int collection_id,
                              const QString& tx_id,
                              int vout);

bool nunchukDeleteCoinCollection(const QString& wallet_id,
                                int collection_id);

bool nunchukDeleteCoinTag(const QString& wallet_id,
                          int tag_id);

bool nunchukAddToCoinTag(const QString& wallet_id,
                         int tag_id,
                         const QString& tx_id,
                         int vout);

bool nunchukRemoveFromCoinTag(const QString& wallet_id,
                              int tag_id,
                              const QString& tx_id,
                              int vout);

bool nunchukRemoveFromCoinCollection(const QString& wallet_id,
                              int collection_id,
                              const QString& tx_id,
                              int vout);

bool nunchukUpdateCoinMemo(const QString& wallet_id,
                           const QString& tx_id,
                           int vout,
                           const QString& memo);

bool nunchukLockCoin(const QString &wallet_id,
                     const QString &tx_id,
                     int vout);

bool nunchukUnlockCoin(const QString& wallet_id,
                       const QString& tx_id,
                       int vout);

std::vector<nunchuk::UnspentOutput> nunchukGetCoinByTag(const QString& wallet_id,
                                                        int tag_id);

std::vector<nunchuk::UnspentOutput> nunchukGetCoinByCollection(const QString& wallet_id,
                                                        int collection_id);

QList<QUTXOListModel*> nunchukGetCoinAncestry(const QString& wallet_id,
                                              const QString& tx_id,
                                              int vout);

QString nunchukExportBIP329(const QString& wallet_id,
                         QWarningMessage &msg);
void nunchukImportBIP329(const QString& wallet_id,
                  const QString& data,
                  QWarningMessage &msg);

QString nunchukGetAddressPath(const QString& wallet_id,
                              const QString& address);

bool nunchukMarkAddressUsed(const QString& wallet_id,
                            const QString& address);

std::vector<nunchuk::UnspentOutput> nunchukGetOriginCoinsFromTxInputs(const QString &walletId,
                                              const std::vector<nunchuk::TxInput> &inputs);

std::vector<nunchuk::UnspentOutput> nunchukGetOriginCoins(const QString &walletId);

std::vector<nunchuk::UnspentOutput> nunchukGetOriginUnspentOutputs(const QString &walletId);

QMasterSignerPtr ImportBackupKey( const std::vector<unsigned char>& data,
                                 const QString& backup_key,
                                 const QString& name,
                                 bool is_primary,
                                 QWarningMessage& msg);

std::vector<nunchuk::SingleSigner> GetSignersFromMasterSigner(const QString& mastersigner_id);

// Group wallet
void EnableGroupWallet();

void StartConsumeGroupEvent();

void StopConsumeGroupEvent();

std::vector<nunchuk::Wallet> nunchukGetOriginGroupWallets(QWarningMessage& msg);

// Group chat
void SendGroupMessage(const std::string& walletId,
                      const std::string& message,
                      const nunchuk::SingleSigner& signer,
                      QWarningMessage &msg);

std::vector<nunchuk::GroupMessage> GetGroupMessages(const std::string& walletId,
                                                    int page,
                                                    int pageSize,
                                                    bool latest,
                                                    QWarningMessage &msg);

nunchuk::GroupWalletConfig GetGroupWalletConfig(const std::string& walletId,
                                                QWarningMessage &msg);

void SetGroupWalletConfig(const std::string& walletId,
                          const nunchuk::GroupWalletConfig& config,
                          QWarningMessage &msg);

QPair<QString, QString> ParseGroupUrl(const QString& url,
                                      QWarningMessage &msg);

nunchuk::GroupConfig GetGroupConfig(QWarningMessage &msg);

nunchuk::GroupSandbox CreateGroup(const QString& name,
                                  int m,
                                  int n,
                                  nunchuk::AddressType addressType,
                                  QWarningMessage &msg);

nunchuk::GroupSandbox CreateGroup(const QString& name,
                                    const QString& script_tmpl,
                                    nunchuk::AddressType addressType,
                                    QWarningMessage &msg);

int GetGroupOnline(const QString& groupId);

nunchuk::GroupSandbox GetGroup(const QString& groupId,
                               QWarningMessage &msg);

std::vector<nunchuk::GroupSandbox> GetGroups(QWarningMessage &msg);

nunchuk::GroupSandbox JoinGroup(const QString& groupId,
                                QWarningMessage &msg);

bool DeleteGroup(const QString &groupId);

nunchuk::GroupSandbox SetSlotOccupied(const nunchuk::GroupSandbox& sandbox,
                                      int index,
                                      bool value);

nunchuk::GroupSandbox SetSlotOccupied(const nunchuk::GroupSandbox& sandbox,
                                        const QString& name,
                                        bool value);

nunchuk::GroupSandbox AddSignerToGroup(const QString& groupId,
                                       const nunchuk::SingleSigner& signer,
                                       int index, 
                                       QWarningMessage &msg);

nunchuk::GroupSandbox AddSignerToGroup(const QString& groupId,
                                        const nunchuk::SingleSigner& signer,
                                        const QString& name,
                                        QWarningMessage &msg);

nunchuk::GroupSandbox RemoveSignerFromGroup(const QString& groupId,
                                            int index,
                                            QWarningMessage &msg);

nunchuk::GroupSandbox RemoveSignerFromGroup(const QString& groupId,
                                                const QString& name,
                                                QWarningMessage &msg);

nunchuk::GroupSandbox UpdateGroup(const QString& groupId,
                                  const QString& name,
                                  int m,
                                  int n,
                                  nunchuk::AddressType addressType,
                                  QWarningMessage &msg);

nunchuk::GroupSandbox UpdateGroup(const QString& groupId,
                                  const QString& name,
                                  const QString& script_tmpl,
                                  nunchuk::AddressType addressType,
                                  QWarningMessage &msg);

nunchuk::GroupSandbox FinalizeGroup(const QString& groupId,
                                     const QSet<size_t> valuekeyset,
                                    QWarningMessage &msg);

bool CheckGroupWalletExists(const nunchuk::Wallet& wallet,
                            QWarningMessage &msg);

void RecoverGroupWallet(const QString& walletId,
                        QWarningMessage &msg);

void SetLastReadMessage(const QString& walletId,
                        const QString& messageId);

int GetUnreadMessagesCount(const QString& walletId);

QString GetGroupDeviceUID();

int GetScriptPathFeeRate(const QString &wallet_id, const nunchuk::Transaction &tx);

nunchuk::GroupSandbox CreateReplaceGroup(const QString& walletId,
                                         QWarningMessage &msg);

QMap<QString, bool> GetReplaceGroups(const QString& walletId,
                                             QWarningMessage &msg);

nunchuk::GroupSandbox AcceptReplaceGroup(const QString& walletId,
                                         const QString& groupId,
                                         QWarningMessage &msg);

void DeclineReplaceGroup(const QString& walletId,
                         const QString& groupId,
                         QWarningMessage &msg);

QStringList GetDeprecatedGroupWallets(QWarningMessage &msg);

QString GetHotKeyMnemonic(const QString &signer_id, const QString &passphrase = {});

// Mini script wallet
QWalletPtr nunchukCreateMiniscriptWallet(const QString& name, const QString& script_template,
                                         const std::map<std::string, nunchuk::SingleSigner>& signers,
                                         nunchuk::AddressType address_type, const QString& description,
                                         bool allow_used_signer, const QString& decoy_pin, QWarningMessage &msg);

nunchuk::Wallet nunchukCreateOriginMiniscriptWallet(const QString& name, const QString& script_template,
                                                    const std::map<std::string, nunchuk::SingleSigner>& signers,
                                                    nunchuk::AddressType address_type, const QString& description,
                                                    bool allow_used_signer, const QString& decoy_pin, QWarningMessage &msg);

std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> nunchukEstimateFeeForSigningPaths(const QString &wallet_id,
                                                                                                const QMap<QString, qint64> outputs,
                                                                                                const QUTXOListModelPtr inputs,
                                                                                                qint64 fee_rate,
                                                                                                bool subtract_fee_from_amount,
                                                                                                const QString &replace_txid);

std::pair<int64_t, nunchuk::Timelock::Based> nunchukGetTimelockedUntil(const QString& wallet_id, const QString& tx_id);

std::vector<nunchuk::SingleSigner> nunchukGetTransactionSigners(const QString& wallet_id, const QString& tx_id);

}
#endif // BRIDGEINTERFACE_H
