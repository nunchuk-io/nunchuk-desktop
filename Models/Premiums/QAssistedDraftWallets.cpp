#include "QAssistedDraftWallets.h"
#include "AppModel.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QUserWalletDummyTx.h"
#include "Premiums/QWalletManagement.h"
#include "Premiums/QWalletServicesTag.h"
#include "Servers/Byzantine.h"
#include "Servers/Draco.h"
#include "ServiceSetting.h"
#include "Signers/QSignerManagement.h"
#include "ViewsEnums.h"
#include "localization/STR_CPP.h"
#include "nunchuckiface.h"
#include <QJsonDocument>
#include <QQmlEngine>
#include "Premiums/QGroupWallets.h"
#include "Premiums/QUserWallets.h"

const QMap<Key, StructAddHardware> map_keys = {
    {Key::ADD_LEDGER, {"LEDGER", "ledger", STR_CPP_122, STR_CPP_121, 124}},
    {Key::ADD_TREZOR, {"TREZOR", "trezor", STR_CPP_124, STR_CPP_123, 124}},
    {Key::ADD_COLDCARD, {"COLDCARD", "coldcard", STR_CPP_126, STR_CPP_125, 152}},
    {Key::ADD_BITBOX, {"BITBOX", "bitbox02", STR_CPP_128, STR_CPP_127, 124}},
    {Key::ADD_JADE, {"JADE", "jade", STR_CPP_133, STR_CPP_132, 152}},
};

namespace {

inline QString titleCase(QString str) {
    if (str.length() == 0)
        return "";
    QString tmp = str;
    // if you want to ensure all other letters are lowercase:
    tmp = tmp.toLower();
    tmp[0] = str[0].toUpper();
    return tmp;
}
} // namespace
QAssistedDraftWallets::QAssistedDraftWallets(int mode)
    : QSwitchAPI(mode), mRefreshDeviceList(QDeviceListModelPtr(new DeviceListModel())),
      m_availableSignerListPtr(QSingleSignerListModelPtr(new SingleSignerListModel())) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QAssistedDraftWallets::~QAssistedDraftWallets() {
    mRefreshDeviceList.clear();
}

QString QAssistedDraftWallets::typeReq() const {
    if (getAddHardware() == 0)
        return "";
    return map_keys.value((Key)getAddHardware()).mType;
}

void QAssistedDraftWallets::GetListAllRequestAddKey(const QJsonArray &groups) {
    if (!CLIENT_INSTANCE->isNunchukLoggedIn()) {
        return;
    }
    QPointer<QAssistedDraftWallets> safeThis(this);
    runInThread(
        [safeThis, groups]() -> QMap<Key, StructAddHardware> {
            QMap<Key, StructAddHardware> requests;
            SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
            if (ptrLamda->m_mode == USER_WALLET) {
                QJsonObject data;
                QString error_msg;
                Draco::instance()->assistedWalletGetListKey(data, error_msg);
                requests = ptrLamda->addRequest(data.value("requests").toArray());
                DBG_INFO << data;
            } else if (ptrLamda->m_mode == GROUP_WALLET) {
                QJsonObject data;
                QString error_msg;
                for (QJsonValue js : groups) {
                    QJsonObject group = js.toObject();
                    QString status = group["status"].toString();
                    if (status == "PENDING_WALLET") {
                        QString group_id = group["id"].toString();
                        Byzantine::instance()->GetAllListRequestAddKey(group_id, data, error_msg);
                        QMap<Key, StructAddHardware> tmps = ptrLamda->addRequest(data.value("requests").toArray(), group_id);
                        for (auto tmp = tmps.begin(); tmp != tmps.end(); ++tmp) {
                            requests.insertMulti(tmp.key(), tmp.value());
                        }
                        DBG_INFO << group_id << data;
                    }
                }
            }
            return requests;
        },
        [safeThis](QMap<Key, StructAddHardware> requests) {
            SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
            ptrLamda->m_requests = requests;
            ptrLamda->makeListRequests();
        });
}

void QAssistedDraftWallets::cancelRequestKey(const QString &request_id, const QString &group_id) {
    if (!request_id.isEmpty()) {
        bool ret{false};
        if (group_id.isEmpty()) {
            ret = Draco::instance()->assistedWalletRemoveId(request_id);
        } else {
            ret = Byzantine::instance()->DeleteRequestAddKey(group_id, request_id);
        }
        if (ret) {
            updateRequestKey();
        }
    }
}

