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
#include <QJsonDocument>
#include <boost/algorithm/string.hpp>

QString qUtils::encryptXOR(const QString data, const QString key) {
    if(key == ""){
        return data;
    }

    QString encryptedData;
    for (int i = 0; i < data.length(); ++i) {
        encryptedData += QChar(data.at(i).unicode() ^ key.at(i % key.length()).unicode());
    }
    return encryptedData;
}

QString qUtils::decryptXOR(const QString encryptedData, const QString key) {
    return qUtils::encryptXOR(encryptedData, key);
}

qint64 qUtils::QAmountFromValue(const QString &btcValue, const bool allow_negative) {
    qint64 ret = -1;
    try {
        ret = nunchuk::Utils::AmountFromValue(btcValue.toStdString(), allow_negative);
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
    QStringList in = qrtags;
    in.removeDuplicates();
    nunchuk::Wallet ret(false);
    std::vector<std::string> qr_result;
    for (QString tag : in) {
        qr_result.push_back(tag.toStdString());
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

QString qUtils::ParseQRTransaction(const QStringList &qrtags, QWarningMessage& msg)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    std::string psbt = "";
    std::vector<std::string> qr_result;
    for (QString tag : in) {
        qr_result.push_back(tag.toStdString());
    }
    try {
        psbt = nunchuk::Utils::ParseKeystoneTransaction(qr_result);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return QString::fromStdString(psbt);
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
    double btcRates = AppModel::instance()->btcRates()/100000000;
    double exRates = AppModel::instance()->exchangeRates();
    double balanceCurrency = btcRates*exRates*amountSats;
    QLocale locale(QLocale::English);
    QString output = locale.toString(balanceCurrency, 'f', 2);
#if 0
    while (output.endsWith('0') && output.contains('.')) {
        output = output.chopped(1);
    }
#endif
    if (output.endsWith('.')) {
        output = output.chopped(1);
    }
    return output;
}

qint64 qUtils::QAmountFromCurrency(const QString &currency)
{
    double btcRates = AppModel::instance()->btcRates()/100000000;
    double exRates = AppModel::instance()->exchangeRates();
    double rate = btcRates*exRates;
    double amountSats = currency.toDouble() / rate;
    return static_cast<qint64>(amountSats);
}

bool qUtils::verifyCheckSum(const QByteArray &data, const QByteArray &expectedCheckSum)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(data);
    QByteArray actualCheckSum = hash.result();
    return (actualCheckSum == expectedCheckSum);
}

int qUtils::Precision(double input)
{
    const int maxDigits = 15;
    double remaining = input;
    int lastDigit = 0;
    for (int i = 1; i <= maxDigits; i++) {
        remaining = (remaining * 10);
        int round = qRound(remaining) % 10;
        if (round != 0) {
            if (round < 0) break;
            lastDigit = i;
        }
    }
    return qMax(lastDigit,2);
}

QJsonObject qUtils::GetJsonObject(QString text)
{
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object";
        }
    }
    else
    {
        qDebug() << "Invalid JSON...\n";
    }
    return obj;
}

uint qUtils::GetTimeSecond(QString time_str)
{
    QStringList list = time_str.split("/");
    if (list.size() < 2) return 0;
    QString month = list.at(0);
    QString day = list.at(1);
    QString year = list.at(2);
    QDate date(year.toInt(), month.toInt(), day.toInt());
    QDateTime time(date);
    return time.toTime_t();
}

uint qUtils::GetCurrentTimeSecond()
{
    return QDateTime::currentDateTime().toTime_t();
}

bool qUtils::strCompare(const QString &str1, const QString &str2)
{
    return 0 == QString::compare(str1, str2, Qt::CaseInsensitive);
}

