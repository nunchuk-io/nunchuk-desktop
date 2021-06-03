#include "Worker.h"
#include "bridgeifaces.h"
#include "ViewsEnums.h"
#include "QQuickViewer.h"

Worker::Worker()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Worker::~Worker()
{
    this->disconnect();
}

void Worker::slotStartCreateMasterSigner(const QString &id, const int deviceIndex)
{
    DBG_INFO << id << deviceIndex;
    QWarningMessage msg;
    AppModel::instance()->setAddSignerStep(0);
    AppModel::instance()->setAddSignerPercentage(0);
    QSharedPointer<MasterSigner> ret = bridge::nunchukCreateMasterSigner(id, deviceIndex, msg);
    AppModel::instance()->setAddSignerPercentage(100);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && ret){
        if(ret.data()->health() == (int)nunchuk::HealthStatus::SUCCESS){
            ret.data()->warningMessage()->setWarningMessage(0, "The Signer has been added successfully!", EWARNING::WarningType::SUCCESS_MSG, "");
        }
        else{
            ret.data()->warningMessage()->setWarningMessage(0, "Something went wrong when adding your signer device.", EWARNING::WarningType::ERROR_MSG, "");
        }
        DBG_INFO << "CREATE MASTER SIGNER DONE";
        emit finishCreateMasterSigner(ret);
    }
    else{
        QSharedPointer<MasterSigner> fail =  QSharedPointer<MasterSigner>(new MasterSigner);
        fail.data()->setId("-1");
        fail.data()->setName("FAIL");
        fail.data()->setHealth(-1);
        fail.data()->warningMessage()->setWarningMessage(msg.code(), msg.what(), (EWARNING::WarningType)msg.type(), "Can not create signer");
        DBG_INFO << "CREATE MASTER SIGNER FAIL";
        emit finishCreateMasterSigner(fail);
    }
}

void Worker::slotStartScanDevices(const QString &fingerprint) {
    QSharedPointer<DeviceListModel> deviceList = bridge::nunchukGetDevices();
    emit finishScanDevices(deviceList, fingerprint);
}

void Worker::slotStartCacheMasterSignerXPub(const QString &mastersigner_id)
{
    DBG_INFO << mastersigner_id;
    AppModel::instance()->setCacheXpubsPercentage(0);
    bridge::nunchukCacheMasterSignerXPub(mastersigner_id);
    AppModel::instance()->setCacheXpubsPercentage(100);
    emit finishCacheMasterSignerXPub(mastersigner_id);
}

void Worker::slotStartSigningTransaction(const QString &walletId, const QString &txid, const int deviceIndex, bool isSoftware)
{
    QSharedPointer<Device> selectedDv = NULL;
    if(isSoftware){
        if(NULL != AppModel::instance()->softwareSignerDeviceList()){
            selectedDv = AppModel::instance()->softwareSignerDeviceList()->getDeviceByIndex(deviceIndex) ;
        }
    }
    else{
        if(NULL != AppModel::instance()->deviceList()){
            QString devicePathSelected = "";
            devicePathSelected = AppModel::instance()->deviceList()->getDevicePathByIndex(deviceIndex);
            QSharedPointer<DeviceListModel> devices = bridge::nunchukGetDevices();
            if(devices){
                selectedDv = AppModel::instance()->deviceList()->getDeviceByPath(devicePathSelected) ;
            }
        }
    }
    QWarningMessage msgwarning;
    if(selectedDv){
        nunchuk::Transaction trans = bridge::nunchukSignTransactionThread(walletId,
                                                                          txid,
                                                                          selectedDv,
                                                                          msgwarning);
        emit finishSigningTransaction(trans, msgwarning.what(), msgwarning.type(), msgwarning.code(), selectedDv.data()->masterSignerId() , isSoftware);
    }
    else{
        msgwarning.setWarningMessage(0, "Something went wrong when trying get your device.", EWARNING::WarningType::ERROR_MSG, "Device is NULL");
        emit finishSigningTransaction(nunchuk::Transaction(), msgwarning.what(), msgwarning.type(), msgwarning.code(), "-1", selectedDv);
    }
}

