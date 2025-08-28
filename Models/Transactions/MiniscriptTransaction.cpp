#include "MiniscriptTransaction.h"
#include "AppSetting.h"
#include "QtGui/qclipboard.h"
#include "qUtils.h"
#include "QOutlog.h"
#include "bridgeifaces.h"
#include "Chats/matrixbrigde.h"
#include "AppModel.h"
#include <QQmlEngine>
#include <nunchukmatrix.h>
#include "Servers/Byzantine.h"
#include "Premiums/QGroupDashboard.h"
#include "ifaces/bridgeifaces.h"

MiniscriptTransaction::MiniscriptTransaction() : BaseTransaction() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

MiniscriptTransaction::MiniscriptTransaction(const nunchuk::Transaction &tx) : BaseTransaction(tx) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

bool MiniscriptTransaction::isScriptPath() const
{
    return AppSetting::instance()->getTransactionUsescriptpath(txid());
}

qint64 MiniscriptTransaction::get_lock_time() const
{
    return nunchukTransaction().get_lock_time();
}

void MiniscriptTransaction::set_lock_time(qint64 value)
{
    if (nunchukTransaction().get_lock_time() == value)
        return;
    auto tx = nunchukTransaction();
    tx.set_lock_time(value);
    setNunchukTransaction(tx);
    emit nunchukTransactionChanged();
}

std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount> > MiniscriptTransaction::signingPaths()
{
    return m_signing_paths;
}

void MiniscriptTransaction::setSigningPaths(const std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount> > &data) {
    m_signing_paths = data;
    createScriptPaths();
}

int MiniscriptTransaction::signingPathsCount() const {
    return m_signing_paths.size();
}

QJsonObject MiniscriptTransaction::createScriptPath(const QJsonArray &scriptTree, const std::pair<nunchuk::SigningPath, nunchuk::Amount> &signingPath) {
    QJsonObject scriptPathObj;
    QJsonArray scriptNodeJson;
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        scriptPathObj["estimatedFee"] = (double)signingPath.second;
    }
    else{
        scriptPathObj["estimatedFee"] = qUtils::QValueFromAmount(signingPath.second);
    }
    scriptPathObj["currencyEstimatedFee"] = qUtils::currencyLocale(signingPath.second);
    QStringList pathNodes = qUtils::SigningPathToStringList(signingPath.first);
    for (const auto& item : scriptTree) {
        scriptNodeJson.append(processNodeJson(item, pathNodes));
    }
    scriptPathObj["scriptTree"] = scriptNodeJson;
    scriptPathObj["signingPath"] = pathNodes.join(",");
    return scriptPathObj;
}

void MiniscriptTransaction::createScriptPaths() {
    auto walletInfo = AppModel::instance()->walletInfo();
    if (!walletInfo) {
        DBG_ERROR << "Wallet info is not available";
        return; 
    }
    std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> signing_paths = signingPaths();
    std::string miniscript = walletInfo->miniscript().toStdString();
    std::vector<std::string> keypaths;
    if (signing_paths.empty()) {
        DBG_ERROR << "No signing paths available";
        return;
    }
    m_selectedSigningPath = signing_paths.front().first; // Default to the first signing path
    nunchuk::ScriptNode script_node = qUtils::GetScriptNode(miniscript, keypaths);
    QList<int> levels {};
    QJsonArray scriptNodeTemplate = walletInfo->createTreeMiniscript(script_node, levels);
    // loop signing_paths
    QJsonArray scriptPaths;
    for (const auto& signing_path : signing_paths) {
        QJsonObject node = createScriptPath(scriptNodeTemplate, signing_path);
        scriptPaths.append(node);
    }
    setScriptPaths(scriptPaths.toVariantList());
}


QVariantList MiniscriptTransaction::scriptPaths()
{
    return m_scriptPaths;
}

void MiniscriptTransaction::setScriptPaths(const QVariantList &newScriptPaths)
{
    if (m_scriptPaths == newScriptPaths)
        return;
    m_scriptPaths = newScriptPaths;
    emit scriptPathsChanged();
}

