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

bool strCompare(const QString& str1, const QString& str2);

QStringList ExportQRTransaction(const QString &tx_to_sign, QWarningMessage &msg);

QStringList DeriveAddresses(const nunchuk::Wallet& wallet, int from_index, int to_index, QWarningMessage &msg);

int GetIndexFromPath(const QString& path);

bool IsValidAddress(const QString& address);

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
}

#endif // QUTILS_H
