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
#include "Worker.h"
#include "bridgeifaces.h"
#include "Chats/matrixbrigde.h"
#include "ViewsEnums.h"
#include "QEventProcessor.h"
#include "Servers/Draco.h"
#include "localization/STR_CPP.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QSignerManagement.h"

Worker *Worker::mInstance = NULL;
Worker::Worker()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Worker::~Worker()
{
    this->disconnect();
}

Worker *Worker::create(QThread *thread)
{
    if(mInstance == NULL){
        mInstance = new Worker;
        mInstance->moveToThread(thread);
    }
    return mInstance;
}

void Worker::slotStartCreateMasterSigner(const QString &name,
                                         const int deviceIndex)
{
    QWarningMessage msg;
    AppModel::instance()->setAddSignerStep(0);
    AppModel::instance()->setAddSignerPercentage(0);
    QMasterSignerPtr ret = bridge::nunchukCreateMasterSigner(name,
                                                             deviceIndex,
                                                             msg);
    AppModel::instance()->setAddSignerPercentage(100);
    AppModel::instance()->setWalletsUsingSigner({});
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && ret){
        DBG_INFO << "CREATE MASTER SIGNER DONE";
        emit finishCreateMasterSigner(ret,
                                      msg.what(),
                                      msg.type(),
                                      msg.code());
    }
    else{
        DBG_INFO << "CREATE MASTER SIGNER FAIL" << msg.what() << msg.type() << msg.code();
        int last = QEventProcessor::instance()->getCurrentStates().last();
        if (last == E::STATE_ID_SCR_ADD_HARDWARE) {
            AppModel::instance()->setAddSignerWizard(1);
        }
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        slotStartScanDevices(last);
        emit finishCreateMasterSigner(NULL,
                                      msg.what(),
                                      msg.type(),
                                      msg.code());
    }
}

