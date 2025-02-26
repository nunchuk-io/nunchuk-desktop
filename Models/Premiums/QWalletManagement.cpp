#include "QWalletManagement.h"
#include <QQmlEngine>
#include <QJsonDocument>
#include "localization/STR_CPP.h"
#include "ServiceSetting.h"
#include "nunchuckiface.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "AppSetting.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QUserWallets.h"
#include "QWalletServicesTag.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QWalletDummyTx.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QUserWalletDummyTx.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QRecurringPayment.h"
#include "utils/enumconverter.hpp"

QWalletManagement *QWalletManagement::instance()
{
    static QWalletManagement mInstance;
    return &mInstance;
}

QWalletManagement::QWalletManagement()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(this, &QWalletManagement::getListWalletFinish, this, &QWalletManagement::slotGetListWalletFinish, Qt::QueuedConnection);
    connect(this, &QWalletManagement::signalUpdateSigner, this, &QWalletManagement::slotUpdateSigner, Qt::QueuedConnection);
}

QWalletManagement::~QWalletManagement()
{
    mActivedWallets.clear();
    mServerKeys.clear();
    mInheritancePlans.clear();
}

void QWalletManagement::GetListWallet(int mode)
{
    QJsonObject data;
    QString error_msg;
    bool ret {false};
    if (mode == USER_WALLET) {
        ret = Draco::instance()->getAssistedWallets(data, error_msg);
    }
    else {
        ret = Byzantine::instance()->GetListGroupWallets(data, error_msg);
    }
    if (ret == false){
        return;
    }
    if(!data.isEmpty()){
#if 0 // FIXME REMOVE GROUP CACHED
        if(!mWallets.isEmpty()){
            mWallets.clear();
        }
        if(!mWalletsInfo.isEmpty()){
            mWalletsInfo.clear();
        }
#endif
        QJsonArray _wallets = data["wallets"].toArray();
        walletArray = _wallets;
        QMap<QString, bool> is_newkey;
        for(QJsonValue wallet_json : _wallets){
            QJsonObject wallet_obj = wallet_json.toObject();
            QString wallet_id   = wallet_obj["local_id"].toString();
            QString group_id    = wallet_obj["group_id"].toString();
            QString status      = wallet_obj["status"].toString();
            if (status == "ACTIVE" || status == "LOCKED" || status == "REPLACED") {
                mWallets.insert(wallet_id, group_id);
                mWalletsInfo.insert(wallet_id, wallet_obj);
            }
        }
#if 0 // FIXME REMOVE GROUP CACHED
        if (auto walletList = AppModel::instance()->walletListPtr()) {
            DBG_INFO << mWallets;
            for(auto w : walletList->fullList()) {
                if(w && !mWallets.contains(w.data()->id())){
                    AppSetting::instance()->deleteWalletCached(w.data()->id());
                    w.data()->groupInfoChanged();
                }
            }
        }
#endif
        emit getListWalletFinish();
        for(QJsonValue wallet_json : _wallets){
            QJsonObject wallet_obj = wallet_json.toObject();
            QString wallet_id   = wallet_obj["local_id"].toString();
            QString group_id    = wallet_obj["group_id"].toString();
            QString status      = wallet_obj["status"].toString();
            QString slug        = wallet_obj["slug"].toString();
            if (status == "ACTIVE" || status == "LOCKED" || status == "REPLACED") {
                QString myRole = "";
                if(mode == GROUP_WALLET && group_id != "" && status != "REPLACED"){
                    QJsonObject output;
                    QString error_msg = "";
                    bool ret = Byzantine::instance()->GetOneGroupWallets(group_id, output, error_msg);
                    if (ret) {
                        QGroupDashboardPtr dashboard = QGroupDashboardPtr(new QGroupDashboard(""));
                        dashboard->setGroupInfo(output["group"].toObject());
                        myRole = dashboard->myRole();
                        if(!dashboard.data()->accepted()){
                            continue;
                        }
                    }
                }
                QWalletCached<QString, QString, QString, QString> cachedData;
                cachedData.first  = group_id;
                cachedData.second = slug;
                cachedData.third  = myRole;
                cachedData.fourth = status;
                AppSetting::instance()->setWalletCached(wallet_id, cachedData);

                QString wallet_name = wallet_obj["name"].toString();
                QString wallet_description = wallet_obj["description"].toString();
                if(!bridge::nunchukHasWallet(wallet_id)){
                    QJsonArray signers = wallet_obj["signers"].toArray();
                    for(QJsonValue signer_json : signers){
                        QJsonObject signer_obj  = signer_json.toObject();
                        QJsonObject tapsigner   = signer_obj["tapsigner"].toObject();
                        QString signer_name     = signer_obj["name"].toString();
                        QString signer_xfp      = signer_obj["xfp"].toString();
                        QString signer_xpub     = signer_obj["xpub"].toString();
                        QString signer_pubkey   = signer_obj["pubkey"].toString();
                        QString signer_derivation_path = signer_obj["derivation_path"].toString();
                        QString signer_type     = signer_obj["type"].toString();

                        nunchuk::SingleSigner signer(signer_name.toStdString(),
                                                     signer_xpub.toStdString(),
                                                     signer_pubkey.toStdString(),
                                                     signer_derivation_path.toStdString(),
                                                     signer_xfp.toStdString(),
                                                     std::time(0));
                        bool has_signer = bridge::nunchukHasSinger(signer);
                        is_newkey[signer_xfp] = !has_signer;
                        if(!tapsigner.isEmpty()){
                            QString card_id     = tapsigner["card_id"].toString();
                            QString version     = tapsigner["version"].toString();
                            int birth_height    = tapsigner["birth_height"].toInt();
                            bool is_testnet     = tapsigner["is_testnet"].toBool();
                            bridge::AddTapsigner(card_id, signer_xfp, signer_name, version, birth_height, is_testnet);
                        }
                        else {
                            if(!has_signer){
                                nunchuk::SignerType type = nunchuk::SignerType::AIRGAP;
                                try {
                                    type = SignerTypeFromStr(signer_type.toStdString());
                                    bridge::nunchukCreateSigner(signer_name,
                                                                signer_xpub,
                                                                signer_pubkey,
                                                                signer_derivation_path,
                                                                signer_xfp,
                                                                type,
                                                                {},
                                                                false);
                                }
                                catch (const nunchuk::BaseException &ex) {
                                    DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
                                }
                                catch (std::exception &e) {
                                    DBG_INFO << "THROW EXCEPTION " << e.what();
                                }
                            }
                        }
                    }
                    QWarningMessage parsewarningmsg;
                    QString bsms = wallet_obj["bsms"].toString();
                    nunchuk::Wallet wallet_result = qUtils::ParseWalletDescriptor(bsms, parsewarningmsg);
                    if((int)EWARNING::WarningType::NONE_MSG == parsewarningmsg.type()){
                        wallet_result.set_name(wallet_name.toStdString());
                        wallet_result.set_description(wallet_description.toStdString());
                        bridge::nunchukCreateWallet(wallet_result, true, parsewarningmsg);
                    }
                }
                QWarningMessage walletwarningmsg;
                nunchuk::Wallet wallet_result = bridge::nunchukGetOriginWallet(wallet_id, walletwarningmsg);
                if((int)EWARNING::WarningType::NONE_MSG == walletwarningmsg.type()){
                    if(qUtils::strCompare(AppModel::instance()->walletInfo()->walletId(), wallet_id)){
                        AppModel::instance()->startSyncWalletDb(wallet_id);
                        emit AppModel::instance()->walletInfo()->groupInfoChanged();
                    }
                    std::vector<nunchuk::SingleSigner> local_signers = wallet_result.get_signers();
                    wallet_result.set_name(wallet_name.toStdString());
                    wallet_result.set_description(wallet_description.toStdString());
                    walletwarningmsg.resetWarningMessage();
                    bridge::UpdateWallet(wallet_result, walletwarningmsg);
                    if((int)EWARNING::WarningType::NONE_MSG == walletwarningmsg.type()){
                        if(AppModel::instance()->walletInfo()){
                            if(qUtils::strCompare(AppModel::instance()->walletInfo()->walletId(), wallet_id)){
                                AppModel::instance()->walletInfo()->setWalletName(wallet_name);
                                AppModel::instance()->walletInfo()->setWalletDescription(wallet_description);
                            }
                        }
                    }
                    QJsonArray signers = wallet_obj["signers"].toArray();
                    for(QJsonValue jv_signer : signers){
                        QJsonObject js_signer = jv_signer.toObject();
                        QString xfp = js_signer["xfp"].toString();
                        QString name = js_signer["name"].toString();
                        bool is_visible = js_signer["is_visible"].toBool();
                        QJsonArray wtags = js_signer["tags"].toArray();
                        std::vector<nunchuk::SingleSigner>::iterator local_signer = std::find_if(local_signers.begin(), local_signers.end(), [&](const nunchuk::SingleSigner &local){
                            return local.get_master_fingerprint() == xfp.toStdString();
                        });
                        QWarningMessage msgIn;
                        if (local_signer != local_signers.end()) {
                            if (local_signer->has_master_signer()) {
                                nunchuk::MasterSigner master_signer = bridge::nunchukGetOriginMasterSigner(QString::fromStdString(local_signer->get_master_signer_id()), msgIn);
                                std::vector<nunchuk::SignerTag> tags; // get tags from api signer.tags
                                for (QJsonValue tag : wtags) {
                                    QString js_tag = tag.toString();
                                    tags.push_back(SignerTagFromStr(js_tag.toStdString()));
                                }
                                // Do update
                                master_signer.set_name(name.toStdString());
                                master_signer.set_tags(tags);
                                if(mode == GROUP_WALLET){
                                    if (is_newkey[xfp]) {
                                        master_signer.set_visible(is_visible);
                                    }
                                    else {
                                        master_signer.set_visible(master_signer.is_visible() || is_visible);
                                    }
                                }
                                msgIn.resetWarningMessage();
                                bridge::UpdateMasterSigner(master_signer, msgIn);
                            }
                            else {
                                nunchuk::SingleSigner remote_signer = *local_signer;
                                std::vector<nunchuk::SignerTag> tags; // get tags from api signer.tags
                                for (QJsonValue tag : wtags) {
                                    QString js_tag = tag.toString();
                                    tags.push_back(SignerTagFromStr(js_tag.toStdString()));
                                }
                                // Do update
                                remote_signer.set_name(name.toStdString());
                                remote_signer.set_tags(tags);
                                if(mode == GROUP_WALLET){
                                    if (is_newkey[xfp]) {
                                        remote_signer.set_visible(is_visible);
                                    }
                                    else {
                                        remote_signer.set_visible(remote_signer.is_visible() || is_visible);
                                    }
                                }
                                msgIn.resetWarningMessage();
                                bridge::UpdateRemoteSigner(remote_signer,msgIn);
                            }
                        }
                    }
                }
            }
            else if (status == "DELETED" && bridge::nunchukHasWallet(wallet_id)){
                QWarningMessage msgwarning;
                bridge::nunchukDeleteWallet(wallet_id, msgwarning);
                if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                    AppModel::instance()->removeWallet(wallet_id);
                    AppModel::instance()->setWalletListCurrentIndex(0);
                    AppSetting::instance()->deleteWalletCached(wallet_id);
                }
            }
            else{}
        }
    }
    UpdateSyncWalletFlows();
}

