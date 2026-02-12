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

#ifndef QUTILS_H
#define QUTILS_H

#include <functional>
#include <string>
#include <vector>
#include <QObject>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <QByteArray>
#include "nunchuk.h"
#include "utils/enumconverter.hpp"
#include "utils/bip32.hpp"
#include "utils/rfc2440.hpp"
#include "QWarningMessage.h"
#include <QTimeZone>

namespace qUtils {
QByteArray aesEncrypt(const QByteArray &plaintext, const QByteArray &key, const QByteArray &iv);

QByteArray aesDecrypt(const QByteArray &ciphertext, const QByteArray &key, const QByteArray &iv);

QString deviceId();

QString deviceClass();

QString osName();

QString osVersion();

QString appVersion();

QString accessToken();

QString encryptXOR(const QString data, const QString key);

QString decryptXOR(const QString encryptedData, const QString key);

qint64 QAmountFromValue(const QString &btcValue, const bool allow_negative = false);

QString QValueFromAmount(const qint64 amount);

QString QGenerateRandomMessage(int message_length = 20);

QString QAddressToScriptPubKey(const QString& address);

QString QGetFilePath(QString in);

bool QIsValidXPub(const QString& value, QString& out);

bool QIsValidPublicKey(const QString& value);

bool QIsValidDerivationPath(const QString& value);

bool QIsValidFingerPrint(const QString& value);

QString GenerateMnemonic();

bool CheckMnemonic(const QString& mnemonic);

QStringList GetBIP39WordList();

void SetPassPhrase(const QString& storage_path,
                   const QString& account, nunchuk::Chain chain,
                   const QString& old_passphrase,
                   const QString& new_passphrase);

nunchuk::Wallet ParseWalletDescriptor(const QString& descs,
                                      QWarningMessage &msg);

nunchuk::AnalyzeQRResult AnalyzeQR(const QStringList& qrtags);

nunchuk::Wallet ParseKeystoneWallet(nunchuk::Chain chain,
                                    const QStringList qrtags,
                                    QWarningMessage& msg);

QString ParseQRTransaction(const QStringList& qrtags, QWarningMessage &msg);

std::vector<nunchuk::PrimaryKey> GetPrimaryKeys(const QString &storage_path,
                                                nunchuk::Chain chain);
QString GetMasterFingerprint(const QString& mnemonic,
                             const QString& passphrase);
QString GetPrimaryKeyAddress(const QString& mnemonic,
                             const QString& passphrase);
QString SignLoginMessage(const QString& mnemonic,
                         const QString& passphrase,
                         const QString& message);
void SetChain(nunchuk::Chain chain);

nunchuk::SignerType GetSignerType(const QString& value);

QString GetSignerTypeString(const nunchuk::SignerType value);

QString GetSHA256(const QString& value);

QString CreateRequestToken(const QString& signature, const QString& fingerprint);

QString GetPartialSignature(const nunchuk::SingleSigner& signer, const QString& signed_psbt);

QString GetHealthCheckMessage(const QString& body,
                              QWarningMessage& msg);

QString GetHealthCheckDummyTx(const nunchuk::Wallet& wallet,
                              const QString& body,
                              QWarningMessage& msg);

nunchuk::Transaction DecodeDummyTx(const nunchuk::Wallet& wallet,
                                   const QString& psbt,
                                   QWarningMessage& msg);

nunchuk::Transaction DecodeTx(const nunchuk::Wallet& wallet,
                              const QString& psbt,
                              const nunchuk::Amount& sub_amount,
                              const nunchuk::Amount& fee,
                              const nunchuk::Amount& fee_rate,
                              bool subtract_fee_from_amount, 
                              QWarningMessage& msg);

QString CreateRequestToken(const QString& signature,
                           const QString& fingerprint,
                           QWarningMessage& msg);

QString currencyLocale(qint64 amountSats);
qint64 QAmountFromCurrency(const QString &currency);

bool verifyCheckSum(const QByteArray& data, const QByteArray& expectedCheckSum);

int Precision(double input);

QJsonObject GetJsonObject(QString text);

uint GetTimeSecond(QString time_str);
uint GetCurrentTimeSecond();
QString GetTimeString(uint time_second, bool is_relative = false);

bool strCompare(const QString& str1, const QString& str2);

QStringList ExportQRTransaction(const QString &tx_to_sign, QWarningMessage &msg);

QStringList DeriveAddresses(const nunchuk::Wallet& wallet, int from_index, int to_index, QWarningMessage &msg);

int GetIndexFromPath(const QString& path);

bool IsValidAddress(const QString& address);

bool IsSilentPaymentAddress(const QString& address);

QString GetBip32DerivationPath(const nunchuk::WalletType& wallet_type, const nunchuk::AddressType& address_type);

QString ExportBitcoinSignedMessage(const QString &message, const QString &address, const QString &signature);

QString qAddressTypeToStr(const nunchuk::AddressType& address_type);

bool isPrimaryKey(const QString& xfp);

nunchuk::Wallet parseBBQRWallet(nunchuk::Chain chain, const QStringList& qrtags, QWarningMessage& msg);

std::vector<nunchuk::Wallet> ParseBBQRWallets(const QStringList& qrtags, QWarningMessage& msg);

QStringList ExportBBQRTransaction( const QString& psbt, QWarningMessage& msg);

QStringList ExportBBQRWallet(const nunchuk::Wallet& wallet, QWarningMessage& msg);

nunchuk::SingleSigner ParseSignerString(const QString key_spec, QWarningMessage& msg);

nunchuk::SingleSigner SanitizeSingleSigner(const nunchuk::SingleSigner& signer, QWarningMessage& msg);

std::vector<nunchuk::Device> GetDevices(const QString& hwi_path, QWarningMessage &msg);

QString SignPsbt(const QString& hwi_path, const nunchuk::Device& device, const QString& psbt, QWarningMessage &msg);

QString ImportDataViaFile(const QString &filepath);

void ExportDataViaFile(const QString &filepath, const QString &data);

bool isValidXPRV(const QString& xprv, QWarningMessage& msg);

QString GetPrimaryKeyAddressFromMasterXprv(
    const QString& master_xprv);

QString GetMasterFingerprintFromMasterXprv(
    const QString& master_xprv);

QString SignLoginMessageWithMasterXprv(
    const QString& master_xprv, const QString& message);

bool CheckElectrumServer(const std::string& server, int timeout = 1);

bool IsValidPolicy(const std::string& policy);

std::string PolicyToMiniscript(
    const std::string& policy,
    const std::map<std::string, nunchuk::SingleSigner>& signers,
    nunchuk::AddressType address_type);

bool IsValidMiniscriptTemplate(const std::string& miniscript_template,
                               nunchuk::AddressType address_type);

bool IsValidTapscriptTemplate(const std::string& tapscript_template,
                              std::string& error);

std::string MiniscriptTemplateToMiniscript(
    const std::string& miniscript_template,
    const std::map<std::string, nunchuk::SingleSigner>& signers);

std::string TapscriptTemplateToTapscript(
    const std::string& tapscript_template,
    const std::map<std::string, nunchuk::SingleSigner>& signers,
    std::vector<std::string>& keypath);

nunchuk::ScriptNode GetScriptNode(const std::string& script,
                                    std::vector<std::string>& keypaths);

std::vector<uint8_t> HashPreimage(const std::vector<uint8_t>& preimage,
                                  nunchuk::PreimageHashType hashType);

std::string RevealPreimage(const std::string& psbt,
                           nunchuk::PreimageHashType hashType,
                           const std::vector<uint8_t>& hash,
                           const std::vector<uint8_t>& preimage,
                           QWarningMessage &msg);

bool IsPreimageRevealed(const std::string& psbt,
                        const std::vector<uint8_t>& hash);

std::string ExpandingMultisigMiniscriptTemplate(
    int m, int n, int new_n, bool reuse_signers, const nunchuk::Timelock& timelock,
    nunchuk::AddressType address_type, QWarningMessage &msg);

std::string DecayingMultisigMiniscriptTemplate(
    int m, int n, int new_m, bool reuse_signers, const nunchuk::Timelock& timelock,
    nunchuk::AddressType address_type, QWarningMessage &msg);

std::string FlexibleMultisigMiniscriptTemplate(
    int m, int n, int new_m, int new_n, bool reuse_signers,
    const nunchuk::Timelock& timelock, nunchuk::AddressType address_type, QWarningMessage &msg);

std::string ZenHodlMiniscriptTemplate(int m, int n,
        const nunchuk::Timelock& timelock,
        nunchuk::AddressType address_type,
        QWarningMessage &msg);

std::vector<nunchuk::UnspentOutput> GetTimelockedCoins(
    const std::string& miniscript, const std::vector<nunchuk::UnspentOutput>& coins,
    int64_t& max_lock_value, int chain_tip);

std::vector<nunchuk::CoinsGroup> GetCoinsGroupedBySubPolicies(
    const nunchuk::ScriptNode& script_node, const std::vector<nunchuk::UnspentOutput>& coins,
    int chain_tip);

nunchuk::AddressType AddressTypeFromStr(const QString &str);
nunchuk::WalletTemplate WalletTemplateFromStr(const QString &str);
nunchuk::WalletType WalletTypeFromStr(const QString &str);

nunchuk::ScriptNodeId ScriptNodeIdFromString(const QString &path);
QString ScriptNodeIdToString(const nunchuk::ScriptNodeId &id);

nunchuk::SigningPath SigningPathFromStringList(const QStringList &path);
QStringList SigningPathToStringList(const nunchuk::SigningPath &path);

const nunchuk::ScriptNode* FindScriptNodeById(const nunchuk::ScriptNode& root, const nunchuk::ScriptNodeId& id);

QJsonObject JsonDeepMerge(const QJsonObject& a, const QJsonObject& b);

QVariant toVariant(const QList<int> &list);

QString getDateFromTimestamp( qint64 timestamp, const QTimeZone &tz = QTimeZone::systemTimeZone());

qint64 getDaysFromTimestamp(qint64 timestamp, const QTimeZone &tz = QTimeZone::systemTimeZone());

QString getTimeFromTimestamp(qint64 timestamp, const QTimeZone &tz = QTimeZone::systemTimeZone());

QString localTimeZoneString();

QByteArray serializeSigningPath(const nunchuk::SigningPath path);

nunchuk::SigningPath deserializeSigningPath(const QByteArray ba);

QString incrementZeroIndex(const QString &path);

int getIndexAt(const QString &path, int pos);

nunchuk::SingleSigner toSingleSigner(const nunchuk::MasterSigner& master);

QString formatMiniscript(const QString &input, int indentSize = 2, int maxInlineLen = 40);

std::string bytesToHex(const std::vector<uint8_t>& data);
std::vector<uint8_t> hexToBytes(const std::string& hex);

QString BytesToHex(const std::vector<uint8_t>& data);
std::vector<uint8_t> HexToBytes(const QString& hex);

QDateTime convertTimestampToDateTime(qint64 timestamp, const QString& timezoneId);
QString formatTimeZoneString(const QString& timezoneId);
QString extractTimeZoneId(const QString& formattedString);

int GetPurposeFromPath(const QString &path);
int GetCoinTypeFromPath(const QString &path);

QJsonObject SingleSignertoJsonObject(const nunchuk::SingleSigner &single);

bool isEmailSyntax(const QString& str);

nunchuk::BtcUri ParseBtcUri(const QString& value, QWarningMessage &msg);
}
#endif // QUTILS_H
