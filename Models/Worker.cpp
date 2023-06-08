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
#include "QQuickViewer.h"
#include "Draco.h"
#include "localization/STR_CPP.h"

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

void Worker::slotStartCreateMasterSigner(const QString &id,
                                         const int deviceIndex)
{
    QWarningMessage msg;
    AppModel::instance()->setAddSignerStep(0);
    AppModel::instance()->setAddSignerPercentage(0);
    QMasterSignerPtr ret = bridge::nunchukCreateMasterSigner(id,
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
        DBG_INFO << "CREATE MASTER SIGNER FAIL";
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
                                         const int event)
{
    QWarningMessage msg;
    nunchuk::SingleSigner ret = bridge::nunchukCreateOriginSigner(name,
                                                                  xpub,
                                                                  public_key,
                                                                  derivation_path,
                                                                  master_fingerprint,
                                                                  msg);
    AppModel::instance()->setWalletsUsingSigner({});
    emit finishCreateRemoteSigner(event,
                                  ret,
                                  msg.what(),
                                  msg.type(),
                                  msg.code());
}

void Worker::slotStartScanDevices(const int state_id) {
    QWarningMessage msg;
    std::vector<nunchuk::Device> deviceList = bridge::nunchukGetOriginDevices(msg);
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
    if(AppModel::instance()->transactionInfo()){
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
        if(AppModel::instance()->transactionInfo()->roomId() != ""){ // shared
            QNunchukRoom* room = CLIENT_INSTANCE->GetRoomById(AppModel::instance()->transactionInfo()->roomId());
            if(room){
                QString init_event_id = AppModel::instance()->transactionInfo()->initEventId();
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
                msgwarning.setWarningMessage(0,
                                             STR_CPP_059,
                                             EWARNING::WarningType::ERROR_MSG,
                                             STR_CPP_059);
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
                DBG_INFO << "selectedDv";
                nunchuk::Transaction trans = bridge::nunchukSignTransactionThread(walletId,
                                                                                  txid,
                                                                                  selectedDv,
                                                                                  msgwarning);
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
                                             EWARNING::WarningType::ERROR_MSG,
                                             STR_CPP_055);
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
                                              const QString &signerId,
                                              const QString &message)
{
    QString out_signature = "";
    QString out_path = "";
    QWarningMessage msgwarning;
    int health = (int)bridge::nunchukHealthCheckMasterSigner(signerId,
                                                             message,
                                                             out_signature,
                                                             out_path,
                                                             msgwarning);
    emit finishHealthCheckMasterSigner(state_id,
                                       health,
                                       message,
                                       out_signature,
                                       signerId,
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

void Worker::slotStartHealthCheckRemoteSigner()
{
    QWarningMessage msgwarning;
    int status = (int)bridge::nunchukHealthCheckSingleSigner(AppModel::instance()->singleSignerInfoPtr(),
                                                             msgwarning);
    emit finishHealthCheckRemoteSigner(status,
                                       msgwarning.what(),
                                       msgwarning.type(),
                                       msgwarning.code());
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
        if(deviceList && AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->singleSignersAssigned()){
            QStringList xfps = deviceList->getXFPList();
            foreach (QString xfp, xfps) {
                ret = bridge::nunchukDisplayAddressOnDevice(wallet_id,
                                                            address,
                                                            xfp);
            }
        }
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
                                           const QString passphrase)
{
    QWarningMessage msg;
    bool isPrimaryKey = QQuickViewer::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY ||
            QQuickViewer::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY;
    AppModel::instance()->setAddSignerPercentage(0);
    QMasterSignerPtr ret = bridge::nunchukCreateSoftwareSigner(name,
                                                               mnemonic,
                                                               passphrase,
                                                               msg,
                                                               isPrimaryKey);
    AppModel::instance()->setAddSignerPercentage(100);
    AppModel::instance()->setWalletsUsingSigner({});
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && ret){
        emit finishCreateSoftwareSigner(ret,
                                        msg.what(),
                                        msg.type(),
                                        msg.code());
    }
    else{
        emit finishCreateSoftwareSigner(NULL,
                                        msg.what(),
                                        msg.type(),
                                        msg.code());
    }
}

void Worker::slotStartCreateWallet(bool backup,
                                   QString file_path)
{
    QWarningMessage msgWarning;
    nunchuk::Wallet ret = bridge::nunchukCreateOriginWallet(AppModel::instance()->newWalletInfo()->name(),
                                                            AppModel::instance()->newWalletInfo()->m(),
                                                            AppModel::instance()->newWalletInfo()->n(),
                                                            AppModel::instance()->newWalletInfo()->singleSignersAssigned(),
                                                            (ENUNCHUCK::AddressType)AppModel::instance()->newWalletInfo()->addressType().toInt(),
                                                            AppModel::instance()->newWalletInfo()->escrow(),
                                                            AppModel::instance()->newWalletInfo()->description(),
                                                            msgWarning);
    emit finishCreateWallet(ret,
                            msgWarning.what(),
                            msgWarning.type(),
                            msgWarning.code());
    if(backup){
        bridge::nunchukExportWallet(QString::fromStdString(ret.get_id()),
                                    file_path,
                                    nunchuk::ExportFormat::BSMS);
    }
}

void Worker::slotStartBalanceChanged(const QString &id,
                                     const qint64 balance)
{
    if(AppModel::instance()->walletList()){
        AppModel::instance()->walletList()->updateBalance(id, balance);
    }
//    emit finishBalanceChanged(id, balance); //FIXME FOR IMPROVEMENT
}

void Worker::slotStartTransactionChanged(const QString &tx_id,
                                         const int status,
                                         const QString &wallet_id)
{
    QWarningMessage msgWarning;
    nunchuk::Transaction tx = bridge::nunchukGetOriginTransaction(wallet_id, tx_id, msgWarning);
#if 0
    if(tx.is_receive()){
        DBG_INFO << "FIXME" << tx.get_sub_amount();
    }
    else{
        std::vector<nunchuk::TxOutput> addrs = tx.get_outputs();
        int index_change = tx.get_change_index();
        if(index_change >= 0 && index_change < (int)addrs.size()) {
            addrs.erase(addrs.begin() + index_change);
        }
        qint64 sub = 0;
        for (int i = 0; i < (int)addrs.size(); i++){
            std::pair<std::string, nunchuk::Amount> item = addrs.at(i);
            sub=sub+item.second;
        }
        DBG_INFO << "FIXME" << sub;
    }
#endif
    emit finishTransactionChanged(tx_id,
                                  status,
                                  wallet_id,
                                  tx);
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
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(wallet_id != ""){
        std::vector<nunchuk::Transaction> trans_result = bridge::nunchukGetOriginTransactionHistory(wallet_id);
        emit finishGetTransactionHistory(wallet_id, trans_result);
    }
    qApp->restoreOverrideCursor();
}

void Worker::slotStartGetEstimatedFee()
{
    if(AppSetting::instance()->primaryServer() == (int)AppSetting::Chain::MAIN){
        Draco::instance()->feeRates();
    }
}

void Worker::slotStartDeviceClosePin(int device_index)
{
    QWarningMessage msgwarning;
    QDevicePtr selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(device_index) ;
    bridge::nunchukSendPinToDevice(selectedDv, "1", msgwarning);
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

void Worker::slotStartReloadWallets()
{
    QWarningMessage msg;
    std::vector<nunchuk::Wallet> wallets = bridge::nunchukGetOriginWallets(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadWallets(wallets);
    }
}

void Worker::slotStartReloadMasterSigners()
{
    QWarningMessage msg;
    std::vector<nunchuk::MasterSigner> masters = bridge::nunchukGetOriginMasterSigners(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadMasterSigners(masters);
    }
}

void Worker::slotStartReloadRemoteSigners()
{
    QWarningMessage msg;
    std::vector<nunchuk::SingleSigner> remotes = bridge::nunchukGetOriginRemoteSigners(msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        emit finishReloadRemoteSigners(remotes);
    }
}

void Worker::slotStartSyncWalletDb(const QString &wallet_id)
{
    if(wallet_id != ""){
        QStringList used_addr = bridge::nunchukGetUsedAddresses(wallet_id, false);
        QStringList used_change_addr = bridge::nunchukGetUsedAddresses(wallet_id, true);

        QStringList unused_addr = bridge::nunchukGetUnusedAddresses(wallet_id, false);
        QStringList unsued_chabge_addr = bridge::nunchukGetUnusedAddresses(wallet_id, true);

        QWalletPtr wallet = NULL;
        if(AppModel::instance()->walletInfo()){
            if(0 == QString::compare(AppModel::instance()->walletInfo()->id(), wallet_id, Qt::CaseInsensitive)){
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
            wallet.data()->setUnUsedChangeddAddressList(unsued_chabge_addr);
        }
        std::vector<nunchuk::Transaction> trans_result = bridge::nunchukGetOriginTransactionHistory(wallet_id);
        emit finishGetTransactionHistory(wallet_id, trans_result);
    }
    emit finishSyncWalletDb(wallet_id);
}

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

    connect(this, &Controller::startDeviceClosePin, worker, &Worker::slotStartDeviceClosePin, Qt::QueuedConnection);

    connect(this, &Controller::startSendPinToDevice, worker, &Worker::slotStartSendPinToDevice, Qt::QueuedConnection);
    connect(worker, &Worker::finishSendPinToDevice, this, &Controller::slotFinishSendPinToDevice, Qt::QueuedConnection);

    connect(this, &Controller::startSendPassphraseToDevice, worker, &Worker::slotStartSendPassphraseToDevice, Qt::QueuedConnection);
    connect(worker, &Worker::finishSendPassphraseToDevice, this, &Controller::slotFinishSendPassphraseToDevice, Qt::QueuedConnection);


    connect(this, &Controller::startRemoveAllWallets, worker, &Worker::slotStartRemoveAllWallets, Qt::QueuedConnection);
    connect(worker, &Worker::finishRemoveAllWallets, this, &Controller::slotFinishRemoveAllWallets, Qt::QueuedConnection);

    connect(this, &Controller::startRemoveAllSigners, worker, &Worker::slotStartRemoveAllSigners, Qt::QueuedConnection);
    connect(worker, &Worker::finishRemoveAllSigners, this, &Controller::slotFinishRemoveAllSigners, Qt::QueuedConnection);

    connect(this, &Controller::startMultiDeviceSync, worker, &Worker::slotStartMultiDeviceSync, Qt::QueuedConnection);

    connect(this, &Controller::startReloadWallets, worker, &Worker::slotStartReloadWallets, Qt::QueuedConnection);
    connect(worker, &Worker::finishReloadWallets, this, &Controller::slotFinishReloadWallets, Qt::QueuedConnection);

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
        workerThread.wait();
    }
    this->disconnect();
}

void Controller::scanDevicesSync()
{
    QWarningMessage msg;
    QDeviceListModelPtr deviceList = bridge::nunchukGetDevices(msg);
    if(msg.type() != (int)EWARNING::WarningType::NONE_MSG){
        AppModel::instance()->showToast(msg.code(),msg.what(),
                                        (EWARNING::WarningType)msg.type(),
                                        STR_CPP_056);
    }
    AppModel::instance()->deviceList()->updateDeviceList(deviceList);
    AppModel::instance()->checkDeviceUsableToSign();
    AppModel::instance()->checkDeviceUsableToAdd();
}

void Controller::slotFinishCreateMasterSigner(const QMasterSignerPtr ret,
                                              QString what,
                                              int type,
                                              int code)
{
    if(ret && type != (int)EWARNING::WarningType::EXCEPTION_MSG){
        QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
        if(mastersigners){
            AppModel::instance()->setMasterSignerList(mastersigners);
            QMasterSignerPtr newsigner =  AppModel::instance()->masterSignerList()->getMasterSignerByXfp(ret.data()->fingerPrint());
            AppModel::instance()->setMasterSignerInfo(newsigner);
        }
        if(QQuickViewer::instance()->getCurrentStates().last() == E::STATE_ID_SCR_ADD_HARDWARE_SIGNER){
            QQuickViewer::instance()->sendEvent(E::EVT_ADD_MASTER_SIGNER_RESULT);
        }
        else{
            QQuickViewer::instance()->sendEvent(E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT);
        }
        if (AppModel::instance()->addSignerWizard() != 3) {
            AppModel::instance()->showToast(0,
                                            STR_CPP_057,
                                            EWARNING::WarningType::SUCCESS_MSG,
                                            STR_CPP_057);
        }
    }
    else{
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_060);
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
        if(event == E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER){
            QQuickViewer::instance()->sendEvent(E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT);
        }
        else{
            QQuickViewer::instance()->sendEvent(E::EVT_ADD_REMOTE_SIGNER_RESULT);
        }
        AppModel::instance()->showToast(0,
                                        STR_CPP_057,
                                        EWARNING::WarningType::SUCCESS_MSG,
                                        STR_CPP_057);
    }
    else{
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_058);
    }
    emit finishedCreateRemoteSigner();
}

void Controller::slotFinishScanDevices(const int state_id,
                                       std::vector<nunchuk::Device> ret,
                                       QString what,
                                       int type,
                                       int code) {
    QDeviceListModelPtr deviceList(new DeviceListModel());
    if(type == (int)EWARNING::WarningType::NONE_MSG){
        for (nunchuk::Device it : ret) {
//            QString type = QString::fromStdString(it.get_type());
//            QString path = QString::fromStdString(it.get_path());
//            QString model = QString::fromStdString(it.get_model());
//            QString master_fingerprint = QString::fromStdString(it.get_master_fingerprint());
//            bool connected = it.connected();
//            bool needs_pass_phrase_sent = it.needs_pass_phrase_sent();
//            bool needs_pin_sent = it.needs_pin_sent();
//            DBG_INFO << model << type << "XFP:" << master_fingerprint << "PPhrase:" << needs_pass_phrase_sent << "PIN:" << needs_pin_sent;
            QDevicePtr device = QDevicePtr(new QDevice(it));
            deviceList.data()->addDevice(device);
        }
        AppModel::instance()->setDeviceList(deviceList);
    }
    else{
        AppModel::instance()->setDeviceList(deviceList);
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_056);
    }
    AppModel::instance()->checkDeviceUsableToSign();
    AppModel::instance()->checkDeviceUsableToAdd();
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
        if(wallet && wallet->isAssistedWallet()){
            QJsonObject data = Draco::instance()->assistedWalletSignTx(walletId,tx_id,QString::fromStdString(result.get_psbt()),QString::fromStdString(result.get_memo()));
            QJsonObject transaction = data.value("transaction").toObject();
            QString status = transaction.value("status").toString();
            QString psbt = transaction.value("psbt").toString();
            if (status == "PENDING_CONFIRMATION" ||
                status == "CONFIRMED" ||
                status == "NETWORK_REJECTED"){
                QWarningMessage _msg;
                bridge::nunchukImportPsbt(walletId, psbt, _msg);
                QString id = transaction.value("id").toString();
                QString hex = transaction.value("hex").toString();
                QString reject_msg = transaction.value("reject_msg").toString();
                bridge::nunchukUpdateTransaction(walletId, QString::fromStdString(result.get_txid()), id, hex, reject_msg, _msg);
            } else if (status == "READY_TO_BROADCAST" ||
                       status == "PENDING_SIGNATURES") {
                QWarningMessage _msg;
                bridge::nunchukImportPsbt(walletId, psbt, _msg);
            }
        }
        if(AppModel::instance()->transactionInfo() && (0 == QString::compare(tx_id, AppModel::instance()->transactionInfo()->txid(), Qt::CaseInsensitive))){
            if(0 == QString::compare(walletId, AppModel::instance()->transactionInfo()->walletId(), Qt::CaseInsensitive))
            {
                AppModel::instance()->setTransactionInfo(bridge::convertTransaction(result,walletId));
            }
        }
        AppModel::instance()->requestSyncWalletDb(walletId);
    }
    else{
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_059);
    }
    AppModel::instance()->checkDeviceUsableToSign();
    emit finishedSigningTransaction();
}

