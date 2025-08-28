#include "QSharedWallets.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "Models/QWarningMessage.h"
#include "ifaces/nunchuckiface.h"
#include "Models/AppModel.h"
#include "Chats/ClientController.h"
#include "WalletModel.h"

QSharedWallets::QSharedWallets()
    : m_sandboxList(new GroupSandboxModel())
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

void QSharedWallets::clear()
{
    if(sandboxList()){
        sandboxList()->cleardata();
    }
}

QSharedWallets::~QSharedWallets()
{
    if (m_sandboxList) {
        m_sandboxList->clearOccupied();
    }
    m_sandboxList.clear();
}

QSharedWallets *QSharedWallets::instance()
{
    static QSharedWallets mInstance;
    return &mInstance;
}

void QSharedWallets::GetAllGroups()
{
    if(m_sandboxList.isValid()) {
        m_sandboxList->GetGroups();
    }
    CreateDeprecatedWallets();
}

void QSharedWallets::GetGroup(const QString &sandbox_id)
{
    AppModel::instance()->setNewWalletInfo(QWalletPtr(new Wallet()));
    if (auto gw = AppModel::instance()->newWalletInfoPtr()) {
        if (auto sandbox = sandboxListPtr()->GetGroup(sandbox_id)) {
            gw->setWalletAddressType(sandbox->addressType());
            gw->setGroupSandbox(sandbox);
            gw->setWalletOptType((int)ENUNCHUCK::WalletOptionType_t::E_GROUP_WALLET);
            sandbox->setScreenFlow("setup-group-wallet");
            sandbox->GetNumberForAGroup();
            sandbox->GetGroup(sandbox_id);
            sandbox->setSandbox(sandbox->sandbox());            
            QJsonObject json;
            json["type"] = "setup-group-wallet";
            QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST, json);
        }
    }
}

bool QSharedWallets::CheckGroupConfig()
{
    QWarningMessage msg;
    nunchuk::GroupConfig config = bridge::GetGroupConfig(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        DBG_INFO << config.get_remain() << config.get_max_keys(nunchuk::AddressType::NATIVE_SEGWIT);
        if (config.get_remain() <= 0) {
            // UI show error: user reach group wallet limit (check Figma for error message)
            emit groupWalletLimitChanged(!CLIENT_INSTANCE->isNunchukLoggedIn());
            return false;
        }
        m_config = config;
    }
    else{
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return true;
}

QGroupSandboxModelPtr QSharedWallets::sandboxListPtr() const
{
    return m_sandboxList;
}

GroupSandboxModel *QSharedWallets::sandboxList() const
{
    return sandboxListPtr().data();
}

int QSharedWallets::currentIndex() const
{
    return mCurrentIndex;
}

void QSharedWallets::setCurrentIndex(int currentIndex)
{
    mCurrentIndex = currentIndex;
    emit currentIndexChanged();
}

nunchuk::GroupConfig QSharedWallets::config() const
{
    return m_config;
}

QString QSharedWallets::uid()
{
    return m_uid;
}

void QSharedWallets::GetGroupDeviceUID()
{
    m_uid = bridge::GetGroupDeviceUID();
}

bool QSharedWallets::RecoverSandboxWallet(const QString &file_path)
{
    QString fileContent = qUtils::ImportDataViaFile(file_path);
    QWarningMessage msg;
    nunchuk::Wallet wallet = qUtils::ParseWalletDescriptor(fileContent, msg);
    QWalletPtr w = QWalletPtr(new Wallet(wallet));
    if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
        msg.resetWarningMessage();
        bool isGroupWallet = bridge::CheckGroupWalletExists(wallet, msg);
        if (isGroupWallet) {
            w->setWalletName("Group Wallet");
            AppModel::instance()->setNewWalletInfo(w);
            if (auto ptr = w->groupSandboxPtr()) {
                auto sandbox = CreateSandboxFromRecoverWallet(w);                
                ptr->setSandbox(sandbox);
                ptr->setFilePathRecovery(file_path);
                w->globalGroupWalletChanged();
                w->groupSandboxPtr()->setScreenFlow("setup-group-wallet");
            }
            QJsonObject json;
            json["type"] = "setup-group-wallet";
            QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST, json);
            return true;
        } else {
            QString message = QString("Group wallet not found");
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
        }
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return false;
}

