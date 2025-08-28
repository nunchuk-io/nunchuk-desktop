/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
#include "bridgeifaces.h"
#include "Chats/matrixbrigde.h"
#include "Chats/ClientController.h"
#include <QTextCodec>
#include <QJsonObject>
#include <QJsonDocument>
#include "utils/enumconverter.hpp"
#include "Servers/Draco.h"
#include "ProfileSetting.h"

void bridge::nunchukMakeInstance(const QString& passphrase, QWarningMessage& msg)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    AppModel::instance()->requestClearData();
    bool encrypted = (passphrase == "") ? false : true;
    AppSetting::instance()->setGroupSetting("");
    AppSetting::instance()->setEnableDBEncryption(encrypted);
    AppSetting::instance()->resetNetworkSetting();

    nunchuk::AppSettings setting;

    // Chain setting
    qUtils::SetChain((nunchuk::Chain)AppSetting::instance()->primaryServer());
    setting.set_chain((nunchuk::Chain)AppSetting::instance()->primaryServer());

    // mainnet sever
    std::vector<std::string> mainnetServer;
    mainnetServer.push_back(AppSetting::instance()->mainnetServer().toMap()["url"].toString().toStdString());
    mainnetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    setting.set_mainnet_servers(mainnetServer);

    // Testnet server
    std::vector<std::string> testnetServer;
    testnetServer.push_back(AppSetting::instance()->testnetServer().toStdString());
    testnetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    setting.set_testnet_servers(testnetServer);

    // Signet server
    std::vector<std::string> signetServer;
    signetServer.push_back(AppSetting::instance()->signetServer().toStdString());
    signetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    setting.set_signet_servers(signetServer);

    // hwi path
    setting.set_hwi_path(bridge::hwiPath().toStdString());

    //  certificate file
    QString certPath = "";
    if(AppSetting::instance()->enableCertificateFile()){
        certPath = AppSetting::instance()->certificateFile();
    }
    setting.set_certificate_file(certPath.toStdString());

    // Storage path
    DBG_INFO << "DATAPATH" << AppSetting::instance()->storagePath();
    setting.set_storage_path(AppSetting::instance()->storagePath().toStdString());

    setting.enable_proxy(AppSetting::instance()->enableTorProxy());
    setting.set_proxy_host(AppSetting::instance()->torProxyAddress().toStdString());
    setting.set_proxy_port(AppSetting::instance()->torProxyPort());
    setting.set_proxy_username(AppSetting::instance()->torProxyName().toStdString());
    setting.set_proxy_password(AppSetting::instance()->torProxyPassword().toStdString());

    // Core RPC
    if(AppSetting::instance()->enableCoreRPC()){
        setting.set_backend_type(nunchuk::BackendType::CORERPC);
        DBG_INFO << "BACKEND TYPE = nunchuk::BackendType::CORERPC";
    }
    else{
        setting.set_backend_type(nunchuk::BackendType::ELECTRUM);
        DBG_INFO << "BACKEND TYPE = nunchuk::BackendType::ELECTRUM";
    }
    setting.set_corerpc_host(AppSetting::instance()->coreRPCAddress().toStdString());
    setting.set_corerpc_port(AppSetting::instance()->coreRPCPort());
    setting.set_corerpc_username(AppSetting::instance()->coreRPCName().toStdString());
    setting.set_corerpc_password(AppSetting::instance()->coreRPCPassword().toStdString());

    // For Groupwallet
    if((int)AppSetting::Chain::MAIN == AppSetting::instance()->primaryServer()){
        QString group_server = "https://api.nunchuk.io";
        setting.set_group_server(group_server.toStdString());
    }
    else if((int)AppSetting::Chain::TESTNET == AppSetting::instance()->primaryServer()){
        QString group_server = "https://api-testnet.nunchuk.io";
        setting.set_group_server(group_server.toStdString());
    }
    else {
        // Signet not support
    }

    DBG_INFO << "START";
    nunchukiface::instance()->makeNunchukInstance(setting,
                                                  passphrase.toStdString(),
                                                  msg);
    DBG_INFO << "END";
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        AppModel::instance()->setInititalized(true);
        AppModel::instance()->setBlockHeight(nunchukBlockHeight());
        AppModel::instance()->startGetEstimatedFee();
    }
    else{
        AppModel::instance()->setInititalized(false);
    }
}

void bridge::nunchukMakeInstanceForAccount(const QString &account,
                                           const QString &passphrase,
                                           QWarningMessage &msg)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    AppModel::instance()->requestClearData();
    bool encrypted = (passphrase == "") ? false : true;
    AppSetting::instance()->setGroupSetting(account);
    AppSetting::instance()->setEnableDBEncryption(encrypted);
    AppSetting::instance()->resetNetworkSetting();

    nunchuk::AppSettings setting;
    // Chain setting
    qUtils::SetChain((nunchuk::Chain)AppSetting::instance()->primaryServer());
    setting.set_chain((nunchuk::Chain)AppSetting::instance()->primaryServer());

    // mainnet sever
    std::vector<std::string> mainnetServer;
    mainnetServer.push_back(AppSetting::instance()->mainnetServer().toMap()["url"].toString().toStdString());
    mainnetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    setting.set_mainnet_servers(mainnetServer);

    // Testnet server
    std::vector<std::string> testnetServer;
    testnetServer.push_back(AppSetting::instance()->testnetServer().toStdString());
    testnetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    setting.set_testnet_servers(testnetServer);

    // Signet server
    std::vector<std::string> signetServer;
    signetServer.push_back(AppSetting::instance()->signetServer().toStdString());
    signetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    setting.set_signet_servers(signetServer);

    // hwi path 
    setting.set_hwi_path(bridge::hwiPath().toStdString());

    //  certificate file
    QString certPath = "";
    if(AppSetting::instance()->enableCertificateFile()){
        certPath = AppSetting::instance()->certificateFile();
    }
    setting.set_certificate_file(certPath.toStdString());

    // Storage path
    DBG_INFO << "DATAPATH" << AppSetting::instance()->storagePath();
    setting.set_storage_path(AppSetting::instance()->storagePath().toStdString());

    setting.enable_proxy(AppSetting::instance()->enableTorProxy());
    setting.set_proxy_host(AppSetting::instance()->torProxyAddress().toStdString());
    setting.set_proxy_port(AppSetting::instance()->torProxyPort());
    setting.set_proxy_username(AppSetting::instance()->torProxyName().toStdString());
    setting.set_proxy_password(AppSetting::instance()->torProxyPassword().toStdString());

    // Core RPC
    if(AppSetting::instance()->enableCoreRPC()){
        setting.set_backend_type(nunchuk::BackendType::CORERPC);
        DBG_INFO << "BACKEND TYPE = nunchuk::BackendType::CORERPC";
    }
    else{
        setting.set_backend_type(nunchuk::BackendType::ELECTRUM);
        DBG_INFO << "BACKEND TYPE = nunchuk::BackendType::ELECTRUM";
    }
    setting.set_corerpc_host(AppSetting::instance()->coreRPCAddress().toStdString());
    setting.set_corerpc_port(AppSetting::instance()->coreRPCPort());
    setting.set_corerpc_username(AppSetting::instance()->coreRPCName().toStdString());
    setting.set_corerpc_password(AppSetting::instance()->coreRPCPassword().toStdString());

    // For Groupwallet
    if((int)AppSetting::Chain::MAIN == AppSetting::instance()->primaryServer()){
        QString group_server = "https://api.nunchuk.io";
        setting.set_group_server(group_server.toStdString());
    }
    else if((int)AppSetting::Chain::TESTNET == AppSetting::instance()->primaryServer()){
        QString group_server = "https://api-testnet.nunchuk.io";
        setting.set_group_server(group_server.toStdString());
    }
    else {
        // Signet not support
    }

    DBG_INFO << "START";
    nunchukiface::instance()->makeNunchukInstanceForAccount(setting,
                                                            passphrase.toStdString(),
                                                            account.toStdString(),
                                                            msg);
    DBG_INFO << "END";
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        AppModel::instance()->setInititalized(true);
        AppModel::instance()->setBlockHeight(nunchukBlockHeight());
        AppModel::instance()->startGetEstimatedFee();
    }
    else{
        AppModel::instance()->setInititalized(false);
    }
}

QTransactionPtr bridge::convertTransaction(const nunchuk::Transaction &in, const QString &wallet_id)
{
    QTransactionPtr ret = QTransactionPtr(new Transaction(in));
    ret.data()->setWalletId(wallet_id);
    return ret;
}


QWalletListModelPtr bridge::nunchukGetWallets() {
    DBG_INFO;
    QWarningMessage msg;
    std::vector<nunchuk::Wallet> resultWallets = nunchukiface::instance()->GetWallets(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QWalletListModelPtr walletList(new WalletListModel());
        for (nunchuk::Wallet it : resultWallets) {
            walletList.data()->addWallet(bridge::convertWallet(it));
        }
        return walletList;
    }
    else{
        return NULL;
    }
}

std::vector<nunchuk::Wallet> bridge::nunchukGetOriginWallets(QWarningMessage& msg)
{
    return nunchukiface::instance()->GetWallets(msg);
}

QWalletListModelPtr bridge::nunchukConvertWallets(std::vector<nunchuk::Wallet> list)
{
    QWalletListModelPtr walletList(new WalletListModel());
    for (nunchuk::Wallet it : list) {
        QWalletPtr wallet = bridge::convertWallet(it);
        walletList.data()->addWallet(wallet);
    }
    return walletList;
}

QWalletPtr bridge::convertWallet(const nunchuk::Wallet &wallet)
{
    QWalletPtr to = QWalletPtr(new Wallet());
    to.data()->convert(wallet);
    return to;
}

QWalletPtr bridge::nunchukGetWallet(const QString &wallet_id)
{
    if(wallet_id == ""){
        return NULL;
    }
    QWarningMessage msg;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return convertWallet(wallet);
    }
    else{
        return NULL;
    }
}

QMasterSignerListModelPtr bridge::nunchukGetMasterSigners() {
    QWarningMessage msg;
    std::vector<nunchuk::MasterSigner> masterSignerList_result = nunchukiface::instance()->GetMasterSigners(msg);
    AppModel::instance()->setSoftwareSignerDeviceList(QDeviceListModelPtr(new DeviceListModel()));


    return nunchukConvertMasterSigners(masterSignerList_result);
}

std::vector<nunchuk::MasterSigner> bridge::nunchukGetOriginMasterSigners(QWarningMessage& msg)
{
    std::vector<nunchuk::MasterSigner> singers = nunchukiface::instance()->GetMasterSigners(msg);
    singers.erase(std::remove_if(singers.begin(), singers.end(), [](const nunchuk::MasterSigner& signer) {
        return signer.get_type() == nunchuk::SignerType::SERVER;
    }), singers.end());
    return singers;
}

nunchuk::MasterSigner bridge::nunchukGetOriginMasterSigner(const QString &id, QWarningMessage &msg)
{
    return nunchukiface::instance()->GetMasterSigner(id.toStdString(), msg);
}

QMasterSignerListModelPtr bridge::nunchukConvertMasterSigners(std::vector<nunchuk::MasterSigner> list)
{
    QMasterSignerListModelPtr masterSignerlist(new MasterSignerListModel());
    for (nunchuk::MasterSigner it : list) {
        QMasterSignerPtr signer = QMasterSignerPtr(new QMasterSigner(it));
        int signer_type = (int)it.get_type();
        if(signer_type == (int)ENUNCHUCK::SignerType::SERVER){
            continue;
        }
        if(signer_type == (int)ENUNCHUCK::SignerType::SOFTWARE){
            AppModel::instance()->softwareSignerDeviceList()->addDevice(QDevicePtr(new QDevice(it.get_device())));
        }
        if(signer_type == (int)ENUNCHUCK::SignerType::NFC){
            QWarningMessage msgGetTap;
            nunchuk::TapsignerStatus tapsigner = nunchukiface::instance()->GetTapsignerStatusFromMasterSigner(it.get_device().get_master_fingerprint(), msgGetTap);
            if((int)EWARNING::WarningType::NONE_MSG == msgGetTap.type()){
                signer.data()->device()->setCardId(QString::fromStdString(tapsigner.get_card_ident()));
            }
        }
        masterSignerlist.data()->addMasterSigner(signer);
    }
    return masterSignerlist;
}

QDeviceListModelPtr bridge::nunchukGetDevices(QWarningMessage& msg) {

    std::vector<nunchuk::Device> deviceList_result {};
    if (AppModel::instance()->isSignIn()) {
        deviceList_result = qUtils::GetDevices(bridge::hwiPath(), msg);
        DBG_INFO << "deviceList_result.size():" << deviceList_result.size();
    } else {
        deviceList_result = bridge::nunchukGetOriginDevices(msg);
    }
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QDeviceListModelPtr deviceList(new DeviceListModel());
        for (nunchuk::Device it : deviceList_result) {
            QDevicePtr device = QDevicePtr(new QDevice(it));
            deviceList.data()->addDevice(device);
        }
        return deviceList;
    }
    else{
        return NULL;
    }
}

std::vector<nunchuk::Device> bridge::nunchukGetOriginDevices(QWarningMessage &msg)
{
    return nunchukiface::instance()->GetDevices(msg);
}

