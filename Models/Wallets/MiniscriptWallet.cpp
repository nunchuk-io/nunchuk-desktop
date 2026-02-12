#include "MiniscriptWallet.h"
#include "AppModel.h"
#include "Signers/QSignerManagement.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"
#include <descriptor.h>
#include "Models/WalletModel.h"                                                             

QPair<int,int> parseKeyIndex(const QString &str)
{
    static const QRegularExpression re("^key_(\\d+)_(\\d+)$");
    QRegularExpressionMatch match = re.match(str);

    if (match.hasMatch()) {
        int first  = match.captured(1).toInt(); // number after "key_"
        int second = match.captured(2).toInt(); // number after first underscore
        return qMakePair(first, second);
    }
    return qMakePair(-1, -1); // invalid format
}

MiniscriptWallet::MiniscriptWallet(const nunchuk::Wallet &w) :
    SandboxWallet{w},
    m_timezones(QWalletTimezoneModelPtr(new QWalletTimezoneModel()))
{
    clearTimeMiniscript();
}

void MiniscriptWallet::convert(const nunchuk::Wallet w) {
    SandboxWallet::convert(w);
    convertToMiniscript(w);
    if(timelockType() == (int)nunchuk::Timelock::Type::LOCKTYPE_ABSOLUTE){
        m_sortByCoinAge = false;
    }
    else {
        m_sortByCoinAge = true;
    }
}

void MiniscriptWallet::convertToMiniscript(const nunchuk::Wallet &w) {
    std::string tmpl = w.get_miniscript();
    if (tmpl.empty())
        return;
    nunchuk::MiniscriptTimeline timeline(tmpl);
    setTimeUnit(static_cast<int>(timeline.get_lock_type())); // TIME_LOCK, HEIGHT_LOCK
    if (timeline.get_absolute_locks().size() > 0) {
        setTimelockType(static_cast<int>(nunchuk::Timelock::Type::LOCKTYPE_ABSOLUTE));
        setTimelocklist(timeline.get_absolute_locks());
    }
    else if (timeline.get_relative_locks().size() > 0) {
        setTimelockType(static_cast<int>(nunchuk::Timelock::Type::LOCKTYPE_RELATIVE));
        setTimelocklist(timeline.get_relative_locks());
    }

    std::vector<std::string> tmp;    
    nunchuk::ScriptNode script_node = qUtils::GetScriptNode(tmpl, tmp);
    nunchuk::AddressType addressType = static_cast<nunchuk::AddressType>(walletAddressType());
    nunchuk::WalletTemplate wallet_template = w.get_wallet_template();
    if (addressType == nunchuk::AddressType::TAPROOT && wallet_template == nunchuk::WalletTemplate::DEFAULT && !w.get_signers().empty()) {
        QStringList keypathsList;
        std::vector<nunchuk::SingleSigner> keysPath;
        for(int i = 0; i < w.get_m(); i++) {
            const nunchuk::SingleSigner &signer = w.get_signers()[i];
            keysPath.push_back(signer);
        }
        for (const auto& keypath_signer : keysPath) {
            std::string keypath = nunchuk::GetDescriptorForSigner(keypath_signer, nunchuk::DescriptorPath::EXTERNAL_ALL);
            keypathsList.append(QString::fromStdString(keypath));
        }
        setKeypaths(createKeypaths(keypathsList));
    }
    QList<int> levels {};
    QJsonArray tree = createTreeMiniscript(script_node, levels);
    setTreeMiniscript(tree);
}

bool MiniscriptWallet::isValidTapscriptTemplate(const QString &userInput) {
    QString tmpInput = userInput;
    tmpInput.remove(QRegExp("[\\s]")); //\\n\\r
    std::string error;
    std::string user_input = tmpInput.toStdString();
    bool is_taproot = qUtils::IsValidTapscriptTemplate(user_input, error);
    DBG_INFO << is_taproot << user_input << error;
    return is_taproot;
}

bool MiniscriptWallet::isValidMiniscriptTemplate(const QString &userInput)
{
    QString tmpInput = userInput;
    tmpInput.remove(QRegExp("[\\s]")); //\\n\\r
    std::string user_input = tmpInput.toStdString();
    nunchuk::AddressType address_type = static_cast<nunchuk::AddressType>(walletAddressType());
    bool is_miniscript = qUtils::IsValidMiniscriptTemplate(user_input, address_type);
    DBG_INFO << is_miniscript << user_input;
    return is_miniscript;
}

bool MiniscriptWallet::enterCustomMiniscript(const QString &userInput) {
    setWalletType(static_cast<int>(nunchuk::WalletType::MINISCRIPT));
    QString tmpInput = userInput;
    tmpInput.remove(QRegExp("[\\s]")); //\\n\\r
    std::string tmpl;
    std::string error;
    std::string user_input = tmpInput.toStdString();
    DBG_INFO << "MiniscriptWallet::enterCustomMiniscript: walletAddressType:" << walletAddressType();
    nunchuk::AddressType address_type = static_cast<nunchuk::AddressType>(walletAddressType());
    if (qUtils::IsValidMiniscriptTemplate(user_input, address_type)) {
        tmpl = user_input;
    }
    else if (qUtils::IsValidPolicy(user_input)) {
        tmpl = qUtils::PolicyToMiniscript(user_input, {}, address_type);
    }
    else if (address_type == nunchuk::AddressType::TAPROOT && qUtils::IsValidTapscriptTemplate(user_input, error)) {
        tmpl = user_input;
    }
    else {
        AppModel::instance()->showToast(-1, "Format not supported", EWARNING::WarningType::ERROR_MSG);
        DBG_INFO << "MiniscriptWallet::enterCustomMiniscript: ERROR:" << error;
        return false; // Invalid input
    }
    reformatMiniscript(QString::fromStdString(tmpl));
    return true;
}

bool MiniscriptWallet::miniscriptTemplateSelected(const QString &userSelect) {
    setWalletType(static_cast<int>(nunchuk::WalletType::MINISCRIPT));
    std::string tmpl;
    int m = walletM();
    int n = walletN();
    int new_m = newWalletM();
    int new_n = newWalletN();
    bool reuse_signers = reUseKeys();
    nunchuk::AddressType address_type = static_cast<nunchuk::AddressType>(walletAddressType());
    nunchuk::Timelock::Type timelock_type = static_cast<nunchuk::Timelock::Type>(timelockType());
    nunchuk::Timelock::Based timelock_based = static_cast<nunchuk::Timelock::Based>(timeUnit());

    QWarningMessage msg;
    if (userSelect == "expanding-multisig") {
        nunchuk::Timelock expanding_time(timelock_based, timelock_type, timeMiniValue());
        tmpl = qUtils::ExpandingMultisigMiniscriptTemplate(m, n, new_n, reuse_signers, expanding_time, address_type, msg);
    } else if (userSelect == "decaying-multisig") {
        nunchuk::Timelock decay_time(timelock_based, timelock_type, timeMiniValue());
        tmpl = qUtils::DecayingMultisigMiniscriptTemplate(m, n, new_m, reuse_signers, decay_time, address_type, msg);
    } else if (userSelect == "flexible-multisig") {
        nunchuk::Timelock flexible_time(timelock_based, timelock_type, timeMiniValue());
        tmpl = qUtils::FlexibleMultisigMiniscriptTemplate(m, n, new_m, new_n, reuse_signers, flexible_time, address_type, msg);
    } else if (userSelect == "zen-hodl") {
        nunchuk::Timelock zen_hodl_time(timelock_based, timelock_type, timeMiniValue());
        tmpl = qUtils::ZenHodlMiniscriptTemplate(m, n, zen_hodl_time, address_type, msg);
    } else {
        DBG_INFO << "MiniscriptWallet::miniscriptTemplateSelected: ERROR: unknown template type";
        return false; // Invalid input
    }
    
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        reformatMiniscript(QString::fromStdString(tmpl));
        return true;
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        return false;
    }
}

