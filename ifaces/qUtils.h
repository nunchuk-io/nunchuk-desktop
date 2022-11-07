#ifndef QUTILS_H
#define QUTILS_H

#include <functional>
#include <string>
#include <vector>
#include <QObject>
#include "nunchuk.h"
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
                   const QString& account,
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
}

#endif // QUTILS_H