nunchuk::HealthStatus bridge::nunchukHealthCheckMasterSigner(const QString& xfp,
                                                             const QString& message,
                                                             QString& signature,
                                                             QString& path,
                                                             QWarningMessage& msg){
    std::string in_message = message.toStdString();
    std::string out_signature = "";
    std::string out_path = "";
    nunchuk::HealthStatus ret = nunchukiface::instance()->HealthCheckMasterSigner(xfp.toStdString(), in_message, out_signature, out_path, msg);
    signature = QString::fromStdString(out_signature);
    path = QString::fromStdString(out_path);
    return ret;
}

nunchuk::HealthStatus bridge::nunchukHealthCheckSingleSigner(const QSingleSignerPtr &signer,
                                                             QWarningMessage &msg)
{
    if(signer){
        QString message = signer.data()->message();
        QString signature = signer.data()->signature();
        return nunchukiface::instance()->HealthCheckSingleSigner(signer->singleSigner(),
                                                                 message.toStdString(),
                                                                 signature.toStdString(),
                                                                 msg);
    }
    else{
        return nunchuk::HealthStatus::NO_SIGNATURE;
    }
}

QMasterSignerPtr bridge::nunchukCreateMasterSigner(const QString& name,
                                                   const int deviceIndex,
                                                   QWarningMessage &msg)
{
    QString devicePathSelected = "";
    if(NULL != AppModel::instance()->deviceList()){
        devicePathSelected = AppModel::instance()->deviceList()->getDevicePathByIndex(deviceIndex);
        QDeviceListModelPtr devices = bridge::nunchukGetDevices(msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            if(devices){
                QDevicePtr selectedDv = devices->getDeviceByPath(devicePathSelected);
                QString in_message = qUtils::QGenerateRandomMessage();
                AppModel::instance()->setNewKeySignMessage(in_message);
                if(selectedDv.data()){
                    AppModel::instance()->setAddSignerStep(1);
#if 0 //SKIP HEALTHCHECK //Redundant
                    ENUNCHUCK::HealthStatus healthResult = (ENUNCHUCK::HealthStatus)nunchukHealthCheckMasterSigner(selectedDv.data()->masterFingerPrint(),
                                                                                                                   in_message,
                                                                                                                   out_signature,
                                                                                                                   out_path,
                                                                                                                   msg);
                    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                        if(ENUNCHUCK::HealthStatus::SUCCESS == healthResult){
#endif
                            nunchuk::Device dv(selectedDv.data()->type().toStdString(),
                                               selectedDv.data()->path().toStdString(),
                                               selectedDv.data()->model().toStdString(),
                                               selectedDv.data()->masterFingerPrint().toStdString(),
                                               selectedDv.data()->needsPassPhraseSent(),
                                               selectedDv.data()->needsPinSent());
                            AppModel::instance()->setAddSignerStep(2);
                            nunchuk::MasterSigner masterSigner = nunchukiface::instance()->CreateMasterSigner(name.toStdString(), dv, msg);
                            if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                                QMasterSignerPtr signer =  QMasterSignerPtr(new QMasterSigner(masterSigner));
                                signer.data()->setMessage(in_message);
#if 0 //SKIP HEALTHCHECK //Redundant
                                signer.data()->setHealth((int)healthResult);
#endif
                                signer.data()->setDeviceIndex(deviceIndex);
                                return signer;
                            }
#if 0 //SKIP HEALTHCHECK //Redundant
                        }
                    }
#endif
                }
                else {
                    msg.setCode(-12);
                    msg.setType((int)EWARNING::WarningType::EXCEPTION_MSG);
                    msg.setWhat("Device not found, please check your device.");
                }
            }
        }
    }
    return NULL;
}

int bridge::nunchukGetLastUsedSignerIndex(const QString &xfp,
                                          const ENUNCHUCK::WalletType &wallet_type,
                                          const ENUNCHUCK::AddressType &address_type,
                                          QWarningMessage &msg)
{
    return nunchukiface::instance()->GetLastUsedSignerIndex(xfp.toStdString(),
                                                           (nunchuk::WalletType)wallet_type,
                                                           (nunchuk::AddressType)address_type,
                                                           msg);
}

QSingleSignerPtr bridge::nunchukGetSignerFromMasterSigner(const QString &mastersigner_id,
                                                          const ENUNCHUCK::WalletType &wallet_type,
                                                          const ENUNCHUCK::AddressType &address_type,
                                                          const int index,
                                                          QWarningMessage &msg)
{
    nunchuk::SingleSigner signer = nunchukiface::instance()->GetSignerFromMasterSigner(mastersigner_id.toStdString(),
                                                                                       (nunchuk::WalletType)wallet_type,
                                                                                       (nunchuk::AddressType)address_type,
                                                                                       index,
                                                                                       msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
        return ret;
    }
    else{
        return NULL;
    }
}

nunchuk::SingleSigner bridge::nunchukGetOriginSingleSigner(const QString& xfp,
                                                           const ENUNCHUCK::WalletType &wallet_type,
                                                           const ENUNCHUCK::AddressType &address_type,
                                                           const int index,
                                                           QWarningMessage& msg)
{
    nunchuk::SingleSigner signer = nunchukiface::instance()->GetSigner(xfp.toStdString(),
                                                                       (nunchuk::WalletType)wallet_type,
                                                                       (nunchuk::AddressType)address_type,
                                                                       index,
                                                                       msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return signer;
    }
    else{
        return nunchuk::SingleSigner("","","","","",0,"");
    }
}

nunchuk::SingleSigner bridge::nunchukGetOriginSingleSigner(const nunchuk::SingleSigner &signer, QWarningMessage &msg)
{
    nunchuk::SingleSigner ret = nunchukiface::instance()->GetSigner(signer, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return signer;
    }
}


QSingleSignerPtr bridge::nunchukGetUnusedSignerFromMasterSigner(const QString &mastersigner_id,
                                                                const ENUNCHUCK::WalletType &wallet_type,
                                                                const ENUNCHUCK::AddressType &address_type,
                                                                QWarningMessage &msg)
{
    nunchuk::SingleSigner signer = nunchukiface::instance()->GetUnusedSignerFromMasterSigner(mastersigner_id.toStdString(),
                                                                                             (nunchuk::WalletType)wallet_type,
                                                                                             (nunchuk::AddressType)address_type,
                                                                                             msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
        return ret;
    }
    else{
        return QSingleSignerPtr();
    }
}

QSingleSignerPtr bridge::nunchukGetDefaultSignerFromMasterSigner(const QString &mastersigner_id,
                                                                const ENUNCHUCK::WalletType &wallet_type,
                                                                const ENUNCHUCK::AddressType &address_type,
                                                                QWarningMessage &msg)
{
    DBG_INFO << mastersigner_id;
    nunchuk::SingleSigner signer = nunchukiface::instance()->GetDefaultSignerFromMasterSigner(mastersigner_id.toStdString(),
                                                                                             (nunchuk::WalletType)wallet_type,
                                                                                             (nunchuk::AddressType)address_type,
                                                                                             msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
        return ret;
    }
    else{
        return QSingleSignerPtr();
    }
}

QSingleSignerPtr bridge::nunchukGetAvailableSignerFromMasterSigner(const QMasterSignerPtr &master, const ENUNCHUCK::WalletType &wallet_type,
                                                                   const ENUNCHUCK::AddressType &address_type, QWarningMessage &msg) {
    QSingleSignerPtr signer;
    if (master->signerType() == (int)nunchuk::SignerType::NFC) {
        signer = bridge::nunchukGetDefaultSignerFromMasterSigner(master->id(), wallet_type, address_type, msg);
    } else {
        signer = bridge::nunchukGetUnusedSignerFromMasterSigner(master->id(), wallet_type, address_type, msg);
    }
    return signer;
}

QSingleSignerPtr bridge::nunchukGetSignerFromMasterSigner(const QString &mastersigner_id, const QString &derivation_path, QWarningMessage &msg) {
    auto tmp = bridge::GetSignerFromMasterSigner(mastersigner_id, derivation_path, msg);
    return QSingleSignerPtr(new QSingleSigner(tmp));
}

QSingleSignerPtr bridge::nunchukCreateSigner(const QString &name,
                                             const QString &xpub,
                                             const QString &public_key,
                                             const QString &derivation_path,
                                             const QString &master_fingerprint,
                                             const nunchuk::SignerType type,
                                             const std::vector<nunchuk::SignerTag> tags,
                                             bool replace)
{
    QWarningMessage msg;
    nunchuk::SingleSigner signer = nunchukiface::instance()->CreateSigner(name.toStdString(),
                                                                          xpub.toStdString(),
                                                                          public_key.toStdString(),
                                                                          derivation_path.toStdString(),
                                                                          master_fingerprint.toStdString(),
                                                                          type,
                                                                          tags,
                                                                          replace,
                                                                          msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
        return ret;
    }
    else{
        return NULL;
    }
}

nunchuk::SingleSigner bridge::nunchukCreateOriginSigner(const QString &name,
                                                        const QString &xpub,
                                                        const QString &public_key,
                                                        const QString &derivation_path,
                                                        const QString &master_fingerprint,
                                                        const nunchuk::SignerType type,
                                                        const std::vector<nunchuk::SignerTag> tags,
                                                        bool replace,
                                                        QWarningMessage &msg)
{
    return nunchukiface::instance()->CreateSigner(name.toStdString(),
                                                  xpub.toStdString(),
                                                  public_key.toStdString(),
                                                  derivation_path.toStdString(),
                                                  master_fingerprint.toStdString(),
                                                  type,
                                                  tags,
                                                  replace,
                                                  msg);
}

QWalletPtr bridge::nunchukCreateWallet(const nunchuk::Wallet &wallet, bool allow_used_signer, QWarningMessage &msg)
{
    nunchuk::Wallet walletResult = nunchukiface::instance()->CreateWallet(wallet, allow_used_signer, "", msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::convertWallet(walletResult);
    }
    else {
        return NULL;
    }
}

nunchuk::Wallet bridge::nunchukCreateOriginWallet(const QString& name,
                                                  int m,
                                                  int n,
                                                  SingleSignerListModel *signers,
                                                  nunchuk::AddressType address_type,
                                                  nunchuk::WalletType wallet_type,
                                                  const QString& description,
                                                  bool allow_used_signer,
                                                  const QString& decoy_pin,
                                                  nunchuk::WalletTemplate walletTemplate,
                                                  QWarningMessage &msg)
{
    return nunchukiface::instance()->CreateWallet(name.toStdString(),
                                                  m,
                                                  n,
                                                  signers->signers(),
                                                  address_type,
                                                  wallet_type,
                                                  description.toStdString(),
                                                  allow_used_signer,
                                                  decoy_pin.toStdString(),
                                                  walletTemplate,
                                                  msg);

}

QString bridge::nunchukDraftWallet(const QString &name,
                                   int m,
                                   int n,
                                   SingleSignerListModel *signers,
                                   ENUNCHUCK::AddressType address_type,
                                   bool is_escrow,
                                   const QString &desc,
                                   QWarningMessage &msg)
{
    string ret = nunchukiface::instance()->DraftWallet(name.toStdString(),
                                                       m,
                                                       n,
                                                       signers->signers(),
                                                       (nunchuk::AddressType)address_type,
                                                       is_escrow,
                                                       desc.toStdString(),
                                                       msg);

    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return QString::fromStdString(ret);
    }
    else {
        return "";
    }
}


QStringList bridge::nunchukGetUnusedAddresses(const QString &walletId, bool internal)
{
    QWarningMessage msg;
    std::vector<std::string> addrs = nunchukiface::instance()->GetAddresses(walletId.toStdString(), false, internal, msg);
    QStringList ret;
    ret.reserve(addrs.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        if(0 == addrs.size()){
            ret << bridge::nunchukGenNewAddresses(walletId, internal);
        }
        else{
            int cnt = 0;
            for (std::string it : addrs) {
                ret  << QString::fromStdString(it);
                cnt++;
                if(cnt >= MAX_UNUSED_ADDR){
                    break;
                }
            }
        }
    }
    return ret;
}

QStringList bridge::nunchukGetUsedAddresses(const QString &walletId, bool internal)
{
    QWarningMessage msg;
    std::vector<std::string> addrs = nunchukiface::instance()->GetAddresses(walletId.toStdString(), true, internal, msg);
    QStringList ret;
    ret.reserve(addrs.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : addrs) {
            ret  << QString::fromStdString(it);
        }
    }
    return ret;
}

QString bridge::nunchukGenNewAddresses(const QString &walletId, bool internal)
{
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->NewAddress(walletId.toStdString(), internal, msg));
}

QStringList bridge::nunchukGetAddresses(const QString &wallet_id, bool used, bool internal)
{
    QStringList ret;
    ret.clear();
    QWarningMessage msg;
    std::vector<std::string> addrs = nunchukiface::instance()->GetAddresses(wallet_id.toStdString(), used, internal, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        if(0 == addrs.size()){
            QString newaddr = bridge::nunchukGenNewAddresses(wallet_id, internal);
            addrs = nunchukiface::instance()->GetAddresses(wallet_id.toStdString(), used, internal, msg);
        }
        for (std::string it : addrs) {
            ret  << QString::fromStdString(it);
        }
    }
    return ret;
}