int MiniscriptWallet::newWalletN() const {
    return m_newWalletN;
}

void MiniscriptWallet::setNewWalletN(int n) {
    if (m_newWalletN != n) {
        m_newWalletN = n;
        emit newWalletNChanged();
    }
}

int MiniscriptWallet::newWalletM() const {
    return m_newWalletM;
}

void MiniscriptWallet::setNewWalletM(int m) {
    if (m_newWalletM != m) {
        m_newWalletM = m;
        emit newWalletMChanged();
    }
}

bool MiniscriptWallet::reUseKeys() const {
    return m_reUseKeys;
}

void MiniscriptWallet::setReUseKeys(bool reUse) {
    if (m_reUseKeys != reUse) {
        m_reUseKeys = reUse;
        emit reUseKeysChanged();
    }
}

QString MiniscriptWallet::scriptTemplate() {
    QString tmpInput = m_customizeMiniscript;
    tmpInput.remove(QRegExp("[\\s]")); //\\n\\r
    return tmpInput;
}

QString MiniscriptWallet::customizeMiniscript() {
    return m_customizeMiniscript;
}

void MiniscriptWallet::setCustomizeMiniscript(const QString &miniscript) {
    if (m_customizeMiniscript != miniscript) {
        m_customizeMiniscript = miniscript;
        DBG_INFO << m_customizeMiniscript;
        emit customizeMiniscriptChanged();
    }
}

void MiniscriptWallet::reformatMiniscript(const QString &miniscript) {
    if (m_customizeMiniscript != miniscript) {
        m_customizeMiniscript = qUtils::formatMiniscript(miniscript);
        emit customizeMiniscriptChanged();
    }
}

int MiniscriptWallet::timelockType() const {
    return m_timelockType;
}

void MiniscriptWallet::setTimelockType(int type) {
    if (m_timelockType != type) {
        m_timelockType = type;
        emit timelockTypeChanged();
    }
}

int MiniscriptWallet::timeUnit() const {
    return m_timeUnit;
}

void MiniscriptWallet::setTimeUnit(int unit) {
    if (m_timeUnit != unit) {
        m_timeUnit = unit;
        emit timeUnitChanged();
    }
}

QVariant MiniscriptWallet::timeMini() const {
    return QVariant::fromValue(m_timeMiniData);
}

QVariantList MiniscriptWallet::treeMiniscript() const {
    return m_treeMiniscript.toVariantList();
}

QJsonArray MiniscriptWallet::treeMiniscriptJs() const {
    return m_treeMiniscript;
}

void MiniscriptWallet::setTreeMiniscript(const QJsonArray &tree) {
    m_treeMiniscript = tree;    
    emit treeMiniscriptChanged();
}

quint64 MiniscriptWallet::timeMiniValue() const {
    quint64 value = 0;
    nunchuk::Timelock::Type timelock_type = static_cast<nunchuk::Timelock::Type>(timelockType());
    nunchuk::Timelock::Based timelock_based = static_cast<nunchuk::Timelock::Based>(timeUnit());
    if (timelock_type == nunchuk::Timelock::Type::LOCKTYPE_ABSOLUTE && timelock_based == nunchuk::Timelock::Based::TIME_LOCK) {
        QByteArray selectedTimezone = timezonesPtr()->selectedTimezoneId();
        QString date_str = m_timeMiniData["absoluteTimestamp"].toMap().value("valueDate").toString();
        QString time_str = m_timeMiniData["absoluteTimestamp"].toMap().value("valueTime").toString();
        QDate date = QDate::fromString(date_str, "MM/dd/yyyy");
        QTime time = QTime::fromString(time_str, "hh:mm");
        QDateTime dt((QDate(date)), QTime(time), QTimeZone(selectedTimezone));

        qint64 epochsecs = dt.toSecsSinceEpoch(); // UTC seconds
        value =  epochsecs; //qUtils::GetTimeSecond(tmp);
    }
    else if (timelock_type == nunchuk::Timelock::Type::LOCKTYPE_ABSOLUTE && timelock_based == nunchuk::Timelock::Based::HEIGHT_LOCK) {
        value = m_timeMiniData["absoluteBlockheight"].toULongLong();
    }
    else if (timelock_type == nunchuk::Timelock::Type::LOCKTYPE_RELATIVE && timelock_based == nunchuk::Timelock::Based::TIME_LOCK) {
        int day = m_timeMiniData["relativeTimestamp"].toMap().value("valueDay").toInt();
        int hour = m_timeMiniData["relativeTimestamp"].toMap().value("valueHour").toInt();
        int minute = m_timeMiniData["relativeTimestamp"].toMap().value("valueMinute").toInt();
        // Convert to seconds
        value = (day * 24 * 3600) + (hour * 3600) + (minute * 60);
    }
    else if (timelock_type == nunchuk::Timelock::Type::LOCKTYPE_RELATIVE && timelock_based == nunchuk::Timelock::Based::HEIGHT_LOCK) {
        value = m_timeMiniData["relativeBlockheight"].toULongLong();
    }

    DBG_INFO << "timelock_type:" << (int)timelock_type
             << "timelock_based:" << (int)timelock_based
             << "value:" << value;
    return value;
}