bool QAssistedDraftWallets::AddOrUpdateToDraftWallet(nunchuk::SingleSigner hardware) {
    QJsonObject data = progressAddOrReplacementInfo(hardware);
    QJsonObject currentSignerInfo = QSignerManagement::instance()->currentSignerJs();
    QString group_id = currentSignerInfo.value("group_id").toString();
    QString request_id = currentSignerInfo.value("request_id").toString();

    bool ret{false};
    QJsonObject output;
    auto dash = QGroupWallets::instance()->GetDashboard(group_id);
    if (group_id.isEmpty()) {
        ret = Draco::instance()->assistedWalletAddKey(request_id, data, output);
    } else {
        if (dash && dash->isUserDraftWallet()) {
            ret = Draco::instance()->assistedWalletAddKey(request_id, data, output);
        } else {
            ret = Byzantine::instance()->DraftWalletAddKey(group_id, request_id, data, output);
        }
        if (ret) {
            if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
                dashboard->GetAlertsInfo();
                dashboard->GetHealthCheckInfo();
            }
        }
    }
    DBG_INFO << "data:" << data << "output:" << output;
    QString message = output["message"].toString();
    if (message.contains("Duplicate")) {
        emit addHardwareAlert();
    }
    return ret;
}

QMap<Key, StructAddHardware> QAssistedDraftWallets::addRequest(const QJsonArray &requests, const QString &group_id) {
    DBG_INFO << requests;
    QMap<Key, StructAddHardware> reqs;
    for (QJsonValue request : requests) {
        QJsonObject requestObj = request.toObject();
        QString status = requestObj.value("status").toString();
        QJsonArray tags = requestObj.value("tags").toArray();
        QString request_id = requestObj.value("id").toString();
        int key_index = requestObj.value("key_index").toInt(-1);
        if (status == "PENDING") {
            for (Key key : map_keys.uniqueKeys()) {
                StructAddHardware hardware = map_keys.value(key);
                if (tags.contains(hardware.mTag) && status == "PENDING") {
                    hardware.mGroupId = group_id;
                    hardware.mRequestId = request_id;
                    hardware.mKeyIndex = key_index;
                    hardware.mTags.clear();
                    for (auto tag : tags) {
                        hardware.mTags.append(tag.toString());
                    }
                    hardware.mConfig = requestObj;
                    reqs.insert(key, hardware);
                }
            }
        }
    }
    return reqs;
}

void QAssistedDraftWallets::makeListRequests() {
    QVariantList hardwares;
    DBG_INFO << m_requests.size();
    for (auto i = m_requests.cbegin(), end = m_requests.cend(); i != end; ++i) {
        StructAddHardware hardware = i.value();
        Key key = i.key();
        QMap<QString, QVariant> maps = hardware.mConfig.toVariantMap();
        maps["hwType"] = (int)key;
        maps["textBtn"] = hardware.mTextBtn;
        maps["title"] = hardware.mTitle;
        maps["mHeight"] = hardware.mHeight;
        maps["group_id"] = hardware.mGroupId;
        maps["request_id"] = hardware.mRequestId;
        maps["is_inheritance"] = hardware.mTag.contains("INHERITANCE");
        DBG_INFO << maps;
        hardwares.append(QVariant::fromValue(maps));
    }
    setHardwareReq(hardwares);
}

void QAssistedDraftWallets::reset() {
    m_requests.clear();
    makeListRequests();
}

DeviceListModel *QAssistedDraftWallets::refreshDeviceList() const {
    return mRefreshDeviceList.data();
}

void QAssistedDraftWallets::addHardwareFromConfig(int hardwareType, const QString &group_id, int key_index, bool isIh) {
    DBG_INFO << hardwareType << group_id << key_index;
    QSwitchAPI::setAddHardware(hardwareType);
    auto request = map_keys.value((Key)hardwareType);
    m_request = request;
    m_request.mGroupId = group_id;
    m_request.mRequestId = "";
    m_request.mKeyIndex = key_index;
    QJsonObject currentSignerInfo = QSignerManagement::instance()->currentSignerJs();
    currentSignerInfo["hwType"] = hardwareType;

    bool is_inheritance = currentSignerInfo.value("is_inheritance").toBool();
    QJsonArray tags = currentSignerInfo.value("tags").toArray();
    if (tags.isEmpty() && !request.mTag.isEmpty()) {
        tags.append(request.mTag);
        if (is_inheritance) {
            tags.append("INHERITANCE");
        }
        currentSignerInfo["tags"] = tags;
    }

    QSignerManagement::instance()->setCurrentSigner(currentSignerInfo);
}

void QAssistedDraftWallets::addHardwareFromBanner(const QString &request_id) {
    for (int i = 0; i < m_hardwareReq.size(); ++i) {
        auto info = QJsonValue::fromVariant(m_hardwareReq.at(i)).toObject();
        auto key = static_cast<Key>(info.value("hwType").toInt());
        StructAddHardware hardware = m_requests.value(key);
        if (qUtils::strCompare(hardware.mRequestId, request_id)) {
            QSwitchAPI::setAddHardware((int)key);
            m_request = hardware;
            QJsonObject keyInfo = info.value("key").toObject();
            QJsonArray key_indices = info.value("key_indices").toArray();
            if (!keyInfo.isEmpty()) {
                info["key_index"] = key_indices.last().toInt(-1);
                info["has"] = true;
                info["new_account_index"] = 1;
            } else {
                info["key_index"] = key_indices.first().toInt(-1);
                info["has"] = false;
                info["new_account_index"] = 0;
            }
            QString magic = info.value("magic").toString();
            info["onlyUseForClaim"] = magic != "";
            QSignerManagement::instance()->setCurrentSigner(info);
            QString wallet_type = info.value("wallet_type").toString();
            ServiceSetting::instance()->CreateAssignAvailableSigners(wallet_type);
        }
    }
}

