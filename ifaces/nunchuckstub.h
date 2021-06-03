#ifndef NUNCHUCKSTUB_H
#define NUNCHUCKSTUB_H

#ifdef USING_STUB_API

#include <string>
#include <vector>
#include <map>
#include <random>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include <time.h>
#include <boost/optional.hpp>
#include <functional>
#include <memory>
#include <vector>

std::string random_number_as_string( size_t length );
std::string random_string( size_t length );
int random_int(int min, int max);
time_t random_time();

namespace nunchuk {

const int CONF_TARGET_PRIORITY = 2;
const int CONF_TARGET_STANDARD = 6;
const int CONF_TARGET_ECONOMICAL = 144;

typedef int64_t Amount;
typedef std::pair<std::string, int> TxInput;      // txid-vout pair
typedef std::pair<std::string, Amount> TxOutput;  // address-amount pair

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
    COBO,
    CSV,
    BSMS,
};

enum class Unit {
  BTC,
  SATOSHI,
};

class BaseException : public std::exception {
 public:
  explicit BaseException(int code, const char* message)
      : code_(code), message_(message) {}
  explicit BaseException(int code, const std::string& message)
      : code_(code), message_(message) {}
  int code() const noexcept { return code_; }
  const char* what() const noexcept { return message_.c_str(); }

 private:
  int code_;
  std::string message_;
};

class NunchukException : public BaseException {
 public:
  // Nunchuck-defined error codes
    static const int INVALID_ADDRESS = -1001;
    static const int INVALID_AMOUNT = -1002;
    static const int INVALID_PSBT = -1003;
    static const int INVALID_RAW_TX = -1004;
    static const int INVALID_FEE_RATE = -1005;
    static const int INVALID_ADDRESS_TYPE = -1006;
    static const int INVALID_WALLET_TYPE = -1007;
    static const int AMOUNT_OUT_OF_RANGE = -1008;
    static const int RUN_OUT_OF_CACHED_XPUB = -1009;
    static const int MESSAGE_TOO_SHORT = -1010;
    static const int COIN_SELECTION_ERROR = -1011;
    static const int PSBT_INCOMPLETE = -1012;
    static const int SERVER_REQUEST_ERROR = -1013;
    static const int INVALID_PASSPHRASE = -1014;
    static const int PASSPHRASE_ALREADY_USED = -1015;
    static const int INVALID_CHAIN = -1016;
    static const int INVALID_PARAMETER = -1017;
    static const int CREATE_DUMMY_SIGNATURE_ERROR = -1018;
    static const int APP_RESTART_REQUIRED = -1019;
    static const int INVALID_FORMAT = -1020;
    using BaseException::BaseException;
};

class StorageException : public BaseException {
 public:
  static const int WALLET_NOT_FOUND = -2001;
  static const int MASTERSIGNER_NOT_FOUND = -2002;
  static const int TX_NOT_FOUND = -2003;
  static const int SIGNER_USED = -2005;
  static const int INVALID_DATADIR = -2006;
  static const int SQL_ERROR = -2007;
  static const int WALLET_EXISTED = -2008;
  using BaseException::BaseException;
};

class RPCException : public BaseException {
 public:
  // Error codes from contrib/bitcoin/src/rpc/protocol.h
  static const int RPC_MISC_ERROR = -3001;
  static const int RPC_TYPE_ERROR = -3003;
  static const int RPC_INVALID_ADDRESS_OR_KEY = -3005;
  static const int RPC_OUT_OF_MEMORY = -3007;
  static const int RPC_INVALID_PARAMETER = -3008;
  static const int RPC_DATABASE_ERROR = -3020;
  static const int RPC_DESERIALIZATION_ERROR = -3022;
  static const int RPC_VERIFY_ERROR = -3025;
  static const int RPC_VERIFY_REJECTED = -3026;
  static const int RPC_VERIFY_ALREADY_IN_CHAIN = -3027;
  static const int RPC_IN_WARMUP = -3028;
  static const int RPC_METHOD_DEPRECATED = -3032;
  // Nunchuck-defined error codes
  static const int RPC_REQUEST_ERROR = -3099;
  using BaseException::BaseException;
};

