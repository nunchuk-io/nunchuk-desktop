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
#include <descriptor.h>

MiniscriptTransaction::MiniscriptTransaction() : BaseTransaction() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

MiniscriptTransaction::MiniscriptTransaction(const nunchuk::Transaction &tx) : BaseTransaction(tx) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

bool MiniscriptTransaction::isScriptPath() const
{
    auto wallet = AppModel::instance()->walletList()->getWalletById(walletId());
    if (!wallet) {
        DBG_ERROR << "Wallet info is not available";
        return false;
    }
    auto w = wallet->nunchukWallet();
    if (w.get_wallet_type() == nunchuk::WalletType::MINISCRIPT) {
        if (w.get_address_type() == nunchuk::AddressType::NATIVE_SEGWIT) {
            return true;
        } else {
            return AppSetting::instance()->getTransactionUsescriptpath(txid());
        }
    } else {
        return false;
    }
}

QJsonArray MiniscriptTransaction::walletTreeMiniscriptJs() const
{
    auto wallet = AppModel::instance()->walletList()->getWalletById(walletId());
    if (!wallet) {
        DBG_ERROR << "Wallet info is not available";
        return {};
    }
    return wallet->treeMiniscriptJs();
}

QVariantList MiniscriptTransaction::treeMiniscript() const {
    return m_treeMiniscript.toVariantList();
}

void MiniscriptTransaction::setTreeMiniscript(const QJsonArray &tree) {
    m_treeMiniscript = tree;    
    emit treeMiniscriptChanged();
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
    createScriptPaths(data);   
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
    DBG_INFO << "pathNodes" << pathNodes;
    for (const auto& item : scriptTree) {
        scriptNodeJson.append(processNodeJson(item, pathNodes));
    }
    scriptPathObj["scriptTree"] = scriptNodeJson;
    scriptPathObj["signingPath"] = pathNodes.join(",");
    return scriptPathObj;
}

void MiniscriptTransaction::createScriptPaths(const std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> &signing_paths) {
    if (signing_paths.empty()) {
        DBG_ERROR << "No signing paths available";
        return;
    }
    m_selectedSigningPath = signing_paths.front().first; // Default to the first signing path
    QList<int> levels {};
    QJsonArray scriptNodeTemplate = walletTreeMiniscriptJs();
    // loop signing_paths
    QJsonArray scriptPaths;
    for (const auto& signing_path : signing_paths) {
        QJsonObject node = createScriptPath(scriptNodeTemplate, signing_path);
        scriptPaths.append(node);
    }
    setScriptPaths(scriptPaths.toVariantList());
}

int MiniscriptTransaction::getLevel(const QString &path) {
    if (path.isEmpty()) return -1;
    return path.length() / 2;
}

int MiniscriptTransaction::getMaxLevel(const QJsonArray &tree)  {
    int maxLevel = 0;
    for (const auto &js : tree) {
        QJsonObject jo = js.toObject();
        QString firstLine = jo.value("firstLine").toString();
        maxLevel = std::max(maxLevel, getLevel(firstLine));       
    }
    return maxLevel;
}

QString MiniscriptTransaction::getParentPath(const QString &path) {
    if (path.isEmpty() || path.length() < 3) return "";
    return path.left(path.length() - 2);
}

QStringList MiniscriptTransaction::findPaths(const QJsonArray &tree, int level)  {
    QStringList paths = {};
    for (const auto &js : tree) {
        QJsonObject jo = js.toObject();
        QString firstLine = jo.value("firstLine").toString();
        if (getLevel(firstLine) == level) {
            paths.append(firstLine);
        }        
    }
    return paths;
}

QStringList MiniscriptTransaction::findPathsOfParentChecked(const QSet<QString> &checkedList, const QString &parentPath) {
    QStringList paths = {};
    for (const auto &path : checkedList) {
        QString pPath = getParentPath(path);
        if (qUtils::strCompare(pPath, parentPath)) {
            paths.append(path);
        }
    }
    return paths;
}

