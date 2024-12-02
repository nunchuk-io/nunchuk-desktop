#include "QAssistedDraftWallets.h"
#include <QQmlEngine>
#include <QJsonDocument>
#include "AppModel.h"
#include "localization/STR_CPP.h"
#include "ServiceSetting.h"
#include "nunchuckiface.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QWalletManagement.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QUserWalletDummyTx.h"
#include "Premiums/QGroupWallets.h"

const QMap<Key, StructAddHardware> map_keys = {
    {Key::ADD_LEDGER,   {"LEDGER",   "ledger",   STR_CPP_122, STR_CPP_121, 124}},
    {Key::ADD_TREZOR,   {"TREZOR",   "trezor",   STR_CPP_124, STR_CPP_123, 124}},
    {Key::ADD_COLDCARD, {"COLDCARD", "coldcard", STR_CPP_126, STR_CPP_125, 152}},
    {Key::ADD_BITBOX,   {"BITBOX",   "bitbox02", STR_CPP_128, STR_CPP_127, 124}},
};

namespace {

inline QString titleCase(QString str)
{
    if (str.length() == 0) return "";
    QString tmp = str;
    // if you want to ensure all other letters are lowercase:
    tmp = tmp.toLower();
    tmp[0] = str[0].toUpper();
    return tmp;
}
}
QAssistedDraftWallets::QAssistedDraftWallets(int mode)
    : QSwitchAPI(mode)
    , m_servicesTag(new QWalletServicesTag(mode))
    , mRefreshDeviceList(QDeviceListModelPtr(new DeviceListModel()))
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QAssistedDraftWallets::~QAssistedDraftWallets(){
    mRefreshDeviceList.clear();
}

QString QAssistedDraftWallets::typeReq() const
{
    if(getAddHardware() == 0) return "";
    return map_keys.value((Key)getAddHardware()).mType;
}

void QAssistedDraftWallets::GetListAllRequestAddKey(const QJsonArray& groups)
{
    if(!CLIENT_INSTANCE->isNunchukLoggedIn()){
        return;
    }
    if (m_mode == USER_WALLET) {
        QtConcurrent::run([&]() {
            QJsonObject data;
            QString error_msg;
            m_requests.clear();
            Draco::instance()->assistedWalletGetListKey(data, error_msg);
            addRequest(data.value("requests").toArray());
            makeListRequests();
            DBG_INFO << data;
        });
    }
    else if (m_mode == GROUP_WALLET) {
        QJsonObject data;
        QString error_msg;
        m_requests.clear();
        for (QJsonValue js : groups) {
            QJsonObject group = js.toObject();
            QString status = group["status"].toString();
            if (status == "PENDING_WALLET") {
                QString group_id = group["id"].toString();
                Byzantine::instance()->GetAllListRequestAddKey(group_id, data, error_msg);
                addRequest(data.value("requests").toArray(), group_id);
                DBG_INFO << group_id << data;
            }
        }
        makeListRequests();
    } else {
        return;
    }
}

bool QAssistedDraftWallets::AddOrUpdateAKeyToDraftWallet()
{
    return RequestAddOrUpdateAKeyToDraftWallet(m_request);
}

bool QAssistedDraftWallets::AddOrUpdateReuseKeyToDraftWallet(nunchuk::SingleSigner keyresued)
{
    return RequestAddOrUpdateReuseKeyToDraftWallet(m_request, keyresued);;
}

void QAssistedDraftWallets::cancelRequestKey(const QString &request_id, const QString& group_id)
{
    if (!request_id.isEmpty()) {
        bool ret {false};
        if (group_id.isEmpty()) {
            ret = Draco::instance()->assistedWalletRemoveId(request_id);
        }
        else {
            ret = Byzantine::instance()->DeleteRequestAddKey(group_id, request_id);
        }
        if (ret) {
            updateRequestKey();
        }
    }
}