class HWIException : public BaseException {
 public:
  // Error codes from contrib/hwi/hwilib/errors.py
  static const int NO_DEVICE_TYPE = -4001;
  static const int MISSING_ARGUMENTS = -4002;
  static const int DEVICE_CONN_ERROR = -4003;
  static const int UNKNOWN_DEVICE_TYPE = -4004;
  static const int INVALID_TX = -4005;
  static const int NO_PASSWORD = -4006;
  static const int BAD_ARGUMENT = -4007;
  static const int NOT_IMPLEMENTED = -4008;
  static const int UNAVAILABLE_ACTION = -4009;
  static const int DEVICE_ALREADY_INIT = -4010;
  static const int DEVICE_ALREADY_UNLOCKED = -4011;
  static const int DEVICE_NOT_READY = -4012;
  static const int UNKNOWN_ERROR = -4013;
  static const int ACTION_CANCELED = -4014;
  static const int DEVICE_BUSY = -4015;
  static const int NEED_TO_BE_ROOT = -4016;
  static const int HELP_TEXT = -4017;
  static const int DEVICE_NOT_INITIALIZED = -4018;
  // Nunchuck-defined error codes
  static const int RUN_ERROR = -4099;
  static const int INVALID_RESULT = -4098;
  using BaseException::BaseException;
};

class Device {
public:
    Device(const std::string& fingerprint);
    Device(const std::string& type, const std::string& path,
           const std::string& model, const std::string& master_fingerprint,
           bool needs_pass_phrase_sent, bool needs_pin_sent);

    std::string get_type() const;
    std::string get_path() const;
    std::string get_model() const;
    std::string get_master_fingerprint() const;
    bool connected() const;
    bool needs_pass_phrase_sent() const;
    bool needs_pin_sent() const;

private:
    std::string type_;
    std::string path_;
    std::string model_;
    std::string master_fingerprint_;
    bool connected_;
    bool needs_pass_phrase_sent_;
    bool needs_pin_sent_;
};

class SingleSigner {
public:
 SingleSigner(const std::string& name,
              const std::string& xpub,
              const std::string& public_key,
              const std::string& derivation_path,
              const std::string& master_fingerprint,
              time_t last_health_check,
              const std::string& master_signer_id = {});

 std::string get_name() const;
 std::string get_xpub() const;
 std::string get_public_key() const;
 std::string get_derivation_path() const;
 std::string get_master_fingerprint() const;
 std::string get_master_signer_id() const;
 bool is_used() const;
 bool has_master_signer() const;
 time_t get_last_health_check() const;
 void set_name(const std::string& value);

private:
 std::string name_;
 std::string xpub_;
 std::string public_key_;
 std::string derivation_path_;
 std::string master_fingerprint_;
 std::string master_signer_id_;
 time_t last_health_check_;
};

class MasterSigner {
public:
    MasterSigner(const std::string& id, const Device& device,
                 time_t last_health_check, bool software = false);

    std::string get_id() const;
    std::string get_name() const;
    Device get_device() const;
    time_t get_last_health_check() const;
    bool is_software() const;
    void set_name(const std::string& value);

private:
    std::string id_;
    std::string name_;
    Device device_;
    time_t last_health_check_;
    bool software_;
};

class Wallet {
public:
 Wallet(const std::string& id, int m, int n,
        std::vector<SingleSigner>& signers, AddressType address_type,
        bool is_escrow, time_t create_date);

 std::string get_id() const;
 std::string get_name() const;
 int get_m() const;
 int get_n() const;
 std::vector<SingleSigner> get_signers() const;
 AddressType get_address_type() const;
 bool is_escrow() const;
 Amount get_balance() const;
 time_t get_create_date() const;
 std::string get_description() const;
 void set_name(const std::string& value);
 void set_balance(const Amount& value);
 void set_description(const std::string& value);

private:
 std::string id_;
 std::string name_;
 int m_;
 int n_;
 std::vector<SingleSigner> signers_;
 AddressType address_type_;
 bool escrow_;
 Amount balance_;
 time_t create_date_;
 std::string description_;
};

class UnspentOutput {
public:
 UnspentOutput();

 std::string get_txid() const;
 int get_vout() const;
 std::string get_address() const;
 Amount get_amount() const;
 int get_height() const;
 std::string get_memo() const;

 void set_txid(const std::string& value);
 void set_vout(int value);
 void set_address(const std::string& value);
 void set_amount(const Amount& value);
 void set_height(int value);
 void set_memo(const std::string& value);

private:
 std::string txid_;
 int vout_;
 std::string address_;
 Amount amount_;
 int height_;
 std::string memo_;
};

class Transaction {
public:
 Transaction();

 std::string get_txid() const;
 int get_height() const;
 std::vector<TxInput> const get_inputs() const;
 std::vector<TxOutput> const get_outputs() const;
 std::vector<TxOutput> const get_user_outputs() const;
 std::vector<TxOutput> const get_receive_outputs() const;
 int get_change_index() const;
 int get_m() const;
 std::map<std::string, bool> const get_signers() const;
 std::string get_memo() const;
 TransactionStatus get_status() const;
 Amount get_fee() const;
 Amount get_fee_rate() const;
 time_t get_blocktime() const;
 bool subtract_fee_from_amount() const;
 bool is_receive() const;
 Amount get_sub_amount() const;

