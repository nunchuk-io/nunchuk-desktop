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
#include "qUtils.h"
#include "AppSetting.h"
#include "AppModel.h"
#include "QOutlog.h"
#include <QCryptographicHash>

qint64 qUtils::QAmountFromValue(const QString &value, const bool allow_negative) {
    qint64 ret = -1;
    try {
        ret = nunchuk::Utils::AmountFromValue(value.toStdString(), allow_negative);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        ret = -1;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        ret = -1;
    }
    return ret;
}

QString qUtils::QValueFromAmount(const qint64 &amount) {
    QString ret = "";
    ret = QString::fromStdString(nunchuk::Utils::ValueFromAmount(amount));
    if((int)AppSetting::Unit::BTC == AppSetting::instance()->unit() && false == AppSetting::instance()->enableFixedPrecision()){
        ret.remove( QRegExp("0+$") ); // Remove any number of trailing 0's
        ret.remove( QRegExp("\\.$") ); // If the last character is just a '.' then remove it
    }
    return ret;
}

QString qUtils::QGenerateRandomMessage(int message_length)
{
    return QString::fromStdString(nunchuk::Utils::GenerateRandomMessage(message_length));
}

QString qUtils::QAddressToScriptPubKey(const QString &address)
{
    try {
        return QString::fromStdString(nunchuk::Utils::AddressToScriptPubKey(address.toStdString()));
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        return QString("[%1] %2").arg(ex.code()).arg(ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        return e.what();
    }
}

QString qUtils::QGetFilePath(QString in)
{
    if(in.isEmpty() || in == ""){
        return "";
    }
#ifdef _WIN32
    return in.remove("file:///");
#else
    return in.remove("file://");
#endif
}

bool qUtils::QIsValidXPub(const QString &value, QString &out)
{
    bool valid = false;
    std::string target_format = "";
    if((int)AppSetting::Chain::MAIN == AppSetting::instance()->primaryServer()){
        target_format = "xpub";
    }
    else{
        target_format = "tpub";
    }

    try {
        std::string ret = nunchuk::Utils::SanitizeBIP32Input(value.toStdString(), target_format);
        valid = nunchuk::Utils::IsValidXPub(ret);
        if(valid){out = QString::fromStdString(ret);}
    }  catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        valid = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        valid = false;
    }
    if(!valid){out = "";}
    return valid;
}

bool qUtils::QIsValidPublicKey(const QString &value)
{
    return nunchuk::Utils::IsValidPublicKey(value.toStdString());
}

bool qUtils::QIsValidDerivationPath(const QString &value)
{
    return nunchuk::Utils::IsValidDerivationPath(value.toStdString());
}

bool qUtils::QIsValidFingerPrint(const QString &value)
{
    return nunchuk::Utils::IsValidFingerPrint(value.toStdString());
}

QString qUtils::GenerateMnemonic()
{
    return QString::fromStdString(nunchuk::Utils::GenerateMnemonic());
}

bool qUtils::CheckMnemonic(const QString &mnemonic)
{
    return nunchuk::Utils::CheckMnemonic(mnemonic.toStdString());
}

QStringList qUtils::GetBIP39WordList()
{
    QStringList result;
    result.clear();
    std::vector<std::string> ret = nunchuk::Utils::GetBIP39WordList();
    for (std::string word : ret) {
        result << QString::fromStdString(word);
    }
    return result;
}

void qUtils::SetPassPhrase(const QString &storage_path,
                           const QString &account,
                           nunchuk::Chain chain,
                           const QString &old_passphrase,
                           const QString &new_passphrase)
{
    nunchuk::Utils::SetPassPhrase(storage_path.toStdString(),
                                  account.toStdString(),
                                  chain,
                                  old_passphrase.toStdString(),
                                  new_passphrase.toStdString());
}