QJsonObject MiniscriptTransaction::findSigningPathParent(const QJsonArray &tree, const QString &path) {
    QString parentPath = getParentPath(path);
    for (const auto &js : tree) {
        QJsonObject jo = js.toObject();
        QString firstLine = jo.value("firstLine").toString();
        if (qUtils::strCompare(firstLine, parentPath)) {
            return jo;
        }       
    }
    return {};
}

QJsonArray MiniscriptTransaction::refreshTreeChecked(const QJsonArray &tree, const QSet<QString> &checkeds) {
    QJsonArray updatedTree = {};
    for (const auto &js : tree) {
        QJsonObject jo = js.toObject();
        QString firstLine = jo.value("firstLine").toString();
        bool hasCheckBox = jo.value("hasCheckBox").toBool();
        jo["checked"] = checkeds.contains(firstLine);
        updatedTree.append(jo);
    }
    return updatedTree;
}

QJsonArray MiniscriptTransaction::processCheckedBoxes(const QJsonArray &tree, const QJsonArray &newTreeTmp, int level, const QString& path, QSet<QString>& checkeds, QSet<QString>& allowAutos) {
    QJsonArray newTree = newTreeTmp;
    for (int i = 0; i < tree.size(); i++) {
        QJsonValue jsValue = tree.at(i);
        QJsonObject node = jsValue.toObject();
        QString firstLine = node.value("firstLine").toString();
        if (!qUtils::strCompare(firstLine, path)) {
            continue;
        }
        QJsonObject parentNode = findSigningPathParent(tree, firstLine);
        bool hasBoxParentChecked = parentNode.value("checked").toBool();
        QString parentPath = getParentPath(firstLine);

        if (hasBoxParentChecked) {
            node["hasCheckBox"] = true;
            bool isChecked = checkeds.contains(firstLine);
            node["checked"] = isChecked;
            node["enable"] = true;
            auto type = static_cast<nunchuk::ScriptNode::Type>(parentNode.value("type").toInt());
            switch (type) {
                case nunchuk::ScriptNode::Type::ANDOR: {
                    QStringList pathsCheckedAtLevel = findPathsOfParentChecked(checkeds, parentPath);
                    int mNode = 1;
                    if (pathsCheckedAtLevel.size() >= mNode && !isChecked) {
                        node["enable"] = false;
                    }
                } break;
                case nunchuk::ScriptNode::Type::AND: {
                    if (checkeds.contains(parentPath) && allowAutos.contains(parentPath)) {
                        checkeds.insert(firstLine);
                        allowAutos.insert(firstLine);
                    }
                    node["hasCheckBox"] = true;
                    node["checked"] = checkeds.contains(firstLine);
                    node["enable"] = false;
                } break;
                case nunchuk::ScriptNode::Type::OR_TAPROOT:
                case nunchuk::ScriptNode::Type::OR: {
                    QStringList pathsCheckedAtLevel = findPathsOfParentChecked(checkeds, parentPath);
                    int mNode = 1;
                    if (pathsCheckedAtLevel.size() >= mNode && !isChecked) {
                        node["enable"] = false;
                    }
                } break;
                case nunchuk::ScriptNode::Type::MULTI:
                case nunchuk::ScriptNode::Type::MUSIG: {
                    if (checkeds.contains(parentPath)) {
                        node["hasCheckBox"] = false;
                        node["checked"] = false;
                        node["enable"] = false;
                    }                    
                } break;
                case nunchuk::ScriptNode::Type::THRESH: {
                    QStringList pathsCheckedAtLevel = findPathsOfParentChecked(checkeds, parentPath);
                    int mNode = parentNode.value("m").toInt();
                    if (pathsCheckedAtLevel.size() >= mNode && !isChecked) {
                        node["enable"] = false;
                    }
                } break;
                default: {
                } break;
            }
            newTree.append(node);
        } else {
            if (level == 1) {
                int countFirst = findPaths(tree, 1).size();
                if (countFirst == 1) {
                    node["hasCheckBox"] = false;
                    node["checked"] = checkeds.contains(firstLine);
                } else {
                    node["hasCheckBox"] = true;
                    node["checked"] = checkeds.contains(firstLine);
                }
                newTree.append(node);
            } else {
                node["hasCheckBox"] = false;
                node["checked"] = false;
                node["enable"] = false;
                checkeds.remove(firstLine);
                allowAutos.remove(firstLine);
                newTree.append(node);
            }
        }
    }
    return newTree;
}