void Worker::slotStartCreateRemoteSigner(const QString &name,
                                         const QString &xpub,
                                         const QString &public_key,
                                         const QString &derivation_path,
                                         const QString &master_fingerprint,
                                         const nunchuk::SignerType type,
                                         const std::vector<nunchuk::SignerTag> tags,
                                         const bool replace,
                                         const int event)
{
    QWarningMessage msg;
    nunchuk::SingleSigner ret = bridge::nunchukCreateOriginSigner(name,
                                                                  xpub,
                                                                  public_key,
                                                                  derivation_path,
                                                                  master_fingerprint,
                                                                  type,
                                                                  tags,
                                                                  replace,
                                                                  msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && replace){
        if (QEventProcessor::instance()->currentFlow() != (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_USER_FREE) {
            WalletKeys::SyncSignerToServer(ret);
            AppModel::instance()->startReloadWallets();
        }
    }
    AppModel::instance()->setWalletsUsingSigner({});
    emit finishCreateRemoteSigner(event,
                                  ret,
                                  msg.what(),
                                  msg.type(),
                                  msg.code());
}

void Worker::slotStartScanDevices(const int state_id) {
    QWarningMessage msg;
    std::vector<nunchuk::Device> deviceList {};
    if (AppModel::instance()->isSignIn()) {
        deviceList = qUtils::GetDevices(bridge::hwiPath(), msg);
        DBG_INFO << "deviceList.size():" << deviceList.size();
    } else {
        deviceList = bridge::nunchukGetOriginDevices(msg);
    }
    emit finishScanDevices(state_id,
                           deviceList,
                           msg.what(),
                           msg.type(),
                           msg.code());
}

void Worker::slotStartSigningTransaction(const QString &walletId,
                                         const QString &txid,
                                         const QString &deviceXfp,
                                         bool isSoftware)
{
    DBG_INFO << walletId << deviceXfp << isSoftware << txid;
    QTransactionPtr transaction = AppModel::instance()->transactionInfoPtr();
    if(transaction){
        QWarningMessage msgwarning;
        QDevicePtr selectedDv = NULL;
        if(isSoftware){
            if(AppModel::instance()->softwareSignerDeviceList()){
                selectedDv = AppModel::instance()->softwareSignerDeviceList()->getDeviceByXfp(deviceXfp) ;
            }
        }
        else{
            if(AppModel::instance()->deviceList()){
                selectedDv = AppModel::instance()->deviceList()->getDeviceByXfp(deviceXfp) ;
            }
        }
        if(transaction.data()->roomId() != ""){ // shared
            QNunchukRoom* room = CLIENT_INSTANCE->GetRoomById(transaction.data()->roomId());
            if(room && selectedDv){
                QString init_event_id = transaction.data()->initEventId();
                QWarningMessage signwarning;
                matrixbrigde::SignTransaction(room->id(),init_event_id, selectedDv, signwarning);
                nunchuk::Transaction trans = bridge::nunchukGetOriginTransaction(walletId,
                                                                                 txid,
                                                                                 signwarning);
                emit finishSigningTransaction(walletId,
                                              trans,
                                              msgwarning.what(),
                                              msgwarning.type(),
                                              msgwarning.code(),
                                              selectedDv.data()->masterSignerId() ,
                                              isSoftware);
            }
            else{
                msgwarning.setWarningMessage(0, STR_CPP_059, EWARNING::WarningType::ERROR_MSG);
                emit finishSigningTransaction(walletId,
                                              nunchuk::Transaction(),
                                              msgwarning.what(),
                                              msgwarning.type(),
                                              msgwarning.code(),
                                              "-1",
                                              selectedDv);
            }
        }
        else{
            if(selectedDv){
                nunchuk::Transaction trans = bridge::nunchukSignTransaction(walletId, txid, selectedDv, msgwarning);
                emit finishSigningTransaction(walletId,
                                              trans,
                                              msgwarning.what(),
                                              msgwarning.type(),
                                              msgwarning.code(),
                                              selectedDv.data()->masterSignerId() ,
                                              isSoftware);
            }
            else{
                msgwarning.setWarningMessage(0,
                                             STR_CPP_054,
                                             EWARNING::WarningType::ERROR_MSG);
                emit finishSigningTransaction(walletId,
                                              nunchuk::Transaction(),
                                              msgwarning.what(),
                                              msgwarning.type(),
                                              msgwarning.code(),
                                              "-1",
                                              selectedDv);
            }
        }
    }
}

void Worker::slotStartHealthCheckMasterSigner(const int state_id,
                                              const QString &xfp,
                                              const QString &message)
{
    QString out_signature = "";
    QString out_path = "";
    QWarningMessage msgwarning;
    int health = (int)bridge::nunchukHealthCheckMasterSigner(xfp, message, out_signature, out_path, msgwarning);
    DBG_INFO << health;
    emit finishHealthCheckMasterSigner(state_id,
                                       health,
                                       message,
                                       out_signature,
                                       xfp,
                                       msgwarning.what(),
                                       msgwarning.type(),
                                       msgwarning.code());
}

void Worker::slotStartTopXPUBsMasterSigner(const QVariant &data)
{
    QMap<QString,QVariant> maps = data.toMap();
    QWarningMessage warningmsg;
    QString masterSignerId = maps["masterSignerId"].toString();
    bridge::nunchukCacheMasterSignerXPub(masterSignerId,warningmsg);
    maps["what"] = warningmsg.what();
    maps["type"] = warningmsg.type();
    maps["code"] = warningmsg.code();
    emit finishTopXPUBsMasterSigner(QVariant::fromValue(maps));
}

void Worker::slotStartHealthCheckRemoteSigner(const int state_id, const QString& xfp, const int signer_type, const QString& message)
{
    DBG_INFO << state_id << xfp << signer_type << message;
    if((int)ENUNCHUCK::SignerType::SOFTWARE == signer_type
            || (int)ENUNCHUCK::SignerType::HARDWARE == signer_type
            || (int)ENUNCHUCK::SignerType::COLDCARD_NFC == signer_type)
    {
        QString out_signature = "";
        QString out_path = "";
        QWarningMessage msgwarning;
        int status = (int)bridge::nunchukHealthCheckMasterSigner(xfp, message, out_signature, out_path, msgwarning);
        emit finishHealthCheckRemoteSigner(status,
                                           msgwarning.what(),
                                           msgwarning.type(),
                                           msgwarning.code());
    }
    else{
        QWarningMessage msgwarning;
        int status = (int)bridge::nunchukHealthCheckSingleSigner(AppModel::instance()->singleSignerInfoPtr(), msgwarning);
        emit finishHealthCheckRemoteSigner(status,
                                           msgwarning.what(),
                                           msgwarning.type(),
                                           msgwarning.code());
    }
}

void Worker::slotStartDisplayAddress(const QString &wallet_id,
                                     const QString &address)
{
    DBG_INFO << wallet_id << address;
    emit AppModel::instance()->displayAddressOnDevices();
    bool ret = false;
    QWarningMessage msg;
    QDeviceListModelPtr deviceList = bridge::nunchukGetDevices(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        msg.resetWarningMessage();
        if(deviceList && AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->singleSignersAssigned()){
            QStringList xfps = deviceList->getXFPList();
            foreach (QString xfp, xfps) {
                msg.resetWarningMessage();
                ret = bridge::nunchukDisplayAddressOnDevice(wallet_id,
                                                            address,
                                                            xfp,
                                                            msg);
            }
        }
    }
    if(!ret){
        QString msg = QString("Address verification failed");
        AppModel::instance()->showToast(0, msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    else{
        QString msg = QString("Address successfully verified");
        AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
    }
    emit finishDisplayAddress(ret);
}

void Worker::slotStartRescanBlockchain(int start,
                                       int stop)
{
    DBG_INFO << start << stop;
    bridge::nunchukRescanBlockchain(start, stop); // Default stop = -1
    emit finishRescanBlockchain();
}

void Worker::slotStartCreateSoftwareSigner(const QString name,
                                           const QString mnemonic,
                                           const QString passphrase,
                                           bool replace)
{
    QWarningMessage msg;
    bool isPrimaryKey = (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY) ||
                        (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY);
    AppModel::instance()->setAddSignerPercentage(0);
    QMasterSignerPtr ret = bridge::nunchukCreateSoftwareSigner(name,
                                                               mnemonic,
                                                               passphrase,
                                                               isPrimaryKey,
                                                               replace,
                                                               msg);
    AppModel::instance()->setAddSignerPercentage(100);
    AppModel::instance()->setWalletsUsingSigner({});
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && ret){
        if (replace) {
            nunchuk::SingleSigner singlesinger(ret->name().toStdString(),
                                               "",
                                               "",
                                               "",
                                               ret->fingerPrint().toStdString(),
                                               std::time(0),
                                               ret->id().toStdString(),
                                               true,
                                               nunchuk::SignerType::SOFTWARE,
                                               ret->originMasterSigner().get_tags()
                                               );
            WalletKeys::SyncSignerToServer(singlesinger);
        }
        emit finishCreateSoftwareSigner(ret->originMasterSigner(),
                                        msg.what(),
                                        msg.type(),
                                        msg.code());
    }
    else{
        if (msg.code() == (int)nunchuk::StorageException::SIGNER_EXISTS) {
            QString fingerprint = qUtils::GetMasterFingerprint(mnemonic, passphrase);
            emit AppModel::instance()->notifySignerExist(true, fingerprint);
        } else {
            emit finishCreateSoftwareSigner({},
                                            msg.what(),
                                            msg.type(),
                                            msg.code());
        }
    }
}

void Worker::slotStartCreateSoftwareSignerXprv(const QString name, const QString xprv, bool replace)
{
    QWarningMessage msg;
    bool isPrimaryKey = (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY) ||
                        (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY);
    AppModel::instance()->setAddSignerPercentage(0);
    nunchuk::MasterSigner ret = bridge::CreateSoftwareSignerFromMasterXprv(name, xprv, isPrimaryKey, replace, msg);
    AppModel::instance()->setAddSignerPercentage(100);
    AppModel::instance()->setWalletsUsingSigner({});
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        if (replace) {
            nunchuk::SingleSigner singlesinger(ret.get_name(),
                                               "",
                                               "",
                                               "",
                                               ret.get_device().get_master_fingerprint(),
                                               std::time(0),
                                               ret.get_id(),
                                               true,
                                               nunchuk::SignerType::SOFTWARE,
                                               ret.get_tags());
            WalletKeys::SyncSignerToServer(singlesinger);
        }
        emit finishCreateSoftwareSignerXprv(ret,
                                            msg.what(),
                                            msg.type(),
                                            msg.code());
    }
    else{
        if (msg.code() == (int)nunchuk::StorageException::SIGNER_EXISTS) {
            emit AppModel::instance()->notifySignerExist(true, QString::fromStdString(ret.get_device().get_master_fingerprint()));
        }
        AppModel::instance()->showToast(msg.code(),msg.what(), (EWARNING::WarningType)msg.type());
        emit finishCreateSoftwareSignerXprv(ret,
                                            msg.what(),
                                            msg.type(),
                                            msg.code());
    }
}

void Worker::slotStartCreateWallet(bool need_backup, QString file_path)
{
    DBG_INFO << need_backup << file_path;
    if(AppModel::instance()->newWalletInfo()){
        DBG_INFO << "CREATE"
                 << AppModel::instance()->newWalletInfo()->walletAddressType()
                 << AppModel::instance()->newWalletInfo()->walletType()
                 << AppModel::instance()->newWalletInfo()->walletM()
                 << AppModel::instance()->newWalletInfo()->walletN();
        if(AppModel::instance()->newWalletInfo()->walletAddressType() == (int)nunchuk::AddressType::TAPROOT){
            QJsonObject data;
            QString     error_msg = "";
            bool get_result = Draco::instance()->GetTaprootSupportedSigners(data, error_msg);
            if(get_result){
                DBG_INFO << data;
                QJsonArray supported_signers = data["supported_signers"].toArray();
                enum class SignerType {
                    UNKNOWN = -1,
                    HARDWARE,
                    AIRGAP,
                    SOFTWARE,
                    FOREIGN_SOFTWARE,
                    NFC,
                    COLDCARD_NFC,
                    SERVER,
                    PORTAL_NFC,
                };

                QSet<int> all_types = {
                    (int)SignerType::UNKNOWN,
                    (int)SignerType::HARDWARE,
                    (int)SignerType::AIRGAP,
                    (int)SignerType::SOFTWARE,
                    (int)SignerType::FOREIGN_SOFTWARE,
                    (int)SignerType::NFC,
                    (int)SignerType::COLDCARD_NFC,
                    (int)SignerType::SERVER,
                    (int)SignerType::PORTAL_NFC
                };

                QSet<int> supported_types;
                supported_types.clear();
                foreach (const QJsonValue &value, supported_signers) {
                    QJsonObject obj = value.toObject();
                    QString type = obj["signer_type"].toString();
                    nunchuk::SignerType type_enum = qUtils::GetSignerType(type);
                    supported_types.insert((int)type_enum);
                }

                QSet<int> unsupported_types = all_types - supported_types;
                foreach (int unsupported_type, unsupported_types) {
                    AppModel::instance()->newWalletInfo()->singleSignersAssigned()->removeSingleSignerByType(unsupported_type);
                }
            }
        }
        QWarningMessage msgWarning;
        nunchuk::Wallet ret = bridge::nunchukCreateOriginWallet(AppModel::instance()->newWalletInfo()->walletNameDisplay(),
                                                                AppModel::instance()->newWalletInfo()->walletM(),
                                                                AppModel::instance()->newWalletInfo()->walletN(),
                                                                AppModel::instance()->newWalletInfo()->singleSignersAssigned(),
                                                                (nunchuk::AddressType)AppModel::instance()->newWalletInfo()->walletAddressType(),
                                                                nunchuk::WalletType::MULTI_SIG,//(nunchuk::WalletType)AppModel::instance()->newWalletInfo()->walletType(),
                                                                AppModel::instance()->newWalletInfo()->walletDescription(),
                                                                false /*allow used signer*/,
                                                                "" /*decoy pin*/,
                                                                msgWarning);

        DBG_INFO << "CREATE" << ret.get_name() << (int)ret.get_wallet_type() << (int)ret.get_address_type() << ret.get_m() << ret.get_n();

        emit finishCreateWallet(ret,
                                msgWarning.what(),
                                msgWarning.type(),
                                msgWarning.code());
        if(need_backup){
            bridge::nunchukExportWallet(QString::fromStdString(ret.get_id()), file_path, nunchuk::ExportFormat::BSMS);
        }
    }
}

void Worker::slotStartBalanceChanged(const QString &id,
                                     const qint64 balance)
{
    QWarningMessage msg;
    nunchuk::Wallet origin_wallet = bridge::nunchukGetOriginWallet(id, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        if(AppModel::instance()->walletList()){
            AppModel::instance()->walletList()->updateNunchukWallet(id, origin_wallet);
        }
        if(AppModel::instance()->walletInfo()  && qUtils::strCompare(id, AppModel::instance()->walletInfo()->walletId())){
            AppModel::instance()->walletInfo()->setNunchukWallet(origin_wallet);
            slotStartGetTransactionHistory(id);
        }
    }
    emit finishBalanceChanged(id, balance);
}

void Worker::slotStartTransactionChanged(const QString &tx_id,
                                         const int status,
                                         const QString &wallet_id)
{
    QWarningMessage msgWarning;
    nunchuk::Transaction tx = bridge::nunchukGetOriginTransaction(wallet_id, tx_id, msgWarning);
    emit finishTransactionChanged(tx_id, status, wallet_id, tx);
}

void Worker::slotStartBlockChanged(const int height,
                                   const QString &hex_header)
{
    emit finishBlockChanged(height, hex_header);
}

void Worker::slotStartGetUsedAddresses(const QString wallet_id)
{
    if(wallet_id != ""){
        QStringList addr = bridge::nunchukGetUsedAddresses(wallet_id, false);
        QStringList caddr = bridge::nunchukGetUsedAddresses(wallet_id, true);
        emit finishGetUsedAddresses(wallet_id, addr, caddr);
    }
}

void Worker::slotStartGetUnusedAddresses(const QString wallet_id)
{
    if(wallet_id != ""){
        QStringList addr = bridge::nunchukGetUnusedAddresses(wallet_id, false);
        QStringList caddr = bridge::nunchukGetUnusedAddresses(wallet_id, true);
        emit finishGetUnusedAddresses(wallet_id, addr, caddr);
    }
}

void Worker::slotStartGetTransactionHistory(const QString wallet_id)
{
    if(wallet_id != ""){
        std::vector<nunchuk::Transaction> trans_result = bridge::nunchukGetOriginTransactionHistory(wallet_id);
        emit finishGetTransactionHistory(wallet_id, trans_result);
    }
}

void Worker::slotStartGetEstimatedFee()
{
    Draco::instance()->feeRates();
}

void Worker::slotStartSendPinToDevice(const int state_id, const int device_idx, const QString &pin)
{
    QWarningMessage msgwarning;
    if(AppModel::instance()->deviceList()){
        QDevicePtr selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(device_idx) ;
        if(selectedDv && selectedDv.data()->needsPinSent()){
            bridge::nunchukSendPinToDevice(selectedDv, pin, msgwarning);
        }
    }
    emit finishSendPinToDevice(state_id,
                               msgwarning.what(),
                               msgwarning.code(),
                               (int)msgwarning.type());
}

void Worker::slotStartSendPassphraseToDevice(const int state_id, const int device_idx, const QString &pprase)
{
    QWarningMessage msgwarning;
    if(AppModel::instance()->deviceList()){
        QDevicePtr selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(device_idx) ;
        if(selectedDv && selectedDv.data()->needsPassPhraseSent()){
            bridge::nunchukSendPassphraseToDevice(selectedDv, pprase, msgwarning);
        }
    }
    emit finishSendPassphraseToDevice(state_id,
                                      msgwarning.what(),
                                      msgwarning.code(),
                                      (int)msgwarning.type());
}

void Worker::slotStartRemoveAllWallets()
{
    bridge::nunchukDeleteAllWallet();
    emit finishRemoveAllWallets();
}

void Worker::slotStartRemoveAllSigners()
{
    QWarningMessage msg;
    std::vector<nunchuk::MasterSigner> master_signers;
    do{
        master_signers = bridge::nunchukGetOriginMasterSigners(msg);
        for (nunchuk::MasterSigner it : master_signers) {
            bridge::nunchukDeleteMasterSigner(QString::fromStdString(it.get_id()));
        }
    }while(master_signers.size() > 0);
    std::vector<nunchuk::SingleSigner> remote_signers;
    do{
        remote_signers = bridge::nunchukGetOriginRemoteSigners(msg);
        for (nunchuk::SingleSigner it : remote_signers) {
            bridge::nunchukDeleteRemoteSigner(QString::fromStdString(it.get_master_fingerprint()), QString::fromStdString(it.get_derivation_path()));
        }
    }while(remote_signers.size() > 0);
    emit finishRemoveAllSigners();
}

void Worker::slotStartMultiDeviceSync(const bool state)
{
    if(CLIENT_INSTANCE->isNunchukLoggedIn() && CLIENT_INSTANCE->isMatrixLoggedIn()){
        matrixbrigde::EnableAutoBackup(state);
        if(state){
            matrixbrigde::Backup();
        }
    }
}

void Worker::slotStartReloadUserDb()
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QWarningMessage msg;

    std::vector<nunchuk::Wallet> group_wallets = bridge::nunchukGetOriginGroupWallets(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadGroupWallets(group_wallets);
    }

    std::vector<nunchuk::Wallet> wallets = bridge::nunchukGetOriginWallets(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadWallets(wallets);
    }

    std::vector<nunchuk::MasterSigner> masters = bridge::nunchukGetOriginMasterSigners(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadMasterSigners(masters);
    }
    std::vector<nunchuk::SingleSigner> remotes = bridge::nunchukGetOriginRemoteSigners(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadRemoteSigners(remotes);
    }
}