void QWalletManagement::UpdateSigner(const QJsonObject &signer)
{
    QJsonObject js_signer = signer;
    QString xfp = js_signer["xfp"].toString();
    QString name = js_signer["name"].toString();
    bool is_visible = js_signer["is_visible"].toBool();
    QJsonArray wtags = js_signer["tags"].toArray();
    QWarningMessage msgIn;
    nunchuk::MasterSigner master_signer = bridge::nunchukGetOriginMasterSigner(xfp, msgIn);
    if (msgIn.type() == (int)EWARNING::WarningType::NONE_MSG) {
        std::vector<nunchuk::SignerTag> tags; // get tags from api signer.tags
        for (QJsonValue tag : wtags) {
            QString js_tag = tag.toString();
            tags.push_back(SignerTagFromStr(js_tag.toStdString()));
        }
        // Do update
        master_signer.set_name(name.toStdString());
        master_signer.set_tags(tags);
        master_signer.set_visible(is_visible);
        msgIn.resetWarningMessage();
        bridge::UpdateMasterSigner(master_signer, msgIn);
    }
}

void QWalletManagement::UpdateSyncWalletFlows()
{
    QMap<QString, QJsonObject> server_signers;
    for(QJsonValue wallet_json : walletArray){
        QJsonObject wallet_obj = wallet_json.toObject();
        QString wallet_id   = wallet_obj["local_id"].toString();
        QJsonArray signers = wallet_obj["signers"].toArray();
        for(QJsonValue jv_signer : signers){
            QJsonObject js_signer = jv_signer.toObject();
            QString xfp = js_signer["xfp"].toString();
            server_signers.insert(xfp, js_signer);
        }
        //Update sync wallet flows
        QWarningMessage msg;
        auto w = bridge::nunchukGetOriginWallet(wallet_id, msg);
        auto local_signers = w.get_signers();
        for (auto &&local_signer : local_signers)
        {
            if (!local_signer.is_visible() || local_signer.get_type() == nunchuk::SignerType::SERVER) {
                continue;
            }
            auto server_signer = server_signers[QString::fromStdString(local_signer.get_master_fingerprint())];
            if (local_signer.get_type() != SignerTypeFromStr(server_signer["type"].toString().toStdString()))
            { // local signer type not match server type
                if (local_signer.get_type() == nunchuk::SignerType::SOFTWARE)
                {
                    // UI: “The software key (local_signer.get_master_fingerprint()) was upgraded to a hardware key from another device. Do you want to remove the software key from this device?”
                    // UI: “The software key (local_signer.get_master_fingerprint()) will be removed from this device. Please tap ‘Delete key’ to confirm.”
                    software_signer = server_signer;
                    m_local_signer = local_signer;
                    AppModel::instance()->syncingWalletFromServer(QString::fromStdString(m_local_signer.get_master_fingerprint()));
                }
                else
                {
                    // update local to match server
                    QJsonObject tapsigner   = server_signer["tapsigner"].toObject();
                    QString signer_name     = server_signer["name"].toString();
                    QString signer_xfp      = server_signer["xfp"].toString();
                    QString signer_xpub     = server_signer["xpub"].toString();
                    QString signer_pubkey   = server_signer["pubkey"].toString();
                    QString signer_derivation_path = server_signer["derivation_path"].toString();
                    QString signer_type     = server_signer["type"].toString();

                    nunchuk::SingleSigner signer(signer_name.toStdString(),
                                                 signer_xpub.toStdString(),
                                                 signer_pubkey.toStdString(),
                                                 signer_derivation_path.toStdString(),
                                                 signer_xfp.toStdString(),
                                                 std::time(0));
                    bool has_signer = bridge::nunchukHasSinger(signer);
                    if(!tapsigner.isEmpty()){
                        QString card_id     = tapsigner["card_id"].toString();
                        QString version     = tapsigner["version"].toString();
                        int birth_height    = tapsigner["birth_height"].toInt();
                        bool is_testnet     = tapsigner["is_testnet"].toBool();
                        bridge::AddTapsigner(card_id, signer_xfp, signer_name, version, birth_height, is_testnet, true);
                    }
                    else {
                        if(!has_signer){
                            nunchuk::SignerType type = nunchuk::SignerType::AIRGAP;
                            try {
                                type = SignerTypeFromStr(signer_type.toStdString());
                                bridge::nunchukCreateSigner(signer_name,
                                                            signer_xpub,
                                                            signer_pubkey,
                                                            signer_derivation_path,
                                                            signer_xfp,
                                                            type,
                                                            {},
                                                            true);
                            }
                            catch (const nunchuk::BaseException &ex) {
                                DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
                            }
                            catch (std::exception &e) {
                                DBG_INFO << "THROW EXCEPTION " << e.what();
                            }
                        }
                    }
                }
            }
        }
    }
}