int MiniscriptTransaction::numberFromPath(const QString &path) {
    QString tmp = path;
    int level = getLevel(path);
    tmp.remove('.'); // Remove all dots
    bool ok;
    int num = tmp.toInt(&ok)*qPow(10, m_maxLevel)/qPow(10, level);
    if (ok) return num;
    return 0;
}

QJsonArray MiniscriptTransaction::processLevels(const QJsonArray &tree, QSet<QString>& checkeds, QSet<QString>& allowAutos) {
    QJsonArray newTree = {};
    for (int level = 1; level <= m_maxLevel; ++level) {
        QStringList pathsAtLevel = findPaths(tree, level);
        for (const QString &path : pathsAtLevel) {
            newTree = processCheckedBoxes(tree, newTree, level, path, checkeds, allowAutos);
        }
    }
    return newTree;
}

bool MiniscriptTransaction::compareNodes(const QJsonValue &a, const QJsonValue &b) {
    QJsonObject aObj = a.toObject();
    QJsonObject bObj = b.toObject();
    QString firstLineA = aObj.value("firstLine").toString();
    QString firstLineB = bObj.value("firstLine").toString();
    return numberFromPath(firstLineA) < numberFromPath(firstLineB);
}

QJsonArray MiniscriptTransaction::sortJsonArrayByFirstLine(const QJsonArray &arr) {
    QVector<QJsonValue> values;
    values.reserve(arr.size());
    for (auto v : arr)
        values.append(v);

    std::sort(values.begin(), values.end(),
        [this](const QJsonValue &a, const QJsonValue &b) {
            return this->compareNodes(a, b);
        });

    QJsonArray sorted;
    for (auto &v : values)
        sorted.append(v);
    return sorted;
}

void MiniscriptTransaction::firstCreateChooseScriptPath() {
    QJsonArray tmpTree = walletTreeMiniscriptJs();
    if (tmpTree.size() == 0) {
        return;
    }
    int countFirst = findPaths(tmpTree, 1).size();
    DBG_INFO << "Only one signing path available, auto-selecting it." << countFirst;
    if (countFirst == 1) {
        setCheckedStateForPath("1.", true);
    } else {
        createChooseScriptPath();
    }
}

void MiniscriptTransaction::createChooseScriptPath() {
    QJsonArray tmpTree = walletTreeMiniscriptJs();
    m_maxLevel = getMaxLevel(tmpTree);
    auto signingPathsChecked = m_signingPathsChecked;
    auto signingPathsAutoChecked = m_signingPathsAutoChecked;
begin:;
    tmpTree = refreshTreeChecked(tmpTree, signingPathsChecked);
    tmpTree = processLevels(tmpTree, signingPathsChecked, signingPathsAutoChecked);

    if (signingPathsChecked != m_signingPathsChecked) {
        m_signingPathsChecked = signingPathsChecked;
        m_signingPathsAutoChecked = signingPathsAutoChecked;
        goto begin;
    }
    tmpTree = sortJsonArrayByFirstLine(tmpTree);
    setTreeMiniscript(tmpTree);
}

bool MiniscriptTransaction::hasSigningPathChecked() const {
    return !m_signingPathsChecked.isEmpty();
}

void MiniscriptTransaction::setCheckedStateForPath(const QString &path, bool checked) {
    if (checked) {
        m_signingPathsChecked.insert(path);
        m_signingPathsAutoChecked.insert(path);
    } else {
        m_signingPathsChecked.remove(path);
        m_signingPathsAutoChecked.remove(path);
    }
    std::function<void(const QString&)> removeParentAutoCheck =
    [&]( const QString& path) {
        QString parentPath = getParentPath(path);
        if (!parentPath.isEmpty()) {
            m_signingPathsAutoChecked.remove(parentPath);
            removeParentAutoCheck(parentPath);
        }
    };
    removeParentAutoCheck(path);
    createChooseScriptPath();
}