QWalletPtr bridge::nunchukImportWallet(const QString &dbFile,
                                       QWarningMessage &msg)
{
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportWalletDb(dbFile.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::convertWallet(walletResult);
    }
    else {
        return NULL;
    }
}


QWalletPtr bridge::nunchukImportWalletDescriptor(const QString &dbFile,
                                                 const QString& name,
                                                 const QString& description,
                                                 QWarningMessage &msg)
{
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportWalletDescriptor(dbFile.toStdString(),
                                                                                    name.toStdString(),
                                                                                    description.toStdString(),
                                                                                    msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        msg.resetWarningMessage();
        if (auto w = AppModel::instance()->newWalletInfo()) {
            bool needTopUp = w->singleSignersAssigned()->needTopUpXpubs();
            w->setCapableCreate(!needTopUp);
        }
        bool isGroupWallet = bridge::CheckGroupWalletExists(walletResult, msg);
        DBG_INFO << "isGroupWallet: " << isGroupWallet;
        if (isGroupWallet) {
            std::string walletid = "";
            try {
                walletid = walletResult.get_id();
            }
            catch (const nunchuk::BaseException &ex) {
                DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
            }
            catch (std::exception &e) {
                DBG_INFO << "THROW EXCEPTION" << e.what();
            }
            nunchukiface::instance()->RecoverGroupWallet(walletid, msg);
        }
        return bridge::convertWallet(walletResult);
    }
    else {
        return NULL;
    }
}

QTransactionPtr bridge::nunchukCreateTransaction(const QString &wallet_id,
                                                 const QMap<QString, qint64> outputs,
                                                 const QString &memo,
                                                 const QUTXOListModelPtr inputs,
                                                 const int fee_rate,
                                                 const bool subtract_fee_from_amount,
                                                 const QString &replace_txid,
                                                 bool anti_fee_sniping,
                                                 bool use_script_path,
                                                 const nunchuk::SigningPath& signing_path,
                                                 QWarningMessage& msg)
{
    std::map<std::string, nunchuk::Amount> out;
    for (int i = 0; i < outputs.keys().count(); i++) {
        out[outputs.keys().at(i).toStdString()] = outputs[outputs.keys().at(i)];
    }

    std::vector<nunchuk::UnspentOutput> in;
    if(inputs){
        for (int j = 0; j < inputs.data()->rowCount(); j++) {
            QUTXOPtr it = inputs.data()->getUTXOByIndex(j);
            if(it){
                nunchuk::UnspentOutput utxo;
                utxo.set_txid(it.data()->txid().toStdString());
                utxo.set_vout(it.data()->vout());
                utxo.set_address(it.data()->address().toStdString());
                utxo.set_amount(it.data()->amountSats());
                utxo.set_height(it.data()->height());
                in.push_back(utxo);
            }
        }
    }

    nunchuk::Transaction trans_result = nunchukiface::instance()->CreateTransaction(wallet_id.toStdString(),
                                                                                    out,
                                                                                    memo.toStdString(),
                                                                                    in,
                                                                                    fee_rate,
                                                                                    subtract_fee_from_amount,
                                                                                    replace_txid.toStdString(),
                                                                                    anti_fee_sniping,
                                                                                    use_script_path,
                                                                                    signing_path,
                                                                                    msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        if(final){
            final.data()->setUseScriptPath(use_script_path, true);
        }
        return final;
    }
    else{
        return NULL;
    }
}


QTransactionPtr bridge::nunchukCancelCreateTransaction(const QString &wallet_id,
                                                       nunchuk::Transaction origin_tx,
                                                       QString input_address,
                                                       const QString &memo,
                                                       const int fee_rate,
                                                       const QString &replace_txid,
                                                       bool anti_fee_sniping,
                                                       bool use_script_path,
                                                       const nunchuk::SigningPath& signing_path,
                                                       QWarningMessage &msg)
{
    auto inputs = nunchukiface::instance()->GetUnspentOutputsFromTxInputs(wallet_id.toStdString(), origin_tx.get_inputs(), msg);
    auto totalAmount = 0;
    for (auto input : inputs) totalAmount += input.get_amount();
    std::map<std::string, nunchuk::Amount> out;
    out[input_address.toStdString()] = static_cast<nunchuk::Amount>(totalAmount);

    nunchuk::Transaction trans_result = nunchukiface::instance()->CreateTransaction(wallet_id.toStdString(),
                                                                                    out,
                                                                                    memo.toStdString(),
                                                                                    inputs,
                                                                                    fee_rate,
                                                                                    true,
                                                                                    replace_txid.toStdString(),
                                                                                    anti_fee_sniping,
                                                                                    use_script_path,
                                                                                    signing_path,
                                                                                    msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);        
        if(final){
            final.data()->setUseScriptPath(use_script_path, true);
        }
        return final;
    }
    else{
        return NULL;
    }
}

bool bridge::nunchukExportTransaction(const QString &wallet_id,
                                      const QString &tx_id,
                                      const QString &file_path,
                                      QWarningMessage &msg)
{
    return nunchukiface::instance()->ExportTransaction(wallet_id.toStdString(),
                                                       tx_id.toStdString(),
                                                       file_path.toStdString(),
                                                       msg);
}

QTransactionPtr bridge::nunchukImportTransaction(const QString &wallet_id, const QString &file_path, QWarningMessage& msg)
{
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportTransaction(wallet_id.toStdString(), file_path.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        if(final && final.data()->roomId() != ""){
            QWarningMessage tmsg;
            std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(final.data()->roomId().toStdString(), tmsg);
            foreach (nunchuk::RoomTransaction tx, results) {
                if(0 == tx.get_tx_id().compare(final.data()->txid().toStdString())){
                    final.data()->setInitEventId(QString::fromStdString(tx.get_init_event_id()));
                }
            }
            QWarningMessage msgSign;
            for(QSingleSignerPtr signer:final->singleSignersAssigned()->fullList()){
                if(signer->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP
                        && signer->signerSigned()
                        && final->singleSignersAssigned()->containsFingerPrint(signer->masterFingerPrint())
                        && final.data()->initEventId() != "")
                {
                    matrixbrigde::SignAirgapTransaction(final.data()->initEventId(), signer->masterFingerPrint(), msgSign);
                }
            }
        }
        return final;
    }
    else{
        if (msg.code() == nunchuk::NunchukException::INVALID_PSBT) {
            AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        }
        return NULL;
    }
}

QTransactionPtr bridge::nunchukUpdateTransaction(const QString &wallet_id,
                                                 const QString &tx_id,
                                                 const QString &new_txid,
                                                 const QString &raw_tx,
                                                 const QString &reject_msg,
                                                 QWarningMessage &msg)
{
    nunchuk::Transaction trans_result = nunchukiface::instance()->UpdateTransaction(wallet_id.toStdString(),
                                                                                    tx_id.toStdString(),
                                                                                    new_txid.toStdString(),
                                                                                    raw_tx.toStdString(),
                                                                                    reject_msg.toStdString(),
                                                                                    msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        return final;
    }
    return NULL;
}

QTransactionPtr bridge::nunchukImportPsbt(const QString &wallet_id, const QString &psbt, QWarningMessage &msg)
{
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportPsbt(wallet_id.toStdString(),
                                                                             psbt.toStdString(),
                                                                             msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        return final;
    }
    return NULL;
}

bool bridge::nunchukReplaceTransactionId(const QString &wallet_id, const QString &transaction_id, const QString &replace_txid, QWarningMessage &msg)
{
    return nunchukiface::instance()->ReplaceTransactionId(wallet_id.toStdString(), transaction_id.toStdString(), replace_txid.toStdString(), msg);
}

nunchuk::Transaction bridge::nunchukSignTransaction(const QString &wallet_id,
                                                          const QString &tx_id,
                                                          const QDevicePtr &device,
                                                          QWarningMessage& msg)
{
    DBG_INFO << wallet_id << tx_id << device;
    if(device.data()){
        nunchuk::Device dv(device.data()->type().toStdString(),
                           device.data()->path().toStdString(),
                           device.data()->model().toStdString(),
                           device.data()->masterFingerPrint().toStdString(),
                           device.data()->needsPassPhraseSent(),
                           device.data()->needsPinSent());
        nunchuk::Transaction trans_result = nunchukiface::instance()->SignTransaction(wallet_id.toStdString(), tx_id.toStdString(), dv, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            return trans_result;
        }
    }
    return nunchuk::Transaction();
}

QTransactionPtr bridge::nunchukBroadcastTransaction(const QString &wallet_id,
                                                    const QString &tx_id,
                                                    QWarningMessage& msg)
{
    nunchuk::Transaction trans_result = nunchukiface::instance()->BroadcastTransaction(wallet_id.toStdString(), tx_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::convertTransaction(trans_result, wallet_id);
    }
    else{
        return NULL;
    }
}

QTransactionPtr bridge::nunchukGetTransaction(const QString &wallet_id,
                                              const QString &tx_id)
{
    QWarningMessage msg;
    nunchuk::Transaction trans_result = nunchukiface::instance()->GetTransaction(wallet_id.toStdString(),
                                                                                 tx_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::convertTransaction(trans_result, wallet_id);
    }
    else{
        return NULL;
    }
}

nunchuk::Transaction bridge::nunchukGetOriginTransaction(const QString &wallet_id,
                                                         const QString &tx_id,
                                                         QWarningMessage &msg)
{
    return nunchukiface::instance()->GetTransaction(wallet_id.toStdString(),
                                                    tx_id.toStdString(),
                                                    msg);
}

bool bridge::nunchukDeleteTransaction(const QString &wallet_id, const QString &tx_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->DeleteTransaction(wallet_id.toStdString(),
                                                       tx_id.toStdString(),
                                                       msg);
}

bool bridge::nunchukDeleteMasterSigner(const QString &mastersigner_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->DeleteMasterSigner(mastersigner_id.toStdString(), msg);
}

bool bridge::nunchukDeletePrimaryKey()
{
    return nunchukiface::instance()->DeletePrimaryKey();
}

bool bridge::nunchukDeleteRemoteSigner(const QString &master_fingerprint, const QString &derivation_path)
{
    QWarningMessage msg;
    return nunchukiface::instance()->DeleteRemoteSigner(master_fingerprint.toStdString(), derivation_path.toStdString(), msg);
}

void bridge::nunchukUpdateMasterSigner(const QMasterSignerPtr &signer, bool sync)
{
    if(signer){
        QtConcurrent::run([signer]() {
            QWarningMessage msg;
            nunchukiface::instance()->UpdateMasterSigner(signer.data()->originMasterSigner(), msg);
        });
        if (sync) {
            Draco::instance()->assistedKeyUpdateName(signer->fingerPrint(), signer->name());
        }
    }
}

bool bridge::nunchukDeleteWallet(const QString &wallet_id, QWarningMessage& msg)
{
    bool ret = nunchukiface::instance()->DeleteWallet(wallet_id.toStdString(), msg);
    if(ret){
        AppSetting::instance()->deleteWalletCached(wallet_id);
    }
    return ret;
}

void bridge::nunchukDeleteAllWallet()
{
    QWarningMessage msg;
    std::vector<nunchuk::Wallet> wallets = bridge::nunchukGetOriginWallets(msg);
    for (nunchuk::Wallet it : wallets) {
        std::string walletid = "";
        try {
            walletid = it.get_id();
        }
        catch (const nunchuk::BaseException &ex) {
            DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        }
        catch (std::exception &e) {
            DBG_INFO << "THROW EXCEPTION" << e.what();
        }
        bool ret = nunchukiface::instance()->DeleteWallet(walletid, msg);
        if(ret){
            AppSetting::instance()->deleteWalletCached(QString::fromStdString(walletid));
        }
    }
}

void bridge::nunchukUpdateWallet(const QString &wallet_id, const QString &name, const QString &description)
{
    QWarningMessage msg;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        if(!qUtils::strCompare(QString::fromStdString(wallet.get_name()), name)){
            wallet.set_name(name.toStdString());
            DBG_INFO << wallet.get_name();
        }
        if(!qUtils::strCompare(QString::fromStdString(wallet.get_description()), description)){
            wallet.set_description(description.toStdString());
        }
        msg.resetWarningMessage();
        nunchukiface::instance()->UpdateWallet(wallet, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type() && AppModel::instance()->walletInfo()){
            AppModel::instance()->walletInfo()->setNunchukWallet(wallet);
            if(AppModel::instance()->walletList()){
                AppModel::instance()->walletList()->updateNunchukWallet(wallet_id, wallet);
            }
            QString room_id = AppModel::instance()->walletInfo()->roomId();
            QString group_id = AppModel::instance()->walletInfo()->groupId();
            CLIENT_INSTANCE->renameRoomByzantineChat(room_id, group_id, name);

            if(auto groupList = AppModel::instance()->groupWalletList()){
                groupList->updateNunchukWallet(wallet_id, wallet);
                groupList->requestSortLastTimestamp();
            }
        }
    }
}