bool QAssistedDraftWallets::RequestAddOrUpdateAKeyToDraftWallet(StructAddHardware hardware){
    if(!CLIENT_INSTANCE->isNunchukLoggedIn()){
        return false;
    }
    if (!m_selectFingerPrint.isEmpty()) {
        QJsonObject data;
        QWarningMessage warningmsg;
        QSingleSignerPtr single = AppModel::instance()->remoteSignerListPtr()->getSingleSignerByFingerPrint(m_selectFingerPrint);
        if (single.isNull()) {
            QMasterSignerPtr master = AppModel::instance()->masterSignerListPtr()->getMasterSignerByXfp(m_selectFingerPrint);
            single = bridge::nunchukGetDefaultSignerFromMasterSigner(master->id(),
                                                                     ENUNCHUCK::WalletType::MULTI_SIG,
                                                                     ENUNCHUCK::AddressType::NATIVE_SEGWIT ,
                                                                     warningmsg);
        }
        if (single.isNull()){
            return false;
        }
        DBG_INFO << single->tag() << "tag:" << hardware.mTag << m_mode << hardware.mGroupId << " ==== " << hardware.mRequestId;
        if (single->tag() == hardware.mTag) {
            if (warningmsg.type() == (int)EWARNING::WarningType::NONE_MSG) {
                QJsonArray tags;
                if (!hardware.mTag.isEmpty()) {
                    tags.append(hardware.mTag);
                }

                data["name"] = single->name() != "" ? single->name() : titleCase(hardware.mTag);
                data["xfp"] = single->masterFingerPrint();
                data["derivation_path"] = single->derivationPath();
                data["xpub"] = single->xpub();
                data["pubkey"] = single->publickey();
                data["type"] = qUtils::GetSignerTypeString(single->singleSigner().get_type());
                auto dash = QGroupWallets::instance()->GetDashboard(hardware.mGroupId);
                if (dash) {
                    if (dash->allowInheritance()) { // Request from Alert DashBoard
                        if (dash->nInfo() == 4 && hardware.mKeyIndex == 0) {
                            tags.append("INHERITANCE");
                        }
                        else if (dash->nInfo() == 5 && (hardware.mKeyIndex == 0 || hardware.mKeyIndex == 1)) {
                            tags.append("INHERITANCE");
                        }
                    }
                }
                if (hardware.mTags.contains("INHERITANCE") && !tags.contains("INHERITANCE")) { // Request receive from Mobile Banner
                    tags.append("INHERITANCE");
                }
                data["name"] = tags.contains("INHERITANCE") ? data["name"].toString() + "(inh.)" : data["name"].toString();
                data["tags"] = tags;
                data["tapsigner"] = {};
                if (hardware.mKeyIndex >= 0) {
                    data["key_index"] = hardware.mKeyIndex;
                }
                else {
                    data["key_index"] = {};
                }
                DBG_INFO << data;
                bool isDuplicateKey {false};
                bool ret {false};
                QString error_msg;
                if (hardware.mGroupId.isEmpty()) {
                    ret = Draco::instance()->assistedWalletAddKey(hardware.mRequestId, data, isDuplicateKey, error_msg);
                    DBG_INFO << ret << isDuplicateKey;
                } else {
                    if (dash && dash->isDraftWallet()) {
                        ret = Draco::instance()->assistedWalletAddKey(hardware.mRequestId, data, isDuplicateKey, error_msg);
                    } else {
                        ret = Byzantine::instance()->DraftWalletAddKey(hardware.mGroupId, hardware.mRequestId, data, isDuplicateKey, error_msg);
                        DBG_INFO << ret << error_msg << isDuplicateKey;
                    }
                    if (ret) {
                        if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
                            dashboard->GetAlertsInfo();
                            dashboard->GetHealthCheckInfo();
                        }
                    }
                }
                if (ret) {
                    AppModel::instance()->setAddSignerWizard(3);
                    return true;
                }
                else {
                    AppModel::instance()->setAddSignerWizard(4);
                    if (isDuplicateKey) {
                        emit addHardwareAlert();
                    }
                    return false;
                }
            }
        }
    }
    AppModel::instance()->setAddSignerWizard(4);
    return false;
}