bool MiniscriptTransaction::createTransactionOrSingingPolicy() {
    QJsonArray tmpTree = walletTreeMiniscriptJs();
    std::function<bool(const nunchuk::SigningPath, const nunchuk::SigningPath)> isChildOfParent = 
    [](const nunchuk::SigningPath userPath, const nunchuk::SigningPath libPath) {
        QStringList userPathStrs = qUtils::SigningPathToStringList(userPath);
        QStringList libPathStrs = qUtils::SigningPathToStringList(libPath);
        for (QString str: userPathStrs) {
            if (!libPathStrs.contains(str)) {
                return false;
            }
        }
        return true;
    };

    std::function<int(const QJsonArray&, const QString&)> getType = 
    [](const QJsonArray &tree, const QString& path) {
        for (const auto &js : tree) {
            QJsonObject jo = js.toObject();
            QString firstLine = jo.value("firstLine").toString();
            if (qUtils::strCompare(firstLine, path)) {
                return jo.value("type").toInt();
            }        
        }
        return -1;
    };

    std::function<QSet<QString>(const QJsonArray&, const QSet<QString>&)> removeParent = 
    [getType](const QJsonArray& tree, const QSet<QString>& checkes) {
        QSet<QString> tmpCheckeds = checkes;
        for (const auto &path : checkes) {
            int type = getType(tree, path);
            if (type == (int)nunchuk::ScriptNode::Type::AND || type == (int)nunchuk::ScriptNode::Type::OR || type == (int)nunchuk::ScriptNode::Type::ANDOR || type == (int)nunchuk::ScriptNode::Type::THRESH) {
                tmpCheckeds.remove(path);
            }
        }
        return tmpCheckeds;
    };

    auto tmpCheckeds = removeParent(tmpTree, m_signingPathsChecked);
    std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> userSelectedSigningPaths;
    for (const auto &libPath: signingPaths()) {
        const nunchuk::SigningPath userPath = qUtils::SigningPathFromStringList(tmpCheckeds.values());
        if (isChildOfParent(userPath, libPath.first)) {
            userSelectedSigningPaths.push_back(libPath);
        }
    }
    DBG_INFO << "userSelectedSigningPaths size" << userSelectedSigningPaths.size() << "libPath size" << signingPaths().size();
    if (userSelectedSigningPaths.size() == 1) {
        setSigningPathSelected(userSelectedSigningPaths.front().first, true);
        return true;
    } else {
        createScriptPaths(userSelectedSigningPaths);
        setScreenFlow("choose-signing-policy");
        return false;
    }    
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
    auto wallet = AppModel::instance()->walletList()->getWalletById(walletId());
    if (!wallet) {
        DBG_ERROR << "Wallet info is not available";
        return {};
    }
    DBG_INFO << "isScriptPath:" << isScriptPath();
    if (isScriptPath()) {
        return miniTreeForSigningScriptPath();
    }
    else {
        return miniTreeForSigningKeyPath();
    }
}