QJsonObject MiniscriptWallet::getKeyDetails(const QJsonObject &oldKey, const QString &key) {
    QJsonObject keyDetails = oldKey;
    auto parseSignerString = [this](const QJsonObject &oldKey, const QString &key, const QString &keySpec) -> QJsonObject {
        QJsonObject keyDetails = oldKey;
        QWarningMessage msg;
        nunchuk::SingleSigner signer = qUtils::ParseSignerString(keySpec, msg);
        bool has_signer = bridge::nunchukHasSinger(signer);
        if (has_signer) {
            signer = bridge::nunchukGetOriginSingleSigner(signer, msg);
        }        
        if (msg.type() == (int)EWARNING::WarningType::NONE_MSG) {
            auto s = QSingleSignerPtr(new QSingleSigner(signer));
            if (s != nullptr) {
                if (!has_signer) {
                    qUtils::ParseSignerString(key, msg);
                    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                        if (!this->m_defaultKeys.contains(s->masterFingerPrint())) {
                            auto lastKey = this->m_defaultKeys.keys().size() > 0 ? this->m_defaultKeys.values().last() : 0;
                            this->m_defaultKeys.insert(s->masterFingerPrint(), lastKey + 1);
                        } 
                        auto keyIndex = this->m_defaultKeys.value(s->masterFingerPrint());
                        s->setName(QString("Key %1").arg(keyIndex));
                    } else {
                        s->setName("Key " + key);
                    }                    
                }
                keyDetails["key"] = key;
                keyDetails["keyObj"] = QJsonValue::fromVariant(SingleSignerListModel::useQml(s));
                keyDetails["keyStr"] = keySpec;
            } else {
                keyDetails["key"] = key;
                keyDetails["keyObj"] = {};
                keyDetails["keyStr"] = {};
            }
        } else {
            keyDetails["key"] = key;
            keyDetails["keyObj"] = {};
            keyDetails["keyStr"] = {};
        }
        return keyDetails;
    };
    
    if (m_signersMiniscript.contains(key)) {
        std::string keyStr = key.toStdString();
        QWarningMessage msg;
        qUtils::ParseSignerString(key, msg);
        if ((int)EWARNING::WarningType::NONE_MSG != msg.type()) {
            auto p = m_signersMiniscript.value(key);
            if (!p.isNull()) {
                if (!p->masterFingerPrint().isEmpty()) {
                    keyStr = nunchuk::GetDescriptorForSigner(p->originSingleSigner(), nunchuk::DescriptorPath::EXTERNAL_ALL);
                }                
            }            
        }
        
        keyDetails = parseSignerString(oldKey, key, QString::fromStdString(keyStr));
    } else {
        keyDetails = parseSignerString(oldKey, key, key);
    }
    return keyDetails;
}