bool QAssistedDraftWallets::RequestAddOrUpdateReuseKeyToDraftWallet(StructAddHardware hardware, nunchuk::SingleSigner keyresued)
{
    if(!CLIENT_INSTANCE->isNunchukLoggedIn()){
        return false;
    }
    if (!m_selectFingerPrint.isEmpty()) {
        QJsonObject data;
        QJsonArray tags;
        if (!hardware.mTag.isEmpty()) {
            tags.append(hardware.mTag);
        }

        QString name = QString::fromStdString(keyresued.get_name());
        data["name"]            = name != "" ? name : titleCase(hardware.mTag);
        data["xfp"]             = QString::fromStdString(keyresued.get_master_fingerprint());
        data["derivation_path"] = QString::fromStdString(keyresued.get_derivation_path());
        data["xpub"]            = QString::fromStdString(keyresued.get_xpub());
        data["pubkey"]          = QString::fromStdString(keyresued.get_public_key());
        data["type"]            = qUtils::GetSignerTypeString(keyresued.get_type());
        auto dash = QGroupWallets::instance()->GetDashboard(hardware.mGroupId);
        if (dash) {
            if (dash->allowInheritance()) { // Request from Alert Dasboard
                if (dash->nInfo() == 4 && hardware.mKeyIndex == 0) {
                    tags.append("INHERITANCE");
                }
                else if (dash->nInfo() == 5 && (hardware.mKeyIndex == 0 || hardware.mKeyIndex == 1)) {
                    tags.append("INHERITANCE");
                }
            }
        }
        if (hardware.mTags.contains("INHERITANCE") && !tags.contains("INHERITANCE")) { // Request receive from Mobile Banner
            tags.append("INHERITANCE");
        }
        data["name"] = tags.contains("INHERITANCE") ? data["name"].toString() + "(inh.)" : data["name"].toString();
        data["tags"] = tags;
        data["tapsigner"] = {};
        if (hardware.mKeyIndex >= 0) {
            data["key_index"] = hardware.mKeyIndex;
        }
        else {
            data["key_index"] = {};
        }
        DBG_INFO << data;
        bool isDuplicateKey {false};
        bool ret {false};
        QString error_msg;
        if (hardware.mGroupId.isEmpty()) {
            ret = Draco::instance()->assistedWalletAddKey(hardware.mRequestId, data, isDuplicateKey, error_msg);
            DBG_INFO << ret << isDuplicateKey << "Honybadger/IronHand";
        }
        else {
            if (dash && dash->isDraftWallet()) {
                ret = Draco::instance()->assistedWalletAddKey(hardware.mRequestId, data, isDuplicateKey, error_msg);
                DBG_INFO << ret << isDuplicateKey << "Honybadger/IronHand";
            } else {
                ret = Byzantine::instance()->DraftWalletAddKey(hardware.mGroupId, hardware.mRequestId, data, isDuplicateKey, error_msg);
                DBG_INFO << ret << error_msg << isDuplicateKey << "Not Honybadger/IronHand";
            }
            if (ret) {
                if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
                    dashboard->GetAlertsInfo();
                    dashboard->GetHealthCheckInfo();
                }
            }
        }
        if (ret) {
            return true;
        }
        else {
            if (isDuplicateKey) {
                emit addHardwareAlert();
            }
            return false;
        }
    }
    return false;
}

void QAssistedDraftWallets::addRequest(const QJsonArray &requests, const QString& group_id)
{
    DBG_INFO << requests;
    for (QJsonValue request : requests) {
        QJsonObject requestObj = request.toObject();
        QString status = requestObj.value("status").toString();
        QJsonArray tags = requestObj.value("tags").toArray();
        QString request_id = requestObj.value("id").toString();
        int key_index = requestObj.value("key_index").toInt(-1);
        if (status == "PENDING") {
            for (Key key: map_keys.uniqueKeys()) {
                StructAddHardware hardware = map_keys.value(key);
                if (tags.contains(hardware.mTag) && status == "PENDING") {
                    hardware.mGroupId = group_id;
                    hardware.mRequestId = request_id;
                    hardware.mKeyIndex = key_index;
                    hardware.mTags.clear();
                    for (auto tag: tags) {
                        hardware.mTags.append(tag.toString());
                    }
                    m_requests.insert(key, hardware);
                }
            }
        }
    }
}

