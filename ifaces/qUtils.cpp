#include "qUtils.h"
#include "AppSetting.h"
#include "QOutlog.h"

qint64 qUtils::QAmountFromValue(const QString &value, const bool allow_negative) {
    qint64 ret = -1;
#ifdef USING_STUB_API
    ret = CAmountFromValue(value.toStdString(), allow_negative);
#else
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
#endif
    return ret;
}

QString qUtils::QValueFromAmount(const qint64 &amount) {
    QString ret = "";
#ifdef USING_STUB_API
    ret = QString::fromStdString(ValueFromCAmount(amount));
#else
    ret = QString::fromStdString(nunchuk::Utils::ValueFromAmount(amount));
#endif
    if((int)AppSetting::Unit::BTC == AppSetting::instance()->unit() && false == AppSetting::instance()->enableFixedPrecision()){
        ret.remove( QRegExp("0+$") ); // Remove any number of trailing 0's
        ret.remove( QRegExp("\\.$") ); // If the last character is just a '.' then remove it
    }
//    Consider using locale
//    qlonglong btc = ret.toLongLong();
//    if(btc > 999){
//        QLocale locale(QLocale::English);
//        return locale.toString(btc);
//    }
//    else{
        return ret;
//    }
}

QString qUtils::QGenerateRandomMessage(int message_length)
{
#ifdef USING_STUB_API
    auto randchar = []() -> char {
      const char charset[] =
          "0123456789"
          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
          "abcdefghijklmnopqrstuvwxyz";
      const size_t max_index = (sizeof(charset) - 1);
      return charset[rand() % max_index];
    };
    std::string str(message_length, 0);
    std::generate_n(str.begin(), message_length, randchar);
    return QString::fromStdString(str);
#else
    return QString::fromStdString(nunchuk::Utils::GenerateRandomMessage(message_length));
#endif
}

#ifdef USING_STUB_API
CAmount qUtils::CAmountFromValue(const std::string &value, const bool allow_negative) {
    CAmount amount;
    if (!ParseFixedPoint(value, 8, &amount))
        throw "invalid amount";
    if (!allow_negative) {
        if (!MoneyRange(amount))
            throw "amount out of range";
    } else {
        if (abs(amount) > MAX_MONEY)
            throw "amount out of range";
    }
    return amount;
}

std::string qUtils::ValueFromCAmount(const CAmount &amount) {
    bool sign = amount < 0;
    int64_t n_abs = (sign ? -amount : amount);
    int64_t quotient = n_abs / COIN;
    int64_t remainder = n_abs % COIN;
    return boost::str(boost::format{"%s%d.%08d"} % (sign ? "-" : "") % quotient % remainder);
}
#endif

QString qUtils::QAddressToScriptPubKey(const QString &address)
{
#ifdef USING_STUB_API
    auto randchar = []() -> char {
      const char charset[] =
          "0123456789"
          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
          "abcdefghijklmnopqrstuvwxyz";
      const size_t max_index = (sizeof(charset) - 1);
      return charset[rand() % max_index];
    };
    std::string str(50, 0);
    std::generate_n(str.begin(), 50, randchar);
    return QString("[%1] %2").arg(QString::fromStdString(str)).arg("OKCONDE");
#else
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
#endif
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
#ifdef USING_STUB_API
    valid = true;
#else
    std::string target_format = "";
    if((int)AppSetting::Chain::TESTNET == AppSetting::instance()->primaryServer()){
        target_format = "tpub";
    }
    else{
        target_format = "xpub";
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
#endif
    if(!valid){out = "";}
    return valid;
}

bool qUtils::QIsValidPublicKey(const QString &value)
{
#ifdef USING_STUB_API
    return true;
#else
    return nunchuk::Utils::IsValidPublicKey(value.toStdString());
#endif
}

bool qUtils::QIsValidDerivationPath(const QString &value)
{
#ifdef USING_STUB_API
    return true;
#else
    return nunchuk::Utils::IsValidDerivationPath(value.toStdString());
#endif
}

bool qUtils::QIsValidFingerPrint(const QString &value)
{
#ifdef USING_STUB_API
    return true;
#else
    return nunchuk::Utils::IsValidFingerPrint(value.toStdString());
#endif
}

QString qUtils::GenerateMnemonic()
{
#ifdef USING_STUB_API
    return QString("Please take a moment to write down this mnemonic phrase. It is your signer's backup. You can use it to recover the signer later.");
#else
    return QString::fromStdString(nunchuk::Utils::GenerateMnemonic());
#endif
}

bool qUtils::CheckMnemonic(const QString &mnemonic)
{
#ifdef USING_STUB_API
    return true;
#else
    return nunchuk::Utils::CheckMnemonic(mnemonic.toStdString());
#endif
}

QStringList qUtils::GetBIP39WordList()
{
    QStringList result;
    result.clear();
#ifdef USING_STUB_API
    for (int i = 0; i < 200; ++i) {
        auto randchar = []() -> char {
                const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
                const size_t max_index = (sizeof(charset) - 1);
                return charset[rand() % max_index];
        };

        int random = rand() % 7 + 3;
        std::string str(random, 0);
        std::generate_n(str.begin(), random, randchar);
        result << QString::fromStdString(str);
    }
    return result;
#else
    std::vector<std::string> ret = nunchuk::Utils::GetBIP39WordList();
    for (std::string word : ret) {
        result << QString::fromStdString(word);
    }
#endif
    return result;
}