void bridge::nunchukUpdateWalletGapLimit(const QString &wallet_id, int gap_limit)
{
    QWarningMessage msg;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        wallet.set_gap_limit(gap_limit);
        QtConcurrent::run([wallet]() {
            QWarningMessage msgupdate;
            nunchukiface::instance()->UpdateWallet(wallet, msgupdate);
        });
    }
}

QTransactionListModelPtr bridge::nunchukGetTransactionHistory(const QString &wallet_id, int count, int skip)
{
    QWarningMessage msg;
    std::vector<nunchuk::Transaction> trans_result = nunchukiface::instance()->GetTransactionHistory(wallet_id.toStdString(),
                                                                                                     count,
                                                                                                     skip,
                                                                                                     msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionListModelPtr trans_ret = QTransactionListModelPtr(new TransactionListModel);
        for (nunchuk::Transaction it : trans_result) {
            QTransactionPtr tx = convertTransaction(it, wallet_id);
            trans_ret.data()->addTransaction(tx);
        }
        return trans_ret;
    }
    return QTransactionListModelPtr(new TransactionListModel);
}

std::vector<nunchuk::Transaction> bridge::nunchukGetOriginTransactionHistory(const QString &wallet_id, int count, int skip)
{
    QWarningMessage msg;
    std::vector<nunchuk::Transaction> trans_result = nunchukiface::instance()->GetTransactionHistory(wallet_id.toStdString(),
                                                                                                     count,
                                                                                                     skip,
                                                                                                     msg);
    return trans_result;
}

QUTXOListModelPtr bridge::nunchukGetUnspentOutputs(const QString &wallet_id)
{
    QWarningMessage msg;
    std::vector<nunchuk::UnspentOutput> utxo_result = nunchukiface::instance()->GetUnspentOutputs(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QUTXOListModelPtr ret = QUTXOListModelPtr(new QUTXOListModel(wallet_id));
        for (nunchuk::UnspentOutput it : utxo_result) {
            ret.data()->addUTXO(it);
        }
        return ret;
    }
    else{
        return NULL;
    }
}


QUTXOListModelPtr bridge::nunchukLockedGetUnspentOutputs(const QString &wallet_id)
{
    QWarningMessage msg;
    std::vector<nunchuk::UnspentOutput> utxo_result = nunchukiface::instance()->GetUnspentOutputs(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QUTXOListModelPtr ret = QUTXOListModelPtr(new QUTXOListModel(wallet_id));
        for (nunchuk::UnspentOutput it : utxo_result) {
            if (it.is_locked()) {
                ret.data()->addUTXO(it);
            }
        }
        return ret;
    }
    else{
        return NULL;
    }
}

QTransactionPtr bridge::nunchukDraftTransaction(const QString &wallet_id,
                                                const QMap<QString, qint64> outputs,
                                                const QUTXOListModelPtr inputs,
                                                const qint64 fee_rate,
                                                const bool subtract_fee_from_amount,
                                                const QString &replace_txid,
                                                bool use_script_path,
                                                const nunchuk::SigningPath& signing_path,
                                                QWarningMessage& msg)
{
    std::map<std::string, nunchuk::Amount> out;
    for (int i = 0; i < outputs.keys().count(); i++) {
        out[outputs.keys().at(i).toStdString()] = outputs[outputs.keys().at(i)];
    }

    std::vector<nunchuk::UnspentOutput> in;
    if(inputs){
        for (int j = 0; j < inputs.data()->rowCount(); j++) {
            QUTXOPtr it = inputs.data()->getUTXOByIndex(j);
            if(it){
                nunchuk::UnspentOutput utxo;
                utxo.set_txid(it.data()->txid().toStdString());
                utxo.set_vout(it.data()->vout());
                utxo.set_address(it.data()->address().toStdString());
                utxo.set_amount(it.data()->amountSats());
                utxo.set_height(it.data()->height());
                in.push_back(utxo);
            }
        }
    }

    nunchuk::Transaction trans_result = nunchukiface::instance()->DraftTransaction(wallet_id.toStdString(),
                                                                                   out,
                                                                                   in,
                                                                                   fee_rate,
                                                                                   subtract_fee_from_amount,
                                                                                   replace_txid.toStdString(),
                                                                                   use_script_path,
                                                                                   signing_path,
                                                                                   msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        final.data()->setStatus((int)nunchuk::TransactionStatus::PENDING_SIGNATURES);
        final.data()->setUseScriptPath(use_script_path);
        nunchuk::Amount packageFeeRate{0};
        if (nunchukiface::instance()->IsCPFP(wallet_id.toStdString(), trans_result, packageFeeRate, msg)) {
            final.data()->setPackageFeeRate(packageFeeRate);
        }
        return final;
    }
    else{
        if(((int)EWARNING::WarningType::EXCEPTION_MSG == msg.type()) && (msg.code() == nunchuk::NunchukException::COIN_SELECTION_ERROR) && subtract_fee_from_amount == false) {
            // Retry with subtract fee from amount is true
            msg.resetWarningMessage();
            nunchuk::Transaction trans_result = nunchukiface::instance()->DraftTransaction(wallet_id.toStdString(),
                                                                                           out,
                                                                                           in,
                                                                                           fee_rate,
                                                                                           true,
                                                                                           replace_txid.toStdString(),
                                                                                           use_script_path,
                                                                                           signing_path,
                                                                                           msg);
            if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
                final.data()->setStatus((int)nunchuk::TransactionStatus::PENDING_SIGNATURES);
                final.data()->setUseScriptPath(use_script_path, true);
                nunchuk::Amount packageFeeRate{0};
                if (nunchukiface::instance()->IsCPFP(wallet_id.toStdString(), trans_result, packageFeeRate, msg)) {
                    final.data()->setPackageFeeRate(packageFeeRate);
                }
                return final;
            }
            else {
                AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
            }
        }
        return NULL;
    }
}

nunchuk::Transaction bridge::nunchukDraftOriginTransaction(const string &wallet_id,
                                                           std::vector<nunchuk::TxOutput> tx_outputs,
                                                           std::vector<nunchuk::TxInput> tx_inputs,
                                                           nunchuk::Amount fee_rate,
                                                           const bool subtract_fee_from_amount,
                                                           const string &replace_txid,
                                                           bool use_script_path,
                                                           const nunchuk::SigningPath& signing_path,
                                                           QWarningMessage &msg)
{
    std::map<std::string, nunchuk::Amount> outputs;
    outputs.clear();
    for (int i = 0; i < (int)tx_outputs.size(); i++) {
        outputs[tx_outputs.at(i).first] = tx_outputs.at(i).second;
    }
    std::vector<nunchuk::UnspentOutput> inputs = nunchukiface::instance()->GetUnspentOutputsFromTxInputs(wallet_id, tx_inputs, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        msg.resetWarningMessage();
        nunchuk::Transaction trans_result =  nunchukiface::instance()->DraftTransaction(wallet_id,
                                                                                       outputs,
                                                                                       inputs,
                                                                                       fee_rate,
                                                                                       subtract_fee_from_amount,
                                                                                       replace_txid,
                                                                                       use_script_path,
                                                                                       signing_path,
                                                                                       msg);

        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            return trans_result;
        }
        else{
            if(((int)EWARNING::WarningType::EXCEPTION_MSG == msg.type()) && (msg.code() == nunchuk::NunchukException::COIN_SELECTION_ERROR) && subtract_fee_from_amount == false) {
                msg.resetWarningMessage();
                nunchuk::Transaction trans_result =  nunchukiface::instance()->DraftTransaction(wallet_id,
                                                                                               outputs,
                                                                                               inputs,
                                                                                               fee_rate,
                                                                                               true, // subtract_fee_from_amount is true for first false
                                                                                               replace_txid,
                                                                                               use_script_path,
                                                                                               signing_path,
                                                                                               msg);
                if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                    return trans_result;
                }
            }
        }
    }
    AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    return nunchuk::Transaction();
}

QTransactionPtr bridge::nunchukReplaceTransaction(const QString &wallet_id,
                                                  const QString &tx_id,
                                                  const int new_fee_rate,
                                                  bool anti_fee_sniping,
                                                  bool use_script_path,
                                                  QWarningMessage& msg)
{
    nunchuk::Transaction trans_result = nunchukiface::instance()->ReplaceTransaction(wallet_id.toStdString(),
                                                                                     tx_id.toStdString(),
                                                                                     new_fee_rate,
                                                                                     anti_fee_sniping,
                                                                                     use_script_path,
                                                                                     msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        if(final){
            final.data()->setUseScriptPath(use_script_path, true);
        }
        return final;
    }
    else{
        return NULL;
    }
}

void bridge::nunchukUpdateTransactionMemo(const QString &wallet_id, const QString &tx_id, const QString &new_memo)
{
    QtConcurrent::run([wallet_id, tx_id, new_memo]() {
        QWarningMessage msg;
        nunchukiface::instance()->UpdateTransactionMemo(wallet_id.toStdString(),
                                                        tx_id.toStdString(),
                                                        new_memo.toStdString(),
                                                        msg);
    });
}

void bridge::nunchukBalanceChanged(const QString &walletId, const qint64 balance)
{
    AppModel::instance()->startBalanceChanged(walletId, balance);
}

void bridge::nunchukDevicesChanged(const QString &fingerprint, const bool connected)
{
//    AppModel::instance()->startScanDevices();
}

void bridge::nunchukTransactionChanged(const QString &tx_id, const int status, const QString &wallet_id)
{
    AppModel::instance()->startTransactionChanged(tx_id, status, wallet_id);
}

void bridge::nunchukBlockChanged(const int height, const QString &hex_header)
{
    AppModel::instance()->startBlockChanged(height, hex_header);
}

void bridge::nunchukCacheMasterSignerXPub(const QString &mastersigner_id,
                                          QWarningMessage &msg)
{
    nunchukiface::instance()->CacheMasterSignerXPub(mastersigner_id.toStdString(), msg);
}

void bridge::nunchukUpdateAppSettings(QWarningMessage &msg)
{
    AppSetting::instance()->resetNetworkSetting();
    nunchuk::AppSettings ret;

    // Chain setting
    ret.set_chain((nunchuk::Chain)AppSetting::instance()->primaryServer());

    // mainnet sever
    std::vector<std::string> mainnetServer;
    mainnetServer.push_back(AppSetting::instance()->mainnetServer().toMap()["url"].toString().toStdString());
    mainnetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    ret.set_mainnet_servers(mainnetServer);

    // Testnet server
    std::vector<std::string> testnetServer;
    testnetServer.push_back(AppSetting::instance()->testnetServer().toStdString());
    testnetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    ret.set_testnet_servers(testnetServer);

    // Signet server
    std::vector<std::string> signetServer;
    signetServer.push_back(AppSetting::instance()->signetServer().toStdString());
    signetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    ret.set_signet_servers(signetServer);

    // hwi path
    ret.set_hwi_path(bridge::hwiPath().toStdString());

    // Storage path
    ret.set_storage_path(AppSetting::instance()->storagePath().toStdString());

    ret.enable_proxy(AppSetting::instance()->enableTorProxy());
    ret.set_proxy_host(AppSetting::instance()->torProxyAddress().toStdString());
    ret.set_proxy_port(AppSetting::instance()->torProxyPort());
    ret.set_proxy_username(AppSetting::instance()->torProxyName().toStdString());
    ret.set_proxy_password(AppSetting::instance()->torProxyPassword().toStdString());

    //  certificate file
    QString certPath = "";
    if(AppSetting::instance()->enableCertificateFile()){
        certPath = AppSetting::instance()->certificateFile();
    }
    ret.set_certificate_file(certPath.toStdString());

    // Core RPC
    if(AppSetting::instance()->enableCoreRPC()){
        ret.set_backend_type(nunchuk::BackendType::CORERPC);
        DBG_INFO << "BACKEND TYPE = nunchuk::BackendType::CORERPC";
    }
    else{
        ret.set_backend_type(nunchuk::BackendType::ELECTRUM);
        DBG_INFO << "BACKEND TYPE = nunchuk::BackendType::ELECTRUM";
    }
    ret.set_corerpc_host(AppSetting::instance()->coreRPCAddress().toStdString());
    ret.set_corerpc_port(AppSetting::instance()->coreRPCPort());
    ret.set_corerpc_username(AppSetting::instance()->coreRPCName().toStdString());
    ret.set_corerpc_password(AppSetting::instance()->coreRPCPassword().toStdString());

    nunchukiface::instance()->UpdateAppSettings(ret, msg);
}

bool bridge::nunchukExportWallet(const QString &wallet_id, const QString &file_path, const nunchuk::ExportFormat format)
{
    QWarningMessage msg;
    QString path = qUtils::QGetFilePath(file_path);
    return nunchukiface::instance()->ExportWallet(wallet_id.toStdString(),
                                                  path.toStdString(),
                                                  format, msg);
}

void bridge::nunchukSetPassphrase(const QString &passphrase, QWarningMessage& msg)
{
    nunchukiface::instance()->SetPassphrase(passphrase.toStdString(), msg);
}

int bridge::nunchukBlockHeight()
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetChainTip(msg);
}

bool bridge::nunchukExportHealthCheckMessage(const QString &message, const QString &file_path)
{
    QWarningMessage msg;
    return nunchukiface::instance()->ExportHealthCheckMessage(message.toStdString(), file_path.toStdString(), msg);
}