nunchuk::SigningPath MiniscriptTransaction::signingPathSelected() {
    QStringList signing_paths;
    for (const auto &nodeVec : m_selectedSigningPath) {
        QString s;
        for (int i = 0; i < nodeVec.size(); ++i) {
            if (!s.isEmpty()) s += ".";
            s += QString::number(nodeVec[i]);
        }
        s += ".";
        signing_paths.append(s);
    }
    DBG_INFO << "signing_paths" << signing_paths;

    return m_selectedSigningPath;
}

void MiniscriptTransaction::setSigningPathSelected(const nunchuk::SigningPath path, bool cached) {
    m_selectedSigningPath = path;
    if(cached){
        QString tx_id = txid();
        AppSetting::instance()->setTransactionSigningPath(tx_id, qUtils::serializeSigningPath(m_selectedSigningPath));
    }
}

void MiniscriptTransaction::setSigningPathSelected(const QString &path) {
    std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> signing_paths = signingPaths();
    for (const auto& signing_path : signing_paths) {
        QStringList nodeStrs;
        for (const auto& node : signing_path.first) {
            QStringList parts;
            for (size_t x : node) {
                parts << QString::number(x);
            }
            nodeStrs << parts.join(".") + ".";
        }
        QString pathStr = nodeStrs.join(",");
        if (qUtils::strCompare(pathStr, path)) {
            setSigningPathSelected(signing_path.first);
            break; // Found the matching signing path
        }
    }
}

QVariantList MiniscriptTransaction::miniTreeForSigning() {
    QString tx_id = txid();
    QVariantList miniTreeForSigning;

#if 0 // TBD
    nunchuk::SigningPath path = qUtils::deserializeSigningPath(AppSetting::instance()->getTransactionSigningPath(tx_id));
    setSigningPathSelected(path); // Set selected signing path
    QStringList cachedPaths;
    for (const auto &nodeVec : path) {
        QString s;
        for (int i = 0; i < nodeVec.size(); ++i) {
            if (!s.isEmpty()) s += ".";
            s += QString::number(nodeVec[i]);
        }
        s += ".";
        cachedPaths.append(s);
    }

    DBG_INFO << "cachedPaths" << cachedPaths;
#endif
    auto tx = nunchukTransaction();
    std::map<std::string, bool> signers = tx.get_signers();
    std::string psbt = tx.get_psbt();
    std::string miniscript = AppModel::instance()->walletInfo()->miniscript().toStdString();
    std::vector<std::string> keypaths;
    nunchuk::ScriptNode script_node = qUtils::GetScriptNode(miniscript, keypaths);
    QList<int> levels {};
    QJsonArray scriptTree = AppModel::instance()->walletInfo()->createTreeMiniscript(script_node, levels);
    int64_t chain_tip = bridge::nunchukBlockHeight();
    std::vector<nunchuk::UnspentOutput> coins = bridge::nunchukGetOriginCoinsFromTxInputs(AppModel::instance()->walletInfo()->walletId(), tx.get_inputs());
    DBG_INFO << "coins" << coins.size();
    QJsonArray scriptTreeTx = createTreeMiniscriptTransaction(script_node, chain_tip, coins, nunchuk::CoinsGroup(), true);
    QJsonArray scriptNodeJson;
    for (const auto& node : scriptTree) {
        for (const auto& txNode : scriptTreeTx) {
            auto mergeNode = processSignNodeJson(node, txNode).toObject();
            if (!mergeNode.isEmpty()) {
                scriptNodeJson.append(mergeNode);
                break;
            }
        }
    }

    miniTreeForSigning = scriptNodeJson.toVariantList();
    for (int i = 0; i < miniTreeForSigning.size(); ++i) {
        QVariantMap node = miniTreeForSigning[i].toMap();
        QString fl = node.value("firstLine").toString();
        if (!fl.endsWith('.')) fl += '.';
#if 0 // TBD
        if(cachedPaths.count() > 0){
            bool enable = false;
            for (const QString &p : cachedPaths) {
                if (p.startsWith(fl) || fl.startsWith(p)) {
                    enable = true;
                    break;
                }
            }
            node["enable"] = enable;
        }
        else {
#endif
            node["enable"] = node["satisfiable"];
#if 0
        }
#endif
        miniTreeForSigning[i] = node;
    }
    return miniTreeForSigning;
}