int QAssistedDraftWallets::reuseKeyGetCurrentIndex(const QString &xfp) {
    QWarningMessage msg;
    int ret = bridge::nunchukGetLastUsedSignerIndex(xfp, walletType(), ENUNCHUCK::AddressType::NATIVE_SEGWIT, msg);
    if ((int)EWARNING::WarningType::NONE_MSG != msg.type()) {
        DBG_INFO << msg.code() << msg.what();
    }
    return ret;
}

bool QAssistedDraftWallets::checkAndGetSingleSigner(const QString &xfp, int index, nunchuk::SingleSigner &outSigner) {
    DBG_INFO << "xfp: " << xfp << "index: " << index;
    QWarningMessage msg;
    int tmpIndex = index < 0 ? 0 : index;
    nunchuk::SingleSigner ret = bridge::nunchukGetOriginSingleSigner(xfp, walletType(), ENUNCHUCK::AddressType::NATIVE_SEGWIT, tmpIndex, msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        outSigner = ret;
        return true;
    } else {
        msg.resetWarningMessage();
        ret = bridge::nunchukGetOriginSignerFromMasterSigner(xfp, walletType(), ENUNCHUCK::AddressType::NATIVE_SEGWIT, tmpIndex, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            outSigner = ret;
            return true;
        }
    }
    if ((int)EWARNING::WarningType::EXCEPTION_MSG == msg.type() && nunchuk::NunchukException::RUN_OUT_OF_CACHED_XPUB == msg.code()) {
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::WARNING_MSG);
        return false;
    } else {
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
        return false;
    }
}

QJsonObject QAssistedDraftWallets::progressAddOrReplacementInfo(nunchuk::SingleSigner hardware) {
    QString xfp = hardware.get_master_fingerprint().empty() ? QString::fromStdString(hardware.get_master_signer_id()) : QString::fromStdString(hardware.get_master_fingerprint());
    QJsonObject currentSignerInfo = QSignerManagement::instance()->currentSignerJs();
    QString selectXfp = currentSignerInfo.value("xfp").toString();
    QString group_id = currentSignerInfo.value("group_id").toString();
    DBG_INFO << selectXfp << "|" << xfp << "|" << group_id << "|" << currentSignerInfo;
    if (xfp.isEmpty()) {
        resultAddOrUpdateAKeyToDraftWallet("eSCREEN_ERROR");
        return {};
    }
    QJsonObject data;
    QJsonArray tags = currentSignerInfo.value("tags").toArray();
    QString tag = "";
    for (auto t : tags) {
        if (!qUtils::strCompare(t.toString(), "INHERITANCE")) {
            tag = t.toString();
        }
    }

    QString name = QString::fromStdString(hardware.get_name());
    auto dash = QGroupWallets::instance()->GetDashboard(group_id);
    QString finalName = dash.isNull() ? (name != "" ? name : titleCase(tag)) : dash->generateName({tag});
    bool is_inheritance = currentSignerInfo.value("is_inheritance").toBool();
    if (is_inheritance && !finalName.contains("(inh.)")) {
        finalName += "(inh.)";
    }
    data["name"] = finalName;
    data["xfp"] = xfp;
    data["derivation_path"] = QString::fromStdString(hardware.get_derivation_path());
    data["xpub"] = QString::fromStdString(hardware.get_xpub());
    data["pubkey"] = QString::fromStdString(hardware.get_public_key());
    data["type"] = qUtils::GetSignerTypeString(hardware.get_type());
    data["tags"] = tags;
    data["tapsigner"] = {};

    int key_index = currentSignerInfo.value("key_index").toInt(-1);
    if (key_index >= 0) {
        data["key_index"] = key_index;
    } else {
        data["key_index"] = {};
    }
    return data;
}

bool QAssistedDraftWallets::requestAddOrReplacementToServer(nunchuk::SingleSigner signer) {
    DBG_INFO << signer.get_master_fingerprint() << "tag:" << signer.get_tags();
    if (!CLIENT_INSTANCE->isNunchukLoggedIn()) {
        return false;
    }
    return canReplaceKey() ? requestKeyReplacement(signer) : AddOrUpdateToDraftWallet(signer);
}