void Worker::slotStartReloadWallets()
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QWarningMessage msg;
    std::vector<nunchuk::Wallet> group_wallets = bridge::nunchukGetOriginGroupWallets(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadGroupWallets(group_wallets);
    }

    std::vector<nunchuk::Wallet> wallets = bridge::nunchukGetOriginWallets(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadWallets(wallets);
    }
}

void Worker::slotStartReloadMasterSigners()
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QWarningMessage msg;
    std::vector<nunchuk::MasterSigner> masters = bridge::nunchukGetOriginMasterSigners(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadMasterSigners(masters);
    }
}

void Worker::slotStartReloadRemoteSigners()
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QWarningMessage msg;
    std::vector<nunchuk::SingleSigner> remotes = bridge::nunchukGetOriginRemoteSigners(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadRemoteSigners(remotes);
    }
}

void Worker::slotStartSyncWalletDb(const QString &wallet_id)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    if(wallet_id != ""){
        QWarningMessage msg;
        nunchuk::Wallet origin_wallet = bridge::nunchukGetOriginWallet(wallet_id, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            AppModel::instance()->walletInfo()->setNunchukWallet(origin_wallet);
            AppModel::instance()->walletInfo()->startDownloadConversation();
        }

        std::vector<nunchuk::Transaction> trans_result = bridge::nunchukGetOriginTransactionHistory(wallet_id);
        emit finishGetTransactionHistory(wallet_id, trans_result);

        QStringList used_addr = bridge::nunchukGetUsedAddresses(wallet_id, false);
        QStringList used_change_addr = bridge::nunchukGetUsedAddresses(wallet_id, true);

        QStringList unused_addr = bridge::nunchukGetUnusedAddresses(wallet_id, false);
        QStringList unsued_change_addr = bridge::nunchukGetUnusedAddresses(wallet_id, true);

        QWalletPtr wallet = NULL;
        if(AppModel::instance()->walletInfo()){
            if(qUtils::strCompare(AppModel::instance()->walletInfo()->walletId(), wallet_id)){
                wallet = AppModel::instance()->walletInfoPtr();
            }
        }
        else{
            if(AppModel::instance()->walletList()){
                wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
            }
        }
        if(wallet){
            wallet.data()->setUsedAddressList(used_addr);
            wallet.data()->setUsedChangeAddressList(used_change_addr);
            wallet.data()->setunUsedAddressList(unused_addr);
            wallet.data()->setUnUsedChangeddAddressList(unsued_change_addr);
            wallet.data()->startDownloadConversation();
            if(wallet){
                QtConcurrent::run([wallet, wallet_id]() {
                    wallet.data()->GetAssistedTxs();
                    AppModel::instance()->startGetTransactionHistory(wallet_id);
                });
            }
        }
    }
    emit finishSyncWalletDb(wallet_id);
}