QString QWalletManagement::UpdateSyncWalletFlows(bool yes, bool no)
{
    if (yes) {
        // update local to match server
        QJsonObject tapsigner   = software_signer["tapsigner"].toObject();
        QString signer_name     = software_signer["name"].toString();
        QString signer_xfp      = software_signer["xfp"].toString();
        QString signer_xpub     = software_signer["xpub"].toString();
        QString signer_pubkey   = software_signer["pubkey"].toString();
        QString signer_derivation_path = software_signer["derivation_path"].toString();
        QString signer_type     = software_signer["type"].toString();

        nunchuk::SingleSigner signer(signer_name.toStdString(),
                                     signer_xpub.toStdString(),
                                     signer_pubkey.toStdString(),
                                     signer_derivation_path.toStdString(),
                                     signer_xfp.toStdString(),
                                     std::time(0));
        bool has_signer = bridge::nunchukHasSinger(signer);
        if(!tapsigner.isEmpty()){
            QString card_id     = tapsigner["card_id"].toString();
            QString version     = tapsigner["version"].toString();
            int birth_height    = tapsigner["birth_height"].toInt();
            bool is_testnet     = tapsigner["is_testnet"].toBool();
            bridge::AddTapsigner(card_id, signer_xfp, signer_name, version, birth_height, is_testnet, true);
        }
        else {
            if(!has_signer){
                nunchuk::SignerType type = nunchuk::SignerType::AIRGAP;
                try {
                    type = SignerTypeFromStr(signer_type.toStdString());
                    bridge::nunchukCreateSigner(signer_name,
                                                signer_xpub,
                                                signer_pubkey,
                                                signer_derivation_path,
                                                signer_xfp,
                                                type,
                                                {},
                                                true);
                }
                catch (const nunchuk::BaseException &ex) {
                    DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
                }
                catch (std::exception &e) {
                    DBG_INFO << "THROW EXCEPTION " << e.what();
                }
            }
        }
    }
    if (no) {
        // update server to match local
        WalletKeys::SyncSignerToServer(m_local_signer);
    }
    return QString::fromStdString(m_local_signer.get_master_fingerprint());
}