bool QAssistedDraftWallets::canReplaceKey() {
    auto dashboard = AppModel::instance()->walletInfoPtr()->dashboard();
    if (dashboard && dashboard->canReplaceKey()) {
        return true;
    } else {
        return false;
    }
}

void QAssistedDraftWallets::resultAddOrUpdateAKeyToDraftWallet(const QString &result, bool resetFlow) {
    if (resetFlow) {
        QSignerManagement::instance()->clearState();
    }
    QJsonObject currentSigerInfo = QSignerManagement::instance()->currentSignerJs();
    QString xfp = currentSigerInfo.value("xfp").toString();
    bool isColdcardNFC = false;
    if (!m_availableSignerListPtr.isNull()) {
        auto signer = m_availableSignerListPtr->getSingleSignerByFingerPrint(xfp);
        isColdcardNFC = signer && signer->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC;
    }
    DBG_INFO << result << isColdcardNFC;
    if (isColdcardNFC) {
        if (result == "eSCREEN_SUCCESS") {
            if (canReplaceKey()) {
                QSignerManagement::instance()->setScreenFlow(result);
            }
            else {
                QGroupWallets::instance()->refresh();
                if(auto dash = QGroupWallets::instance()->dashboardInfoPtr()) {
                    dash->requestShowLetAddYourKeys();
                }                
            }
        } else {
            QSignerManagement::instance()->setScreenFlow(result);
        }
    } else {
        QSignerManagement::instance()->setScreenFlow(result);
    }
}

bool QAssistedDraftWallets::requestKeyReplacement(nunchuk::SingleSigner hardware) {
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        QJsonObject data = progressAddOrReplacementInfo(hardware);
        DBG_INFO << data;
        bool ret = dashboard->FinishKeyReplacement(data);
        return ret;
    }
    return false;
}

QAssistedDraftWallets::ImportColdcard_t QAssistedDraftWallets::ImportColdcardViaFile(const QString &fileName, int new_index) {
    // Preserve old behavior by delegating based on canReplaceKey() 
    DBG_INFO << "canReplaceKey:" << canReplaceKey() << "fileName:" << fileName << "new_index: " << new_index;
    using Enum = QAssistedDraftWallets::ImportColdcard_t;
    QWarningMessage msg;
    QString file_path = qUtils::QGetFilePath(fileName);
    QSingleSignerPtr retJson = bridge::nunchukParseJSONSigners(file_path, ENUNCHUCK::SignerType::AIRGAP, ENUNCHUCK::AddressType::NATIVE_SEGWIT, msg);
    if (retJson && (int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        msg.resetWarningMessage();
        int current_index = reuseKeyGetCurrentIndex(retJson->masterFingerPrint());
        int account_index = qUtils::GetIndexFromPath(retJson->derivationPath());
        if (new_index != -1 && current_index != new_index && new_index == account_index) {
            bool ret = requestAddOrReplacementToServer(retJson->originSingleSigner());
            return ret ? Enum::eOK : Enum::eError_Keep_Screen;
        } else if (new_index != -1 && current_index != new_index && new_index != account_index) {
            QString message = QString("The account imported does not match the account that you entered (%1). Please try again.").arg(new_index);
            AppModel::instance()->showToast(-1, message, EWARNING::WarningType::EXCEPTION_MSG);
            return Enum::eError_Back;
        } else {
            if (bridge::nunchukHasSinger(retJson->originSingleSigner())) {
                bool ret = requestAddOrReplacementToServer(retJson->originSingleSigner());
                return ret ? Enum::eOK : Enum::eError_Keep_Screen;
            } else {
                nunchuk::SingleSigner create = bridge::nunchukCreateOriginSigner(retJson->name(), retJson->xpub(), retJson->publickey(),
                                                                                 retJson->derivationPath(), retJson->masterFingerPrint(),
                                                                                 (nunchuk::SignerType)retJson->signerType(), retJson->signerTags(), false, msg);
                if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                    create.set_tags({nunchuk::SignerTag::COLDCARD});
                    msg.resetWarningMessage();
                    bridge::UpdateRemoteSigner(create, msg);
                    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                        msg.resetWarningMessage();
                        QSingleSignerPtr signer = QSingleSignerPtr(new QSingleSigner(create));
                        AppModel::instance()->setSingleSignerInfo(signer);
                        QSingleSignerListModelPtr remoteSigners = bridge::nunchukGetRemoteSigners();
                        if (remoteSigners) {
                            AppModel::instance()->setRemoteSignerList(remoteSigners);
                        }
                        if (signer) {
                            bool ret = requestAddOrReplacementToServer(create);
                            return ret ? Enum::eOK : Enum::eError_Keep_Screen;
                        }
                    }
                }
            }
        }
    } else if (msg.code() == -100) {
        AppModel::instance()->showToast(-1, msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
        return Enum::eError_Keep_Screen;
    }
    if ((int)EWARNING::WarningType::EXCEPTION_MSG == msg.type() && nunchuk::NunchukException::RUN_OUT_OF_CACHED_XPUB == msg.code()) {
        AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::WARNING_MSG);
        return Enum::eError_Keep_Screen;
    } else {
        AppModel::instance()->showToast(msg.code(), "XPUBs file is invalid", EWARNING::WarningType::EXCEPTION_MSG);
        return Enum::eError_Keep_Screen;
    }
}