QJsonArray MiniscriptWallet::createTreeMiniscript(const nunchuk::ScriptNode &node, QList<int> &levels) {
    // display current node
    QJsonArray treeLines;
    QString firstLine = "";
    QJsonObject firstLineObj;
    nunchuk::ScriptNodeId nodeId = node.get_id();    
    firstLine = qUtils::ScriptNodeIdToString(nodeId);
    firstLineObj["firstLine"] = firstLine;
    firstLineObj["enable"] = true; // Default to true
    firstLineObj["type"] = static_cast<int>(node.get_type());
    firstLineObj["typeString"] = QString::fromStdString(nunchuk::ScriptNode::type_to_string(node.get_type()));
    firstLineObj["m"] = 0;
    if (node.get_type() == nunchuk::ScriptNode::Type::MULTI ||
        node.get_type() == nunchuk::ScriptNode::Type::MUSIG){
        firstLineObj["m"] = static_cast<int>(node.get_k());
        firstLineObj["n"] = static_cast<int>(node.get_keys().size());
    }
    else if (node.get_type() == nunchuk::ScriptNode::Type::THRESH) {
        firstLineObj["m"] = static_cast<int>(node.get_k());
        firstLineObj["n"] = static_cast<int>(node.get_subs().size());
    }
    
    switch (node.get_type()) {
    case nunchuk::ScriptNode::Type::PK: {
        QJsonObject keyLineObj = firstLineObj;
        auto key = QString::fromStdString(node.get_keys()[0]);
        keyLineObj = getKeyDetails(keyLineObj, key);
        keyLineObj["levels"] = QJsonValue::fromVariant(qUtils::toVariant(levels));
        treeLines.append(keyLineObj);
        break;
    } 
    case nunchuk::ScriptNode::Type::MULTI:
    case nunchuk::ScriptNode::Type::MUSIG: {
        firstLineObj["levels"] = QJsonValue::fromVariant(qUtils::toVariant(levels));
        treeLines.append(firstLineObj);
        QList<int> tmpLevels = levels;
        if (node.get_keys().size() > 0) {
            if (levels.size() > 0) {
                tmpLevels.append(levels.last() + 1); // +1 for the dot
            } else {
                tmpLevels.append(1); // Start from 0 if levels is empty
            }
        }
        for (int i = 0; i < node.get_keys().size(); i++) {
            QString firstLineKey = QString("%1%2.").arg(firstLine).arg(i + 1);
            QJsonObject keyLineObj = firstLineObj;
            keyLineObj["firstLine"] = firstLineKey;
            auto key = QString::fromStdString(node.get_keys()[i]);
            keyLineObj = getKeyDetails(keyLineObj, key);
            keyLineObj["levels"] = QJsonValue::fromVariant(qUtils::toVariant(tmpLevels));
            keyLineObj["isLast"] = i == (node.get_keys().size() - 1);
            treeLines.append(keyLineObj);
        }
        break;
    } 
    case nunchuk::ScriptNode::Type::AFTER: {
        nunchuk::Timelock timelock = nunchuk::Timelock::FromK(true, node.get_k());
        nunchuk::Timelock::Based lockType = timelock.based();
        firstLineObj["lockType"] = static_cast<int>(lockType);
        firstLineObj["timelockType"] = static_cast<int>(timelock.type());
        auto absolute_lock = timelock.value();
        if (lockType == nunchuk::Timelock::Based::TIME_LOCK) {
            QTime time(0, 0);
            QDateTime datetime(QDate::currentDate().addDays(30), time, Qt::LocalTime);
            QString date_str = qUtils::getDateFromTimestamp(absolute_lock);
            QString time_str = qUtils::getTimeFromTimestamp(absolute_lock);

            QVariantMap absolute_time_data;
            absolute_time_data["valueDate"] = date_str;
            absolute_time_data["valueTime"] = time_str;
            absolute_time_data["valueDisplay"] = (time_str != "00:00") ? date_str + " " + time_str : date_str;;
            QByteArray selectedTimezone = timezonesPtr()->selectedTimezoneId();
            QTimeZone tz(selectedTimezone);
            QDate input_date = QDate::fromString(date_str, "MM/dd/yyyy");
            QTime intput_time = QTime::fromString(time_str, "hh:mm");
            QDateTime dt(input_date, intput_time, tz);
            QDateTime now = QDateTime::currentDateTimeUtc().toTimeZone(tz);
            qint64 secsRemaining = now.secsTo(dt);
            QString valueRemaining;
            QString valueFrom = "today";
            if (secsRemaining <= 0) {
                valueRemaining = "expired";
            } else {
                int days = secsRemaining / (24 * 3600);
                QStringList parts;
                if (days > 0){
                    parts << QString("%1 day%2").arg(days).arg(days > 1 ? "s" : "");
                }
                else{
                    int hours = (secsRemaining % (24 * 3600)) / 3600;
                    int minutes = (secsRemaining % 3600) / 60;
                    valueFrom = "now";
                    if (hours > 0){
                        parts << QString("%1 hour%2").arg(hours).arg(hours > 1 ? "s" : "");
                    }
                    if (minutes > 0){
                        parts << QString("%1 minute%2").arg(minutes).arg(minutes > 1 ? "s" : "");
                    }
                }
                valueRemaining = parts.join(" ");
            }
            absolute_time_data["valueRemaining"] = valueRemaining;
            absolute_time_data["valueFrom"]      = valueFrom;
            firstLineObj["hasUnlocked"] = secsRemaining > 0 ? false : true;
            firstLineObj["absoluteTimestamp"]  = QJsonObject::fromVariantMap(absolute_time_data);
        }
        else if (lockType == nunchuk::Timelock::Based::HEIGHT_LOCK) {
            firstLineObj["absoluteBlockheight"] = (double)absolute_lock;
            firstLineObj["hasUnlocked"] = absolute_lock > 0 ? false : true;
        }
        else {
            firstLineObj["absoluteTimestamp"] = "N/A";
            firstLineObj["absoluteBlockheight"] = 0;
            firstLineObj["hasUnlocked"] = true;
        }
        firstLineObj["levels"] = QJsonValue::fromVariant(qUtils::toVariant(levels));
        treeLines.append(firstLineObj);
        break;
    } 
    case nunchuk::ScriptNode::Type::OLDER: {
        nunchuk::Timelock timelock = nunchuk::Timelock::FromK(false, node.get_k());
        nunchuk::Timelock::Based lockType = timelock.based();
        firstLineObj["lockType"] = static_cast<int>(lockType);
        firstLineObj["timelockType"] = static_cast<int>(timelock.type());
        auto relative_lock = timelock.value();
        if (lockType == nunchuk::Timelock::Based::TIME_LOCK) {
            QString day = QString::number(relative_lock / (24 * 3600));
            relative_lock %= (24 * 3600);
            QString hour = QString::number(relative_lock / 3600);
            relative_lock %= 3600;
            QString minute = QString::number(relative_lock / 60);
            relative_lock %= 60;
            if (relative_lock > 0) {
                minute = QString::number(relative_lock);
            } else {
                minute = "0";
            }

            QStringList parts;
            parts << QString("%1d").arg(day);
            if (hour.toInt() != 0){
                parts << QString("%1h").arg(hour);
            }
            if (minute.toInt() != 0){
                parts << QString("%1m").arg(minute);
            }
            QString valueDisplay = parts.join(" ");
            QVariantMap relative_time_data;
            relative_time_data["valueDay"] = day;
            relative_time_data["valueHour"] = hour;
            relative_time_data["valueMinute"] = minute;
            relative_time_data["valueDisplay"] = valueDisplay;
            firstLineObj["hasUnlocked"] = relative_lock > 0 ? false : true;
            firstLineObj["relativeTimestamp"] = QJsonObject::fromVariantMap(relative_time_data);
        }
        else if (lockType == nunchuk::Timelock::Based::HEIGHT_LOCK) {
            firstLineObj["hasUnlocked"] = relative_lock > 0 ? false : true;
            firstLineObj["relativeBlockheight"] = (double)relative_lock;
        }
        else {
            firstLineObj["relativeTimestamp"] = 0;
            firstLineObj["relativeBlockheight"] = 0;
            firstLineObj["hasUnlocked"] = true;
        }
        firstLineObj["levels"] = QJsonValue::fromVariant(qUtils::toVariant(levels));
        treeLines.append(firstLineObj);
        break;
    } 
    case nunchuk::ScriptNode::Type::HASH160:
    case nunchuk::ScriptNode::Type::HASH256:
    case nunchuk::ScriptNode::Type::RIPEMD160:
    case nunchuk::ScriptNode::Type::SHA256: {
        std::vector<uint8_t> hash = node.get_data();
        firstLineObj["hashData"] = qUtils::BytesToHex(hash);
        treeLines.append(firstLineObj);
        break;
    } 
    default: {
        firstLineObj["levels"] = QJsonValue::fromVariant(qUtils::toVariant(levels));
        treeLines.append(firstLineObj);
        break;
    }
    }

    QList<int> tmpLevels = levels;
    if (node.get_subs().size() > 0) {
        if (levels.size() > 0) {
            tmpLevels.append(levels.last() + 1); // +1 for the dot
        } else {
            tmpLevels.append(1); // Start from 0 if levels is empty
        }
    }
    // loop through subs to show subtree
    for (int i = 0; i < node.get_subs().size(); i++) {
        QJsonArray tmpList = createTreeMiniscript(node.get_subs()[i], tmpLevels);
        for (int j = 0; j < tmpList.size(); j++) {
            QJsonObject subLineObj = tmpList.at(j).toObject();
            if (i == (node.get_subs().size() - 1)) {
                auto levels = subLineObj["levels"].toArray();
                QJsonArray newLevels;
                for (int k = 0; k < levels.size(); k++) {
                    if (k == 0) {
                        newLevels.append(0);
                    } else {
                        newLevels.append(levels.at(k)); // Increment level by 1 for the last item
                    }
                }
                if (j != 0) {
                    subLineObj["levels"] = newLevels;
                } else {
                    subLineObj["isLast"] = i == (node.get_subs().size() - 1);
                }
            }
            treeLines.append(subLineObj);
        }
    }
    return treeLines;
}

QVariantList MiniscriptWallet::keypaths() {
    if(m_keypaths.isEmpty() && keyPathActivated()){
        convertToMiniscript(nunchukWallet());
    }
    return m_keypaths.toVariantList();
}

QJsonArray MiniscriptWallet::keypathsJs() {
    if(m_keypaths.isEmpty() && keyPathActivated()){
        convertToMiniscript(nunchukWallet());
    }
    return m_keypaths;
}

QJsonArray MiniscriptWallet::createKeypaths(const QStringList &keypaths) {
    QJsonArray _keypaths = {};
    if (keypaths.size() == 1) {
        for (const auto &key : keypaths) {
            QJsonObject keyObj = getKeyDetails(QJsonObject(), key);
            keyObj["firstLine"] = "";
            _keypaths.append(keyObj);
        }
    } else {
        QString firstLine = "";
        QJsonObject firstLineObj;
        QList<int> levels = {};
        firstLineObj["firstLine"] = QString("1."); // must have first line
        firstLineObj["enable"] = true; // Default to true
        firstLineObj["type"] = static_cast<int>(nunchuk::ScriptNode::Type::MUSIG);
        firstLineObj["typeString"] = QString::fromStdString(nunchuk::ScriptNode::type_to_string(nunchuk::ScriptNode::Type::MUSIG));
        firstLineObj["m"] = keypaths.size();
        firstLineObj["n"] = keypaths.size();
        firstLineObj["levels"] = QJsonValue::fromVariant(qUtils::toVariant(levels));
        firstLineObj["isLast"] = false;
        _keypaths.append(firstLineObj);
        for(int i = 0; i < keypaths.size(); i++) {
            QString key = keypaths[i];
            QJsonObject keyLineObj = getKeyDetails(firstLineObj, key);
            levels = {1};
            keyLineObj["firstLine"] = QString("1.%1.").arg(i + 1);
            keyLineObj["levels"] = QJsonValue::fromVariant(qUtils::toVariant(levels));
            keyLineObj["isLast"] = i == (keypaths.size() - 1);
            _keypaths.append(keyLineObj);
        }
    }    
    return _keypaths;
}

