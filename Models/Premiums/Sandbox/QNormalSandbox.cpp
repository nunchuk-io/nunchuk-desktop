#include "QNormalSandbox.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "Models/QWarningMessage.h"
#include "ifaces/nunchuckiface.h"
#include "Models/AppModel.h"
#include "Chats/ClientController.h"
#include "Premiums/QSharedWallets.h"
#include "Signers/QSignerManagement.h"


QNormalSandbox::QNormalSandbox()
    : m_sandbox("")
{
    initialize();
}

QNormalSandbox::QNormalSandbox(nunchuk::GroupSandbox sandbox)
    : m_sandbox(sandbox)
{
    setSandbox(sandbox);
    initialize();
}

QNormalSandbox::~QNormalSandbox()
{

}

void QNormalSandbox::initialize()
{
    connect(QEventProcessor::instance(), &QEventProcessor::stateChanged, this, &QNormalSandbox::slotClearOccupied);
    connect(&m_occupied, &QTimer::timeout, [this]() {
        DBG_INFO;
        GetGroup(groupId());
    });
    connect(this, &QNormalSandbox::finishSandboxWallet, this, &QNormalSandbox::slotFinishSandboxWallet);
    connect(this, &QNormalSandbox::screenFlowChanged, [this]() {
        if (screenFlow() == "setup-group-wallet") {
            m_occupied.start(30000);
        }
    });
}

QString QNormalSandbox::groupId() const
{
    return QString::fromStdString(m_sandbox.get_id());
}

int QNormalSandbox::groupM() const
{
    return m_sandbox.get_m();
}

void QNormalSandbox::setGroupM(int m)
{
    if (m != groupM()) {
        m_sandbox.set_m(m);
        emit groupSandboxChanged();
    }
}

int QNormalSandbox::groupN() const
{
    return m_sandbox.get_n();
}

void QNormalSandbox::setGroupN(int n)
{
    if (n != groupN()) {
        m_sandbox.set_n(n);
        emit groupSandboxChanged();
    }
}

QString QNormalSandbox::groupName() const
{
    QString name = QString::fromStdString(m_sandbox.get_name());
    return name.isEmpty() ? "Group wallet" : name;
}

void QNormalSandbox::setGroupName(const QString &name)
{
    if (name != groupName()) {
        m_sandbox.set_name(name.toStdString());
        emit groupSandboxChanged();
    }
}

int QNormalSandbox::addressType() const
{
    if (m_sandbox.get_address_type() >= nunchuk::AddressType::ANY && m_sandbox.get_address_type() <= nunchuk::AddressType::TAPROOT) {
        return static_cast<int>(m_sandbox.get_address_type());
    } else {
        return static_cast<int>(nunchuk::AddressType::NATIVE_SEGWIT);
    }
}

void QNormalSandbox::setAddressType(int addr)
{
    if (addr != addressType()) {
        m_sandbox.set_address_type(static_cast<nunchuk::AddressType>(addr));
        emit groupSandboxChanged();
    }
}

int QNormalSandbox::walletType() const
{
    if (m_sandbox.get_wallet_type() >= nunchuk::WalletType::SINGLE_SIG && m_sandbox.get_wallet_type() <= nunchuk::WalletType::MINISCRIPT) {
        return static_cast<int>(m_sandbox.get_wallet_type());
    } else {
        return static_cast<int>(nunchuk::WalletType::SINGLE_SIG);
    }
}

QString QNormalSandbox::url() const
{
    return QString::fromStdString(m_sandbox.get_url());
}

QVariantList QNormalSandbox::groupKeys() const
{
    return m_groupKeys;
}

int QNormalSandbox::userCount() const
{
    return m_userCount;
}

void QNormalSandbox::setUserCount(int number)
{
    if (number != m_userCount) {
        m_userCount = number;
        emit groupSandboxChanged();
    }
}

bool QNormalSandbox::enoughSigner()
{
    return groupId().isEmpty() ? HasOneKeyRecovery() : HasEnoughSigner();
}