QVariantList QAssistedDraftWallets::hardwareReq() const {
    return m_hardwareReq;
}

void QAssistedDraftWallets::setHardwareReq(const QVariantList &hardwareReq) {
    if (m_hardwareReq == hardwareReq)
        return;
    m_hardwareReq = hardwareReq;
    emit hardwareReqChanged();
}

void QAssistedDraftWallets::updateRequestKey() {
    GetListAllRequestAddKey();
}

void QAssistedDraftWallets::createAssignAvailableSigners(const QString &wallet_type) {
    nunchuk::WalletType wtype = nunchuk::WalletType::MULTI_SIG;
    if (qUtils::strCompare(wallet_type, "MINISCRIPT")) {
        wtype = nunchuk::WalletType::MINISCRIPT;
    }
    QPointer<QAssistedDraftWallets> safeThis(this);
    bridge::CreateAssignAvailableSigners(nunchuk::AddressType::NATIVE_SEGWIT, wtype, [safeThis](const QSingleSignerListModelPtr &available_signers) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        ptrLamda->m_availableSignerListPtr = available_signers;
    });
}

QWalletServicesTagPtr QAssistedDraftWallets::servicesTagPtr() const {
    return QWalletServicesTag::instance();
}

void QAssistedDraftWallets::MixMasterSignerAndSingleSigner(const QString &tag) {
    if (m_availableSignerListPtr.isNull()) {
        DBG_ERROR << "availableSignerListPtr is null";
        return;
    }
    DBG_INFO << "tag: " << tag << "size: " << m_availableSignerListPtr->rowCount();
    QVariantList arrays;
    for (auto s : m_availableSignerListPtr->fullList()) {
        DBG_INFO << s->tag() << s->signerType();
        if ((s->tag() == tag && (s->signerType() == (int)ENUNCHUCK::SignerType::HARDWARE || s->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP)) ||
            (qUtils::strCompare(tag, "COLDCARD") && s->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC) || qUtils::strCompare(s->tag(), "KEEPKEY")) {
            auto signer = SingleSignerListModel::useQml(s);
            arrays.append(signer);
        }
    }
    setSignerExistList(arrays);
}

void QAssistedDraftWallets::MixMasterSignerAndSingleSignerMiniscript(const QString &tag) {
    if (m_availableSignerListPtr.isNull()) {
        DBG_ERROR << "availableSignerListPtr is null";
        return;
    }    
    QJsonObject currentSigerInfo = QSignerManagement::instance()->currentSignerJs();
    QString wallet_type = currentSigerInfo.value("wallet_type").toString();
    bool has = currentSigerInfo.value("has").toBool();
    bool hasSecond = currentSigerInfo.value("hasSecond").toBool();
    bool notAllowDisplayKeySameXfp = currentSigerInfo.value("notAllowDisplayKeySameXfp").toBool();
    
    QVariantList arrays;
    for (auto s : m_availableSignerListPtr->fullList()) {
        DBG_INFO << s->tag() << s->signerType();
        if ((s->tag() == tag && (s->signerType() == (int)ENUNCHUCK::SignerType::HARDWARE || s->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP)) ||
            (qUtils::strCompare(tag, "COLDCARD") && s->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC) || qUtils::strCompare(s->tag(), "KEEPKEY")) {
            auto signer = SingleSignerListModel::useQml(s);
            auto maps = signer.toMap();
            maps["wallet_type"] = wallet_type;
            signer = QVariant::fromValue(maps);
            if (notAllowDisplayKeySameXfp) {
                QString xfp = currentSigerInfo.value("xfp").toString();
                if (qUtils::strCompare(s->masterFingerPrint(), xfp)) {
                    continue;
                }
            }
            if (!has) {
                int account_index = qUtils::GetIndexFromPath(s->derivationPath());
                if (account_index == 0) {
                    arrays.append(signer);
                }
            } else if (has && !hasSecond) {
                int account_index = qUtils::GetIndexFromPath(s->derivationPath());
                if (account_index == 1) {
                    arrays.append(signer);
                }
            }
        }
    }
    DBG_INFO << "tag: " << tag << "size: " << m_availableSignerListPtr->rowCount() << "filtered size: " << arrays.size();
    setSignerExistList(arrays);
}