void MiniscriptWallet::setKeypaths(const QJsonArray &keypath) {
    m_keypaths = keypath;
    emit keypathsChanged();
}

std::map<std::string, nunchuk::SingleSigner> MiniscriptWallet::signersCreateWallet() {
    std::map<std::string, nunchuk::SingleSigner> signers;
    for (auto it = m_signersMiniscript.constBegin(); it != m_signersMiniscript.constEnd(); ++it) {
        if (it.value() != nullptr) {
            signers[it.key().toStdString()] = (it.value())->originSingleSigner();
        }
    }
    return signers;
}

QString MiniscriptWallet::keySelected() const {
    return m_keySelected;
}

void MiniscriptWallet::setKeySelected(const QString &key) {
    if (m_keySelected != key) {
        m_keySelected = key;
    }
}

bool MiniscriptWallet::enoughSigners() const {
    QMap<QString, int> keyMapAdded = {};
    auto createMap = [] (const QJsonArray &tree, QMap<QString, int> &map) {
        for (auto js : tree) {
            QJsonObject jo = js.toObject();
            QString key = jo.value("key").toString();
            int countSameKey = jo.value("keyStrCount").toInt();
            if (key.isEmpty() == false) {
                map.insert(key, countSameKey);
            }
        }
    };
    createMap(m_keypaths, keyMapAdded);
    createMap(m_treeMiniscript, keyMapAdded);
    auto keys = keyMapAdded.keys();
    bool allKeyOK = true;
    for (auto it = keyMapAdded.constBegin(); it != keyMapAdded.constEnd(); ++it) {
        if (it.value() != 1) {
            allKeyOK = false;
        }
    }
    return allKeyOK;
}

bool MiniscriptWallet::AddMasterToWallet() {
    if (auto key = AppModel::instance()->masterSignerInfoPtr()) {
        QWarningMessage msg;
        QSingleSignerPtr signer = bridge::nunchukGetAvailableSignerFromMasterSigner(key, ENUNCHUCK::WalletType::MINISCRIPT,
                                                                                    static_cast<ENUNCHUCK::AddressType>(walletAddressType()), msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            AddSignerToWallet(signer);
            return true;
        }
        else {
            if(nunchuk::NunchukException::RUN_OUT_OF_CACHED_XPUB == msg.code()){
                emit needTopUpXpub();
                return false;
            }
        }
    }
    return false;
}

bool MiniscriptWallet::AddRemoteToWallet() {
    if (auto signer = AppModel::instance()->singleSignerInfoPtr()) {
        AddSignerToWallet(signer);    
    }
    return false;
}

bool MiniscriptWallet::AddSignerToWallet(const QSingleSignerPtr &signerPtr) {
    if (signerPtr.isNull()) {
        return false;
    }
    QtConcurrent::run([this, signerPtr]() {
        updateSignersMiniscript(keySelected(), signerPtr);
        auto signer = signerPtr->originSingleSigner();
        auto listKey = keySameList(keySelected());
        setNeedCheckDuplicate(true);
        for (const auto &k : listKey) {
            if (k != keySelected()) {
                auto path = QString::fromStdString(signer.get_derivation_path());
                auto master_id = QString::fromStdString(signer.get_master_fingerprint());
                // Increment the last index in the BIP32 path
                auto new_path = qUtils::incrementZeroIndex(path);
                QWarningMessage msgTmp;
                nunchuk::SingleSigner signer_tmp = bridge::GetSignerFromMasterSigner(master_id, new_path, msgTmp);
                if (msgTmp.type() == (int)EWARNING::WarningType::NONE_MSG) {
                    signer = signer_tmp;
                    updateSignersMiniscript(k, QSingleSignerPtr(new QSingleSigner(signer_tmp)));
                } else {
                    DBG_ERROR << "Failed to derive new signer for key:" << k << " Error:" << msgTmp.what();
                }
            }
        }        
    });   
    return true;
}

void MiniscriptWallet::registerSigners() {
    auto masterFunc = []() -> bool {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->AddMasterToWallet();
            w->configureWallet();
            w->setScreenFlow("setup-mini-script");
            QEventProcessor::instance()->sendEvent(E::EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST);
        }
        return true;
    };

    QString mnemonic = qUtils::GenerateMnemonic();
    AppModel::instance()->setMnemonic(mnemonic);
    QSignerManagement::instance()->registerCreateMasterSigner(masterFunc);
    QSignerManagement::instance()->registerCreateSoftwareSigner(masterFunc);
    QSignerManagement::instance()->registerCreateSoftwareSignerXprv(masterFunc);
    QSignerManagement::instance()->registerCreateRemoteSigner([]() -> bool {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->AddRemoteToWallet();
            w->configureWallet();
            w->setScreenFlow("setup-mini-script");
            QEventProcessor::instance()->sendEvent(E::EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST);
        }
        return true;
    });
}

void MiniscriptWallet::makeExistingSigners() {
    QJsonArray arrays = bridge::makeExistingSigners(static_cast<nunchuk::AddressType>(walletAddressType()), nunchuk::WalletType::MINISCRIPT);
    setSignerExistList(arrays.toVariantList());
    dynamic_cast<Wallet *>(this)->CreateAssignAvailableSigners();
}

QStringList MiniscriptWallet::keySameList(const QString& key_i_j) {
    auto findKey = [](const QJsonArray &tree, const QString &key_i_j) -> QStringList {
        QStringList keysRet {};
        for (const auto &js : tree) {
            QJsonObject jo = js.toObject();
            auto key = jo.value("key").toString();
            int key_i_j_index = parseKeyIndex(key_i_j).first;
            int key_index = parseKeyIndex(key).first;
            if (key_i_j_index == key_index && key_index != -1 && !qUtils::strCompare(key, key_i_j)) {
                keysRet.append(key);
            }
        }
        return keysRet;
    };
    auto keys = findKey(m_treeMiniscript, key_i_j);
    auto pkeys = findKey(m_keypaths, key_i_j);
    auto allKeys = keys + pkeys;
    allKeys.append(key_i_j);
    return allKeys;
}

