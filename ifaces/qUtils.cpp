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
#include "AppModel.h"
#include "AppSetting.h"
#include "QOutlog.h"
#include "ifaces/Servers/Draco.h"
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QHash>
#include <boost/algorithm/string.hpp>
#include "utils/enumconverter.hpp"

QString qUtils::deviceId() {
    QString deviceId = QSysInfo::machineUniqueId();
    if (deviceId.isEmpty()) {
        deviceId = QSysInfo::machineHostName(); // Dự phòng
    }
    return deviceId;
}

QString qUtils::deviceClass() {
#if defined(Q_OS_MACOS)
    return "Desktop-MACOS";
#elif defined(Q_OS_WINDOWS)
    return "Desktop-WINDOWS";
#elif defined(Q_OS_LINUX)
    return "Desktop-LINUX";
#else
    return "Desktop";
#endif
}

QString qUtils::osName() {
    return QSysInfo::prettyProductName();
}

QString qUtils::osVersion() {
    return QSysInfo::kernelVersion();
}

QString qUtils::appVersion() {
    return qApp->applicationVersion();
}

QString qUtils::accessToken() {
    return Draco::instance()->dracoToken();
}

QString qUtils::encryptXOR(const QString data, const QString key) {
    if (key == "") {
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
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        ret = -1;
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        ret = -1;
    }
    return ret;
}

QString qUtils::QValueFromAmount(const qint64 amount) {
    // DBG_INFO << amount << INT64_MAX;
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::ValueFromAmount(amount));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        ret = "";
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        ret = "";
    }
    if ((int)AppSetting::Unit::BTC == AppSetting::instance()->unit() && false == AppSetting::instance()->enableFixedPrecision()) {
        ret.remove(QRegExp("0+$"));  // Remove any number of trailing 0's
        ret.remove(QRegExp("\\.$")); // If the last character is just a '.' then remove it
    }
    return ret;
}

QString qUtils::QGenerateRandomMessage(int message_length) {
    return QString::fromStdString(nunchuk::Utils::GenerateRandomMessage(message_length));
}