QVariant MiniscriptTransaction::timelockedUntil() {
    loadTimelockedUntil();
    return m_timelockedUntil;
}

void MiniscriptTransaction::loadTimelockedUntil() {

    QString txid = this->txid();
    if (txid.isEmpty()) {
        DBG_ERROR << "Transaction ID is empty, cannot load timelocked until";
        return;
    }
    QString walletId = this->walletId();
    if (walletId.isEmpty()) {
        DBG_ERROR << "Wallet ID is empty, cannot load timelocked until";
        return;
    }

    DBG_INFO << walletId << ", txid:" << txid << this;

    auto locked = bridge::nunchukGetTimelockedUntil(walletId, txid);
    int64_t block_height = bridge::nunchukBlockHeight();
    QJsonObject lockedObj = {};
    lockedObj["hasLocked"] = false;
    lockedObj["activeUntil"] = "";
    lockedObj["enabledBroadcastTxBtn"] = true;

    // std::string miniscript = AppModel::instance()->walletInfo()->miniscript().toStdString();
    // std::vector<std::string> keypaths;
    // nunchuk::ScriptNode script_node = qUtils::GetScriptNode(miniscript, keypaths);
    // nunchuk::Timelock timelock = nunchuk::Timelock::FromK(false, script_node.get_k());
    // nunchuk::Timelock::Based lockType = timelock.based();
    DBG_INFO << "locked.first:" << locked.first << ", locked.second:" << (int)locked.second << ", block_height:" << block_height;

    if (locked.first > 0) {
        if (locked.second == nunchuk::Timelock::Based::HEIGHT_LOCK) {
            if (static_cast<int64_t>(locked.first) > block_height) {
                lockedObj["hasLocked"] = true;
                lockedObj["activeUntil"] = QString("block %1").arg((qint64)locked.first);
                lockedObj["enabledBroadcastTxBtn"] = false;
            }
        }
        else if (locked.second == nunchuk::Timelock::Based::TIME_LOCK) {
            uint64_t timestamp = static_cast<uint64_t>(locked.first);
            auto now = qUtils::GetCurrentTimeSecond();
            if (now < locked.first) {
                QDateTime dtUtc = QDateTime::fromSecsSinceEpoch(timestamp, Qt::UTC);
                QDateTime dtLocal = dtUtc.toLocalTime();
                lockedObj["hasLocked"] = true;
                lockedObj["activeUntil"] = dtLocal.toString("MM/dd/yyyy hh:mm");
                lockedObj["enabledBroadcastTxBtn"] = false;
            }
        }
    }
    m_timelockedUntil = (QJsonValue::fromVariant(lockedObj));
}

void MiniscriptTransaction::refreshScanDevices() {
    BaseTransaction::refreshScanDevices();
}