Q_DECLARE_METATYPE(nunchuk::SignerTag)
Q_DECLARE_METATYPE(nunchuk::SignerType)
Controller::Controller() {
    qRegisterMetaType<Conversation>();
    qRegisterMetaType<nunchuk::Wallet>();
    qRegisterMetaType<nunchuk::SingleSigner>();
    qRegisterMetaType<nunchuk::MasterSigner>();
    qRegisterMetaType<QMasterSignerPtr>();
    qRegisterMetaType<nunchuk::Transaction>();
    qRegisterMetaType<nunchuk::RoomTransaction>();
    qRegisterMetaType<nunchuk::Device>();
    qRegisterMetaType<std::vector<nunchuk::Device> >();
    qRegisterMetaType<std::vector<nunchuk::Transaction> >();
    qRegisterMetaType<std::vector<nunchuk::SignerTag> >();
    qRegisterMetaType<nunchuk::SignerType>();

    Worker *worker = Worker::create(&workerThread);

    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    // Create master signer
    connect(this, &Controller::startCreateMasterSigner, worker, &Worker::slotStartCreateMasterSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateMasterSigner, this, &Controller::slotFinishCreateMasterSigner, Qt::QueuedConnection);

    // Create remote signer
    connect(this, &Controller::startCreateRemoteSigner, worker, &Worker::slotStartCreateRemoteSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateRemoteSigner, this, &Controller::slotFinishCreateRemoteSigner, Qt::QueuedConnection);

    // Get devices
    connect(this, &Controller::startScanDevices, worker, &Worker::slotStartScanDevices, Qt::QueuedConnection);
    connect(worker, &Worker::finishScanDevices, this, &Controller::slotFinishScanDevices, Qt::QueuedConnection);

    // Balance changed
    connect(this, &Controller::startBalanceChanged, worker, &Worker::slotStartBalanceChanged, Qt::QueuedConnection);
    connect(worker, &Worker::finishBalanceChanged, this, &Controller::slotFinishBalanceChanged, Qt::QueuedConnection);

    // Transaction changed
    connect(this, &Controller::startTransactionChanged, worker, &Worker::slotStartTransactionChanged, Qt::QueuedConnection);
    connect(worker, &Worker::finishTransactionChanged, this, &Controller::slotFinishTransactionChanged, Qt::QueuedConnection);

    // Block changed
    connect(this, &Controller::startBlockChanged, worker, &Worker::slotStartBlockChanged, Qt::QueuedConnection);
    connect(worker, &Worker::finishBlockChanged, this, &Controller::slotFinishBlockChanged, Qt::QueuedConnection);

    // Signing TX
    connect(this, &Controller::startSigningTransaction, worker, &Worker::slotStartSigningTransaction, Qt::QueuedConnection);
    connect(worker, &Worker::finishSigningTransaction, this, &Controller::slotFinishSigningTransaction, Qt::QueuedConnection);

    // Health check master signer
    connect(this, &Controller::startHealthCheckMasterSigner, worker, &Worker::slotStartHealthCheckMasterSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishHealthCheckMasterSigner, this, &Controller::slotFinishHealthCheckMasterSigner, Qt::QueuedConnection);

    // Health check remote signer
    connect(this, &Controller::startHealthCheckRemoteSigner, worker, &Worker::slotStartHealthCheckRemoteSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishHealthCheckRemoteSigner, this, &Controller::slotFinishHealthCheckRemoteSigner, Qt::QueuedConnection);

    // get top up XPUBs
    connect(this, &Controller::startTopXPUBsMasterSigner, worker, &Worker::slotStartTopXPUBsMasterSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishTopXPUBsMasterSigner, this, &Controller::slotFinishTopXPUBsMasterSigner, Qt::QueuedConnection);

    // Display address
    connect(this, &Controller::startDisplayAddress, worker, &Worker::slotStartDisplayAddress, Qt::QueuedConnection);
    connect(worker, &Worker::finishDisplayAddress, this, &Controller::slotFinishDisplayAddress, Qt::QueuedConnection);

    // Display address
    connect(this, &Controller::startRescanBlockchain, worker, &Worker::slotStartRescanBlockchain, Qt::QueuedConnection);
    connect(worker, &Worker::finishRescanBlockchain, this, &Controller::slotFinishRescanBlockchain, Qt::QueuedConnection);

    // Create master signer
    connect(this, &Controller::startCreateSoftwareSigner, worker, &Worker::slotStartCreateSoftwareSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateSoftwareSigner, this, &Controller::slotFinishCreateSoftwareSigner, Qt::QueuedConnection);

    // Create software signerX
    connect(this, &Controller::startCreateSoftwareSignerXprv, worker, &Worker::slotStartCreateSoftwareSignerXprv, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateSoftwareSignerXprv, this, &Controller::slotFinishCreateSoftwareSignerXprv, Qt::QueuedConnection);

    // Create wallet
    connect(this, &Controller::startCreateWallet, worker, &Worker::slotStartCreateWallet, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateWallet, this, &Controller::slotFinishCreateWallet, Qt::QueuedConnection);

    // Get used addr
    connect(this, &Controller::startGetUsedAddresses, worker, &Worker::slotStartGetUsedAddresses, Qt::QueuedConnection);
    connect(worker, &Worker::finishGetUsedAddresses, this, &Controller::slotFinishGetUsedAddresses, Qt::QueuedConnection);

    // Get unused addr
    connect(this, &Controller::startGetUnusedAddresses, worker, &Worker::slotStartGetUnusedAddresses, Qt::QueuedConnection);
    connect(worker, &Worker::finishGetUnusedAddresses, this, &Controller::slotFinishGetUnusedAddresses, Qt::QueuedConnection);

    // Get txs
    connect(this, &Controller::startGetTransactionHistory, worker, &Worker::slotStartGetTransactionHistory, Qt::QueuedConnection);
    connect(worker, &Worker::finishGetTransactionHistory, this, &Controller::slotFinishGetTransactionHistory, Qt::QueuedConnection);

    // Get estimated fee
    connect(this, &Controller::startGetEstimatedFee, worker, &Worker::slotStartGetEstimatedFee, Qt::QueuedConnection);

    connect(this, &Controller::checkAndUnlockDevice, this, &Controller::slotCheckAndUnlockDevice, Qt::QueuedConnection);

    connect(this, &Controller::startSendPinToDevice, worker, &Worker::slotStartSendPinToDevice, Qt::QueuedConnection);
    connect(worker, &Worker::finishSendPinToDevice, this, &Controller::slotFinishSendPinToDevice, Qt::QueuedConnection);

    connect(this, &Controller::startSendPassphraseToDevice, worker, &Worker::slotStartSendPassphraseToDevice, Qt::QueuedConnection);
    connect(worker, &Worker::finishSendPassphraseToDevice, this, &Controller::slotFinishSendPassphraseToDevice, Qt::QueuedConnection);


    connect(this, &Controller::startRemoveAllWallets, worker, &Worker::slotStartRemoveAllWallets, Qt::QueuedConnection);
    connect(worker, &Worker::finishRemoveAllWallets, this, &Controller::slotFinishRemoveAllWallets, Qt::QueuedConnection);

    connect(this, &Controller::startRemoveAllSigners, worker, &Worker::slotStartRemoveAllSigners, Qt::QueuedConnection);
    connect(worker, &Worker::finishRemoveAllSigners, this, &Controller::slotFinishRemoveAllSigners, Qt::QueuedConnection);

    connect(this, &Controller::startMultiDeviceSync, worker, &Worker::slotStartMultiDeviceSync, Qt::QueuedConnection);

    connect(this, &Controller::startReloadUserDb, worker, &Worker::slotStartReloadUserDb, Qt::QueuedConnection);

    connect(this, &Controller::startReloadWallets, worker, &Worker::slotStartReloadWallets, Qt::QueuedConnection);
    connect(worker, &Worker::finishReloadWallets, this, &Controller::slotFinishReloadWallets, Qt::QueuedConnection);
    connect(worker, &Worker::finishReloadGroupWallets, this, &Controller::slotFinishReloadGroupWallets, Qt::QueuedConnection);

    connect(this, &Controller::startReloadMasterSigners, worker, &Worker::slotStartReloadMasterSigners, Qt::QueuedConnection);
    connect(worker, &Worker::finishReloadMasterSigners, this, &Controller::slotFinishReloadMasterSigners, Qt::QueuedConnection);

    connect(this, &Controller::startReloadRemoteSigners, worker, &Worker::slotStartReloadRemoteSigners, Qt::QueuedConnection);
    connect(worker, &Worker::finishReloadRemoteSigners, this, &Controller::slotFinishReloadRemoteSigners, Qt::QueuedConnection);

    connect(this, &Controller::startSyncWalletDb, worker, &Worker::slotStartSyncWalletDb, Qt::QueuedConnection);
    connect(worker, &Worker::finishSyncWalletDb, this, &Controller::slotFinishSyncWalletDb, Qt::QueuedConnection);

    workerThread.start();
}