nunchuk::Wallet qUtils::ParseWalletDescriptor(const QString &descs,
                                              QWarningMessage& msg)
{
    nunchuk::Wallet ret(false);
    try {
        ret = nunchuk::Utils::ParseWalletDescriptor(descs.toStdString());
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Wallet qUtils::ParseKeystoneWallet(nunchuk::Chain chain, const QStringList qrtags, QWarningMessage& msg)
{
    nunchuk::Wallet ret(false);
    std::vector<std::string> qr_result;
    for (QString it : qrtags) {
        qr_result.push_back(it.toStdString());
    }
    try {
        ret = nunchuk::Utils::ParseKeystoneWallet(chain,qr_result);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::PrimaryKey> qUtils::GetPrimaryKeys(const QString &storage_path, nunchuk::Chain chain)
{
    return nunchuk::Utils::GetPrimaryKeys(storage_path.toStdString(),chain);
}

QString qUtils::GetMasterFingerprint(const QString& mnemonic, const QString& passphrase)
{
    return QString::fromStdString(nunchuk::Utils::GetMasterFingerprint(mnemonic.toStdString(),passphrase.toStdString()));
}

QString qUtils::GetPrimaryKeyAddress(const QString& mnemonic, const QString& passphrase)
{
    return QString::fromStdString(nunchuk::Utils::GetPrimaryKeyAddress(mnemonic.toStdString(),passphrase.toStdString()));
}

QString qUtils::SignLoginMessage(const QString &mnemonic, const QString &passphrase, const QString &message)
{
    return QString::fromStdString(nunchuk::Utils::SignLoginMessage(mnemonic.toStdString(),passphrase.toStdString(),message.toStdString()));
}

void qUtils::SetChain(nunchuk::Chain chain)
{
    nunchuk::Utils::SetChain(chain);
}

nunchuk::SignerType qUtils::GetSignerType(const QString &value)
{
    nunchuk::SignerType ret = nunchuk::SignerType::UNKNOWN;
    try {
        ret = SignerTypeFromStr(value.toStdString());
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::GetSignerTypeString(const nunchuk::SignerType value)
{
    QString ret = "UNKNOWN";
    try {
        ret = QString::fromStdString(SignerTypeToStr(value));
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::GetSHA256(const QString &value)
{
    QString ret = value;
    try {
        ret = QString::fromStdString(nunchuk::Utils::SHA256(value.toStdString()));
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        QByteArray bytes = QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Sha256);
        ret = QString(bytes.toHex());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        QByteArray bytes = QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Sha256);
        ret = QString(bytes.toHex());
    }
    return ret;
}

QString qUtils::CreateRequestToken(const QString &signature, const QString &fingerprint)
{
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::CreateRequestToken(signature.toStdString(), fingerprint.toStdString()));
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::GetPartialSignature(const nunchuk::SingleSigner &signer, const QString &signed_psbt)
{
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::GetPartialSignature(signer, signed_psbt.toStdString()));
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::GetHealthCheckMessage(const QString &body,
                                      QWarningMessage &msg)
{
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::GetHealthCheckMessage(body.toStdString()));
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::GetHealthCheckDummyTx(const nunchuk::Wallet &wallet,
                                      const QString &body,
                                      QWarningMessage &msg)
{
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::GetHealthCheckDummyTx(wallet, body.toStdString()));
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction qUtils::DecodeDummyTx(const nunchuk::Wallet &wallet,
                                           const QString &psbt,
                                           QWarningMessage &msg)
{
    nunchuk::Transaction ret;
    try {
        ret = nunchuk::Utils::DecodeDummyTx(wallet, psbt.toStdString());
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction qUtils::DecodeTx(const nunchuk::Wallet &wallet,
                                      const QString &psbt,
                                      const nunchuk::Amount &sub_amount,
                                      const nunchuk::Amount &fee,
                                      const nunchuk::Amount &fee_rate,
                                      QWarningMessage &msg)
{
    nunchuk::Transaction ret;
    try {
        ret = nunchuk::Utils::DecodeTx(wallet, psbt.toStdString(), sub_amount, fee, fee_rate);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::CreateRequestToken(const QString &signature,
                                   const QString &fingerprint,
                                   QWarningMessage &msg)
{
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::CreateRequestToken(signature.toStdString(), fingerprint.toStdString()));
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::currencyLocale(qint64 amountSats)
{
    double btRates = AppModel::instance()->btcRates()/100000000;
    double exRates = AppModel::instance()->exchangeRates();
    double balanceCurrency = btRates*exRates*amountSats;
    QLocale locale(QLocale::English);
    QString output = locale.toString(balanceCurrency, 'f', 8);
    while (output.endsWith('0') && output.contains('.')) {
        output = output.chopped(1);
    }
    if (output.endsWith('.')) {
        output = output.chopped(1);
    }
    return output;
}