QString qUtils::QAddressToScriptPubKey(const QString &address) {
    try {
        return QString::fromStdString(nunchuk::Utils::AddressToScriptPubKey(address.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        return QString("[%1] %2").arg(ex.code()).arg(ex.what());
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        return e.what();
    }
}

QString qUtils::QGetFilePath(QString in) {
    if (in.isEmpty() || in == "") {
        return "";
    }
#ifdef _WIN32
    return in.remove("file:///");
#else
    return in.remove("file://");
#endif
}

bool qUtils::QIsValidXPub(const QString &value, QString &out) {
    bool valid = false;
    std::string target_format = "";
    if ((int)AppSetting::Chain::MAIN == AppSetting::instance()->primaryServer()) {
        target_format = "xpub";
    } else {
        target_format = "tpub";
    }

    try {
        std::string ret = nunchuk::Utils::SanitizeBIP32Input(value.toStdString(), target_format);
        valid = nunchuk::Utils::IsValidXPub(ret);
        if (valid) {
            out = QString::fromStdString(ret);
        }
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        valid = false;
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        valid = false;
    }
    if (!valid) {
        out = "";
    }
    return valid;
}

bool qUtils::QIsValidPublicKey(const QString &value) {
    return nunchuk::Utils::IsValidPublicKey(value.toStdString());
}

bool qUtils::QIsValidDerivationPath(const QString &value) {
    return nunchuk::Utils::IsValidDerivationPath(value.toStdString());
}

bool qUtils::QIsValidFingerPrint(const QString &value) {
    return nunchuk::Utils::IsValidFingerPrint(value.toStdString());
}

QString qUtils::GenerateMnemonic() {
    return QString::fromStdString(nunchuk::Utils::GenerateMnemonic());
}

bool qUtils::CheckMnemonic(const QString &mnemonic) {
    return nunchuk::Utils::CheckMnemonic(mnemonic.toStdString());
}

QStringList qUtils::GetBIP39WordList() {
    QStringList result;
    result.clear();
    std::vector<std::string> ret = nunchuk::Utils::GetBIP39WordList();
    for (std::string word : ret) {
        result << QString::fromStdString(word);
    }
    return result;
}

void qUtils::SetPassPhrase(const QString &storage_path, const QString &account, nunchuk::Chain chain, const QString &old_passphrase,
                           const QString &new_passphrase) {
    nunchuk::Utils::SetPassPhrase(storage_path.toStdString(), account.toStdString(), chain, old_passphrase.toStdString(), new_passphrase.toStdString());
}

nunchuk::Wallet qUtils::ParseWalletDescriptor(const QString &descs, QWarningMessage &msg) {
    nunchuk::Wallet ret(false);
    try {
        ret = nunchuk::Utils::ParseWalletDescriptor(descs.toStdString());
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Wallet qUtils::ParseKeystoneWallet(nunchuk::Chain chain, const QStringList qrtags, QWarningMessage &msg) {
    QStringList in = qrtags;
    in.removeDuplicates();
    nunchuk::Wallet ret(false);
    std::vector<std::string> qr_result;
    for (QString tag : in) {
        qr_result.push_back(tag.toStdString());
    }
    try {
        ret = nunchuk::Utils::ParseKeystoneWallet(chain, qr_result);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::ParseQRTransaction(const QStringList &qrtags, QWarningMessage &msg) {
    QStringList in = qrtags;
    in.removeDuplicates();
    std::string psbt = "";
    std::vector<std::string> qr_result;
    for (QString tag : in) {
        qr_result.push_back(tag.toStdString());
    }
    try {
        psbt = nunchuk::Utils::ParseKeystoneTransaction(qr_result);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return QString::fromStdString(psbt);
}

std::vector<nunchuk::PrimaryKey> qUtils::GetPrimaryKeys(const QString &storage_path, nunchuk::Chain chain) {
    return nunchuk::Utils::GetPrimaryKeys(storage_path.toStdString(), chain);
}

QString qUtils::GetMasterFingerprint(const QString &mnemonic, const QString &passphrase) {
    return QString::fromStdString(nunchuk::Utils::GetMasterFingerprint(mnemonic.toStdString(), passphrase.toStdString()));
}

QString qUtils::GetPrimaryKeyAddress(const QString &mnemonic, const QString &passphrase) {
    return QString::fromStdString(nunchuk::Utils::GetPrimaryKeyAddress(mnemonic.toStdString(), passphrase.toStdString()));
}

QString qUtils::SignLoginMessage(const QString &mnemonic, const QString &passphrase, const QString &message) {
    return QString::fromStdString(nunchuk::Utils::SignLoginMessage(mnemonic.toStdString(), passphrase.toStdString(), message.toStdString()));
}

void qUtils::SetChain(nunchuk::Chain chain) {
    nunchuk::Utils::SetChain(chain);
}

nunchuk::SignerType qUtils::GetSignerType(const QString &value) {
    nunchuk::SignerType ret = nunchuk::SignerType::UNKNOWN;
    try {
        ret = SignerTypeFromStr(value.toStdString());
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::GetSignerTypeString(const nunchuk::SignerType value) {
    QString ret = "UNKNOWN";
    try {
        ret = QString::fromStdString(SignerTypeToStr(value));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::GetSHA256(const QString &value) {
    QString ret = value;
    try {
        ret = QString::fromStdString(nunchuk::Utils::SHA256(value.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        QByteArray bytes = QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Sha256);
        ret = QString(bytes.toHex());
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        QByteArray bytes = QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Sha256);
        ret = QString(bytes.toHex());
    }
    return ret;
}

QString qUtils::CreateRequestToken(const QString &signature, const QString &fingerprint) {
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::CreateRequestToken(signature.toStdString(), fingerprint.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::GetPartialSignature(const nunchuk::SingleSigner &signer, const QString &signed_psbt) {
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::GetPartialSignature(signer, signed_psbt.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::GetHealthCheckMessage(const QString &body, QWarningMessage &msg) {
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::GetHealthCheckMessage(body.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::GetHealthCheckDummyTx(const nunchuk::Wallet &wallet, const QString &body, QWarningMessage &msg) {
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::GetHealthCheckDummyTx(wallet, body.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction qUtils::DecodeDummyTx(const nunchuk::Wallet &wallet, const QString &psbt, QWarningMessage &msg) {
    nunchuk::Transaction ret;
    try {
        ret = nunchuk::Utils::DecodeDummyTx(wallet, psbt.toStdString());
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction qUtils::DecodeTx(const nunchuk::Wallet &wallet, const QString &psbt, const nunchuk::Amount &sub_amount, const nunchuk::Amount &fee,
                                      const nunchuk::Amount &fee_rate, QWarningMessage &msg) {
    nunchuk::Transaction ret;
    try {
        ret = nunchuk::Utils::DecodeTx(wallet, psbt.toStdString(), sub_amount, fee, fee_rate);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::CreateRequestToken(const QString &signature, const QString &fingerprint, QWarningMessage &msg) {
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::CreateRequestToken(signature.toStdString(), fingerprint.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::currencyLocale(qint64 amountSats) {
    double btcRates = AppModel::instance()->btcRates() / 100000000;
    double exRates = AppModel::instance()->exchangeRates();
    double balanceCurrency = btcRates * exRates * amountSats;
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

qint64 qUtils::QAmountFromCurrency(const QString &currency) {
    double btcRates = AppModel::instance()->btcRates() / 100000000;
    double exRates = AppModel::instance()->exchangeRates();
    double rate = btcRates * exRates;
    double amountSats = currency.toDouble() / rate;
    return static_cast<qint64>(amountSats);
}

bool qUtils::verifyCheckSum(const QByteArray &data, const QByteArray &expectedCheckSum) {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(data);
    QByteArray actualCheckSum = hash.result();
    return (actualCheckSum == expectedCheckSum);
}

int qUtils::Precision(double input) {
    const int maxDigits = 15;
    double remaining = input;
    int lastDigit = 0;
    for (int i = 1; i <= maxDigits; i++) {
        remaining = (remaining * 10);
        int round = qRound(remaining) % 10;
        if (round != 0) {
            if (round < 0)
                break;
            lastDigit = i;
        }
    }
    return qMax(lastDigit, 2);
}

QJsonObject qUtils::GetJsonObject(QString text) {
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());

    // check validity of the document
    if (!doc.isNull()) {
        if (doc.isObject()) {
            obj = doc.object();
        } else {
            qDebug() << "Document is not an object";
        }
    } else {
        qDebug() << "Invalid JSON...\n";
    }
    return obj;
}

uint qUtils::GetTimeSecond(QString time_str) {
    QStringList list = time_str.split("/");
    if (list.size() < 2)
        return 0;
    QString month = list.at(0);
    QString day = list.at(1);
    QString year = list.at(2);
    QDate date(year.toInt(), month.toInt(), day.toInt());
    QDateTime time(date);
    return time.toTime_t();
}

uint qUtils::GetCurrentTimeSecond() {
    return QDateTime::currentDateTime().toTime_t();
}

QString qUtils::GetTimeString(uint time_second, bool is_relative)
{
    if (is_relative) {
        if (time_second < 60)
            return QStringLiteral("%1 seconds").arg(time_second);
        else if (time_second < 3600)
            return QStringLiteral("%1 minutes").arg(time_second / 60);
        else if (time_second < 86400)
            return QStringLiteral("%1 hours").arg(time_second / 3600);
        else if (time_second < 2592000) // < 30 days
            return QStringLiteral("%1 days").arg(time_second / 86400);
        else if (time_second < 31536000) // < 12 months
            return QStringLiteral("%1 months").arg(time_second / 2592000);
        else
            return QStringLiteral("%1 years").arg(time_second / 31536000);
    }
    else {
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(time_second, Qt::UTC);
        return dateTime.toString("MM/dd/yyyy");
    }
}

bool qUtils::strCompare(const QString &str1, const QString &str2) {
    return (0 == QString::compare(str1, str2, Qt::CaseInsensitive)) && !str1.isEmpty();
}

nunchuk::AnalyzeQRResult qUtils::AnalyzeQR(const QStringList &qrtags) {
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
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

QStringList qUtils::ExportQRTransaction(const QString &tx_to_sign, QWarningMessage &msg) {
    QStringList result{};
    try {
        std::vector<std::string> data = nunchuk::Utils::ExportKeystoneTransaction(tx_to_sign.toStdString());
        result.reserve(data.size());
        for (std::string tag : data) {
            result.append(QString::fromStdString(tag));
        }
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

QStringList qUtils::DeriveAddresses(const nunchuk::Wallet &wallet, int from_index, int to_index, QWarningMessage &msg) {
    QStringList result{};
    try {
        std::vector<std::string> data = nunchuk::Utils::DeriveAddresses(wallet, from_index, to_index);
        result.reserve(data.size());
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

int qUtils::GetIndexFromPath(const QString &path) {
    QWarningMessage msg;
    int index = -1;
    try {
        index = nunchuk::Utils::GetIndexFromPath(path.toStdString());
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return index;
}

bool qUtils::IsValidAddress(const QString &address) {
    QWarningMessage msg;
    bool ret{false};
    try {
        ret = nunchuk::Utils::IsValidAddress(address.toStdString());
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::GetBip32DerivationPath(const nunchuk::WalletType &wallet_type, const nunchuk::AddressType &address_type) {
    QWarningMessage msg;
    std::string path = "";
    try {
        nunchuk::Chain chain = static_cast<nunchuk::Chain>(AppSetting::instance()->primaryServer());
        path = GetBip32Path(chain, wallet_type, address_type, 0);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return QString::fromStdString(path);
}

QString qUtils::ExportBitcoinSignedMessage(const QString &message, const QString &address, const QString &signature) {
    nunchuk::BitcoinSignedMessage btc(message.toStdString(), address.toStdString(), signature.toStdString());
    return QString::fromStdString(nunchuk::ExportBitcoinSignedMessage(btc));
}

QString qUtils::qAddressTypeToStr(const nunchuk::AddressType &address_type) {
    return QString::fromStdString(AddressTypeToStr(address_type));
}

bool qUtils::isPrimaryKey(const QString &xfp) {
    nunchuk::PrimaryKey key = AppModel::instance()->findPrimaryKey(xfp);
    if (key.get_master_fingerprint() != "") {
        return true;
    }
    return false;
}

nunchuk::Wallet qUtils::parseBBQRWallet(nunchuk::Chain chain, const QStringList &qrtags, QWarningMessage &msg) {
    QStringList in = qrtags;
    in.removeDuplicates();
    nunchuk::Wallet ret(false);
    std::vector<std::string> qr_result;
    for (QString tag : in) {
        qr_result.push_back(tag.toStdString());
    }
    try {
        // ret = nunchuk::Utils::ParseBBQRWallet(chain, qr_result);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::Wallet> qUtils::ParseBBQRWallets(const QStringList &qrtags, QWarningMessage &msg) {
    QStringList in = qrtags;
    in.removeDuplicates();
    std::vector<nunchuk::Wallet> ret;
    std::vector<std::string> qr_result;
    for (QString tag : in) {
        qr_result.push_back(tag.toStdString());
    }
    try {
        ret = nunchuk::Utils::ParseBBQRWallets(qr_result);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QStringList qUtils::ExportBBQRTransaction(const QString &psbt, QWarningMessage &msg) {
    QStringList result{};
    try {
        std::vector<std::string> data = nunchuk::Utils::ExportBBQRTransaction(psbt.toStdString());
        result.reserve(data.size());
        for (std::string tag : data) {
            result.append(QString::fromStdString(tag));
        }
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

QStringList qUtils::ExportBBQRWallet(const nunchuk::Wallet &wallet, QWarningMessage &msg) {
    QStringList result{};
    try {
        std::vector<std::string> data;
        nunchuk::WalletType type = wallet.get_wallet_type();
        // if(nunchuk::WalletType::MINISCRIPT == type){
        //     data = nunchuk::Utils::ExportBBQRWallet(wallet, nunchuk::ExportFormat::DESCRIPTOR_EXTERNAL_INTERNAL);
        // }
        // else {
        //     data = nunchuk::Utils::ExportBBQRWallet(wallet, nunchuk::ExportFormat::DESCRIPTOR_EXTERNAL_ALL);
        // }
        // 6.Dec.25: Always export descriptor external/internal for Coldcard as per new spec
        data = nunchuk::Utils::ExportBBQRWallet(wallet, nunchuk::ExportFormat::DESCRIPTOR_EXTERNAL_INTERNAL);
        result.reserve(data.size());
        for (std::string tag : data) {
            result.append(QString::fromStdString(tag));
        }
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return result;
}

nunchuk::SingleSigner qUtils::ParseSignerString(const QString key_spec, QWarningMessage &msg) {
    nunchuk::SingleSigner ret{};
    static std::regex SIGNER_REGEX("\\[([0-9a-fA-F]{8})(.+)\\](.+?)(/.*\\*)?\n?");
    std::string key_spec_str = key_spec.toStdString();
    std::smatch sm;
    if (std::regex_match(key_spec_str, sm, SIGNER_REGEX)) {
        const std::string xfp = boost::algorithm::to_lower_copy(sm[1].str());
        if (sm[3].str().rfind("tpub", 0) == 0 || sm[3].str().rfind("xpub", 0) == 0) {
            try {
                ret = nunchuk::SingleSigner(sm[1], sm[3], {}, "m" + sm[2].str(), {}, xfp, 0);
            } catch (const nunchuk::BaseException &ex) {
                DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
                msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
            } catch (std::exception &e) {
                DBG_INFO << "THROW EXCEPTION" << e.what();
                msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
            }
        } else {
            try {
                ret = nunchuk::SingleSigner(sm[1], {}, sm[3], "m" + sm[2].str(), {}, xfp, 0);
            } catch (const nunchuk::BaseException &ex) {
                DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
                msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
            } catch (std::exception &e) {
                DBG_INFO << "THROW EXCEPTION" << e.what();
                msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
            }
        }
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            ret = qUtils::SanitizeSingleSigner(ret, msg);
        }
    } else {
        msg.setWarningMessage(nunchuk::NunchukException::INVALID_PARAMETER, "Could not parse descriptor. Please check your input and try again.",
                              EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::SingleSigner qUtils::SanitizeSingleSigner(const nunchuk::SingleSigner &signer, QWarningMessage &msg) {
    nunchuk::SingleSigner ret{};
    try {
        ret = nunchuk::Utils::SanitizeSingleSigner(signer);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::Device> qUtils::GetDevices(const QString &hwi_path, QWarningMessage &msg) {
    std::vector<nunchuk::Device> ret{};
    try {
        ret = nunchuk::Utils::GetDevices(hwi_path.toStdString());
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::SignPsbt(const QString &hwi_path, const nunchuk::Device &device, const QString &psbt, QWarningMessage &msg) {
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::SignPsbt(hwi_path.toStdString(), device, psbt.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::ImportDataViaFile(const QString &filepath) {
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

void qUtils::ExportDataViaFile(const QString &filepath, const QString &data) {
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream st(&file);
        st.setCodec("UTF-8");
        st << data << endl;
        st.flush();
        file.close();
    }
}

bool qUtils::isValidXPRV(const QString &xprv, QWarningMessage &msg) {
    bool ret = false;
    try {
        ret = nunchuk::Utils::IsValidXPrv(xprv.toStdString());
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString qUtils::GetPrimaryKeyAddressFromMasterXprv(const QString &master_xprv) {
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::GetPrimaryKeyAddressFromMasterXprv(master_xprv.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::GetMasterFingerprintFromMasterXprv(const QString &master_xprv) {
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::GetMasterFingerprintFromMasterXprv(master_xprv.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QString qUtils::SignLoginMessageWithMasterXprv(const QString &master_xprv, const QString &message) {
    QString ret = "";
    try {
        ret = QString::fromStdString(nunchuk::Utils::SignLoginMessageWithMasterXprv(master_xprv.toStdString(), message.toStdString()));
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

QByteArray qUtils::aesEncrypt(const QByteArray &plaintext, const QByteArray &key, const QByteArray &iv) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    QByteArray ciphertext;
    int len;
    int ciphertext_len;

    if (!ctx)
        return QByteArray();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char *>(key.constData()),
                                reinterpret_cast<const unsigned char *>(iv.constData())))
        return QByteArray();

    ciphertext.resize(plaintext.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

    if (1 != EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char *>(ciphertext.data()), &len, reinterpret_cast<const unsigned char *>(plaintext.constData()),
                               plaintext.size()))
        return QByteArray();

    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(ciphertext.data()) + len, &len))
        return QByteArray();

    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
}

QByteArray qUtils::aesDecrypt(const QByteArray &ciphertext, const QByteArray &key, const QByteArray &iv) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    QByteArray plaintext;
    int len;
    int plaintext_len;

    if (!ctx)
        return QByteArray();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char *>(key.constData()),
                                reinterpret_cast<const unsigned char *>(iv.constData())))
        return QByteArray();

    plaintext.resize(ciphertext.size());

    if (1 != EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char *>(plaintext.data()), &len, reinterpret_cast<const unsigned char *>(ciphertext.constData()),
                               ciphertext.size()))
        return QByteArray();

    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(plaintext.data()) + len, &len))
        return QByteArray();

    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}

bool qUtils::CheckElectrumServer(const std::string &server, int timeout) {
    try {
        return nunchuk::Utils::CheckElectrumServer(server, timeout);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return false;
}

bool qUtils::IsValidPolicy(const std::string &policy) {
    try {
        return nunchuk::Utils::IsValidPolicy(policy);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return false;
}

std::string qUtils::PolicyToMiniscript(const std::string &policy, const std::map<std::string, nunchuk::SingleSigner> &signers,
                                       nunchuk::AddressType address_type) {
    try {
        return nunchuk::Utils::PolicyToMiniscript(policy, signers, address_type);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return "";
}

bool qUtils::IsValidMiniscriptTemplate(const std::string &miniscript_template, nunchuk::AddressType address_type) {
    try {
        return nunchuk::Utils::IsValidMiniscriptTemplate(miniscript_template, address_type);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return false;
}

bool qUtils::IsValidTapscriptTemplate(const std::string &tapscript_template, std::string &error) {
    try {
        return nunchuk::Utils::IsValidTapscriptTemplate(tapscript_template, error);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return false;
}

std::string qUtils::MiniscriptTemplateToMiniscript(const std::string &miniscript_template, const std::map<std::string, nunchuk::SingleSigner> &signers) {
    try {
        return nunchuk::Utils::MiniscriptTemplateToMiniscript(miniscript_template, signers);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return "";
}

std::string qUtils::TapscriptTemplateToTapscript(const std::string &tapscript_template, const std::map<std::string, nunchuk::SingleSigner> &signers,
                                                std::vector<std::string> &keypaths) {
    try {
        return nunchuk::Utils::TapscriptTemplateToTapscript(tapscript_template, signers, keypaths);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return "";
}

nunchuk::ScriptNode qUtils::GetScriptNode(const std::string &script, std::vector<std::string> &keypaths) {
    try {
        return nunchuk::Utils::GetScriptNode(script, keypaths);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return nunchuk::ScriptNode();
}

std::vector<uint8_t> qUtils::HashPreimage(const std::vector<uint8_t> &preimage, nunchuk::PreimageHashType hashType) {
    try {
        return nunchuk::Utils::HashPreimage(preimage, hashType);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return {};
}

std::string qUtils::RevealPreimage(const std::string &psbt, nunchuk::PreimageHashType hashType, const std::vector<uint8_t> &hash,
                                   const std::vector<uint8_t> &preimage, QWarningMessage &msg) {
    try {
        return nunchuk::Utils::RevealPreimage(psbt, hashType, hash, preimage);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return "";
}

bool qUtils::IsPreimageRevealed(const std::string &psbt, const std::vector<uint8_t> &hash) {
    try {
        return nunchuk::Utils::IsPreimageRevealed(psbt, hash);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return false;
}

std::string qUtils::ExpandingMultisigMiniscriptTemplate(int m, int n, int new_n, bool reuse_signers, const nunchuk::Timelock &timelock, nunchuk::AddressType address_type, QWarningMessage &msg) {
    try {
        return nunchuk::Utils::ExpandingMultisigMiniscriptTemplate(m, n, new_n, reuse_signers, timelock, address_type);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &ex) {
        DBG_INFO << "THROW EXCEPTION" << ex.what();
        msg.setWarningMessage(-1, ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return "";
}

std::string qUtils::DecayingMultisigMiniscriptTemplate(int m, int n, int new_m, bool reuse_signers, const nunchuk::Timelock &timelock, nunchuk::AddressType address_type, QWarningMessage &msg) {
    try {
        return nunchuk::Utils::DecayingMultisigMiniscriptTemplate(m, n, new_m, reuse_signers, timelock, address_type);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &ex) {
        DBG_INFO << "THROW EXCEPTION" << ex.what();
        msg.setWarningMessage(-1, ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return "";
}

std::string qUtils::FlexibleMultisigMiniscriptTemplate(int m, int n, int new_m, int new_n, bool reuse_signers, const nunchuk::Timelock &timelock,
                                                       nunchuk::AddressType address_type, QWarningMessage &msg) {
    try {
        return nunchuk::Utils::FlexibleMultisigMiniscriptTemplate(m, n, new_m, new_n, reuse_signers, timelock, address_type);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &ex) {
        DBG_INFO << "THROW EXCEPTION" << ex.what();
        msg.setWarningMessage(-1, ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return "";
}

std::string qUtils::ZenHodlMiniscriptTemplate(int m, int n, const nunchuk::Timelock& timelock, nunchuk::AddressType address_type, QWarningMessage &msg) {
    try {
        return nunchuk::Utils::ZenHodlMiniscriptTemplate(m, n, timelock, address_type);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    } catch (std::exception &ex) {
        DBG_INFO << "THROW EXCEPTION" << ex.what();
        msg.setWarningMessage(-1, ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return "";
}

std::vector<nunchuk::UnspentOutput> qUtils::GetTimelockedCoins(const std::string &miniscript, const std::vector<nunchuk::UnspentOutput> &coins,
                                                               int64_t &max_lock_value, int chain_tip) {
    try {
        return nunchuk::Utils::GetTimelockedCoins(miniscript, coins, max_lock_value, chain_tip);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return {};
}

std::vector<nunchuk::CoinsGroup> qUtils::GetCoinsGroupedBySubPolicies(const nunchuk::ScriptNode &script_node, const std::vector<nunchuk::UnspentOutput> &coins,
                                                                      int chain_tip) {
    try {
        return nunchuk::Utils::GetCoinsGroupedBySubPolicies(script_node, coins, chain_tip);
    } catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    } catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return {};
}

nunchuk::AddressType qUtils::AddressTypeFromStr(const QString &str) {
    if( str == "ANY" ) return nunchuk::AddressType::ANY;
    if( str == "LEGACY" ) return nunchuk::AddressType::LEGACY;
    if( str == "NESTED_SEGWIT" ) return nunchuk::AddressType::NESTED_SEGWIT;
    if( str == "NATIVE_SEGWIT" ) return nunchuk::AddressType::NATIVE_SEGWIT;
    if( str == "TAPROOT" ) return nunchuk::AddressType::TAPROOT;
    return nunchuk::AddressType::ANY; // Default case, should be handled properly
}

nunchuk::WalletTemplate qUtils::WalletTemplateFromStr(const QString &str) {
    if (str == "DEFAULT") return nunchuk::WalletTemplate::DEFAULT;
    if (str == "DISABLE_KEY_PATH") return nunchuk::WalletTemplate::DISABLE_KEY_PATH;
    return nunchuk::WalletTemplate::DEFAULT;
}

nunchuk::WalletType qUtils::WalletTypeFromStr(const QString &str) {
    if (str == "MULTI_SIG") return nunchuk::WalletType::MULTI_SIG;
    if (str == "SINGLE_SIG") return nunchuk::WalletType::SINGLE_SIG;
    if (str == "ESCROW") return nunchuk::WalletType::ESCROW;
    if (str == "MINISCRIPT") return nunchuk::WalletType::MINISCRIPT;
    return nunchuk::WalletType::MULTI_SIG; // Default case, should be handled properly
}

nunchuk::ScriptNodeId qUtils::ScriptNodeIdFromString(const QString &path) {
    nunchuk::ScriptNodeId id;
    QStringList list = path.split(".");
    for (const QString &item : list) {
        if (item.isEmpty()) continue; // Skip empty items
        try {
            int index = item.toInt();
            id.push_back(index);
        } catch (const std::exception &e) {
            DBG_INFO << "Invalid ScriptNodeId item: " << item << ", error: " << e.what();
            return nunchuk::ScriptNodeId(); // Return empty id on error
        }
    }
    return id;
}

QString qUtils::ScriptNodeIdToString(const nunchuk::ScriptNodeId &id) {
    QString result;
    QStringList list;
    for (const auto &item : id) {
        list.append(QString::number(item));
    }
    result = list.join(".") + ".";
    return result;
}

nunchuk::SigningPath qUtils::SigningPathFromStringList(const QStringList &path) {
    nunchuk::SigningPath result;
    for(const QString &item : path) {
        if (item.isEmpty()) continue; // Skip empty items
        try {
            auto id = ScriptNodeIdFromString(item);
            result.push_back(id);
        } catch (const std::exception &e) {
            DBG_INFO << "Invalid SigningPath item: " << item << ", error: " << e.what();
            return nunchuk::SigningPath(); // Return empty path on error
        }
    }
    return result;
}
QStringList qUtils::SigningPathToStringList(const nunchuk::SigningPath &path) {
    QStringList result;
    for (const auto &id : path) {
        result.append(ScriptNodeIdToString(id));
    }
    return result;
}

const nunchuk::ScriptNode* qUtils::FindScriptNodeById(const nunchuk::ScriptNode& root, const nunchuk::ScriptNodeId& id) {
    // Check if the current node matches the ID
    if (root.get_id() == id) {
        return &root;
      }
    
      // Recursively search in the sub-nodes
      for (const auto& sub_node : root.get_subs()) {
        const nunchuk::ScriptNode* result = FindScriptNodeById(sub_node, id);
        if (result != nullptr) {
          return result;
        }
      }
    
      // Return nullptr if no match is found
      return nullptr;
}

QJsonObject qUtils::JsonDeepMerge(const QJsonObject& a, const QJsonObject& b) {
    QJsonObject result = a;
    for (auto it = b.begin(); it != b.end(); ++it) {
        if (result.contains(it.key()) &&
            result[it.key()].isObject() &&
            it.value().isObject()) {
            result[it.key()] = JsonDeepMerge(result[it.key()].toObject(), it.value().toObject());
        } else {
            result[it.key()] = it.value();  // Overwrite
        }
    }
    return result;
}

QVariant qUtils::toVariant(const QList<int> &list) {
    QVariantList varList;
    varList.reserve(list.size());
    for (int value : list) {
        varList.append(value);
    }
    return varList;
}

QString qUtils::getTimeFromTimestamp(qint64 timestamp, const QTimeZone &tz) {
    QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp, tz);
    return dt.time().toString("HH:mm");
}

QString qUtils::getDateFromTimestamp(qint64 timestamp, const QTimeZone &tz) {
    QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp, tz);
    return dt.date().toString("MM/dd/yyyy");
}

qint64 qUtils::getDaysFromTimestamp(qint64 timestamp, const QTimeZone &tz) {
    QDateTime dt  = QDateTime::fromSecsSinceEpoch(timestamp, tz);
    QDateTime now = QDateTime::currentDateTime();
    return now.date().daysTo(dt.date());
}

QString qUtils::localTimeZoneString() {
    QTimeZone tz = QTimeZone::systemTimeZone();

    QString name = QString::fromUtf8(tz.id()); // Asia/Jakarta
    int offsetSec = tz.offsetFromUtc(QDateTime::currentDateTime());
    int hours = offsetSec / 3600;
    int minutes = (abs(offsetSec) % 3600) / 60;
    QString sign = hours >= 0 ? "+" : "-";

    return QString("%1 (GMT%2%3:%4)")
        .arg(name)
        .arg(sign)
        .arg(QString::number(abs(hours)).rightJustified(2,'0'))
        .arg(QString::number(minutes).rightJustified(2,'0'));
}

QByteArray qUtils::serializeSigningPath(const nunchuk::SigningPath path) {
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << static_cast<quint64>(path.size());
    for (const auto &node : path) {
        out << static_cast<quint64>(node.size());
        for (size_t x : node) {
            out << static_cast<quint64>(x);
        }
    }
    return ba;
}

nunchuk::SigningPath qUtils::deserializeSigningPath(const QByteArray ba) {
    QDataStream in(ba);
    nunchuk::SigningPath path;

    quint64 outerSize;
    in >> outerSize;
    path.reserve(outerSize);

    for (quint64 i = 0; i < outerSize; i++) {
        quint64 innerSize;
        in >> innerSize;
        std::vector<size_t> node;
        node.reserve(innerSize);

        for (quint64 j = 0; j < innerSize; j++) {
            quint64 val;
            in >> val;
            node.push_back(static_cast<size_t>(val));
        }
        path.push_back(std::move(node));
    }
    return path;
}

QString qUtils::incrementZeroIndex(const QString &path) {
    if (!path.startsWith("m"))
        return path;

    QStringList parts = path.split('/');
    if (parts.size() < 3) // need at least "m/X/Y"
        return path;

    int pos = parts.size() - 2; // second last
    QString part = parts.at(pos);

    bool hardened = part.endsWith("h") || part.endsWith("'");
    QString numberStr = part;
    if (hardened)
        numberStr.chop(1);

    bool ok = false;
    int value = numberStr.toInt(&ok);
    if (ok) {
        value++;
        parts[pos] = QString::number(value) + (hardened ? "h" : "");
    }

    return parts.join('/');
}

int qUtils::getIndexAt(const QString &path, int pos)
{
    if (!path.startsWith("m"))
        return -1;

    QStringList parts = path.split('/');
    if (parts.isEmpty())
        return -1;

    // remove "m"
    parts.removeFirst();

    if (pos < 0 || pos >= parts.size())
        return -1;

    QString part = parts.at(pos);

    // handle hardened suffix h or '
    if (part.endsWith('h') || part.endsWith('\'')) {
        part.chop(1);
    }

    bool ok = false;
    int value = part.toInt(&ok);
    return ok ? value : -1;
}

nunchuk::SingleSigner qUtils::toSingleSigner(const nunchuk::MasterSigner& master) {
    std::string name = master.get_name();

    nunchuk::Device dev = master.get_device();

    std::string xpub;              
    std::string public_key;        
    std::string derivation_path;   

    std::string master_fingerprint = dev.get_master_fingerprint();

    std::string master_signer_id = master.get_id();

    time_t last_health_check = master.get_last_health_check();

    nunchuk::SignerType signer_type = master.get_type();

    // Tags
    std::vector<nunchuk::SignerTag> tags = master.get_tags();

    // Visibility
    bool visible = master.is_visible();

    // Construct SingleSigner object
    return nunchuk::SingleSigner(
        name,
        xpub,
        public_key,
        derivation_path,
        {},
        master_fingerprint,
        last_health_check,
        master_signer_id,
        /* used = */ false,
        signer_type,
        tags,
        visible
    );
}

// Tìm đóng tương ứng cho '(' hoặc '{' tại pos (trả về index đóng hoặc -1)
static int findMatchingBracket(const QString &s, int pos) {
    if (pos < 0 || pos >= s.size()) return -1;
    QChar open = s[pos];
    QChar close = (open == '(') ? ')' : (open == '{' ? '}' : QChar());
    if (close.isNull()) return -1;
    int depth = 1;
    for (int i = pos + 1; i < s.size(); ++i) {
        if (s[i] == open) ++depth;
        else if (s[i] == close) {
            --depth;
            if (depth == 0) return i;
        }
    }
    return -1;
}

// Quyết định có inline cặp (open..close) hay không
static bool canInlineRange(const QString &s, int open, int close, int maxInlineLen) {
    if (open < 0 || close <= open) return true;
    int len = close - open - 1;
    if (len <= 0) return true;
    if (len > maxInlineLen) return false;
    // nếu nội dung có newline thì không inline
    QString inner = s.mid(open + 1, len);
    if (inner.contains('\n')) return false;
    // Đơn giản: nếu ngắn (< threshold) thì inline tốt
    return true;
}

QString qUtils::formatMiniscript(const QString &input, int indentSize, int maxInlineLen) {
    QString output;
    QTextStream out(&output);

    QVector<bool> inlineStack; // stack lưu trạng thái inline cho mỗi cặp ngoặc đang mở
    int indent = 0;
    int n = input.size();

    for (int i = 0; i < n; ++i) {
        QChar c = input[i];

        if (c == '(' || c == '{') {
            int match = findMatchingBracket(input, i);
            // nếu không tìm thấy đóng, cứ in ra ký tự và tiếp tục
            if (match == -1) {
                out << c;
                continue;
            }

            // Nếu ngoặc cha đang inline thì ép con inline
            bool forcedInline = (!inlineStack.isEmpty() && inlineStack.last());
            bool inlineMode = forcedInline ? true : canInlineRange(input, i, match, maxInlineLen);
            inlineStack.append(inlineMode);

            out << c;
            if (!inlineMode) {
                out << "\n";
                indent += indentSize;
                out << QString(indent, ' ');
            }
        }
        else if (c == ',') {
            // chuẩn hoá: nếu trong inline => ", " ; nếu trong multiline => ",\n<indent>"
            bool topInline = (!inlineStack.isEmpty() && inlineStack.last());
            if (topInline) {
                out << ", ";
                // bỏ các khoảng trắng sau dấu phẩy trong input (chuẩn hoá spacing)
                int j = i + 1;
                while (j < n && input[j].isSpace()) ++j;
                i = j - 1;
            } else {
                out << ",\n" << QString(indent, ' ');
                int j = i + 1;
                while (j < n && input[j].isSpace()) ++j;
                i = j - 1;
            }
        }
        else if (c == ')' || c == '}') {
            bool isInline = (!inlineStack.isEmpty() && inlineStack.last());
            if (!inlineStack.isEmpty()) inlineStack.removeLast();

            if (isInline) {
                out << c;
            } else {
                out << "\n";
                indent -= indentSize;
                if (indent < 0) indent = 0;
                out << QString(indent, ' ') << c;
            }
        }
        else {
            out << c;
        }
    }

    return output;
}

// Convert vector<uint8_t> -> hex string
std::string qUtils::bytesToHex(const std::vector<uint8_t>& data) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (uint8_t byte : data) {
        oss << std::setw(2) << static_cast<int>(byte);
    }
    return oss.str();
}

// Convert hex string -> vector<uint8_t>
std::vector<uint8_t> qUtils::hexToBytes(const std::string& hex) {
    if (hex.size() % 2 != 0) {
        return {};
    }

    std::vector<uint8_t> bytes;
    bytes.reserve(hex.size() / 2);

    for (size_t i = 0; i < hex.size(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

QString qUtils::BytesToHex(const std::vector<uint8_t>& data) {
    return QString::fromStdString(bytesToHex(data));
}
std::vector<uint8_t> qUtils::HexToBytes(const QString& hex) {
    return hexToBytes(hex.toStdString());
}

QDateTime qUtils::convertTimestampToDateTime(qint64 timestamp, const QString& timezoneId)
{
    QTimeZone targetTimeZone(timezoneId.toUtf8());
    if (!targetTimeZone.isValid()) {
        return QDateTime::fromSecsSinceEpoch(timestamp, Qt::UTC);
    }

    QDateTime dateTimeUtc = QDateTime::fromSecsSinceEpoch(timestamp, Qt::UTC);
    QDateTime finalDateTime = dateTimeUtc.toTimeZone(targetTimeZone);
    return finalDateTime;
}

QString qUtils::formatTimeZoneString(const QString& timezoneIdInput)
{
    QString tzInput = timezoneIdInput.trimmed();
    QTimeZone targetZone(tzInput.toUtf8());

    // If not a valid IANA ID, try parse "GMT+7", "GMT+07:00", "UTC-5", ...
    bool parsedFromGmt = false;
    QString normalizedId = tzInput; // used for display when input was GMT/UTC style

    if (!targetZone.isValid()) {
        // regex: optional whitespace, (GMT|UTC) case-insensitive, optional space,
        // optional sign, 1-2 digit hours, optional :mm or mm
        static const QRegularExpression re(R"(^\s*(?:GMT|UTC)\s*([+-])?(\d{1,2})(?::?(\d{2}))?\s*$)",
                                           QRegularExpression::CaseInsensitiveOption);
        const QRegularExpressionMatch m = re.match(tzInput);
        if (m.hasMatch()) {
            parsedFromGmt = true;
            QString signStr = m.captured(1);
            // If no sign provided (rare), treat as plus
            int sign = (signStr == "-") ? -1 : 1;
            int hours = m.captured(2).toInt();
            int minutes = m.captured(3).isEmpty() ? 0 : m.captured(3).toInt();

            int offsetSec = sign * (hours * 3600 + minutes * 60);

            // Build a normalized GMT ID for display like "GMT+07:00"
            QString signDisplay = (offsetSec >= 0) ? "+" : "-";
            int absSec = qAbs(offsetSec);
            int h = absSec / 3600;
            int mm = (absSec % 3600) / 60;
            normalizedId = QString("GMT%1%2:%3")
                               .arg(signDisplay)
                               .arg(QString::number(h).rightJustified(2, '0'))
                               .arg(QString::number(mm).rightJustified(2, '0'));

            // Construct a fixed-offset QTimeZone
            targetZone = QTimeZone(offsetSec);
            // Note: QTimeZone(int seconds) creates a fixed-offset timezone.
        }
    }

    // If still invalid (not an IANA ID and not a GMT/UTC offset), fall back to original input
    if (!targetZone.isValid()) {
        return timezoneIdInput;
    }

    // Use current time to compute DST-aware offset
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const int offsetSec = targetZone.offsetFromUtc(now);

    // Special-case UTC display
    if (offsetSec == 0) {
        // Prefer showing canonical UTC-like label
        // If parsedFromGmt, show normalizedId e.g. "GMT+00:00 (UTC)"
        const QString displayId = parsedFromGmt ? normalizedId : timezoneIdInput;
        return QString("Etc/UTC (%1)").arg(displayId);
    }

    const int absSec = qAbs(offsetSec);
    const int hours = absSec / 3600;
    const int minutes = (absSec % 3600) / 60;
    const QString sign = (offsetSec >= 0) ? "+" : "-";

    const QString gmtString = QString("GMT%1%2:%3")
                                  .arg(sign)
                                  .arg(QString::number(hours).rightJustified(2, '0'))
                                  .arg(QString::number(minutes).rightJustified(2, '0'));

    // If we parsed from a GMT input, show the normalizedId (e.g. "GMT+07:00"), else show original ID
    const QString displayId = parsedFromGmt ? normalizedId : timezoneIdInput;

    return QString("%1 (%2)").arg(displayId, gmtString);
}

QString qUtils::extractTimeZoneId(const QString& formattedString)
{
    auto normalizeId = [](const QByteArray& id) -> QString {
        QTimeZone tz(id);
        return tz.isValid() ? QString::fromUtf8(tz.id()) : QString();
    };

    // 1) Trim and split possible "Name (GMT±HH:MM)" or "Name (UTC)" format
    const QString input = formattedString.trimmed();
    int paren = input.indexOf(" (");
    const QString leading = (paren > 0) ? input.left(paren).trimmed() : input;
    const QString inside  = (paren > 0 && input.endsWith(')'))
                                ? input.mid(paren + 2, input.size() - paren - 3).trimmed()
                                : QString();

    // 2) If leading part is already a valid IANA zone, return it normalized
    if (!leading.isEmpty()) {
        QString normalized = normalizeId(leading.toUtf8());
        if (!normalized.isEmpty())
            return normalized;
    }

    // 3) Map common Windows time zone names to IANA (Qt version without ianaIdsForWindowsId)
    static const QHash<QString, QString> windowsToIana {
        { "Pacific Standard Time", "America/Los_Angeles" },
        { "Eastern Standard Time", "America/New_York" },
        { "Central Standard Time", "America/Chicago" },
        { "Mountain Standard Time", "America/Denver" },
        { "China Standard Time", "Asia/Shanghai" },
        { "Tokyo Standard Time", "Asia/Tokyo" },
        { "India Standard Time", "Asia/Kolkata" },
        { "W. Europe Standard Time", "Europe/Berlin" },
        { "Central European Standard Time", "Europe/Berlin" },
        { "Brazil Standard Time", "America/Sao_Paulo" },
        { "Greenwich Standard Time", "Etc/UTC" },
        { "UTC", "Etc/UTC" },
        { "GMT", "Etc/UTC" }
    };
    if (!leading.isEmpty()) {
        QString key = leading;
        if (windowsToIana.contains(key)) {
            QString normalized = normalizeId(windowsToIana.value(key).toUtf8());
            if (!normalized.isEmpty())
                return normalized;
        }
    }

    // 4) Handle UTC/GMT and fixed offsets like "UTC+07:00", "GMT-0530", "+02:00", etc.
    auto parseOffsetToId = [](QString s) -> QString {
        s = s.trimmed();

        // Strip known prefixes
        if (s.startsWith("UTC", Qt::CaseInsensitive)) s = s.mid(3).trimmed();
        else if (s.startsWith("GMT", Qt::CaseInsensitive)) s = s.mid(3).trimmed();

        if (s.compare("", Qt::CaseInsensitive) == 0)
            return "Etc/UTC";

        if (s.isEmpty() || (s[0] != '+' && s[0] != '-'))
            return QString();

        const QChar signChar = s[0];
        s = s.mid(1).trimmed();

        int h = 0, m = 0;
        bool ok = false;
        if (s.contains(':')) {
            const auto parts = s.split(':');
            if (parts.size() != 2) return QString();
            h = parts[0].toInt(&ok); if (!ok) return QString();
            m = parts[1].toInt(&ok); if (!ok) return QString();
        } else {
            if (s.size() <= 2) {
                h = s.toInt(&ok); if (!ok) return QString();
            } else if (s.size() == 4) {
                bool okh=false, okm=false;
                h = s.left(2).toInt(&okh);
                m = s.mid(2,2).toInt(&okm);
                if (!okh || !okm) return QString();
            } else {
                return QString();
            }
        }
        if (h < 0 || h > 14 || m < 0 || m > 59) return QString();

        const int sign = (signChar == '-') ? -1 : 1;
        const int seconds = sign * (h * 3600 + m * 60);

        // Build a fixed-offset QTimeZone and return its canonical ID (e.g. "UTC+07:00")
        QTimeZone fixed(seconds);
        return fixed.isValid() ? QString::fromUtf8(fixed.id()) : QString();
    };

    // Try: inside "(GMT+...)" first, then leading if it's an offset form
    if (!inside.isEmpty()) {
        QString id = parseOffsetToId(inside);
        if (!id.isEmpty()) return id;
    }
    {
        QString id = parseOffsetToId(leading);
        if (!id.isEmpty()) return id;
    }

    // 5) Handle plain "UTC"/"GMT"
    if (leading.compare("UTC", Qt::CaseInsensitive) == 0 ||
        leading.compare("Etc/UTC", Qt::CaseInsensitive) == 0 ||
        leading.compare("GMT", Qt::CaseInsensitive) == 0) {
        return "Etc/UTC";
    }

    // 6) As a last attempt, resolve by current abbreviation (e.g. "PST")
    //    Note: abbreviations are ambiguous; this picks the first match.
    if (!leading.isEmpty()) {
        const auto now = QDateTime::currentDateTime();
        const auto ids = QTimeZone::availableTimeZoneIds();
        for (const auto& id : ids) {
            QTimeZone tz(id);
            if (!tz.isValid()) continue;
            if (tz.abbreviation(now).compare(leading, Qt::CaseInsensitive) == 0)
                return QString::fromUtf8(tz.id());
        }
    }

    // 7) Fallback: return the leading part unchanged
    return leading;
}

// Normalize a path: h → '
static QString normalizePath(const QString &path)
{
    QString p = path.trimmed();
    p.replace("H", "'");
    p.replace("h", "'");
    return p;
}

// ----------------------------
// Get Purpose from path
// (44, 49, 84, ...)
// ----------------------------
int qUtils::GetPurposeFromPath(const QString &path)
{
    QString p = normalizePath(path);
    QStringList parts = p.split('/');

    if (parts.size() < 2) {
        DBG_INFO << "Invalid derivation path:" << path;
        return -1;
    }

    QString purposeStr = parts[1];
    purposeStr.remove("'");

    bool ok = false;
    int purpose = purposeStr.toInt(&ok);
    return ok ? purpose : -1;
}

// ----------------------------
// Get CoinType from path
// (0: Bitcoin mainnet,
//  60: Ethereum, ...)
// ----------------------------
int qUtils::GetCoinTypeFromPath(const QString &path)
{
    // QString p = normalizePath(path);
    // QStringList parts = p.split('/');

    // if (parts.size() < 3) {
    //     DBG_INFO << "Invalid derivation path:" << path;
    //     return -1;
    // }

    // QString coinStr = parts[2];
    // coinStr.remove("'");

    // bool ok = false;
    // int coinType = coinStr.toInt(&ok);
    // return ok ? coinType : -1;
    QString p = normalizePath(path).trimmed();

    // Handle leading "m" or "m/"
    if (p == "m") {
        DBG_INFO << "Derivation path contains only 'm' (no components):" << path;
        return -1;
    }
    if (p.startsWith("m/")) {
        p = p.mid(2);
    }

    // Split path components
    QStringList parts = p.split('/', Qt::SkipEmptyParts);

    // Expected minimum structure: purpose/coin_type
    if (parts.size() < 2) {
        DBG_INFO << "Derivation path does not contain a coin_type component:" << path;
        return -1;
    }

    QString coinStr = parts[1].trimmed();
    if (coinStr.isEmpty()) {
        DBG_INFO << "Empty coin_type component in derivation path:" << path;
        return -1;
    }

    // Remove hardened suffix if present (', h, or H)
    QChar last = coinStr.at(coinStr.size() - 1);
    if (last == '\'' || last == 'h' || last == 'H') {
        coinStr.chop(1);
    }

    bool ok = false;
    int coinType = coinStr.toInt(&ok);
    if (!ok) {
        DBG_INFO << "Unable to parse coin type from derivation path component:"
                   << parts[1] << "full path:" << path;
        return -1;
    }

    return coinType;
}

QJsonObject qUtils::SingleSignertoJsonObject(const nunchuk::SingleSigner &single) {
    QJsonObject obj;

    // Basic fields
    obj["name"] = QString::fromStdString(single.get_name());
    obj["xfp"] = QString::fromStdString(single.get_master_fingerprint());
    obj["derivation_path"] = QString::fromStdString(single.get_derivation_path());
    obj["xpub"] = QString::fromStdString(single.get_xpub());
    obj["pubkey"] = QString::fromStdString(single.get_public_key());

    // Signer type as string
    obj["type"] = qUtils::GetSignerTypeString(single.get_type());

    obj["tapsigner"] = {};

    // Tags as array of strings (use tag.value when present)
    QJsonArray tagsArray;
    for (const auto &tag : single.get_tags()) {
        QString value = QString::fromStdString(SignerTagToStr(tag));
        if (!value.isEmpty()) {
            tagsArray.append(value);
        }
    }
    obj["tags"] = tagsArray;

    // key_index (default 0 if not provided by SingleSigner)
    obj["key_index"] = 0;

    return obj;
}
