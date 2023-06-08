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
#include "nunchuk.h"
#include "utils/enumconverter.hpp"
#include "QWarningMessage.h"

namespace qUtils {

qint64 QAmountFromValue(const QString &value, const bool allow_negative = false);

QString QValueFromAmount(const qint64 &amount);

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

nunchuk::Wallet ParseKeystoneWallet(nunchuk::Chain chain,
                                    const QStringList qrtags,
                                    QWarningMessage& msg);

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

bool verifyCheckSum(const QByteArray& data, const QByteArray& expectedCheckSum);

int Precision(double input);
}

#endif // QUTILS_H
