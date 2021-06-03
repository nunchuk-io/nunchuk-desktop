#include "bridgeifaces.h"
#include "QWarningMessage.h"
#include <QTextCodec>

bool bridge::nunchukMakeInstance(const QString& passphrase, QWarningMessage& msg)
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

    // hwi path
    QString hwiPath = "";
    if(AppSetting::instance()->enableCustomizeHWIDriver()){
        hwiPath = AppSetting::instance()->hwiPath();
    }
    else{
        hwiPath = AppSetting::instance()->executePath() + "/hwi";
    }
    DBG_INFO << "HWI:" << hwiPath;
    ret.set_hwi_path(hwiPath.toStdString());

    //  certificate file
    QString certPath = "";
    if(AppSetting::instance()->enableCertificateFile()){
        certPath = AppSetting::instance()->certificateFile();
    }
    ret.set_certificate_file(certPath.toStdString());
    DBG_INFO << "CERTIFICATE FILE:" << certPath;

    // Storage path
    ret.set_storage_path(AppSetting::instance()->storagePath().toStdString());

    ret.enable_proxy(AppSetting::instance()->enableTorProxy());
    ret.set_proxy_host(AppSetting::instance()->torProxyAddress().toStdString());
    ret.set_proxy_port(AppSetting::instance()->torProxyPort());
    ret.set_proxy_username(AppSetting::instance()->torProxyName().toStdString());
    ret.set_proxy_password(AppSetting::instance()->torProxyPassword().toStdString());

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

    bool makeRet = nunchukiface::instance()->makeNunchukInstance(ret, passphrase.toStdString(), msg);
    if(makeRet && (int)EWARNING::WarningType::NONE_MSG == msg.type()){
        int currentChainTip = nunchukGetChainTip();
        AppModel::instance()->setChainTip(currentChainTip);
        DBG_INFO << makeRet;
        return makeRet;
    }
    else{
        return false;
    }
}