WalletIdList QWalletManagement::wallets() const
{
    return mWallets.uniqueKeys();
}

GroupIdList QWalletManagement::groupIds() const
{
    return mWallets.values();
}

GroupId QWalletManagement::groupId(WalletId wallet_id) const
{
    return mWallets.value(wallet_id);
}

WalletId QWalletManagement::walletId(GroupId group_id) const
{
    return mWallets.key(group_id);
}

QString QWalletManagement::slugInfo(WalletId wallet_id) const
{
    if(mWalletsInfo.contains(wallet_id)){
        return mWalletsInfo.value(wallet_id)["slug"].toString();
    }
    return "";
}

QJsonObject QWalletManagement::walletInfo(WalletId wallet_id) const
{
    if(mWalletsInfo.contains(wallet_id)){
        return mWalletsInfo.value(wallet_id);
    }
    return {};
}

void QWalletManagement::clear()
{
    if (mActivedWallets.size() > 0) {
        mActivedWallets.clear();
    }
    if (mServerKeys.size() > 0) {
        mServerKeys.clear();
    }
    if (mInheritancePlans.size() > 0) {
        mInheritancePlans.clear();
    }
    if (mDummyTxs.size() > 0) {
        mDummyTxs.clear();
    }
    if (mHealths.size() > 0) {
        mHealths.clear();
    }
    if (mRecurringPayments.size() > 0)
    {
        mRecurringPayments.clear();
    }
    mWallets.clear();
    mWalletsInfo.clear();
}