QJsonArray MiniscriptTransaction::createTreeMiniscriptTransaction(
    const nunchuk::ScriptNode &node,
    int64_t chain_tip,
    const std::vector<nunchuk::UnspentOutput> &coins,
    const nunchuk::CoinsGroup &coinsGroup,
    bool parentSatisfiable)
{
    int top_level = INT_MAX;
    auto tx = nunchukTransaction();
    int tx_status = (int)tx.get_status();

    std::map<std::string, bool> signers;
    if(!tx.get_raw().empty()){
        std::vector<nunchuk::SingleSigner> signers_signedInfo = bridge::nunchukGetTransactionSigners(walletId(), txid());
        for (const auto& signer : signers_signedInfo) {
            signers[signer.get_master_fingerprint()] = true;
        }
    }
    else {
        signers = tx.get_signers();
    }

    QWarningMessage msg;
    QJsonArray treeLines;
    QJsonObject firstLineObj;

    // --- Check satisfiable ---
    bool selfSatisfiable = parentSatisfiable && node.is_satisfiable(tx);

    // --- Common fields ---
    QString firstLine = qUtils::ScriptNodeIdToString(node.get_id());
    firstLineObj["firstLine"] = firstLine;
    firstLineObj["satisfiable"] = selfSatisfiable;

    std::vector<nunchuk::CoinsGroup> coinsGroupList {};
    // --- Handle different node types ---
    switch (node.get_type()) {
    case nunchuk::ScriptNode::Type::PK: {
        std::string xfp = qUtils::ParseSignerString(QString::fromStdString(node.get_keys()[0]),msg).get_master_fingerprint();
        firstLineObj = processKeyNodeJson(firstLineObj, QString::fromStdString(xfp), tx_status, signers[xfp]);
        treeLines.append(firstLineObj);
        break;
    }

    case nunchuk::ScriptNode::Type::MULTI: {
        nunchuk::Timelock timelock = nunchuk::Timelock::FromK(false, node.get_k());
        nunchuk::Timelock::Based lockType = timelock.based();
        firstLineObj["lockType"] = static_cast<int>(lockType);
        firstLineObj["timelockType"] = static_cast<int>(timelock.type());
        auto tmpCoinsGroup = generateCoinsGroup(coinsGroup);
        firstLineObj["coinsGroup"] = tmpCoinsGroup;
        treeLines.append(firstLineObj);
        for (int i = 0; i < node.get_keys().size(); i++) {
            QJsonObject keyObj;
            QString firstLineKey = QString("%1%2.").arg(firstLine).arg(i + 1);
            keyObj["firstLine"] = firstLineKey;
            keyObj["satisfiable"] = selfSatisfiable; // From parent
            std::string xfp = qUtils::ParseSignerString( QString::fromStdString(node.get_keys()[i]), msg ).get_master_fingerprint();
            keyObj = processKeyNodeJson(keyObj, QString::fromStdString(xfp), tx_status, signers[xfp]);
            treeLines.append(keyObj);
        }
        break;
    }

    case nunchuk::ScriptNode::Type::HASH160:
    case nunchuk::ScriptNode::Type::HASH256:
    case nunchuk::ScriptNode::Type::RIPEMD160:
    case nunchuk::ScriptNode::Type::SHA256: {
        std::vector<uint8_t> hash = node.get_data();
        firstLineObj["hasUnlocked"] = qUtils::IsPreimageRevealed(tx.get_psbt(), hash);
        QString hashString = QString::fromStdString(std::string(hash.begin(), hash.end()));
        firstLineObj["hashData"] = hashString;
        treeLines.append(firstLineObj);
        break;
    }

    case nunchuk::ScriptNode::Type::ANDOR:
#if 0
        if((!node.get_subs()[0].is_satisfiable(tx))){
            firstLineObj["satisfiable"] = false;
        }
#endif
    case nunchuk::ScriptNode::Type::OR:
    case nunchuk::ScriptNode::Type::THRESH:
    case nunchuk::ScriptNode::Type::OR_TAPROOT: {
        int level = node.get_id().size();
        if (level < top_level) {
            top_level = level;
        }
        if (level == top_level) {
            coinsGroupList = qUtils::GetCoinsGroupedBySubPolicies(node, coins, chain_tip);
        }
        treeLines.append(firstLineObj);
        break;
    }

    case nunchuk::ScriptNode::Type::MUSIG: {
        nunchuk::Timelock timelock = nunchuk::Timelock::FromK(false, node.get_k());
        nunchuk::Timelock::Based lockType = timelock.based();
        firstLineObj["lockType"] = static_cast<int>(lockType);
        firstLineObj["timelockType"] = static_cast<int>(timelock.type());
        auto tmpCoinsGroup = generateCoinsGroup(coinsGroup);
        firstLineObj["coinsGroup"] = tmpCoinsGroup;
        nunchuk::KeysetStatus keyset;
        try {
            if (tx.get_keyset_status().size() > 0) {
                keyset = node.get_keyset_status(tx);
            }
        }
        catch (const nunchuk::BaseException &ex) {
            DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        }
        catch (std::exception &e) {
            DBG_INFO << "THROW EXCEPTION " << e.what();
        }

        nunchuk::TransactionStatus  tx_status = keyset.first;
        nunchuk::KeyStatus          keystatus = keyset.second;

        int pending_nonce = 0;
        if (tx_status == nunchuk::TransactionStatus::PENDING_NONCE) {
            for (std::map<std::string, bool>::iterator it = keystatus.begin(); it != keystatus.end(); it++){
                QString xfp = QString::fromStdString(it->first);
                bool    signedStatus = it->second;
                if (!signedStatus) {
                    pending_nonce++;
                }
            }
        }
        int pending_signature = 0;
        if (tx_status == nunchuk::TransactionStatus::PENDING_SIGNATURES) {
            for (std::map<std::string, bool>::iterator it = keystatus.begin(); it != keystatus.end(); it++){
                QString xfp = QString::fromStdString(it->first);
                bool    signedStatus = it->second;
                if (!signedStatus) {
                    pending_signature++;
                }
            }
        }
        firstLineObj["pendingNonce"] = pending_nonce;
        firstLineObj["pendingSignature"] = pending_signature;        
        firstLineObj["satisfiable"]   = selfSatisfiable; // From parent
        treeLines.append(firstLineObj);
        std::set<int> valid_numbers = {(int)nunchuk::TransactionStatus::CONFIRMED, (int)nunchuk::TransactionStatus::READY_TO_BROADCAST, (int)nunchuk::TransactionStatus::PENDING_CONFIRMATION};
        bool allSigned = valid_numbers.find((int)tx_status) != valid_numbers.end();
        
        for (int i = 0; i < node.get_keys().size(); i++) {
            QJsonObject keyObj;
            QString firstLineKey = QString("%1%2.").arg(firstLine).arg(i + 1);
            keyObj["firstLine"] = firstLineKey;
            keyObj["satisfiable"] = selfSatisfiable; // From parent
            std::string xfp = qUtils::ParseSignerString( QString::fromStdString(node.get_keys()[i]), msg ).get_master_fingerprint();
            bool keyset_signed = allSigned ? true : keystatus[xfp];
            keyObj = processKeyNodeJson(keyObj, QString::fromStdString(xfp), (int)tx_status, keyset_signed);
            treeLines.append(keyObj);
        }

        break;
    }

    default:
        treeLines.append(firstLineObj);
        break;
    }
    // --- Recurse into sub-nodes ---
    for (int i = 0; i < node.get_subs().size(); i++) {
        auto group = coinsGroupList.empty() ? nunchuk::CoinsGroup() : coinsGroupList[i];
        QJsonArray tmpList = createTreeMiniscriptTransaction(node.get_subs()[i], chain_tip, coins, group, selfSatisfiable);
        for (int j = 0; j < tmpList.size(); j++) {
            treeLines.append(tmpList.at(j).toObject());
        }
    }

    return treeLines;
}

