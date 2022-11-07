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
#include <QTextCodec>
#include <QJsonObject>
#include <QJsonDocument>

void bridge::nunchukMakeInstance(const QString& passphrase,
                                 QWarningMessage& msg)
{
    bool encrypted = (passphrase == "") ? false : true;
    AppSetting::instance()->setGroupSetting("");
    AppSetting::instance()->setEnableDBEncryption(encrypted);

    nunchuk::AppSettings setting;

    // Chain setting
    setting.set_chain((nunchuk::Chain)AppSetting::instance()->primaryServer());

    // mainnet sever
    std::vector<std::string> mainnetServer;
    mainnetServer.push_back(AppSetting::instance()->mainnetServer().toStdString());
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
    QString hwiPath = "";
    if(AppSetting::instance()->enableCustomizeHWIDriver()){
        hwiPath = AppSetting::instance()->hwiPath();
    }
    else{
        hwiPath = AppSetting::instance()->executePath() + "/hwi";
    }
    setting.set_hwi_path(hwiPath.toStdString());

    //  certificate file
    QString certPath = "";
    if(AppSetting::instance()->enableCertificateFile()){
        certPath = AppSetting::instance()->certificateFile();
    }
    setting.set_certificate_file(certPath.toStdString());

    // Storage path
    setting.set_storage_path(AppSetting::instance()->storagePath().toStdString());

    setting.enable_proxy(AppSetting::instance()->enableTorProxy());
    setting.set_proxy_host(AppSetting::instance()->torProxyAddress().toStdString());
    setting.set_proxy_port(AppSetting::instance()->torProxyPort());
    setting.set_proxy_username(AppSetting::instance()->torProxyName().toStdString());
    setting.set_proxy_password(AppSetting::instance()->torProxyPassword().toStdString());

    // Core RPC
    if(AppSetting::instance()->enableCoreRPC()){
        setting.set_backend_type(nunchuk::BackendType::CORERPC);
    }
    else{
        setting.set_backend_type(nunchuk::BackendType::ELECTRUM);
    }
    setting.set_corerpc_host(AppSetting::instance()->coreRPCAddress().toStdString());
    setting.set_corerpc_port(AppSetting::instance()->coreRPCPort());
    setting.set_corerpc_username(AppSetting::instance()->coreRPCName().toStdString());
    setting.set_corerpc_password(AppSetting::instance()->coreRPCPassword().toStdString());
    nunchukiface::instance()->makeNunchukInstance(setting,
                                                  passphrase.toStdString(),
                                                  msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        AppModel::instance()->setInititalized(true);
        AppModel::instance()->setChainTip(nunchukGetChainTip());
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
    bool encrypted = (passphrase == "") ? false : true;
    AppSetting::instance()->setGroupSetting(account);
    AppSetting::instance()->setEnableDBEncryption(encrypted);

    nunchuk::AppSettings setting;
    // Chain setting
    setting.set_chain((nunchuk::Chain)AppSetting::instance()->primaryServer());

    // mainnet sever
    std::vector<std::string> mainnetServer;
    mainnetServer.push_back(AppSetting::instance()->mainnetServer().toStdString());
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
    QString hwiPath = "";
    if(AppSetting::instance()->enableCustomizeHWIDriver()){
        hwiPath = AppSetting::instance()->hwiPath();
    }
    else{
        hwiPath = AppSetting::instance()->executePath() + "/hwi";
    }
    setting.set_hwi_path(hwiPath.toStdString());

    //  certificate file
    QString certPath = "";
    if(AppSetting::instance()->enableCertificateFile()){
        certPath = AppSetting::instance()->certificateFile();
    }
    setting.set_certificate_file(certPath.toStdString());

    // Storage path
    setting.set_storage_path(AppSetting::instance()->storagePath().toStdString());

    setting.enable_proxy(AppSetting::instance()->enableTorProxy());
    setting.set_proxy_host(AppSetting::instance()->torProxyAddress().toStdString());
    setting.set_proxy_port(AppSetting::instance()->torProxyPort());
    setting.set_proxy_username(AppSetting::instance()->torProxyName().toStdString());
    setting.set_proxy_password(AppSetting::instance()->torProxyPassword().toStdString());

    // Core RPC
    if(AppSetting::instance()->enableCoreRPC()){
        setting.set_backend_type(nunchuk::BackendType::CORERPC);
    }
    else{
        setting.set_backend_type(nunchuk::BackendType::ELECTRUM);
    }
    setting.set_corerpc_host(AppSetting::instance()->coreRPCAddress().toStdString());
    setting.set_corerpc_port(AppSetting::instance()->coreRPCPort());
    setting.set_corerpc_username(AppSetting::instance()->coreRPCName().toStdString());
    setting.set_corerpc_password(AppSetting::instance()->coreRPCPassword().toStdString());

    nunchukiface::instance()->makeNunchukInstanceForAccount(setting,
                                                            passphrase.toStdString(),
                                                            account.toStdString(),
                                                            msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        AppModel::instance()->setInititalized(true);
        AppModel::instance()->setChainTip(nunchukGetChainTip());
        AppModel::instance()->startGetEstimatedFee();
    }
    else{
        AppModel::instance()->setInititalized(false);
        AppSetting::instance()->setGroupSetting("");
    }
}

QTransactionPtr bridge::convertTransaction(nunchuk::Transaction in, const QString &wallet_id)
{
    QTransactionPtr ret = QTransactionPtr(new Transaction());
    if(AppModel::instance()->walletList()){
        QWalletPtr targetWallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        if(targetWallet){
            ret.data()->setSingleSignersAssigned(targetWallet->singleSignersAssigned()->clone());
            ret.data()->setRoomId(targetWallet->roomId());
        }
    }
    if(AppModel::instance()->masterSignerList() && ret.data()->singleSignersAssigned()){
        for (QMasterSignerPtr master : AppModel::instance()->masterSignerList()->fullList()) {
            ret.data()->singleSignersAssigned()->updateSignerIsLocalAndReadyToSign(master->fingerPrint(),master->signerType());
        }
    }
    ret.data()->setWalletId(wallet_id);
    ret.data()->setNunchukTransaction(in);
    ret.data()->setTxid(QString::fromStdString(in.get_txid()));
    ret.data()->setHeight(in.get_height());
    ret.data()->setMemo(QString::fromStdString(in.get_memo()));
    ret.data()->setM(in.get_m());
    ret.data()->setSubtractFromFeeAmount(in.subtract_fee_from_amount());
    nunchuk::TransactionStatus txstate = in.get_status();
    ret.data()->setStatus((int)txstate);
    int index_change = in.get_change_index();
    if(index_change == -1 || index_change >= (int)in.get_outputs().size()){
        ret.data()->setHasChange(false);
    }
    else{
        ret.data()->setHasChange(true);
        std::pair<std::string, int> change_ret = in.get_outputs().at(index_change);
        QDestinationPtr change = QDestinationPtr(new Destination());
        change.data()->setAddress(QString::fromStdString(change_ret.first));
        change.data()->setAmount(change_ret.second);
        ret.data()->setChange(change);
    }
    for (std::pair<std::string, nunchuk::Amount> item : in.get_inputs()) {
        if(AppModel::instance()->utxoList()){
            AppModel::instance()->utxoList()->updateSelected(QString::fromStdString(item.first), item.second);
        }
    }
    qint64 fee = in.get_fee();
    ret.data()->setFee(fee);

    qint64 feeRate = in.get_fee_rate();
    ret.data()->setFeeRate(feeRate);
    std::map<std::string, bool> signers = in.get_signers();
    int numsigned = 0;
    if(ret.data()->singleSignersAssigned()){
        ret.data()->singleSignersAssigned()->resetSignerReadyToSign();
        for ( std::map<std::string, bool>::iterator it = signers.begin(); it != signers.end(); it++ ){
            ret.data()->singleSignersAssigned()->updateSignatures(QString::fromStdString(it->first), it->second);
            if(it->second) { numsigned++; }
        }
    }
    ret.data()->setNumberSigned(numsigned);
    qint64 subtotal = 0;
    bool isReceiveTx = in.is_receive();
    ret.data()->setIsReceiveTx(isReceiveTx);
    if(ret.data()->isReceiveTx()){
        subtotal = in.get_sub_amount();
        QDestinationListModelPtr recevedAddr = QDestinationListModelPtr(new DestinationListModel());
        // Get receive address
        for (std::pair<std::string, nunchuk::Amount> item : in.get_receive_outputs()) {
            recevedAddr.data()->addDestination(QString::fromStdString(item.first), item.second);
        }
        ret.data()->setDestinationList(recevedAddr);
    }
    else{
        QDestinationListModelPtr dest = QDestinationListModelPtr(new DestinationListModel());
        int idxOutput = 0;
        for (std::pair<std::string, nunchuk::Amount> item : in.get_outputs()) {
            if(index_change != -1 && index_change == idxOutput){
                continue;
            }
            else{
                dest.data()->addDestination(QString::fromStdString(item.first), item.second);
                subtotal+=item.second;
            }
            idxOutput++;
        }
        ret.data()->setDestinationList(dest);
    }

    qint64 total = subtotal + fee;
    ret.data()->setSubtotal(subtotal);
    ret.data()->setTotal(total);
    // Orderred tx
    //    DBG_INFO << "From client: subtotal :" << subtotal << " Fee:" << fee << " Status:" << (int)txstate << " Signed:" << numsigned;
    if(nunchuk::TransactionStatus::PENDING_SIGNATURES == txstate){
        ret.data()->setBlocktime(-2);
    }
    else if(nunchuk::TransactionStatus::READY_TO_BROADCAST == txstate){
        ret.data()->setBlocktime(-1);
    }
    else if(nunchuk::TransactionStatus::PENDING_CONFIRMATION == txstate){
        ret.data()->setBlocktime(0);
    }
    else if(nunchuk::TransactionStatus::REPLACED == txstate){
        ret.data()->setBlocktime(in.get_blocktime());
        ret.data()->setReplacedTxid(QString::fromStdString(in.get_replaced_by_txid()));
    }
    else{
        ret.data()->setBlocktime(in.get_blocktime());
    }
    return ret;
}


QWalletListModelPtr bridge::nunchukGetWallets() {
    QWarningMessage msg;
    std::vector<nunchuk::Wallet> resultWallets = nunchukiface::instance()->GetWallets(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QWalletListModelPtr walletList(new WalletListModel());
        for (nunchuk::Wallet it : resultWallets) {
            QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
            for (nunchuk::SingleSigner signer : it.get_signers()) {
                QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
                ret.data()->setName(QString::fromStdString(signer.get_name()));
                ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
                ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
                ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
                ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
                ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
                ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
                ret.data()->setSignerType((int)signer.get_type());
                signersAssinged.data()->addSingleSigner(ret);
            }
            QString id = QString::fromStdString(it.get_id());
            int m = it.get_m();
            int n = it.get_n();
            QString name = QString::fromStdString(it.get_name());
            QString addrType = QString::number((int)it.get_address_type());
            qint64 balance = it.get_balance();
            QDateTime createDate = QDateTime::fromTime_t(it.get_create_date());
            bool escrow = it.is_escrow();
            QString description = QString::fromStdString(it.get_description());
            walletList.data()->addWallet(id, m, n, name, addrType, balance, createDate, escrow, signersAssinged, description);
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
        QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : it.get_signers()) {
            QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
            ret.data()->setName(QString::fromStdString(signer.get_name()));
            ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
            ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
            ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
            ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
            ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
            ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
            ret.data()->setSignerType((int)signer.get_type());
            signersAssinged.data()->addSingleSigner(ret);
        }
        QString id = QString::fromStdString(it.get_id());
        int m = it.get_m();
        int n = it.get_n();
        QString name = QString::fromStdString(it.get_name());
        QString addrType = QString::number((int)it.get_address_type());
        qint64 balance = it.get_balance();
        QDateTime createDate = QDateTime::fromTime_t(it.get_create_date());
        bool escrow = it.is_escrow();
        QString description = QString::fromStdString(it.get_description());
        walletList.data()->addWallet(id, m, n, name, addrType, balance, createDate, escrow, signersAssinged, description);
    }
    return walletList;
}

QWalletPtr bridge::nunchukGetWallet(const QString &wallet_id)
{
    if(wallet_id == ""){
        return NULL;
    }
    QWarningMessage msg;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QWalletPtr walletResult = QWalletPtr(new Wallet());
        walletResult.data()->setId(QString::fromStdString(wallet.get_id()));
        walletResult.data()->setM(wallet.get_m());
        walletResult.data()->setN(wallet.get_n());
        walletResult.data()->setName(QString::fromStdString(wallet.get_name()));
        walletResult.data()->setAddressType(QString::number((int)wallet.get_address_type()));
        walletResult.data()->setBalance(wallet.get_balance());
        walletResult.data()->setCreateDate(QDateTime::fromTime_t(wallet.get_create_date()));
        walletResult.data()->setEscrow(wallet.is_escrow());
        QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : wallet.get_signers()) {
            QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
            ret.data()->setName(QString::fromStdString(signer.get_name()));
            ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
            ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
            ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
            ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
            ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
            ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
            ret.data()->setSignerType((int)signer.get_type());
            signersAssinged.data()->addSingleSigner(ret);
        }
        walletResult.data()->setSigners(signersAssinged);
        return walletResult;
    }
    else{
        return NULL;
    }
}