void Worker::slotStartHealthCheckMasterSigner(const QString &signerId, const QString &message)
{
    QString out_signature = "";
    QString out_path = "";
    QWarningMessage msgwarning;
    int health = (int)bridge::nunchukHealthCheckMasterSigner(signerId, message, out_signature, out_path, msgwarning);
    emit finishHealthCheckMasterSigner(health, out_signature, signerId, msgwarning.what(), msgwarning.type(), msgwarning.code());
}

void Worker::slotStartDisplayAddress(const QString &wallet_id, const QString &address, const QString &device_fingerprint)
{
    DBG_INFO << wallet_id << address << device_fingerprint;
    QSharedPointer<DeviceListModel> deviceList = bridge::nunchukGetDevices();

    bool retPreCheck = false;
    if(deviceList && AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->singleSignersAssigned()){
        QStringList xfp = deviceList->getXFPList();
        retPreCheck = AppModel::instance()->walletInfo()->singleSignersAssigned()->containsDevicesConnected(xfp);
    }
    emit AppModel::instance()->preCheckAddressOnDevice(retPreCheck);
    if(retPreCheck){
        bool ret = bridge::nunchukDisplayAddressOnDevice(wallet_id, address, device_fingerprint);
        emit finishDisplayAddress(ret);
    }
}

void Worker::slotStartRescanBlockchain(int start, int stop)
{
    DBG_INFO << start << stop;
    bridge::nunchukRescanBlockchain(start, stop); // Default stop = -1
    emit finishRescanBlockchain();
}

void Worker::slotStartCreateSoftwareSigner(const QString name, const QString mnemonic, const QString passphrase)
{
    DBG_INFO;
    QWarningMessage msg;
    AppModel::instance()->setAddSignerPercentage(0);
    QSharedPointer<MasterSigner> ret = bridge::nunchukCreateSoftwareSigner(name, mnemonic, passphrase, msg);
    AppModel::instance()->setAddSignerPercentage(100);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && ret){
        if(ret.data()->health() == (int)nunchuk::HealthStatus::SUCCESS){
            ret.data()->warningMessage()->setWarningMessage(0, "The Signer has been added successfully!", EWARNING::WarningType::SUCCESS_MSG, "");
        }
        else{
            ret.data()->warningMessage()->setWarningMessage(0, "Something went wrong when adding your software signer.", EWARNING::WarningType::ERROR_MSG, "");
        }
        DBG_INFO << "CREATE MASTER SIGNER DONE";
        emit finishCreateSoftwareSigner(ret);
    }
    else{
        QSharedPointer<MasterSigner> fail =  QSharedPointer<MasterSigner>(new MasterSigner);
        fail.data()->setId("-1");
        fail.data()->setName("FAIL");
        fail.data()->setHealth(-1);
        fail.data()->warningMessage()->setWarningMessage(msg.code(), msg.what(), (EWARNING::WarningType)msg.type(), "Can not create signer");
        DBG_INFO << "CREATE MASTER SIGNER FAIL";
        emit finishCreateSoftwareSigner(fail);
    }
}

void Worker::slotStartBalanceChanged(const QString &id, const int value)
{
    DBG_INFO << id << value;
    emit finishBalanceChanged(id, value);
}

void Worker::slotStartTransactionChanged(const QString &tx_id, const int status)
{
    DBG_INFO << tx_id << status;
    emit finishTransactionChanged(tx_id, status);
}

void Worker::slotStartBlockChanged(const int height, const QString &hex_header)
{
    DBG_INFO << height << hex_header;
    emit finishBlockChanged(height, hex_header);
}