bool QNormalSandbox::HasEnoughSigner()
{
    for (int i = 0; i < m_sandbox.get_signers().size(); i++) {
        auto key = m_sandbox.get_signers().at(i);
        if (key.get_name() == "") {
            return false;
        }
    }
    return true;
}

bool QNormalSandbox::HasOneKeyRecovery()
{
    return true;
}

bool QNormalSandbox::FinalizeGroup()
{
    DBG_INFO << groupId() << enoughSigner();
    if (!groupId().isEmpty() && enoughSigner()) {
        QWarningMessage msg;
        QSet<size_t> valuekeyset = (m_sandbox.get_address_type() == nunchuk::AddressType::TAPROOT && m_sandbox.get_wallet_template() == nunchuk::WalletTemplate::DEFAULT) ? ValueKeyset() : QSet<size_t>();
        for (auto &signer : m_sandbox.get_signers()) {
            DBG_INFO << QString::fromStdString(signer.get_name()) << QString::fromStdString(signer.get_master_fingerprint()) << signer.get_derivation_path();
        }

        DBG_INFO << "valuekeyset size: " << valuekeyset.size() << valuekeyset << (int)m_sandbox.get_address_type() << (int)m_sandbox.get_wallet_template();

        auto sandbox = bridge::FinalizeGroup(groupId(), valuekeyset, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            m_isCreateWallet = true;
            setSandbox(sandbox);
            if (auto nw = AppModel::instance()->newWalletInfoPtr()) {
                nw->setWalletId(QString::fromStdString(sandbox.get_wallet_id()));
            }
            QSharedWallets::instance()->GetAllGroups();
            finishSandboxWallet(QString::fromStdString(sandbox.get_wallet_id()), true);
            return true;
        }
        else {
            if(msg.code() == (int)nunchuk::GroupException::GROUP_NOT_FOUND || msg.code() == (int)nunchuk::GroupException::SANDBOX_FINALIZED){
                // FIXME1
                // Show popup
                // Title: Unable to access link
                // Content: The wallet link has expired or wallet has already been created
            }
            else {
                AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
            }
        }
    }
    return false;
}

bool QNormalSandbox::isReplace() const
{
    return !m_sandbox.get_replace_wallet_id().empty() && !m_sandbox.get_url().empty();
}

bool QNormalSandbox::isRecovery() const
{
    return m_sandbox.get_replace_wallet_id().empty() && m_sandbox.get_url().empty();
}

bool QNormalSandbox::isCreate() const
{
    return m_sandbox.get_replace_wallet_id().empty() && !m_sandbox.get_url().empty();
}