Controller::~Controller() {
    if(workerThread.isRunning()){
        workerThread.quit();
        workerThread.wait(5);
    }
    this->disconnect();
}

void Controller::scanDevicesSync()
{
    QWarningMessage msg;
    QDeviceListModelPtr deviceList = bridge::nunchukGetDevices(msg);
    if(msg.type() != (int)EWARNING::WarningType::NONE_MSG){
        AppModel::instance()->showToast(msg.code(),msg.what(), (EWARNING::WarningType)msg.type());
    }
    AppModel::instance()->deviceList()->updateDeviceList(deviceList);
}

void Controller::slotFinishCreateMasterSigner(const QMasterSignerPtr ret,
                                              QString what,
                                              int type,
                                              int code)
{
    if(ret && type == (int)EWARNING::WarningType::NONE_MSG){
        QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
        QString selectFingerPrint = "";
        QString keyName = "";
        QMasterSignerPtr newsigner;
        if(mastersigners){
            AppModel::instance()->setMasterSignerList(mastersigners);
            newsigner =  AppModel::instance()->masterSignerList()->getMasterSignerByXfp(ret.data()->fingerPrint());
            AppModel::instance()->setMasterSignerInfo(newsigner);
            selectFingerPrint = newsigner->fingerPrint();
            keyName = newsigner->name();
        }
        DBG_INFO << selectFingerPrint << keyName;
        if (QSignerManagement::instance()->finishCreateMasterSigner())  {
            AppModel::instance()->setAddSignerStep(-1);
            return;
        }
        int last = QEventProcessor::instance()->getCurrentStates().last();
        DBG_INFO << last << selectFingerPrint << keyName;
        if(last == E::STATE_ID_SCR_ADD_HARDWARE_SIGNER){
            switch (QEventProcessor::instance()->currentFlow()) {
            case (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_USER_FREE: {
                if (auto w = AppModel::instance()->newWalletInfoPtr()) {
                    if (newsigner) {
                        w->updateNewKeyReplace(newsigner->fingerPrint());
                    }
                }
                break;
            }
            default:
                QEventProcessor::instance()->sendEvent(E::EVT_ADD_MASTER_SIGNER_RESULT);
                QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_NONE);
                break;
            }
        }
        else if (last == E::STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET){
            QEventProcessor::instance()->sendEvent(E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT);
        }
        else if (last == E::STATE_ID_SCR_ADD_HARDWARE) {
            auto dashboard = QGroupWallets::instance()->dashboardInfoPtr();
            if (dashboard && dashboard->canReplaceKey()) {
                if (QAssistedDraftWallets::IsByzantine()) {
                    QGroupWallets::instance()->setSelectFingerPrint(selectFingerPrint);
                    QGroupWallets::instance()->requestKeyReplacement(NULL);
                } else {
                    QUserWallets::instance()->setSelectFingerPrint(selectFingerPrint);
                    QUserWallets::instance()->requestKeyReplacement(NULL);
                }
            } else {
                if (QAssistedDraftWallets::IsByzantine()) {
                    QGroupWallets::instance()->setSelectFingerPrint(selectFingerPrint);
                    QGroupWallets::instance()->AddOrUpdateAKeyToDraftWallet();
                } else {
                    QUserWallets::instance()->setSelectFingerPrint(selectFingerPrint);
                    QUserWallets::instance()->AddOrUpdateAKeyToDraftWallet();
                }
            }
        }
        else if (last == E::STATE_ID_SCR_REPLACE_KEYS) {
            if (auto w = AppModel::instance()->newWalletInfoPtr()) {
                if (newsigner) {
                    w->updateNewKeyReplace(newsigner->fingerPrint());
                }
            }
        }

        if (last == E::STATE_ID_SCR_ADD_HARDWARE_SIGNER || last == E::STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
            || last == E::STATE_ID_SCR_REPLACE_KEYS) {
            QString msg = QString("<b>%1</b> has been added").arg(keyName);
            AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
        }
    }
    AppModel::instance()->setAddSignerStep(-1);
}