void QAssistedDraftWallets::finishScanDevices() {
    if (!typeReq().isEmpty()) {
        auto deviceList = AppModel::instance()->deviceListPtr();
        if (mRefreshDeviceList) {
            mRefreshDeviceList->clearList();
            for (auto device : deviceList->fullList()) {
                if (device->type() == typeReq()) {
                    mRefreshDeviceList->addDevice(device);
                }
            }
            DBG_INFO << m_mode << typeReq() << "size: " << mRefreshDeviceList->rowCount();
        }
        emit deviceListChanged();
    }
    if (auto w = AppModel::instance()->walletInfoPtr()) {
        if (auto dummy = w->groupDummyTxPtr()) {
            dummy->finishScanDevices();
        }
    }
}

QVariantList QAssistedDraftWallets::signerExistList() const {
    return m_signerExistList;
}

void QAssistedDraftWallets::setSignerExistList(QVariantList signerExistList) {
    if (m_signerExistList == signerExistList)
        return;

    m_signerExistList = signerExistList;
    emit signerExistListChanged();
}

bool QAssistedDraftWallets::requestAddOrReplacementWithIndex(const QString &xfp, int index) {
    DBG_INFO << "xfp: " << xfp << "index: " << index;
    nunchuk::SingleSigner outSigner;
    bool ret = checkAndGetSingleSigner(xfp, index, outSigner);
    if (ret) {
        ret = requestAddOrReplacementToServer(outSigner);
    }
    QList<uint> states = QEventProcessor::instance()->getCurrentStates();
    if(!states.isEmpty() && states.last() == (uint)E::STATE_ID_SCR_ADD_HARDWARE_EXIST)
    {
        resultAddOrUpdateAKeyToDraftWallet(ret ? "eSCREEN_SUCCESS" : "eSCREEN_REFRESH_DEVICE");
    } else {
        resultAddOrUpdateAKeyToDraftWallet(ret ? "eSCREEN_SUCCESS" : "eSCREEN_ERROR");
    }
    return ret;
}

void QAssistedDraftWallets::requestAddOrReplacementWithIndexAsync(const QString &xfp, int index) {
    DBG_INFO << "xfp: " << xfp << "index: " << index;
    newAccountIndexCached(xfp, index);
    QtConcurrent::run([this, xfp, index]() {
        requestAddOrReplacementWithIndex(xfp, index);
    });
}

QString QAssistedDraftWallets::bip32path(const QString &xfp, int index) {
    DBG_INFO << xfp << index;
    nunchuk::SingleSigner outSigner;
    bool ret = checkAndGetSingleSigner(xfp, index, outSigner);
    if (ret) {
        return QString::fromStdString(outSigner.get_derivation_path());
    }
    return "";
}

QString QAssistedDraftWallets::reuseKeyXfp(const QString &fileName) {
    QWarningMessage msg;
    QString file_path = qUtils::QGetFilePath(fileName);
    QSingleSignerPtr retJson = bridge::nunchukParseJSONSigners(file_path, ENUNCHUCK::SignerType::AIRGAP, ENUNCHUCK::AddressType::NATIVE_SEGWIT, msg);
    if (retJson && (int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        return retJson->masterFingerPrint();
    }
    return "";
}

StructAddHardware QAssistedDraftWallets::request() const {
    return m_request;
}

ENUNCHUCK::WalletType QAssistedDraftWallets::walletType() const {
    QJsonObject currentSigerInfo = QSignerManagement::instance()->currentSignerJs();
    QString wallet_type = currentSigerInfo.value("wallet_type").toString();
    if (qUtils::strCompare(wallet_type, "MINISCRIPT")) {
        return ENUNCHUCK::WalletType::MINISCRIPT;
    } else {
        return ENUNCHUCK::WalletType::MULTI_SIG;
    }
}

void QAssistedDraftWallets::requestVerifySingleSignerViaConnectDevice(const int index, const QString &verifyType) {
    
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    runInConcurrent([this, verifyType, index]() ->bool{
        auto currentSigerInfo = QSignerManagement::instance()->currentSignerJs();
        auto xfpSelected = currentSigerInfo.value("xfp").toString();
        DBG_INFO << "index: " << index << "verifyType: " << verifyType << "currentSigerInfo: " << currentSigerInfo;
        auto deviceList = refreshDeviceList();
        if (deviceList) {
            auto device = deviceList->getDeviceByIndex(qMax(0, index));
            if (device) {
                QWarningMessage msg;
                auto derivation_path = currentSigerInfo.value("derivation_path").toString();
                auto signer = bridge::nunchukGetSignerFromMasterSigner(xfpSelected, derivation_path, msg);
                if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                    msg.resetWarningMessage();
                    bridge::nunchukVerifySingleSigner(device, signer, msg);
                }
                if ((int)EWARNING::WarningType::NONE_MSG != msg.type()) {
                    AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
                } else {
                    requestVerifySingleSigner(verifyType);
                }
            }
        }
        return true;
    },[](bool ret) {
        qApp->restoreOverrideCursor();
    });
}