 std::string get_replaced_by_txid() const;

 void set_txid(const std::string& value);
 void set_height(int value);
 void add_input(const TxInput& value);
 void add_output(const TxOutput& value);
 void add_user_output(const TxOutput& value);
 void set_change_index(int value);
 void set_m(int value);
 void set_signer(const std::string& signer_id, bool has_signature);
 void set_memo(const std::string& value);
 void set_status(TransactionStatus value);
 void set_fee(const Amount& value);
 void set_fee_rate(const Amount& value);
 void set_blocktime(time_t value);
 void set_subtract_fee_from_amount(bool value);
 void set_receive(bool value);
 void set_sub_amount(const Amount& value);

private:
 std::string txid_;
 int height_;
 std::vector<TxInput> inputs_;
 std::vector<TxOutput> outputs_;
 std::vector<TxOutput> user_outputs_;
 int change_index_;
 int m_;
 std::map<std::string, bool> signers_;
 std::string memo_;
 TransactionStatus status_;
 Amount fee_;
 Amount fee_rate_;
 time_t blocktime_;
 bool subtract_fee_from_amount_;
 bool is_receive_;
 Amount sub_amount_;
};

class AppSettings {
public:
 AppSettings();

 Chain get_chain() const;
 std::vector<std::string> get_mainnet_servers() const;
 std::vector<std::string> get_testnet_servers() const;
 std::string get_hwi_path() const;
 std::string get_storage_path() const;
 bool use_proxy() const;
 std::string get_proxy_host() const;
 int get_proxy_port() const;
 std::string get_proxy_username() const;
 std::string get_proxy_password() const;
 std::string get_certificate_file() const;

 void set_chain(Chain value);
 void set_mainnet_servers(const std::vector<std::string>& value);
 void set_testnet_servers(const std::vector<std::string>& value);
 void set_hwi_path(const std::string& value);
 void set_storage_path(const std::string& value);
 void enable_proxy(bool value);
 void set_proxy_host(const std::string& value);
 void set_proxy_port(int value);
 void set_proxy_username(const std::string& value);
 void set_proxy_password(const std::string& value);
 void set_certificate_file(const std::string& value);

 BackendType get_backend_type() const;
 std::string get_corerpc_host() const;
 int get_corerpc_port() const;
 std::string get_corerpc_username() const;
 std::string get_corerpc_password() const;

 void set_backend_type(BackendType value);
 void set_corerpc_host(const std::string& value);
 void set_corerpc_port(int value);
 void set_corerpc_username(const std::string& value);
 void set_corerpc_password(const std::string& value);

private:
 Chain chain_;
 std::vector<std::string> mainnet_servers_;
 std::vector<std::string> testnet_servers_;
 std::string hwi_path_;
 std::string storage_path_;
 bool enable_proxy_;
 std::string proxy_host_;
 int proxy_port_;
 std::string proxy_username_;
 std::string proxy_password_;
 std::string certificate_file_;

 BackendType backend_type_;
 std::string corerpc_host_;
 int corerpc_port_;
 std::string corerpc_username_;
 std::string corerpc_password_;
};

// STUB FUNCTIONS
void SetPassphrase(const std::string& passphrase);
Wallet CreateWallet(const std::string& name, int m, int n,
                    std::vector<SingleSigner>& signers,
                    AddressType address_type, bool is_escrow, const std::string &desc);

std::string DraftWallet(const std::string& name, int m, int n,
                    std::vector<SingleSigner>& signers,
                    AddressType address_type, bool is_escrow, const std::string &desc);

std::vector<Wallet> GetWallets();
Wallet GetWallet(const std::string& wallet_id);
bool DeleteWallet(const std::string& wallet_id);
bool UpdateWallet(Wallet& wallet);
bool ExportWallet(const std::string& wallet_id,
                  const std::string& file_path,
                  ExportFormat format);
Wallet ImportWalletDb(const std::string& file_path);
Wallet ImportWalletDescriptor(const std::string &file_path, const std::string &name, const std::string &description);

std::vector<Device> GetDevices();
MasterSigner CreateMasterSigner(const std::string& name,
                                const Device& device);
SingleSigner GetSignerFromMasterSigner(
        const std::string& mastersigner_id, const WalletType& wallet_type,
        const AddressType& address_type, int index);
SingleSigner CreateSigner(const std::string& name,
                          const std::string& xpub,
                          const std::string& public_key,
                          const std::string& derivation_path,
                          const std::string& master_fingerprint);
int GetCurrentIndexFromMasterSigner(
        const std::string& mastersigner_id, const WalletType& wallet_type,
        const AddressType& address_type);