QJsonValue MiniscriptTransaction::processNodeJson(const QJsonValue& value, const QStringList& nodes) {
    QJsonObject line = value.toObject();
    if (!line.contains("firstLine")) {
        DBG_ERROR << "No firstLine in JSON object";
        return value;
    }
    QString firstLine = line["firstLine"].toString();
    line["enable"] = false; // Default to false
    if (firstLine.length() <= 2) {
        line["enable"] = true;
        return line;
    }
    for (const QString& node : nodes) {
        if (node.startsWith(firstLine)) {
            line["enable"] = true;
            return line;
        }
    }
    for (const QString& node : nodes) {
        if (firstLine.startsWith(node)) {
            line["enable"] = true;
            return line;
        }
    }
    return line;
}

QJsonValue MiniscriptTransaction::processSignNodeJson(const QJsonValue& walletJs, const QJsonValue& txJs) {
    QJsonObject walletLine = walletJs.toObject();
    QJsonObject txLine = txJs.toObject();
    if (!walletLine.contains("firstLine") || !txLine.contains("firstLine")) {
        DBG_ERROR << "No firstLine in JSON object";
        return {}; // No firstLine, return as is
    }
    QString firstLineWallet = walletLine["firstLine"].toString();
    QString firstLineTx = txLine["firstLine"].toString();
    if (qUtils::strCompare(firstLineWallet, firstLineTx)) {
        auto mergeNode = qUtils::JsonDeepMerge(walletLine, txLine);
        return mergeNode;
    }
    return {};
}

