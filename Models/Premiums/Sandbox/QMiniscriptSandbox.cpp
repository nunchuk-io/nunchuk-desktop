#include "QMiniscriptSandbox.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "Models/QWarningMessage.h"
#include "ifaces/nunchuckiface.h"
#include "Models/AppModel.h"
#include "Chats/ClientController.h"
#include "Premiums/QSharedWallets.h"
#include "Signers/QSignerManagement.h"
#include <descriptor.h>

QString incrementZeroIndex(const QString &path) {
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

int getIndexAt(const QString &path, int pos)
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

QMiniscriptSandbox::QMiniscriptSandbox()
    : QNormalSandbox()
{
    
}

QMiniscriptSandbox::QMiniscriptSandbox(nunchuk::GroupSandbox sandbox)
    : QNormalSandbox(sandbox)
{
    
}

QMiniscriptSandbox::~QMiniscriptSandbox()
{

}

void QMiniscriptSandbox::setKeyName(const QString &keyName)
{
    m_keyName = keyName;
    m_sandbox = bridge::SetSlotOccupied(m_sandbox, keyName, true);
}

int QMiniscriptSandbox::walletType() {
    int cdata = (int)m_sandbox.get_wallet_type();
    std::set<int> valid_numbers = {(int)nunchuk::WalletType::SINGLE_SIG, (int)nunchuk::WalletType::MULTI_SIG, (int)nunchuk::WalletType::ESCROW,
                                   (int)nunchuk::WalletType::MINISCRIPT};
    if (valid_numbers.find(cdata) != valid_numbers.end()) {
        m_walletType = cdata;
    }
    return m_walletType;
}

bool QMiniscriptSandbox::enoughSigner() {
    if (walletType() == (int)nunchuk::WalletType::MINISCRIPT) {
        if (auto gw = AppModel::instance()->newWalletInfoPtr()) {
            return gw->enoughSigners();
        }
        return false;
    } else {
        return QNormalSandbox::enoughSigner();
    }
}

void QMiniscriptSandbox::clearOccupied()
{
    if (walletType() == (int)nunchuk::WalletType::MINISCRIPT) {
        std::vector<std::string> slotsToClear;
        for (const auto& pair : m_sandbox.get_named_occupied()) {
            if (pair.second.second == QSharedWallets::instance()->uid().toStdString()) {
                slotsToClear.push_back(pair.first);
            }
        }
        for (const std::string &keyName : slotsToClear) {
            m_sandbox = bridge::SetSlotOccupied(m_sandbox, QString::fromStdString(keyName), false);
        }
    } else {
        QNormalSandbox::clearOccupied();
    }
}

void QMiniscriptSandbox::CreateAGroup(const QString &name, int m, int n, int addType) {
    QNormalSandbox::CreateAGroup(name, m, n, addType);
};
void QMiniscriptSandbox::UpdateGroup(const QString &name, int m, int n, int addType) {
    QNormalSandbox::UpdateGroup(name, m, n, addType);
};

void QMiniscriptSandbox::CreateAGroup(const QString& name, const QString& script_tmpl, int addType) {
    nunchuk::AddressType addressType = static_cast<nunchuk::AddressType>(addType);
    QWarningMessage msg;
    auto sandbox = bridge::CreateGroup(name, script_tmpl, addressType, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setSandbox(sandbox);
        QSharedWallets::instance()->GetAllGroups();
        GetNumberForAGroup();
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void QMiniscriptSandbox::UpdateGroup(const QString &name, const QString& script_tmpl, int addType) {
    nunchuk::AddressType addressType = static_cast<nunchuk::AddressType>(addType);
    QWarningMessage msg;
    auto sandbox = bridge::UpdateGroup(groupId(), name, script_tmpl, addressType, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setSandbox(sandbox);
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void QMiniscriptSandbox::setSandbox(const nunchuk::GroupSandbox& sandbox) {
    m_sandbox = sandbox;
    if (walletType() == (int)nunchuk::WalletType::MINISCRIPT) {
        QString uid = QSharedWallets::instance()->uid();
        auto find = [&](const std::string key_name) -> QSingleSignerPtr {
            auto signer = sandbox.get_named_signers().at(key_name);
            if (signer.get_master_fingerprint().empty()) {
                QSingleSignerPtr signerPtr = QSingleSignerPtr(new QSingleSigner(signer));
                if (signer.get_name() == "ADDED") {
                    // Signer at this key_name has been added, but its info is encrypted
                    // Display gray out `placeholder`, don't allow add signer here
                    return signerPtr;
                } else if (sandbox.get_named_occupied().contains(key_name)) {
                    // Signer at this key_name has not been added yet
                    std::pair<time_t, std::string> ts_uid = sandbox.get_named_occupied().at(key_name);
                    time_t timeout = 5 * 60; // 5 minutes
                    if (ts_uid.first + timeout > std::time(0) && ts_uid.second != uid.toStdString()) {
                        // Slot occupied by other device is not timeout
                        // Display `Occupied`, don't allow add signer here
                        signerPtr->setIsOccupied(true);
                    } else {
                        // Allow add signer here
                        signerPtr->setIsOccupied(false);
                    }
                    return signerPtr;
                }
                return QSingleSignerPtr(nullptr);
            } else {
                QWarningMessage msg;
                nunchuk::SingleSigner localSigner = bridge::nunchukGetOriginSingleSigner(signer, msg);
                return QSingleSignerPtr(new QSingleSigner(localSigner));
            }
            return QSingleSignerPtr(nullptr);
        };    
        

        if (auto gw = AppModel::instance()->newWalletInfoPtr()) {
            if (isReplace()) {
                // if (auto list = AppModel::instance()->walletListPtr()) {
                //     auto currentWallet = list->getWalletById(QString::fromStdString(m_sandbox.get_replace_wallet_id()));
                //     if (currentWallet) {
                //         auto wallet = currentWallet->nunchukWallet();
                //         QVariantList groupKeys;
                //         for (int i = 0; i < sandbox.get_n(); i++) {
                //             auto newSigner = sandbox.get_signers().at(i);
                //             auto oldSigner = wallet.get_signers().at(i);
                //             QSingleSignerPtr signerPtr = find(i);
                //             if (newSigner.get_master_fingerprint() == oldSigner.get_master_fingerprint()) {
                //                 signerPtr->setIsReplaced(false);
                //             } else if (newSigner.get_master_fingerprint().empty()) {
                //                 signerPtr->setIsReplaced(false);
                //                 signerPtr->setMasterFingerPrint(QString::fromStdString(oldSigner.get_master_fingerprint()));
                //             } else {
                //                 signerPtr->setIsReplaced(true);
                //                 auto oldPtr = QSingleSignerPtr(new QSingleSigner(oldSigner));
                //                 signerPtr->setKeyReplaced(oldPtr);
                //             }
                //             groupKeys.insert(i, SingleSignerListModel::useQml(signerPtr));
                //         }
                //         m_groupKeys = groupKeys;
                //     }
                // }
            } else {
                auto maps = sandbox.get_named_signers();
                for (std::map<std::string, nunchuk::SingleSigner>::iterator it = maps.begin(); it != maps.end(); it++ ){
                    std::string keyName = it->first;
                    QSingleSignerPtr signerPtr = find(keyName);
                    if(!signerPtr.isNull()) {
                        gw->updateSignersMiniscript(QString::fromStdString(keyName), signerPtr, false);
                    }
                }
            }
            gw->configureWallet(QString::fromStdString(sandbox.get_miniscript_template()));
        }
        emit groupSandboxChanged();
    } else {
        QNormalSandbox::setSandbox(sandbox);
    }
}

bool QMiniscriptSandbox::AddSignerToGroup(const nunchuk::SingleSigner &signer)
{
    if (walletType() == (int)nunchuk::WalletType::MINISCRIPT) {
        QWarningMessage msg;
        auto sandbox_value = bridge::AddSignerToGroup(QString::fromStdString(m_sandbox.get_id()), signer, m_keyName, msg);
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->updateSignersMiniscript(m_keyName, QSingleSignerPtr(new QSingleSigner(signer)));
            w->setNeedCheckDuplicate(true);            
            if(signer.get_type() == nunchuk::SignerType::SOFTWARE) {
                auto listKey = w->keySameList(m_keyName);
                for (const auto &k : listKey) {
                    if (k != m_keyName) {
                        auto path = QString::fromStdString(signer.get_derivation_path());
                        auto master_id = QString::fromStdString(signer.get_master_fingerprint());
                        // Increment the last index in the BIP32 path
                        auto new_path = incrementZeroIndex(path);
                        QWarningMessage msgTmp;
                        DBG_INFO << "Incremented path: " << k << new_path << QString::fromStdString(signer.get_master_fingerprint());
                        nunchuk::SingleSigner signer_tmp = bridge::GetSignerFromMasterSigner(master_id, new_path, msgTmp);
                        sandbox_value = bridge::AddSignerToGroup(QString::fromStdString(m_sandbox.get_id()), signer_tmp, k, msgTmp);
                    }
                }
            }
        }
        if((int)EWARNING::WarningType::NONE_MSG == msg.type() && sandbox_value.get_id().empty() == false){
            setSandbox(sandbox_value);
            return true;
        } else {
            setSandbox(m_sandbox);
            return (nunchuk::GroupException::SIGNER_EXISTS == msg.code());
        }
    } else {
        return QNormalSandbox::AddSignerToGroup(signer);
    }
}

void QMiniscriptSandbox::removeKeyName(const QString &group, const QString &keyName) {
    DBG_INFO << "group: " << group << "keyName: " << keyName;
    auto removeKey = [this] (const QString &key) -> nunchuk::GroupSandbox{
        QWarningMessage msg;
        nunchuk::GroupSandbox sandbox = bridge::RemoveSignerFromGroup(groupId(), key, msg);
        if (walletType() == (int)nunchuk::WalletType::MINISCRIPT) {
            if (auto w = AppModel::instance()->newWalletInfoPtr()) {
                w->removeSignersMiniscript(key);
            }
        }
        return sandbox;
    };
    nunchuk::GroupSandbox sandbox = m_sandbox;
    if (auto w = AppModel::instance()->newWalletInfoPtr()) {
        auto listKey = w->keySameList(keyName);
        for (const auto &k : listKey) {
            sandbox = removeKey(k);
        }
    }
    
    setSandbox(sandbox);
}

bool QMiniscriptSandbox::editBIP32Path(int index, const QString &master_id, const QString& path) {
    return QNormalSandbox::editBIP32Path(index, master_id, path);
}

bool QMiniscriptSandbox::editBIP32Path(const QString &keyName, const QString &master_id, const QString &path)
{
    DBG_INFO << "keyName: " << keyName << "master_id: " << master_id << "path: " << path;
    if (!qUtils::QIsValidDerivationPath(path)) {
        emit editBIP32PathSuccess(-1); // Invalid format
        return false;
    }
    QPointer<QNormalSandbox> safeThis(this);
    struct DataStruct
    {
        nunchuk::GroupSandbox sandbox {""};
        int errorType {0};
    };
    if (auto w = AppModel::instance()->newWalletInfoPtr()) {
        w->setKeySelected(keyName);
    }
    runInConcurrent([safeThis, keyName, master_id, path]() -> DataStruct {
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QWarningMessage msg;
        nunchuk::SingleSigner signer = bridge::GetSignerFromMasterSigner(master_id, path, msg);
        DataStruct data;
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            msg.resetWarningMessage();
            auto sandbox = bridge::AddSignerToGroup(QString::fromStdString(ptrLamda->sandbox().get_id()), signer, keyName, msg);
            if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                QString msg = "BIP32 path updated.";
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
                data.errorType = 1;
                data.sandbox = sandbox;
                return data;
            } else {
                data.errorType = -3; // Signer exists
            }
        } else {
            data.errorType = -2;
        }
        return data;
    },[safeThis](DataStruct data) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        if (data.errorType == 1) {
            ptrLamda->setSandbox(data.sandbox);
        }
        emit ptrLamda->editBIP32PathSuccess(data.errorType); // -2 Not Found Key
    });
    return false;
}

void QMiniscriptSandbox::requestAddOrRepaceKey(const QVariant &msg) {
    if (walletType() == (int)nunchuk::WalletType::MINISCRIPT) {
        QSignerManagement::instance()->clearExecute();
        QMap<QString, QVariant> maps = msg.toMap();
        QString keyName = maps["keyName"].toString();
        QString fingerPrint = maps["fingerPrint"].toString();
        setKeyName(keyName);
        setFingerprintRecovery(fingerPrint);
        registerSigners();
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->setKeySelected(keyName);
            w->MixMasterSignerAndSingleSignerAll();
            bool existSigner = w->signerExistList().size() > 0;
            if (existSigner) {
                if (auto sandbox = w->groupSandboxPtr()) {
                    sandbox->setScreenFlow("sandbox-add-existing-key");
                }
            } else {
                QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_NEW_SIGNER_REQUEST, msg);
            }
        }
    }
    else {
        QNormalSandbox::requestAddOrRepaceKey(msg);
    }
}

int QMiniscriptSandbox::getIndexColor(const QVariant & keyObj) {
    QStringList list;
    for(const auto& signer : sandbox().get_signers()) {
        list.append(QString::fromStdString(signer.get_master_fingerprint()));
    }
    auto maps = keyObj.toMap();
    auto fingerPrint = maps.value("singleSigner_masterFingerPrint").toString();
    return list.indexOf(fingerPrint);
}