QString bridge::nunchukImportHealthCheckSignature(const QString &file_path)
{
    QWarningMessage msg;
    std::string ret = nunchukiface::instance()->ImportHealthCheckSignature(file_path.toStdString(), msg);
    return QString::fromStdString(ret);
}

qint64 bridge::nunchukEstimateFee(int conf_target)
{
    QWarningMessage msg;
    return nunchukiface::instance()->EstimateFee(conf_target, msg);
}

QString bridge::nunchukGetHealthCheckPath()
{
    return QString::fromStdString(nunchukiface::instance()->GetHealthCheckPath());
}

bool bridge::nunchukExportUnspentOutputs(const QString &wallet_id, const QString &file_path, nunchuk::ExportFormat format)
{
    QWarningMessage msg;
    return nunchukiface::instance()->ExportUnspentOutputs(wallet_id.toStdString(),
                                                          file_path.toStdString(),
                                                          format,
                                                          msg);
}

bool bridge::nunchukExportTransactionHistory(const QString &wallet_id, const QString &file_path, nunchuk::ExportFormat format)
{
    QWarningMessage msg;
    return nunchukiface::instance()->ExportTransactionHistory(wallet_id.toStdString(),
                                                              file_path.toStdString(),
                                                              format,
                                                              msg);
}

QSingleSignerListModelPtr bridge::nunchukGetRemoteSigners()
{
    QWarningMessage msg;
    std::vector<nunchuk::SingleSigner> remoteignerList_result = nunchukiface::instance()->GetRemoteSigners(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::nunchukConvertRemoteSigners(remoteignerList_result);
    }
    else{
        return NULL;
    }
}


QSingleSignerPtr bridge::nunchukGetRemoteSigner(const QString &master_fingerprint)
{
    QWarningMessage msg;
    auto signer = nunchukiface::instance()->GetRemoteSigner(master_fingerprint.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return QSingleSignerPtr(new QSingleSigner(signer));
    }
    else{
        return NULL;
    }
}

QMasterSignerPtr bridge::nunchukGetMasterSignerFingerprint(const QString &master_fingerprint)
{
    QWarningMessage msg;
    auto signer = nunchukiface::instance()->GetMasterSignerFingerprint(master_fingerprint.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return QMasterSignerPtr(new QMasterSigner(signer));
    }
    else{
        return NULL;
    }
}

std::vector<nunchuk::SingleSigner> bridge::nunchukGetOriginRemoteSigners(QWarningMessage& msg)
{
    std::vector<nunchuk::SingleSigner> singers = nunchukiface::instance()->GetRemoteSigners(msg);
    singers.erase(std::remove_if(singers.begin(), singers.end(), [](const nunchuk::SingleSigner& signer) {
        return signer.get_type() == nunchuk::SignerType::SERVER;
    }), singers.end());
    return singers;
}

QSingleSignerListModelPtr bridge::nunchukConvertRemoteSigners(std::vector<nunchuk::SingleSigner> list)
{
    QSingleSignerListModelPtr remoteSignerlist(new SingleSignerListModel());
    for (nunchuk::SingleSigner signer : list) {
        if((int)signer.get_type() == (int)ENUNCHUCK::SignerType::SERVER){
            continue;
        }
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
        remoteSignerlist.data()->addSingleSigner(ret);
    }
    return remoteSignerlist;
}

void bridge::nunchukUpdateRemoteSigner(const QSingleSignerPtr &signer, bool sync)
{
    if(signer) {
        QtConcurrent::run([signer]() {
            QWarningMessage msg;
            nunchukiface::instance()->UpdateRemoteSigner(signer.data()->originSingleSigner(), msg);
        });
        if (sync) {
            Draco::instance()->assistedKeyUpdateName(signer->masterFingerPrint(), signer->name());
        }
    }
}

bool bridge::nunchukDisplayAddressOnDevice(const QString &wallet_id, const QString &address, const QString &device_fingerprint, QWarningMessage &msg)
{
    nunchukiface::instance()->DisplayAddressOnDevice(wallet_id.toStdString(),
                                                     address.toStdString(),
                                                     device_fingerprint.toStdString(),
                                                     msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return true;
    }
    else{
        return false;
    }
}

qint64 bridge::nunchukGetAddressBalance(const QString &wallet_id, const QString &address)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetAddressBalance(wallet_id.toStdString(), address.toStdString(), msg);
}

QString bridge::nunchukGetSelectedWallet()
{
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->GetSelectedWallet(msg));
}

bool bridge::nunchukSetSelectedWallet(const QString &wallet_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->SetSelectedWallet(wallet_id.toStdString(), msg);
}

void bridge::nunchukPromtPinOnDevice(const QDevicePtr &device, QWarningMessage &msg)
{
    if(device.data()){
        nunchukiface::instance()->PromtPinOnDevice(device->originDevice(), msg);
    }
}

void bridge::nunchukSendPinToDevice(const QDevicePtr &device, const QString &pin, QWarningMessage &msg)
{
    if(device.data()){
        nunchukiface::instance()->SendPinToDevice(device->originDevice(), pin.toStdString(), msg);
    }
}

void bridge::nunchukSendPassphraseToDevice(const QDevicePtr &device, const QString &passphrase, QWarningMessage &msg)
{
    if(device.data()){
        nunchukiface::instance()->SendPassphraseToDevice(device->originDevice(), passphrase.toStdString(), msg);
    }
}

QSingleSignerPtr bridge::nunchukCreateCoboSigner(const QString &name, const QString &json_info, QWarningMessage &msg)
{
    nunchuk::SingleSigner signer = nunchukiface::instance()->CreateCoboSigner(name.toStdString(),
                                                                              json_info.toStdString(),
                                                                              msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
        return ret;
    }
    else{
        return NULL;
    }
}