void QAssistedDraftWallets::makeListRequests()
{
    QVariantList hardwares;
    DBG_INFO << m_requests.size();
    for (auto i = m_requests.cbegin(), end = m_requests.cend(); i != end; ++i) {
        StructAddHardware hardware = i.value();
        Key key = i.key();
        QMap<QString, QVariant> maps;
        maps["key"] = hardware.mTag;
        maps["type"] = (int)key;
        maps["textBtn"] = hardware.mTextBtn;
        maps["title"] = hardware.mTitle;
        maps["mHeight"] = hardware.mHeight;
        maps["group_id"] = hardware.mGroupId;
        maps["request_id"] = hardware.mRequestId;
        DBG_INFO << maps;
        hardwares.append(QVariant::fromValue(maps));
    }
    setHardwareReq(hardwares);
}

void QAssistedDraftWallets::reset()
{
    m_requests.clear();
    makeListRequests();
}

DeviceListModel* QAssistedDraftWallets::refreshDeviceList() const
{
    return mRefreshDeviceList.data();
}

void QAssistedDraftWallets::addHardwareFromConfig(int hardwareType, const QString& group_id, int key_index, bool is_inheritance)
{
    DBG_INFO << hardwareType << group_id << key_index;
    QSwitchAPI::setAddHardware(hardwareType);
    m_request = map_keys.value((Key)hardwareType);
    if (is_inheritance) {
        m_request.mTags.append("INHERITANCE");
    }
    m_request.mGroupId = group_id;
    m_request.mRequestId = "";
    m_request.mKeyIndex = key_index;
}

void QAssistedDraftWallets::addHardwareFromBanner(const QString &request_id)
{
    for (auto i = m_requests.cbegin(), end = m_requests.cend(); i != end; ++i) {
        StructAddHardware hardware = i.value();
        Key key = i.key();
        if (qUtils::strCompare(hardware.mRequestId, request_id)) {
            QSwitchAPI::setAddHardware((int)key);
            m_request = hardware;
        }
    }
}

int QAssistedDraftWallets::reuseKeyGetCurrentIndex(const QString &xfp)
{
    QWarningMessage msg;
    int ret = bridge::nunchukGetLastUsedSignerIndex(xfp,
                                                    ENUNCHUCK::WalletType::MULTI_SIG,
                                                    ENUNCHUCK::AddressType::NATIVE_SEGWIT,
                                                    msg);
    if((int)EWARNING::WarningType::NONE_MSG != msg.type()){
        DBG_INFO << msg.code() << msg.what();
    }
    return ret;
}

bool QAssistedDraftWallets::getSignerFromMasterSigner(const QString &xfp, const int index)
{
    DBG_INFO << xfp << index;
    QWarningMessage msg;
    QSingleSignerPtr ret = bridge::nunchukGetSignerFromMasterSigner(xfp,
                                                                    ENUNCHUCK::WalletType::MULTI_SIG,
                                                                    ENUNCHUCK::AddressType::NATIVE_SEGWIT,
                                                                    index,
                                                                    msg);
    if(ret && (int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setSelectFingerPrint(xfp);
        return canReplaceKey() ? requestKeyReplacement(ret) : AddOrUpdateReuseKeyToDraftWallet(ret.data()->singleSigner());
    }
    else if((int)EWARNING::WarningType::EXCEPTION_MSG == msg.type() && nunchuk::NunchukException::RUN_OUT_OF_CACHED_XPUB == msg.code()){
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::WARNING_MSG);
        return false;
    }
    else{
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
        return false;
    }
}

bool QAssistedDraftWallets::getSigner(const QString &xfp, const int index)
{
    DBG_INFO << xfp << index;
    QWarningMessage msg;
    nunchuk::SingleSigner ret = bridge::nunchukGetOriginSingleSigner(xfp,
                                                                     ENUNCHUCK::WalletType::MULTI_SIG,
                                                                     ENUNCHUCK::AddressType::NATIVE_SEGWIT,
                                                                     index,
                                                                     msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setSelectFingerPrint(xfp);
        return canReplaceKey() ? requestKeyReplacement(QSingleSignerPtr(new QSingleSigner(ret))) : AddOrUpdateReuseKeyToDraftWallet(ret);
    }
    //Don't show toast here
    return false;
}