QSharedPointer<Transaction> bridge::convertTransaction(nunchuk::Transaction in)
{
    QSharedPointer<Transaction> ret = QSharedPointer<Transaction>(new Transaction());
    ret.data()->setTxid(QString::fromStdString(in.get_txid()));
    ret.data()->setHeight(in.get_height());
    ret.data()->setMemo(QString::fromStdString(in.get_memo()));
    ret.data()->setM(in.get_m());
    ret.data()->setSubtractFromFeeAmount(in.subtract_fee_from_amount());
    int index_change = in.get_change_index();
    if(index_change == -1){
        ret.data()->setHasChange(false);
    }
    else{
        ret.data()->setHasChange(true);
        std::pair<std::string, int> change_ret = in.get_outputs().at(index_change);
        QSharedPointer<Destination> change = QSharedPointer<Destination>(new Destination());
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

    ret.data()->setSingleSignersAssigned(AppModel::instance()->walletInfo()->singleSignersAssignedPtr());
    std::map<std::string, bool> signers = in.get_signers();
    int numsigned = 0;
    for ( std::map<std::string, bool>::iterator it = signers.begin(); it != signers.end(); it++ )
    {
        ret.data()->singleSignersAssigned()->updateSignatures(QString::fromStdString(it->first), it->second);
        if(it->second) { numsigned++; }
    }
    ret.data()->setNumberSigned(numsigned);

    qint64 subtotal = 0;
    // Check tx is send or receives
    bool isReceiveTx = in.is_receive();
    ret.data()->setIsReceiveTx(isReceiveTx);
    if(ret.data()->isReceiveTx()){
        subtotal = in.get_sub_amount();
        QSharedPointer<DestinationListModel> recevedAddr = QSharedPointer<DestinationListModel>(new DestinationListModel());
        // Get receive address
        for (std::pair<std::string, nunchuk::Amount> item : in.get_receive_outputs()) {
            recevedAddr.data()->addDestination(QString::fromStdString(item.first), item.second);
        }
        ret.data()->setDestinationList(recevedAddr);
    }
    else{
        QSharedPointer<DestinationListModel> dest = QSharedPointer<DestinationListModel>(new DestinationListModel());
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
    nunchuk::TransactionStatus txstate = in.get_status();
    ret.data()->setStatus((int)txstate);
    DBG_INFO << "From client: subtotal :" << subtotal << " Fee:" << fee <<  " Receive:" << isReceiveTx << " Status:" << (int)txstate;

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

QSharedPointer<WalletListModel> bridge::nunchukGetWallets() {
    QWarningMessage msg;
    std::vector<nunchuk::Wallet> resultWallets = nunchukiface::instance()->GetWallets(msg);
    DBG_INFO << resultWallets.size();
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSharedPointer<WalletListModel> walletList(new WalletListModel());
        for (nunchuk::Wallet it : resultWallets) {
            std::vector<nunchuk::SingleSigner> resultSigners = it.get_signers();
            QSharedPointer<SingleSignerListModel> signersAssinged( new SingleSignerListModel);
            for (nunchuk::SingleSigner signer : resultSigners) {
                QString signer_name = QString::fromStdString(signer.get_name());
                QString signer_xpub = QString::fromStdString(signer.get_xpub());
                QString signer_public_key = QString::fromStdString(signer.get_public_key());
                QString signer_derivation_path = QString::fromStdString(signer.get_derivation_path());
                QString signer_master_fingerprint = QString::fromStdString(signer.get_master_fingerprint());
                QString signer_master_signer_id = QString::fromStdString(signer.get_master_signer_id());
                QDateTime signer_last_health_check = QDateTime::fromTime_t(signer.get_last_health_check());
                signersAssinged.data()->addSingleSigner(signer_name,
                                                        signer_xpub,
                                                        signer_public_key,
                                                        signer_derivation_path,
                                                        signer_master_fingerprint,
                                                        signer_master_signer_id,
                                                        signer_last_health_check);
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

QSharedPointer<Wallet> bridge::nunchukGetWallet(const QString &wallet_id)
{
    QWarningMessage msg;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        DBG_INFO << "BALANCE FROM CLIENT" << wallet.get_balance();
        QSharedPointer<Wallet> walletResult = QSharedPointer<Wallet>(new Wallet());
        walletResult.data()->setId(QString::fromStdString(wallet.get_id()));
        walletResult.data()->setM(wallet.get_m());
        walletResult.data()->setN(wallet.get_n());
        walletResult.data()->setName(QString::fromStdString(wallet.get_name()));
        walletResult.data()->setAddressType(QString::number((int)wallet.get_address_type()));
        walletResult.data()->setBalance(wallet.get_balance());
        walletResult.data()->setCreateDate(QDateTime::fromTime_t(wallet.get_create_date()));
        walletResult.data()->setEscrow(wallet.is_escrow());
        std::vector<nunchuk::SingleSigner> resultSigners = wallet.get_signers();
        QSharedPointer<SingleSignerListModel> signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : resultSigners) {
            QString signer_name = QString::fromStdString(signer.get_name());
            QString signer_xpub = QString::fromStdString(signer.get_xpub());
            QString signer_public_key = QString::fromStdString(signer.get_public_key());
            QString signer_derivation_path = QString::fromStdString(signer.get_derivation_path());
            QString signer_master_fingerprint = QString::fromStdString(signer.get_master_fingerprint());
            QString signer_master_signer_id = QString::fromStdString(signer.get_master_signer_id());
            QDateTime signer_last_health_check = QDateTime::fromTime_t(signer.get_last_health_check());
            signersAssinged.data()->addSingleSigner(signer_name,
                                                    signer_xpub,
                                                    signer_public_key,
                                                    signer_derivation_path,
                                                    signer_master_fingerprint,
                                                    signer_master_signer_id,
                                                    signer_last_health_check);
        }
        walletResult.data()->setSigners(signersAssinged);
        return walletResult;
    }
    else{
        return NULL;
    }
}

QSharedPointer<MasterSignerListModel> bridge::nunchukGetMasterSigners() {
    QWarningMessage msg;
    std::vector<nunchuk::MasterSigner> masterSignerList_result = nunchukiface::instance()->GetMasterSigners(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSharedPointer<MasterSignerListModel> masterSignerlist(new MasterSignerListModel());
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
            bool is_software = it.is_software();
            int health = -1;

            QSharedPointer<Device> device(new Device(master_signer_name,
                                                     type, path, model,
                                                     master_fingerprint,
                                                     connected,
                                                     needs_pass_phrase_sent,
                                                     needs_pin_sent));
            masterSignerlist.data()->addMasterSigner(master_signer_id,
                                                     master_signer_name,
                                                     device,
                                                     health,
                                                     is_software);

            if(is_software && AppModel::instance()->softwareSignerDeviceList()){
                AppModel::instance()->softwareSignerDeviceList()->addDevice(master_signer_name,
                                                                            type,
                                                                            path,
                                                                            model,
                                                                            master_fingerprint,
                                                                            false,
                                                                            needs_pass_phrase_sent,
                                                                            needs_pin_sent,
                                                                            master_signer_id);
                DBG_INFO << "SOFTWARE SIGNER" << master_signer_name << master_fingerprint;
            }
        }
        return masterSignerlist;
    }
    else{
        return NULL;
    }
}

QSharedPointer<DeviceListModel> bridge::nunchukGetDevices() {
    QWarningMessage msg;
    std::vector<nunchuk::Device> deviceList_result = nunchukiface::instance()->GetDevices(msg);
    QSharedPointer<DeviceListModel> deviceList(new DeviceListModel());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
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
    }
    else{
        deviceList.data()->warningMessage()->setWarningMessage(msg.code(),
                                                               msg.what(),
                                                               (EWARNING::WarningType)msg.type(),
                                                               "Can not scan devices");
    }
    return deviceList;
}

nunchuk::HealthStatus bridge::nunchukHealthCheckMasterSigner(const QString& signerId,
                                                             const QString& message,
                                                             QString& signature,
                                                             QString& path,
                                                             QWarningMessage& msg){
    std::string in_message = message.toStdString();
    std::string out_signature = "";
    std::string out_path = "";
    nunchuk::HealthStatus ret = nunchukiface::instance()->HealthCheckMasterSigner(signerId.toStdString(), in_message, out_signature, out_path, msg);
    signature = QString::fromStdString(out_signature);
    path = QString::fromStdString(out_path);
    DBG_INFO << signerId << in_message << out_signature << out_path;
    return ret;
}

nunchuk::HealthStatus bridge::nunchukHealthCheckSingleSigner(const QSharedPointer<SingleSigner> &signer,
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
        return nunchukiface::instance()->HealthCheckSingleSigner(singlesinger,
                                                                 message.toStdString(),
                                                                 signature.toStdString(),
                                                                 msg);
    }
    else{
        return nunchuk::HealthStatus::NO_SIGNATURE;
    }
}

QSharedPointer<MasterSigner> bridge::nunchukCreateMasterSigner(const QString& name, const int deviceIndex, QWarningMessage &msg)
{
    DBG_INFO << name << deviceIndex;
    QString devicePathSelected = "";
    if(NULL != AppModel::instance()->deviceList()){
        devicePathSelected = AppModel::instance()->deviceList()->getDevicePathByIndex(deviceIndex);
        DBG_INFO << devicePathSelected;
        QSharedPointer<DeviceListModel> devices = bridge::nunchukGetDevices();
        DBG_INFO << devices;
        if(devices){
            QSharedPointer<Device> selectedDv = devices->getDeviceByPath(devicePathSelected);
            DBG_INFO << "Device: " << selectedDv;
            QSharedPointer<MasterSigner> resultmasterSinger =  QSharedPointer<MasterSigner>(new MasterSigner("", name, selectedDv));
            if(AppModel::instance()->masterSignerInfo()){
                resultmasterSinger.data()->setMessage(AppModel::instance()->masterSignerInfo()->message());
            }

            if(selectedDv.data()){
                QString out_signature = "";
                QString out_path = "";
                AppModel::instance()->setAddSignerStep(1);
                ENUNCHUCK::HealthStatus healthResult = (ENUNCHUCK::HealthStatus)nunchukHealthCheckMasterSigner(selectedDv.data()->masterFingerPrint(),
                                                                                                               resultmasterSinger.data()->message(),
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

QSharedPointer<SingleSigner> bridge::nunchukGetSignerFromMasterSigner(const QString &mastersigner_id, const ENUNCHUCK::WalletType &wallet_type, const ENUNCHUCK::AddressType &address_type, int index)
{
    QWarningMessage msg;
    nunchuk::SingleSigner signer = nunchukiface::instance()->GetSignerFromMasterSigner(mastersigner_id.toStdString(),
                                                                                       (nunchuk::WalletType)wallet_type,
                                                                                       (nunchuk::AddressType)address_type,
                                                                                       index,
                                                                                       msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QString signer_name = QString::fromStdString(signer.get_name());
        QString signer_xpub = QString::fromStdString(signer.get_xpub());
        QString signer_public_key = QString::fromStdString(signer.get_public_key());
        QString signer_derivation_path = QString::fromStdString(signer.get_derivation_path());
        QString signer_master_fingerprint = QString::fromStdString(signer.get_master_fingerprint());
        QString signer_master_signer_id = QString::fromStdString(signer.get_master_signer_id());
        QDateTime signer_last_health_check = QDateTime::fromTime_t(signer.get_last_health_check());

        return QSharedPointer<SingleSigner>(new SingleSigner(signer_name,
                                                             signer_xpub,
                                                             signer_public_key,
                                                             signer_derivation_path,
                                                             signer_master_fingerprint,
                                                             signer_master_signer_id,
                                                             signer_last_health_check));

    }
    else{
        return NULL;
    }
}

QSharedPointer<SingleSigner> bridge::nunchukGetUnusedSignerFromMasterSigner(const QString &mastersigner_id, const ENUNCHUCK::WalletType &wallet_type, const ENUNCHUCK::AddressType &address_type)
{
    QWarningMessage msg;
    nunchuk::SingleSigner signer = nunchukiface::instance()->GetUnusedSignerFromMasterSigner(mastersigner_id.toStdString(),
                                                                                             (nunchuk::WalletType)wallet_type,
                                                                                             (nunchuk::AddressType)address_type,
                                                                                             msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QString signer_name = QString::fromStdString(signer.get_name());
        QString signer_xpub = QString::fromStdString(signer.get_xpub());
        QString signer_public_key = QString::fromStdString(signer.get_public_key());
        QString signer_derivation_path = QString::fromStdString(signer.get_derivation_path());
        QString signer_master_fingerprint = QString::fromStdString(signer.get_master_fingerprint());
        QString signer_master_signer_id = QString::fromStdString(signer.get_master_signer_id());
        QDateTime signer_last_health_check = QDateTime::fromTime_t(signer.get_last_health_check());
        DBG_INFO << "master ID :" << mastersigner_id << "Master ID-client:" << signer_master_signer_id;
        return QSharedPointer<SingleSigner>(new SingleSigner(signer_name,
                                                             signer_xpub,
                                                             signer_public_key,
                                                             signer_derivation_path,
                                                             signer_master_fingerprint,
                                                             signer_master_signer_id,
                                                             signer_last_health_check));
    }
    else{
        return NULL;
    }
}

QSharedPointer<SingleSigner> bridge::nunchukCreateSigner(const QString &name,
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
        QString signer_name = QString::fromStdString(signer.get_name());
        QString signer_xpub = QString::fromStdString(signer.get_xpub());
        QString signer_public_key = QString::fromStdString(signer.get_public_key());
        QString signer_derivation_path = QString::fromStdString(signer.get_derivation_path());
        QString signer_master_fingerprint = QString::fromStdString(signer.get_master_fingerprint());
        QString signer_master_signer_id = QString::fromStdString(signer.get_master_signer_id());
        QDateTime signer_last_health_check = QDateTime::fromTime_t(signer.get_last_health_check());

        return QSharedPointer<SingleSigner>(new SingleSigner(signer_name,
                                                             signer_xpub,
                                                             signer_public_key,
                                                             signer_derivation_path,
                                                             signer_master_fingerprint,
                                                             signer_master_signer_id,
                                                             signer_last_health_check));
    }
    else{
        return NULL;
    }
}

QSharedPointer<Wallet> bridge::nunchukCreateWallet(const QString &name,
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
        QSharedPointer<SingleSigner> singlerSigner = signers->getSingleSignerByIndex(i);
        signerList.push_back(nunchuk::SingleSigner(singlerSigner.data()->name().toStdString(),
                                                   singlerSigner.data()->xpub().toStdString(),
                                                   singlerSigner.data()->publickey().toStdString(),
                                                   singlerSigner.data()->derivationPath().toStdString(),
                                                   singlerSigner.data()->masterFingerPrint().toStdString(),
                                                   singlerSigner.data()->lastHealthCheckDateTime().toTime_t(),
                                                   singlerSigner.data()->masterSignerId().toStdString()));
        DBG_INFO << "SEND: SIGNER:" << singlerSigner.data()->name()
                 << " | xpub:" << singlerSigner.data()->xpub()
                 << " | public key:" << singlerSigner.data()->publickey()
                 << " | bip32 path:" << singlerSigner.data()->derivationPath()
                 << " | fingerprint:" << singlerSigner.data()->masterFingerPrint()
                 << " | master id:" << singlerSigner.data()->masterSignerId()
                 << " | last check:" << singlerSigner.data()->lastHealthCheckDateTime();
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
        QSharedPointer<SingleSignerListModel> signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner it : walletResult.get_signers()) {
            QString signer_name = QString::fromStdString(it.get_name());
            QString signer_xpub = QString::fromStdString(it.get_xpub());
            QString signer_public_key = QString::fromStdString(it.get_public_key());
            QString signer_derivation_path = QString::fromStdString(it.get_derivation_path());
            QString signer_master_fingerprint = QString::fromStdString(it.get_master_fingerprint());
            QString signer_master_signer_id = QString::fromStdString(it.get_master_signer_id());
            QDateTime signer_last_health_check = QDateTime::fromTime_t(it.get_last_health_check());
            signersAssinged.data()->addSingleSigner(signer_name,
                                                    signer_xpub,
                                                    signer_public_key,
                                                    signer_derivation_path,
                                                    signer_master_fingerprint,
                                                    signer_master_signer_id,
                                                    signer_last_health_check);

            DBG_INFO << "RECEIVED: SIGNER:" << signer_name
                     << " | txpub:" << signer_xpub
                     << " | tpublic key:" << signer_public_key
                     << " | tbip32 path:" << signer_derivation_path
                     << " | fingerprint:" << signer_master_fingerprint
                     << " | tmaster id:" << signer_master_signer_id
                     << " | tlast check:" << signer_last_health_check;
        }

        QSharedPointer<Wallet> ret = QSharedPointer<Wallet>(new Wallet(QString::fromStdString(walletResult.get_id()),
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
        QSharedPointer<SingleSigner> singlerSigner = signers->getSingleSignerByIndex(i);
        signerList.push_back(nunchuk::SingleSigner(singlerSigner.data()->name().toStdString(),
                                                   singlerSigner.data()->xpub().toStdString(),
                                                   singlerSigner.data()->publickey().toStdString(),
                                                   singlerSigner.data()->derivationPath().toStdString(),
                                                   singlerSigner.data()->masterFingerPrint().toStdString(),
                                                   singlerSigner.data()->lastHealthCheckDateTime().toTime_t(),
                                                   singlerSigner.data()->masterSignerId().toStdString()));
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
    QStringList ret;
    ret.clear();
    std::vector<std::string> addrs = nunchukiface::instance()->GetAddresses(walletId.toStdString(), false, internal, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        if(0 == addrs.size()){
            QString newaddr = bridge::nunchukGenNewAddresses(walletId, internal);
            addrs = nunchukiface::instance()->GetAddresses(walletId.toStdString(), false, internal, msg);
            DBG_INFO << "Actively generate new address when have no remaining unused address" << newaddr;
        }
        for (std::string it : addrs) {
            ret  << QString::fromStdString(it);
        }
    }
    return ret;
}

QStringList bridge::nunchukGetUsedAddresses(const QString &walletId, bool internal)
{
    QWarningMessage msg;
    QStringList ret;
    ret.clear();
    std::vector<std::string> addrs = nunchukiface::instance()->GetAddresses(walletId.toStdString(), true, internal, msg);
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

QSharedPointer<Wallet> bridge::nunchukImportWallet(const QString &dbFile)
{
    QWarningMessage msg;
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportWalletDb(dbFile.toStdString(), msg);
    if((int)EWARNING::WarningType::EXCEPTION_MSG != msg.type()){
        QSharedPointer<SingleSignerListModel> signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner it : walletResult.get_signers()) {
            QString signer_name = QString::fromStdString(it.get_name());
            QString signer_xpub = QString::fromStdString(it.get_xpub());
            QString signer_public_key = QString::fromStdString(it.get_public_key());
            QString signer_derivation_path = QString::fromStdString(it.get_derivation_path());
            QString signer_master_fingerprint = QString::fromStdString(it.get_master_fingerprint());
            QString signer_master_signer_id = QString::fromStdString(it.get_master_signer_id());
            QDateTime signer_last_health_check = QDateTime::fromTime_t(it.get_last_health_check());
            signersAssinged.data()->addSingleSigner(signer_name,
                                                    signer_xpub,
                                                    signer_public_key,
                                                    signer_derivation_path,
                                                    signer_master_fingerprint,
                                                    signer_master_signer_id,
                                                    signer_last_health_check);
        }

        QSharedPointer<Wallet> ret = QSharedPointer<Wallet>(new Wallet(QString::fromStdString(walletResult.get_id()),
                                                                       walletResult.get_m(),
                                                                       walletResult.get_n(),
                                                                       QString::fromStdString(walletResult.get_name()),
                                                                       QString::number((int)walletResult.get_address_type()),
                                                                       walletResult.get_balance(),
                                                                       QDateTime::fromTime_t(walletResult.get_create_date()),
                                                                       walletResult.is_escrow(),
                                                                       signersAssinged,
                                                                       QString::fromStdString(walletResult.get_description())));

        ret.data()->warningMessage()->setWarningMessage(0,
                                                        "Your wallet has been imported.",
                                                        EWARNING::WarningType::SUCCESS_MSG,
                                                        "");
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return ret;
    }
    else {
        QSharedPointer<Wallet> retfail = QSharedPointer<Wallet>(new Wallet());
        retfail.data()->warningMessage()->setWarningMessage(msg.code(),
                                                            msg.what(),
                                                            (EWARNING::WarningType)msg.type(),
                                                            "Can not import Wallet");
        AppModel::instance()->newWalletInfo()->setCapableCreate(false);
        return retfail;
    }
}


QSharedPointer<Wallet> bridge::nunchukImportWalletDescriptor(const QString &dbFile, const QString& name, const QString& description)
{
    QWarningMessage msg;
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportWalletDescriptor(dbFile.toStdString(),
                                                                                    name.toStdString(),
                                                                                    description.toStdString(),
                                                                                    msg);
    if((int)EWARNING::WarningType::EXCEPTION_MSG != msg.type()){
        QSharedPointer<SingleSignerListModel> signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner it : walletResult.get_signers()) {
            QString signer_name = QString::fromStdString(it.get_name());
            QString signer_xpub = QString::fromStdString(it.get_xpub());
            QString signer_public_key = QString::fromStdString(it.get_public_key());
            QString signer_derivation_path = QString::fromStdString(it.get_derivation_path());
            QString signer_master_fingerprint = QString::fromStdString(it.get_master_fingerprint());
            QString signer_master_signer_id = QString::fromStdString(it.get_master_signer_id());
            QDateTime signer_last_health_check = QDateTime::fromTime_t(it.get_last_health_check());
            signersAssinged.data()->addSingleSigner(signer_name,
                                                    signer_xpub,
                                                    signer_public_key,
                                                    signer_derivation_path,
                                                    signer_master_fingerprint,
                                                    signer_master_signer_id,
                                                    signer_last_health_check);
        }

        QSharedPointer<Wallet> ret = QSharedPointer<Wallet>(new Wallet(QString::fromStdString(walletResult.get_id()),
                                                                       walletResult.get_m(),
                                                                       walletResult.get_n(),
                                                                       QString::fromStdString(walletResult.get_name()),
                                                                       QString::number((int)walletResult.get_address_type()),
                                                                       walletResult.get_balance(),
                                                                       QDateTime::fromTime_t(walletResult.get_create_date()),
                                                                       walletResult.is_escrow(),
                                                                       signersAssinged,
                                                                       QString::fromStdString(walletResult.get_description())));

        ret.data()->warningMessage()->setWarningMessage(0,
                                                        "Your wallet has been imported.",
                                                        EWARNING::WarningType::SUCCESS_MSG,
                                                        "");
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return ret;
    }
    else {
        QSharedPointer<Wallet> retfail = QSharedPointer<Wallet>(new Wallet());
        retfail.data()->warningMessage()->setWarningMessage(msg.code(),
                                                            msg.what(),
                                                            (EWARNING::WarningType)msg.type(),
                                                            "Can not import Wallet");
        AppModel::instance()->newWalletInfo()->setCapableCreate(false);
        return retfail;
    }
}

QSharedPointer<Transaction> bridge::nunchukCreateTransaction(const QString &wallet_id,
                                                             const QMap<QString, qint64> outputs,
                                                             const QString &memo,
                                                             const QSharedPointer<UTXOListModel> inputs,
                                                             const int fee_rate,
                                                             const bool subtract_fee_from_amount,
                                                             QWarningMessage& msg)
{
    QSharedPointer<Transaction> ret = QSharedPointer<Transaction>(new Transaction());
    std::map<std::string, nunchuk::Amount> out;
    for (int i = 0; i < outputs.keys().count(); i++) {
        out[outputs.keys().at(i).toStdString()] = outputs[outputs.keys().at(i)];
    }

    std::vector<nunchuk::UnspentOutput> in;
    if(inputs){
        for (int j = 0; j < inputs.data()->rowCount(); j++) {
            QSharedPointer<UTXO> it = inputs.data()->getUTXOByIndex(j);
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
        QSharedPointer<Transaction> final = bridge::convertTransaction(trans_result);
        final.data()->setStatus((int)nunchuk::TransactionStatus::PENDING_SIGNATURES);
        return final;
    }
    else{
        return NULL;
    }
}

bool bridge::nunchukExportTransaction(const QString &wallet_id, const QString &tx_id, const QString &file_path)
{
    QWarningMessage msg;
    return nunchukiface::instance()->ExportTransaction(wallet_id.toStdString(), tx_id.toStdString(), file_path.toStdString(), msg);
}

QSharedPointer<Transaction> bridge::nunchukImportTransaction(const QString &wallet_id, const QString &file_path)
{
    QWarningMessage msg;
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportTransaction(wallet_id.toStdString(), file_path.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::convertTransaction(trans_result);
    }
    else{
        return NULL;
    }
}

QSharedPointer<Transaction> bridge::nunchukSignTransaction(const QString &wallet_id,
                                                           const QString &tx_id,
                                                           const QSharedPointer<Device> &device,
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
            return bridge::convertTransaction(trans_result);
        }
    }
    return NULL;
}

nunchuk::Transaction bridge::nunchukSignTransactionThread(const QString &wallet_id,
                                                          const QString &tx_id,
                                                          const QSharedPointer<Device> &device,
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

QSharedPointer<Transaction> bridge::nunchukBroadcastTransaction(const QString &wallet_id,
                                                                const QString &tx_id,
                                                                QWarningMessage& msg)
{
    nunchuk::Transaction trans_result = nunchukiface::instance()->BroadcastTransaction(wallet_id.toStdString(), tx_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::convertTransaction(trans_result);
    }
    else{
        return NULL;
    }
}

QSharedPointer<Transaction> bridge::nunchukGetTransaction(const QString &wallet_id, const QString &tx_id)
{
    QWarningMessage msg;
    nunchuk::Transaction trans_result = nunchukiface::instance()->GetTransaction(wallet_id.toStdString(), tx_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return bridge::convertTransaction(trans_result);
    }
    else{
        return NULL;
    }
}

bool bridge::nunchukDeleteTransaction(const QString &wallet_id, const QString &tx_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->DeleteTransaction(wallet_id.toStdString(), tx_id.toStdString(), msg);
}

bool bridge::nunchukDeleteMasterSigner(const QString &mastersigner_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->DeleteMasterSigner(mastersigner_id.toStdString(), msg);
}

bool bridge::nunchukDeleteRemoteSigner(const QString &master_fingerprint, const QString &derivation_path)
{
    QWarningMessage msg;
    return nunchukiface::instance()->DeleteRemoteSigner(master_fingerprint.toStdString(), derivation_path.toStdString(), msg);
}

bool bridge::nunchukUpdateMasterSigner(const QString& mastersignerId, const QString &name )
{
    QWarningMessage msg;
    nunchuk::MasterSigner masterSigner = nunchukiface::instance()->GetMasterSigner(mastersignerId.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        masterSigner.set_name(name.toStdString());
        QWarningMessage msg;
        return nunchukiface::instance()->UpdateMasterSigner(masterSigner, msg);
    }
    return false;
}

bool bridge::nunchukDeleteWallet(const QString &wallet_id)
{
    QWarningMessage msg;
    return nunchukiface::instance()->DeleteWallet(wallet_id.toStdString(), msg);
}

bool bridge::nunchukUpdateWalletName(const QString &wallet_id, const QString &name)
{
    bool ret = false;
    QWarningMessage msg;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        wallet.set_name(name.toStdString());
        ret = nunchukiface::instance()->UpdateWallet(wallet, msg);
    }
    return ret;
}

bool bridge::nunchukUpdateWalletDescription(const QString &wallet_id, const QString &description)
{
    bool ret = false;
    QWarningMessage msg;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        wallet.set_description(description.toStdString());
        ret = nunchukiface::instance()->UpdateWallet(wallet, msg);
    }
    return ret;
}

QSharedPointer<TransactionListModel> bridge::nunchukGetTransactionHistory(const QString &wallet_id, int count, int skip)
{
    QWarningMessage msg;
    std::vector<nunchuk::Transaction> trans_result = nunchukiface::instance()->GetTransactionHistory(wallet_id.toStdString(),
                                                                                                     count,
                                                                                                     skip,
                                                                                                     msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSharedPointer<TransactionListModel> trans_ret = QSharedPointer<TransactionListModel>(new TransactionListModel);
        for (nunchuk::Transaction it : trans_result) {
            QSharedPointer<Transaction> tx = convertTransaction(it);
            trans_ret.data()->addTransaction(tx);
        }
        return trans_ret;
    }
    return QSharedPointer<TransactionListModel>(new TransactionListModel);
}

QSharedPointer<UTXOListModel> bridge::nunchukGetUnspentOutputs(const QString &walletId)
{
    QWarningMessage msg;
    std::vector<nunchuk::UnspentOutput> utxo_result = nunchukiface::instance()->GetUnspentOutputs(walletId.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSharedPointer<UTXOListModel> ret = QSharedPointer<UTXOListModel>(new UTXOListModel());
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

QSharedPointer<Transaction> bridge::nunchukDraftTransaction(const QString &wallet_id,
                                                            const QMap<QString, qint64> outputs,
                                                            const QSharedPointer<UTXOListModel> inputs,
                                                            const int fee_rate,
                                                            const bool subtract_fee_from_amount,
                                                            QWarningMessage& msg)
{
    QSharedPointer<Transaction> ret = QSharedPointer<Transaction>(new Transaction());
    std::map<std::string, nunchuk::Amount> out;
    for (int i = 0; i < outputs.keys().count(); i++) {
        out[outputs.keys().at(i).toStdString()] = outputs[outputs.keys().at(i)];
    }

    std::vector<nunchuk::UnspentOutput> in;
    if(inputs){
        for (int j = 0; j < inputs.data()->rowCount(); j++) {
            QSharedPointer<UTXO> it = inputs.data()->getUTXOByIndex(j);
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
        QSharedPointer<Transaction> final = bridge::convertTransaction(trans_result);
        final.data()->setStatus((int)nunchuk::TransactionStatus::PENDING_SIGNATURES);
        return final;
    }
    else{
        return NULL;
    }
}

QSharedPointer<Transaction> bridge::nunchukReplaceTransaction(const QString &wallet_id,
                                                              const QString &tx_id,
                                                              const int new_fee_rate,
                                                              QWarningMessage& msg)
{
    nunchuk::Transaction trans_result = nunchukiface::instance()->ReplaceTransaction(wallet_id.toStdString(),
                                                                                     tx_id.toStdString(),
                                                                                     new_fee_rate,
                                                                                     msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSharedPointer<Transaction> final = bridge::convertTransaction(trans_result);
        final.data()->setStatus((int)nunchuk::TransactionStatus::PENDING_SIGNATURES);
        return final;
    }
    else{
        return NULL;
    }
}

void bridge::nunchukUpdateTransactionMemo(const QString &wallet_id, const QString &tx_id, const QString &new_memo)
{
    QWarningMessage msg;
    bool ret = nunchukiface::instance()->UpdateTransactionMemo(wallet_id.toStdString(),
                                                               tx_id.toStdString(),
                                                               new_memo.toStdString(),
                                                               msg);
    DBG_INFO << ret;
}

void bridge::nunchukBalanceChanged(const QString &walletId, const int value)
{
    DBG_INFO << walletId << value;
    AppModel::instance()->startBalanceChanged(walletId, value);
}

void bridge::nunchukDevicesChanged(const QString &fingerprint, const bool connected)
{
    DBG_INFO << fingerprint << connected;
    AppModel::instance()->startScanDevices();
}

void bridge::nunchukTransactionChanged(const QString &tx_id, const int status)
{
    DBG_INFO << tx_id << status;
    AppModel::instance()->startTransactionChanged(tx_id, status);
}

void bridge::nunchukBlockChanged(const int height, const QString &hex_header)
{
    DBG_INFO << height << hex_header;
    AppModel::instance()->startBlockChanged(height, hex_header);
}

void bridge::nunchukCacheMasterSignerXPub(const QString &mastersigner_id)
{
    DBG_INFO << mastersigner_id;
    QWarningMessage msg;
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

bool bridge::nunchukSetPassphrase(const QString &passphrase, QWarningMessage& msg)
{
    return nunchukiface::instance()->SetPassphrase(passphrase.toStdString(), msg);
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

int bridge::nunchukEstimateFee(int conf_target)
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

QSharedPointer<SingleSignerListModel> bridge::nunchukGetRemoteSigners()
{
    QWarningMessage msg;
    std::vector<nunchuk::SingleSigner> remoteignerList_result = nunchukiface::instance()->GetRemoteSigners(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSharedPointer<SingleSignerListModel> remoteSignerlist(new SingleSignerListModel());
        for (nunchuk::SingleSigner it : remoteignerList_result) {
            QSharedPointer<SingleSigner> signer = QSharedPointer<SingleSigner>(new SingleSigner());
            signer.data()->setName(QString::fromStdString(it.get_name()));
            signer.data()->setXpub(QString::fromStdString(it.get_xpub()));
            signer.data()->setPublickey(QString::fromStdString(it.get_public_key()));
            signer.data()->setDerivationPath(QString::fromStdString(it.get_derivation_path()));
            signer.data()->setMasterFingerPrint(QString::fromStdString(it.get_master_fingerprint()));
            signer.data()->setMasterSignerId("");
            signer.data()->setIsRemote(true);
            signer.data()->setIsBeingUsed(it.is_used());
            remoteSignerlist.data()->addSingleSigner(signer);
        }
        return remoteSignerlist;
    }
    else{
        return NULL;
    }
}

bool bridge::nunchukUpdateRemoteSigner(const QString &newname)
{
    AppModel::instance()->singleSignerInfo();
    nunchuk::SingleSigner sn(newname.toStdString(),
                             AppModel::instance()->singleSignerInfo()->xpub().toStdString(),
                             AppModel::instance()->singleSignerInfo()->publickey().toStdString(),
                             AppModel::instance()->singleSignerInfo()->derivationPath().toStdString(),
                             AppModel::instance()->singleSignerInfo()->masterFingerPrint().toStdString(),
                             AppModel::instance()->singleSignerInfo()->lastHealthCheckDateTime().toTime_t(),
                             AppModel::instance()->singleSignerInfo()->masterSignerId().toStdString());

    QWarningMessage msg;
    bool ret = nunchukiface::instance()->UpdateRemoteSigner(sn, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return ret;
    }
    else{
        return false;
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

void bridge::nunchukPromtPinOnDevice(const QSharedPointer<Device> &device, QWarningMessage &msg)
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

void bridge::nunchukSendPinToDevice(const QSharedPointer<Device> &device, const QString &pin, QWarningMessage &msg)
{
    if(device.data()){
        nunchuk::Device dv(device.data()->type().toStdString(),
                           device.data()->path().toStdString(),
                           device.data()->model().toStdString(),
                           device.data()->masterFingerPrint().toStdString(),
                           device.data()->needsPassPhraseSent(),
                           device.data()->needsPinSent());
        nunchukiface::instance()->SendPinToDevice(dv, pin.toStdString(), msg);
        emit AppModel::instance()->sentPINToDeviceResult(msg.type());
    }
}

QSharedPointer<SingleSigner> bridge::nunchukCreateCoboSigner(const QString &name, const QString &json_info, QWarningMessage &msg)
{
    nunchuk::SingleSigner signer = nunchukiface::instance()->CreateCoboSigner(name.toStdString(),
                                                                              json_info.toStdString(),
                                                                              msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QString signer_name = QString::fromStdString(signer.get_name());
        QString signer_xpub = QString::fromStdString(signer.get_xpub());
        QString signer_public_key = QString::fromStdString(signer.get_public_key());
        QString signer_derivation_path = QString::fromStdString(signer.get_derivation_path());
        QString signer_master_fingerprint = QString::fromStdString(signer.get_master_fingerprint());
        QString signer_master_signer_id = QString::fromStdString(signer.get_master_signer_id());
        QDateTime signer_last_health_check = QDateTime::fromTime_t(signer.get_last_health_check());

        return QSharedPointer<SingleSigner>(new SingleSigner(signer_name,
                                                             signer_xpub,
                                                             signer_public_key,
                                                             signer_derivation_path,
                                                             signer_master_fingerprint,
                                                             signer_master_signer_id,
                                                             signer_last_health_check));
    }
    else{
        return NULL;
    }
}

QStringList bridge::nunchukExportCoboTransaction(const QString &wallet_id, const QString &tx_id, QWarningMessage &msg)
{
    QStringList result;
    result.clear();
    std::vector<std::string> data = nunchukiface::instance()->ExportCoboTransaction(wallet_id.toStdString(), tx_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

QSharedPointer<Transaction> bridge::nunchukImportCoboTransaction(const QString &wallet_id, const QStringList &qr_data, QWarningMessage &msg)
{
    std::vector<std::string> qr_result;
    for (QString it : qr_data) {
        qr_result.push_back(it.toStdString());
    }
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportCoboTransaction(wallet_id.toStdString(),
                                                                                        qr_result,
                                                                                        msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QSharedPointer<Transaction> final = bridge::convertTransaction(trans_result);
        return final;
    }
    else{
        return NULL;
    }
}

QStringList bridge::nunchukExportCoboWallet(const QString &wallet_id, QWarningMessage &msg)
{
    QStringList result;
    result.clear();
    std::vector<std::string> data = nunchukiface::instance()->ExportCoboWallet(wallet_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        for (std::string it : data) {
            result.append(QString::fromStdString(it));
        }
    }
    return result;
}

QSharedPointer<Wallet> bridge::nunchukImportCoboWallet(const QStringList &qr_data, const QString &description)
{
    QWarningMessage msg;
    std::vector<std::string> qr_result;
    for (QString it : qr_data) {
        qr_result.push_back(it.toStdString());
        DBG_INFO << it;
    }
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportCoboWallet(qr_result,
                                                                              description.toStdString(),
                                                                              msg);
    if((int)EWARNING::WarningType::EXCEPTION_MSG != msg.type()){
        QSharedPointer<SingleSignerListModel> signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner it : walletResult.get_signers()) {
            QString signer_name = QString::fromStdString(it.get_name());
            QString signer_xpub = QString::fromStdString(it.get_xpub());
            QString signer_public_key = QString::fromStdString(it.get_public_key());
            QString signer_derivation_path = QString::fromStdString(it.get_derivation_path());
            QString signer_master_fingerprint = QString::fromStdString(it.get_master_fingerprint());
            QString signer_master_signer_id = QString::fromStdString(it.get_master_signer_id());
            QDateTime signer_last_health_check = QDateTime::fromTime_t(it.get_last_health_check());
            signersAssinged.data()->addSingleSigner(signer_name,
                                                    signer_xpub,
                                                    signer_public_key,
                                                    signer_derivation_path,
                                                    signer_master_fingerprint,
                                                    signer_master_signer_id,
                                                    signer_last_health_check);
        }

        QSharedPointer<Wallet> ret = QSharedPointer<Wallet>(new Wallet(QString::fromStdString(walletResult.get_id()),
                                                                       walletResult.get_m(),
                                                                       walletResult.get_n(),
                                                                       QString::fromStdString(walletResult.get_name()),
                                                                       QString::number((int)walletResult.get_address_type()),
                                                                       walletResult.get_balance(),
                                                                       QDateTime::fromTime_t(walletResult.get_create_date()),
                                                                       walletResult.is_escrow(),
                                                                       signersAssinged,
                                                                       QString::fromStdString(walletResult.get_description())));

        ret.data()->warningMessage()->setWarningMessage(0,
                                                        "Your wallet has been imported.",
                                                        EWARNING::WarningType::SUCCESS_MSG,
                                                        "");
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return ret;
    }
    else {
        QSharedPointer<Wallet> retfail = QSharedPointer<Wallet>(new Wallet());
        retfail.data()->warningMessage()->setWarningMessage(msg.code(),
                                                            msg.what(),
                                                            (EWARNING::WarningType)msg.type(),
                                                            "Can not import Wallet");
        AppModel::instance()->newWalletInfo()->setCapableCreate(false);
        return retfail;
    }
}

QSharedPointer<Wallet> bridge::nunchukImportWalletConfigFile(const QString &file_path, const QString &description)
{
    QWarningMessage msg;
    nunchuk::Wallet walletResult = nunchukiface::instance()->ImportWalletConfigFile(file_path.toStdString(),
                                                                                    description.toStdString(),
                                                                                    msg);
    if((int)EWARNING::WarningType::EXCEPTION_MSG != msg.type()){
        QSharedPointer<SingleSignerListModel> signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner it : walletResult.get_signers()) {
            QString signer_name = QString::fromStdString(it.get_name());
            QString signer_xpub = QString::fromStdString(it.get_xpub());
            QString signer_public_key = QString::fromStdString(it.get_public_key());
            QString signer_derivation_path = QString::fromStdString(it.get_derivation_path());
            QString signer_master_fingerprint = QString::fromStdString(it.get_master_fingerprint());
            QString signer_master_signer_id = QString::fromStdString(it.get_master_signer_id());
            QDateTime signer_last_health_check = QDateTime::fromTime_t(it.get_last_health_check());
            signersAssinged.data()->addSingleSigner(signer_name,
                                                    signer_xpub,
                                                    signer_public_key,
                                                    signer_derivation_path,
                                                    signer_master_fingerprint,
                                                    signer_master_signer_id,
                                                    signer_last_health_check);
        }

        QSharedPointer<Wallet> ret = QSharedPointer<Wallet>(new Wallet(QString::fromStdString(walletResult.get_id()),
                                                                       walletResult.get_m(),
                                                                       walletResult.get_n(),
                                                                       QString::fromStdString(walletResult.get_name()),
                                                                       QString::number((int)walletResult.get_address_type()),
                                                                       walletResult.get_balance(),
                                                                       QDateTime::fromTime_t(walletResult.get_create_date()),
                                                                       walletResult.is_escrow(),
                                                                       signersAssinged,
                                                                       QString::fromStdString(walletResult.get_description())));

        ret.data()->warningMessage()->setWarningMessage(0,
                                                        "Your wallet has been imported.",
                                                        EWARNING::WarningType::SUCCESS_MSG,
                                                        "");
        bool needTopUp = AppModel::instance()->newWalletInfo()->singleSignersAssigned()->needTopUpXpubs();
        AppModel::instance()->newWalletInfo()->setCapableCreate(!needTopUp);
        return ret;
    }
    else {
        QSharedPointer<Wallet> retfail = QSharedPointer<Wallet>(new Wallet());
        retfail.data()->warningMessage()->setWarningMessage(msg.code(),
                                                            msg.what(),
                                                            (EWARNING::WarningType)msg.type(),
                                                            "Can not import Wallet");
        AppModel::instance()->newWalletInfo()->setCapableCreate(false);
        return retfail;
    }
}

void bridge::nunchukRescanBlockchain(int start_height, int stop_height)
{
    QWarningMessage msg;
    nunchukiface::instance()->RescanBlockchain(start_height, stop_height, msg);
}

QSharedPointer<MasterSigner> bridge::nunchukCreateSoftwareSigner(const QString &name, const QString &mnemonic, const QString &passphrase, QWarningMessage &msg)
{
    QSharedPointer<MasterSigner> resultSoftSinger =  QSharedPointer<MasterSigner>(new MasterSigner());
    if(resultSoftSinger){
        resultSoftSinger.data()->setName(name);
        nunchuk::MasterSigner masterSigner = nunchukiface::instance()->CreateSoftwareSigner(name.toStdString(),
                                                                                            mnemonic.toStdString(),
                                                                                            passphrase.toStdString(),
                                                                                            msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            QString out_signature = "";
            QString out_path = "";
            ENUNCHUCK::HealthStatus healthResult = (ENUNCHUCK::HealthStatus)nunchukHealthCheckMasterSigner(QString::fromStdString(masterSigner.get_device().get_master_fingerprint()),
                                                                                                           resultSoftSinger.data()->message(),
                                                                                                           out_signature,
                                                                                                           out_path,
                                                                                                           msg);
            if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                resultSoftSinger.data()->setHealth((int)healthResult);
                resultSoftSinger.data()->setPath(out_path);
                if(ENUNCHUCK::HealthStatus::SUCCESS == healthResult){
                    resultSoftSinger.data()->setId(QString::fromStdString(masterSigner.get_id()));
                    resultSoftSinger.data()->setName(QString::fromStdString(masterSigner.get_name()));
                    resultSoftSinger.data()->device()->setType(QString::fromStdString(masterSigner.get_device().get_type()));
                    resultSoftSinger.data()->device()->setPath(QString::fromStdString(masterSigner.get_device().get_path()));
                    resultSoftSinger.data()->device()->setModel(QString::fromStdString(masterSigner.get_device().get_model()));
                    resultSoftSinger.data()->device()->setMasterFingerPrint(QString::fromStdString(masterSigner.get_device().get_master_fingerprint()));
                    resultSoftSinger.data()->device()->setConnected(/*masterSigner.get_device().connected()*/false);
                    resultSoftSinger.data()->device()->setNeedsPassPhraseSent(masterSigner.get_device().needs_pass_phrase_sent());
                    resultSoftSinger.data()->device()->setNeedsPinSent(masterSigner.get_device().needs_pin_sent());
                    resultSoftSinger.data()->setIsSoftwareSigner(true);
                }
            }
        }
    }
    return resultSoftSinger;
}

void bridge::nunchukSendSignerPassphrase(const QString &mastersigner_id, const QString &passphrase, QWarningMessage &msg)
{
    nunchukiface::instance()->SendSignerPassphrase(mastersigner_id.toStdString(), passphrase.toStdString(), msg);
}

QStringList bridge::nunchuckGetBIP39WordList()
{
    return qUtils::GetBIP39WordList();
}

void bridge::nunchukClearSignerPassphrase(const QString &mastersigner_id, QWarningMessage &msg)
{
    nunchukiface::instance()->ClearSignerPassphrase(mastersigner_id.toStdString(), msg);
}