void Controller::slotFinishCreateRemoteSigner(const int event,
                                              const nunchuk::SingleSigner ret,
                                              QString what,
                                              int type,
                                              int code)
{
    if((int)EWARNING::WarningType::NONE_MSG == type){
        QSingleSignerPtr signer = QSingleSignerPtr(new QSingleSigner(ret));
        signer.data()->setSignerType((int)ENUNCHUCK::SignerType::AIRGAP);
        AppModel::instance()->setSingleSignerInfo(signer);
        QSingleSignerListModelPtr remoteSigners = bridge::nunchukGetRemoteSigners();
        if(remoteSigners){
            AppModel::instance()->setRemoteSignerList(remoteSigners);
        }
        DBG_INFO << event;
        if (QSignerManagement::instance()->finishCreateRemoteSigner()) {
            emit finishedCreateRemoteSigner();
            return;
        }
        if(event == E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER){
            QEventProcessor::instance()->sendEvent(E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT);
        }
        else {
            switch (QEventProcessor::instance()->currentFlow()) {
            case (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_USER_FREE: {
                if (auto w = AppModel::instance()->newWalletInfoPtr()) {
                    if (signer) {
                        w->updateNewKeyReplace(signer->fingerPrint());
                    }
                }
                break;
            }
            default:
                QEventProcessor::instance()->sendEvent(E::EVT_ADD_REMOTE_SIGNER_RESULT);
                QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_NONE);
                break;
            }
        }
        QString msg = QString("<b>%1</b> has been added").arg(signer->name());
        AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
    emit finishedCreateRemoteSigner();
}

void Controller::slotFinishScanDevices(const int state_id,
                                       std::vector<nunchuk::Device> ret,
                                       QString what,
                                       int type,
                                       int code)
{
    QDeviceListModelPtr deviceList(new DeviceListModel());
    if(type == (int)EWARNING::WarningType::NONE_MSG){
        for (nunchuk::Device it : ret) {
            QDevicePtr device = QDevicePtr(new QDevice(it));
            deviceList.data()->addDevice(device);
        }
        AppModel::instance()->setDeviceList(deviceList);
    }
    else{
        AppModel::instance()->setDeviceList(deviceList);
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
    if(AppModel::instance()->transactionInfoPtr()){
        emit AppModel::instance()->transactionInfoPtr()->nunchukTransactionChanged();
    }
    if (auto s = AppModel::instance()->masterSignerInfoPtr()) {
        if (auto d = s->devicePtr()) {
            d->setConnected(true);
            DBG_INFO << d->connected();
        }
    }
    DBG_INFO << ret.size() << deviceList->rowCount() << AppModel::instance()->newWalletInfoPtr().isNull();
    QGroupWallets::instance()->finishScanDevices();
    QUserWallets::instance()->finishScanDevices();
    if (auto w = AppModel::instance()->newWalletInfoPtr()) {
        w->refreshScanDevices();
    }
    emit finishedScanDevices();
    emit checkAndUnlockDevice(state_id);
}

void Controller::slotFinishSigningTransaction(const QString &walletId,
                                              nunchuk::Transaction result,
                                              QString what,
                                              int type,
                                              int code,
                                              QString masterSignerId,
                                              bool isSoftware)
{
    if(type == (int)EWARNING::WarningType::NONE_MSG){
        if(isSoftware) {
            QWarningMessage warningmsg;
            bridge::nunchukClearSignerPassphrase(masterSignerId,
                                                 warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
                if(mastersigners){
                    AppModel::instance()->setMasterSignerList(mastersigners);
                }
            }
        }
        QString tx_id = QString::fromStdString(result.get_txid());
        QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(walletId);
        if(AppModel::instance()->transactionInfo())
            if(qUtils::strCompare(tx_id, AppModel::instance()->transactionInfo()->txid()) && qUtils::strCompare(walletId, AppModel::instance()->transactionInfo()->walletId())){
                DBG_INFO << "TAPROOT-TEST";
                AppModel::instance()->transactionInfo()->setNunchukTransaction(result);
                if(wallet){
                    wallet.data()->SignAsisstedTxs(tx_id, QString::fromStdString(result.get_psbt()), QString::fromStdString(result.get_memo()));
                }
            }
        AppModel::instance()->requestSyncWalletDb(walletId);
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
        if(!isSoftware){
            startScanDevices(E::STATE_ID_SCR_TRANSACTION_INFO);
        }
    }
    emit finishedSigningTransaction();
}

void Controller::slotFinishHealthCheckMasterSigner(const int state_id,
                                                   const int status,
                                                   const QString &message,
                                                   const QString &signature,
                                                   const QString &xfp,
                                                   QString what,
                                                   int type,
                                                   int code)
{
    if((int)EWARNING::WarningType::NONE_MSG == type){
        QString master_signer_id = AppModel::instance()->masterSignerInfo()->id();
        QWarningMessage msg;
        bridge::nunchukClearSignerPassphrase(master_signer_id, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
            if(mastersigners){
                AppModel::instance()->setMasterSignerList(mastersigners);
            }
            QMasterSignerPtr currentMastersigner = mastersigners.data()->getMasterSignerById(master_signer_id);
            if(currentMastersigner){
                AppModel::instance()->setMasterSignerInfo(currentMastersigner);
            }
        }
        AppModel::instance()->masterSignerInfo()->setHealth(status);
        AppModel::instance()->masterSignerInfo()->setSignature(signature);
        AppModel::instance()->masterSignerInfo()->setMessage(message);
        if(AppModel::instance()->walletList()) {
            AppModel::instance()->walletList()->updateSignerHealthStatus(xfp, status, QDateTime::currentDateTime().toTime_t());
        }
        emit finishedHealthCheckMasterSigner(true);
    }
    else{
        if(type == (int)EWARNING::WarningType::EXCEPTION_MSG && nunchuk::HWIException::DEVICE_CONN_ERROR == code){
            startScanDevices(state_id);
        }
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
        emit finishedHealthCheckMasterSigner(false);
    }
}

void Controller::slotFinishTopXPUBsMasterSigner(const QVariant &data)
{
    QMap<QString,QVariant> maps = data.toMap();
    QString masterSignerId = maps["masterSignerId"].toString();
    QString what = maps["what"].toString();
    int type = maps["type"].toInt();
    int code = maps["code"].toInt();
    if((int)EWARNING::WarningType::NONE_MSG == type){
        QWarningMessage msg;
        bridge::nunchukClearSignerPassphrase(masterSignerId, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            AppModel::instance()->showToast(0, STR_CPP_097, EWARNING::WarningType::SUCCESS_MSG );
        }
    }
    else{
        if(type == (int)EWARNING::WarningType::EXCEPTION_MSG && nunchuk::HWIException::DEVICE_CONN_ERROR == code){
            int state_id = maps["state_id"].toInt();
            startScanDevices(state_id);
            AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
        }
        else{
            AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
        }
    }
    emit AppModel::instance()->finishedTopXPUBsMasterSigner();
}

void Controller::slotFinishHealthCheckRemoteSigner(const int status,
                                                   QString what,
                                                   int type,
                                                   int code)
{
    if((int)EWARNING::WarningType::NONE_MSG == type){
        AppModel::instance()->singleSignerInfo()->setHealth(status);
        emit finishedHealthCheckRemoteSigner(true);
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
        emit finishedHealthCheckRemoteSigner(false);
    }
    
}

void Controller::slotFinishDisplayAddress(bool result)
{
    emit finishedDisplayAddress(result);
}

void Controller::slotFinishRescanBlockchain()
{
    DBG_INFO;
}

void Controller::slotFinishCreateSoftwareSigner(nunchuk::MasterSigner ret,
                                                QString what,
                                                int type,
                                                int code)
{
    if(type != (int)EWARNING::WarningType::EXCEPTION_MSG){
        QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
        QMasterSignerPtr newsigner;
        if(mastersigners){
            AppModel::instance()->setMasterSignerList(mastersigners);
            newsigner =  AppModel::instance()->masterSignerList()->getMasterSignerByXfp(QString::fromStdString(ret.get_device().get_master_fingerprint()));
            AppModel::instance()->setMasterSignerInfo(newsigner);
        }

        if (QSignerManagement::instance()->finishCreateSoftwareSigner()) {
            return;
        }

        switch (QEventProcessor::instance()->currentFlow()) {
        case (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY:
        {
            uint last = QEventProcessor::instance()->getCurrentStates().last();
            if(last == (int)E::STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION){
                AppModel::instance()->showToast(0, STR_CPP_102.arg(QString::fromStdString(ret.get_name())), EWARNING::WarningType::SUCCESS_MSG);
            }
            else if(last == (int)E::STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY){
                QEventProcessor::instance()->sendEvent(E::EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED);
            }
            else if(last == (int)E::STATE_ID_SCR_UNLOCK_DB){
                QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
                AppModel::instance()->makeMatrixInstanceForAccount();
                QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
                if(pKey){
                    timeoutHandler(3000,[pKey](){
                        AppModel::instance()->showToast(0,STR_CPP_108.arg(pKey->name()), EWARNING::WarningType::SUCCESS_MSG);
                        QWarningMessage msg;
                        bridge::nunchukClearSignerPassphrase(pKey->fingerPrint(),msg);
                    });
                }
            }
            else{}
            break;
        }
        case (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY:
        {
            QEventProcessor::instance()->sendEvent(E::EVT_ADD_SOFTWARE_SIGNER_RESULT);
            AppModel::instance()->setPrimaryKey(ClientController::instance()->getMe().username);
            break;
        }
        case (int)ENUNCHUCK::IN_FLOW::FLOW_ADD_SIGNER:
        case (int)ENUNCHUCK::IN_FLOW::FLOW_ADD_WALLET:
        {
            QEventProcessor::instance()->sendEvent(E::EVT_ADD_SOFTWARE_SIGNER_RESULT);
            QString message = QString("<b>%1</b> has been added").arg(newsigner->name());
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::SUCCESS_MSG);

            break;
        }
        case (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_USER_FREE: {
            if (auto w = AppModel::instance()->newWalletInfoPtr()) {
                if (newsigner) {
                    w->updateNewKeyReplace(newsigner->fingerPrint());
                    AppModel::instance()->showToast(0, STR_CPP_062, EWARNING::WarningType::SUCCESS_MSG);
                }
            }
            break;
        }
        default:
            QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_NONE);
            break;
        }
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
}

void Controller::slotFinishCreateSoftwareSignerXprv(nunchuk::MasterSigner ret, QString what, int type, int code)
{
    if(type != (int)EWARNING::WarningType::EXCEPTION_MSG){
        QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
        QMasterSignerPtr newsigner;
        if(mastersigners){
            AppModel::instance()->setMasterSignerList(mastersigners);
            newsigner =  AppModel::instance()->masterSignerList()->getMasterSignerByXfp(QString::fromStdString(ret.get_device().get_master_fingerprint()));
            AppModel::instance()->setMasterSignerInfo(newsigner);
        }

        if (QSignerManagement::instance()->finishCreateSoftwareSignerXprv()) {
            return;
        }

        switch (QEventProcessor::instance()->currentFlow()) {
        case (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY:
        {
            uint last = QEventProcessor::instance()->getCurrentStates().last();
            if(last == (int)E::STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION){
                AppModel::instance()->showToast(0, STR_CPP_102.arg(QString::fromStdString(ret.get_name())), EWARNING::WarningType::SUCCESS_MSG);
            }
            else if(last == (int)E::STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY){
                QEventProcessor::instance()->sendEvent(E::EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED);
            }
            else if(last == (int)E::STATE_ID_SCR_UNLOCK_DB){
                QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
                AppModel::instance()->makeMatrixInstanceForAccount();
                QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
                if(pKey){
                    timeoutHandler(3000,[pKey](){
                        AppModel::instance()->showToast(0,STR_CPP_108.arg(pKey->name()), EWARNING::WarningType::SUCCESS_MSG);
                        QWarningMessage msg;
                        bridge::nunchukClearSignerPassphrase(pKey->fingerPrint(),msg);
                    });
                }
            }
            else{}
            break;
        }
        case (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY:
        {
            QEventProcessor::instance()->sendEvent(E::EVT_ADD_SOFTWARE_SIGNER_RESULT);
            AppModel::instance()->setPrimaryKey(ClientController::instance()->getMe().username);
            break;
        }
        case (int)ENUNCHUCK::IN_FLOW::FLOW_ADD_SIGNER:
        case (int)ENUNCHUCK::IN_FLOW::FLOW_ADD_WALLET:
        {
            QEventProcessor::instance()->sendEvent(E::EVT_ADD_SOFTWARE_SIGNER_RESULT);
            QString message = QString("<b>%1</b> has been added").arg(newsigner->name());
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::SUCCESS_MSG);

            break;
        }
        case (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_USER_FREE: {
            if (auto w = AppModel::instance()->newWalletInfoPtr()) {
                if (newsigner) {
                    w->updateNewKeyReplace(newsigner->fingerPrint());
                    AppModel::instance()->showToast(0, STR_CPP_062, EWARNING::WarningType::SUCCESS_MSG);
                }
            }
            break;
        }
        default:
            QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_NONE);
            break;
        }
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
}

void Controller::slotFinishCreateWallet(nunchuk::Wallet ret,
                                        QString what,
                                        int type,
                                        int code)
{
    DBG_INFO << "walletListCurrentIndex";
    if(type != (int)EWARNING::WarningType::EXCEPTION_MSG){
        QString wallet_id = QString::fromStdString(ret.get_id());
        AppModel::instance()->walletList()->addWallet(bridge::convertWallet(ret));
        AppModel::instance()->resetSignersChecked();
        AppModel::instance()->walletList()->requestSort(WalletListModel::WalletRoles::wallet_createDate_Role, Qt::AscendingOrder);
        int index = AppModel::instance()->walletList()->getWalletIndexById(wallet_id);
        bool replaceFree = false;
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            replaceFree = w->replaceFree();
        }
        DBG_INFO << replaceFree;
        if(-1 != index && replaceFree == false){
            AppModel::instance()->setWalletListCurrentIndex(index);
        }
        QEventProcessor::instance()->sendEvent(E::EVT_ADD_WALLET_SUCCESSFULLY);
        AppModel::instance()->showToast(0, STR_CPP_064, EWARNING::WarningType::SUCCESS_MSG);
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->walletCreateDone();
            QString replaceFlow = w->replaceFlow();
            auto newWallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
            newWallet->setReplaceFlow(replaceFlow);
            AppModel::instance()->setNewWalletInfo(newWallet);
        }
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
    emit finishedCreateWallet();
}

void Controller::slotFinishBackupWallet(QString what,
                                        int type,
                                        int code)
{

}

void Controller::slotFinishBalanceChanged(const QString &id,
                                          const qint64 balance)
{

}

void Controller::slotFinishTransactionChanged(const QString &tx_id,
                                              const int status,
                                              const QString &wallet_id,
                                              nunchuk::Transaction tx)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(AppModel::instance()->walletList()){
        QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        if(wallet){
            QTransactionPtr trans = wallet.data()->SyncAssistedTxs(tx);
            if(trans){
                wallet.data()->transactionHistory()->updateTransaction(tx_id, trans);
                if(AppModel::instance()->transactionInfo()){
                    QString current_tx_wallet_id = AppModel::instance()->transactionInfo()->walletId();
                    QString current_tx_id        = AppModel::instance()->transactionInfo()->txid();
                    if(qUtils::strCompare(wallet_id, current_tx_wallet_id) && qUtils::strCompare(tx_id, current_tx_id)){
                        AppModel::instance()->setTransactionInfo(trans);
                    }
                }
            }
        }
    }
    CLIENT_INSTANCE->transactionChanged(wallet_id, tx_id, status, tx.get_height());
    qApp->restoreOverrideCursor();
}

void Controller::slotFinishBlockChanged(const int height,
                                        const QString &hex_header)
{
    AppModel::instance()->setChainTip(height);
}

void Controller::slotFinishGetUsedAddresses(const QString &wallet_id,
                                            const QStringList &addr, const QStringList &changeAddr)
{
    if(AppModel::instance()->walletList()){
        QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        if(wallet){
            wallet.data()->setUsedAddressList(addr);
            wallet.data()->setUsedChangeAddressList(changeAddr);
        }
    }
}

void Controller::slotFinishGetUnusedAddresses(const QString& wallet_id,
                                              const QStringList &addr,
                                              const QStringList &changeAddr)
{
    if(AppModel::instance()->walletList()){
        QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        if(wallet){
            wallet.data()->setunUsedAddressList(addr);
            wallet.data()->setUnUsedChangeddAddressList(changeAddr);
        }
    }
}

void Controller::slotFinishGetTransactionHistory(const QString wallet_id,
                                                 std::vector<nunchuk::Transaction> ret)
{
    QWalletPtr wallet = NULL;
    if(AppModel::instance()->walletInfo()){
        if(qUtils::strCompare(AppModel::instance()->walletInfo()->walletId(), wallet_id)){
            wallet = AppModel::instance()->walletInfoPtr();
        }
    }
    else{
        if(AppModel::instance()->walletList()){
            wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        }
    }
    if(wallet){
        wallet.data()->transactionHistory()->updateTransaction(wallet_id, ret);
        wallet.data()->transactionHistory()->requestSort(TransactionListModel::TransactionRoles::transaction_blocktime_role, Qt::DescendingOrder);
        emit wallet.data()->transactionHistoryChanged();
    }
    emit finishedGetTransactionHistory();
}

void Controller::slotCheckAndUnlockDevice(const int state_id)
{
    DBG_INFO << state_id;
    if(AppModel::instance()->deviceList()){
        for (int i = 0; i < AppModel::instance()->deviceList()->fullList().count(); i++) {
            QDevicePtr selectedDv = AppModel::instance()->deviceList()->fullList()[i];
            if(selectedDv){
                DBG_INFO << selectedDv.data()->needsPinSent() << selectedDv.data()->needsPassPhraseSent();
                if(selectedDv.data()->needsPinSent()){
                    QWarningMessage msgwarning;
                    bridge::nunchukPromtPinOnDevice(selectedDv, msgwarning);
                    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                        QMap<QString, QVariant> pinData;
                        pinData["state_id"] = state_id;
                        pinData["device_idx"] = i;
                        QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PIN, pinData);
                    }
                    else{
                        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
                    }
                }
                else if(selectedDv.data()->needsPassPhraseSent()){
                    QMap<QString, QVariant> passPhraseData;
                    passPhraseData["state_id"] = state_id;
                    passPhraseData["device_idx"] = i;
                    passPhraseData["is_software"] = false;
                    QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
                }
                else{}
            }
            else{ continue;}
        }
    }
    emit finishedScanDevices();
}

void Controller::slotFinishSendPinToDevice(const int state_id,
                                           QString what,
                                           int type,
                                           int code)
{
    if((int)EWARNING::WarningType::NONE_MSG == type){
        AppModel::instance()->startScanDevices(state_id);
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
    QEventProcessor::instance()->sendEvent(E::EVT_INPUT_PIN_CLOSE);
    emit finishSendPinToDevice(type);
}

void Controller::slotFinishSendPassphraseToDevice(const int state_id,
                                                  QString what,
                                                  int type,
                                                  int code)
{
    if((int)EWARNING::WarningType::NONE_MSG == type){
        AppModel::instance()->startScanDevices(state_id);
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
    QEventProcessor::instance()->sendEvent(E::EVT_INPUT_PASSPHRASE_CLOSE);
    emit finishSendPassphraseToDevice(type);
}

void Controller::slotFinishRemoveAllWallets()
{
    if(AppModel::instance()->walletList()){
        AppModel::instance()->walletList()->cleardata();
        AppModel::instance()->setWalletInfoByIndex(-1);
    }
    emit AppModel::instance()->startRemoveAllSigners();
}

void Controller::slotFinishRemoveAllSigners()
{
    if(AppModel::instance()->masterSignerList()){
        AppModel::instance()->masterSignerList()->cleardata();
        AppModel::instance()->setMasterSignerInfo(QMasterSignerPtr(new QMasterSigner));
    }
    if(AppModel::instance()->remoteSignerList()){
        AppModel::instance()->remoteSignerList()->cleardata();
        AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new QSingleSigner));
    }
    CLIENT_INSTANCE->requestSignout();
}

void Controller::slotFinishReloadMasterSigners(std::vector<nunchuk::MasterSigner> masterSigners)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QMasterSignerListModelPtr ret = bridge::nunchukConvertMasterSigners(masterSigners);
    if(ret){
        AppModel::instance()->setMasterSignerList(ret);
    }
}

void Controller::slotFinishReloadRemoteSigners(std::vector<nunchuk::SingleSigner> remoteSigners)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QSingleSignerListModelPtr ret = bridge::nunchukConvertRemoteSigners(remoteSigners);
    if(ret){
        AppModel::instance()->setRemoteSignerList(ret);
    }
}