bool QAssistedDraftWallets::canReplaceKey()
{
    auto dashboard = AppModel::instance()->walletInfoPtr()->dashboard();
    if (dashboard && dashboard->canReplaceKey()) {
        return true;
    } else {
        return false;
    }
}

bool QAssistedDraftWallets::requestKeyReplacement(QSingleSignerPtr signer)
{
    StructAddHardware hardware = m_request;
    if(!CLIENT_INSTANCE->isNunchukLoggedIn()){
        return false;
    }
    if (!m_selectFingerPrint.isEmpty()) {
        QJsonObject data;
        QWarningMessage warningmsg;
        QSingleSignerPtr single {NULL};
        if (signer.isNull()) {
            single = bridge::nunchukGetRemoteSigner(m_selectFingerPrint);
        } else {
            single = signer;
        }

        if (single.isNull()) {
            QMasterSignerPtr master = bridge::nunchukGetMasterSignerFingerprint(m_selectFingerPrint);
            single = bridge::nunchukGetDefaultSignerFromMasterSigner(master->id(),
                                                                     ENUNCHUCK::WalletType::MULTI_SIG,
                                                                     ENUNCHUCK::AddressType::NATIVE_SEGWIT ,
                                                                     warningmsg);
        }
        if (single.isNull()){
            return false;
        }

        DBG_INFO << single->tag() << "tag:" << single->tag() << m_mode << hardware.mGroupId;
        if (warningmsg.type() == (int)EWARNING::WarningType::NONE_MSG) {
            QJsonArray tags;
            if (!single->tags().isEmpty()) {
                QStringList list = single->tags();
                list.removeAll("INHERITANCE");
                for (auto tag : list) {
                    tags.append(tag);
                }
            }
            data["name"] = single->name() != "" ? single->name() : titleCase(single->tag());
            data["xfp"] = single->masterFingerPrint();
            data["derivation_path"] = single->derivationPath();
            data["xpub"] = single->xpub();
            data["pubkey"] = single->publickey();
            data["type"] = qUtils::GetSignerTypeString(single->singleSigner().get_type());
            auto dashboard = QGroupWallets::instance()->dashboardInfoPtr();
            if (dashboard.isNull()) return false;
            if (dashboard->isInheritance() || hardware.mTags.contains("INHERITANCE")) { // Request receive from Mobile Banner
                tags.append("INHERITANCE");
            }
            data["name"] = tags.contains("INHERITANCE") ? data["name"].toString() + "(inh.)" : data["name"].toString();
            data["tags"] = tags;
            data["tapsigner"] = {};
            if (hardware.mKeyIndex >= 0) {
                data["key_index"] = hardware.mKeyIndex;
            }
            else {
                data["key_index"] = {};
            }
            bool ret = dashboard->FinishKeyReplacement(data);
            if (ret) {
                AppModel::instance()->setAddSignerWizard(3);
                return true;
            }
            else {
                AppModel::instance()->setAddSignerWizard(4);
                return false;
            }
        }
    }
    AppModel::instance()->setAddSignerWizard(4);
    return false;
}