void Controller::slotFinishHealthCheckMasterSigner(const int state_id,
                                                   const int status,
                                                   const QString &message,
                                                   const QString &signature,
                                                   const QString &id,
                                                   QString what,
                                                   int type,
                                                   int code)
{
    if((int)EWARNING::WarningType::NONE_MSG == type){
        QWarningMessage msg;
        bridge::nunchukClearSignerPassphrase(id, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
            if(mastersigners){
                AppModel::instance()->setMasterSignerList(mastersigners);
            }
            QMasterSignerPtr currentMastersigner = mastersigners.data()->getMasterSignerById(id);
            if(currentMastersigner){
                AppModel::instance()->setMasterSignerInfo(currentMastersigner);
            }
        }
        AppModel::instance()->masterSignerInfo()->setHealth(status);
        AppModel::instance()->masterSignerInfo()->setSignature(signature);
        AppModel::instance()->masterSignerInfo()->setMessage(message);
        if(AppModel::instance()->walletList())
        {
            AppModel::instance()->walletList()->updateSignerHealthStatus(id,
                                                                         status,
                                                                         QDateTime::currentDateTime().toTime_t());
        }
    }
    else{
        if(type == (int)EWARNING::WarningType::EXCEPTION_MSG && nunchuk::HWIException::DEVICE_CONN_ERROR == code){
            startScanDevices(state_id);
        }
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_061);
    }
    emit finishedHealthCheckMasterSigner();
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
            AppModel::instance()->showToast(0,
                                            STR_CPP_097,
                                            EWARNING::WarningType::SUCCESS_MSG,
                                            STR_CPP_097);
        }
    }
    else{
        if(type == (int)EWARNING::WarningType::EXCEPTION_MSG && nunchuk::HWIException::DEVICE_CONN_ERROR == code){
            int state_id = maps["state_id"].toInt();
            startScanDevices(state_id);
            AppModel::instance()->showToast(code,
                                            what,
                                            (EWARNING::WarningType)type,
                                            STR_CPP_098);
        }
        else{
            AppModel::instance()->showToast(code,
                                            what,
                                            (EWARNING::WarningType)type,
                                            STR_CPP_070.arg(AppModel::instance()->masterSignerInfo()->fingerPrint().toUpper()));

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
    }
    else{
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_061);
    }
    emit finishedHealthCheckRemoteSigner();
}