QMasterSignerListModelPtr bridge::nunchukGetMasterSigners() {
    QWarningMessage msg;
    std::vector<nunchuk::MasterSigner> masterSignerList_result = nunchukiface::instance()->GetMasterSigners(msg);
    AppModel::instance()->setSoftwareSignerDeviceList(QDeviceListModelPtr(new DeviceListModel()));
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QMasterSignerListModelPtr masterSignerlist(new MasterSignerListModel());
        for (nunchuk::MasterSigner it : masterSignerList_result) {
            QString master_signer_id = QString::fromStdString(it.get_id());
            QString master_signer_name = QString::fromStdString(it.get_name()) == "" ? "UNKNOWN" : QString::fromStdString(it.get_name());
            QString type = QString::fromStdString(it.get_device().get_type());
            QString path = QString::fromStdString(it.get_device().get_path());
            QString model = QString::fromStdString(it.get_device().get_model());
            QString master_fingerprint = QString::fromStdString(it.get_device().get_master_fingerprint());
            bool connected = it.get_device().connected();
            bool needs_pass_phrase_sent = it.get_device().needs_pass_phrase_sent();
            bool needs_pin_sent = it.get_device().needs_pin_sent();
            int signer_type = (int)it.get_type();
            int health = -1;
            QDevicePtr device(new QDevice(master_signer_name,
                                         type, path, model,
                                         master_fingerprint,
                                         connected,
                                         needs_pass_phrase_sent,
                                         needs_pin_sent));
            masterSignerlist.data()->addMasterSigner(master_signer_id,
                                                     master_signer_name,
                                                     device,
                                                     health,
                                                     signer_type);
            if(signer_type == (int)ENUNCHUCK::SignerType::SOFTWARE){
                AppModel::instance()->softwareSignerDeviceList()->addDevice(master_signer_name,
                                                                            type,
                                                                            path,
                                                                            model,
                                                                            master_fingerprint,
                                                                            false,
                                                                            needs_pass_phrase_sent,
                                                                            needs_pin_sent,
                                                                            master_signer_id);
            }
        }
        return masterSignerlist;
    }
    else{
        return NULL;
    }
}

