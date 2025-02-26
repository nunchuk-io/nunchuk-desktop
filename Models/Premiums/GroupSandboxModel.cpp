#include "GroupSandboxModel.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "Models/QWarningMessage.h"
#include "ifaces/nunchuckiface.h"
#include "Models/AppModel.h"
#include "Chats/ClientController.h"
#include "Premiums/QSharedWallets.h"
#include "Premiums/QSignerManagement.h"


QGroupSandbox::QGroupSandbox()
    : m_sandbox("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QGroupSandbox::QGroupSandbox(nunchuk::GroupSandbox sandbox)
    : m_sandbox(sandbox)
{
    setSandbox(sandbox);
    connect(QEventProcessor::instance(), &QEventProcessor::stateChanged, this, &QGroupSandbox::slotClearOccupied);
    connect(&m_occupied, &QTimer::timeout, [this]() {
        GetGroup(groupId());
    });
    connect(this, &QGroupSandbox::finishSandboxWallet, this, &QGroupSandbox::slotFinishSandboxWallet);
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QGroupSandbox::~QGroupSandbox()
{

}

QString QGroupSandbox::groupId() const
{
    return QString::fromStdString(m_sandbox.get_id());
}

int QGroupSandbox::groupM() const
{
    return m_sandbox.get_m();
}

void QGroupSandbox::setGroupM(int m)
{
    if (m != groupM()) {
        m_sandbox.set_m(m);
        emit groupSandboxChanged();
    }
}

int QGroupSandbox::groupN() const
{
    return m_sandbox.get_n();
}

void QGroupSandbox::setGroupN(int n)
{
    if (n != groupN()) {
        m_sandbox.set_n(n);
        emit groupSandboxChanged();
    }
}

QString QGroupSandbox::groupName() const
{
    QString name = QString::fromStdString(m_sandbox.get_name());
    return name.isEmpty() ? "Group wallet" : name;
}

void QGroupSandbox::setGroupName(const QString &name)
{
    if (name != groupName()) {
        m_sandbox.set_name(name.toStdString());
        emit groupSandboxChanged();
    }
}

int QGroupSandbox::addressType() const
{
    if (m_sandbox.get_address_type() >= nunchuk::AddressType::ANY && m_sandbox.get_address_type() <= nunchuk::AddressType::TAPROOT) {
        return static_cast<int>(m_sandbox.get_address_type());
    } else {
        return static_cast<int>(nunchuk::AddressType::NATIVE_SEGWIT);
    }
}

void QGroupSandbox::setAddressType(int addr)
{
    if (addr != addressType()) {
        m_sandbox.set_address_type(static_cast<nunchuk::AddressType>(addr));
        emit groupSandboxChanged();
    }
}

QString QGroupSandbox::url() const
{
    return QString::fromStdString(m_sandbox.get_url());
}

QVariantList QGroupSandbox::groupKeys() const
{
    return m_groupKeys;
}

int QGroupSandbox::userCount() const
{
    return m_userCount;
}

QString QGroupSandbox::screenFlow() const
{
    return m_screenFlow;
}

void QGroupSandbox::setScreenFlow(const QString &flow)
{
    if (m_screenFlow != flow) {
        m_screenFlow = flow;
        emit screenFlowChanged();
        if (flow == "setup-group-wallet") {
            m_occupied.start(30000);
        }
    }
}

void QGroupSandbox::setUserCount(int number)
{
    if (number != m_userCount) {
        m_userCount = number;
        emit groupSandboxChanged();
    }
}

bool QGroupSandbox::enoughSigner()
{
    return groupId().isEmpty() ? HasOneKeyRecovery() : HasEnoughSigner();
}

bool QGroupSandbox::HasEnoughSigner()
{
    for (int i = 0; i < m_sandbox.get_signers().size(); i++) {
        auto key = m_sandbox.get_signers().at(i);
        if (key.get_name() == "") {
            return false;
        }
    }
    return true;
}

bool QGroupSandbox::HasOneKeyRecovery()
{
    return true;
}

bool QGroupSandbox::FinalizeGroup()
{
    DBG_INFO << groupId() << HasEnoughSigner();
    if (!groupId().isEmpty() && HasEnoughSigner()) {
        QWarningMessage msg;
        auto sandbox = bridge::FinalizeGroup(groupId(), msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            m_isCreateWallet = true;
            setSandbox(sandbox);
            setScreenFlow("bsms-file-success");
            QSharedWallets::instance()->GetAllGroups();
            AppModel::instance()->startReloadUserDb();
            QString msg = "Wallet has been created";
            AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
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

void QGroupSandbox::ExportWalletViaBSMS(const QString& file_path)
{
    DBG_INFO << QString::fromStdString(m_sandbox.get_wallet_id());
    bridge::nunchukExportWallet(QString::fromStdString(m_sandbox.get_wallet_id()), file_path, nunchuk::ExportFormat::BSMS);
}

void QGroupSandbox::ExportWalletViaQRBCUR2Legacy()
{
    QWarningMessage msgwarning;
    QStringList qrtags = bridge::nunchukExportKeystoneWallet(QString::fromStdString(m_sandbox.get_wallet_id()), msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
        AppModel::instance()->setQrExported(qrtags);
    }
    else{
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
    }
}

void QGroupSandbox::ExportWalletViaQRBCUR2()
{
    QWarningMessage msgwarning;
    QStringList qrtags = bridge::nunchukExportBCR2020010Wallet(QString::fromStdString(m_sandbox.get_wallet_id()), msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()){
        AppModel::instance()->setQrExported(qrtags);
    }
    else{
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
    }
}

void QGroupSandbox::CreateAGroup(const QString& name, int m, int n, int addType)
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
    }
}

void QGroupSandbox::UpdateGroup(const QString &name, int m, int n, int addType)
{
    nunchuk::AddressType addressType = static_cast<nunchuk::AddressType>(addType);
    QWarningMessage msg;
    auto sandbox = bridge::UpdateGroup(groupId(), name, m, n, addressType, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setSandbox(sandbox);
    }
}

void QGroupSandbox::setSandbox(const nunchuk::GroupSandbox &sandbox)
{
    m_sandbox = sandbox;
     DBG_INFO << sandbox.is_finalized() << m_isCreateWallet;
    if (sandbox.is_finalized() && !m_isCreateWallet) {
        emit finishSandboxWallet();
    }
    QString uid = QSharedWallets::instance()->uid();
    auto find = [&](const int index) -> QJsonObject {
        if (index < sandbox.get_signers().size()) {
            auto signer = sandbox.get_signers().at(index);
            if (signer.get_master_fingerprint().empty()) {
                QJsonObject json;
                if (signer.get_name() == "ADDED") {
                    // Signer at this index has been added, but its info is encrypted
                    // Display gray out `placeholder`, don't allow add signer here
                    json["isAdded"] = true;
                } else if (sandbox.get_occupied().contains(index)) {
                    // Signer at this index has not been added yet
                    std::pair<time_t, std::string> ts_uid = sandbox.get_occupied().at(index);
                    time_t timeout = 5 * 60; // 5 minutes
                    if (ts_uid.first + timeout > std::time(0) && ts_uid.second != uid.toStdString()) {
                        // Slot occupied by other device is not timeout
                        // Display `Occupied`, don't allow add signer here
                        json["isOccupied"] = true;
                    } else {
                        // Allow add signer here
                        json["isOccupied"] = false;
                    }
                }
                return json;
            } else {
                QWarningMessage msg;
                nunchuk::SingleSigner localSigner = bridge::nunchukGetOriginSingleSigner(signer, msg);
                DBG_INFO << localSigner.get_name() << msg.type() << localSigner.is_visible();
                if (localSigner.is_visible() && bridge::nunchukHasSinger(signer) && (int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                    QSingleSignerPtr signerPtr = QSingleSignerPtr(new QSingleSigner(localSigner));
                    if (signerPtr->isValid()) {
                        QJsonObject json;
                        json["name"] = signerPtr->name();
                        json["hisSigner"] = true;
                        json["has"] = true;
                        json["tag"] = signerPtr->tag();
                        json["type"] = qUtils::GetSignerTypeString(static_cast<nunchuk::SignerType>(signerPtr->signerType()));
                        json["signer_type"] = signerPtr->signerType();
                        json["account_index"] = qUtils::GetIndexFromPath(signerPtr->derivationPath());
                        json["card_id"] = signerPtr->cardId();
                        json["xfp"] = signerPtr->fingerPrint();
                        json["derivationPath"] = signerPtr->derivationPath();
                        return json;
                    }
                } else {
                    QSingleSignerPtr signerPtr = QSingleSignerPtr(new QSingleSigner(signer));
                    if (signerPtr->isValid()) {
                        QJsonObject json;
                        json["name"] = "";
                        json["hisSigner"] = false;
                        json["has"] = true;
                        json["tag"] = signerPtr->tag();
                        json["type"] = qUtils::GetSignerTypeString(static_cast<nunchuk::SignerType>(signerPtr->signerType()));
                        json["signer_type"] = signerPtr->signerType();
                        json["account_index"] = qUtils::GetIndexFromPath(signerPtr->derivationPath());
                        json["card_id"] = signerPtr->cardId();
                        json["xfp"] = signerPtr->fingerPrint();
                        json["derivationPath"] = signerPtr->derivationPath();
                        return json;
                    }
                }
            }
        }
        return {};
    };
    QVariantList groupKeys;
    for(int i = 0; i < sandbox.get_n(); i++) {
        QJsonObject obj = find(i);
        QString xfp = obj["xfp"].toString();
        if (xfp.isEmpty()) {
            obj["key_index"] = i;
            obj["has"] = false;
            obj["hisSigner"] = false;
            obj["colorIndex"] = i;
            if (!obj.contains("isOccupied")) {
                obj["isOccupied"] = false;
            }
            if (!obj.contains("isAdded")) {
                obj["isAdded"] = false;
            }
        } else {
            obj["isAdded"] = true;
            obj["isOccupied"] = false;
            obj["key_index"] = i;
            obj["colorIndex"] = i;
        }
        DBG_INFO << obj;
        groupKeys.insert(i, QVariant::fromValue(obj));
    }
    m_groupKeys = groupKeys;
    emit groupSandboxChanged();
    BackToSetupGroupWallet();
}

bool QGroupSandbox::JoinGroup(const QString &url)
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

bool QGroupSandbox::GetGroup(const QString &group_id)
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

bool QGroupSandbox::AddSignerToGroup(const nunchuk::SingleSigner &signer)
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

bool QGroupSandbox::AddMasterToGroup()
{
    if (auto key = AppModel::instance()->masterSignerInfoPtr()) {
        QWarningMessage msg;
        auto signer = bridge::nunchukGetSignerFromMasterSigner(key->id(),ENUNCHUCK::WalletType::MULTI_SIG, static_cast<ENUNCHUCK::AddressType>(addressType()), key->deviceIndex(), msg);
        if (url().isEmpty() && !qUtils::strCompare(m_fingerprintRecovery, signer->fingerPrint())) {
            emit recoverKeyError();
            return false; // add same finger print for recovery
        }
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            return AddSignerToGroup(signer->originSingleSigner());
        } else {
            AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    return false;
}

bool QGroupSandbox::AddRemoteToGroup()
{
    if (auto signer = AppModel::instance()->singleSignerInfoPtr()) {
        if (url().isEmpty() && !qUtils::strCompare(m_fingerprintRecovery, signer->fingerPrint())) return true; // add same finger print for recovery
        AddSignerToGroup(signer->originSingleSigner());
    }
    return false;
}

void QGroupSandbox::setIndex(int idx)
{
    m_index = idx;
    m_sandbox = bridge::SetSlotOccupied(m_sandbox, m_index, true);
}

void QGroupSandbox::deleteGroup()
{
    if(bridge::DeleteGroup(groupId())) {
        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
        QSharedWallets::instance()->GetAllGroups();
    }
}

void QGroupSandbox::removeKey(int index)
{
    DBG_INFO << index;
    QWarningMessage msg;
    nunchuk::GroupSandbox sandbox = bridge::RemoveSignerFromGroup(groupId(), index, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        setSandbox(sandbox);
    }
}

void QGroupSandbox::requestAddOrRepaceKey(const QVariant &msg)
{
    QSignerManagement::instance()->clearExecute();
    QMap<QString, QVariant> maps = msg.toMap();
    int index = maps["idx"].toInt();
    QString fingerPrint = maps["fingerPrint"].toString();
    setIndex(index);
    setFingerprintRecovery(fingerPrint);
    auto masterFunc = []()->bool {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            if (auto sandbox = w->groupSandboxPtr()) {
                sandbox->setScreenFlow("setup-group-wallet");
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST);
                return sandbox->AddMasterToGroup();
            }
        }
        return true;
    };
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
    if (auto w = AppModel::instance()->newWalletInfoPtr()) {
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

void QGroupSandbox::slotClearOccupied()
{
    DBG_INFO << QEventProcessor::instance()->getCurrentStates().size();
    if(QEventProcessor::instance()->getCurrentStates().size() == 1) {
        if (m_occupied.isActive()) {
            m_occupied.stop();
        }
        clearOccupied();
    }
}

void QGroupSandbox::clearOccupied()
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

bool QGroupSandbox::editBIP32Path(int index, const QString &master_id, const QString &path)
{
    QWarningMessage msg;
    nunchuk::SingleSigner signer = bridge::GetSignerFromMasterSigner(master_id, path, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        msg.resetWarningMessage();
        auto sandbox = bridge::AddSignerToGroup(QString::fromStdString(m_sandbox.get_id()), signer, index, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            setSandbox(sandbox);
            return true;
        }
    }
    return false;
}

void QGroupSandbox::slotFinishSandboxWallet()
{
    emit AppModel::instance()->startReloadUserDb();
    QSharedWallets::instance()->GetAllGroups();
}

QString QGroupSandbox::filePathRecovery() const
{
    return m_filePathRecovery;
}

void QGroupSandbox::setFilePathRecovery(const QString &newFilePathRecovery)
{
    m_filePathRecovery = newFilePathRecovery;
}

void QGroupSandbox::setFingerprintRecovery(const QString &newFingerprintRecovery)
{
    m_fingerprintRecovery = newFingerprintRecovery;
}

bool QGroupSandbox::checkWaitingOthers()
{
    for (int i = 0; i < m_sandbox.get_signers().size(); i++) {
        auto signer = m_sandbox.get_signers().at(i);
        if (signer.get_name() == "ADDED") {
            return true;
        }
    }
    return false;
}

nunchuk::GroupSandbox QGroupSandbox::sandbox() const
{
    return m_sandbox;
}

void QGroupSandbox::DeleteGroup(const QString &sandbox_id)
{
    if (qUtils::strCompare(groupId(), sandbox_id)) {
        emit finishSandboxWallet();
        timeoutHandler(1000,[=]{
            QSharedWallets::instance()->GetAllGroups();
        });
    }
}

void QGroupSandbox::GetNumberForAGroup()
{
    int tmpCount = bridge::GetGroupOnline(groupId());
    DBG_INFO << tmpCount;
    setUserCount(tmpCount);
}

void QGroupSandbox::BackToSetupGroupWallet()
{
    if (screenFlow() == "review-wallet") {
        if (!HasEnoughSigner()) {
            setScreenFlow("setup-group-wallet");
        }
    }
}

GroupSandboxModel::GroupSandboxModel()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

GroupSandboxModel::~GroupSandboxModel()
{
    m_data.clear();
}

int GroupSandboxModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

QVariant GroupSandboxModel::data(const QModelIndex &index, int role) const
{
    if(auto data = m_data[index.row()]){
        switch (role) {
        case group_Id_Role:
            return data->groupId();
        case group_Name_Role:
            DBG_INFO << data->groupName();
            return data->groupName();
        case group_M_Role:
            return data->groupM();
        case group_N_Role:
            return data->groupN();
        case group_AddressType_Role:
            return data->addressType();
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QHash<int, QByteArray> GroupSandboxModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[group_Id_Role]   = "group_id";
    roles[group_Name_Role] = "group_name";
    roles[group_M_Role]    = "group_M";
    roles[group_N_Role]    = "group_N";
    roles[group_AddressType_Role]  = "group_AddressType";
    return roles;
}

int GroupSandboxModel::count() const
{
    return m_data.size();
}

void GroupSandboxModel::GetGroups()
{
    QPointer<GroupSandboxModel> safeThis(this);
    runInThread<std::vector<nunchuk::GroupSandbox>>([]() ->std::vector<nunchuk::GroupSandbox>{
        QWarningMessage msg;
        return bridge::GetGroups(msg);
    },[safeThis](std::vector<nunchuk::GroupSandbox> sandboxs) {
        safeThis->beginResetModel();
        safeThis.data()->m_data.clear();
        for(auto sandbox : sandboxs) {
            safeThis.data()->m_data.append(QGroupSandboxPtr(new QGroupSandbox(sandbox)));
        }
        safeThis->endResetModel();
        DBG_INFO << safeThis->count();
    });
}

QGroupSandboxPtr GroupSandboxModel::GetGroup(const QString &sandbox_id)
{
    for(auto sandbox: m_data) {
        if (qUtils::strCompare(sandbox_id, sandbox->groupId())) return sandbox;
    }
    return NULL;
}

void GroupSandboxModel::updateSandox(const nunchuk::GroupSandbox sandbox)
{
    for(int i = 0; i < m_data.count(); i++){
        if(m_data.at(i) && qUtils::strCompare(QString::fromStdString(m_data.at(i).data()->sandbox().get_id()), QString::fromStdString(sandbox.get_id()))){
            m_data.at(i).data()->setSandbox(sandbox);
            emit dataChanged(index(i),index(i));
        }
    }
}

bool GroupSandboxModel::contains(const QString &sandbox_id)
{
    for(auto it : m_data){
        if(it && qUtils::strCompare(it->groupId(), sandbox_id)){
            return true;
        }
    }
    return false;
}

void GroupSandboxModel::clearOccupied()
{
    for(auto it : m_data){
        if(it){
            it->clearOccupied();
        }
    }
}

void GroupSandboxModel::cleardata()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}