QVariantList MiniscriptTransaction::miniTreeForSigningScriptPath() {
    auto wallet = AppModel::instance()->walletList()->getWalletById(walletId());
    if (!wallet) {
        DBG_ERROR << "Wallet info is not available";
        return {};
    }
    QString tx_id = txid();
    QVariantList miniTreeForSigning;

    auto tx = nunchukTransaction();
    std::string miniscript = wallet->miniscript().toStdString();
    std::vector<std::string> keypaths;
    nunchuk::ScriptNode script_node = qUtils::GetScriptNode(miniscript, keypaths);
    QJsonArray scriptTree = wallet->treeMiniscriptJs();
    int64_t chain_tip = bridge::nunchukBlockHeight();
    std::vector<nunchuk::UnspentOutput> coins = bridge::nunchukGetOriginCoinsFromTxInputs(walletId(), tx.get_inputs());
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

QVariantList MiniscriptTransaction::miniTreeForSigningKeyPath() {
    // auto wallet = AppModel::instance()->walletInfo();
    auto wallet = AppModel::instance()->walletList()->getWalletById(walletId());
    if (!wallet) {
        DBG_ERROR << "Wallet info is not available";
        return {};
    }
    auto w = wallet->nunchukWallet();
    QString tx_id = txid();
    QVariantList miniTreeForSigning;
    QStringList keypathsList;
    std::vector<std::string> keypaths;
    for(int i = 0; i < w.get_m(); i++) {
        const nunchuk::SingleSigner &signer = w.get_signers()[i];
        std::string keypath = nunchuk::GetDescriptorForSigner(signer, nunchuk::DescriptorPath::EXTERNAL_ALL);
        keypaths.push_back(keypath);
    }
    QJsonArray keyPaths = wallet->keypathsJs();
    QJsonArray keyPathsTx = createTreeMiniscriptTransaction(keypaths);
    QJsonArray scriptNodeJson;
    for (const auto& node : keyPaths) {
        for (const auto& txNode : keyPathsTx) {
            auto mergeNode = processSignNodeJson(node, txNode).toObject();
            if (!mergeNode.isEmpty()) {
                scriptNodeJson.append(mergeNode);
                break;
            }
        }
    }
    miniTreeForSigning = scriptNodeJson.toVariantList();

    DBG_INFO << scriptNodeJson;
    
    return miniTreeForSigning;
}

QVariant MiniscriptTransaction::timelockedUntil() {
    loadTimelockedUntil();
    return m_timelockedUntil;
}

void MiniscriptTransaction::loadTimelockedUntil() {

    QString txid = this->txid();
    QJsonObject lockedObj = {};
    lockedObj["hasLocked"] = false;
    lockedObj["activeUntil"] = "";
    lockedObj["enabledBroadcastTxBtn"] = true;
    if (txid.isEmpty()) {
        DBG_ERROR << "Transaction ID is empty, cannot load timelocked until";
        m_timelockedUntil = (QJsonValue::fromVariant(lockedObj));
        return;
    }
    QString walletId = this->walletId();
    if (walletId.isEmpty()) {
        DBG_ERROR << "Wallet ID is empty, cannot load timelocked until";
        m_timelockedUntil = (QJsonValue::fromVariant(lockedObj));
        return;
    }
    auto locked = bridge::nunchukGetTimelockedUntil(walletId, txid);
    int64_t block_height = bridge::nunchukBlockHeight();   

    if (locked.first > 0) {
        DBG_INFO << "locked.first:" << locked.first
                 << ", locked.second:" << (int)locked.second
                 << ", block_height:" << block_height
                 << ", block_height:" << walletId
                 << ", txid:" << txid << this;

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
        if (qUtils::IsPreimageRevealed(tx.get_psbt(), hash)) {
            firstLineObj["hasUnlocked"] = true;
            firstLineObj["hasEnter"] = false;
        } else {
            firstLineObj["hasUnlocked"] = false;
            firstLineObj["hasEnter"] = true;
        }
        firstLineObj["hashData"] = qUtils::BytesToHex(hash);
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

        tx_status = tx_status == (int)nunchuk::TransactionStatus::PENDING_SIGNATURES ? (int)keyset.first : tx_status;
        nunchuk::KeyStatus          keystatus = keyset.second;

        int pending_nonce = 0;
        if (tx_status == (int)nunchuk::TransactionStatus::PENDING_NONCE) {
            for (std::map<std::string, bool>::iterator it = keystatus.begin(); it != keystatus.end(); it++){
                QString xfp = QString::fromStdString(it->first);
                bool    signedStatus = it->second;
                if (!signedStatus) {
                    pending_nonce++;
                }
            }
        }
        int pending_signature = 0;
        if (tx_status == (int)nunchuk::TransactionStatus::PENDING_SIGNATURES) {
            for (std::map<std::string, bool>::iterator it = keystatus.begin(); it != keystatus.end(); it++){
                QString xfp = QString::fromStdString(it->first);
                bool    signedStatus = it->second;
                if (!signedStatus) {
                    pending_signature++;
                }
            }
        }
        firstLineObj["txStatus"] = (int)tx_status;
        firstLineObj["pendingNonce"]     = pending_nonce;
        firstLineObj["pendingSignature"] = pending_signature;        
        firstLineObj["satisfiable"]      = selfSatisfiable; // From parent
        treeLines.append(firstLineObj);

        std::set<int> valid_numbers = {(int)nunchuk::TransactionStatus::CONFIRMED, (int)nunchuk::TransactionStatus::READY_TO_BROADCAST, (int)nunchuk::TransactionStatus::PENDING_CONFIRMATION};
        bool allSigned = valid_numbers.find((int)tx_status) != valid_numbers.end();

        for (int i = 0; i < (int)node.get_keys().size(); i++) {
            QJsonObject keyObj;
            QString firstLineKey = QString("%1%2.").arg(firstLine).arg(i + 1);
            keyObj["firstLine"] = firstLineKey;
            keyObj["satisfiable"] = selfSatisfiable; // From parent
            std::string xfp = qUtils::ParseSignerString( QString::fromStdString(node.get_keys()[i]), msg ).get_master_fingerprint();

            bool keyset_signed = !tx.get_raw().empty() ? signers[xfp] : allSigned ? true : keystatus[xfp];
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

QJsonArray MiniscriptTransaction::createTreeMiniscriptTransaction(const std::vector<std::string> &keypaths) {
    auto tx = nunchukTransaction();
    nunchuk::TransactionStatus tx_status = tx.get_status();

    std::map<std::string, bool> signers;
    if (!tx.get_raw().empty()) {
        std::vector<nunchuk::SingleSigner> signers_signedInfo = bridge::nunchukGetTransactionSigners(walletId(), txid());
        for (const auto &signer : signers_signedInfo) {
            signers[signer.get_master_fingerprint()] = true;
        }
    } else {
        signers = tx.get_signers();
    }
    

    QWarningMessage msg;
    QJsonArray _keypaths = {};
    QString firstLine = "";
    QJsonObject firstLineObj;
    firstLineObj["firstLine"] = QString("1.");
    firstLineObj["satisfiable"] = true;

    std::function<nunchuk::KeysetStatus(const nunchuk::Transaction &, int)> keysetFunc = [&](const nunchuk::Transaction &tx,
                                                                                             int index) -> nunchuk::KeysetStatus {
        std::vector<nunchuk::KeysetStatus> keysets = tx.get_keyset_status();
        if (keysets.size() == 0) {
            return {};
        }
        nunchuk::KeysetStatus keyset;
        if (index == (keysets.size() - 1)) {
            keyset = keysets.back();
        } else {
            keyset = keysets[index];
        }
        nunchuk::KeyStatus keystatus = keyset.second;
        for (std::map<std::string, bool>::iterator it = keystatus.begin(); it != keystatus.end(); it++) {
            QString xfp = QString::fromStdString(it->first);
            bool signedStatus = it->second;
            if (!signedStatus) {
                return keyset;
            }
        }
        return keysetFunc(tx, index + 1);
    };

    nunchuk::KeysetStatus keyset = keysetFunc(tx, 0);
    tx_status = tx_status == nunchuk::TransactionStatus::PENDING_SIGNATURES ? keyset.first : tx_status;
    nunchuk::KeyStatus keystatus = keyset.second;
    
    int pending_nonce = 0;
    if (tx_status == nunchuk::TransactionStatus::PENDING_NONCE) {
        for (std::map<std::string, bool>::iterator it = keystatus.begin(); it != keystatus.end(); it++) {
            QString xfp = QString::fromStdString(it->first);
            bool signedStatus = it->second;
            if (!signedStatus) {
                pending_nonce++;
            }
        }
    }
    int pending_signature = 0;
    if (tx_status == nunchuk::TransactionStatus::PENDING_SIGNATURES) {
        for (std::map<std::string, bool>::iterator it = keystatus.begin(); it != keystatus.end(); it++) {
            QString xfp = QString::fromStdString(it->first);
            bool signedStatus = it->second;
            if (!signedStatus) {
                pending_signature++;
            }
        }
    }
    firstLineObj["txStatus"] = (int)tx_status;
    firstLineObj["pendingNonce"] = pending_nonce;
    firstLineObj["pendingSignature"] = pending_signature;
    _keypaths.append(firstLineObj);
    std::set<int> valid_numbers = {(int)nunchuk::TransactionStatus::CONFIRMED, (int)nunchuk::TransactionStatus::READY_TO_BROADCAST, (int)nunchuk::TransactionStatus::PENDING_CONFIRMATION};
    auto signAlled = valid_numbers.find((int)tx_status) != valid_numbers.end();
    for (int i = 0; i < keypaths.size(); i++) {
        std::string key = keypaths[i];
        QJsonObject keyLineObj = firstLineObj;
        keyLineObj["firstLine"] = QString("1.%1.").arg(i + 1);
        std::string xfp = qUtils::ParseSignerString(QString::fromStdString(key), msg).get_master_fingerprint();
        bool signedStatus = (tx_status == nunchuk::TransactionStatus::PENDING_NONCE || tx_status == nunchuk::TransactionStatus::PENDING_SIGNATURES) ? keystatus[xfp] : signAlled;
        keyLineObj = processKeyNodeJson(keyLineObj, QString::fromStdString(xfp), (int)tx_status, signedStatus);
        _keypaths.append(keyLineObj);
    }
    return _keypaths;
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
        if (!txJson().isEmpty() || isDummyTx()) {
            line["alreadySigned"] = signer->signerSigned();
        }
        if(signer->signerType() == (int)ENUNCHUCK::SignerType::HARDWARE) {
            line["hasSignOrScan"] = true; // always show sign button or scan button
            line["signerReadyToSign"] = AppModel::instance()->deviceList()->contains(QString::fromStdString(xfp.toStdString()));
        } else if (signer->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE) {
            line["hasSignOrScan"] = true; // always show sign button or scan button
            line["signerReadyToSign"] = true;
        } else if (signer->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP) {
            line["hasSignOrScan"] = true; // only sign button, no scan button
            line["signerReadyToSign"] = true;
        } else {
            line["hasSignOrScan"] = false; // no sign button, no scan button
            line["signerReadyToSign"] = false;
        }
        line["signerType"] = signer->signerType();
    } else {
        line["hasSignOrScan"] = false; // no sign button, no scan button
        line["signerReadyToSign"] = false;
    }
    return line;
}

void MiniscriptTransaction::clearSigningPaths() {
    setSigningPaths({});
    m_selectedSigningPath.clear();
    m_signingPathsChecked.clear();
    m_signingPathsAutoChecked.clear();    
    QString tx_id = txid();
    AppSetting::instance()->setTransactionSigningPath(tx_id, "");
    auto wallet = AppModel::instance()->walletList()->getWalletById(walletId());
    if (wallet) {
        wallet->convertToMiniscript(wallet->nunchukWallet());
    }
}

int MiniscriptTransaction::numberOfPolices() {
    return signingPaths().size();
}

bool MiniscriptTransaction::enterPreimageInput(const QString &hasData, const QString &preimage, int typeNode) {
    DBG_INFO << " hasData:" << hasData << ", preimage:" << preimage << ", typeNode:" << typeNode;
    if (preimage.isEmpty()) {
        DBG_ERROR << "Preimage input is empty";
        emit preimageInputAlert();
        return false;
    }
    QtConcurrent::run([=, this]() {
        std::vector<uint8_t> hashVec = qUtils::HexToBytes(hasData);
        std::vector<uint8_t> preimageVec = qUtils::HexToBytes(preimage);
        QWarningMessage msg;
        bool result = bridge::nunchukRevealPreimage(walletId(), txid(), hashVec, preimageVec, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type() && result){ 
            DBG_INFO << "Reveal preimage success " << result;
            AppModel::instance()->startGetTransactionHistory(walletId());
            emit nunchukTransactionChanged();
            AppModel::instance()->showToast(msg.code(), "Hashlock unlocked successfully", EWARNING::WarningType::SUCCESS_MSG);
        } else {
            AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
            emit preimageInputAlert();
        }
    });    
    return false;
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