nunchuk::AnalyzeQRResult qUtils::AnalyzeQR(const QStringList &qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    nunchuk::AnalyzeQRResult result;
    std::vector<std::string> qr_result;
    QWarningMessage msg;
    for (QString tag : in) {
        qr_result.push_back(tag.toStdString());
    }
    try {
        result = nunchuk::Utils::AnalyzeQR(qr_result);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

QStringList qUtils::ExportQRTransaction(const QString &tx_to_sign, QWarningMessage &msg)
{
    QStringList result {};
    try {
        std::vector<std::string> data = nunchuk::Utils::ExportKeystoneTransaction(tx_to_sign.toStdString());
        result.reserve(data.size());
        for (std::string tag : data) {
            result.append(QString::fromStdString(tag));
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

QStringList qUtils::DeriveAddresses(const nunchuk::Wallet &wallet, int from_index, int to_index, QWarningMessage &msg)
{
    QStringList result {};
    try {
        std::vector<std::string> data = nunchuk::Utils::DeriveAddresses(wallet, from_index, to_index);
        result.reserve(data.size());
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

int qUtils::GetIndexFromPath(const QString &path)
{
    QWarningMessage msg;
    int index = -1;
    try {
        index = nunchuk::Utils::GetIndexFromPath(path.toStdString());
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return index;
}

bool qUtils::IsValidAddress(const QString &address)
{
    QWarningMessage msg;
    bool ret {false};
    try {
        ret = nunchuk::Utils::IsValidAddress(address.toStdString());
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

QString qUtils::GetBip32DerivationPath(const nunchuk::WalletType& wallet_type, const nunchuk::AddressType& address_type)
{
    QWarningMessage msg;
    std::string path = "";
    try {
        nunchuk::Chain chain = static_cast<nunchuk::Chain>(AppSetting::instance()->primaryServer());
        path = GetBip32Path(chain, wallet_type, address_type, 0);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return QString::fromStdString(path);
}

QString qUtils::ExportBitcoinSignedMessage(const QString &message, const QString &address, const QString &signature)
{
    nunchuk::BitcoinSignedMessage btc(message.toStdString(), address.toStdString(), signature.toStdString());
    return QString::fromStdString(nunchuk::ExportBitcoinSignedMessage(btc));
}

QString qUtils::qAddressTypeToStr(const nunchuk::AddressType &address_type)
{
    return QString::fromStdString(AddressTypeToStr(address_type));
}

bool qUtils::isPrimaryKey(const QString &xfp)
{
    nunchuk::PrimaryKey key = AppModel::instance()->findPrimaryKey(xfp);
    if(key.get_master_fingerprint() != ""){
        return true;
    }
    return false;
}

nunchuk::Wallet qUtils::parseBBQRWallet(nunchuk::Chain chain, const QStringList &qrtags, QWarningMessage &msg)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    nunchuk::Wallet ret(false);
    std::vector<std::string> qr_result;
    for (QString tag : in) {
        qr_result.push_back(tag.toStdString());
    }
    try {
        // ret = nunchuk::Utils::ParseBBQRWallet(chain, qr_result);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::Wallet> qUtils::ParseBBQRWallets(const QStringList &qrtags, QWarningMessage &msg)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    std::vector<nunchuk::Wallet> ret;
    std::vector<std::string> qr_result;
    for (QString tag : in) {
        qr_result.push_back(tag.toStdString());
    }
    try {
        ret = nunchuk::Utils::ParseBBQRWallets(qr_result);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QStringList qUtils::ExportBBQRTransaction(const QString &psbt, QWarningMessage &msg)
{
    QStringList result {};
    try {
        std::vector<std::string> data = nunchuk::Utils::ExportBBQRTransaction(psbt.toStdString());
        result.reserve(data.size());
        for (std::string tag : data) {
            result.append(QString::fromStdString(tag));
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

QStringList qUtils::ExportBBQRWallet(const nunchuk::Wallet &wallet, QWarningMessage &msg)
{
    QStringList result {};
    try {
        std::vector<std::string> data = nunchuk::Utils::ExportBBQRWallet(wallet);
        result.reserve(data.size());
        for (std::string tag : data) {
            result.append(QString::fromStdString(tag));
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

nunchuk::SingleSigner qUtils::ParseSignerString(const QString key_spec, QWarningMessage &msg)
{
    nunchuk::SingleSigner ret {};
    static std::regex SIGNER_REGEX("\\[([0-9a-fA-F]{8})(.+)\\](.+?)(/.*\\*)?\n?");
    std::string key_spec_str = key_spec.toStdString();
    std::smatch sm;
    if (std::regex_match(key_spec_str, sm, SIGNER_REGEX)) {
        const std::string xfp = boost::algorithm::to_lower_copy(sm[1].str());
        if (sm[3].str().rfind("tpub", 0) == 0 ||
            sm[3].str().rfind("xpub", 0) == 0) {
            try {
                ret = nunchuk::SingleSigner(sm[1], sm[3], {}, "m" + sm[2].str(), xfp, 0);
            }
            catch (const nunchuk::BaseException &ex) {
                DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
                msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
            }
            catch (std::exception &e) {
                DBG_INFO << "THROW EXCEPTION" << e.what();
                msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
            }
        }
        else {
            try {
                ret = nunchuk::SingleSigner(sm[1], {}, sm[3], "m" + sm[2].str(), xfp, 0);
            }
            catch (const nunchuk::BaseException &ex) {
                DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
                msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
            }
            catch (std::exception &e) {
                DBG_INFO << "THROW EXCEPTION" << e.what();
                msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
            }
        }
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            ret = qUtils::SanitizeSingleSigner(ret, msg);
        }
    }
    else{
        msg.setWarningMessage(nunchuk::NunchukException::INVALID_PARAMETER,
                              "Could not parse descriptor. Please check your input and try again.",
                              EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::SingleSigner qUtils::SanitizeSingleSigner(const nunchuk::SingleSigner &signer, QWarningMessage &msg)
{
    nunchuk::SingleSigner ret {};
    try {
        ret = nunchuk::Utils::SanitizeSingleSigner(signer);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::Device> qUtils::GetDevices(const QString &hwi_path, QWarningMessage &msg)
{
    std::vector<nunchuk::Device> ret {};
    try {
        ret = nunchuk::Utils::GetDevices(hwi_path.toStdString());
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::SignPsbt(const QString &hwi_path, const nunchuk::Device &device, const QString &psbt, QWarningMessage &msg)
{
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::SignPsbt(hwi_path.toStdString(), device, psbt.toStdString()));
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::ImportDataViaFile(const QString &filepath)
{
    if (QFile::exists(filepath)) {
        QFile file(filepath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();
            file.close();
            return fileContent;
        }
    }
    return "";
}

void qUtils::ExportDataViaFile(const QString &filepath, const QString &data)
{
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream st(&file);
        st.setCodec("UTF-8");
        st << data << endl;
        st.flush();
        file.close();
    }
}

bool qUtils::isValidXPRV(const QString &xprv, QWarningMessage &msg)
{
    bool ret = false;
    try {
        ret = nunchuk::Utils::IsValidXPrv(xprv.toStdString());
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}