template<>
bool QWalletManagement::contains<QGroupDashboardPtr>(WalletId wallet_id) {
    for (auto ptr : mActivedWallets) {
        if (ptr->wallet_id() == wallet_id) {
            return true;
        }
    }
    return false;
}

template<>
QGroupDashboardPtr QWalletManagement::data<QGroupDashboardPtr>(WalletId wallet_id) {
    for (auto ptr : mActivedWallets) {
        if (ptr->wallet_id() == wallet_id) {
            return ptr;
        }
    }
    return {};
}

template<>
void QWalletManagement::CreateData<QGroupDashboardPtr>(WalletId wallet_id) {
    QString slug        = slugInfo(wallet_id);
    if (slug != "") {
        GroupId group_id = groupId(wallet_id);
        QJsonObject info;
        info["id"] = group_id;
        QGroupDashboardPtr dashboard = QGroupDashboardPtr(new QGroupDashboard(wallet_id));
        if (!contains<QGroupDashboardPtr>(wallet_id)) {
            mActivedWallets.append(dashboard);
            dashboard->setGroupInfo(info);
        }
    }
}


template<>
bool QWalletManagement::contains<QServerKeyPtr>(WalletId wallet_id) {
    for (auto ptr : mServerKeys) {
        if (ptr->wallet_id() == wallet_id) {
            return true;
        }
    }
    return false;
}

