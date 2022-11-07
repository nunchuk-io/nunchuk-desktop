#ifndef NUNCHUCKIFACE_H
#define NUNCHUCKIFACE_H

#include "QWarningMessage.h"
#include <nunchuk.h>
#include <map>

enum nunchukMode {
    LOCAL_MODE = 0,
    ONLINE_MODE
};

class nunchukiface
{
public:
    static nunchukiface *instance();
    void setNunchukMode(int mode);
    int nunchukMode() const;

    nunchukiface(nunchukiface &other) = delete;
    nunchukiface(nunchukiface const &other) = delete;
    void operator=(const nunchukiface &other) = delete;

    void makeNunchukInstance(const nunchuk::AppSettings &appsettings,
                             const std::string &passphrase,
                             QWarningMessage &msg);

    void makeNunchukInstanceForAccount(const nunchuk::AppSettings& appsettings,
                                       const std::string& passphrase,
                                       const std::string& account,
                                       QWarningMessage &msg);

    const std::unique_ptr<nunchuk::Nunchuk> &nunchukinstance() const;

    bool SetPassphrase(const std::string& passphrase, QWarningMessage &msg);

    nunchuk::Wallet CreateWallet(const std::string& name,
                                 int m,
                                 int n,
                                 std::vector<nunchuk::SingleSigner>& signers,
                                 nunchuk::AddressType address_type,
                                 bool is_escrow,
                                 const std::string& desc,
                                 QWarningMessage& msg);

    std::string DraftWallet(const std::string& name,
                            int m,
                            int n,
                            std::vector<nunchuk::SingleSigner>& signers,
                            nunchuk::AddressType address_type,
                            bool is_escrow,
                            const std::string& desc,
                            QWarningMessage& msg);

    std::vector<nunchuk::Wallet> GetWallets(QWarningMessage& msg);

    nunchuk::Wallet GetWallet(const std::string& wallet_id,
                              QWarningMessage& msg);

    bool DeleteWallet(const std::string& wallet_id, QWarningMessage& msg);

    bool UpdateWallet(const nunchuk::Wallet &wallet, QWarningMessage& msg);

    bool ExportWallet(const std::string& wallet_id,
                      const std::string& file_path,
                      nunchuk::ExportFormat format,
                      QWarningMessage& msg);

    nunchuk::Wallet ImportWalletDb(const std::string& file_path,
                                   QWarningMessage& msg);

    nunchuk::Wallet ImportWalletDescriptor(const std::string& file_path,
                                           const std::string& name,
                                           const std::string& description,
                                           QWarningMessage& msg);

    std::vector<nunchuk::Device> GetDevices(QWarningMessage& msg);

    nunchuk::MasterSigner CreateMasterSigner(const std::string& name,
                                             const nunchuk::Device& device,
                                             QWarningMessage& msg);

    nunchuk::SingleSigner GetSignerFromMasterSigner(const std::string& mastersigner_id,
                                                    const nunchuk::WalletType& wallet_type,
                                                    const nunchuk::AddressType& address_type,
                                                    int index,
                                                    QWarningMessage& msg);

    nunchuk::SingleSigner CreateSigner(const std::string& name,
                                       const std::string& xpub,
                                       const std::string& public_key,
                                       const std::string& derivation_path,
                                       const std::string& master_fingerprint,
                                       QWarningMessage& msg);

    int GetCurrentIndexFromMasterSigner( const std::string& mastersigner_id,
                                         const nunchuk::WalletType& wallet_type,
                                         const nunchuk::AddressType& address_type,
                                         QWarningMessage& msg);

    nunchuk::SingleSigner GetUnusedSignerFromMasterSigner( const std::string& mastersigner_id,
                                                           const nunchuk::WalletType& wallet_type,
                                                           const nunchuk::AddressType& address_type,
                                                           QWarningMessage& msg);

    std::vector<nunchuk::SingleSigner> GetSignersFromMasterSigner(const std::string& mastersigner_id,
                                                                  QWarningMessage& msg);

    bool HasSigner(const nunchuk::SingleSigner& signer,QWarningMessage& msg);

    int GetNumberOfSignersFromMasterSigner(const std::string& mastersigner_id,
                                           QWarningMessage& msg);

    std::vector<nunchuk::MasterSigner> GetMasterSigners(QWarningMessage& msg);

    nunchuk::MasterSigner GetMasterSigner(const std::string& mastersigner_id,
                                          QWarningMessage& msg);

    bool DeleteMasterSigner(const std::string& mastersigner_id,
                            QWarningMessage& msg);

    bool DeleteRemoteSigner(const std::string& master_fingerprint,
                            const std::string& derivation_path,
                            QWarningMessage& msg);