void QAssistedDraftWallets::requestVerifySingleSignerViaQR(const QStringList &qr_data, const QString &verifyType) {
    DBG_INFO << "verifyType: " << verifyType << "qr_data size: " << qr_data.size();
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    runInConcurrent([this, qr_data, verifyType]() ->bool{
        QWarningMessage msg;
        nunchuk::SingleSigner qrSigner = bridge::nunchukParseQRSigners(qr_data, 0, msg);
        auto xfpSelected = QSignerManagement::instance()->currentSignerJs().value("xfp").toString();
        auto derivation_path = QSignerManagement::instance()->currentSignerJs().value("derivation_path").toString();
        auto signer = bridge::nunchukGetOriginSingleSigner(xfpSelected, walletType(), ENUNCHUCK::AddressType::NATIVE_SEGWIT, 0, msg);
        if (signer.get_descriptor() != qrSigner.get_descriptor()) {
            AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
        } else {
            requestVerifySingleSigner(verifyType);
        }        
        return true;
    },[](bool ret) {
        qApp->restoreOverrideCursor();
    });
}

void QAssistedDraftWallets::requestVerifySingleSignerViaFile(const QString &fileName, const QString &verifyType) {
    DBG_INFO << "verifyType: " << verifyType << "fileName: " << fileName;
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    runInConcurrent([this, fileName, verifyType]() ->bool{
        QWarningMessage msg;
        QString file_path = qUtils::QGetFilePath(fileName);
        nunchuk::SingleSigner fileSigner = bridge::nunchukParseJSONSigners(file_path, 0, nunchuk::SignerType::AIRGAP, msg);
        auto xfpSelected = QSignerManagement::instance()->currentSignerJs().value("xfp").toString();
        auto derivation_path = QSignerManagement::instance()->currentSignerJs().value("derivation_path").toString();
        auto signer = bridge::nunchukGetOriginSingleSigner(xfpSelected, walletType(), ENUNCHUCK::AddressType::NATIVE_SEGWIT, 0, msg);
        if (signer.get_descriptor() != fileSigner.get_descriptor()) {
            AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
        } else {
            requestVerifySingleSigner(verifyType);
        }        
        return true;
    },[](bool ret) {
        qApp->restoreOverrideCursor();
    });
}

bool QAssistedDraftWallets::requestVerifySingleSigner(const QString &verifyType) {
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        if (dashboard->canReplaceKey()) {
            return replacementVerifySingleSigner(verifyType);
        } else {
            return addVerifySingleSigner(verifyType);
        }
    }
    return false;
}

bool QAssistedDraftWallets::addVerifySingleSigner(const QString &verifyType) {
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        QString error_msg;
        auto xfpSelected = QSignerManagement::instance()->currentSignerJs().value("xfp").toString();
        bool ret{false};
        if (dashboard->isUserWallet() || dashboard->isUserDraftWallet()) {
            ret = Draco::instance()->DraftWalletSignerVerify(xfpSelected, verifyType, error_msg);
        } else {
            ret = Byzantine::instance()->DraftWalletSignerVerify(dashboard->groupId(), xfpSelected, verifyType, error_msg);
        }
        DBG_INFO << "xfpSelected: " << xfpSelected << "verifyType: " << verifyType << "ret: " << ret << "error_msg: " << error_msg;
        if (ret) {
            emit verifySingleSignerResult(1);
        }
        return ret;
    }
    return false;
}

bool QAssistedDraftWallets::replacementVerifySingleSigner(const QString &verifyType) {
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        QJsonObject result;
        auto xfpSelected = QSignerManagement::instance()->currentSignerJs().value("xfp").toString();
        bool ret{false};
        if (dashboard->isUserWallet() || dashboard->isUserDraftWallet()) {
            ret = Draco::instance()->VerifyKeyReplacement(dashboard->wallet_id(), xfpSelected, verifyType, servicesTagPtr()->passwordToken(), result);
        } else {
            ret = Byzantine::instance()->VerifyKeyReplacement(dashboard->groupId(), dashboard->wallet_id(), xfpSelected, verifyType, servicesTagPtr()->passwordToken(), result);
        }
        DBG_INFO << "xfpSelected: " << xfpSelected << "verifyType: " << verifyType << "result: " << result;
        if (ret) {
            emit verifySingleSignerResult(1);
        }
        return ret;
    }
    return false;
}

bool QAssistedDraftWallets::requestRemoveSingleSigner(const QString &xfp) {
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        QJsonObject result;
        auto xfpSelected = QSignerManagement::instance()->currentSignerJs().value("xfp").toString();
        bool ret{false};
        if (dashboard->isUserWallet() || dashboard->isUserDraftWallet()) {
            ret = Draco::instance()->RemoveKeyReplacement(dashboard->wallet_id(), xfpSelected, servicesTagPtr()->passwordToken(), result);
        } else {
            ret = Byzantine::instance()->RemoveKeyReplacement(dashboard->groupId(), dashboard->wallet_id(), xfpSelected, servicesTagPtr()->passwordToken(), result);
        }
        if (ret) {
            // QString 
            // emit removeSingleSignerResult(1);
        }
        return ret;
    }
    return false;
}