bool MiniscriptWallet::keyPathActivated()
{
    int wallet_Type = walletType();
    int walletAddress_Type = walletAddressType();
    int walletTemplate_Type = walletTemplate();
    if(((int)nunchuk::AddressType::TAPROOT == walletAddress_Type)
        && ((int)nunchuk::WalletType::MINISCRIPT == wallet_Type)
        && ((int)nunchuk::WalletTemplate::DEFAULT == walletTemplate_Type))
    {
        return true;
    }
    else {
        return false;
    }
}

SingleSignerListModel* MiniscriptWallet::singleSignersKeyPath() {
    if(!m_signersKeyPath){
        m_signersKeyPath = QSingleSignerListModelPtr(new SingleSignerListModel());
    }
    if(m_signersKeyPath.data()->signerCount() == 0){
        m_signersKeyPath->cleardata();
        int m = walletM();
        std::vector<nunchuk::SingleSigner> signers = nunchukWallet().get_signers();
        for (int i = 0; i < m; i++) {
            nunchuk::SingleSigner signer = signers.at(i);
            QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
            m_signersKeyPath->addSingleSigner(ret);
        }
    }
    if(m_signersKeyPath.data()->signerCount() > 1){
        m_signersKeyPath->requestSort();
    }
    return m_signersKeyPath.data();
}

QWalletTimezoneModel *MiniscriptWallet::timezones() const
{
    return m_timezones.data();
}

QWalletTimezoneModelPtr MiniscriptWallet::timezonesPtr() const
{
    return m_timezones;
}

void MiniscriptWallet::setTimezones(QWalletTimezoneModelPtr timezones)
{
    if (m_timezones == timezones)
        return;
    m_timezones = timezones;
    emit timezonesChanged();
}

bool MiniscriptWallet::needCheckDuplicate() const {
    return m_needCheckDuplicate;
}

void MiniscriptWallet::setNeedCheckDuplicate(bool needCheck) {
    m_needCheckDuplicate = needCheck;
}

QVariantList MiniscriptWallet::timelocklist()
{
    return m_timelocklist;
}

void MiniscriptWallet::setTimelocklist(const std::vector<int64_t> timelocks)
{
    QVariantList ret;
    bool anyLocked = false;

    for (int i = 0; i < (int)timelocks.size(); ++i) {
        int64_t val = timelocks[i];
        QVariantMap obj;
        QString valueNodeStr;
        QString valueRemainingStr;
        QString valueFrom = "today";
        bool isLocked = false;
        if (timeUnit() == (int)nunchuk::Timelock::Based::TIME_LOCK) {
            QDateTime dt = QDateTime::fromSecsSinceEpoch(val, Qt::UTC);
            valueNodeStr = dt.toString("MM/dd/yyyy");
            QDate dateNode = dt.date();
            QDate today = QDateTime::currentDateTimeUtc().date();
            int days = today.daysTo(dateNode);
            if (days > 0) {
                valueRemainingStr = QString::number(days) + (days == 1 ? " day" : " days");
                isLocked = true;
            }
            else {
                qint64 secondsTo = QDateTime::currentDateTimeUtc().secsTo(dt);
                qint64 hours = secondsTo / 3600;
                qint64 minutes = (secondsTo % 3600) / 60;
                if (hours >= 1) {
                    valueRemainingStr = QString::number(hours) + (hours == 1 ? " hour" : " hours");
                    isLocked = true;
                }
                else if (minutes >= 1) {
                    valueRemainingStr = QString::number(minutes) + (minutes == 1 ? " minute" : " minutes");
                    isLocked = true;
                }
                else {
                    valueRemainingStr = "";
                    valueFrom = "now";
                    isLocked = false;
                }
            }
        }
        else {
            QLocale locale(QLocale::English);
            int64_t currentHeight = AppModel::instance()->blockHeight();
            int64_t remain = val - currentHeight;
            valueNodeStr = locale.toString(val);
            if (val > currentHeight) {
                QString remainStr = locale.toString(remain);
                valueRemainingStr = remainStr + (remain == 1 ? " block" : " blocks");
                isLocked = true;
            }
            else {
                valueRemainingStr = "";
                valueFrom = "now";
            }
        }

        if (isLocked) anyLocked = true;

        obj["valueNode"] = valueNodeStr;
        obj["valueIndex"] = i;
        obj["valueRemaining"] = valueRemainingStr;
        obj["valueLocked"] = isLocked;
        obj["valueFrom"] = valueFrom;
        ret << obj;
    }

    if (ret.size() == 1) {
        QVariantMap emptyObj;
        emptyObj["valueNode"] = "";
        emptyObj["valueIndex"] = ret.size();
        emptyObj["valueLocked"] = false;
        emptyObj["valueRemaining"] = anyLocked ? "" : "Unlocked";
        emptyObj["valueFrom"] = "today";
        ret << emptyObj;
    }
    setTimeLocked(anyLocked);
    m_timelocklist = ret;
    emit timelocklistChanged();
}

QString MiniscriptWallet::miniscript() const
{
    return QString::fromStdString(nunchukWallet().get_miniscript());
}

void MiniscriptWallet::updateTimeMiniscript(const QString &key, const QVariant &value) {

    if (m_timeMiniData[key] == value)
        return;

    if(qUtils::strCompare("absoluteTimestamp", key)) {
        QString date_str = value.toMap().value("valueDate").toString();
        QString time_str = value.toMap().value("valueTime").toString();
        QString valueDisplay = (time_str != "00:00") ? date_str + " " + time_str : date_str;
        QVariantMap absolute_data;
        absolute_data["valueDisplay"] = valueDisplay;
        absolute_data["valueDate"] = date_str;
        absolute_data["valueTime"] = time_str;

        QByteArray selectedTimezone = timezonesPtr()->selectedTimezoneId();
        QTimeZone tz(selectedTimezone);
        QDate date = QDate::fromString(date_str, "MM/dd/yyyy");
        QTime time = QTime::fromString(time_str, "hh:mm");
        QDateTime dt(date, time, tz);
        QDateTime now = QDateTime::currentDateTimeUtc().toTimeZone(tz);
        qint64 secsRemaining = now.secsTo(dt);
        QString valueRemaining;
        QString valueFrom = "today";
        if (secsRemaining <= 0) {
            valueRemaining = "expired";
        }
        else {
            int days = secsRemaining / (24 * 3600);
            QStringList parts;
            if (days > 0){
                parts << QString("%1 day%2").arg(days).arg(days > 1 ? "s" : "");
            }
            else {
                int hours = (secsRemaining % (24 * 3600)) / 3600;
                int minutes = (secsRemaining % 3600) / 60;
                valueFrom = "now";
                if (hours > 0){
                    parts << QString("%1 hour%2").arg(hours).arg(hours > 1 ? "s" : "");
                }
                if (minutes > 0){
                    parts << QString("%1 minute%2").arg(minutes).arg(minutes > 1 ? "s" : "");
                }
            }
            valueRemaining = parts.join(" ");
        }
        absolute_data["valueRemaining"] = valueRemaining;
        absolute_data["valueFrom"]      = valueFrom;
        m_timeMiniData[key] = absolute_data;
    }
    else if(qUtils::strCompare("relativeTimestamp", key)) {
        QString day = value.toMap().value("valueDay").toString();
        QString hour = value.toMap().value("valueHour").toString();
        QString minute = value.toMap().value("valueMinute").toString();
        QStringList parts;
        parts << QString("%1d").arg(day);
        if (hour.toInt() != 0){
            parts << QString("%1h").arg(hour);
        }
        if (minute.toInt() != 0){
            parts << QString("%1m").arg(minute);
        }
        QString valueDisplay = parts.join(" ");
        QVariantMap relative_data;
        relative_data["valueDay"] = day;
        relative_data["valueHour"] = hour;
        relative_data["valueMinute"] = minute;
        relative_data["valueDisplay"] = valueDisplay;
        m_timeMiniData[key] = relative_data;
    }
    else {
        m_timeMiniData[key] = QString::number(value.toULongLong());
    }
    emit timeMiniChanged();
}