QAssistedDraftWallets::ImportColdcard_t QAssistedDraftWallets::ImportColdcardViaFile(const QString &fileName, int new_index)
{
    using Enum = QAssistedDraftWallets::ImportColdcard_t;
    QWarningMessage msg;
    QString file_path = qUtils::QGetFilePath(fileName);
    QSingleSignerPtr retJson = bridge::nunchukParseJSONSigners(file_path, ENUNCHUCK::SignerType::AIRGAP, ENUNCHUCK::AddressType::NATIVE_SEGWIT, msg);
    if(retJson && (int)EWARNING::WarningType::NONE_MSG == msg.type()){
        msg.resetWarningMessage();
        int current_index = reuseKeyGetCurrentIndex(retJson->masterFingerPrint());
        int account_index = qUtils::GetIndexFromPath(retJson->derivationPath());
        if(new_index != -1 && current_index != new_index && new_index == account_index){
            setSelectFingerPrint(retJson->masterFingerPrint());
            bool ret = canReplaceKey() ? requestKeyReplacement(retJson) : AddOrUpdateReuseKeyToDraftWallet(retJson->originSingleSigner());
            return ret ? Enum::eOK : Enum::eError_Keep_Screen;
        }
        else if (new_index != -1 && current_index != new_index && new_index != account_index) {
            QString message = QString("The account imported does not match the account that you entered (%1). Please try again.").arg(new_index);
            AppModel::instance()->showToast(-1, message, EWARNING::WarningType::EXCEPTION_MSG);
            return Enum::eError_Back;
        }
        else {
            if (bridge::nunchukHasSinger(retJson->originSingleSigner())) {
                setSelectFingerPrint(retJson->masterFingerPrint());
                bool ret = canReplaceKey() ? requestKeyReplacement(retJson) : AddOrUpdateReuseKeyToDraftWallet(retJson->originSingleSigner());
                return ret ? Enum::eOK : Enum::eError_Keep_Screen;
            }
            else {
                nunchuk::SingleSigner create = bridge::nunchukCreateOriginSigner(retJson->name(),
                                                                                 retJson->xpub(),
                                                                                 retJson->publickey(),
                                                                                 retJson->derivationPath(),
                                                                                 retJson->masterFingerPrint(),
                                                                                 (nunchuk::SignerType)retJson->signerType(),
                                                                                 retJson->signerTags(),
                                                                                 false,
                                                                                 msg);
                if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                    create.set_tags({nunchuk::SignerTag::COLDCARD});
                    msg.resetWarningMessage();
                    bridge::UpdateRemoteSigner(create, msg);
                    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                        msg.resetWarningMessage();
                        QSingleSignerPtr signer = QSingleSignerPtr(new QSingleSigner(create));
                        AppModel::instance()->setSingleSignerInfo(signer);
                        QSingleSignerListModelPtr remoteSigners = bridge::nunchukGetRemoteSigners();
                        if(remoteSigners){
                            AppModel::instance()->setRemoteSignerList(remoteSigners);
                        }
                        if (signer) {
                            setSelectFingerPrint(signer->masterFingerPrint());
                            bool ret = canReplaceKey() ? requestKeyReplacement(signer) : AddOrUpdateReuseKeyToDraftWallet(create);
                            return ret ? Enum::eOK : Enum::eError_Keep_Screen;
                        }
                    }
                }
            }
        }

    }
    else if (msg.code() == -100){
        AppModel::instance()->showToast(-1, msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
        return Enum::eError_Keep_Screen;
    }
    if((int)EWARNING::WarningType::EXCEPTION_MSG == msg.type() && nunchuk::NunchukException::RUN_OUT_OF_CACHED_XPUB == msg.code()){
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::WARNING_MSG);
        return Enum::eError_Keep_Screen;
    }
    else{
        AppModel::instance()->showToast(msg.code(), "XPUBs file is invalid", EWARNING::WarningType::EXCEPTION_MSG);
        return Enum::eError_Keep_Screen;
    }
}

void QAssistedDraftWallets::setSelectFingerPrint(const QString &selectFingerPrint)
{
    m_selectFingerPrint = selectFingerPrint;
}

QVariantList QAssistedDraftWallets::hardwareReq() const
{
    return m_hardwareReq;
}

void QAssistedDraftWallets::setHardwareReq(const QVariantList &hardwareReq)
{
    if (m_hardwareReq == hardwareReq) return;
    m_hardwareReq = hardwareReq;
    emit hardwareReqChanged();
}

void QAssistedDraftWallets::updateRequestKey()
{
    GetListAllRequestAddKey();
}

QWalletServicesTagPtr QAssistedDraftWallets::servicesTagPtr() const
{
    return m_servicesTag;
}