SingleSigner GetUnusedSignerFromMasterSigner(
        const std::string& mastersigner_id, const WalletType& wallet_type,
        const AddressType& address_type);
std::vector<SingleSigner> GetSignersFromMasterSigner(
        const std::string& mastersigner_id);
int GetNumberOfSignersFromMasterSigner(
        const std::string& mastersigner_id);
std::vector<MasterSigner> GetMasterSigners();
MasterSigner GetMasterSigner(const std::string& mastersigner_id);
bool DeleteMasterSigner(const std::string& mastersigner_id);
bool UpdateMasterSigner(MasterSigner& mastersigner);
std::vector<SingleSigner> GetRemoteSigners();
std::string GetHealthCheckPath() ;
HealthStatus HealthCheckMasterSigner(const std::string& fingerprint, const std::string &message, std::string &signature, std::string &path);
HealthStatus HealthCheckSingleSigner(
        const SingleSigner& signer, const std::string& message,
        const std::string& signature);

std::vector<Transaction> GetTransactionHistory(
    const std::string& wallet_id, int count, int skip);
AppSettings GetAppSettings();
AppSettings UpdateAppSettings(AppSettings& appSettings);

std::vector<std::string> GetAddresses(const std::string& wallet_id, bool used , bool internal);

std::string NewAddress(const std::string& wallet_id,
                               bool internal = false);
std::vector<UnspentOutput> GetUnspentOutputs(
    const std::string& wallet_id);

Transaction CreateTransaction(
      const std::string& wallet_id, const std::map<std::string, Amount> outputs,
      const std::string& memo = {},
      const std::vector<UnspentOutput> inputs = {}, Amount fee_rate = -1,
      bool subtract_fee_from_amount = false);

bool ExportTransaction(const std::string& wallet_id,
                               const std::string& tx_id,
                               const std::string& file_path);
Transaction ImportTransaction(const std::string& wallet_id,
                                      const std::string& file_path);
Transaction SignTransaction(const std::string& wallet_id,
                                    const std::string& tx_id,
                                    const Device& device);
Transaction BroadcastTransaction(const std::string& wallet_id,
                                         const std::string& tx_id);
Transaction GetTransaction(const std::string& wallet_id,
                                   const std::string& tx_id);
bool DeleteTransaction(const std::string& wallet_id,
                               const std::string& tx_id);

Transaction DraftTransaction(
        const std::string& wallet_id,
        const std::map<std::string, Amount> outputs,
        const std::vector<UnspentOutput> inputs = {},
        Amount fee_rate = -1,
        bool subtract_fee_from_amount = false);

Transaction ReplaceTransaction(const std::string& tx_id,
                               Amount new_fee_rate);

bool UpdateTransactionMemo(const std::string& wallet_id,
                           const std::string& tx_id,
                           const std::string& new_memo);
int GetChainTip();

bool ExportHealthCheckMessage(const std::string &message, const std::string &file_path);

std::string ImportHealthCheckSignature(const std::string &file_path);

Amount EstimateFee(int conf_target);


bool ExportUnspentOutputs( const std::string& wallet_id,
                                    const std::string& file_path,
                                    nunchuk::ExportFormat format);

bool ExportTransactionHistory(const std::string& wallet_id,
                                       const std::string& file_path,
                                       nunchuk::ExportFormat format);

void DisplayAddressOnDevice(const std::string &wallet_id,
                            const std::string &address,
                            const std::string &device_fingerprint);

nunchuk::Amount GetAddressBalance(const std::string &wallet_id, const std::string &address);

bool DeleteRemoteSigner(const std::string &master_fingerprint, const std::string &derivation_path);

bool UpdateRemoteSigner(const nunchuk::SingleSigner &remotesigner);

std::string GetSelectedWallet();

bool SetSelectedWallet(const std::string& wallet_id);

nunchuk::Transaction ImportCoboTransaction(const std::string& wallet_id,
                                           const std::vector<std::string>& qr_data);

std::vector<std::string> ExportCoboTransaction(const std::string &wallet_id,
                                               const std::string &tx_id);

std::vector<std::string> ExportCoboWallet(const std::string &wallet_id);

nunchuk::Wallet ImportCoboWallet(const std::vector<std::string>& qr_data,
                                 const std::string& description);

nunchuk::Wallet ImportWalletConfigFile( const std::string& file_path,
                                        const std::string& description);

nunchuk::MasterSigner CreateSoftwareSigner(const std::string &name,
                                           const std::string &mnemonic,
                                           const std::string &passphrase);

void SendSignerPassphrase(const std::string& mastersigner_id,
                          const std::string& passphrase);

};

#endif // USING_STUB_API

#endif // NUNCHUCKSTUB_H