    bool UpdateRemoteSigner(const nunchuk::SingleSigner& remotesigner,
                            QWarningMessage &msg);


    bool UpdateMasterSigner(const nunchuk::MasterSigner &mastersigner,
                            QWarningMessage& msg);

    std::vector<nunchuk::SingleSigner> GetRemoteSigners(QWarningMessage& msg);

    std::string GetHealthCheckPath();

    nunchuk::HealthStatus HealthCheckMasterSigner(const std::string& fingerprint,
                                                  std::string &message,
                                                  std::string &signature,
                                                  std::string &path,
                                                  QWarningMessage& msg);

    nunchuk::HealthStatus HealthCheckSingleSigner(const nunchuk::SingleSigner& signer,
                                                  const std::string& message,
                                                  const std::string& signature,
                                                  QWarningMessage& msg);

    std::vector<nunchuk::Transaction> GetTransactionHistory(const std::string& wallet_id,
                                                            int count,
                                                            int skip,
                                                            QWarningMessage& msg);

    nunchuk::AppSettings GetAppSettings(QWarningMessage& msg);

    nunchuk::AppSettings UpdateAppSettings(nunchuk::AppSettings& appSettings,
                                           QWarningMessage& msg);

    std::vector<std::string> GetAddresses(const std::string& wallet_id,
                                          bool used,
                                          bool internal,
                                          QWarningMessage& msg);

    std::string NewAddress(const std::string& wallet_id, bool internal, QWarningMessage& msg);

    std::vector<nunchuk::UnspentOutput> GetUnspentOutputs(const std::string& wallet_id,
                                                          QWarningMessage& msg);

    nunchuk::Transaction CreateTransaction(const std::string& wallet_id,
                                           const std::map<std::string, nunchuk::Amount> outputs,
                                           const std::string& memo,
                                           const std::vector<nunchuk::UnspentOutput> inputs,
                                           nunchuk::Amount fee_rate,
                                           bool subtract_fee_from_amount,
                                           QWarningMessage& msg);

    nunchuk::Transaction DraftTransaction(const std::string& wallet_id,
                                          const std::map<std::string, nunchuk::Amount> outputs,
                                          const std::vector<nunchuk::UnspentOutput> inputs,
                                          nunchuk::Amount fee_rate,
                                          const bool subtract_fee_from_amount,
                                          QWarningMessage& msg);

    nunchuk::Transaction ReplaceTransaction(const std::string &wallet_id,
                                            const std::string& tx_id,
                                            nunchuk::Amount new_fee_rate,
                                            QWarningMessage& msg);

    bool UpdateTransactionMemo(const std::string& wallet_id,
                               const std::string& tx_id,
                               const std::string& new_memo,
                               QWarningMessage& msg);

    bool ExportTransaction(const std::string& wallet_id,
                           const std::string& tx_id,
                           const std::string& file_path,
                           QWarningMessage& msg);

    nunchuk::Transaction ImportTransaction(const std::string& wallet_id,
                                           const std::string& file_path,
                                           QWarningMessage& msg);

    nunchuk::Transaction SignTransaction(const std::string& wallet_id,
                                         const std::string& tx_id,
                                         const nunchuk::Device& device,
                                         QWarningMessage& msg);

    nunchuk::Transaction BroadcastTransaction(const std::string& wallet_id,
                                              const std::string& tx_id,
                                              QWarningMessage& msg);

    nunchuk::Transaction GetTransaction(const std::string& wallet_id,
                                        const std::string& tx_id,
                                        QWarningMessage& msg);

    bool DeleteTransaction(const std::string& wallet_id,
                           const std::string& tx_id,
                           QWarningMessage& msg);

    void CacheMasterSignerXPub(const std::string& mastersigner_id, QWarningMessage& msg);

    int GetChainTip(QWarningMessage& msg);

    bool ExportHealthCheckMessage(const std::string& message, const std::string& file_path, QWarningMessage& msg);

    std::string ImportHealthCheckSignature( const std::string& file_path, QWarningMessage& msg);

    nunchuk::Amount EstimateFee(int conf_target, QWarningMessage& msg);

    bool ExportUnspentOutputs( const std::string& wallet_id,
                               const std::string& file_path,
                               nunchuk::ExportFormat format,
                               QWarningMessage& msg);

    bool ExportTransactionHistory(const std::string& wallet_id,
                                  const std::string& file_path,
                                  nunchuk::ExportFormat format,
                                  QWarningMessage& msg);

    void DisplayAddressOnDevice( const std::string& wallet_id,
                                 const std::string& address,
                                 const std::string& device_fingerprint,
                                 QWarningMessage& msg);