std::vector<nunchuk::MasterSigner> bridge::nunchukGetOriginMasterSigners(QWarningMessage& msg)
{
    return nunchukiface::instance()->GetMasterSigners(msg);
}

QMasterSignerListModelPtr bridge::nunchukConvertMasterSigners(std::vector<nunchuk::MasterSigner> list)
{
    QMasterSignerListModelPtr masterSignerlist(new MasterSignerListModel());
    for (nunchuk::MasterSigner it : list) {
        QString master_signer_id = QString::fromStdString(it.get_id());
        QString master_signer_name = QString::fromStdString(it.get_name()) == "" ? "UNKNOWN" : QString::fromStdString(it.get_name());
        QString type = QString::fromStdString(it.get_device().get_type());
        QString path = QString::fromStdString(it.get_device().get_path());
        QString model = QString::fromStdString(it.get_device().get_model());
        QString master_fingerprint = QString::fromStdString(it.get_device().get_master_fingerprint());
        bool connected = it.get_device().connected();
        bool needs_pass_phrase_sent = it.get_device().needs_pass_phrase_sent();
        bool needs_pin_sent = it.get_device().needs_pin_sent();
        int signer_type = (int)it.get_type();
        int health = -1;
        QDevicePtr device(new QDevice(master_signer_name,
                                     type, path, model,
                                     master_fingerprint,
                                     connected,
                                     needs_pass_phrase_sent,
                                     needs_pin_sent));
        masterSignerlist.data()->addMasterSigner(master_signer_id,
                                                 master_signer_name,
                                                 device,
                                                 health,
                                                 signer_type);
        if(signer_type == (int)ENUNCHUCK::SignerType::SOFTWARE){
            AppModel::instance()->softwareSignerDeviceList()->addDevice(master_signer_name,
                                                                        type,
                                                                        path,
                                                                        model,
                                                                        master_fingerprint,
                                                                        false,
                                                                        needs_pass_phrase_sent,
                                                                        needs_pin_sent,
                                                                        master_signer_id);
        }
    }
    return masterSignerlist;
}