Controller::Controller() {
    qRegisterMetaType<QSharedPointer<MasterSigner>>();
    qRegisterMetaType<nunchuk::Transaction>();

    Worker *worker = new Worker;
    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    // Create master signer
    connect(this, &Controller::startCreateMasterSigner, worker, &Worker::slotStartCreateMasterSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateMasterSigner, this, &Controller::slotFinishCreateMasterSigner, Qt::QueuedConnection);

    // Get devices
    connect(this, &Controller::startScanDevices, worker, &Worker::slotStartScanDevices, Qt::QueuedConnection);
    connect(worker, &Worker::finishScanDevices, this, &Controller::slotFinishScanDevices, Qt::QueuedConnection);

    // Cached xpubs
    connect(this, &Controller::startCacheMasterSignerXPub, worker, &Worker::slotStartCacheMasterSignerXPub, Qt::QueuedConnection);
    connect(worker, &Worker::finishCacheMasterSignerXPub, this, &Controller::slotFinishCacheMasterSignerXPub, Qt::QueuedConnection);

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

    // Display address
    connect(this, &Controller::startDisplayAddress, worker, &Worker::slotStartDisplayAddress, Qt::QueuedConnection);
    connect(worker, &Worker::finishDisplayAddress, this, &Controller::slotFinishDisplayAddress, Qt::QueuedConnection);

    // Display address
    connect(this, &Controller::startRescanBlockchain, worker, &Worker::slotStartRescanBlockchain, Qt::QueuedConnection);
    connect(worker, &Worker::finishRescanBlockchain, this, &Controller::slotFinishRescanBlockchain, Qt::QueuedConnection);

    // Create master signer
    connect(this, &Controller::startCreateSoftwareSigner, worker, &Worker::slotStartCreateSoftwareSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateSoftwareSigner, this, &Controller::slotFinishCreateSoftwareSigner, Qt::QueuedConnection);

    workerThread.start();
}

Controller::~Controller() {
    if(workerThread.isRunning()){
        workerThread.quit();
        workerThread.wait();
    }
    this->disconnect();
}

void Controller::slotFinishCreateMasterSigner(const QSharedPointer<MasterSigner> ret)
{
    QSharedPointer<MasterSigner> rettmp =  QSharedPointer<MasterSigner>(new MasterSigner);
    if(ret && ret.data()->warningMessage()->type() != (int)EWARNING::WarningType::EXCEPTION_MSG){
        rettmp.data()->setId(ret.data()->id());
        rettmp.data()->setName(ret.data()->name());
        QSharedPointer<Device> dv = QSharedPointer<Device>(new Device());
        DBG_INFO << dv.data()->masterFingerPrint();
        if(ret.data()->device()){
            dv.data()->setName(ret.data()->device()->name());
            dv.data()->setType(ret.data()->device()->type());
            dv.data()->setPath(ret.data()->device()->path());
            dv.data()->setModel(ret.data()->device()->model());
            dv.data()->setMasterFingerPrint(ret.data()->device()->masterFingerPrint());
            dv.data()->setConnected(ret.data()->device()->connected());
            dv.data()->setNeedsPassPhraseSent(ret.data()->device()->needsPassPhraseSent());
            dv.data()->setNeedsPinSent(ret.data()->device()->needsPinSent());
        }
        rettmp.data()->setDevice(dv);
        rettmp.data()->setHealth(ret.data()->health());
        rettmp.data()->setPath(ret.data()->path());
        rettmp.data()->warningMessage()->setWarningMessage(ret.data()->warningMessage()->code(),
                                                           ret.data()->warningMessage()->what(),
                                                           (EWARNING::WarningType)ret.data()->warningMessage()->type(),
                                                           ret.data()->warningMessage()->explaination());

        DBG_INFO << "REQUEST CACHED XPUBS";
        startCacheMasterSignerXPub(rettmp.data()->id());
        AppModel::instance()->setMasterSignerInfo(rettmp);
        DBG_INFO << rettmp.data()->device();
        DBG_INFO << rettmp.data()->name();
    }
    else{
        DBG_INFO;
        rettmp.data()->warningMessage()->setWarningMessage(ret.data()->warningMessage()->code(),
                                                           ret.data()->warningMessage()->what(),
                                                           (EWARNING::WarningType)ret.data()->warningMessage()->type(),
                                                           ret.data()->warningMessage()->explaination());
        AppModel::instance()->setMasterSignerInfo(rettmp);
    }
}