    nunchuk::Amount GetAddressBalance(const std::string& wallet_id,
                                      const std::string& address,
                                      QWarningMessage& msg);

    std::string GetSelectedWallet(QWarningMessage& msg);

    void PromtPinOnDevice(const nunchuk::Device& device, QWarningMessage& msg);

    void SendPinToDevice(const nunchuk::Device& device, const std::string& pin, QWarningMessage& msg);

    void SendPassphraseToDevice(const nunchuk::Device& device,
                                const std::string& passphrase,
                                QWarningMessage& msg);

    bool SetSelectedWallet(const std::string& wallet_id, QWarningMessage& msg);

    nunchuk::SingleSigner CreateCoboSigner(const std::string& name,
                                           const std::string& json_info,
                                           QWarningMessage& msg);

    std::vector<std::string> ExportCoboTransaction( const std::string& wallet_id,
                                                    const std::string& tx_id,
                                                    QWarningMessage& msg);

    nunchuk::Transaction ImportCoboTransaction(const std::string& wallet_id,
                                               const std::vector<std::string>& qr_data,
                                               QWarningMessage& msg);

    std::vector<std::string> ExportCoboWallet( const std::string& wallet_id,
                                               QWarningMessage& msg);


    nunchuk::Wallet ImportCoboWallet(const std::vector<std::string>& qr_data,
                                     const std::string& description,
                                     QWarningMessage& msg);

    nunchuk::Wallet ImportWalletConfigFile( const std::string& file_path,
                                            const std::string& description,
                                            QWarningMessage& msg);

    nunchuk::MasterSigner CreateSoftwareSigner(const std::string& name,
                                               const std::string& mnemonic,
                                               const std::string &passphrase,
                                               QWarningMessage& msg,
                                               bool isPrimaryKey);

    bool DeletePrimaryKey();

    std::string SignLoginMessage(const std::string& mastersigner_id,
                                 const std::string& message,
                                 QWarningMessage& msg);

    void SendSignerPassphrase(const std::string& mastersigner_id,
                              const std::string& passphrase,
                              QWarningMessage& msg );


    void ClearSignerPassphrase(const std::string& mastersigner_id,
                               QWarningMessage& msg);

    void RescanBlockchain(int start_height,
                          int stop_height,
                          QWarningMessage& msg);

    void AddBalanceListener( std::function<void(std::string, nunchuk::Amount)> listener);

    void AddTransactionListener( std::function<void(std::string, nunchuk::TransactionStatus, std::string)> listener);

    void AddDeviceListener( std::function<void(std::string, bool)> listener);

    void AddBlockListener( std::function<void(int , std::string)> listener);

    void AddBlockchainConnectionListener( std::function<void(nunchuk::ConnectionStatus, int)> listener);

    nunchuk::SingleSigner ParseKeystoneSigner(const std::string& qr_data,
                                              QWarningMessage& msg);

    std::vector<std::string> ExportKeystoneWallet(const std::string& wallet_id,
                                                  QWarningMessage& msg);

    std::vector<std::string> ExportKeystoneTransaction(const std::string& wallet_id,
                                                       const std::string& tx_id,
                                                       QWarningMessage& msg);

    nunchuk::Transaction ImportKeystoneTransaction(const std::string& wallet_id,
                                                   const std::vector<std::string>& qr_data,
                                                   QWarningMessage& msg);

    nunchuk::Wallet ImportKeystoneWallet(const std::vector<std::string>& qr_data,
                                         const std::string& description,
                                         QWarningMessage& msg);

    std::vector<nunchuk::SingleSigner> ParsePassportSigners(const std::vector<std::string>& qr_data,
                                                            QWarningMessage& msg);

    std::vector<nunchuk::SingleSigner> ParseQRSigners(const std::vector<std::string>& qr_data,
                                                            QWarningMessage& msg);

    std::vector<nunchuk::SingleSigner> ParseJSONSigners(const std::string& json_str,
                                                            QWarningMessage& msg);

    std::vector<std::string> ExportPassportWallet(const std::string& wallet_id,
                                                  QWarningMessage& msg);

    std::vector<std::string> ExportPassportTransaction(const std::string& wallet_id,
                                                       const std::string& tx_id,
                                                       QWarningMessage& msg);

    nunchuk::Transaction ImportPassportTransaction(const std::string& wallet_id,
                                                   const std::vector<std::string>& qr_data,
                                                   QWarningMessage& msg);
    void stopInstance();
private:
    nunchukiface();
    ~nunchukiface();
    void registerCallback();
    int nunchukMode_;
    std::unique_ptr<nunchuk::Nunchuk> nunchuk_instance_[2] = {NULL};
};

#endif // NUNCHUCKIFACE_H