QStringList bridge::nunchukExportCoboTransaction(const QString &wallet_id, const QString &tx_id, QWarningMessage &msg)
{
    std::vector<std::string> data = nunchukiface::instance()->ExportCoboTransaction(wallet_id.toStdString(), tx_id.toStdString(), msg);
    QStringList result;
    result.reserve(data.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

QTransactionPtr bridge::nunchukImportCoboTransaction(const QString &wallet_id, const QStringList &qr_data, QWarningMessage &msg)
{
    std::vector<std::string> qr_result;
    for (QString it : qr_data) {
        qr_result.push_back(it.toStdString());
    }
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportCoboTransaction(wallet_id.toStdString(),
                                                                                        qr_result,
                                                                                        msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        return final;
    }
    else{
        return NULL;
    }
}

QStringList bridge::nunchukExportCoboWallet(const QString &wallet_id, QWarningMessage &msg)
{
    std::vector<std::string> data = nunchukiface::instance()->ExportCoboWallet(wallet_id.toStdString(), msg);
    QStringList result;
    result.reserve(data.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

QWalletPtr bridge::nunchukImportCoboWallet(const QStringList &qr_data,
                                           const QString &description,
                                           QWarningMessage &msg)
{
    std::vector<std::string> qr_result;
    for (QString it : qr_data) {
        qr_result.push_back(it.toStdString());
    }
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportCoboWallet(qr_result,
                                                                              description.toStdString(),
                                                                              msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return bridge::convertWallet(walletResult);
    }
    else {
        return NULL;
    }
}

QWalletPtr bridge::nunchukImportWalletConfigFile(const QString &file_path,
                                                 const QString &description,
                                                 QWarningMessage &msg)
{
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportWalletConfigFile(file_path.toStdString(),
                                                                                    description.toStdString(),
                                                                                    msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return bridge::convertWallet(walletResult);
    }
    else {
        return NULL;
    }
}

void bridge::nunchukRescanBlockchain(int start_height, int stop_height)
{
    QWarningMessage msg;
    nunchukiface::instance()->RescanBlockchain(start_height, stop_height, msg);
}

QMasterSignerPtr bridge::nunchukCreateSoftwareSigner(const QString &name,
                                                     const QString &mnemonic,
                                                     const QString &passphrase,
                                                     bool isPrimaryKey,
                                                     bool replace,
                                                     QWarningMessage& msg)
{
    nunchuk::MasterSigner masterSigner = nunchukiface::instance()->CreateSoftwareSigner(name.toStdString(),
                                                                                        mnemonic.toStdString(),
                                                                                        passphrase.toStdString(),
                                                                                        isPrimaryKey,
                                                                                        replace,
                                                                                        msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QMasterSignerPtr signer =  QMasterSignerPtr(new QMasterSigner(masterSigner));
        return signer;
    }
    return NULL;
}

QString bridge::SignLoginMessage(const QString &mastersigner_id, const QString &message)
{
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->SignLoginMessage(mastersigner_id.toStdString(), message.toStdString(), msg));
}

void bridge::nunchukSendSignerPassphrase(const QString &mastersigner_id, const QString &passphrase, QWarningMessage &msg)
{
    nunchukiface::instance()->SendSignerPassphrase(mastersigner_id.toStdString(), passphrase.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        AppModel::instance()->masterSignerList()->reloadOriginMasterSignerById(mastersigner_id);
    }
    emit AppModel::instance()->finishSendPassphraseToDevice(msg.type());
}

QStringList bridge::nunchuckGetBIP39WordList()
{
    return qUtils::GetBIP39WordList();
}

void bridge::nunchukClearSignerPassphrase(const QString &mastersigner_id, QWarningMessage &msg)
{
    nunchukiface::instance()->ClearSignerPassphrase(mastersigner_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        AppModel::instance()->masterSignerList()->reloadOriginMasterSignerById(mastersigner_id);
    }
}

QString bridge::hwiPath()
{
    // hwi path
    QString hwiPath = "";
    if(AppSetting::instance()->enableCustomizeHWIDriver()){
        hwiPath = AppSetting::instance()->hwiPath();
    }
    else{
        hwiPath = AppSetting::instance()->executePath() + "/hwi";
    }
    return hwiPath;
}


int bridge::nunchukCurrentMode()
{
    return nunchukiface::instance()->nunchukMode();
}

void bridge::nunchukSetCurrentMode(int mode)
{
    DBG_INFO << "=========================================" << mode;
    AppModel::instance()->setNunchukMode(mode);
    nunchukiface::instance()->setNunchukMode(mode);
    if (mode == LOCAL_MODE) {
        CLIENT_INSTANCE->setIsNunchukLoggedIn(false);
    }
}

QString bridge::nunchukParseKeystoneSigner(const QString &qr_data)
{
    if(qr_data.isEmpty()){
        return "";
    }
    QJsonObject ret;
    QWarningMessage msg;
    nunchuk::SingleSigner data = nunchukiface::instance()->ParseKeystoneSigner(qr_data.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        ret["xpub"] = QString::fromStdString(data.get_xpub());
        ret["derivationpath"] = QString::fromStdString(data.get_derivation_path());
        ret["publickey"] = QString::fromStdString(data.get_public_key());
        ret["fingerprint"] = QString::fromStdString(data.get_master_fingerprint());
        ret["descriptor"] = QString::fromStdString(data.get_descriptor());
        return QString(QJsonDocument(ret).toJson(QJsonDocument::Compact));
    }
    else{
        return "";
    }
}

QStringList bridge::nunchukExportKeystoneWallet(const QString &wallet_id,
                                                QWarningMessage &msg)
{
    std::vector<std::string> data = nunchukiface::instance()->ExportKeystoneWallet(wallet_id.toStdString(), msg);
    QStringList result;
    result.reserve(data.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

QStringList bridge::nunchukExportKeystoneTransaction(const QString &wallet_id,
                                                     const QString &tx_id,
                                                     QWarningMessage &msg)
{
    std::vector<std::string> data = nunchukiface::instance()->ExportKeystoneTransaction(wallet_id.toStdString(), tx_id.toStdString(), msg);
    QStringList result;
    result.reserve(data.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

QTransactionPtr bridge::nunchukImportKeystoneTransaction(const QString &wallet_id,
                                                         const QList<QString> &qr_data,
                                                         QWarningMessage &msg)
{
    std::vector<std::string> qr_result;
    for (QString it : qr_data) {
        qr_result.push_back(it.toStdString());
    }
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportKeystoneTransaction(wallet_id.toStdString(),
                                                                                            qr_result,
                                                                                            msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        if(final && final.data()->roomId() != ""){
            QWarningMessage tmsg;
            std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(final.data()->roomId().toStdString(), tmsg);
            foreach (nunchuk::RoomTransaction tx, results) {
                if(0 == tx.get_tx_id().compare(final.data()->txid().toStdString())){
                    final.data()->setInitEventId(QString::fromStdString(tx.get_init_event_id()));
                }
            }
            QWarningMessage msgSign;
            for(QSingleSignerPtr signer:final->singleSignersAssigned()->fullList()){
                if(signer->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP
                        && signer->signerSigned()
                        && final->singleSignersAssigned()->containsFingerPrint(signer->masterFingerPrint())
                        && final.data()->initEventId() != ""){
                    matrixbrigde::SignAirgapTransaction(final.data()->initEventId(),
                                                        signer->masterFingerPrint(),
                                                        msgSign);
                }
            }
        }
        return final;
    }
    else{
        return NULL;
    }
}

QWalletPtr bridge::nunchukImportKeystoneWallet(const QList<QString> &qr_data,
                                               const QString &description,
                                               QWarningMessage &msg)
{
    std::vector<std::string> qr_result;
    for (QString it : qr_data) {
        qr_result.push_back(it.toStdString());
    }
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportKeystoneWallet(qr_result, description.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        if (auto w = AppModel::instance()->newWalletInfo()) {
            bool needTopUp = w->singleSignersAssigned()->needTopUpXpubs();
            w->setCapableCreate(!needTopUp);
        }
        return bridge::convertWallet(walletResult);
    }
    else {
        return NULL;
    }
}

QString bridge::nunchukParsePassportSigner(const QStringList &qr_data)
{
    if(qr_data.isEmpty()){
        return "";
    }
    QJsonObject ret;
    QWarningMessage msg;
    std::vector<std::string> qr_in;
    for (QString qr : qr_data) {
        qr_in.push_back(qr.toStdString());
    }
    std::vector<nunchuk::SingleSigner> signers = nunchukiface::instance()->ParsePassportSigners(qr_in, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && signers.size() > 0){
        nunchuk::SingleSigner data = signers.at(0);
        ret["xpub"] = QString::fromStdString(data.get_xpub());
        ret["derivationpath"] = QString::fromStdString(data.get_derivation_path());
        ret["publickey"] = QString::fromStdString(data.get_public_key());
        ret["fingerprint"] = QString::fromStdString(data.get_master_fingerprint());
        ret["descriptor"] = QString::fromStdString(data.get_descriptor());
        return QString(QJsonDocument(ret).toJson(QJsonDocument::Compact));
    }
    else{
        return "";
    }
}


QString bridge::nunchukParseQRSigners(const QStringList &qr_data)
{
    if(qr_data.isEmpty()){
        return "";
    }
    QJsonObject ret;
    QWarningMessage msg;
    std::vector<std::string> qr_in;
    for (QString qr : qr_data) {
        qr_in.push_back(qr.toStdString());
    }
    std::vector<nunchuk::SingleSigner> signers = nunchukiface::instance()->ParseQRSigners(qr_in, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && signers.size() > 0){
        nunchuk::SingleSigner data = signers.at(0);
        ret["xpub"] = QString::fromStdString(data.get_xpub());
        ret["derivationpath"] = QString::fromStdString(data.get_derivation_path());
        ret["publickey"] = QString::fromStdString(data.get_public_key());
        ret["fingerprint"] = QString::fromStdString(data.get_master_fingerprint());
        ret["descriptor"] = QString::fromStdString(data.get_descriptor());
        return QString(QJsonDocument(ret).toJson(QJsonDocument::Compact));
    }
    else{
        return "";
    }
}


QString bridge::loadJsonFile(const QString &filePathName)
{
    DBG_INFO << filePathName;
    QFile file(filePathName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return NULL;
    QTextStream in(&file);
    QString json_str = in.readAll();
    file.close();
    if(json_str.isEmpty()) return "";
    return json_str;
}

QString bridge::nunchukParseJSONSigners(const QString &filePathName)
{
    QWarningMessage msg;
    QSingleSignerPtr ret = nunchukParseJSONSigners(filePathName, ENUNCHUCK::SignerType::COLDCARD_NFC, msg);
    if(ret && (int)EWARNING::WarningType::NONE_MSG == msg.type()){
        nunchuk::SingleSigner data = ret->singleSigner();
        QJsonObject json;
        json["xpub"] = QString::fromStdString(data.get_xpub());
        json["derivationpath"] = QString::fromStdString(data.get_derivation_path());
        json["publickey"] = QString::fromStdString(data.get_public_key());
        json["fingerprint"] = QString::fromStdString(data.get_master_fingerprint());
        json["descriptor"] = QString::fromStdString(data.get_descriptor());
        return QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }
    else{
        return "";
    }
}

QSingleSignerPtr bridge::nunchukParseJSONSigners(const QString &filePathName, ENUNCHUCK::SignerType signer_type, QWarningMessage &msg)
{
    QString json_str = loadJsonFile(filePathName);
    std::vector<nunchuk::SingleSigner> signers = nunchukiface::instance()->ParseJSONSigners(json_str.toStdString(), (nunchuk::SignerType)signer_type, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && signers.size() > 0){
        nunchuk::SingleSigner signer = signers.at(0);
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
        ret->setName("Coldcard");
        DBG_INFO << ret->name();
        return ret;
    }
    else{
        return NULL;
    }
}

QSingleSignerPtr bridge::nunchukParseJSONSigners(const QString &filePathName, ENUNCHUCK::SignerType signer_type, ENUNCHUCK::AddressType address_type, QWarningMessage &msg)
{
    QString json_str = loadJsonFile(filePathName);
    QString path = qUtils::GetBip32DerivationPath(nunchuk::WalletType::MULTI_SIG, static_cast<nunchuk::AddressType>(address_type));
    //m/48h/0h/0h/2h
    QString prefix = path.mid(0, 8);
    QString suffix = path.mid(path.length() - 3, path.length());

    std::vector<nunchuk::SingleSigner> signers = nunchukiface::instance()->ParseJSONSigners(json_str.toStdString(), (nunchuk::SignerType)signer_type, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && signers.size() > 0){
        nunchuk::SingleSigner signer;
        for (auto s : signers) {
            QString str = QString::fromStdString(s.get_derivation_path());
            if (str.startsWith(prefix) && str.endsWith(suffix)) {
                signer = s;
                break;
            }
        }
        if (signer.get_derivation_path() != "") {
            signer.set_tags({nunchuk::SignerTag::COLDCARD});
            QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
            ret->setName("Coldcard");
            DBG_INFO << ret->name();
            return ret;
        }
        else {
            nunchuk::Chain chain = static_cast<nunchuk::Chain>(AppSetting::instance()->primaryServer());
            if (chain == nunchuk::Chain::MAIN) {
                msg.setWarningMessage(-100, "It looks like your device is in testnet mode. Please change it back to mainnet before continuing.", EWARNING::WarningType::EXCEPTION_MSG);
            }
            return NULL;
        }
    }
    else{
        return NULL;
    }
}

QStringList bridge::nunchukExportPassportWallet(const QString &wallet_id, QWarningMessage &msg)
{
    std::vector<std::string> data = nunchukiface::instance()->ExportPassportWallet(wallet_id.toStdString(), msg);
    QStringList result;
    result.reserve(data.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

QStringList bridge::nunchukExportPassportTransaction(const QString &wallet_id,
                                                     const QString &tx_id,
                                                     QWarningMessage &msg)
{
    std::vector<std::string> data = nunchukiface::instance()->ExportPassportTransaction(wallet_id.toStdString(),
                                                                                        tx_id.toStdString(),
                                                                                        msg);
    QStringList result;
    result.reserve(data.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

QTransactionPtr bridge::nunchukImportPassportTransaction(const QString &wallet_id,
                                                         const QStringList &qr_data,
                                                         QWarningMessage &msg)
{
    std::vector<std::string> qr_result;
    for (QString it : qr_data) {
        qr_result.push_back(it.toStdString());
    }
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportPassportTransaction(wallet_id.toStdString(),
                                                                                            qr_result,
                                                                                            msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        if(final && final.data()->roomId() != ""){
            QWarningMessage tmsg;
            std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(final.data()->roomId().toStdString(), tmsg);
            foreach (nunchuk::RoomTransaction tx, results) {
                if(0 == tx.get_tx_id().compare(final.data()->txid().toStdString())){
                    final.data()->setInitEventId(QString::fromStdString(tx.get_init_event_id()));
                }
            }
            QWarningMessage msgSign;
            for(QSingleSignerPtr signer:final->singleSignersAssigned()->fullList()){
                if(signer->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP
                        && signer->signerSigned()
                        && final->singleSignersAssigned()->containsFingerPrint(signer->masterFingerPrint())
                        && final.data()->initEventId() != ""){
                    matrixbrigde::SignAirgapTransaction(final.data()->initEventId(),
                                                        signer->masterFingerPrint(),
                                                        msgSign);
                }
            }
        }
        return final;
    }
    else{
        return NULL;
    }
}

void bridge::stopNunchuk()
{
    nunchukiface::instance()->stopOneInstance();
}

void bridge::stopAllNunchuk()
{
    nunchukiface::instance()->stopAllInstance();
}

bool bridge::nunchukHasSinger(const nunchuk::SingleSigner &signer)
{
    QWarningMessage msg;
    return nunchukiface::instance()->HasSigner(signer,msg);
}

bool bridge::nunchukHasWallet(const QString &wallet_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->HasWallet(wallet_id.toStdString(),msg);
}

void bridge::AddTapsigner(const QString &card_ident, const QString &xfp, const QString &name, const QString &version, int birth_height, bool is_testnet, bool replace)
{
    QWarningMessage msg;
    nunchukiface::instance()->AddTapsigner(card_ident.toStdString(), xfp.toStdString(), name.toStdString(), version.toStdString(), birth_height, is_testnet, replace, msg);
}

bool bridge::nunchukUpdateTransactionSchedule(const QString &wallet_id, const QString &tx_id, time_t ts, QWarningMessage &msg)
{
    return nunchukiface::instance()->UpdateTransactionSchedule(wallet_id.toStdString(),tx_id.toStdString(),ts,msg);
}

void bridge::ForceRefreshWallet(const QString &wallet_id, QWarningMessage &msg)
{
    nunchukiface::instance()->ForceRefreshWallet(wallet_id.toStdString(),msg);
}

QString bridge::SignHealthCheckMessage(const QSingleSignerPtr &signer, const QString &message, QWarningMessage &msg)
{
    return SignHealthCheckMessage(signer.data()->originSingleSigner(), message, msg);
}

QString bridge::SignHealthCheckMessage(const nunchuk::SingleSigner &signer, const QString &message, QWarningMessage &msg)
{
    return QString::fromStdString(nunchukiface::instance()->SignHealthCheckMessage(signer, message.toStdString(), msg));
}

QMasterSignerPtr bridge::ImportTapsignerMasterSigner( const std::vector<unsigned char>& data,
                                                      const QString& backup_key,
                                                      const QString& name,
                                                      bool is_primary,
                                                      QWarningMessage& msg)
{
    nunchuk::MasterSigner it = nunchukiface::instance()->ImportTapsignerMasterSigner(data,
                                                                                     backup_key.toStdString(),
                                                                                     name.toStdString(),
                                                                                     is_primary,
                                                                                     msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QMasterSignerPtr signer = QMasterSignerPtr(new QMasterSigner(it));
        QWarningMessage msgGetTap;
        nunchuk::TapsignerStatus tapsigner = nunchukiface::instance()->GetTapsignerStatusFromMasterSigner(it.get_device().get_master_fingerprint(), msgGetTap);
        if((int)EWARNING::WarningType::NONE_MSG == msgGetTap.type()){
            signer.data()->device()->setCardId(QString::fromStdString(tapsigner.get_card_ident()));
        }
        return signer;
    }
    return NULL;
}

nunchuk::Wallet bridge::nunchukGetOriginWallet(const QString &wallet_id, QWarningMessage &msg)
{
    return nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
}

nunchuk::Transaction bridge::nunchukImportQRTransaction(const QString &wallet_id, const QList<QString> &qr_data, QWarningMessage &msg)
{
    std::vector<std::string> qr_result;
    for (QString it : qr_data) {
        qr_result.push_back(it.toStdString());
    }
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportKeystoneTransaction(wallet_id.toStdString(), qr_result, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return trans_result;
    }
    else{
        return {};
    }
}

QStringList bridge::nunchukExportQRTransaction(const QString &wallet_id, const QString &tx_id, QWarningMessage &msg)
{
    std::vector<std::string> data = nunchukiface::instance()->ExportKeystoneTransaction(wallet_id.toStdString(), tx_id.toStdString(), msg);
    QStringList result;
    result.reserve(data.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

QString bridge::nunchukGetWalletExportData(const nunchuk::Wallet &wallet, nunchuk::ExportFormat format)
{
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->GetWalletExportData(wallet, format, msg));
}

bool bridge::IsMyAddress(const QString &wallet_id, const QString &address, QWarningMessage &msg)
{
    return nunchukiface::instance()->IsMyAddress(wallet_id.toStdString(), address.toStdString(), msg);
}

bool bridge::IsCPFP(const QString &wallet_id, const nunchuk::Transaction &tx, nunchuk::Amount &package_fee_rate, QWarningMessage &msg)
{
    return nunchukiface::instance()->IsCPFP(wallet_id.toStdString(), tx, package_fee_rate , msg);
}

nunchuk::SingleSigner bridge::GetSignerFromMasterSigner(const QString &mastersigner_id, const QString &derivation_path, QWarningMessage &msg)
{
    return nunchukiface::instance()->GetSignerFromMasterSigner(mastersigner_id.toStdString(), derivation_path.toStdString(), msg);
}

nunchuk::Transaction bridge::SignTransaction(const nunchuk::Wallet &wallet, const nunchuk::Transaction &tx, const nunchuk::Device &device, QWarningMessage &msg)
{
    return nunchukiface::instance()->SignTransaction(wallet, tx, device, msg);
}

bool bridge::UpdateRemoteSigner(const nunchuk::SingleSigner &remotesigner, QWarningMessage &msg)
{
    return nunchukiface::instance()->UpdateRemoteSigner(remotesigner, msg);
}

bool bridge::UpdateMasterSigner(const nunchuk::MasterSigner &mastersigner, QWarningMessage &msg)
{
    return nunchukiface::instance()->UpdateMasterSigner(mastersigner, msg);
}

bool bridge::UpdateWallet(const nunchuk::Wallet &wallet, QWarningMessage &msg)
{
    return nunchukiface::instance()->UpdateWallet(wallet, msg);
}

nunchuk::TapsignerStatus bridge::GetTapsignerStatusFromMasterSigner(const QString &fingerPrint)
{
    QWarningMessage msgGetTap;
    return nunchukiface::instance()->GetTapsignerStatusFromMasterSigner(fingerPrint.toStdString(), msgGetTap);
}

QString bridge::SignMessage(const nunchuk::SingleSigner &signer, const QString &message)
{
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->SignMessage(signer, message.toStdString(), msg));
}

QString bridge::GetSignerAddress(const nunchuk::SingleSigner &signer, const nunchuk::AddressType &address_type)
{
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->GetSignerAddress(signer, address_type, msg));
}

QString bridge::GetHotWalletMnemonic(const QString &wallet_id, const QString &passphrase)
{
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->GetHotWalletMnemonic(wallet_id.toStdString(), passphrase.toStdString(), msg));
}

QWalletPtr bridge::nunchukCreateHotWallet(const QString &mnemonic, const QString &passphrase,  bool need_backup, bool replace, QWarningMessage &msg)
{
    nunchuk::Wallet walletResult = nunchukiface::instance()->CreateHotWallet(mnemonic.toStdString(), passphrase.toStdString(), need_backup, replace, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::convertWallet(walletResult);
    }
    else {
        return NULL;
    }
}

QStringList bridge::nunchukExportBCR2020010Wallet(const QString &wallet_id, QWarningMessage &msg)
{
    std::vector<std::string> data = nunchukiface::instance()->ExportBCR2020010Wallet(wallet_id.toStdString(), msg);
    QStringList result;
    result.reserve(data.size());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

nunchuk::MasterSigner bridge::CreateSoftwareSignerFromMasterXprv(const QString &name,
                                                                 const QString &xprv,
                                                                 bool isPrimaryKey,
                                                                 bool replace,
                                                                 QWarningMessage &msg)
{
    return nunchukiface::instance()->CreateSoftwareSignerFromMasterXprv(name.toStdString(),
                                                                        xprv.toStdString(),
                                                                        isPrimaryKey,
                                                                        replace,
                                                                        msg);
}

std::vector<nunchuk::CoinCollection> bridge::nunchukGetCoinCollections(const std::string &wallet_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetCoinCollections(wallet_id, msg);
}

std::vector<nunchuk::CoinTag> bridge::nunchukGetCoinTags(const string &wallet_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetCoinTags(wallet_id, msg);
}

bool bridge::nunchukImportCoinControlData(const QString &wallet_id, const QString &data, bool force, QWarningMessage &msg)
{
    return nunchukiface::instance()->ImportCoinControlData(wallet_id.toStdString(), data.toStdString(), force, msg);
}

QString bridge::nunchukExportCoinControlData(const QString &wallet_id, QWarningMessage &msg)
{
    return QString::fromStdString(nunchukiface::instance()->ExportCoinControlData(wallet_id.toStdString(), msg));
}

QUTXOListModelPtr bridge::nunchukGetCoinInCollection(const QString &wallet_id, int collection_id)
{
    QWarningMessage msg;
    std::vector<nunchuk::UnspentOutput> utxo_result = nunchukiface::instance()->GetCoinInCollection(wallet_id.toStdString(), collection_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QUTXOListModelPtr ret = QUTXOListModelPtr(new QUTXOListModel(wallet_id));
        for (nunchuk::UnspentOutput it : utxo_result) {
            DBG_INFO << it.get_amount() << it.get_height() << (int)it.get_status() << it.get_memo() << it.get_tags();
            ret.data()->addUTXO(it);
        }
        return ret;
    }
    else{
        return NULL;
    }
}

QUTXOListModelPtr bridge::nunckGetCoinByTag(const QString &wallet_id, int tag_id)
{
    QWarningMessage msg;
    std::vector<nunchuk::UnspentOutput> utxo_result = nunchukiface::instance()->GetCoinByTag(wallet_id.toStdString(), tag_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QUTXOListModelPtr ret = QUTXOListModelPtr(new QUTXOListModel(wallet_id));
        for (nunchuk::UnspentOutput it : utxo_result) {
            ret.data()->addUTXO(it);
        }
        return ret;
    }
    else{
        return NULL;
    }
}

bool bridge::nunchukUpdateCoinTag(const QString &wallet_id, const nunchuk::CoinTag &tag)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->UpdateCoinTag(wallet_id.toStdString(), tag, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
    }
}

bool bridge::nunchukUpdateCoinCollection(const QString &wallet_id, const nunchuk::CoinCollection &collection, bool apply_to_existing_coins)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->UpdateCoinCollection(wallet_id.toStdString(), collection, apply_to_existing_coins, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
    }
}

nunchuk::CoinCollection bridge::nunchukCreateCoinCollection(const QString &wallet_id, const QString &name)
{
    QWarningMessage msg;
    return nunchukiface::instance()->CreateCoinCollection(wallet_id.toStdString(), name.toStdString(), msg);
}


bool bridge::nunchukAddToCoinCollection(const QString &wallet_id, int collection_id, const QString &tx_id, int vout)
{
    QWarningMessage msg;
    nunchukiface::instance()->AddToCoinCollection(wallet_id.toStdString(), collection_id, tx_id.toStdString(), vout, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return true;
    }
    else{
        return false;
    }
}

bool bridge::nunchukDeleteCoinCollection(const QString &wallet_id, int collection_id)
{
    QWarningMessage msg;
    nunchukiface::instance()->DeleteCoinCollection(wallet_id.toStdString(), collection_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return true;
    }
    else{
        return false;
    }
}

bool bridge::nunchukDeleteCoinTag(const QString &wallet_id, int tag_id)
{
    DBG_INFO << wallet_id << tag_id;
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->DeleteCoinTag(wallet_id.toStdString(), tag_id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
    }
}

bool bridge::nunchukUpdateCoinMemo(const QString &wallet_id, const QString &tx_id, int vout, const QString &memo)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->UpdateCoinMemo(wallet_id.toStdString(), tx_id.toStdString(), vout, memo.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
    }
}

bool bridge::nunchukLockCoin(const QString &wallet_id, const QString &tx_id, int vout)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->LockCoin(wallet_id.toStdString(), tx_id.toStdString(), vout, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
    }
}

bool bridge::nunchukUnlockCoin(const QString &wallet_id, const QString &tx_id, int vout)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->UnlockCoin(wallet_id.toStdString(), tx_id.toStdString(), vout, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
    }
}

bool bridge::nunchukAddToCoinTag(const QString &wallet_id, int tag_id, const QString &tx_id, int vout)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->AddToCoinTag(wallet_id.toStdString(), tag_id, tx_id.toStdString(), vout, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
    }
}

bool bridge::nunchukRemoveFromCoinTag(const QString &wallet_id, int tag_id, const QString &tx_id, int vout)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->RemoveFromCoinTag(wallet_id.toStdString(), tag_id, tx_id.toStdString(), vout, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
    }
}

bool bridge::nunchukRemoveFromCoinCollection(const QString &wallet_id, int collection_id, const QString &tx_id, int vout)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->RemoveFromCoinCollection(wallet_id.toStdString(), collection_id, tx_id.toStdString(), vout, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
    }
}

bool bridge::nunchukCreateCoinTag(const QString &wallet_id, const QString &name, const QString &color)
{
    QWarningMessage msg;
    auto tag = nunchukiface::instance()->CreateCoinTag(wallet_id.toStdString(), name.toStdString(), color.toStdString(), msg);
    DBG_INFO << tag.get_id() << tag.get_name() << tag.get_color();
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return true;
    }
    else{
        return false;
    }
}

std::vector<nunchuk::UnspentOutput> bridge::nunchukGetCoinByTag(const QString &wallet_id, int tag_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetCoinByTag(wallet_id.toStdString(), tag_id, msg);
}

std::vector<nunchuk::UnspentOutput> bridge::nunchukGetCoinByCollection(const QString &wallet_id, int collection_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetCoinInCollection(wallet_id.toStdString(), collection_id, msg);
}

QList<QUTXOListModel *> bridge::nunchukGetCoinAncestry(const QString &wallet_id, const QString &tx_id, int vout)
{
    QList<QUTXOListModel *> list;
    QWarningMessage msg;
    std::vector<nunchuk::UnspentOutput> utxo_result = nunchukiface::instance()->GetUnspentOutputs(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QUTXOListModel *model = new QUTXOListModel(wallet_id);
        for (nunchuk::UnspentOutput it : utxo_result) {
            if (it.get_txid() == tx_id.toStdString() && it.get_vout() == vout) {
                model->addUTXO(it);
            }
        }
        if (model->count() > 0) {
            list.append(model);
        }
    }

    std::vector<std::vector<nunchuk::UnspentOutput>> ancestry = nunchukiface::instance()->GetCoinAncestry(wallet_id.toStdString(), tx_id.toStdString(), vout, msg);

    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::vector<nunchuk::UnspentOutput> it : ancestry) {
            QUTXOListModel *model = new QUTXOListModel(wallet_id);
            for (nunchuk::UnspentOutput utxo : it) {
                model->addUTXO(utxo);
            }
            if (model->count() > 0) {
                list.append(model);
            }
        }
        return list;
    }
    else{
        return {};
    }
}