void Controller::slotFinishScanDevices(QSharedPointer<DeviceListModel> ret, const QString &fingerprint) {
    DBG_INFO;
    QSharedPointer<DeviceListModel> rettmp = QSharedPointer<DeviceListModel>(new DeviceListModel());
    if(ret->rowCount() > 0){
        bool need_promt_pin = false;
        QSharedPointer<Device> dev_need_promt_pin;
        for (int var = 0; var < ret.data()->rowCount(); var++) {
            QSharedPointer<Device> it = ret.data()->getDeviceByIndex(var);
            QString signername = "";
            rettmp.data()->addDevice(it.data()->name(),
                                     it.data()->type(),
                                     it.data()->path(),
                                     it.data()->model(),
                                     it.data()->masterFingerPrint(),
                                     it.data()->connected(),
                                     it.data()->needsPassPhraseSent(),
                                     it.data()->needsPinSent());
            if((fingerprint == "") && it.data()->needsPinSent()){
                need_promt_pin = true;
                dev_need_promt_pin = it;
            }
        }

        if(need_promt_pin && (fingerprint == "")){
            DBG_INFO << "need_promt_pin:" << need_promt_pin;
            QWarningMessage msgwarning;
            bridge::nunchukPromtPinOnDevice(dev_need_promt_pin, msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                QQuickViewer::instance()->sendEvent(E::EVT_ROOT_PROMT_PIN);
            }
        }
        need_promt_pin = false;
    }
    else {
        DBG_INFO << "SCAN DEVICE IS FALSE";
    }
    AppModel::instance()->deviceList()->updateDeviceList(rettmp);
    AppModel::instance()->deviceList()->warningMessage()->setWarningMessage(ret.data()->warningMessage()->code(),
                                                                            ret.data()->warningMessage()->what(),
                                                                            (EWARNING::WarningType)ret.data()->warningMessage()->type(),
                                                                            ret.data()->warningMessage()->explaination());
    // Update usable
    AppModel::instance()->checkDeviceUsableToSign();
    AppModel::instance()->checkDeviceUsableToAdd();

    emit finishedScanDevices();
}

void Controller::slotFinishCacheMasterSignerXPub(const QString &mastersigner_id)
{
    DBG_INFO << mastersigner_id;
}

void Controller::slotFinishSigningTransaction(nunchuk::Transaction result, QString what, int type, int code, QString masterSignerId, bool isSoftware)
{
    if(type == (int)EWARNING::WarningType::NONE_MSG){
        QSharedPointer<Transaction> trans = QSharedPointer<Transaction>(bridge::convertTransaction(result));
        AppModel::instance()->setTransactionInfo(trans);
        if(AppModel::instance()->transactionHistory()){
            AppModel::instance()->transactionHistory()->replaceTransaction(trans.data()->txid(), trans);
        }
        if(AppModel::instance()->transactionHistoryShort()){
            AppModel::instance()->transactionHistoryShort()->replaceTransaction(trans.data()->txid(), trans);
        }
        if(isSoftware) {
            QWarningMessage warningmsg;
            bridge::nunchukClearSignerPassphrase(masterSignerId, warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                QSharedPointer<MasterSignerListModel> mastersigners = bridge::nunchukGetMasterSigners();
                if(mastersigners){
                    AppModel::instance()->setMasterSignerList(mastersigners);
                }
            }
        }

        // Update usable
        AppModel::instance()->checkDeviceUsableToSign();
    }
    else{
        AppModel::instance()->transactionInfo()->warningMessage()->setWarningMessage(code,what,(EWARNING::WarningType)type, "Cannot sign transaction");
    }
    emit finishedSigningTransaction();
}

void Controller::slotFinishHealthCheckMasterSigner(const int status, const QString &signature, const QString &id, QString what, int type, int code)
{
    DBG_INFO << "Health check: " << id << status;
    if((int)EWARNING::WarningType::EXCEPTION_MSG == type){
        AppModel::instance()->masterSignerInfo()->warningMessage()->setWarningMessage(code, what, (EWARNING::WarningType)type, "Can not health check signer");
    }
    else{
        AppModel::instance()->masterSignerInfo()->setHealth(status);
        AppModel::instance()->masterSignerInfo()->setSignature(signature);
        if(AppModel::instance()->walletList())
        {
            AppModel::instance()->walletList()->updateSignerHealthStatus(id, status, QDateTime::currentDateTime().toTime_t());
        }
        QWarningMessage msg;
        bridge::nunchukClearSignerPassphrase(id, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            QSharedPointer<MasterSignerListModel> mastersigners = bridge::nunchukGetMasterSigners();
            if(mastersigners){
                AppModel::instance()->setMasterSignerList(mastersigners);
            }
            QSharedPointer<MasterSigner> currentMastersigner = mastersigners.data()->getMasterSignerById(AppModel::instance()->masterSignerInfo()->id());
            AppModel::instance()->masterSignerInfo()->setNeedPassphraseSent(currentMastersigner.data()->needPassphraseSent());
        }
    }
    emit finishedHealthCheckMasterSigner();
}