void Controller::slotFinishDisplayAddress(bool result)
{
    emit finishedDisplayAddress(result);
}

void Controller::slotFinishRescanBlockchain()
{
    DBG_INFO;
}

void Controller::slotFinishCreateSoftwareSigner(const QMasterSignerPtr ret,
                                                QString what,
                                                int type,
                                                int code)
{
    if(ret && type != (int)EWARNING::WarningType::EXCEPTION_MSG){
        QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
        if(mastersigners){
            AppModel::instance()->setMasterSignerList(mastersigners);
            QMasterSignerPtr newsigner =  AppModel::instance()->masterSignerList()->getMasterSignerByXfp(ret.data()->fingerPrint());
            AppModel::instance()->setMasterSignerInfo(newsigner);
        }

        switch (QQuickViewer::instance()->currentFlow()) {
        case (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY:
        {
            uint last = QQuickViewer::instance()->getCurrentStates().last();
            if(last == (int)E::STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION){
                AppModel::instance()->setToast(0,
                                                STR_CPP_102.arg(ret.data()->name()),
                                                EWARNING::WarningType::SUCCESS_MSG,
                                                STR_CPP_102.arg(ret.data()->name()));
            }
            else if(last == (int)E::STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY){
                QQuickViewer::instance()->sendEvent(E::EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED);
            }else if(last == (int)E::STATE_ID_SCR_UNLOCK_DB){
                QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
                AppModel::instance()->makeMatrixInstanceForAccount();
                QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
                if(pKey){
                    timeoutHandler(3000,[pKey](){
                        AppModel::instance()->showToast(0,
                                                       STR_CPP_108.arg(pKey->name()),
                                                       EWARNING::WarningType::SUCCESS_MSG,
                                                       STR_CPP_108.arg(pKey->name()));
                        QWarningMessage msg;
                        bridge::nunchukClearSignerPassphrase(pKey->fingerPrint(),msg);
                    });
                }
            }
            break;
        }
        case (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY:
        {
            QQuickViewer::instance()->sendEvent(E::EVT_ADD_SOFTWARE_SIGNER_RESULT);
            AppModel::instance()->setPrimaryKey(ClientController::instance()->getMe().username);
            break;
        }
        case (int)ENUNCHUCK::IN_FLOW::FLOW_ADD_SIGNER:
        case (int)ENUNCHUCK::IN_FLOW::FLOW_ADD_WALLET:
        {
            QQuickViewer::instance()->sendEvent(E::EVT_ADD_SOFTWARE_SIGNER_RESULT);
            AppModel::instance()->setToast(0,
                                            STR_CPP_062,
                                            EWARNING::WarningType::SUCCESS_MSG,
                                            STR_CPP_062);
            break;
        }
        default:
            QQuickViewer::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_NONE);
            break;
        }
    }
    else{
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_063);
    }
}