QString bridge::nunchukExportBIP329(const QString &wallet_id, QWarningMessage &msg)
{
    return QString::fromStdString(nunchukiface::instance()->ExportBIP329(wallet_id.toStdString(), msg));
}

void bridge::nunchukImportBIP329(const QString &wallet_id, const QString &data, QWarningMessage &msg)
{
    nunchukiface::instance()->ImportBIP329(wallet_id.toStdString(), data.toStdString(), msg);
}

QString bridge::nunchukGetAddressPath(const QString &wallet_id, const QString &address)
{
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->GetAddressPath(wallet_id.toStdString(), address.toStdString(), msg));
}

bool bridge::nunchukMarkAddressUsed(const QString &wallet_id, const QString &address)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->MarkAddressAsUsed(wallet_id.toStdString(), address.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        AppModel::instance()->showToast(0, STR_CPP_131, EWARNING::WarningType::SUCCESS_MSG);
    }
    else{
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return ret;
}

std::vector<nunchuk::UnspentOutput> bridge::nunchukGetOriginCoinsFromTxInputs(const QString &walletId, const std::vector<nunchuk::TxInput> &inputs)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetCoinsFromTxInputs(walletId.toStdString(), inputs, msg);
}

std::vector<nunchuk::UnspentOutput> bridge::nunchukGetOriginCoins(const QString &walletId)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetCoins(walletId.toStdString(), msg);
}

std::vector<nunchuk::UnspentOutput> bridge::nunchukGetOriginUnspentOutputs(const QString &walletId)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetUnspentOutputs(walletId.toStdString(), msg);
}

QMasterSignerPtr bridge::ImportBackupKey(const std::vector<unsigned char> &data, const QString &backup_key, const QString &name, bool is_primary, QWarningMessage &msg)
{
    nunchuk::MasterSigner it = nunchukiface::instance()->ImportBackupKey(data,
                                                                         backup_key.toStdString(),
                                                                         name.toStdString(),
                                                                         is_primary,
                                                                         msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QMasterSignerPtr signer = QMasterSignerPtr(new QMasterSigner(it));
        QWarningMessage msgGetTap;
        nunchuk::TapsignerStatus tapsigner = nunchukiface::instance()->GetTapsignerStatusFromMasterSigner(it.get_device().get_master_fingerprint(), msgGetTap);
        if((int)EWARNING::WarningType::NONE_MSG == msgGetTap.type()){
            signer.data()->device()->setCardId(QString::fromStdString(tapsigner.get_card_ident()));
        }
        return signer;
    }
    return NULL;
}

std::vector<nunchuk::SingleSigner> bridge::GetSignersFromMasterSigner(const QString &mastersigner_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetSignersFromMasterSigner(mastersigner_id.toStdString(), msg);
}

void bridge::StartConsumeGroupEvent()
{
    QtConcurrent::run([]() {
        QWarningMessage msg;
        DBG_INFO << "CONSUME START";
        nunchukiface::instance()->StartConsumeGroupEvent(msg);
        DBG_INFO << "CONSUME FINISHED";
    });
}

void bridge::StopConsumeGroupEvent()
{
    QWarningMessage msg;
    DBG_INFO << "CONSUME STOP";
    nunchukiface::instance()->StopConsumeGroupEvent(msg);
    DBG_INFO << "CONSUME STOP FINSHED";
}