nunchuk::GroupSandbox QSharedWallets::CreateSandboxFromRecoverWallet(const QWalletPtr& wallet)
{
    if (!wallet || !wallet.isValid()) {
        return nunchuk::GroupSandbox{""};
    }
    auto nWallet = wallet->nunchukWallet();
    std::string walletid = "";
    try {
        walletid = nWallet.get_id();
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }

    nunchuk::AddressType addressType = nWallet.get_address_type();
    QString name = wallet->walletName().isEmpty() ? "Group Wallet" : wallet->walletName();
    nunchuk::GroupSandbox sandbox {""};
    QWarningMessage msg;
    if (wallet->walletType() == (int)nunchuk::WalletType::MINISCRIPT) {
        QString script_tmpl = nWallet.get_miniscript().empty() ? "" : QString::fromStdString(nWallet.get_miniscript());
        sandbox.set_name(nWallet.get_name());
        sandbox.set_m(nWallet.get_m());
        sandbox.set_n(nWallet.get_n());
        sandbox.set_wallet_template(nWallet.get_wallet_template());
        sandbox.set_signers(nWallet.get_signers());
        sandbox.set_wallet_id(walletid);
        sandbox.set_address_type(nWallet.get_address_type());
        sandbox.set_miniscript_template(nWallet.get_miniscript());
    } else {
        sandbox.set_name(nWallet.get_name());
        sandbox.set_m(nWallet.get_m());
        sandbox.set_n(nWallet.get_n());
        sandbox.set_signers(nWallet.get_signers());
        sandbox.set_wallet_id(walletid);
        sandbox.set_address_type(nWallet.get_address_type());
    }
    return sandbox;
}

void QSharedWallets::changeToWallet()
{
    if (auto groupWalletList = AppModel::instance()->groupWalletList()) {
        if (auto walletList = AppModel::instance()->walletList()) {
            if (auto currentWallet = groupWalletList->currentWalletPtr()) {
                int index = walletList->getWalletIndexById(currentWallet->walletId());
                AppModel::instance()->setWalletListCurrentIndex(index);
                QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
            }
        }
    }
}

bool QSharedWallets::checkSandboxWalletLimit()
{
    return QSharedWallets::instance()->CheckGroupConfig();
}

QStringList QSharedWallets::deprecatedWallets() const
{
    return m_deprecatedWallets;
}

void QSharedWallets::CreateDeprecatedWallets()
{
    QWarningMessage msg;
    m_deprecatedWallets = bridge::GetDeprecatedGroupWallets(msg);
}

bool QSharedWallets::importQrSandboxWallet(const QStringList qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    if(in.isEmpty()){
        return false;
    }
    QWarningMessage msg;
    nunchuk::Wallet wallet = qUtils::ParseKeystoneWallet((nunchuk::Chain)AppSetting::instance()->primaryServer(),in, msg);
    QWalletPtr w = QWalletPtr(new Wallet(wallet));
    if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
        msg.resetWarningMessage();
        bool isGroupWallet = bridge::CheckGroupWalletExists(wallet, msg);
        DBG_INFO << "isGroupWallet: " << isGroupWallet;
        if (isGroupWallet) {
            w->setWalletName("Group Wallet");
            AppModel::instance()->setNewWalletInfo(w);
            if (auto ptr = w->groupSandboxPtr()) {
                auto sandbox = CreateSandboxFromRecoverWallet(w);
                ptr->setSandbox(sandbox);
                ptr->setQrTagsRecovery(in);
                w->globalGroupWalletChanged();
                w->groupSandboxPtr()->setScreenFlow("setup-group-wallet");
            }
            QJsonObject json;
            json["type"] = "setup-group-wallet";
            QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST, json);
        }
        else {
            QString message = QString("Group wallet not found");
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
        }
        return true;
    }
    else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return false;
}