void QNormalSandbox::CreateAGroup(const QString& name, int m, int n, int addType)
{
    nunchuk::AddressType addressType = static_cast<nunchuk::AddressType>(addType);
    if (QSharedWallets::instance()->config().get_max_keys(addressType) < n) {
        // UI show error: n is bigger than key limit for addressType (check Figma for error message)
    }
    QWarningMessage msg;
    auto sandbox = bridge::CreateGroup(name, m, n, addressType, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setSandbox(sandbox);
        QSharedWallets::instance()->GetAllGroups();
        GetNumberForAGroup();
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void QNormalSandbox::UpdateGroup(const QString &name, int m, int n, int addType)
{
    nunchuk::AddressType addressType = static_cast<nunchuk::AddressType>(addType);
    QWarningMessage msg;
    auto sandbox = bridge::UpdateGroup(groupId(), name, m, n, addressType, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setSandbox(sandbox);
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void QNormalSandbox::setSandbox(const nunchuk::GroupSandbox &sandbox)
{
    QString uid = QSharedWallets::instance()->uid();
    auto find = [&](const int index) -> QSingleSignerPtr {
        if (index < sandbox.get_signers().size()) {
            auto signer = sandbox.get_signers().at(index);
            if (signer.get_master_fingerprint().empty()) {
                QSingleSignerPtr signerPtr = QSingleSignerPtr(new QSingleSigner(signer));
                if (signer.get_name() == "ADDED") {
                    // Signer at this index has been added, but its info is encrypted
                    // Display gray out `placeholder`, don't allow add signer here
                    return signerPtr;
                } else if (sandbox.get_occupied().contains(index)) {
                    // Signer at this index has not been added yet
                    std::pair<time_t, std::string> ts_uid = sandbox.get_occupied().at(index);
                    time_t timeout = 5 * 60; // 5 minutes
                    if (ts_uid.first + timeout > std::time(0) && ts_uid.second != uid.toStdString()) {
                        // Slot occupied by other device is not timeout
                        // Display `Occupied`, don't allow add signer here
                        signerPtr->setIsOccupied(true);
                    } else {
                        // Allow add signer here
                        signerPtr->setIsOccupied(false);
                    }
                }
                return signerPtr;
            } else {
                QWarningMessage msg;
                nunchuk::SingleSigner localSigner = bridge::nunchukGetOriginSingleSigner(signer, msg);
                return QSingleSignerPtr(new QSingleSigner(localSigner));
            }
        }
        return {};
    };
    if (isReplace()) {
        if(auto list = AppModel::instance()->walletListPtr()){
            auto currentWallet = list->getWalletById(QString::fromStdString(m_sandbox.get_replace_wallet_id()));
            if (currentWallet){
                if (currentWallet->walletM() != groupM() || currentWallet->walletN() != groupN()) {
                    return;
                }
                auto wallet = currentWallet->nunchukWallet();
                QVariantList groupKeys;
                for(int i = 0; i < sandbox.get_n(); i++) {
                    auto newSigner = sandbox.get_signers().at(i);
                    auto oldSigner = wallet.get_signers().at(i);
                    QSingleSignerPtr signerPtr = find(i);
                    if (newSigner.get_master_fingerprint() == oldSigner.get_master_fingerprint()) {
                        signerPtr->setIsReplaced(false);
                    }
                    else if (newSigner.get_master_fingerprint().empty()) {
                        signerPtr->setIsReplaced(false);
                        signerPtr->setMasterFingerPrint(QString::fromStdString(oldSigner.get_master_fingerprint()));
                    }
                    else {
                        signerPtr->setIsReplaced(true);
                        auto oldPtr = QSingleSignerPtr(new QSingleSigner(oldSigner));
                        signerPtr->setKeyReplaced(oldPtr);
                    }
                    groupKeys.insert(i, SingleSignerListModel::useQml(signerPtr));
                }
                m_groupKeys = groupKeys;
            }
        }
    } else {
        QVariantList groupKeys;
        for(int i = 0; i < sandbox.get_n(); i++) {
            QSingleSignerPtr signerPtr = find(i);
            groupKeys.insert(i, SingleSignerListModel::useQml(signerPtr));
        }
        m_groupKeys = groupKeys;
    }
    emit groupSandboxChanged();

}

void QNormalSandbox::setCurrentSandbox(const nunchuk::GroupSandbox &sandbox)
{
    m_sandbox = sandbox;
    DBG_INFO << sandbox.is_finalized() << m_isCreateWallet << "wallet_id: " << sandbox.get_wallet_id();
    if (sandbox.is_finalized() && !m_isCreateWallet) {
        emit finishSandboxWallet(QString::fromStdString(sandbox.get_wallet_id()), true);
    }
    setSandbox(sandbox);
    BackToSetupGroupWallet();
    CreateSignerListReviewWallet();
}

bool QNormalSandbox::JoinGroup(const QString &url)
{
    QWarningMessage msg;
    QPair<QString, QString> pair = bridge::ParseGroupUrl(url, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QString first = pair.first;
        msg.resetWarningMessage();
        nunchuk::GroupSandbox sanbox_value = bridge::JoinGroup(first, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            DBG_INFO << pair.first << sanbox_value.get_name() << sanbox_value.get_m();
            setSandbox(sanbox_value);
            GetNumberForAGroup();
            return true;
        }
    }
    QString message = "Unable to access link";
    AppModel::instance()->showToast(0, message, EWARNING::WarningType::NONE_MSG);
    return false;
}

bool QNormalSandbox::GetGroup(const QString &group_id)
{
    QWarningMessage msg;
    nunchuk::GroupSandbox sandbox = bridge::GetGroup(group_id, msg);
    DBG_INFO << msg.type() << msg.what() << sandbox.is_finalized();
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        setSandbox(sandbox);
        return true;
    }
    else {
        if(msg.code() == (int)nunchuk::GroupException::GROUP_NOT_FOUND || msg.code() == (int)nunchuk::GroupException::SANDBOX_FINALIZED){
            // FIXME1
            // Show popup
            // Title: Unable to access link
            // Content: The wallet link has expired or wallet has already been created
        }
        else {
            AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        }
    }
    return false;
}

bool QNormalSandbox::AddSignerToGroup(const nunchuk::SingleSigner &signer)
{
    QWarningMessage msg;
    auto sandbox_value = bridge::AddSignerToGroup(QString::fromStdString(m_sandbox.get_id()), signer, m_index, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        setSandbox(sandbox_value);
        return true;
    } else {
        setSandbox(m_sandbox);
        if(msg.code() == (int)nunchuk::GroupException::SIGNER_EXISTS){
            QString message = "Key already in wallet";
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
        }
        else {
            AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
        }
        return false;
    }
}

bool QNormalSandbox::AddMasterToGroup()
{
    if (auto key = AppModel::instance()->masterSignerInfoPtr()) {
        QWarningMessage msg;
        QSingleSignerPtr signer = bridge::nunchukGetAvailableSignerFromMasterSigner(
            key,
            static_cast<ENUNCHUCK::WalletType>(m_sandbox.get_wallet_type()),
            static_cast<ENUNCHUCK::AddressType>(addressType()),
            msg);
        
        if(nunchuk::NunchukException::RUN_OUT_OF_CACHED_XPUB == msg.code()){
            emit needTopUpXpub();
            return false;
        }
        if (!signer.isNull() && url().isEmpty() && !qUtils::strCompare(m_fingerprintRecovery, signer->fingerPrint())) {
            emit recoverKeyError();
            return false; // add same finger print for recovery
        }
        if(!signer.isNull() && (int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            return AddSignerToGroup(signer->originSingleSigner());
        } else {
            DBG_INFO << "FixMe " << msg.type() << msg.what();
            AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    return false;
}

bool QNormalSandbox::AddRemoteToGroup()
{
    if (auto signer = AppModel::instance()->singleSignerInfoPtr()) {
        if (url().isEmpty() && !qUtils::strCompare(m_fingerprintRecovery, signer->fingerPrint())) return true; // add same finger print for recovery
        DBG_INFO << "Add remote: " << "fingerprint: " << signer->fingerPrint();
        AddSignerToGroup(signer->originSingleSigner());
    }
    return false;
}

void QNormalSandbox::setIndex(int idx)
{
    m_index = idx;
    m_sandbox = bridge::SetSlotOccupied(m_sandbox, m_index, true);
}

void QNormalSandbox::deleteGroup()
{
    if(bridge::DeleteGroup(groupId())) {
        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
        QSharedWallets::instance()->GetAllGroups();
    }
}

void QNormalSandbox::removeKey(int index)
{
    DBG_INFO << index;
    QWarningMessage msg;
    nunchuk::GroupSandbox sandbox = bridge::RemoveSignerFromGroup(groupId(), index, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        setSandbox(sandbox);
    }
}

void QNormalSandbox::registerSigners() {
    auto masterFunc = []()->bool {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                auto ret = sandbox->AddMasterToGroup();
                if (ret) {
                    sandbox->setScreenFlow("setup-group-wallet");
                    QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST);
                }
                return ret;
            }
        }
        return true;
    };
    QString mnemonic = qUtils::GenerateMnemonic();
    AppModel::instance()->setMnemonic(mnemonic);
    QSignerManagement::instance()->registerCreateMasterSigner(masterFunc);
    QSignerManagement::instance()->registerCreateSoftwareSigner(masterFunc);
    QSignerManagement::instance()->registerCreateSoftwareSignerXprv(masterFunc);
    QSignerManagement::instance()->registerCreateRemoteSigner([]()->bool {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                auto ret = sandbox->AddRemoteToGroup();
                sandbox->setScreenFlow("setup-group-wallet");
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST);
                if (ret) {
                    emit sandbox->recoverKeyError();
                    return false;
                }
            }
        }
        return true;
    });
}