void bridge::EnableGroupWallet()
{
    QWarningMessage msg;
    QString osName      = qUtils::osName();
    QString osVersion   = qUtils::osVersion();
    QString appVersion  = qUtils::appVersion();
    QString deviceClass = qUtils::deviceClass();
    QString deviceId    = qUtils::deviceId();
    QString accessToken = qUtils::accessToken();

    DBG_INFO << "OS name: " << osName;
    DBG_INFO << "OS version:" << osVersion;
    DBG_INFO << "Nunchuk version:" << appVersion;
    DBG_INFO << "Device class:" << deviceClass;
    DBG_INFO << "Device ID:" << deviceId;
    // DBG_INFO << "FIXME-REMOVE:" << accessToken;

    nunchukiface::instance()->EnableGroupWallet(osName.toStdString(),
                                                osVersion.toStdString(),
                                                appVersion.toStdString(),
                                                deviceClass.toStdString(),
                                                deviceId.toStdString(),
                                                accessToken.toStdString(),
                                                msg);
}

std::vector<nunchuk::Wallet> bridge::nunchukGetOriginGroupWallets(QWarningMessage &msg)
{
    return nunchukiface::instance()->GetGroupWallets(msg);
}

void bridge::SendGroupMessage(const string &walletId, const string &message, const nunchuk::SingleSigner &signer, QWarningMessage &msg)
{
    nunchukiface::instance()->SendGroupMessage(walletId, message, signer, msg);
}

std::vector<nunchuk::GroupMessage> bridge::GetGroupMessages(const string &walletId, int page, int pageSize, bool latest, QWarningMessage &msg)
{
    return nunchukiface::instance()->GetGroupMessages(walletId, page, pageSize, latest, msg);
}

nunchuk::GroupWalletConfig bridge::GetGroupWalletConfig(const string &walletId, QWarningMessage &msg)
{
    return nunchukiface::instance()->GetGroupWalletConfig(walletId, msg);
}

void bridge::SetGroupWalletConfig(const string &walletId, const nunchuk::GroupWalletConfig &config, QWarningMessage &msg)
{
    nunchukiface::instance()->SetGroupWalletConfig(walletId, config, msg);
}

QPair<QString, QString> bridge::ParseGroupUrl(const QString &url, QWarningMessage &msg)
{
    auto pair = nunchukiface::instance()->ParseGroupUrl(url.toStdString(), msg);
    return QPair(QString::fromStdString(pair.first), QString::fromStdString(pair.second));
}

nunchuk::GroupConfig bridge::GetGroupConfig(QWarningMessage &msg)
{
    return nunchukiface::instance()->GetGroupConfig(msg);
}

nunchuk::GroupSandbox bridge::CreateGroup(const QString &name, int m, int n, nunchuk::AddressType addressType, QWarningMessage &msg)
{
    return nunchukiface::instance()->CreateGroup(name.toStdString(), m, n, addressType, msg);
}

nunchuk::GroupSandbox bridge::CreateGroup(const QString &name, const QString &script_tmpl, nunchuk::AddressType addressType, QWarningMessage &msg)
{
    return nunchukiface::instance()->CreateGroup(name.toStdString(), script_tmpl.toStdString(), addressType, msg);
}

int bridge::GetGroupOnline(const QString &groupId)
{
    QWarningMessage msg;
    int ret = nunchukiface::instance()->GetGroupOnline(groupId.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    return 0;
}

nunchuk::GroupSandbox bridge::GetGroup(const QString &groupId, QWarningMessage &msg)
{
    return nunchukiface::instance()->GetGroup(groupId.toStdString(), msg);
}

std::vector<nunchuk::GroupSandbox> bridge::GetGroups(QWarningMessage &msg)
{
    return nunchukiface::instance()->GetGroups(msg);
}

nunchuk::GroupSandbox bridge::JoinGroup(const QString &groupId, QWarningMessage &msg)
{
    return nunchukiface::instance()->JoinGroup(groupId.toStdString(), msg);
}

bool bridge::DeleteGroup(const QString &groupId)
{
    QWarningMessage msg;
    nunchukiface::instance()->DeleteGroup(groupId.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return true;
    }
    else{
        return false;
    }
}

nunchuk::GroupSandbox bridge::AddSignerToGroup(const QString &groupId, const nunchuk::SingleSigner &signer, int index, QWarningMessage &msg)
{
    return nunchukiface::instance()->AddSignerToGroup(groupId.toStdString(), signer, index, msg);
}

nunchuk::GroupSandbox bridge::AddSignerToGroup(const QString &groupId, const nunchuk::SingleSigner &signer, const QString& name, QWarningMessage &msg)
{
    return nunchukiface::instance()->AddSignerToGroup(groupId.toStdString(), signer, name.toStdString(), msg);
}

nunchuk::GroupSandbox bridge::RemoveSignerFromGroup(const QString &groupId, int index, QWarningMessage &msg)
{
    return nunchukiface::instance()->RemoveSignerFromGroup(groupId.toStdString(), index, msg);
}

nunchuk::GroupSandbox bridge::RemoveSignerFromGroup(const QString &groupId, const QString& name, QWarningMessage &msg)
{
    return nunchukiface::instance()->RemoveSignerFromGroup(groupId.toStdString(), name.toStdString(), msg);
}

nunchuk::GroupSandbox bridge::UpdateGroup(const QString &groupId, const QString &name, int m, int n, nunchuk::AddressType addressType, QWarningMessage &msg)
{
    return nunchukiface::instance()->UpdateGroup(groupId.toStdString(), name.toStdString(), m, n, addressType, msg);
}

nunchuk::GroupSandbox bridge::UpdateGroup(const QString& groupId, const QString& name, const QString& script_tmpl, nunchuk::AddressType addressType, QWarningMessage &msg) {
    return nunchukiface::instance()->UpdateGroup(groupId.toStdString(), name.toStdString(), script_tmpl.toStdString(), addressType, msg);
}

nunchuk::GroupSandbox bridge::FinalizeGroup(const QString &groupId, const QSet<size_t> valuekeyset, QWarningMessage &msg)
{
    std::set<size_t> stdSet(valuekeyset.begin(), valuekeyset.end());
    return nunchukiface::instance()->FinalizeGroup(groupId.toStdString(), stdSet, msg);
}

bool bridge::CheckGroupWalletExists(const nunchuk::Wallet &wallet, QWarningMessage &msg)
{
    return nunchukiface::instance()->CheckGroupWalletExists(wallet, msg);
}

void bridge::RecoverGroupWallet(const QString &walletId, QWarningMessage &msg)
{
    nunchukiface::instance()->RecoverGroupWallet(walletId.toStdString(), msg);
}

nunchuk::GroupSandbox bridge::SetSlotOccupied(const nunchuk::GroupSandbox &sandbox, int index, bool value)
{
    QWarningMessage msg;
    nunchuk::GroupSandbox ret = nunchukiface::instance()->SetSlotOccupied(sandbox.get_id(), index, value, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        // TODO
        return ret;
    }
    return sandbox;
}

nunchuk::GroupSandbox bridge::SetSlotOccupied(const nunchuk::GroupSandbox &sandbox, const QString& name, bool value)
{
    QWarningMessage msg;
    nunchuk::GroupSandbox ret = nunchukiface::instance()->SetSlotOccupied(sandbox.get_id(), name.toStdString(), value, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        // TODO
        return ret;
    }
    return sandbox;
}

void bridge::SetLastReadMessage(const QString &walletId, const QString &messageId)
{
    QWarningMessage msg;
    nunchukiface::instance()->SetLastReadMessage(walletId.toStdString(), messageId.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        // TODO
    }
}

int bridge::GetUnreadMessagesCount(const QString &walletId)
{
    QWarningMessage msg;
    int ret = nunchukiface::instance()->GetUnreadMessagesCount(walletId.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    return 0;
}

QString bridge::GetGroupDeviceUID()
{
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->GetGroupDeviceUID(msg));
}

int bridge::GetScriptPathFeeRate(const QString &wallet_id, const nunchuk::Transaction &tx)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetScriptPathFeeRate(wallet_id.toStdString(), tx, msg);
}

nunchuk::GroupSandbox bridge::CreateReplaceGroup(const QString &walletId, QWarningMessage &msg)
{
    return nunchukiface::instance()->CreateReplaceGroup(walletId.toStdString(), msg);
}

QMap<QString, bool> bridge::GetReplaceGroups(const QString &walletId, QWarningMessage &msg)
{
    std::map<std::string, bool> replacements = nunchukiface::instance()->GetReplaceGroups(walletId.toStdString(), msg);
    QMap<QString, bool> maps;
    for (auto&& [group_id, accepted] : replacements) {
        maps.insert(QString::fromStdString(group_id), accepted);
    }
    return maps;
}

nunchuk::GroupSandbox bridge::AcceptReplaceGroup(const QString &walletId, const QString &groupId, QWarningMessage &msg)
{
    return nunchukiface::instance()->AcceptReplaceGroup(walletId.toStdString(), groupId.toStdString(), msg);
}

void bridge::DeclineReplaceGroup(const QString &walletId, const QString &groupId, QWarningMessage &msg)
{
    nunchukiface::instance()->DeclineReplaceGroup(walletId.toStdString(), groupId.toStdString(), msg);
}

QStringList bridge::GetDeprecatedGroupWallets(QWarningMessage &msg)
{
    auto vec = nunchukiface::instance()->GetDeprecatedGroupWallets(msg);
    QStringList qlist;
    for (std::string value : vec) {
        qlist.append(QString::fromStdString(value));
    }
    return qlist;
}

QString bridge::GetHotKeyMnemonic(const QString &signer_id, const QString &passphrase) {
    QWarningMessage msg;
    return QString::fromStdString(nunchukiface::instance()->GetHotKeyMnemonic(signer_id.toStdString(), passphrase.toStdString(), msg));
}

QWalletPtr bridge::nunchukCreateMiniscriptWallet(const QString& name, const QString& script_template,
    const std::map<std::string, nunchuk::SingleSigner>& signers,
    nunchuk::AddressType address_type, const QString& description,
    bool allow_used_signer, const QString& decoy_pin, QWarningMessage &msg) {

    nunchuk::Wallet walletResult = nunchukiface::instance()->CreateMiniscriptWallet(name.toStdString(), script_template.toStdString(), signers, address_type, description.toStdString(),
        allow_used_signer, decoy_pin.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::convertWallet(walletResult);
    }
    return NULL;
}

nunchuk::Wallet bridge::nunchukCreateOriginMiniscriptWallet(
    const QString& name, const QString& script_template,
    const std::map<std::string, nunchuk::SingleSigner>& signers,
    nunchuk::AddressType address_type, const QString& description,
    bool allow_used_signer, const QString& decoy_pin, QWarningMessage &msg) {

    return nunchukiface::instance()->CreateMiniscriptWallet(name.toStdString(), script_template.toStdString(), signers, address_type, description.toStdString(),
        allow_used_signer, decoy_pin.toStdString(), msg);
}

std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> bridge::nunchukEstimateFeeForSigningPaths(const QString &wallet_id,
                                                                                                        const QMap<QString, qint64> outputs,
                                                                                                        const QUTXOListModelPtr inputs,
                                                                                                        qint64 fee_rate,
                                                                                                        bool subtract_fee_from_amount,
                                                                                                        const QString &replace_txid)
{
    std::map<std::string, nunchuk::Amount> stdOutputs;
    for (int i = 0; i < outputs.keys().count(); i++) {
        stdOutputs[outputs.keys().at(i).toStdString()] = outputs[outputs.keys().at(i)];
    }

    std::vector<nunchuk::UnspentOutput> stdInputs;
    if(inputs){
        for (int j = 0; j < inputs.data()->rowCount(); j++) {
            QUTXOPtr it = inputs.data()->getUTXOByIndex(j);
            if(it){
                nunchuk::UnspentOutput utxo;
                utxo.set_txid(it.data()->txid().toStdString());
                utxo.set_vout(it.data()->vout());
                utxo.set_address(it.data()->address().toStdString());
                utxo.set_amount(it.data()->amountSats());
                utxo.set_height(it.data()->height());
                stdInputs.push_back(utxo);
            }
        }
    }

    QWarningMessage msg;
    std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount> > result = nunchukiface::instance()->EstimateFeeForSigningPaths(wallet_id.toStdString(), stdOutputs, stdInputs, fee_rate, subtract_fee_from_amount, replace_txid.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return result;
    }
    else{
        DBG_ERROR << "EstimateFeeForSigningPaths failed: " << msg.what();
        return {};
    }
}

std::pair<int64_t, nunchuk::Timelock::Based> bridge::nunchukGetTimelockedUntil(const QString& wallet_id, const QString& tx_id) {
    QWarningMessage msg;
    auto result = nunchukiface::instance()->GetTimelockedUntil(wallet_id.toStdString(), tx_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return result;
    }
    else{
        DBG_ERROR << "GetTimelockedUntil failed: " << msg.what();
        std::pair<int64_t, nunchuk::Timelock::Based> ret {0, nunchuk::Timelock::Based::NONE};
        return ret;
    }
}

std::vector<nunchuk::SingleSigner> bridge::nunchukGetTransactionSigners(const QString &wallet_id, const QString &tx_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->GetTransactionSigners(wallet_id.toStdString(), tx_id.toStdString(), msg);
}