void MiniscriptWallet::updateTimeMiniscript(const QMap<QString, QVariant> &dataMap) {
    for (auto it = dataMap.constBegin(); it != dataMap.constEnd(); ++it) {
        m_timeMiniData[it.key()] = it.value();
    }
    emit timeMiniChanged();
}

void MiniscriptWallet::clearTimeMiniscript(const QString &userInput) {
    m_timeMiniData.clear();
    QTime time(0, 0);
    int days = userInput == "zen-hodl" ? 90 : 30;
    QDateTime datetime(QDate::currentDate().addDays(days), time, Qt::LocalTime);
    QVariantMap absolute_time_data;
    absolute_time_data["valueDisplay"] = datetime.toString("MM/dd/yyyy");
    absolute_time_data["valueDate"] = datetime.toString("MM/dd/yyyy");
    absolute_time_data["valueTime"] = time.toString("hh:mm");
    absolute_time_data["valueRemaining"] = "90d";
    absolute_time_data["valueFrom"]      = "today";
    m_timeMiniData["absoluteTimestamp"] = absolute_time_data;

    QVariantMap relative_time_data;
    relative_time_data["valueDay"] = "90";
    relative_time_data["valueHour"] = "0";
    relative_time_data["valueMinute"] = "0";
    relative_time_data["valueDisplay"] = "90d";
    m_timeMiniData["relativeTimestamp"] = relative_time_data;

    m_timeMiniData["absoluteBlockheight"] = 4320;
    m_timeMiniData["relativeBlockheight"] = 4320;
    emit timeMiniChanged();
}

bool MiniscriptWallet::configureWallet(const QString &script_tmpl) {
    std::string tmpl = script_tmpl.toStdString();
    if (tmpl.empty()) {
        tmpl = scriptTemplate().toStdString();
    }
    std::vector<std::string> keypaths;
    nunchuk::ScriptNode script_node = qUtils::GetScriptNode(tmpl, keypaths);
    nunchuk::AddressType addressType = static_cast<nunchuk::AddressType>(walletAddressType());
    QStringList keypathsList;
    QStringList keyStrs;
    QJsonArray keyPaths;
    setWalletType(static_cast<int>(nunchuk::WalletType::MINISCRIPT));
    if (addressType == nunchuk::AddressType::TAPROOT) {
        if (!keypaths.empty()) {
            setWalletTemplate((int)nunchuk::WalletTemplate::DEFAULT);
            for (const auto& keypath : keypaths) {
                keypathsList.append(QString::fromStdString(keypath));
            }          
            keyPaths = createKeypaths(keypathsList);
            for (const auto &key : keyPaths) {
                QJsonObject keyObj = key.toObject();
                QString keyStr = keyObj.value("keyStr").toString();
                if (!keyStr.isEmpty()) {
                    keyStrs.append(keyStr);
                }                
            }           
        }
    }
    QList<int> levels {};
    QJsonArray scriptPaths = createTreeMiniscript(script_node, levels);
    auto duplicateKeys = getKeyStrCount(scriptPaths, keyPaths);

    QJsonArray sPathRefresh = scriptPathRefresh(scriptPaths, duplicateKeys);
    QJsonArray kPathsRefresh = keyPathRefresh(keyPaths, duplicateKeys);
    setKeypaths(kPathsRefresh);
    setTreeMiniscript(sPathRefresh);
    checkDuplicateKey();   
    return !scriptPaths.isEmpty();
}

bool MiniscriptWallet::importMiniscriptFile(const QString &filePath) {
    QString path = qUtils::QGetFilePath(filePath);
    QString fileContent = qUtils::ImportDataViaFile(path);
    reformatMiniscript(fileContent);
    return true;
}

void MiniscriptWallet::updateSignersMiniscript(const QString &key, const QSingleSignerPtr &value, bool autoreuse) {
    if (m_signersMiniscript.contains(key)) {
        auto old = m_signersMiniscript.value(key);
        if(!value.isNull() && !value->masterFingerPrint().isEmpty()) {
            m_signersMiniscript[key] = value; // Replace with new value
        } else {
            m_signersMiniscript[key] = old;
        }   
    } else {
        if(!value.isNull() && !value->masterFingerPrint().isEmpty()) {
            m_signersMiniscript.insert(key, value);
        } else if (!value.isNull() && value->isOccupied()) {
            m_signersMiniscript.insert(key, value);
        }
    }
    
    if(autoreuse){
        auto listKey = keySameList(key);
        for(const auto &k : listKey) {
            if (!qUtils::strCompare(k, key)) {
                if (!m_signersMiniscript.contains(k)) {
                    if(!value.isNull() && !value->masterFingerPrint().isEmpty()) {
                        m_signersMiniscript.insert(k, value);
                    }

                }
            }
        }
    }
}

void MiniscriptWallet::removeSignersMiniscript(const QString &key) {
    DBG_INFO << "Remove key: " << key << m_signersMiniscript.keys();
    if (m_signersMiniscript.contains(key)) {
        m_signersMiniscript.remove(key);
    }
}

void MiniscriptWallet::clearSignersMiniscript() {
    m_signersMiniscript.clear();
}

void MiniscriptWallet::requestAddSigner(const QString group, const QString &key) {
    QSignerManagement::instance()->clearExecute();
    setKeySelected(key);
    registerSigners();
    makeExistingSigners();
    bool existSigner = signerExistList().size() > 0;
    if (existSigner) {
        setScreenFlow("miniscript-add-existing-key");
    } else {
        QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_NEW_SIGNER_REQUEST);
    }
}

void MiniscriptWallet::requestRemoveSigner(const QString group, const QString &key) {
    removeSignersMiniscript(key);
    configureWallet();
}

void MiniscriptWallet::requestAddNewKey() {
    registerSigners();
    QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_NEW_SIGNER_REQUEST);
}