void Controller::slotFinishCreateWallet(nunchuk::Wallet ret,
                                        QString what,
                                        int type,
                                        int code)
{
    if(type != (int)EWARNING::WarningType::EXCEPTION_MSG){
        QSingleSignerListModelPtr signersAssinged( new SingleSignerListModel);
        for (nunchuk::SingleSigner signer : ret.get_signers()) {
            QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
            signersAssinged.data()->addSingleSigner(ret);
        }
        QWalletPtr wallet_result = QWalletPtr(new Wallet(QString::fromStdString(ret.get_id()),
                                                         ret.get_m(),
                                                         ret.get_n(),
                                                         QString::fromStdString(ret.get_name()),
                                                         QString::number((int)ret.get_address_type()),
                                                         ret.get_unconfirmed_balance(),
                                                         QDateTime::fromTime_t(ret.get_create_date()),
                                                         ret.is_escrow(),
                                                         signersAssinged,
                                                         QString::fromStdString(ret.get_description())));
        QString wallet_id = QString::fromStdString(ret.get_id());
        AppModel::instance()->walletList()->addWallet(wallet_result);
        AppModel::instance()->resetSignersChecked();
        AppModel::instance()->walletList()->requestSort(WalletListModel::WalletRoles::wallet_Name_Role, Qt::AscendingOrder);
        int index = AppModel::instance()->walletList()->getWalletIndexById(wallet_id);
        if(-1 != index){
            AppModel::instance()->setWalletListCurrentIndex(index);
        }
        QQuickViewer::instance()->sendEvent(E::EVT_ADD_WALLET_SUCCESSFULLY);
        AppModel::instance()->showToast(0,
                                        STR_CPP_064,
                                        EWARNING::WarningType::SUCCESS_MSG,
                                        STR_CPP_064);
    }
    else{
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_065);
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
    if(AppModel::instance()->walletList()){
        AppModel::instance()->walletList()->updateBalance(id, balance);
        AppModel::instance()->startGetUsedAddresses(id);
        AppModel::instance()->startGetUnusedAddresses(id);
    }
}