template<>
QServerKeyPtr QWalletManagement::data<QServerKeyPtr>(WalletId wallet_id) {
    for (auto ptr : mServerKeys) {
        if (ptr->wallet_id() == wallet_id) {
            return ptr;
        }
    }
    return {};
}

template<>
void QWalletManagement::CreateData<QServerKeyPtr>(WalletId wallet_id) {
    if (wallet_id.isEmpty()) return;
    QServerKeyPtr serverkey = QServerKeyPtr(new QServerKey(wallet_id));
    if (!contains<QServerKeyPtr>(wallet_id)) {
        mServerKeys.append(serverkey);
    }
}


template<>
bool QWalletManagement::contains<QInheritancePlanPtr>(WalletId wallet_id) {
    for (auto ptr : mInheritancePlans) {
        if (ptr->wallet_id() == wallet_id) {
            return true;
        }
    }
    return false;
}

template<>
QInheritancePlanPtr QWalletManagement::data<QInheritancePlanPtr>(WalletId wallet_id) {
    for (auto ptr : mInheritancePlans) {
        if (ptr->wallet_id() == wallet_id) {
            return ptr;
        }
    }
    return {};
}

template<>
void QWalletManagement::CreateData<QInheritancePlanPtr>(WalletId wallet_id) {
    if (wallet_id.isEmpty()) return;
    QInheritancePlanPtr plan = QInheritancePlanPtr(new QInheritancePlan(wallet_id));
    if (!contains<QInheritancePlanPtr>(wallet_id)) {
        mInheritancePlans.append(plan);
    }
}

template<>
bool QWalletManagement::contains<QWalletDummyTxPtr>(WalletId wallet_id) {
    for (auto ptr : mDummyTxs) {
        if (ptr->wallet_id() == wallet_id) {
            return true;
        }
    }
    return false;
}

template<>
QWalletDummyTxPtr QWalletManagement::data<QWalletDummyTxPtr>(WalletId wallet_id) {
    for (auto ptr : mDummyTxs) {
        if (ptr->wallet_id() == wallet_id) {
            return ptr;
        }
    }
    return {};
}

template<>
void QWalletManagement::CreateData<QWalletDummyTxPtr>(WalletId wallet_id) {
    if (wallet_id.isEmpty()) return;
    QWalletDummyTxPtr dummyTx = QWalletDummyTxPtr(new QWalletDummyTx(wallet_id));
    if (!contains<QWalletDummyTxPtr>(wallet_id)) {
        dummyTx->init(wallet_id);
        mDummyTxs.append(dummyTx);
    }
}


template<>
bool QWalletManagement::contains<QGroupWalletHealthCheckPtr>(WalletId wallet_id) {
    for (auto ptr : mHealths) {
        if (ptr->wallet_id() == wallet_id) {
            return true;
        }
    }
    return false;
}

template<>
QGroupWalletHealthCheckPtr QWalletManagement::data<QGroupWalletHealthCheckPtr>(WalletId wallet_id) {
    for (auto ptr : mHealths) {
        if (ptr->wallet_id() == wallet_id) {
            return ptr;
        }
    }
    return {};
}