QJsonObject MiniscriptTransaction::processKeyNodeJson(const QJsonObject& value, QString xfp, int keyset_status, bool keyset_signed) {
    QJsonObject line = value;
    line["keysetStatus"]  = keyset_status;
    line["keysetSigned"]  = keyset_signed;
    line["alreadySigned"] = keyset_signed;

    auto list = allFinalSigners();
    if (list == nullptr) {
        DBG_ERROR << "Single signers list is null";
        return line;
    }
    auto signer = list->getSingleSignerByFingerPrint(QString::fromStdString(xfp.toStdString()));
    if (!signer.isNull()) {
        if(signer->signerType() == (int)ENUNCHUCK::SignerType::HARDWARE) {
            line["hasSignOrScan"] = true; // always show sign button or scan button
            line["signerReadyToSign"] = AppModel::instance()->deviceList()->contains(QString::fromStdString(xfp.toStdString()));
        } else if (signer->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE) {
            line["hasSignOrScan"] = true; // always show sign button or scan button
            line["signerReadyToSign"] = true;
        } else if (signer->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP) {
            line["hasSignOrScan"] = false; // no sign button, only scan button
            line["signerReadyToSign"] = false;
        } else {
            line["hasSignOrScan"] = false; // no sign button, no scan button
            line["signerReadyToSign"] = false;
        }
    } else {
        line["hasSignOrScan"] = false; // no sign button, no scan button
        line["signerReadyToSign"] = false;
    }
    return line;
}

int MiniscriptTransaction::numberOfPolices() {
    return signingPaths().size();
}

QJsonObject MiniscriptTransaction::generateCoinsGroup(const nunchuk::CoinsGroup &coinsGroup) {
    QVariantMap result;
    // QJsonObject groupObj;
    auto coins = coinsGroup.first;
    QUTXOListModelPtr ret = QUTXOListModelPtr(new QUTXOListModel(walletId()));
    for (nunchuk::UnspentOutput it : coins) {
        ret.data()->addUTXO(it);
    }
    ret.data()->selectAll(true);
    result["amountDisplay"] = ret->amountDisplay();
    result["signXCount"]    = ret->rowCount();

    auto timeRange = coinsGroup.second;
    QDateTime now = QDateTime::currentDateTimeUtc();  // hoặc local tùy use case

    if (timeRange.first != 0 && now < QDateTime::fromSecsSinceEpoch(timeRange.first)) {
        result["hasActiveAfter"] = true;
        result["activeAfter"]    = QDateTime::fromSecsSinceEpoch(timeRange.first).toString("MM/dd/yyyy hh:mm");
    } else {
        result["hasActiveAfter"] = false;
    }
    if (timeRange.second != 0 && now < QDateTime::fromSecsSinceEpoch(timeRange.second)) {
        result["hasActiveUntil"] = true;
        result["activeUntil"]    = QDateTime::fromSecsSinceEpoch(timeRange.second).toString("MM/dd/yyyy hh:mm");
    } else {
        result["hasActiveUntil"] = false;
    }
    return QJsonObject::fromVariantMap(result);
}
