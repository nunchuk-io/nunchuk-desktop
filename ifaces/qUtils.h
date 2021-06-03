#ifndef QUTILS_H
#define QUTILS_H

#include <functional>
#include <string>
#include <vector>
#include <QObject>
#ifdef USING_STUB_API //Just for stub test, will be removed
#include <amount.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/strencodings.h>
#include <boost/format.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#else
#include "nunchuk.h"
#endif

class EWARNING: public QObject
{
    Q_OBJECT
    Q_ENUMS(WarningType)
public:
    enum class WarningType {
        NONE_MSG,
        SUCCESS_MSG,
        WARNING_MSG,
        ERROR_MSG,
        EXCEPTION_MSG
    };
};
Q_DECLARE_METATYPE(EWARNING::WarningType)

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

#ifdef USING_STUB_API
CAmount CAmountFromValue(const std::string &value, const bool allow_negative = false);
std::string ValueFromCAmount(const CAmount &amount);
#endif

QString GenerateMnemonic();

bool CheckMnemonic(const QString& mnemonic);

QStringList GetBIP39WordList();

}

#endif // QUTILS_H