void Controller::slotFinishTransactionChanged(const QString &tx_id,
                                              const int status,
                                              const QString &wallet_id,
                                              nunchuk::Transaction tx)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    QTransactionPtr trans = bridge::convertTransaction(tx, wallet_id);
    if(AppModel::instance()->walletList()){
        QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        if(wallet){
            wallet.data()->updateTransaction(tx_id, trans);
            if(wallet.data()->isAssistedWallet()){
                bridge::assistedWalletUpdateTx(wallet_id, tx);
            }
        }
    }
    if(AppModel::instance()->transactionInfo()){
        if(0 == QString::compare(tx_id, AppModel::instance()->transactionInfo()->txid(), Qt::CaseInsensitive)){
            AppModel::instance()->setTransactionInfo(trans);
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
    qApp->setOverrideCursor(Qt::WaitCursor);
    QWalletPtr wallet = NULL;
    if(AppModel::instance()->walletInfo()){
        if(0 == QString::compare(AppModel::instance()->walletInfo()->id(), wallet_id, Qt::CaseInsensitive)){
            wallet = AppModel::instance()->walletInfoPtr();
        }
    }
    else{
        if(AppModel::instance()->walletList()){
            wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        }
    }
    if(wallet){
        QTransactionListModelPtr trans_ret = QTransactionListModelPtr(new TransactionListModel);
        for (auto it = ret.begin(); it != ret.end(); ++it) {
            const nunchuk::Transaction &element = *it;
            QTransactionPtr tx = bridge::convertTransaction(element, wallet_id);
            trans_ret.data()->addTransaction(tx);
        }
        wallet.data()->setTransactionHistory(trans_ret);
    }
    qApp->restoreOverrideCursor();
    emit finishedGetTransactionHistory();
}

void Controller::slotCheckAndUnlockDevice(const int state_id)
{
    if(AppModel::instance()->deviceList()){
        for (int i = 0; i < AppModel::instance()->deviceList()->fullList().count(); i++) {
            QDevicePtr selectedDv = AppModel::instance()->deviceList()->fullList()[i];
            if(selectedDv){
                if(selectedDv.data()->needsPinSent()){
                    QWarningMessage msgwarning;
                    bridge::nunchukPromtPinOnDevice(selectedDv, msgwarning);
                    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                        QMap<QString, QVariant> pinData;
                        pinData["state_id"] = state_id;
                        pinData["device_idx"] = i;
                        QQuickViewer::instance()->sendEvent(E::EVT_ROOT_PROMT_PIN, pinData);
                    }
                    else{
                        AppModel::instance()->showToast(msgwarning.code(),
                                                        msgwarning.what(),
                                                        (EWARNING::WarningType)msgwarning.type(),
                                                        STR_CPP_066);
                    }
                }
                else if(selectedDv.data()->needsPassPhraseSent()){
                    QMap<QString, QVariant> passPhraseData;
                    passPhraseData["state_id"] = state_id;
                    passPhraseData["device_idx"] = i;
                    passPhraseData["is_software"] = false;
                    QQuickViewer::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
                }
                else{}
            }
            else{ continue;}
        }
        AppModel::instance()->checkDeviceUsableToSign();
        AppModel::instance()->checkDeviceUsableToAdd();
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
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_080);
    }
    QQuickViewer::instance()->sendEvent(E::EVT_INPUT_PIN_CLOSE);
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
        AppModel::instance()->showToast(code,
                                        what,
                                        (EWARNING::WarningType)type,
                                        STR_CPP_080);
    }
    QQuickViewer::instance()->sendEvent(E::EVT_INPUT_PASSPHRASE_CLOSE);
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
    QMasterSignerListModelPtr ret = bridge::nunchukConvertMasterSigners(masterSigners);
    if(ret){
        AppModel::instance()->setMasterSignerList(ret);
    }
}

void Controller::slotFinishReloadRemoteSigners(std::vector<nunchuk::SingleSigner> remoteSigners)
{
    QSingleSignerListModelPtr ret = bridge::nunchukConvertRemoteSigners(remoteSigners);
    if(ret){
        AppModel::instance()->setRemoteSignerList(ret);
    }
}

void Controller::slotFinishReloadWallets(std::vector<nunchuk::Wallet> wallets)
{
    QWalletListModelPtr ret = bridge::nunchukConvertWallets(wallets);
    if(ret){
        AppModel::instance()->setWalletList(ret);
        if(ret->rowCount() > 0){
            QString lastWalletId = bridge::nunchukGetSelectedWallet();
            int lastIndex = -1;
            if(lastWalletId != ""){
                lastIndex = AppModel::instance()->walletList()->getWalletIndexById(lastWalletId);
            }
            AppModel::instance()->setWalletListCurrentIndex(lastIndex);
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

void Controller::slotFinishSyncWalletDb(const QString &wallet_id)
{

}
