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
                                 const std::vector<nunchuk::SingleSigner>& signers,
                                 nunchuk::AddressType address_type,
                                 bool is_escrow,
                                 const std::string& desc,
                                 QWarningMessage& msg);

    nunchuk::Wallet CreateWallet(const nunchuk::Wallet& wallet,
                                 bool allow_used_signer,
                                 const std::string& decoy_pin,
                                 QWarningMessage& msg);

    nunchuk::Wallet CreateWallet(const std::string& name,
                                 int m,
                                 int n,
                                 const std::vector<nunchuk::SingleSigner>& signers,
                                 nunchuk::AddressType address_type,
                                 nunchuk::WalletType wallet_type,
                                 const std::string& description,
                                 bool allow_used_signer,
                                 const std::string& decoy_pin,
                                 nunchuk::WalletTemplate wallet_template,
                                 QWarningMessage& msg);

    std::string DraftWallet(const std::string& name,
                            int m,
                            int n,
                            const std::vector<nunchuk::SingleSigner>& signers,
                            nunchuk::AddressType address_type,
                            bool is_escrow,
                            const std::string& desc,
                            QWarningMessage& msg);

    std::vector<nunchuk::Wallet> GetWallets(QWarningMessage& msg);

    nunchuk::Wallet GetWallet(const std::string& wallet_id,
                              QWarningMessage& msg);

    bool HasWallet(const std::string& wallet_id,
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
                                                    const int index,
                                                    QWarningMessage& msg);

    nunchuk::SingleSigner GetSignerFromMasterSigner(const std::string &mastersigner_id,
                                                    const std::string &derivation_path,
                                                    QWarningMessage& msg);

    nunchuk::SingleSigner CreateSigner(const std::string& name,
                                       const std::string& xpub,
                                       const std::string& public_key,
                                       const std::string& derivation_path,
                                       const std::string& master_fingerprint,
                                       const nunchuk::SignerType &type,
                                       std::vector<nunchuk::SignerTag> tags,
                                       const bool replace,
                                       QWarningMessage& msg);
    
    int GetLastUsedSignerIndex( const std::string& xfp,
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

    void AddTapsigner(const std::string& card_ident, const std::string& xfp,
                      const std::string& name, const std::string& version,
                      int birth_height, bool is_testnet, bool replace, QWarningMessage& msg);

    int GetNumberOfSignersFromMasterSigner(const std::string& mastersigner_id,
                                           QWarningMessage& msg);

    std::vector<nunchuk::MasterSigner> GetMasterSigners(QWarningMessage& msg);

    nunchuk::MasterSigner GetMasterSigner(const std::string& mastersigner_id,
                                          QWarningMessage& msg);

    nunchuk::MasterSigner GetMasterSignerFingerprint(const std::string& fingerprint,
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

    nunchuk::SingleSigner GetRemoteSigner(const std::string& master_fingerprint,
                                          QWarningMessage& msg);

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
                                           const std::string& replace_txid,
                                           bool anti_fee_sniping,
                                           bool use_script_path,
                                           QWarningMessage& msg);

    nunchuk::Transaction DraftTransaction(const std::string& wallet_id,
                                          const std::map<std::string, nunchuk::Amount> outputs,
                                          const std::vector<nunchuk::UnspentOutput> inputs,
                                          nunchuk::Amount fee_rate,
                                          const bool subtract_fee_from_amount,
                                          const std::string &replace_txid,
                                          bool use_script_path,
                                          QWarningMessage& msg);

    nunchuk::Transaction ReplaceTransaction(const std::string &wallet_id,
                                            const std::string& tx_id,
                                            nunchuk::Amount new_fee_rate,
                                            bool anti_fee_sniping,
                                            bool use_script_path,
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

    nunchuk::Transaction UpdateTransaction(const std::string& wallet_id,
                                           const std::string& tx_id,
                                           const std::string& new_txid,
                                           const std::string& raw_tx,
                                           const std::string& reject_msg,
                                           QWarningMessage& msg);

    nunchuk::Transaction ImportPsbt(const std::string& wallet_id,
                                    const std::string& psbt,
                                    QWarningMessage& msg);

    bool ReplaceTransactionId(const std::string& wallet_id,
                              const std::string& transaction_id,
                              const std::string& replace_txid,
                              QWarningMessage& msg);

    nunchuk::Transaction SignTransaction(const std::string& wallet_id,
                                         const std::string& tx_id,
                                         const nunchuk::Device& device,
                                         QWarningMessage& msg);

    nunchuk::Transaction SignTransaction(const nunchuk::Wallet& wallet,
                                         const nunchuk::Transaction& tx,
                                         const nunchuk::Device& device,
                                         QWarningMessage& msg);

    nunchuk::Transaction BroadcastTransaction(const std::string& wallet_id,
                                              const std::string& tx_id,
                                              QWarningMessage& msg);

    nunchuk::Transaction GetTransaction(const std::string& wallet_id,
                                        const std::string& tx_id,
                                        QWarningMessage& msg);

    nunchuk::TapsignerStatus GetTapsignerStatusFromMasterSigner(const std::string& master_signer_id,
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

    std::string GetWalletExportData(const nunchuk::Wallet& wallet,
                                    nunchuk::ExportFormat format,
                                    QWarningMessage& msg);

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
                                               bool isPrimaryKey,
                                               bool replace,
                                               QWarningMessage& msg);

    nunchuk::MasterSigner CreateSoftwareSignerFromMasterXprv(const std::string &name,
                                                             const std::string &master_xprv,
                                                             bool is_primary,
                                                             bool replace,
                                                             QWarningMessage &msg);

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
    void AddBalancesListener( std::function<void(std::string, nunchuk::Amount, nunchuk::Amount)> listener);
    void AddTransactionListener( std::function<void(std::string, nunchuk::TransactionStatus, std::string)> listener);
    void AddDeviceListener( std::function<void(std::string, bool)> listener);
    void AddBlockListener( std::function<void(int , std::string)> listener);
    void AddBlockchainConnectionListener( std::function<void(nunchuk::ConnectionStatus, int)> listener);
    //For group wallet
    void AddGroupUpdateListener(std::function<void(const nunchuk::GroupSandbox& state)> listener);
    void AddGroupMessageListener(std::function<void(const nunchuk::GroupMessage& msg)> listener);
    void AddGroupOnlineListener(std::function<void(const std::string& groupId, int online)> listener);
    void AddGroupDeleteListener(std::function<void(const std::string& groupId)> listener);
    void AddReplaceRequestListener(std::function<void(const std::string& walletId, const std::string& replaceGroupId)> listener);

    nunchuk::SingleSigner ParseKeystoneSigner(const std::string& qr_data,
                                              QWarningMessage& msg);

    std::vector<std::string> ExportKeystoneWallet(const std::string& wallet_id,
                                                  QWarningMessage& msg);

    std::vector<std::string> ExportKeystoneTransaction(const std::string& wallet_id,
                                                       const std::string& tx_id,
                                                       QWarningMessage& msg);

    std::vector<std::string> ExportBCR2020010Wallet(const std::string& wallet_id,
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
                                                        nunchuk::SignerType signer_type,
                                                        QWarningMessage& msg);

    std::vector<std::string> ExportPassportWallet(const std::string& wallet_id,
                                                  QWarningMessage& msg);

    std::vector<std::string> ExportPassportTransaction(const std::string& wallet_id,
                                                       const std::string& tx_id,
                                                       QWarningMessage& msg);

    nunchuk::Transaction ImportPassportTransaction(const std::string& wallet_id,
                                                   const std::vector<std::string>& qr_data,
                                                   QWarningMessage& msg);
    void stopOneInstance();
    void stopAllInstance();

    bool UpdateTransactionSchedule(const std::string& wallet_id,
                                   const std::string& tx_id,
                                   time_t ts,
                                   QWarningMessage& msg);

    void ForceRefreshWallet(const std::string& wallet_id,
                            QWarningMessage& msg);

    // TAPSIGNER
    nunchuk::MasterSigner ImportTapsignerMasterSigner( const std::vector<unsigned char>& data,
                                                      const std::string& backup_key,
                                                      const std::string& name,
                                                      bool is_primary,
                                                      QWarningMessage& msg);

    void VerifyTapsignerBackup( const std::vector<unsigned char>& data,
                               const std::string& backup_key,
                               const std::string& master_signer_id,
                               QWarningMessage& msg);

    nunchuk::TapsignerStatus BackupTapsigner(tap_protocol::Tapsigner* tapsigner,
                                             const std::string& cvc,
                                             const std::string& master_signer_id,
                                             QWarningMessage& msg);

    nunchuk::HealthStatus HealthCheckTapsignerMasterSigner(tap_protocol::Tapsigner* tapsigner,
                                                           const std::string& cvc,
                                                           const std::string& master_signer_id,
                                                           std::string& message,
                                                           std::string& signature,
                                                           std::string& path,
                                                           QWarningMessage& msg);

    nunchuk::TapsignerStatus WaitTapsigner(tap_protocol::Tapsigner* tapsigner,
                                           std::function<bool(int)> progress,
                                           QWarningMessage& msg);

    void CacheTapsignerMasterSignerXPub(tap_protocol::Tapsigner* tapsigner,
                                        const std::string& cvc,
                                        const std::string& master_signer_id,
                                        QWarningMessage& msg);

    void CacheDefaultTapsignerMasterSignerXPub(tap_protocol::Tapsigner* tapsigner,
                                               const std::string& cvc,
                                               const std::string& master_signer_id,
                                               QWarningMessage& msg);

    std::string SignHealthCheckMessage(const nunchuk::SingleSigner& signer,
                                       const std::string& message,
                                       QWarningMessage& msg);

    nunchuk::SingleSigner GetDefaultSignerFromMasterSigner(const std::string &mastersigner_id,
                                                           const nunchuk::WalletType& wallet_type,
                                                           const nunchuk::AddressType& address_type,
                                                           QWarningMessage& msg);

    nunchuk::SingleSigner GetDefaultSignerFromMasterSigner(const std::string &mastersigner_id,
                                                           QWarningMessage& msg);

    nunchuk::SingleSigner GetSigner(const std::string& xfp,
                                    const nunchuk::WalletType& wallet_type,
                                    const nunchuk::AddressType& address_type,
                                    const int index,
                                    QWarningMessage& msg);

    nunchuk::SingleSigner GetSigner(const nunchuk::SingleSigner& signer,
                                    QWarningMessage& msg);

    bool IsCPFP(const std::string& wallet_id,
                const nunchuk::Transaction& tx,
                nunchuk::Amount& package_fee_rate,
                QWarningMessage& msg);

    bool IsMyAddress(const std::string& wallet_id,
                     const std::string& address,
                     QWarningMessage& msg);

    std::vector<nunchuk::UnspentOutput> GetUnspentOutputsFromTxInputs(const std::string& wallet_id,
                                                                      const std::vector<nunchuk::TxInput>& inputs,
                                                                      QWarningMessage &msg);

    std::string SignMessage(const nunchuk::SingleSigner& signer,
                            const std::string& message,
                            QWarningMessage& msg);

    std::string GetSignerAddress(const nunchuk::SingleSigner& signer,
                                 const nunchuk::AddressType& address_type,
                                 QWarningMessage& msg);

    std::string GetHotWalletMnemonic(const std::string& wallet_id,
                                     const std::string& passphrase,
                                     QWarningMessage& msg);

    nunchuk::Wallet CreateHotWallet(const std::string& mnemonic,
                                    const std::string& passphraser,
                                    bool need_backup,
                                    bool replace,
                                    QWarningMessage &msg);

    // Coin control
    bool UpdateCoinMemo(const std::string& wallet_id,
                        const std::string& tx_id,
                        int vout,
                        const std::string& memo,
                        QWarningMessage &msg);

    bool LockCoin(const std::string& wallet_id,
                  const std::string& tx_id,
                  int vout,
                  QWarningMessage &msg);

    bool UnlockCoin(const std::string& wallet_id,
                    const std::string& tx_id,
                    int vout,
                    QWarningMessage &msg);

    nunchuk::CoinTag CreateCoinTag(const std::string& wallet_id,
                                   const std::string& name,
                                   const std::string& color,
                                   QWarningMessage &msg);

    std::vector<nunchuk::CoinTag> GetCoinTags(const std::string& wallet_id,
                                              QWarningMessage &msg);

    bool UpdateCoinTag(const std::string& wallet_id,
                       const nunchuk::CoinTag& tag,
                       QWarningMessage &msg);

    bool DeleteCoinTag(const std::string& wallet_id,
                       int tag_id,
                       QWarningMessage &msg);

    bool AddToCoinTag(const std::string& wallet_id,
                      int tag_id,
                      const std::string& tx_id,
                      int vout,
                      QWarningMessage &msg);

    bool RemoveFromCoinTag(const std::string& wallet_id,
                           int tag_id,
                           const std::string& tx_id,
                           int vout,
                           QWarningMessage &msg);

    std::vector<nunchuk::UnspentOutput> GetCoinByTag(const std::string& wallet_id,
                                                     int tag_id,
                                                     QWarningMessage &msg);

    nunchuk::CoinCollection CreateCoinCollection(const std::string& wallet_id,
                                                 const std::string& name,
                                                 QWarningMessage &msg);

    std::vector<nunchuk::CoinCollection> GetCoinCollections(const std::string& wallet_id,
                                                            QWarningMessage &msg);

    bool UpdateCoinCollection(const std::string& wallet_id,
                              const nunchuk::CoinCollection& collection,
                              bool apply_to_existing_coins,
                              QWarningMessage &msg);

    bool DeleteCoinCollection(const std::string& wallet_id,
                              int collection_id,
                              QWarningMessage &msg);

    bool AddToCoinCollection(const std::string& wallet_id,
                             int collection_id,
                             const std::string& tx_id,
                             int vout,
                             QWarningMessage &msg);

    bool RemoveFromCoinCollection(const std::string& wallet_id,
                                  int collection_id,
                                  const std::string& tx_id,
                                  int vout,
                                  QWarningMessage &msg);

    std::vector<nunchuk::UnspentOutput> GetCoinInCollection(const std::string& wallet_id,
                                                            int collection_id,
                                                            QWarningMessage &msg);

    std::string ExportCoinControlData(const std::string& wallet_id,
                                      QWarningMessage &msg);

    bool ImportCoinControlData(const std::string& wallet_id,
                               const std::string& data,
                               bool force,
                               QWarningMessage &msg);

    std::vector<std::vector<nunchuk::UnspentOutput>> GetCoinAncestry(const std::string& wallet_id,
                                                            const std::string& tx_id,
                                                            int vout,
                                                            QWarningMessage &msg);


    std::string ExportBIP329(const std::string& wallet_id,
                             QWarningMessage &msg);
    void ImportBIP329(const std::string& wallet_id,
                      const std::string& data,
                      QWarningMessage &msg);

    std::string GetAddressPath(const std::string& wallet_id,
                               const std::string& address,
                               QWarningMessage &msg);

    bool MarkAddressAsUsed(const std::string& wallet_id,
                           const std::string& address,
                           QWarningMessage &msg);

    std::vector<nunchuk::UnspentOutput> GetCoinsFromTxInputs(
        const std::string& wallet_id,
        const std::vector<nunchuk::TxInput>& inputs,
        QWarningMessage &msg);

    std::vector<nunchuk::UnspentOutput> GetCoins(
        const std::string& wallet_id,
        QWarningMessage &msg);

    void VerifyColdcardBackup(const std::vector<unsigned char>& data,
                              const std::string& backup_key,
                              const std::string& xfp,
                              QWarningMessage &msg);

    nunchuk::MasterSigner ImportColdcardBackup(const std::vector<unsigned char>& data,
                                               const std::string& backup_key,
                                               const std::string& name,
                                               std::function<bool(int)> progress,
                                               bool is_primary,
                                               QWarningMessage &msg);

    nunchuk::MasterSigner ImportBackupKey(const std::vector<unsigned char>& data,
                                          const std::string& backup_key,
                                          const std::string& name,
                                          bool is_primary,
                                          QWarningMessage &msg);

    // For GroupWallet
    void EnableGroupWallet(const std::string& osName,
                           const std::string& osVersion,
                           const std::string& appVersion,
                           const std::string& deviceClass,
                           const std::string& deviceId,
                           const std::string& accessToken,
                           QWarningMessage &msg);

    void StartConsumeGroupEvent(QWarningMessage &msg);

    void StopConsumeGroupEvent(QWarningMessage &msg);

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

    std::pair<std::string, std::string> ParseGroupUrl(const std::string& url,
                                                      QWarningMessage &msg);

    nunchuk::GroupConfig GetGroupConfig(QWarningMessage &msg);

    nunchuk::GroupSandbox CreateGroup(const std::string& name,
                                      int m,
                                      int n,
                                      nunchuk::AddressType addressType,
                                      QWarningMessage &msg);

    int GetGroupOnline(const std::string& groupId,
                       QWarningMessage &msg);

    nunchuk::GroupSandbox GetGroup(const std::string& groupId,
                                   QWarningMessage &msg);

    std::vector<nunchuk::GroupSandbox> GetGroups(QWarningMessage &msg);

    nunchuk::GroupSandbox JoinGroup(const std::string& groupId,
                                    QWarningMessage &msg);

    void DeleteGroup(const std::string& groupId,
                     QWarningMessage &msg);

    nunchuk::GroupSandbox SetSlotOccupied(const std::string& groupId,
                                          int index,
                                          bool value,
                                          QWarningMessage &msg);

    nunchuk::GroupSandbox AddSignerToGroup(const std::string& groupId,
                                           const nunchuk::SingleSigner& signer,
                                           int index,
                                           QWarningMessage &msg);

    nunchuk::GroupSandbox RemoveSignerFromGroup(const std::string& groupId,
                                                int index,
                                                QWarningMessage &msg);

    nunchuk::GroupSandbox UpdateGroup(const std::string& groupId,
                                      const std::string& name,
                                      int m,
                                      int n,
                                      nunchuk::AddressType addressType,
                                      QWarningMessage &msg);

    nunchuk::GroupSandbox FinalizeGroup(const std::string& groupId,
                                        const std::set<size_t>& valueKeyset,
                                        QWarningMessage &msg);

    std::vector<nunchuk::Wallet> GetGroupWallets(QWarningMessage &msg);

    bool CheckGroupWalletExists(const nunchuk::Wallet& wallet,
                                QWarningMessage &msg);

    void RecoverGroupWallet(const std::string& walletId,
                            QWarningMessage &msg);

    void SetLastReadMessage(const std::string& walletId,
                            const std::string& messageId,
                            QWarningMessage &msg);

    int GetUnreadMessagesCount(const std::string& walletId,
                               QWarningMessage &msg);

    std::string GetGroupDeviceUID(QWarningMessage &msg);

    nunchuk::Amount GetScriptPathFeeRate(const std::string &wallet_id,
                                         const nunchuk::Transaction &tx,
                                         QWarningMessage &msg);

    nunchuk::GroupSandbox CreateReplaceGroup(const std::string& walletId,
                            QWarningMessage &msg);

    std::map<std::string, bool> GetReplaceGroups(const std::string& walletId,
                                             QWarningMessage &msg);

    nunchuk::GroupSandbox AcceptReplaceGroup(const std::string& walletId,
                                             const std::string& groupId,
                                                 QWarningMessage &msg);

    void DeclineReplaceGroup(const std::string& walletId,
                             const std::string& groupId,
                             QWarningMessage &msg);

    std::vector<std::string> GetDeprecatedGroupWallets(QWarningMessage &msg) const;

    std::string GetHotKeyMnemonic(const std::string& signer_id, const std::string& passphrase, QWarningMessage &msg);
private:
    nunchukiface();
    ~nunchukiface();
    void registerCallback();
    int nunchukMode_;
    std::unique_ptr<nunchuk::Nunchuk> nunchuk_instance_[2] = {NULL};
};

#endif // NUNCHUCKIFACE_H