void QAssistedDraftWallets::MixMasterSignerAndSingleSigner(const QString &tag)
{
    auto masterList = AppModel::instance()->masterSignerListPtr();
    auto remoteList = AppModel::instance()->remoteSignerListPtr();
    DBG_INFO << tag << masterList->rowCount() << remoteList->rowCount();
    QJsonArray arrays;
    auto contains = [&](const QString& xfp)->bool {
        for (auto js : arrays) {
            QJsonObject json = js.toObject();
            QString finger = json["signer_fingerPrint"].toString();
            if (finger == xfp) {
                return true;
            }
        }
        return false;
    };
    for (auto m : masterList->fullList()) {
        if ((m->tag() == tag && (m->signerType() == (int)ENUNCHUCK::SignerType::HARDWARE || m->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP)) ||
            (qUtils::strCompare(tag, "COLDCARD") && m->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC) ||
            qUtils::strCompare(m->tag(), "KEEPKEY")) {
            QJsonObject signer;
            signer["signer_deviceType"] = m->devicePtr()->type();
            signer["signer_type"] = (int)m->signerType();
            signer["signer_name"] = m->name();
            signer["signer_fingerPrint"] = m->fingerPrint();
            signer["signer_tag"] = m->tag();
            signer["signer_id"] = m->id();
            signer["signer_bip32_path"] = m->path();
            signer["signer_is_primary"] = m->isPrimaryKey();
            signer["signer_card_id"] = m->device()->cardId();
            if (!contains(m->fingerPrint())) {
                arrays.append(signer);
            }
        }
    }
    for (auto s : remoteList->fullList()) {
        if ((s->tag() == tag && (s->signerType() == (int)ENUNCHUCK::SignerType::HARDWARE || s->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP)) ||
            (qUtils::strCompare(tag, "COLDCARD") && s->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC) ||
            qUtils::strCompare(s->tag(), "KEEPKEY")) {
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
            if (!contains(s->masterFingerPrint())) {
                arrays.append(signer);
            }
        }
    }
    setSignerExistList(arrays);
}

void QAssistedDraftWallets::finishScanDevices()
{
    if (!typeReq().isEmpty()) {
        auto deviceList = AppModel::instance()->deviceListPtr();
        if (mRefreshDeviceList) {
            mRefreshDeviceList->clearList();
            for (auto device : deviceList->fullList()) {
                if (device->type() == typeReq()) {
                    mRefreshDeviceList->addDevice(device);
                }
            }
            DBG_INFO << m_mode << "size: " << mRefreshDeviceList->rowCount();
        }
        emit deviceListChanged();
    }
    if (auto w = AppModel::instance()->walletInfoPtr()) {
        if (auto dummy = w->groupDummyTxPtr()) {
            dummy->finishScanDevices();
        }
    }
}

QVariantList QAssistedDraftWallets::signerExistList() const
{
    return m_signerExistList.toVariantList();
}

void QAssistedDraftWallets::setSignerExistList(QJsonArray signerExistList)
{
    if (m_signerExistList == signerExistList)
        return;

    m_signerExistList = signerExistList;
    emit signerExistListChanged();
}

void QAssistedDraftWallets::reuseKeyFromMasterSigner(const QString &xfp, const int index)
{
    DBG_INFO << xfp << index;
    QtConcurrent::run([this, xfp, index]() {
        bool ret = getSignerFromMasterSigner(xfp, index);
        emit reuseKeyGetSignerResult(ret ? 1 : 0);
    });
}

void QAssistedDraftWallets::reuseKeyGetSigner(const QString &xfp, const int index)
{
    DBG_INFO << xfp << index;
    QtConcurrent::run([this, xfp, index]() {
        bool ret = getSigner(xfp, index);
        emit reuseKeyGetSignerResult(ret ? 1 : 0);
    });
}

QString QAssistedDraftWallets::bip32path(const QString &xfp, int index)
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

QString QAssistedDraftWallets::reuseKeyXfp(const QString &fileName)
{
    QWarningMessage msg;
    QString file_path = qUtils::QGetFilePath(fileName);
    QSingleSignerPtr retJson = bridge::nunchukParseJSONSigners(file_path, ENUNCHUCK::SignerType::AIRGAP, ENUNCHUCK::AddressType::NATIVE_SEGWIT, msg);
    if(retJson && (int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return retJson->masterFingerPrint();
    }
    return "";
}

StructAddHardware QAssistedDraftWallets::request() const
{
    return m_request;
}