QDeviceListModelPtr bridge::nunchukGetDevices(QWarningMessage& msg) {
    std::vector<nunchuk::Device> deviceList_result = nunchukiface::instance()->GetDevices(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QDeviceListModelPtr deviceList(new DeviceListModel());
        for (nunchuk::Device it : deviceList_result) {
            QString type = QString::fromStdString(it.get_type());
            QString path = QString::fromStdString(it.get_path());
            QString model = QString::fromStdString(it.get_model());
            QString master_fingerprint = QString::fromStdString(it.get_master_fingerprint());
            bool connected = it.connected();
            bool needs_pass_phrase_sent = it.needs_pass_phrase_sent();
            bool needs_pin_sent = it.needs_pin_sent();

            QString signername = "";
            if(AppModel::instance()->masterSignerList()){
                signername = AppModel::instance()->masterSignerList()->getMasterSignerNameByFingerPrint(master_fingerprint);
            }
            deviceList.data()->addDevice(signername, type, path, model, master_fingerprint, connected, needs_pass_phrase_sent, needs_pin_sent);
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
    DBG_INFO << (int)ret << xfp << in_message << out_signature << out_path;
    return ret;
}

nunchuk::HealthStatus bridge::nunchukHealthCheckSingleSigner(const QSingleSignerPtr &signer,
                                                             QWarningMessage &msg)
{
    if(signer){
        QString message = signer.data()->message();
        QString signature = signer.data()->signature();
        DBG_INFO << message << signature;
        nunchuk::SingleSigner singlesinger(signer.data()->name().toStdString(),
                                           signer.data()->xpub().toStdString(),
                                           signer.data()->publickey().toStdString(),
                                           signer.data()->derivationPath().toStdString(),
                                           signer.data()->masterFingerPrint().toStdString(),
                                           signer.data()->lastHealthCheckDateTime().toTime_t(),
                                           signer.data()->masterSignerId().toStdString());
        singlesinger.set_type(nunchuk::SignerType(signer.data()->signerType()));
        return nunchukiface::instance()->HealthCheckSingleSigner(singlesinger,
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
    DBG_INFO << name << deviceIndex;
    QString devicePathSelected = "";
    if(NULL != AppModel::instance()->deviceList()){
        devicePathSelected = AppModel::instance()->deviceList()->getDevicePathByIndex(deviceIndex);
        QDeviceListModelPtr devices = bridge::nunchukGetDevices(msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            if(devices){
                QDevicePtr selectedDv = devices->getDeviceByPath(devicePathSelected);
                QMasterSignerPtr resultmasterSinger =  QMasterSignerPtr(new MasterSigner("", name, selectedDv));
                QString in_message = qUtils::QGenerateRandomMessage();
                resultmasterSinger.data()->setMessage(in_message);
                AppModel::instance()->setMsgKeyHealthcheck(in_message);
                if(selectedDv.data()){
                    DBG_INFO << "Device: " << selectedDv.data()->masterFingerPrint();
                    QString out_signature = "";
                    QString out_path = "";
                    AppModel::instance()->setAddSignerStep(1);
                    ENUNCHUCK::HealthStatus healthResult = (ENUNCHUCK::HealthStatus)nunchukHealthCheckMasterSigner(selectedDv.data()->masterFingerPrint(),
                                                                                                                   in_message,
                                                                                                                   out_signature,
                                                                                                                   out_path,
                                                                                                                   msg);
                    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                        resultmasterSinger.data()->setHealth((int)healthResult);
                        resultmasterSinger.data()->setPath(out_path);

                        if(ENUNCHUCK::HealthStatus::SUCCESS == healthResult){
                            nunchuk::Device dv(selectedDv.data()->type().toStdString(),
                                               selectedDv.data()->path().toStdString(),
                                               selectedDv.data()->model().toStdString(),
                                               selectedDv.data()->masterFingerPrint().toStdString(),
                                               selectedDv.data()->needsPassPhraseSent(),
                                               selectedDv.data()->needsPinSent());
                            AppModel::instance()->setAddSignerStep(2);
                            nunchuk::MasterSigner masterSigner = nunchukiface::instance()->CreateMasterSigner(name.toStdString(), dv, msg);
                            if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                                resultmasterSinger.data()->setId(QString::fromStdString(masterSigner.get_id()));
                                resultmasterSinger.data()->setName(QString::fromStdString(masterSigner.get_name()));
                                resultmasterSinger.data()->device()->setType(QString::fromStdString(masterSigner.get_device().get_type()));
                                resultmasterSinger.data()->device()->setPath(QString::fromStdString(masterSigner.get_device().get_path()));
                                resultmasterSinger.data()->device()->setModel(QString::fromStdString(masterSigner.get_device().get_model()));
                                resultmasterSinger.data()->device()->setMasterFingerPrint(QString::fromStdString(masterSigner.get_device().get_master_fingerprint()));
                                resultmasterSinger.data()->device()->setConnected(masterSigner.get_device().connected());
                                resultmasterSinger.data()->device()->setNeedsPassPhraseSent(masterSigner.get_device().needs_pass_phrase_sent());
                                resultmasterSinger.data()->device()->setNeedsPinSent(masterSigner.get_device().needs_pin_sent());
                                return resultmasterSinger;
                            }
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

int bridge::nunchukGetCurrentIndexFromMasterSigner(const QString &mastersigner_id,
                                                   const ENUNCHUCK::WalletType &wallet_type,
                                                   const ENUNCHUCK::AddressType &address_type,
                                                   QWarningMessage &msg)
{
    return nunchukiface::instance()->GetCurrentIndexFromMasterSigner(mastersigner_id.toStdString(),
                                                                     (nunchuk::WalletType)wallet_type,
                                                                     (nunchuk::AddressType)address_type, msg);
}

QSingleSignerPtr bridge::nunchukGetSignerFromMasterSigner(const QString &mastersigner_id, const ENUNCHUCK::WalletType &wallet_type, const ENUNCHUCK::AddressType &address_type, int index)
{
    QWarningMessage msg;
    nunchuk::SingleSigner signer = nunchukiface::instance()->GetSignerFromMasterSigner(mastersigner_id.toStdString(),
                                                                                       (nunchuk::WalletType)wallet_type,
                                                                                       (nunchuk::AddressType)address_type,
                                                                                       index,
                                                                                       msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
        ret.data()->setName(QString::fromStdString(signer.get_name()));
        ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
        ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
        ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
        ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
        ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
        ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
        ret.data()->setSignerType((int)signer.get_type());
        return ret;
    }
    else{
        return NULL;
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
        QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
        ret.data()->setName(QString::fromStdString(signer.get_name()));
        ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
        ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
        ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
        ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
        ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
        ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
        ret.data()->setSignerType((int)signer.get_type());
        return ret;
    }
    else{
        return NULL;
    }
}

QSingleSignerPtr bridge::nunchukCreateSigner(const QString &name,
                                             const QString &xpub,
                                             const QString &public_key,
                                             const QString &derivation_path,
                                             const QString &master_fingerprint,
                                             QWarningMessage& msg)
{
    nunchuk::SingleSigner signer = nunchukiface::instance()->CreateSigner(name.toStdString(),
                                                                          xpub.toStdString(),
                                                                          public_key.toStdString(),
                                                                          derivation_path.toStdString(),
                                                                          master_fingerprint.toStdString(),
                                                                          msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
        ret.data()->setName(QString::fromStdString(signer.get_name()));
        ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
        ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
        ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
        ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
        ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
        ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
        ret.data()->setSignerType((int)signer.get_type());
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
                                                        QWarningMessage &msg)
{
    return nunchukiface::instance()->CreateSigner(name.toStdString(),
                                                  xpub.toStdString(),
                                                  public_key.toStdString(),
                                                  derivation_path.toStdString(),
                                                  master_fingerprint.toStdString(),
                                                  msg);
}

QWalletPtr bridge::nunchukCreateWallet(const QString &name,
                                       int m,
                                       int n,
                                       SingleSignerListModel* signers,
                                       ENUNCHUCK::AddressType address_type,
                                       bool is_escrow,
                                       const QString& desc,
                                       QWarningMessage& msg)
{
    std::vector<nunchuk::SingleSigner> signerList;
    for (int i = 0; i < signers->rowCount(); i++) {
        QSingleSignerPtr singlerSigner = signers->getSingleSignerByIndex(i);
        nunchuk::SingleSigner nunchukSigner(singlerSigner.data()->name().toStdString(),
                                            singlerSigner.data()->xpub().toStdString(),
                                            singlerSigner.data()->publickey().toStdString(),
                                            singlerSigner.data()->derivationPath().toStdString(),
                                            singlerSigner.data()->masterFingerPrint().toStdString(),
                                            singlerSigner.data()->lastHealthCheckDateTime().toTime_t(),
                                            singlerSigner.data()->masterSignerId().toStdString());
        nunchukSigner.set_type(nunchuk::SignerType(singlerSigner.data()->signerType()));
        signerList.push_back(nunchukSigner);
    }
    nunchuk::Wallet walletResult = nunchukiface::instance()->CreateWallet(name.toStdString(),
                                                                          m,
                                                                          n,
                                                                          signerList,
                                                                          (nunchuk::AddressType)address_type,
                                                                          is_escrow,
                                                                          desc.toStdString(),
                                                                          msg);

    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : walletResult.get_signers()) {
            QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
            ret.data()->setName(QString::fromStdString(signer.get_name()));
            ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
            ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
            ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
            ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
            ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
            ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
            ret.data()->setSignerType((int)signer.get_type());
            signersAssinged.data()->addSingleSigner(ret);
        }

        QWalletPtr ret = QWalletPtr(new Wallet(QString::fromStdString(walletResult.get_id()),
                                               walletResult.get_m(),
                                               walletResult.get_n(),
                                               QString::fromStdString(walletResult.get_name()),
                                               QString::number((int)walletResult.get_address_type()),
                                               walletResult.get_balance(),
                                               QDateTime::fromTime_t(walletResult.get_create_date()),
                                               walletResult.is_escrow(),
                                               signersAssinged,
                                               QString::fromStdString(walletResult.get_description())));
        return ret;
    }
    else {
        return NULL;
    }
}

nunchuk::Wallet bridge::nunchukCreateOriginWallet(const QString &name,
                                                  int m,
                                                  int n,
                                                  SingleSignerListModel *signers,
                                                  ENUNCHUCK::AddressType address_type,
                                                  bool is_escrow,
                                                  const QString &desc,
                                                  QWarningMessage &msg)
{
    std::vector<nunchuk::SingleSigner> signerList;
    for (int i = 0; i < signers->rowCount(); i++) {
        QSingleSignerPtr singlerSigner = signers->getSingleSignerByIndex(i);
        nunchuk::SingleSigner nunchukSigner(singlerSigner.data()->name().toStdString(),
                                            singlerSigner.data()->xpub().toStdString(),
                                            singlerSigner.data()->publickey().toStdString(),
                                            singlerSigner.data()->derivationPath().toStdString(),
                                            singlerSigner.data()->masterFingerPrint().toStdString(),
                                            singlerSigner.data()->lastHealthCheckDateTime().toTime_t(),
                                            singlerSigner.data()->masterSignerId().toStdString());
        nunchukSigner.set_type(nunchuk::SignerType(singlerSigner.data()->signerType()));
        signerList.push_back(nunchukSigner);
    }
    return nunchukiface::instance()->CreateWallet(name.toStdString(),
                                                  m,
                                                  n,
                                                  signerList,
                                                  (nunchuk::AddressType)address_type,
                                                  is_escrow,
                                                  desc.toStdString(),
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
    std::vector<nunchuk::SingleSigner> signerList;
    for (int i = 0; i < signers->rowCount(); i++) {
        QSingleSignerPtr singlerSigner = signers->getSingleSignerByIndex(i);
        nunchuk::SingleSigner nunchukSigner(singlerSigner.data()->name().toStdString(),
                                            singlerSigner.data()->xpub().toStdString(),
                                            singlerSigner.data()->publickey().toStdString(),
                                            singlerSigner.data()->derivationPath().toStdString(),
                                            singlerSigner.data()->masterFingerPrint().toStdString(),
                                            singlerSigner.data()->lastHealthCheckDateTime().toTime_t(),
                                            singlerSigner.data()->masterSignerId().toStdString());
        nunchukSigner.set_type(nunchuk::SignerType(singlerSigner.data()->signerType()));
        signerList.push_back(nunchukSigner);
    }
    string ret = nunchukiface::instance()->DraftWallet(name.toStdString(),
                                                       m,
                                                       n,
                                                       signerList,
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
        QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : walletResult.get_signers()) {
            QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
            ret.data()->setName(QString::fromStdString(signer.get_name()));
            ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
            ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
            ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
            ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
            ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
            ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
            ret.data()->setSignerType((int)signer.get_type());
            signersAssinged.data()->addSingleSigner(ret);
        }

        QWalletPtr ret = QWalletPtr(new Wallet(QString::fromStdString(walletResult.get_id()),
                                               walletResult.get_m(),
                                               walletResult.get_n(),
                                               QString::fromStdString(walletResult.get_name()),
                                               QString::number((int)walletResult.get_address_type()),
                                               walletResult.get_balance(),
                                               QDateTime::fromTime_t(walletResult.get_create_date()),
                                               walletResult.is_escrow(),
                                               signersAssinged,
                                               QString::fromStdString(walletResult.get_description())));
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return ret;
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
        QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : walletResult.get_signers()) {
            QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
            ret.data()->setName(QString::fromStdString(signer.get_name()));
            ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
            ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
            ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
            ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
            ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
            ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
            ret.data()->setSignerType((int)signer.get_type());
            signersAssinged.data()->addSingleSigner(ret);
        }

        QWalletPtr ret = QWalletPtr(new Wallet(QString::fromStdString(walletResult.get_id()),
                                               walletResult.get_m(),
                                               walletResult.get_n(),
                                               QString::fromStdString(walletResult.get_name()),
                                               QString::number((int)walletResult.get_address_type()),
                                               walletResult.get_balance(),
                                               QDateTime::fromTime_t(walletResult.get_create_date()),
                                               walletResult.is_escrow(),
                                               signersAssinged,
                                               QString::fromStdString(walletResult.get_description())));
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return ret;
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
                                                                                    msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        final.data()->setStatus((int)nunchuk::TransactionStatus::PENDING_SIGNATURES);
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
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportTransaction(wallet_id.toStdString(),
                                                                                    file_path.toStdString(),
                                                                                    msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        if(final && final.data()->roomId() != "" && final.data()->initEventId() != ""){
            if(final.data()->roomId() != ""){
                QWarningMessage tmsg;
                std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(final.data()->roomId().toStdString(), tmsg);
                foreach (nunchuk::RoomTransaction tx, results) {
                    if(0 == tx.get_tx_id().compare(final.data()->txid().toStdString())){
                        final.data()->setInitEventId(QString::fromStdString(tx.get_init_event_id()));
                    }
                }
            }
            QWarningMessage msgSign;
            for(QSingleSignerPtr signer:final->singleSignersAssigned()->fullList()){
                if(signer->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP && signer->signerSigned() && final->singleSignersAssigned()->contains(signer->masterFingerPrint())){
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

QTransactionPtr bridge::nunchukSignTransaction(const QString &wallet_id,
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
            return bridge::convertTransaction(trans_result, wallet_id);
        }
    }
    return NULL;
}

nunchuk::Transaction bridge::nunchukSignTransactionThread(const QString &wallet_id,
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

void bridge::nunchukUpdateMasterSigner(const QString& mastersignerId, const QString &name )
{
    QWarningMessage msg;
    nunchuk::MasterSigner masterSigner = nunchukiface::instance()->GetMasterSigner(mastersignerId.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        masterSigner.set_name(name.toStdString());
        QtConcurrent::run([masterSigner]() {
            QWarningMessage msg;
            nunchukiface::instance()->UpdateMasterSigner(masterSigner, msg);
        });
    }
}

bool bridge::nunchukDeleteWallet(const QString &wallet_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->DeleteWallet(wallet_id.toStdString(), msg);
}

void bridge::nunchukUpdateWalletName(const QString &wallet_id, const QString &name)
{
    QWarningMessage msg;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        wallet.set_name(name.toStdString());
        QtConcurrent::run([wallet]() {
            QWarningMessage msgupdate;
            nunchukiface::instance()->UpdateWallet(wallet, msgupdate);
        });
    }
}

void bridge::nunchukUpdateWalletDescription(const QString &wallet_id, const QString &description)
{
    QWarningMessage msg;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        wallet.set_description(description.toStdString());
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

QUTXOListModelPtr bridge::nunchukGetUnspentOutputs(const QString &walletId)
{
    QWarningMessage msg;
    std::vector<nunchuk::UnspentOutput> utxo_result = nunchukiface::instance()->GetUnspentOutputs(walletId.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QUTXOListModelPtr ret = QUTXOListModelPtr(new UTXOListModel());
        for (nunchuk::UnspentOutput it : utxo_result) {
            ret.data()->addUTXO(QString::fromStdString(it.get_txid()),
                                it.get_vout(),
                                QString::fromStdString(it.get_address()),
                                it.get_amount(),
                                it.get_height(),
                                QString::fromStdString(it.get_memo()));
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
                                                const int fee_rate,
                                                const bool subtract_fee_from_amount,
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
                                                                                   msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        final.data()->setStatus((int)nunchuk::TransactionStatus::PENDING_SIGNATURES);
        return final;
    }
    else{
        return NULL;
    }
}

QTransactionPtr bridge::nunchukReplaceTransaction(const QString &wallet_id,
                                                  const QString &tx_id,
                                                  const int new_fee_rate,
                                                  QWarningMessage& msg)
{
    nunchuk::Transaction trans_result = nunchukiface::instance()->ReplaceTransaction(wallet_id.toStdString(),
                                                                                     tx_id.toStdString(),
                                                                                     new_fee_rate,
                                                                                     msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QTransactionPtr final = bridge::convertTransaction(trans_result, wallet_id);
        final.data()->setStatus((int)nunchuk::TransactionStatus::PENDING_SIGNATURES);
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

void bridge::nunchukBalanceChanged(const QString &walletId, const qint64 value)
{
    AppModel::instance()->startBalanceChanged(walletId, value);
}

void bridge::nunchukDevicesChanged(const QString &fingerprint, const bool connected)
{
//    AppModel::instance()->startScanDevices();
}

void bridge::nunchukTransactionChanged(const QString &tx_id, const int status, const QString &wallet_id)
{
    AppModel::instance()->startTransactionChanged(tx_id, status, wallet_id);
    AppModel::instance()->startGetTransactionHistory(wallet_id);
    AppModel::instance()->startGetUsedAddresses(wallet_id);
    AppModel::instance()->startGetUnusedAddresses(wallet_id);
    if(AppModel::instance()->walletInfo() && (0 != QString::compare(wallet_id, AppModel::instance()->walletInfo()->id(), Qt::CaseInsensitive))){
        AppModel::instance()->startGetTransactionHistory(AppModel::instance()->walletInfo()->id());
    }
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
    nunchuk::AppSettings ret;

    // Chain setting
    ret.set_chain((nunchuk::Chain)AppSetting::instance()->primaryServer());

    // mainnet sever
    std::vector<std::string> mainnetServer;
    mainnetServer.push_back(AppSetting::instance()->mainnetServer().toStdString());
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
    QString hwiPath = "";
    if(AppSetting::instance()->enableCustomizeHWIDriver()){
        hwiPath = AppSetting::instance()->hwiPath();
    }
    else{
        hwiPath = AppSetting::instance()->executePath() + "/hwi";
    }
    DBG_INFO << "SET HWI:" << hwiPath;
    ret.set_hwi_path(hwiPath.toStdString());

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
    DBG_INFO << "CERTIFICATE FILE:" << certPath;

    // Core RPC
    if(AppSetting::instance()->enableCoreRPC()){
        ret.set_backend_type(nunchuk::BackendType::CORERPC);
    }
    else{
        ret.set_backend_type(nunchuk::BackendType::ELECTRUM);
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
    return nunchukiface::instance()->ExportWallet(wallet_id.toStdString(),
                                                  file_path.toStdString(),
                                                  format, msg);
}

void bridge::nunchukSetPassphrase(const QString &passphrase, QWarningMessage& msg)
{
    nunchukiface::instance()->SetPassphrase(passphrase.toStdString(), msg);
}

int bridge::nunchukGetChainTip()
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
        QSingleSignerListModelPtr remoteSignerlist(new SingleSignerListModel());
        for (nunchuk::SingleSigner signer : remoteignerList_result) {
            QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
            ret.data()->setName(QString::fromStdString(signer.get_name()));
            ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
            ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
            ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
            ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
            ret.data()->setMasterSignerId("");
            ret.data()->setSignerType((int)signer.get_type());
            remoteSignerlist.data()->addSingleSigner(ret);
        }
        return remoteSignerlist;
    }
    else{
        return NULL;
    }
}

std::vector<nunchuk::SingleSigner> bridge::nunchukGetOriginRemoteSigners(QWarningMessage& msg)
{
    return nunchukiface::instance()->GetRemoteSigners(msg);
}

QSingleSignerListModelPtr bridge::nunchukConvertRemoteSigners(std::vector<nunchuk::SingleSigner> list)
{
    QSingleSignerListModelPtr remoteSignerlist(new SingleSignerListModel());
    for (nunchuk::SingleSigner signer : list) {
        QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
        ret.data()->setName(QString::fromStdString(signer.get_name()));
        ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
        ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
        ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
        ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
        ret.data()->setMasterSignerId("");
        ret.data()->setSignerType((int)signer.get_type());
        remoteSignerlist.data()->addSingleSigner(ret);
    }
    return remoteSignerlist;
}

void bridge::nunchukUpdateRemoteSigner(const QString &newname)
{
    if(AppModel::instance()->singleSignerInfo()) {
        nunchuk::SingleSigner nunchukSigner(newname.toStdString(),
                                            AppModel::instance()->singleSignerInfo()->xpub().toStdString(),
                                            AppModel::instance()->singleSignerInfo()->publickey().toStdString(),
                                            AppModel::instance()->singleSignerInfo()->derivationPath().toStdString(),
                                            AppModel::instance()->singleSignerInfo()->masterFingerPrint().toStdString(),
                                            AppModel::instance()->singleSignerInfo()->lastHealthCheckDateTime().toTime_t(),
                                            AppModel::instance()->singleSignerInfo()->masterSignerId().toStdString());
        nunchukSigner.set_type(nunchuk::SignerType(AppModel::instance()->singleSignerInfo()->signerType()));
        QtConcurrent::run([nunchukSigner]() {
            QWarningMessage msg;
            nunchukiface::instance()->UpdateRemoteSigner(nunchukSigner, msg);
        });
    }
}

bool bridge::nunchukDisplayAddressOnDevice(const QString &wallet_id, const QString &address, const QString &device_fingerprint)
{
    QWarningMessage msg;
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
        nunchuk::Device dv(device.data()->type().toStdString(),
                           device.data()->path().toStdString(),
                           device.data()->model().toStdString(),
                           device.data()->masterFingerPrint().toStdString(),
                           device.data()->needsPassPhraseSent(),
                           device.data()->needsPinSent());
        nunchukiface::instance()->PromtPinOnDevice(dv, msg);
    }
}

void bridge::nunchukSendPinToDevice(const QDevicePtr &device, const QString &pin, QWarningMessage &msg)
{
    if(device.data()){
        nunchuk::Device dv(device.data()->type().toStdString(),
                           device.data()->path().toStdString(),
                           device.data()->model().toStdString(),
                           device.data()->masterFingerPrint().toStdString(),
                           device.data()->needsPassPhraseSent(),
                           device.data()->needsPinSent());
        nunchukiface::instance()->SendPinToDevice(dv, pin.toStdString(), msg);
    }
}

void bridge::nunchukSendPassphraseToDevice(const QDevicePtr &device, const QString &passphrase, QWarningMessage &msg)
{
    if(device.data()){
        nunchuk::Device dv(device.data()->type().toStdString(),
                           device.data()->path().toStdString(),
                           device.data()->model().toStdString(),
                           device.data()->masterFingerPrint().toStdString(),
                           device.data()->needsPassPhraseSent(),
                           device.data()->needsPinSent());
        nunchukiface::instance()->SendPassphraseToDevice(dv, passphrase.toStdString(), msg);
    }
}

QSingleSignerPtr bridge::nunchukCreateCoboSigner(const QString &name, const QString &json_info, QWarningMessage &msg)
{
    nunchuk::SingleSigner signer = nunchukiface::instance()->CreateCoboSigner(name.toStdString(),
                                                                              json_info.toStdString(),
                                                                              msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
        ret.data()->setName(QString::fromStdString(signer.get_name()));
        ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
        ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
        ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
        ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
        ret.data()->setMasterSignerId("");
        ret.data()->setSignerType((int)signer.get_type());
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
        QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : walletResult.get_signers()) {
            QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
            ret.data()->setName(QString::fromStdString(signer.get_name()));
            ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
            ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
            ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
            ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
            ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
            ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
            ret.data()->setSignerType((int)signer.get_type());
            signersAssinged.data()->addSingleSigner(ret);
        }

        QWalletPtr ret = QWalletPtr(new Wallet(QString::fromStdString(walletResult.get_id()),
                                               walletResult.get_m(),
                                               walletResult.get_n(),
                                               QString::fromStdString(walletResult.get_name()),
                                               QString::number((int)walletResult.get_address_type()),
                                               walletResult.get_balance(),
                                               QDateTime::fromTime_t(walletResult.get_create_date()),
                                               walletResult.is_escrow(),
                                               signersAssinged,
                                               QString::fromStdString(walletResult.get_description())));
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return ret;
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
        QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : walletResult.get_signers()) {
            QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
            ret.data()->setName(QString::fromStdString(signer.get_name()));
            ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
            ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
            ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
            ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
            ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
            ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
            ret.data()->setSignerType((int)signer.get_type());
            signersAssinged.data()->addSingleSigner(ret);
        }

        QWalletPtr ret = QWalletPtr(new Wallet(QString::fromStdString(walletResult.get_id()),
                                               walletResult.get_m(),
                                               walletResult.get_n(),
                                               QString::fromStdString(walletResult.get_name()),
                                               QString::number((int)walletResult.get_address_type()),
                                               walletResult.get_balance(),
                                               QDateTime::fromTime_t(walletResult.get_create_date()),
                                               walletResult.is_escrow(),
                                               signersAssinged,
                                               QString::fromStdString(walletResult.get_description())));
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return ret;
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

QMasterSignerPtr bridge::nunchukCreateSoftwareSigner(const QString &name, const QString &mnemonic, const QString &passphrase, QWarningMessage &msg,bool isPrimaryKey)
{
    QMasterSignerPtr resultSoftSinger =  QMasterSignerPtr(new MasterSigner());
    resultSoftSinger.data()->setName(name);
    nunchuk::MasterSigner masterSigner = nunchukiface::instance()->CreateSoftwareSigner(name.toStdString(),
                                                                                        mnemonic.toStdString(),
                                                                                        passphrase.toStdString(),
                                                                                        msg,
                                                                                        isPrimaryKey);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        resultSoftSinger.data()->setId(QString::fromStdString(masterSigner.get_id()));
        resultSoftSinger.data()->setName(QString::fromStdString(masterSigner.get_name()));
        resultSoftSinger.data()->device()->setType(QString::fromStdString(masterSigner.get_device().get_type()));
        resultSoftSinger.data()->device()->setPath(QString::fromStdString(masterSigner.get_device().get_path()));
        resultSoftSinger.data()->device()->setModel(QString::fromStdString(masterSigner.get_device().get_model()));
        resultSoftSinger.data()->device()->setMasterFingerPrint(QString::fromStdString(masterSigner.get_device().get_master_fingerprint()));
        resultSoftSinger.data()->device()->setConnected(/*masterSigner.get_device().connected()*/false);
        resultSoftSinger.data()->device()->setNeedsPassPhraseSent(masterSigner.get_device().needs_pass_phrase_sent());
        resultSoftSinger.data()->device()->setNeedsPinSent(masterSigner.get_device().needs_pin_sent());
        resultSoftSinger.data()->setSignerType((int)masterSigner.get_type());
        return resultSoftSinger;
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
    emit AppModel::instance()->finishSendPassphraseToDevice(msg.type());
}

QStringList bridge::nunchuckGetBIP39WordList()
{
    return qUtils::GetBIP39WordList();
}

void bridge::nunchukClearSignerPassphrase(const QString &mastersigner_id, QWarningMessage &msg)
{
    nunchukiface::instance()->ClearSignerPassphrase(mastersigner_id.toStdString(), msg);
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
        if(final && final.data()->roomId() != "" && final.data()->initEventId() != ""){
            if(final.data()->roomId() != ""){
                QWarningMessage tmsg;
                std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(final.data()->roomId().toStdString(), tmsg);
                foreach (nunchuk::RoomTransaction tx, results) {
                    if(0 == tx.get_tx_id().compare(final.data()->txid().toStdString())){
                        final.data()->setInitEventId(QString::fromStdString(tx.get_init_event_id()));
                    }
                }
            }
            QWarningMessage msgSign;
            for(QSingleSignerPtr signer:final->singleSignersAssigned()->fullList()){
                if(signer->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP && signer->signerSigned() && final->singleSignersAssigned()->contains(signer->masterFingerPrint())){
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
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportKeystoneWallet(qr_result,
                                                                                  description.toStdString(),
                                                                                  msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : walletResult.get_signers()) {
            QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
            ret.data()->setName(QString::fromStdString(signer.get_name()));
            ret.data()->setXpub(QString::fromStdString(signer.get_xpub()));
            ret.data()->setPublickey(QString::fromStdString(signer.get_public_key()));
            ret.data()->setDerivationPath(QString::fromStdString(signer.get_derivation_path()));
            ret.data()->setMasterFingerPrint(QString::fromStdString(signer.get_master_fingerprint()));
            ret.data()->setMasterSignerId(QString::fromStdString(signer.get_master_signer_id()));
            ret.data()->setlastHealthCheck(QDateTime::fromTime_t(signer.get_last_health_check()));
            ret.data()->setSignerType((int)signer.get_type());
            signersAssinged.data()->addSingleSigner(ret);
        }

        QWalletPtr ret = QWalletPtr(new Wallet(QString::fromStdString(walletResult.get_id()),
                                               walletResult.get_m(),
                                               walletResult.get_n(),
                                               QString::fromStdString(walletResult.get_name()),
                                               QString::number((int)walletResult.get_address_type()),
                                               walletResult.get_balance(),
                                               QDateTime::fromTime_t(walletResult.get_create_date()),
                                               walletResult.is_escrow(),
                                               signersAssinged,
                                               QString::fromStdString(walletResult.get_description())));
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return ret;
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
    std::vector<nunchuk::SingleSigner> signers = nunchukiface::instance()->ParsePassportSigners(qr_in,
                                                                                                msg);
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
    std::vector<nunchuk::SingleSigner> signers = nunchukiface::instance()->ParseQRSigners(qr_in,
                                                                                          msg);
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

QString bridge::nunchukParseJSONSigners(const QString &filePathName)
{
    DBG_INFO << filePathName;
    QFile file(filePathName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";
    QTextStream in(&file);
    QString json_str = in.readAll();
    DBG_INFO << json_str;
    file.close();
    if(json_str.isEmpty()) return "";
    QJsonObject ret;
    QWarningMessage msg;

    std::vector<nunchuk::SingleSigner> signers = nunchukiface::instance()->ParseJSONSigners(json_str.toStdString(),
                                                                                          msg);
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

QStringList bridge::nunchukExportPassportWallet(const QString &wallet_id,
                                                QWarningMessage &msg)
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
        if(final && final.data()->roomId() != "" && final.data()->initEventId() != ""){
            if(final.data()->roomId() != ""){
                QWarningMessage tmsg;
                std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(final.data()->roomId().toStdString(), tmsg);
                foreach (nunchuk::RoomTransaction tx, results) {
                    if(0 == tx.get_tx_id().compare(final.data()->txid().toStdString())){
                        final.data()->setInitEventId(QString::fromStdString(tx.get_init_event_id()));
                    }
                }
            }
            QWarningMessage msgSign;
            for(QSingleSignerPtr signer:final->singleSignersAssigned()->fullList()){
                if(signer->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP && signer->signerSigned() && final->singleSignersAssigned()->contains(signer->masterFingerPrint())){
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
    nunchukiface::instance()->stopInstance();
}

bool bridge::nunchukHasSinger(const nunchuk::SingleSigner &signer)
{
    QWarningMessage msg;
    return nunchukiface::instance()->HasSigner(signer,msg);
}