void Controller::slotFinishDisplayAddress(bool result)
{
    emit finishedDisplayAddress(result);
}

void Controller::slotFinishRescanBlockchain()
{
    DBG_INFO;
}

void Controller::slotFinishCreateSoftwareSigner(const QSharedPointer<MasterSigner> ret)
{
    QSharedPointer<MasterSigner> rettmp =  QSharedPointer<MasterSigner>(new MasterSigner);
    if(ret && ret.data()->warningMessage()->type() != (int)EWARNING::WarningType::EXCEPTION_MSG){
        rettmp.data()->setId(ret.data()->id());
        rettmp.data()->setName(ret.data()->name());
        QSharedPointer<Device> dv = QSharedPointer<Device>(new Device());
        DBG_INFO << dv.data()->masterFingerPrint();
        if(ret.data()->device()){
            dv.data()->setName(ret.data()->device()->name());
            dv.data()->setType(ret.data()->device()->type());
            dv.data()->setPath(ret.data()->device()->path());
            dv.data()->setModel(ret.data()->device()->model());
            dv.data()->setMasterFingerPrint(ret.data()->device()->masterFingerPrint());
            dv.data()->setConnected(/*ret.data()->device()->connected()*/ false);
            dv.data()->setNeedsPassPhraseSent(ret.data()->device()->needsPassPhraseSent());
            dv.data()->setNeedsPinSent(ret.data()->device()->needsPinSent());
        }
        rettmp.data()->setDevice(dv);
        rettmp.data()->setIsSoftwareSigner(true);
        rettmp.data()->setHealth(ret.data()->health());
        rettmp.data()->setPath(ret.data()->path());
        rettmp.data()->warningMessage()->setWarningMessage(ret.data()->warningMessage()->code(),
                                                           ret.data()->warningMessage()->what(),
                                                           (EWARNING::WarningType)ret.data()->warningMessage()->type(),
                                                           ret.data()->warningMessage()->explaination());
        AppModel::instance()->setMasterSignerInfo(rettmp);
    }
    else{
        DBG_INFO;
        rettmp.data()->warningMessage()->setWarningMessage(ret.data()->warningMessage()->code(),
                                                           ret.data()->warningMessage()->what(),
                                                           (EWARNING::WarningType)ret.data()->warningMessage()->type(),
                                                           ret.data()->warningMessage()->explaination());
        AppModel::instance()->setMasterSignerInfo(rettmp);
    }
}

void Controller::slotFinishBalanceChanged(const QString &id, const int value)
{
    DBG_INFO << id << value;
    if(AppModel::instance()->walletList()){
        AppModel::instance()->walletList()->updateBalance(id, value);
        AppModel::instance()->walletList()->updateAddress(id,
                                                          bridge::nunchukGetUsedAddresses(id, false),
                                                          bridge::nunchukGetUnusedAddresses(id, false));
    }
}

void Controller::slotFinishTransactionChanged(const QString &tx_id, const int status)
{
    DBG_INFO << tx_id << status;
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->setUsedAddressList(bridge::nunchukGetUsedAddresses(AppModel::instance()->walletInfo()->id(), false));
        AppModel::instance()->walletInfo()->setunUsedAddressList(bridge::nunchukGetUnusedAddresses(AppModel::instance()->walletInfo()->id(), false));
        QSharedPointer<Transaction> it = bridge::nunchukGetTransaction(AppModel::instance()->walletInfo()->id(), tx_id);
        if(it && AppModel::instance()->transactionHistory()){
            AppModel::instance()->transactionHistory()->replaceTransaction(tx_id, it);
        }
        if(it && AppModel::instance()->transactionHistoryShort()){
            AppModel::instance()->transactionHistoryShort()->replaceTransaction(tx_id, it);
        }
    }
}

void Controller::slotFinishBlockChanged(const int height, const QString &hex_header)
{
    DBG_INFO << height << hex_header;
    AppModel::instance()->setChainTip(height);
}