void QNormalSandbox::requestAddOrRepaceKey(const QVariant &msg)
{
    QSignerManagement::instance()->clearExecute();
    QMap<QString, QVariant> maps = msg.toMap();
    int index = maps["idx"].toInt();
    QString fingerPrint = maps["fingerPrint"].toString();
    setIndex(index);
    setFingerprintRecovery(fingerPrint);
    registerSigners();
    if (auto w = AppModel::instance()->newWalletInfoPtr()) {
        w->makeExistingSigners();
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

void QNormalSandbox::requestAddNewKey() {
    registerSigners();
    QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_NEW_SIGNER_REQUEST);
}

void QNormalSandbox::slotClearOccupied()
{
    if(QEventProcessor::instance()->getCurrentStates().size() == 1) {
        if (m_occupied.isActive()) {
            m_occupied.stop();
        }
        clearOccupied();
    }
}

void QNormalSandbox::clearOccupied()
{
    std::vector<int> slotsToClear;
    for (const auto& pair : m_sandbox.get_occupied()) {
        if (pair.second.second == QSharedWallets::instance()->uid().toStdString()) {
            slotsToClear.push_back(pair.first);
        }
    }
    for (int index : slotsToClear) {
        m_sandbox = bridge::SetSlotOccupied(m_sandbox, index, false);
    }
}

bool QNormalSandbox::editBIP32Path(int index, const QString &master_id, const QString &path)
{
    DBG_INFO << "index: " << index << "master_id: " << master_id << "path: " << path;
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
    runInConcurrent([safeThis, index, master_id, path]() -> DataStruct {
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QWarningMessage msg;
        nunchuk::SingleSigner signer = bridge::GetSignerFromMasterSigner(master_id, path, msg);
        DataStruct data;
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            msg.resetWarningMessage();
            auto sandbox = bridge::AddSignerToGroup(QString::fromStdString(ptrLamda->sandbox().get_id()), signer, index, msg);
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

void QNormalSandbox::slotFinishSandboxWallet(const QString &wallet_id, bool reqClose)
{
    if (!wallet_id.isEmpty() && !isReplace()) {
        bridge::nunchukSetSelectedWallet(wallet_id);
    }
    QSharedWallets::instance()->GetAllGroups();
    if (reqClose) {
        CloseCurrentSandboxSetup();
    }
    if (!wallet_id.isEmpty()) {
        QString msg = "The group wallet has been created";
        AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
    }
    if (auto newWallet = QWalletPtr(new Wallet())) {
        newWallet->setWalletId(wallet_id);
        newWallet->setNeedBackup(true); // New Wallet needs backup
        newWallet->setNeedRegistered(true); // New Wallet needs registered
    }
    timeoutHandler(1000,[=]{
        emit AppModel::instance()->startReloadWallets();
    });
}

QStringList QNormalSandbox::qrTagsRecovery() const
{
    return m_qrTagsRecovery;
}

void QNormalSandbox::setQrTagsRecovery(const QStringList &newQrTagsRecovery)
{
    m_filePathRecovery.clear();
    m_qrTagsRecovery = newQrTagsRecovery;
}

QString QNormalSandbox::filePathRecovery() const
{
    return m_filePathRecovery;
}

void QNormalSandbox::setFilePathRecovery(const QString &newFilePathRecovery)
{
    m_qrTagsRecovery.clear();
    m_filePathRecovery = newFilePathRecovery;
}

void QNormalSandbox::CreateSignerListReviewWallet()
{
    DBG_INFO << screenFlow() << m_sandbox.is_finalized();
    if (screenFlow() == "sandbox-configure-value-keyset" || screenFlow() == "review-wallet") {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->setWalletN(groupN());
            w->setWalletM(groupM());
            w->setWalletName(groupName());
            w->setWalletAddressType(addressType());
            w->setWalletType(walletType());
            DBG_INFO << "groupM: " << groupM() << "groupN: " << groupN() << "addressType: " << addressType() << w->walletType();
            w->CreateSignerListReviewWallet(m_sandbox.get_signers());
            if (!w->enableValuekeyset()) {
                for(auto signer: w->singleSignersAssigned()->fullList()) {
                    signer->setValueKey(false);
                }
                w->singleSignersAssigned()->requestSortKeyset();
            }
        }
    }
}

QSet<size_t> QNormalSandbox::ValueKeyset()
{
    DBG_INFO << isReplace();
    QSet<size_t> valuekeyset;
    if (isReplace()) {
        auto currentWallet = AppModel::instance()->walletListPtr()->getWalletById(QString::fromStdString(m_sandbox.get_replace_wallet_id()));
        if (currentWallet.isNull()) return {};
        if (currentWallet->walletM() != groupM() || currentWallet->walletN() != groupN()) return {};
        if (auto list = currentWallet->singleSignersAssigned()) {
            std::vector<nunchuk::SingleSigner> signers = m_sandbox.get_signers();
            for (int i = 0; i < signers.size(); i++) {
                if (auto signer = list->getSingleSignerByIndex(i)) {
                    if (signer->valueKey()) {
                        valuekeyset.insert(i);
                    }
                }
            }
        }
    } else {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            DBG_INFO;
            if (auto list = w->singleSignersAssigned()) {
                DBG_INFO;
                std::vector<nunchuk::SingleSigner> signers = m_sandbox.get_signers();
                for (int i = 0; i < signers.size(); i++) {
                    if (auto signer = list->getSingleSignerByFingerPrint(QString::fromStdString(signers.at(i).get_master_fingerprint()))) {
                        DBG_INFO << i << signer->valueKey();
                        if (signer->valueKey()) {
                            valuekeyset.insert(i);
                        }
                    }
                }
            }
        }
    }
    return valuekeyset;
}

bool QNormalSandbox::CreateReplaceGroup(const QString &wallet_id)
{
    QWarningMessage msg;
    auto sandbox = bridge::CreateReplaceGroup(wallet_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setSandbox(sandbox);
        return true;
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return false;
}

bool QNormalSandbox::AcceptReplaceGroup(const QString &wallet_id, const QString &sandbox_id)
{
    QWarningMessage msg;
    auto sandbox = bridge::AcceptReplaceGroup(wallet_id, sandbox_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setSandbox(sandbox);
        return true;
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return false;
}

bool QNormalSandbox::DeclineReplaceGroup(const QString &wallet_id, const QString &sandbox_id)
{
    QWarningMessage msg;
    bridge::DeclineReplaceGroup(wallet_id, sandbox_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return true;
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return false;
}

void QNormalSandbox::CloseCurrentSandboxSetup()
{
    if(QEventProcessor::instance()->getCurrentStates().size() == 2) {
        int last = QEventProcessor::instance()->getCurrentStates().last();
        if (last == E::STATE_ID_SCR_SETUP_GROUP_WALLET) {
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
        }
    }
}

void QNormalSandbox::setFingerprintRecovery(const QString &newFingerprintRecovery)
{
    m_fingerprintRecovery = newFingerprintRecovery;
}

bool QNormalSandbox::checkWaitingOthers()
{
    for (int i = 0; i < m_sandbox.get_signers().size(); i++) {
        auto signer = m_sandbox.get_signers().at(i);
        if (signer.get_name() == "ADDED") {
            return true;
        }
    }
    return false;
}

nunchuk::GroupSandbox QNormalSandbox::sandbox() const
{
    return m_sandbox;
}

void QNormalSandbox::DeleteGroup(const QString &sandbox_id)
{
    if (qUtils::strCompare(groupId(), sandbox_id)) {
        CloseCurrentSandboxSetup();
        timeoutHandler(1000,[=]{
            QSharedWallets::instance()->GetAllGroups();
        });
    }
}

void QNormalSandbox::GetNumberForAGroup()
{
    int tmpCount = bridge::GetGroupOnline(groupId());
    DBG_INFO << tmpCount;
    setUserCount(tmpCount);
}

void QNormalSandbox::BackToSetupGroupWallet()
{
    if (screenFlow() == "review-wallet") {
        if (!HasEnoughSigner()) {
            setScreenFlow("setup-group-wallet");
        }
    }
}
