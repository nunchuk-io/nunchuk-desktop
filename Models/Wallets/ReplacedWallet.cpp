#include "ReplacedWallet.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"
#include "Models/DeviceModel.h"
#include "QThreadForwarder.h"

bool ReplacedWallet::m_tranReplace = false;
ReplacedWallet::ReplacedWallet(const nunchuk::Wallet &w) :
    WalletIO{w}
{}

bool ReplacedWallet::isReplaced() const
{    
    return false;
}

void ReplacedWallet::MixMasterSignerAndSingleSignerAll()
{
    m_replaceFree = true;
    setReplaceFlow("replace-key-info");
    auto masterList = AppModel::instance()->masterSignerListPtr();
    auto remoteList = AppModel::instance()->remoteSignerListPtr();
    auto w = dynamic_cast<Wallet*>(this);
    if (w == nullptr || masterList == nullptr || remoteList == nullptr) return;
    DBG_INFO << w->walletAddressType() << masterList->rowCount() << remoteList->rowCount();
    QJsonArray arrays;
    for (auto m : masterList->fullList()) {
        QJsonObject signer;
        signer["signer_deviceType"] = m->devicePtr()->type();
        signer["signer_type"] = (int)m->signerType();
        signer["signer_name"] = m->name();
        signer["signer_fingerPrint"] = m->fingerPrint();
        signer["signer_tag"] = m->tag();
        signer["signer_id"] = m->id();
        signer["signer_bip32_path"] = m->path();
        signer["signer_is_primary"] = m->isPrimaryKey();
        signer["signer_account_index"] = 0;
        signer["signer_card_id"] = m->device()->cardId();
        signer["signer_allowAssignToWallet"] = m->allowAssignToWallet();
        if (w && w->groupSandboxPtr() && w->groupSandboxPtr()->addressType() == (int)nunchuk::AddressType::TAPROOT) {
            if (m->signerType() == (int)nunchuk::SignerType::SOFTWARE && m->tag().isEmpty()) {
                arrays.append(signer);
            }
        } else {
            arrays.append(signer);
        }
    }
    for (auto s : remoteList->fullList()) {
        QJsonObject signer;
        signer["signer_deviceType"] = s->devicetype();
        signer["signer_type"] = (int)s->signerType();
        signer["signer_name"] = s->name();
        signer["signer_fingerPrint"] = s->masterFingerPrint();
        signer["signer_tag"] = s->tag();
        signer["signer_id"] = s->masterSignerId();
        signer["signer_bip32_path"] = s->derivationPath();
        signer["signer_is_primary"] = s->isPrimaryKey();
        signer["signer_account_index"] = s->accountIndex();
        signer["signer_card_id"] = s->cardId();
        signer["signer_allowAssignToWallet"] = s->allowAssignToWallet();
        if (w && w->groupSandboxPtr() && w->groupSandboxPtr()->addressType() == (int)nunchuk::AddressType::TAPROOT) {
            if (s->signerType() == (int)nunchuk::SignerType::SOFTWARE && s->tag().isEmpty()) {
                arrays.append(signer);
            }
        } else {
            arrays.append(signer);
        }
    }
    setSignerExistList(arrays);
}

QString ReplacedWallet::replaceFlow() const
{
    return m_replaceFlow;
}

void ReplacedWallet::setReplaceFlow(const QString &replaceFlow)
{
    if (m_replaceFlow == replaceFlow)
        return;
    DBG_INFO << QString().sprintf("%p", this) << replaceFlow;
    m_replaceFlow = replaceFlow;
    emit replaceFlowChanged();
}

void ReplacedWallet::SelectKeyToReplace(const QString &xfp, const int index)
{
    DBG_INFO << xfp << index;
    QWarningMessage msg;
    Signer_t signer;
    signer.signer = bridge::nunchukGetOriginSingleSigner(xfp,
                                                         ENUNCHUCK::WalletType::MULTI_SIG,
                                                         ENUNCHUCK::AddressType::NATIVE_SEGWIT,
                                                         index,
                                                         msg);
    signer.index = index;
    m_curReplaceKey = signer;
}

void ReplacedWallet::CreateANewWallet()
{
    AppModel::instance()->startCreateWallet(false, "");
}

void ReplacedWallet::walletCreateDone()
{
    if (m_replaceFree) {
        m_replaceFree = false;
        setReplaceFlow("congratulation-done");
        QWarningMessage msg;
        if (auto wallet = AppModel::instance()->walletInfoPtr()) { // Rename old wallet
            auto w = wallet->nunchukWallet();
            QString oldName = "[DEPRECATED]" + wallet->walletName();
            w.set_name(oldName.toStdString());
            bridge::UpdateWallet(w, msg);
            AppModel::instance()->startReloadUserDb();
        }
    }
}