template<>
void QWalletManagement::CreateData<QGroupWalletHealthCheckPtr>(WalletId wallet_id) {
    QGroupWalletHealthCheckPtr health = QGroupWalletHealthCheckPtr(new QGroupWalletHealthCheck(wallet_id));
    if (!contains<QGroupWalletHealthCheckPtr>(wallet_id)) {
        mHealths.append(health);
    }
}

template<>
bool QWalletManagement::contains<QRecurringPaymentPtr>(WalletId wallet_id) {
    for (auto ptr : mRecurringPayments) {
        if (ptr->wallet_id() == wallet_id) {
            return true;
        }
    }
    return false;
}

template<>
QRecurringPaymentPtr QWalletManagement::data<QRecurringPaymentPtr>(WalletId wallet_id) {
    for (auto ptr : mRecurringPayments) {
        if (ptr->wallet_id() == wallet_id) {
            return ptr;
        }
    }
    return {};
}

template<>
void QWalletManagement::CreateData<QRecurringPaymentPtr>(WalletId wallet_id) {
    QRecurringPaymentPtr payment = QRecurringPaymentPtr(new QRecurringPayment(wallet_id));
    if (!contains<QRecurringPaymentPtr>(wallet_id)) {
        mRecurringPayments.append(payment);
    }
}

void QWalletManagement::initSignInWallet(WalletId wallet_id) {
    QJsonObject info;
    info["id"] = "";
    QGroupDashboardPtr dashboard = QGroupDashboardPtr(new QGroupDashboard(wallet_id));
    if (!contains<QGroupDashboardPtr>(wallet_id)) {
        mActivedWallets.append(dashboard);
        dashboard->setGroupInfo(info);
    }
}

void QWalletManagement::FactoryWorker(WalletId wallet_id, GroupId group_id)
{
    DBG_INFO << wallet_id << group_id;
    if (!wallet_id.isEmpty()) {
        CreateData<QGroupDashboardPtr>(wallet_id);
        CreateData<QRecurringPaymentPtr>(wallet_id);
        CreateData<QServerKeyPtr>(wallet_id);
        CreateData<QInheritancePlanPtr>(wallet_id);
        CreateData<QWalletDummyTxPtr>(wallet_id);
        CreateData<QGroupWalletHealthCheckPtr>(wallet_id);
    }
    DBG_INFO << mActivedWallets.size();
}

int QWalletManagement::activeSize() const
{
    int size = 0;
    if (auto walletList = AppModel::instance()->walletListPtr()) {
        for(auto w : walletList->fullList()) {
            if (w->groupId() != "" ) {
               size ++;
            }
        }
    }
    return size;
}

void QWalletManagement::slotGetListWalletFinish()
{
    if (mWallets.size() > 0) {
        for (auto it = mWallets.cbegin(), end = mWallets.cend(); it != end; ++it)
        {
            FactoryWorker(it.key(), it.value());
        }
    }
    else {
        if (auto walletList = AppModel::instance()->walletListPtr()) {
            for(auto w : walletList->fullList()) {
                if (w->isAssistedWallet() ) {
                    mWallets.insert(w->walletId(), w->groupId());
                    FactoryWorker(w->walletId(), w->groupId());
                }
            }
        }
    }
    if (auto walletList = AppModel::instance()->walletListPtr()) {
        for(auto w : walletList->fullList()) {
            if (auto plan = w->inheritancePlanPtr()) {
                plan->GetInheritancePlan();
            }
        }
        walletList->updateHealthCheckTime();
    }
    emit QGroupWallets::instance()->dashboardInfoChanged();
    QGroupWallets::instance()->clearDashBoard();
    ServiceSetting::instance()->servicesTagPtr()->ConfigServiceTag();
    QGroupWallets::instance()->findPermissionAccount();
    AppModel::instance()->requestOnboarding();
}

void QWalletManagement::slotUpdateSigner()
{
    QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
    if(mastersigners){
        AppModel::instance()->setMasterSignerList(mastersigners);
    }
}