void Controller::slotFinishReloadWallets(std::vector<nunchuk::Wallet> wallets)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QWalletListModelPtr ret = bridge::nunchukConvertWallets(wallets);
    if(ret){
        AppModel::instance()->setWalletList(ret);
        QString lastWalletId = bridge::nunchukGetSelectedWallet();
        int lastIndex = lastWalletId == "" ? 0 : AppModel::instance()->walletList()->getWalletIndexById(lastWalletId);
        lastIndex = max(0, lastIndex);
        AppModel::instance()->setWalletListCurrentIndex(lastIndex, true);
        AppModel::instance()->walletListCurrentIndexChanged();
        if(ONLINE_MODE == bridge::nunchukCurrentMode() && CLIENT_INSTANCE->isNunchukLoggedIn()){
            emit WalletsMng->getListWalletFinish();
        }
    }
    if(ONLINE_MODE == bridge::nunchukCurrentMode()){
        if(CLIENT_INSTANCE->isNunchukLoggedIn() && CLIENT_INSTANCE->isMatrixLoggedIn()){
            if(CLIENT_INSTANCE->rooms()){
                CLIENT_INSTANCE->rooms()->downloadRoomWallets();
            }
        }
    }
}

void Controller::slotFinishReloadGroupWallets(std::vector<nunchuk::Wallet> wallets)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    QWalletListModelPtr ret = bridge::nunchukConvertWallets(wallets);
    if(ret){
        AppModel::instance()->setGroupWalletList(ret);
        if (auto list = AppModel::instance()->groupWalletListPtr()) {
            int index = list->currentIndex();
            if(index < list->count()) {
                list->setCurrentIndex(index);
            }
            else {
                list->setCurrentIndex(0);
            }
        }
    }
}

void Controller::slotFinishSyncWalletDb(const QString &wallet_id)
{
    if(AppModel::instance()->walletList()){
        auto wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        if (wallet) {
            wallet.data()->GetCoinControlFromServer();
        }
    }
}
