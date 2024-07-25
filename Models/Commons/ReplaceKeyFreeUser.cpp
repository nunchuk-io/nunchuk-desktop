#include "ReplaceKeyFreeUser.h"
#include "ViewsEnums.h"
#include "AppModel.h"
#include "SingleSignerModel.h"
#include "MasterSignerModel.h"
#include "bridgeifaces.h"
#include "ServiceSetting.h"
#include "Premiums/QAssistedDraftWallets.h"
#include "Premiums/QWalletServicesTag.h"

void ReplaceKeyFreeUser::MixMasterSignerAndSingleSignerAll()
{
    m_replaceFree = true;
    setReplaceFlow("replace-key-info");
    auto masterList = AppModel::instance()->masterSignerListPtr();
    auto remoteList = AppModel::instance()->remoteSignerListPtr();
    DBG_INFO << masterList->rowCount() << remoteList->rowCount();
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
        arrays.append(signer);
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
        arrays.append(signer);
    }
    setSignerExistList(arrays);
}

QVariantList ReplaceKeyFreeUser::signerExistList() const
{
    return m_signerExistList.toVariantList();
}

void ReplaceKeyFreeUser::setSignerExistList(QJsonArray signerExistList)
{
    if (m_signerExistList == signerExistList)
        return;

    m_signerExistList = signerExistList;
    emit signerExistListChanged();
}

QString ReplaceKeyFreeUser::replaceFlow() const
{
    return m_replaceFlow;
}

void ReplaceKeyFreeUser::setReplaceFlow(const QString &replaceFlow)
{
    if (m_replaceFlow == replaceFlow)
        return;
    DBG_INFO << QString().sprintf("%p", this) << replaceFlow;
    m_replaceFlow = replaceFlow;
    emit replaceFlowChanged();
}

void ReplaceKeyFreeUser::SelectKeyToReplace(const QString &xfp, const int index)
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

void ReplaceKeyFreeUser::CreateANewWallet()
{
    AppModel::instance()->startCreateWallet(false, "");
}

void ReplaceKeyFreeUser::walletCreateDone()
{
    if (m_replaceFree) {
        m_replaceFree = false;
        setReplaceFlow("congratulation-done");
        QWarningMessage msg;
        if (auto wallet = AppModel::instance()->walletInfoPtr()) { // Rename old wallet
            auto w = wallet->wallet();
            QString oldName = "[DEPRECATED]" + wallet->name();
            w.set_name(oldName.toStdString());
            bridge::UpdateWallet(w, msg);
            AppModel::instance()->startReloadUserDb();
        }
    }
}

int ReplaceKeyFreeUser::reuseKeyGetCurrentIndex(const QString &xfp)
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

QString ReplaceKeyFreeUser::bip32path(const QString &xfp, int index)
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

bool ReplaceKeyFreeUser::updateKeyReplace(const QString &xfp, const int index)
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

bool ReplaceKeyFreeUser::replaceFree() const
{
    return m_replaceFree;
}

void ReplaceKeyFreeUser::updateNewKeyReplace(const QString &xfp)
{
    DBG_INFO << xfp;
    if (updateKeyReplace(xfp, 0)) {
        QMap<QString, bool> obj;
        obj["isFirst"] = false;
        QEventProcessor::instance()->sendEvent(E::EVT_REPLACE_KEYS_REQUEST, QVariant::fromValue(obj));
    }
}

void ReplaceKeyFreeUser::refreshScanDevices()
{
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

DeviceListModel *ReplaceKeyFreeUser::deviceList() const
{
    return deviceList_.data();
}

void ReplaceKeyFreeUser::setDeviceList(const QDeviceListModelPtr &d)
{
    if (deviceList_.isNull()) {
        deviceList_ = d;
        emit deviceListChanged();
    } else {
        deviceList_->updateDeviceList(d);
        emit deviceListChanged();
    }
}

QString ReplaceKeyFreeUser::deviceType() const
{
    return m_deviceType;
}

void ReplaceKeyFreeUser::setDeviceType(const QString &deviceType)
{
    if (m_deviceType == deviceType)
        return;
    m_deviceType = deviceType;
    emit deviceTypeChanged();
}

QString ReplaceKeyFreeUser::deviceType(const QString &xfp)
{
    auto m = bridge::nunchukGetMasterSignerFingerprint(xfp);
    if (m) {
        return m->devicePtr()->type();
    } else {
        auto s = bridge::nunchukGetRemoteSigner(xfp);
        if (s) {
            if (!s->tag().isEmpty()) {
                QMap<QString, QString> tags;
                tags.insert("LEDGER",   "ledger");
                tags.insert("TREZOR",   "trezor");
                tags.insert("COLDCARD", "coldcard");
                tags.insert("BITBOX",   "bitbox02");
                return tags.value(s->tag());
            } else if(s->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC) {
                return "coldcard";
            } else {
                return s->devicetype();
            }
        }
    }
    return "";
}

bool ReplaceKeyFreeUser::ImportColdcardViaFile(const QString &fileName, int new_index)
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