void QAssistedDraftWallets::newAccountIndexCached(const QString &xfp, int index) {
    auto currentSigerInfo = QSignerManagement::instance()->currentSignerJs();
    currentSigerInfo["new_account_index"] = index;
    QSignerManagement::instance()->setCurrentSigner(currentSigerInfo);
    
}

bool QAssistedDraftWallets::requestQRAddOrReplacementWithIndexAsync(const QStringList &qr_data, int index) {
    DBG_INFO << "index: " << index << "qr_data size: " << qr_data.size();
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    runInConcurrent([this, index, qr_data]() ->bool{
        QWarningMessage msg;
        nunchuk::SingleSigner qrSigner = bridge::nunchukParseQRSigners(qr_data, index, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            bool ret = requestAddOrReplacementToServer(qrSigner);
            QList<uint> states = QEventProcessor::instance()->getCurrentStates();
            if(!states.isEmpty() && states.last() == (uint)E::STATE_ID_SCR_ADD_HARDWARE_EXIST)
            {
                resultAddOrUpdateAKeyToDraftWallet(ret ? "eSCREEN_SUCCESS" : "eSCREEN_REFRESH_DEVICE");
            } else {
                resultAddOrUpdateAKeyToDraftWallet(ret ? "eSCREEN_SUCCESS" : "eSCREEN_ERROR");
            }
            return ret;
        } else {
            AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
        return false;  
    },[](bool ret) {
        qApp->restoreOverrideCursor();
    });
    return false;    
}

bool QAssistedDraftWallets::requestImportFileAddOrReplacementWithIndexAsync(const QString &fileName, int index) {
    DBG_INFO << "index: " << index << "fileName: " << fileName;
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    runInConcurrent([this, index, fileName]() ->bool{
        auto ret = ImportColdcardViaFile(fileName, index);
        using Enum = QAssistedDraftWallets::ImportColdcard_t;
        if (ret == Enum::eOK) {
            resultAddOrUpdateAKeyToDraftWallet("eSCREEN_SUCCESS");
            if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
                dashboard->GetAlertsInfo();
            }
        }
        else if (ret == Enum::eError_Keep_Screen) {
            // Don't need emit
        }
        else if (ret == Enum::eError_Back) {
            resultAddOrUpdateAKeyToDraftWallet("eSCREEN_INPUT_INDEX");
        }
        return ret == Enum::eOK;
    },[](bool ret) {
        qApp->restoreOverrideCursor();
    });
    return false;
}

bool QAssistedDraftWallets::requestAddOrReplacementBothIndicesIfPossible(const QString &xfp) {
    QJsonObject currentSignerInfo = QSignerManagement::instance()->currentSignerJs();
    const QString wallet_type = currentSignerInfo.value("wallet_type").toString();
    const bool isMiniscript = qUtils::strCompare(wallet_type, "MINISCRIPT");

    auto handleMiniscript = [&, this]() -> bool {
        bool has = currentSignerInfo.value("has").toBool();
        bool hasSecond = currentSignerInfo.value("hasSecond").toBool();
        // First time: need indices 0 and 1
        if (!has) {
            if (!requestAddOrReplacementWithIndex(xfp, 0)) {
                return false;
            }
            currentSignerInfo["has"] = true;
            currentSignerInfo["hasSecond"] = false;
            currentSignerInfo["key_index"] = currentSignerInfo.value("key_index").toInt(-1) + 1;
            currentSignerInfo["new_account_index"] = 1;
            QSignerManagement::instance()->setCurrentSigner(currentSignerInfo);
            return requestAddOrReplacementWithIndex(xfp, 1);
        }

        // Second pending: add index 1 (or provided newIndex)
        if (has && !hasSecond) {
            return requestAddOrReplacementWithIndex(xfp, 1);
        }

        // Already have both, nothing to do
        return true;
    };

    auto handleNormal = [&, this]() -> bool {
        int newIndex = currentSignerInfo.value("new_account_index").toInt(-1);
        return requestAddOrReplacementWithIndex(xfp, newIndex);
    };

    return isMiniscript ? handleMiniscript() : handleNormal();
}

bool QAssistedDraftWallets::requestAddOrReplacementBothIndicesIfPossibleAsync(const QString &xfp) {
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    runInConcurrent([this, xfp]() ->bool{
        bool ret = requestAddOrReplacementBothIndicesIfPossible(xfp);
        return ret;
    },[](bool ret) {
        qApp->restoreOverrideCursor();
    });
    return false;
}