int ReplacedWallet::reuseKeyGetCurrentIndex(const QString &xfp)
{
    QWarningMessage msg;
    int ret = bridge::nunchukGetLastUsedSignerIndex(xfp,
                                                    ENUNCHUCK::WalletType::MULTI_SIG,
                                                    ENUNCHUCK::AddressType::NATIVE_SEGWIT,
                                                    msg);
    if((int)EWARNING::WarningType::NONE_MSG != msg.type()){
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

QString ReplacedWallet::bip32path(const QString &xfp, int index)
{
    DBG_INFO << xfp << index;
    if(index >= 0){
        QWarningMessage msg;
        nunchuk::SingleSigner ret = bridge::nunchukGetOriginSingleSigner(xfp,
                                                                         ENUNCHUCK::WalletType::MULTI_SIG,
                                                                         ENUNCHUCK::AddressType::NATIVE_SEGWIT,
                                                                         index,
                                                                         msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            return QString::fromStdString(ret.get_derivation_path());
        }
        else{
            AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
        }
    }
    return "";
}

bool ReplacedWallet::updateKeyReplace(const QString &xfp, const int index)
{
    DBG_INFO << xfp << index << m_curReplaceKey.index;
    setDeviceType(deviceType(xfp));
    QWarningMessage msg;
    nunchuk::SingleSigner ret = bridge::nunchukGetOriginSingleSigner(xfp,
                                                                     ENUNCHUCK::WalletType::MULTI_SIG,
                                                                     ENUNCHUCK::AddressType::NATIVE_SEGWIT,
                                                                     index,
                                                                     msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        m_curReplaceKey.signer = ret;
        QSingleSignerPtr s = QSingleSignerPtr(new QSingleSigner(ret));
        s->setIsReplaced(true);
        auto old_s = dynamic_cast<Wallet*>(this)->singleSignersAssigned()->getSingleSignerByIndex(m_curReplaceKey.index);
        s->setKeyReplaced(old_s);
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->singleSignersAssigned()->replaceSingleSigner(m_curReplaceKey.index, s);
        }
        setReplaceFlow("replace-select-keys");
        return true;
    } else {
        // AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
        AppModel::instance()->setAddSignerWizard(1);
        setReplaceFlow("replace-add-signer");
        AppModel::instance()->startScanDevices(E::STATE_ID_SCR_ADD_HARDWARE_SIGNER);
    }
    return false;
}

bool ReplacedWallet::removeKeyReplaced(const int index)
{
    auto currentWallet = dynamic_cast<Wallet*>(this)->nunchukWallet();
    if (index >= currentWallet.get_n()) return false;
    auto signer = currentWallet.get_signers().at(index);
    QSingleSignerPtr s = QSingleSignerPtr(new QSingleSigner(signer));
    s->setIsReplaced(false);
    if (auto w = AppModel::instance()->newWalletInfoPtr()) {
        w->singleSignersAssigned()->replaceSingleSigner(index, s);
    }
    return true;
}

bool ReplacedWallet::tranReplace() const
{
    return m_tranReplace;
}

void ReplacedWallet::setTranReplace(bool newTranReplace)
{
    if (m_tranReplace == newTranReplace)
        return;
    m_tranReplace = newTranReplace;
    emit tranReplaceChanged();
}

bool ReplacedWallet::replaceFree() const
{
    return m_replaceFree;
}

void ReplacedWallet::updateNewKeyReplace(const QString &xfp)
{
    DBG_INFO << xfp;
    if (updateKeyReplace(xfp, 0)) {
        QMap<QString, bool> obj;
        obj["isFirst"] = false;
        QEventProcessor::instance()->sendEvent(E::EVT_REPLACE_KEYS_REQUEST, QVariant::fromValue(obj));
    }
}

void ReplacedWallet::refreshScanDevices()
{
    auto m_deviceType = deviceType();
    DBG_INFO << m_replaceFree << m_deviceType;
    if (m_replaceFree) {
        if (!m_deviceType.isEmpty()) {
            auto deviceList = AppModel::instance()->deviceListPtr();
            auto mRefreshDeviceList = QDeviceListModelPtr(new DeviceListModel());
            for (auto device : deviceList->fullList()) {
                if (device->type() == m_deviceType) {
                    mRefreshDeviceList->addDevice(device);
                }
            }
            DBG_INFO << "size: " << mRefreshDeviceList->rowCount();
            setDeviceList(mRefreshDeviceList);
        }
    }
}

QString ReplacedWallet::walletName()
{
    QString data_name = BaseWallet::walletName();
    if(data_name != ""){
        if (isReplaced() && !data_name.contains("DEPRECATED")) {
            setWalletName(QString("[DEPRECATED] %1").arg(data_name));
        }
    }
    return BaseWallet::walletName();
}

bool ReplacedWallet::ImportColdcardViaFile(const QString &fileName, int new_index)
{
    QWarningMessage msg;
    QString file_path = qUtils::QGetFilePath(fileName);
    QSingleSignerPtr retJson = bridge::nunchukParseJSONSigners(file_path, ENUNCHUCK::SignerType::AIRGAP, ENUNCHUCK::AddressType::NATIVE_SEGWIT, msg);
    DBG_INFO << fileName << msg.code() << retJson.isNull();
    if(retJson && (int)EWARNING::WarningType::NONE_MSG == msg.type()){
        msg.resetWarningMessage();
        DBG_INFO << retJson->fingerPrint();
        m_curReplaceKey.signer = retJson->originSingleSigner();
        retJson->setIsReplaced(true);
        auto old_s = dynamic_cast<Wallet*>(this)->singleSignersAssigned()->getSingleSignerByIndex(m_curReplaceKey.index);
        retJson->setKeyReplaced(old_s);
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->singleSignersAssigned()->replaceSingleSigner(m_curReplaceKey.index, retJson);
        }
        setReplaceFlow("replace-select-keys");
        return true;
    } else {
        QString message = QString("The account imported does not match the account that you entered (%1). Please try again.").arg(new_index);
        AppModel::instance()->showToast(-1, message, EWARNING::WarningType::EXCEPTION_MSG);
        return false;
    }
}