void MiniscriptWallet::requestAddExistKey(const QString &xfp) {
    QSignerManagement::instance()->clearExecute();
    const auto available_signers = dynamic_cast<Wallet *>(this)->assignAvailableSigners();
    auto key = available_signers->getSingleSignerByFingerPrint(xfp);
    if (!key) {
        DBG_ERROR << "No signer found with fingerprint:" << xfp;
        return;
    }
    QWarningMessage msg;
    QSingleSignerPtr signer = bridge::nunchukGetAvailableSignerFromSingleSigner(key, ENUNCHUCK::WalletType::MINISCRIPT, static_cast<ENUNCHUCK::AddressType>(walletAddressType()), msg);
    
    if(nunchuk::NunchukException::RUN_OUT_OF_CACHED_XPUB == msg.code()){
        emit needTopUpXpub();
        return;
    }
    if (!signer.isNull() && (int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        AddSignerToWallet(signer);
        configureWallet();
        setScreenFlow("setup-mini-script");       
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void MiniscriptWallet::requestChangeWalletTypeToTaproot() {
    setWalletAddressType((int)nunchuk::AddressType::TAPROOT);
    AppModel::instance()->showToast(0, "Address type changed to Taproot", EWARNING::WarningType::SUCCESS_MSG);
}

bool MiniscriptWallet::editBIP32Path(const QVariant &singleData, const QVariant &customData, const QString &path) {
    auto maps = singleData.toMap();
    auto customMaps = customData.toMap();
    QString master_id = maps.value("singleSigner_masterSignerId").toString();
    QString key = customMaps.value("key").toString();
    DBG_INFO << "key: " << key << "master_id: " << master_id << "path: " << path;
    if (!qUtils::QIsValidDerivationPath(path)) {
        emit editBIP32PathSuccess(-1); // Invalid format
        return false;
    }
    QPointer<MiniscriptWallet> safeThis(this);
    struct DataStruct {
        nunchuk::SingleSigner signer{};
        int errorType{0};
    };
    setKeySelected(key);
    runInConcurrent(
        [safeThis, master_id, path, key]() -> DataStruct {
            SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
            QWarningMessage msg;
            nunchuk::SingleSigner signer = bridge::GetSignerFromMasterSigner(master_id, path, msg);
            DataStruct data;
            if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                msg.resetWarningMessage();
                data.signer = signer;
                data.errorType = 1; // Success
            } else {
                data.errorType = -2;
            }
            return data;
        },
        [safeThis](DataStruct data) {
            SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
            if (data.errorType == 1) {
                auto s = QSingleSignerPtr(new QSingleSigner(data.signer));
                safeThis->updateSignersMiniscript(safeThis->keySelected(), s);
                safeThis->configureWallet();
            }
            emit ptrLamda->editBIP32PathSuccess(data.errorType); // -2 Not Found Key
        });
    return false;
}


QMap<QString, int> MiniscriptWallet::getKeyStrCount(const QJsonArray &scriptPaths, const QJsonArray &keyPaths) {
    QMap<QString, QString> keyMap;
    keyMap.clear();
    QMap<QString, int> duplicateCheck;
    auto createKeyMap = [](const QJsonArray &tree, QMap<QString, QString> &keyMap) {
        for (const auto &js : tree) {
            QJsonObject jo = js.toObject();
            QString key = jo.value("key").toString();
            QString keyStr = jo.value("keyStr").toString();
            if (keyStr.isEmpty()) {
                continue; // Skip empty keys
            }
            keyMap.insertMulti(keyStr, key);
        }
    };

    createKeyMap(scriptPaths, keyMap);
    createKeyMap(keyPaths, keyMap);

    for (const QString &str : keyMap.uniqueKeys()) {
        duplicateCheck[str] = keyMap.values(str).size();
    }
    return duplicateCheck;
}

QJsonArray MiniscriptWallet::scriptPathRefresh(const QJsonArray &tree, const QMap<QString, int> &duplicateCheck) {
    QJsonArray treeRefresh;
    for (const auto &js : tree) {
        QJsonObject jo = js.toObject();
        QString keyStr = jo.value("keyStr").toString();
        if (keyStr.isEmpty()) {
            treeRefresh.append(jo);
        } else {
            jo["keyStrCount"] = duplicateCheck[keyStr];
            treeRefresh.append(jo);
        }
    }
    return treeRefresh;
}

QJsonArray MiniscriptWallet::keyPathRefresh(const QJsonArray &tree, const QMap<QString, int> &duplicateCheck) {
    QJsonArray keyPathRefresh;
    for (const auto &js : tree) {
        QJsonObject jo = js.toObject();
        QString keyStr = jo.value("keyStr").toString();
        if (keyStr.isEmpty()) {
            keyPathRefresh.append(jo);
        } else {
            jo["keyStrCount"] = duplicateCheck[keyStr];
            keyPathRefresh.append(jo);
        }
    }
    return keyPathRefresh;
}

void MiniscriptWallet::checkDuplicateKey() {
    if (!needCheckDuplicate()) {
        return;
    }
    setNeedCheckDuplicate(false);
    timeoutHandler(200, [this] {
        auto findKey = [this](const QJsonArray &tree) -> QJsonObject {
            for (const auto &js : tree) {
                QJsonObject jo = js.toObject();
                auto key = jo.value("key").toString();
                if (key == this->keySelected()) {
                    return jo;
                }
            }
            return {};
        };

        auto keyFound = findKey(this->m_treeMiniscript);
        if (keyFound.isEmpty()) {
            keyFound = findKey(this->m_keypaths);
        }
        DBG_INFO << "checkDuplicateKey keyFound:" << keyFound;
        if (!keyFound.isEmpty()) {
            int keyStrCount = keyFound.value("keyStrCount").toInt();
            auto keyObj = keyFound.value("keyObj").toObject();
            if (keyStrCount > 1) {
                if (groupSandboxPtr()) {
                    emit groupSandboxPtr()->duplicateKeyError(keyObj, keyFound);
                } 
                emit this->duplicateKeyError(keyObj, keyFound);                
            }
        }
    });
}

bool MiniscriptWallet::timeLocked() const
{
    return m_timeLocked;
}

void MiniscriptWallet::setTimeLocked(bool newTimeLocked)
{
    if (m_timeLocked == newTimeLocked)
        return;
    m_timeLocked = newTimeLocked;
    emit timeLockedChanged();
}

QVariantMap MiniscriptWallet::timelockInfo()
{
    m_timelockInfo.clear();
    if (auto list = utxoList()) {
        m_timelockInfo = list->timelockInfo();
        if(!m_timelockInfo.contains("valueNeedVisibleWarning")){
            m_timelockInfo["valueNeedVisibleWarning"] = false;
        }
        if(!m_timelockInfo.contains("valueRemainingNumeric")){
            m_timelockInfo["valueRemainingNumeric"] = 0;
        }
        if(!m_timelockInfo.contains("valueRemainingString")){
            m_timelockInfo["valueRemainingString"] = "";
        }
    }
    return m_timelockInfo;
}
