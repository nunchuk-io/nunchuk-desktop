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
#include "bridgeTransaction.h"
#include "Chats/matrixbrigde.h"
#include "ViewsEnums.h"
#include "QEventProcessor.h"
#include "WorkerThread.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "features/transactions/usecases/FetchCancelTransactionUseCase.h"
#include "features/transactions/usecases/FetchTransactionListUseCase.h"
#include "localization/STR_CPP.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "ServiceSetting.h"
#include "AppSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Signers/QSignerManagement.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iterator>
#include <memory>
#include <mutex>
#include <QPointer>
#include <type_traits>
#include <utility>

namespace {

std::atomic_bool mainThreadDispatchEnabled{true};
std::mutex mainThreadDispatchMutex;

enum class MainThreadCallStatus {
    Pending,
    Running,
    Done,
    Cancelled,
};

struct MainThreadCallState {
    std::mutex mutex;
    std::condition_variable completed;
    MainThreadCallStatus status{MainThreadCallStatus::Pending};
};

WorkerWarningData warningData(const QWarningMessage &warning)
{
    return {warning.what(), warning.type(), warning.code()};
}

std::vector<nunchuk::Device> uniqueDevices(const std::vector<nunchuk::Device> &devices)
{
    std::vector<nunchuk::Device> result;
    result.reserve(devices.size());
    for (const auto &device : devices) {
        const QString fingerprint = QString::fromStdString(device.get_master_fingerprint());
        const auto duplicate = std::find_if(result.cbegin(), result.cend(), [&fingerprint](const nunchuk::Device &existing) {
            return qUtils::strCompare(fingerprint,
                                      QString::fromStdString(existing.get_master_fingerprint()));
        });
        if (duplicate == result.cend()) {
            result.push_back(device);
        }
    }
    return result;
}

template <typename Function>
bool runOnMainThreadBlocking(Function &&function)
{
    std::unique_lock<std::mutex> dispatchLock(mainThreadDispatchMutex);
    auto *application = qApp;
    if (!application ||
        QCoreApplication::closingDown() ||
        !mainThreadDispatchEnabled.load(std::memory_order_acquire)) {
        return false;
    }

    auto *applicationThread = application->thread();
    if (!applicationThread || !applicationThread->isRunning()) {
        return false;
    }
    if (QThread::currentThread() == applicationThread) {
        dispatchLock.unlock();
        function();
        return true;
    }

    using Task = std::decay_t<Function>;
    auto state = std::make_shared<MainThreadCallState>();
    auto task = std::make_shared<Task>(std::forward<Function>(function));
    const bool invoked = QMetaObject::invokeMethod(
        application,
        [state, task]() mutable {
            {
                std::lock_guard<std::mutex> lock(state->mutex);
                if (state->status == MainThreadCallStatus::Cancelled) {
                    return;
                }
                state->status = MainThreadCallStatus::Running;
            }

            (*task)();

            {
                std::lock_guard<std::mutex> lock(state->mutex);
                state->status = MainThreadCallStatus::Done;
            }
            state->completed.notify_all();
        },
        Qt::QueuedConnection);
    dispatchLock.unlock();
    if (!invoked) {
        return false;
    }

    std::unique_lock<std::mutex> lock(state->mutex);
    while (state->status == MainThreadCallStatus::Pending) {
        if (QCoreApplication::closingDown() ||
            !mainThreadDispatchEnabled.load(std::memory_order_acquire)) {
            state->status = MainThreadCallStatus::Cancelled;
            return false;
        }
        state->completed.wait_for(lock, std::chrono::milliseconds(10));
    }

    // Once the callback starts, its captures may refer to the caller's stack.
    // Keep that stack alive until the main-thread callback has completed.
    while (state->status == MainThreadCallStatus::Running) {
        state->completed.wait(lock);
    }
    return state->status == MainThreadCallStatus::Done;
}

bool completedTaprootRoundOne(const nunchuk::Transaction &before,
                              const nunchuk::Transaction &after)
{
    const auto &fromKeysetStatus = before.get_keyset_status();
    const auto &toKeysetStatus = after.get_keyset_status();
    if (fromKeysetStatus.size() != toKeysetStatus.size()) {
        return false;
    }

    for (size_t i = 0; i < fromKeysetStatus.size(); ++i) {
        if (fromKeysetStatus[i].first == nunchuk::TransactionStatus::PENDING_SIGNATURES &&
            toKeysetStatus[i].first == nunchuk::TransactionStatus::READY_TO_BROADCAST) {
            return false;
        }
    }
    for (size_t i = 0; i < fromKeysetStatus.size(); ++i) {
        if (fromKeysetStatus[i].first == nunchuk::TransactionStatus::PENDING_NONCE &&
            toKeysetStatus[i].first == nunchuk::TransactionStatus::PENDING_SIGNATURES) {
            return true;
        }
    }
    return false;
}

} // namespace

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

WorkerCreateMasterSignerResult Worker::createMasterSigner(const WorkerCreateMasterSignerData &data)
{
    QWarningMessage msg;
    std::vector<nunchuk::Device> devices;
    if (data.isSignedIn) {
        devices = qUtils::GetDevices(data.hwiPath, msg);
        if (msg.type() == (int)EWARNING::WarningType::NONE_MSG) {
            msg.resetWarningMessage();
            if (devices.empty()) {
                devices = bridge::nunchukGetOriginDevices(msg);
            }
        }
    } else {
        devices = bridge::nunchukGetOriginDevices(msg);
    }

    WorkerCreateMasterSignerResult result;
    if (msg.type() == (int)EWARNING::WarningType::NONE_MSG) {
        const auto deduplicatedDevices = uniqueDevices(devices);
        result.signerMessage = qUtils::QGenerateRandomMessage();
        emit updateNewKeySignMessage(result.signerMessage);

        const auto selected = std::find_if(deduplicatedDevices.cbegin(),
                                           deduplicatedDevices.cend(),
                                           [&data](const nunchuk::Device &device) {
            return qUtils::strCompare(data.xfp,
                                      QString::fromStdString(device.get_master_fingerprint()));
        });
        if (selected != deduplicatedDevices.cend()) {
            result.deviceIndex = static_cast<int>(std::distance(deduplicatedDevices.cbegin(), selected));
            emit updateAddSignerStep(1);
            nunchuk::Device device(selected->get_type(),
                                   selected->get_path(),
                                   selected->get_model(),
                                   selected->get_master_fingerprint(),
                                   selected->needs_pass_phrase_sent(),
                                   selected->needs_pin_sent());
            emit updateAddSignerStep(2);
            result.signer = bridge::nunchukCreateOriginMasterSigner(data.name, device, msg);
            result.hasSigner = msg.type() == (int)EWARNING::WarningType::NONE_MSG;
        } else {
            msg.setCode(-12);
            msg.setType((int)EWARNING::WarningType::EXCEPTION_MSG);
            msg.setWhat("Device not found, please check your device.");
        }
    }
    result.warning = warningData(msg);
    return result;
}

WorkerScanDevicesResult Worker::scanDevices(const WorkerScanDevicesData &data)
{
    QWarningMessage msg;
    WorkerScanDevicesResult result;
    result.stateId = data.stateId;
    result.isTopUpXpub = data.isTopUpXpub;
    if (data.isSignedIn) {
        result.devices = qUtils::GetDevices(data.hwiPath, msg);
    } else {
        result.devices = bridge::nunchukGetOriginDevices(msg);
    }

    if (msg.type() == (int)EWARNING::WarningType::NONE_MSG) {
        QString masterSignerId = data.masterSignerId;
        if (masterSignerId.isEmpty() && !data.masterFingerprint.isEmpty()) {
            msg.resetWarningMessage();
            WorkerCreateMasterSignerData createData;
            createData.name = data.signerName;
            createData.xfp = data.masterFingerprint;
            createData.isSignedIn = data.isSignedIn;
            createData.hwiPath = data.hwiPath;
            const auto createResult = createMasterSigner(createData);
            masterSignerId = createResult.hasSigner
                                 ? QString::fromStdString(createResult.signer.get_id())
                                 : data.masterFingerprint;
        }
        if (!masterSignerId.isEmpty()) {
            msg.resetWarningMessage();
            bridge::nunchukCacheMasterSignerXPub(masterSignerId, msg);
        }
    }
    result.warning = warningData(msg);
    return result;
}

void Worker::slotStartCreateMasterSigner(const WorkerCreateMasterSignerData &data)
{
    emit beginCreateMasterSigner();
    const auto result = createMasterSigner(data);
    emit completeAddSignerProgress();
    if (result.hasSigner && result.warning.type == (int)EWARNING::WarningType::NONE_MSG) {
        DBG_INFO << "CREATE MASTER SIGNER DONE";
        emit finishCreateMasterSigner(result);
        if (!runOnMainThreadBlocking([] {
                AppModel::instance()->requestCreateUserWallets();
                QGroupWallets::instance()->SyncGroupSignerFromDraftWalletInfo();
            })) {
            return;
        }

        QJsonObject userDraftWallet;
        QString errorMessage;
        const bool fetched = QUserWallets::FetchDraftWallet(userDraftWallet, errorMessage);
        emit applyUserDraftWallet(fetched, userDraftWallet);
    } else {
        DBG_INFO << "CREATE MASTER SIGNER FAIL"
                 << result.warning.what
                 << result.warning.type
                 << result.warning.code;
        emit createMasterSignerFailed(result.warning);
        WorkerScanDevicesData scanData;
        scanData.isSignedIn = data.isSignedIn;
        scanData.hwiPath = data.hwiPath;
        emit finishScanDevices(scanDevices(scanData));
        emit finishCreateMasterSigner(result);
    }
}

void Worker::slotStartCreateRemoteSigner(const WorkerCreateRemoteSignerData &data)
{
    QWarningMessage msg;
    WorkerCreateRemoteSignerResult result;
    result.event = data.event;
    result.replace = data.replace;
    result.signer = bridge::nunchukCreateOriginSigner(data.name,
                                                       data.xpub,
                                                       data.publicKey,
                                                       data.derivationPath,
                                                       data.masterFingerprint,
                                                       data.type,
                                                       data.tags,
                                                       data.replace,
                                                       msg);
    result.warning = warningData(msg);
    if (result.replace &&
        result.warning.type == (int)EWARNING::WarningType::NONE_MSG) {
        if (!runOnMainThreadBlocking([&result]() {
                result.syncReplacement = QEventProcessor::instance()->currentFlow() !=
                                         (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_USER_FREE;
            })) {
            return;
        }
        if (result.syncReplacement) {
            WalletKeys::SyncSignerToServer(result.signer);
        }
    }
    if (!runOnMainThreadBlocking([syncReplacement = result.syncReplacement]() {
            if (syncReplacement) {
                AppModel::instance()->startReloadWallets();
            }
            AppModel::instance()->setWalletsUsingSigner({});
        })) {
        return;
    }
    emit finishCreateRemoteSigner(result);
}

void Worker::slotStartScanDevices(const WorkerScanDevicesData &data)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
    emit scanDevicesStarted(data.isTopUpXpub);
    emit finishScanDevices(scanDevices(data));
}

void Worker::slotStartSigningTransaction(const QString &walletId,
                                         const QString &txid,
                                         const nunchuk::Device &device,
                                         const QString &masterSignerId,
                                         const QString &roomId,
                                         const QString &initEventId,
                                         const nunchuk::Transaction &transactionSnapshot,
                                         bool checkTaprootRoundOne,
                                         bool isSoftware)
{
    DBG_INFO << walletId << QString::fromStdString(device.get_master_fingerprint()) << isSoftware << txid;
    QWarningMessage msgwarning;
    nunchuk::Transaction transaction;

    if (!roomId.isEmpty()) {
        // Keep the shared-wallet flow unchanged: signing and the subsequent
        // transaction reload use their own warning, while the completion path
        // reports the same outer warning as before the thread-affinity split.
        QWarningMessage signwarning;
        matrixbrigde::SignTransaction(roomId, initEventId, device, signwarning);
        emit matrixSigningFinished();
        transaction = bridge::nunchukGetOriginTransaction(walletId, txid, signwarning);
    } else {
        transaction = bridge::transaction::SignTransaction(walletId, txid, device, msgwarning);
    }

    const bool showTaprootRoundOneToast = roomId.isEmpty() &&
                                          checkTaprootRoundOne &&
                                          completedTaprootRoundOne(transactionSnapshot, transaction);

    emit finishSigningTransaction(walletId,
                                  transaction,
                                  msgwarning.what(),
                                  msgwarning.type(),
                                  msgwarning.code(),
                                  masterSignerId,
                                  showTaprootRoundOneToast,
                                  isSoftware);
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

void Worker::slotStartHealthCheckRemoteSigner(const WorkerHealthCheckRemoteSignerData &data)
{
    DBG_INFO << data.stateId << data.xfp << data.signerType << data.message;
    if((int)ENUNCHUCK::SignerType::SOFTWARE == data.signerType
            || (int)ENUNCHUCK::SignerType::HARDWARE == data.signerType
            || (int)ENUNCHUCK::SignerType::COLDCARD_NFC == data.signerType)
    {
        QString out_signature = "";
        QString out_path = "";
        QWarningMessage msgwarning;
        int status = (int)bridge::nunchukHealthCheckMasterSigner(data.xfp, data.message, out_signature, out_path, msgwarning);
        emit finishHealthCheckRemoteSigner(status,
                                           msgwarning.what(),
                                           msgwarning.type(),
                                           msgwarning.code());
    }
    else{
        QWarningMessage msgwarning;
        int status = (int)nunchuk::HealthStatus::NO_SIGNATURE;
        if (data.hasSingleSigner) {
            status = (int)bridge::nunchukHealthCheckSingleSigner(data.singleSigner,
                                                                data.signerMessage,
                                                                data.signerSignature,
                                                                msgwarning);
        }
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
    emit displayAddressStarted();
    QWarningMessage msg;
    WorkerDisplayAddressResult result;
    result.result = bridge::nunchukDisplayAddressOnDevice(wallet_id,
                                                          address,
                                                          "", // check it all devices
                                                          msg);
    result.warning = warningData(msg);
    emit finishDisplayAddress(result);
}

void Worker::slotStartRescanBlockchain(int start,
                                       int stop)
{
    DBG_INFO << start << stop;
    bridge::nunchukRescanBlockchain(start, stop); // Default stop = -1
    emit finishRescanBlockchain();
}

void Worker::slotStartCreateSoftwareSigner(const WorkerCreateSoftwareSignerData &data)
{
    emit beginCreateSoftwareSigner();
    QWarningMessage msg;
    WorkerCreateSoftwareSignerResult result;
    result.signer = bridge::nunchukCreateOriginSoftwareSigner(data.name,
                                                               data.secret,
                                                               data.passphrase,
                                                               data.isPrimaryKey,
                                                               data.replace,
                                                               msg);
    result.warning = warningData(msg);
    emit completeAddSignerProgress();
    result.signerExists = msg.code() == (int)nunchuk::StorageException::SIGNER_EXISTS;
    if (result.signerExists) {
        result.signerExistsFingerprint = qUtils::GetMasterFingerprint(data.secret, data.passphrase);
    } else if (msg.type() == (int)EWARNING::WarningType::NONE_MSG && data.replace) {
        result.replacementSigner = nunchuk::SingleSigner(result.signer.get_name(),
                                                         "",
                                                         "",
                                                         "",
                                                         {},
                                                         result.signer.get_device().get_master_fingerprint(),
                                                         std::time(0),
                                                         result.signer.get_id(),
                                                         true,
                                                         nunchuk::SignerType::SOFTWARE,
                                                         result.signer.get_tags());
        result.hasReplacementSigner = true;
        WalletKeys::SyncSignerToServer(result.replacementSigner);
    } else if (msg.type() != (int)EWARNING::WarningType::NONE_MSG &&
               !result.signerExists) {
        result.signer = nunchuk::MasterSigner();
    }
    DBG_INFO << data.name << msg.type() << data.replace;
    emit finishCreateSoftwareSigner(result);
}

void Worker::slotStartCreateSoftwareSignerXprv(const WorkerCreateSoftwareSignerData &data)
{
    emit beginCreateSoftwareSigner();
    QWarningMessage msg;
    WorkerCreateSoftwareSignerResult result;
    result.signer = bridge::CreateSoftwareSignerFromMasterXprv(data.name,
                                                                data.secret,
                                                                data.isPrimaryKey,
                                                                data.replace,
                                                                msg);
    result.warning = warningData(msg);
    emit completeAddSignerProgress();
    result.signerExists = msg.code() == (int)nunchuk::StorageException::SIGNER_EXISTS;
    if (result.signerExists) {
        result.signerExistsFingerprint = QString::fromStdString(result.signer.get_device().get_master_fingerprint());
    } else if (msg.type() == (int)EWARNING::WarningType::NONE_MSG && data.replace) {
        result.replacementSigner = nunchuk::SingleSigner(result.signer.get_name(),
                                                         "",
                                                         "",
                                                         "",
                                                         {},
                                                         result.signer.get_device().get_master_fingerprint(),
                                                         std::time(0),
                                                         result.signer.get_id(),
                                                         true,
                                                         nunchuk::SignerType::SOFTWARE,
                                                         result.signer.get_tags());
        result.hasReplacementSigner = true;
        WalletKeys::SyncSignerToServer(result.replacementSigner);
    }
    emit finishCreateSoftwareSignerXprv(result);
}

void Worker::slotStartCreateWallet(const WorkerCreateWalletData &data)
{
    DBG_INFO << data.needBackup << data.filePath;
    DBG_INFO << "CREATE"
             << (int)data.addressType
             << (int)data.walletType
             << data.m
             << data.n;

    QWarningMessage msgWarning;
    nunchuk::Wallet ret = bridge::nunchukCreateOriginWallet(data.name,
                                                            data.m,
                                                            data.n,
                                                            data.signers,
                                                            data.addressType,
                                                            data.walletType,
                                                            data.description,
                                                            false /*allow used signer*/,
                                                            "" /*decoy pin*/,
                                                            data.walletTemplate,
                                                            msgWarning);

    DBG_INFO << "CREATE" << ret.get_name() << (int)ret.get_wallet_type() << (int)ret.get_address_type() << ret.get_m() << ret.get_n();

    if ((int)EWARNING::WarningType::NONE_MSG == msgWarning.type() &&
        data.needBackup) {
        std::string walletid = "";
        try {
            walletid = ret.get_id();
        }
        catch (const nunchuk::BaseException &ex) {
            DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        }
        catch (std::exception &e) {
            DBG_INFO << "THROW EXCEPTION" << e.what();
        }

        if (walletid.empty() ||
            !bridge::nunchukExportWallet(QString::fromStdString(walletid),
                                         data.filePath,
                                         nunchuk::ExportFormat::BSMS)) {
            msgWarning.setWarningMessage(
                nunchuk::NunchukException::INVALID_PARAMETER,
                "Wallet created, but the backup file could not be written.",
                EWARNING::WarningType::WARNING_MSG);
        }
    }
    emit finishCreateWallet(ret,
                            msgWarning.what(),
                            msgWarning.type(),
                            msgWarning.code());
}

void Worker::slotCreateMiniscriptWallet(const WorkerCreateMiniscriptWalletData &data)
{
    const QString decoyPin;
    DBG_INFO << "CREATE MINISCRIPT WALLET"
             << data.name
             << data.scriptTemplate
             << data.signers.size()
             << (int)data.addressType
             << data.description
             << data.allowUsedSigner
             << decoyPin;
    QWarningMessage msgWarning;
    nunchuk::Wallet ret = bridge::nunchukCreateOriginMiniscriptWallet(data.name,
                                                                      data.scriptTemplate,
                                                                      data.signers,
                                                                      data.addressType,
                                                                      data.description,
                                                                      data.allowUsedSigner,
                                                                      decoyPin,
                                                                      msgWarning);

    emit finishCreateMiniscriptWallet(ret,
                                      msgWarning.what(),
                                      msgWarning.type(),
                                      msgWarning.code());
}

void Worker::slotStartBalanceChanged(const QString &id,
                                     qint64 balance)
{
    QWarningMessage msg;
    const nunchuk::Wallet origin_wallet = bridge::nunchukGetOriginWallet(id, msg);
    const bool walletLoaded = (int)EWARNING::WarningType::NONE_MSG == msg.type();
    bool refreshTransactionHistory = false;
    if (walletLoaded) {
        if (!runOnMainThreadBlocking([id, origin_wallet, &refreshTransactionHistory]() {
                auto appModel = AppModel::instance();
                if (auto walletList = appModel->walletList()) {
                    walletList->updateNunchukWallet(id, origin_wallet);
                }

                auto currentWallet = appModel->walletInfo();
                refreshTransactionHistory = currentWallet &&
                                            qUtils::strCompare(id, currentWallet->walletId());
                if (refreshTransactionHistory) {
                    currentWallet->setNunchukWallet(origin_wallet);
                }
            })) {
            return;
        }
    }

    if (refreshTransactionHistory) {
        slotStartGetTransactionHistory(id);
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

void Worker::slotRequestGetEstimatedFee()
{
    int primaryServer = (int)AppSetting::Chain::MAIN;
    if (!runOnMainThreadBlocking([&primaryServer]() {
            primaryServer = AppSetting::instance()->primaryServer();
        })) {
        return;
    }
    int replyCode = -1;
    const QJsonObject feeRates = Draco::instance()->fetchFeeRates(primaryServer, replyCode);
    if (!runOnMainThreadBlocking([feeRates, replyCode]() {
            Draco::instance()->applyFeeRates(feeRates, replyCode);
        })) {
        return;
    }
}

void Worker::slotStartSendPinToDevice(const WorkerDeviceInputData &data)
{
    QWarningMessage msgwarning;
    if(data.shouldSend){
        bridge::nunchukSendPinToDevice(data.device, data.input, msgwarning);
    }
    emit finishSendPinToDevice(data.stateId,
                               msgwarning.what(),
                               msgwarning.code(),
                               (int)msgwarning.type());
}

void Worker::slotStartSendPassphraseToDevice(const WorkerDeviceInputData &data)
{
    QWarningMessage msgwarning;
    if(data.shouldSend){
        bridge::nunchukSendPassphraseToDevice(data.device, data.input, msgwarning);
    }
    emit finishSendPassphraseToDevice(data.stateId,
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

void Worker::slotStartMultiDeviceSync(const WorkerMultiDeviceSyncData &data)
{
    if(data.canSync){
        matrixbrigde::EnableAutoBackup(data.state);
        if(data.state){
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
    if (wallet_id.isEmpty()) {
        emit finishSyncWalletDb(wallet_id, {}, {}, {}, {});
        return;
    }

    QWarningMessage msg;
    const nunchuk::Wallet originWallet = bridge::nunchukGetOriginWallet(wallet_id, msg);
    emit prepareSyncWalletDb(wallet_id,
                             originWallet,
                             (int)EWARNING::WarningType::NONE_MSG == msg.type());

    const std::vector<nunchuk::Transaction> transactions = bridge::nunchukGetOriginTransactionHistory(wallet_id);
    emit finishGetTransactionHistory(wallet_id, transactions);

    const QStringList usedAddresses = bridge::nunchukGetUsedAddresses(wallet_id, false);
    const QStringList usedChangeAddresses = bridge::nunchukGetUsedAddresses(wallet_id, true);
    const QStringList unusedAddresses = bridge::nunchukGetUnusedAddresses(wallet_id, false);
    const QStringList unusedChangeAddresses = bridge::nunchukGetUnusedAddresses(wallet_id, true);

    emit finishSyncWalletDb(wallet_id,
                            usedAddresses,
                            usedChangeAddresses,
                            unusedAddresses,
                            unusedChangeAddresses);
}

Controller::Controller() {
    {
        std::lock_guard<std::mutex> lock(mainThreadDispatchMutex);
        mainThreadDispatchEnabled.store(true, std::memory_order_release);
    }
    if (qApp) {
        connect(qApp, &QCoreApplication::aboutToQuit, this, [] {
            std::lock_guard<std::mutex> lock(mainThreadDispatchMutex);
            mainThreadDispatchEnabled.store(false, std::memory_order_release);
        }, Qt::DirectConnection);
    }

    qRegisterMetaType<Conversation>();
    qRegisterMetaType<QMasterSignerPtr>();
    qRegisterMetaType<nunchuk::RoomTransaction>();
    qRegisterMetaType<WorkerWarningData>();
    qRegisterMetaType<WorkerScanDevicesData>();
    qRegisterMetaType<WorkerScanDevicesResult>();
    qRegisterMetaType<WorkerCreateMasterSignerData>();
    qRegisterMetaType<WorkerCreateMasterSignerResult>();
    qRegisterMetaType<WorkerCreateRemoteSignerData>();
    qRegisterMetaType<WorkerCreateRemoteSignerResult>();
    qRegisterMetaType<WorkerDisplayAddressResult>();
    qRegisterMetaType<WorkerCreateSoftwareSignerData>();
    qRegisterMetaType<WorkerCreateSoftwareSignerResult>();
    qRegisterMetaType<WorkerCreateWalletData>();
    qRegisterMetaType<WorkerCreateMiniscriptWalletData>();
    qRegisterMetaType<WorkerHealthCheckRemoteSignerData>();
    qRegisterMetaType<WorkerDeviceInputData>();
    qRegisterMetaType<WorkerMultiDeviceSyncData>();

    Worker *worker = Worker::create(&workerThread);

    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    // Create master signer
    connect(this, &Controller::startCreateMasterSigner, this, &Controller::slotPrepareCreateMasterSigner);
    connect(this, &Controller::startCreateMasterSignerPrepared, worker, &Worker::slotStartCreateMasterSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateMasterSigner, this, &Controller::slotFinishCreateMasterSigner, Qt::QueuedConnection);
    connect(worker, &Worker::beginCreateMasterSigner, this, [] {
        AppModel::instance()->setAddSignerStep(0);
        AppModel::instance()->setAddSignerPercentage(0);
    }, Qt::QueuedConnection);
    connect(worker, &Worker::beginCreateSoftwareSigner, this, [] {
        AppModel::instance()->setAddSignerPercentage(0);
    }, Qt::QueuedConnection);
    connect(worker, &Worker::updateNewKeySignMessage, this, [](const QString &message) {
        AppModel::instance()->setNewKeySignMessage(message);
    }, Qt::QueuedConnection);
    connect(worker, &Worker::updateAddSignerStep, this, [](int step) {
        AppModel::instance()->setAddSignerStep(step);
    }, Qt::QueuedConnection);
    connect(worker, &Worker::completeAddSignerProgress, this, [] {
        AppModel::instance()->setAddSignerPercentage(100);
        AppModel::instance()->setWalletsUsingSigner({});
    }, Qt::QueuedConnection);
    connect(worker, &Worker::createMasterSignerFailed, this, [](const WorkerWarningData &warning) {
        const int last = QEventProcessor::instance()->getCurrentStates().last();
        if (last == E::STATE_ID_SCR_ADD_HARDWARE) {
            AppModel::instance()->setAddSignerWizard(1);
        }
        AppModel::instance()->showToast(warning.code,
                                        warning.what,
                                        static_cast<EWARNING::WarningType>(warning.type));
    }, Qt::QueuedConnection);
    connect(worker, &Worker::applyUserDraftWallet, this, [](bool fetched, const QJsonObject &output) {
        QUserWallets::instance()->ApplyDraftWallet(fetched, output);
    }, Qt::QueuedConnection);

    // Create remote signer
    connect(this, &Controller::startCreateRemoteSigner, this, &Controller::slotPrepareCreateRemoteSigner);
    connect(this, &Controller::startCreateRemoteSignerPrepared, worker, &Worker::slotStartCreateRemoteSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateRemoteSigner, this, &Controller::slotFinishCreateRemoteSigner, Qt::QueuedConnection);

    // Get devices
    connect(this, &Controller::startScanDevices, this, &Controller::slotPrepareScanDevices);
    connect(this, &Controller::startScanDevicesPrepared, worker, &Worker::slotStartScanDevices, Qt::QueuedConnection);
    connect(worker, &Worker::finishScanDevices, this, &Controller::slotFinishScanDevices, Qt::QueuedConnection);
    connect(worker, &Worker::scanDevicesStarted, this, [](bool isTopUpXpub) {
        if (isTopUpXpub) {
            AppModel::instance()->startTopXPUBsSigner();
        }
    }, Qt::QueuedConnection);

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
    connect(this, &Controller::startSigningTransaction, this, &Controller::slotPrepareSigningTransaction);
    connect(this, &Controller::startSigningTransactionPrepared, worker, &Worker::slotStartSigningTransaction, Qt::QueuedConnection);
    connect(worker, &Worker::matrixSigningFinished, this, &Controller::finishedSigningTransaction, Qt::QueuedConnection);
    connect(worker, &Worker::finishSigningTransaction, this, &Controller::slotFinishSigningTransaction, Qt::QueuedConnection);

    // Health check master signer
    connect(this, &Controller::startHealthCheckMasterSigner, worker, &Worker::slotStartHealthCheckMasterSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishHealthCheckMasterSigner, this, &Controller::slotFinishHealthCheckMasterSigner, Qt::QueuedConnection);

    // Health check remote signer
    connect(this, &Controller::startHealthCheckRemoteSigner, this, &Controller::slotPrepareHealthCheckRemoteSigner);
    connect(this, &Controller::startHealthCheckRemoteSignerPrepared, worker, &Worker::slotStartHealthCheckRemoteSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishHealthCheckRemoteSigner, this, &Controller::slotFinishHealthCheckRemoteSigner, Qt::QueuedConnection);

    // get top up XPUBs
    connect(this, &Controller::startTopXPUBsMasterSigner, worker, &Worker::slotStartTopXPUBsMasterSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishTopXPUBsMasterSigner, this, &Controller::slotFinishTopXPUBsMasterSigner, Qt::QueuedConnection);

    // Display address
    connect(this, &Controller::startDisplayAddress, this, &Controller::slotPrepareDisplayAddress);
    connect(this, &Controller::startDisplayAddressPrepared, worker, &Worker::slotStartDisplayAddress, Qt::QueuedConnection);
    connect(worker, &Worker::finishDisplayAddress, this, &Controller::slotFinishDisplayAddress, Qt::QueuedConnection);
    connect(worker, &Worker::displayAddressStarted, this, [] {
        emit AppModel::instance()->displayAddressOnDevices();
    }, Qt::QueuedConnection);

    // Display address
    connect(this, &Controller::startRescanBlockchain, worker, &Worker::slotStartRescanBlockchain, Qt::QueuedConnection);
    connect(worker, &Worker::finishRescanBlockchain, this, &Controller::slotFinishRescanBlockchain, Qt::QueuedConnection);

    // Create master signer
    connect(this, &Controller::startCreateSoftwareSigner, this, &Controller::slotPrepareCreateSoftwareSigner);
    connect(this, &Controller::startCreateSoftwareSignerPrepared, worker, &Worker::slotStartCreateSoftwareSigner, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateSoftwareSigner, this, &Controller::slotFinishCreateSoftwareSigner, Qt::QueuedConnection);

    // Create software signerX
    connect(this, &Controller::startCreateSoftwareSignerXprv, this, &Controller::slotPrepareCreateSoftwareSignerXprv);
    connect(this, &Controller::startCreateSoftwareSignerXprvPrepared, worker, &Worker::slotStartCreateSoftwareSignerXprv, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateSoftwareSignerXprv, this, &Controller::slotFinishCreateSoftwareSignerXprv, Qt::QueuedConnection);

    // Create wallet
    connect(this, &Controller::startCreateWallet, this, &Controller::slotPrepareCreateWallet);
    connect(this, &Controller::startCreateWalletPrepared, worker, &Worker::slotStartCreateWallet, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateWallet, this, &Controller::slotFinishCreateWallet, Qt::QueuedConnection);

    // Create Miniscript wallet
    connect(this, &Controller::startCreateMiniscriptWallet, this, &Controller::slotPrepareCreateMiniscriptWallet);
    connect(this, &Controller::startCreateMiniscriptWalletPrepared, worker, &Worker::slotCreateMiniscriptWallet, Qt::QueuedConnection);
    connect(worker, &Worker::finishCreateMiniscriptWallet, this, &Controller::slotFinishCreateMiniscriptWallet, Qt::QueuedConnection);

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
    connect(this, &Controller::startGetEstimatedFee, worker, &Worker::slotRequestGetEstimatedFee, Qt::QueuedConnection);
    connect(this, &Controller::checkAndUnlockDevice, this, &Controller::slotCheckAndUnlockDevice, Qt::QueuedConnection);

    connect(this, &Controller::startSendPinToDevice, this, &Controller::slotPrepareSendPinToDevice);
    connect(this, &Controller::startSendPinToDevicePrepared, worker, &Worker::slotStartSendPinToDevice, Qt::QueuedConnection);
    connect(worker, &Worker::finishSendPinToDevice, this, &Controller::slotFinishSendPinToDevice, Qt::QueuedConnection);

    connect(this, &Controller::startSendPassphraseToDevice, this, &Controller::slotPrepareSendPassphraseToDevice);
    connect(this, &Controller::startSendPassphraseToDevicePrepared, worker, &Worker::slotStartSendPassphraseToDevice, Qt::QueuedConnection);
    connect(worker, &Worker::finishSendPassphraseToDevice, this, &Controller::slotFinishSendPassphraseToDevice, Qt::QueuedConnection);


    connect(this, &Controller::startRemoveAllWallets, worker, &Worker::slotStartRemoveAllWallets, Qt::QueuedConnection);
    connect(worker, &Worker::finishRemoveAllWallets, this, &Controller::slotFinishRemoveAllWallets, Qt::QueuedConnection);

    connect(this, &Controller::startRemoveAllSigners, worker, &Worker::slotStartRemoveAllSigners, Qt::QueuedConnection);
    connect(worker, &Worker::finishRemoveAllSigners, this, &Controller::slotFinishRemoveAllSigners, Qt::QueuedConnection);

    connect(this, &Controller::startMultiDeviceSync, this, &Controller::slotPrepareMultiDeviceSync);
    connect(this, &Controller::startMultiDeviceSyncPrepared, worker, &Worker::slotStartMultiDeviceSync, Qt::QueuedConnection);

    connect(this, &Controller::startReloadUserDb, worker, &Worker::slotStartReloadUserDb, Qt::QueuedConnection);

    connect(this, &Controller::startReloadWallets, worker, &Worker::slotStartReloadWallets, Qt::QueuedConnection);
    connect(worker, &Worker::finishReloadWallets, this, &Controller::slotFinishReloadWallets, Qt::QueuedConnection);
    connect(worker, &Worker::finishReloadGroupWallets, this, &Controller::slotFinishReloadGroupWallets, Qt::QueuedConnection);

    connect(this, &Controller::startReloadMasterSigners, worker, &Worker::slotStartReloadMasterSigners, Qt::QueuedConnection);
    connect(worker, &Worker::finishReloadMasterSigners, this, &Controller::slotFinishReloadMasterSigners, Qt::QueuedConnection);

    connect(this, &Controller::startReloadRemoteSigners, worker, &Worker::slotStartReloadRemoteSigners, Qt::QueuedConnection);
    connect(worker, &Worker::finishReloadRemoteSigners, this, &Controller::slotFinishReloadRemoteSigners, Qt::QueuedConnection);

    connect(this, &Controller::startSyncWalletDb, worker, &Worker::slotStartSyncWalletDb, Qt::QueuedConnection);
    connect(worker, &Worker::prepareSyncWalletDb, this, &Controller::slotApplySyncWalletSnapshot, Qt::QueuedConnection);
    connect(worker, &Worker::finishSyncWalletDb, this, &Controller::slotFinishSyncWalletDb, Qt::QueuedConnection);

    workerThread.start();
}

Controller::~Controller() {
    {
        std::lock_guard<std::mutex> lock(mainThreadDispatchMutex);
        mainThreadDispatchEnabled.store(false, std::memory_order_release);
    }
    if(workerThread.isRunning()){
        workerThread.requestInterruption();
        workerThread.quit();
        workerThread.wait(5000);
    }
    this->disconnect();
}

bool Controller::scanDevicesSync()
{
    QWarningMessage msg;
    QDeviceListModelPtr deviceList = bridge::nunchukGetDevices(msg);
    if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
        AppModel::instance()->setDeviceList(deviceList);
        return true;
    }
    else{
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return false;
}

void Controller::slotPrepareCreateMasterSigner(const QString &name, const QString &xfp)
{
    WorkerCreateMasterSignerData data;
    data.name = name;
    data.xfp = xfp;
    data.isSignedIn = AppModel::instance()->isSignIn();
    data.hwiPath = bridge::hwiCommand();
    emit startCreateMasterSignerPrepared(data);
}

void Controller::slotFinishCreateMasterSigner(const WorkerCreateMasterSignerResult &result)
{
    QMasterSignerPtr ret;
    if (result.hasSigner) {
        ret = QMasterSignerPtr(new QMasterSigner(result.signer));
        ret->setMessage(result.signerMessage);
        ret->setDeviceIndex(result.deviceIndex);
    }
    const int type = result.warning.type;

    QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
    AppModel::instance()->setMasterSignerList(mastersigners);
    if(ret && type == (int)EWARNING::WarningType::NONE_MSG){
        QString selectFingerPrint = "";
        QString keyName = "";
        QMasterSignerPtr newsigner;
        if(mastersigners){
            newsigner =  AppModel::instance()->masterSignerList()->getMasterSignerByXfp(ret.data()->fingerPrint());
            AppModel::instance()->setMasterSignerInfo(newsigner);
            selectFingerPrint = newsigner->fingerPrint();
            keyName = newsigner->name();
            QSignerManagement::instance()->updateXfpOfCurrentSigner(selectFingerPrint);
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

void Controller::slotPrepareCreateRemoteSigner(const QString &name,
                                               const QString &xpub,
                                               const QString &public_key,
                                               const QString &derivation_path,
                                               const QString &master_fingerprint,
                                               nunchuk::SignerType type,
                                               std::vector<nunchuk::SignerTag> tags,
                                               bool replace,
                                               int event)
{
    WorkerCreateRemoteSignerData data;
    data.name = name;
    data.xpub = xpub;
    data.publicKey = public_key;
    data.derivationPath = derivation_path;
    data.masterFingerprint = master_fingerprint;
    data.type = type;
    data.tags = std::move(tags);
    data.replace = replace;
    data.event = event;
    emit startCreateRemoteSignerPrepared(data);
}

void Controller::slotFinishCreateRemoteSigner(const WorkerCreateRemoteSignerResult &result)
{
    const int event = result.event;
    const nunchuk::SingleSigner &ret = result.signer;
    const QString &what = result.warning.what;
    const int type = result.warning.type;
    const int code = result.warning.code;

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

void Controller::slotPrepareScanDevices(const QVariant &data)
{
    const QVariantMap values = data.toMap();
    WorkerScanDevicesData request;
    request.stateId = values.value("state_id").toInt();
    request.isTopUpXpub = values.value("isTopUpXpub", false).toBool();
    request.masterSignerId = values.value("singleSigner_masterSignerId").toString();
    request.masterFingerprint = values.value("singleSigner_masterFingerPrint").toString();
    request.signerName = values.value("singleSigner_name").toString();
    request.isSignedIn = AppModel::instance()->isSignIn();
    request.hwiPath = bridge::hwiCommand();
    emit startScanDevicesPrepared(request);
}

void Controller::slotFinishScanDevices(const WorkerScanDevicesResult &result)
{
    QDeviceListModelPtr deviceList(new DeviceListModel());
    if(result.warning.type == (int)EWARNING::WarningType::NONE_MSG){
        for (const nunchuk::Device &it : result.devices) {
            QDevicePtr device = QDevicePtr(new QDevice(it));
            deviceList.data()->addDevice(device);
        }
        AppModel::instance()->setDeviceList(deviceList);
    }
    else {
        AppModel::instance()->setDeviceList(deviceList);
        AppModel::instance()->showToast(result.warning.code,
                                        result.warning.what,
                                        static_cast<EWARNING::WarningType>(result.warning.type));
    }
    if (result.isTopUpXpub) {
        AppModel::instance()->finishTopXPUBsSigner();
        QSignerManagement::instance()->finishCreateTopUpXpub();
    }
    if(auto trans = AppModel::instance()->transactionInfoPtr()){
        trans->refreshScanDevices();
    }
    DBG_INFO << result.devices.size() << deviceList->rowCount() << AppModel::instance()->newWalletInfoPtr().isNull();
    QGroupWallets::instance()->finishScanDevices();
    QUserWallets::instance()->finishScanDevices();
    if (auto w = AppModel::instance()->newWalletInfoPtr()) {
        w->refreshScanDevices();
    }
    emit finishedScanDevices();
    emit checkAndUnlockDevice(result.stateId);
}

void Controller::slotPrepareSigningTransaction(const QString &walletId,
                                               const QString &txid,
                                               const QString &deviceXfp,
                                               bool isSoftware)
{
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this,
                                  [this, walletId, txid, deviceXfp, isSoftware]() {
                                      slotPrepareSigningTransaction(walletId, txid, deviceXfp, isSoftware);
                                  },
                                  Qt::QueuedConnection);
        return;
    }

    auto rejectRequest = [this, walletId, isSoftware](const QString &message) {
        QWarningMessage warning;
        warning.setWarningMessage(0, message, EWARNING::WarningType::ERROR_MSG);
        slotFinishSigningTransaction(walletId,
                                     nunchuk::Transaction(),
                                     warning.what(),
                                     warning.type(),
                                     warning.code(),
                                     "-1",
                                     false,
                                     isSoftware);
    };

    auto appModel = AppModel::instance();
    auto transaction = appModel->transactionInfo();
    if (!transaction) {
        return;
    }

    const QString roomId = transaction->roomId();
    DeviceListModel *deviceList = isSoftware ? appModel->softwareSignerDeviceList() : appModel->deviceList();
    QDevicePtr selectedDevice = deviceList ? deviceList->getDeviceByXfp(deviceXfp) : QDevicePtr();
    if (!selectedDevice) {
        rejectRequest(roomId.isEmpty() ? STR_CPP_054 : STR_CPP_059);
        return;
    }

    if (!roomId.isEmpty() && !CLIENT_INSTANCE->GetRoomById(roomId)) {
        rejectRequest(STR_CPP_059);
        return;
    }

    const nunchuk::Transaction transactionSnapshot = transaction->nunchukTransaction();
    bool checkTaprootRoundOne = false;
    if (roomId.isEmpty()) {
        auto walletList = appModel->walletList();
        auto wallet = walletList ? walletList->getWalletById(walletId) : QWalletPtr();
        checkTaprootRoundOne = wallet &&
                                wallet->walletAddressType() ==
                                    (int)nunchuk::AddressType::TAPROOT;
    }

    emit startSigningTransactionPrepared(walletId,
                                         txid,
                                         selectedDevice->originDevice(),
                                         selectedDevice->masterSignerId(),
                                         roomId,
                                         transaction->initEventId(),
                                         transactionSnapshot,
                                         checkTaprootRoundOne,
                                         isSoftware);
}

void Controller::slotPrepareHealthCheckRemoteSigner(const int state_id,
                                                    const QString &xfp,
                                                    const int signer_type,
                                                    const QString &message)
{
    WorkerHealthCheckRemoteSignerData data;
    data.stateId = state_id;
    data.xfp = xfp;
    data.signerType = signer_type;
    data.message = message;

    const bool checksMasterSigner = signer_type == (int)ENUNCHUCK::SignerType::SOFTWARE ||
                                    signer_type == (int)ENUNCHUCK::SignerType::HARDWARE ||
                                    signer_type == (int)ENUNCHUCK::SignerType::COLDCARD_NFC;
    if (!checksMasterSigner) {
        if (auto signer = AppModel::instance()->singleSignerInfoPtr()) {
            data.hasSingleSigner = true;
            data.singleSigner = signer->singleSigner();
            data.signerMessage = signer->message();
            data.signerSignature = signer->signature();
        }
    }

    emit startHealthCheckRemoteSignerPrepared(data);
}

void Controller::slotPrepareCreateWallet(bool need_backup, QString file_path)
{
    auto wallet = AppModel::instance()->newWalletInfo();
    if (!wallet) {
        return;
    }

    WorkerCreateWalletData data;
    data.needBackup = need_backup;
    data.filePath = std::move(file_path);
    data.name = wallet->walletNameDisplay();
    data.m = wallet->walletM();
    data.n = wallet->walletN();
    if (auto signers = wallet->singleSignersAssigned()) {
        data.signers = signers->signers();
    }
    data.addressType = static_cast<nunchuk::AddressType>(wallet->walletAddressType());
    data.walletType = wallet->walletEscrow()
                          ? nunchuk::WalletType::ESCROW
                          : (data.n == 1 ? nunchuk::WalletType::SINGLE_SIG
                                         : nunchuk::WalletType::MULTI_SIG);
    data.description = wallet->walletDescription();
    data.walletTemplate = nunchuk::WalletTemplate::DEFAULT;

    const int configuredWalletType = wallet->walletType();
    const bool valueKeysetEnabled = wallet->enableValuekeyset();
    if (data.addressType == nunchuk::AddressType::TAPROOT &&
        configuredWalletType == (int)nunchuk::WalletType::MULTI_SIG &&
        !valueKeysetEnabled) {
        data.walletTemplate = nunchuk::WalletTemplate::DISABLE_KEY_PATH;
    }

    emit startCreateWalletPrepared(data);
}

void Controller::slotPrepareCreateMiniscriptWallet()
{
    auto wallet = AppModel::instance()->newWalletInfo();
    if (!wallet) {
        return;
    }

    WorkerCreateMiniscriptWalletData data;
    data.name = wallet->walletNameDisplay();
    data.scriptTemplate = wallet->scriptTemplate();
    data.signers = wallet->signersCreateWallet();
    data.addressType = static_cast<nunchuk::AddressType>(wallet->walletAddressType());
    data.description = wallet->walletDescription();
    data.allowUsedSigner = wallet->reUseKeys();
    emit startCreateMiniscriptWalletPrepared(data);
}

void Controller::slotPrepareSendPinToDevice(const int state_id,
                                            const int device_index,
                                            const QString &pin)
{
    WorkerDeviceInputData data;
    data.stateId = state_id;
    data.input = pin;
    if (auto deviceList = AppModel::instance()->deviceList()) {
        if (auto device = deviceList->getDeviceByIndex(device_index);
            device && device->needsPinSent()) {
            data.device = device->originDevice();
            data.shouldSend = true;
        }
    }
    emit startSendPinToDevicePrepared(data);
}

void Controller::slotPrepareSendPassphraseToDevice(const int state_id,
                                                   const int device_index,
                                                   const QString &passphrase)
{
    WorkerDeviceInputData data;
    data.stateId = state_id;
    data.input = passphrase;
    if (auto deviceList = AppModel::instance()->deviceList()) {
        if (auto device = deviceList->getDeviceByIndex(device_index);
            device && device->needsPassPhraseSent()) {
            data.device = device->originDevice();
            data.shouldSend = true;
        }
    }
    emit startSendPassphraseToDevicePrepared(data);
}

void Controller::slotPrepareMultiDeviceSync(const bool state)
{
    WorkerMultiDeviceSyncData data;
    data.state = state;
    data.canSync = CLIENT_INSTANCE->isNunchukLoggedIn() && CLIENT_INSTANCE->isMatrixLoggedIn();
    emit startMultiDeviceSyncPrepared(data);
}

void Controller::slotFinishSigningTransaction(const QString &walletId,
                                              nunchuk::Transaction result,
                                              QString what,
                                              int type,
                                              int code,
                                              QString masterSignerId,
                                              bool showTaprootRoundOneToast,
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
        auto walletList = AppModel::instance()->walletList();
        QWalletPtr wallet = walletList ? walletList->getWalletById(walletId) : QWalletPtr();
        if(auto trans = AppModel::instance()->transactionInfo()){
            if(qUtils::strCompare(tx_id, trans->txid()) && qUtils::strCompare(walletId, trans->walletId())){
                QString serverKeyMsg = trans->serverKeyMessage();
                bool isCosign = trans->isCosigning();

                if (showTaprootRoundOneToast) {
                    AppModel::instance()->showToast(0,
                                                    QString("Round 1 completed"),
                                                    EWARNING::WarningType::SUCCESS_MSG);
                }

                trans->setNunchukTransaction(result);
                trans->setServerKeyMessage(serverKeyMsg);
                trans->setIsCosigning(isCosign);
                if(wallet){
                    wallet.data()->SignAsisstedTxs(tx_id, QString::fromStdString(result.get_psbt()), QString::fromStdString(result.get_memo()));
                }
            }
        }
        AppModel::instance()->requestSyncWalletDb(walletId);
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
        if(!isSoftware){
            QMap<QString,QVariant> data;
            data["state_id"] = E::STATE_ID_SCR_TRANSACTION_INFO;
            startScanDevices(QVariant::fromValue(data));
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
            AppModel::instance()->walletList()->updateSignerHealthStatus(xfp, status, QDateTime::currentDateTime().toMSecsSinceEpoch());
        }
        emit finishedHealthCheckMasterSigner(true);
    }
    else{
        if(type == (int)EWARNING::WarningType::EXCEPTION_MSG && nunchuk::HWIException::DEVICE_CONN_ERROR == code){
            QMap<QString,QVariant> data;
            data["state_id"] = state_id;
            startScanDevices(QVariant::fromValue(data));
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
            startScanDevices(data);
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

void Controller::slotPrepareDisplayAddress(const QString &wallet_id,
                                           const QString &address)
{
    emit startDisplayAddressPrepared(wallet_id, address);
}

void Controller::slotFinishDisplayAddress(const WorkerDisplayAddressResult &result)
{
    if(result.warning.type == (int)EWARNING::WarningType::NONE_MSG){
        if(result.result){
            AppModel::instance()->showToast(0,
                                            QString("Address successfully verified"),
                                            EWARNING::WarningType::SUCCESS_MSG);
        }
        else {
            AppModel::instance()->showToast(0,
                                            QString("Address verification failed"),
                                            EWARNING::WarningType::ERROR_MSG);
        }
    }
    else {
        AppModel::instance()->showToast(result.warning.code,
                                        result.warning.what,
                                        static_cast<EWARNING::WarningType>(result.warning.type));
    }
    emit finishedDisplayAddress(result.result);
}

void Controller::slotFinishRescanBlockchain()
{
    DBG_INFO;
}

void Controller::slotPrepareCreateSoftwareSigner(const QString &name,
                                                 const QString &mnemonic,
                                                 const QString &passphrase,
                                                 bool replace)
{
    const int flow = QEventProcessor::instance()->currentFlow();
    WorkerCreateSoftwareSignerData data;
    data.name = name;
    data.secret = mnemonic;
    data.passphrase = passphrase;
    data.isPrimaryKey = flow == (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY ||
                        flow == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY;
    data.replace = replace;
    emit startCreateSoftwareSignerPrepared(data);
}

void Controller::slotFinishCreateSoftwareSigner(const WorkerCreateSoftwareSignerResult &result)
{
    if (result.signerExists) {
        emit AppModel::instance()->notifySignerExist(true, result.signerExistsFingerprint);
        return;
    }

    const nunchuk::MasterSigner &ret = result.signer;
    const QString &what = result.warning.what;
    const int type = result.warning.type;
    const int code = result.warning.code;
    QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
    AppModel::instance()->setMasterSignerList(mastersigners);
    DBG_INFO << type << ret.get_name();
    if(type != (int)EWARNING::WarningType::EXCEPTION_MSG){
        QMasterSignerPtr newsigner;
        if(mastersigners){
            newsigner =  AppModel::instance()->masterSignerList()->getMasterSignerByXfp(QString::fromStdString(ret.get_device().get_master_fingerprint()));
            AppModel::instance()->setMasterSignerInfo(newsigner);
        }
        DBG_INFO << ret.get_device().get_master_fingerprint() << ret.get_name();
        if (QSignerManagement::instance()->finishCreateSoftwareSigner()) {
            return;
        }
        DBG_INFO << "Not Progress Key: " << ret.get_name();
        QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_NONE);
    }
    else{
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
}

void Controller::slotPrepareCreateSoftwareSignerXprv(const QString &name,
                                                     const QString &xprv,
                                                     bool replace)
{
    const int flow = QEventProcessor::instance()->currentFlow();
    WorkerCreateSoftwareSignerData data;
    data.name = name;
    data.secret = xprv;
    data.isPrimaryKey = flow == (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY ||
                        flow == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY;
    data.replace = replace;
    emit startCreateSoftwareSignerXprvPrepared(data);
}

void Controller::slotFinishCreateSoftwareSignerXprv(const WorkerCreateSoftwareSignerResult &result)
{
    const nunchuk::MasterSigner &ret = result.signer;
    const QString &what = result.warning.what;
    const int type = result.warning.type;
    const int code = result.warning.code;
    if (type != (int)EWARNING::WarningType::NONE_MSG) {
        if (result.signerExists) {
            emit AppModel::instance()->notifySignerExist(true, result.signerExistsFingerprint);
        }
        AppModel::instance()->showToast(code,
                                        what,
                                        static_cast<EWARNING::WarningType>(type));
    }

    QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
    AppModel::instance()->setMasterSignerList(mastersigners);
    if(type != (int)EWARNING::WarningType::EXCEPTION_MSG){
        QMasterSignerPtr newsigner;
        if(mastersigners){
            newsigner =  AppModel::instance()->masterSignerList()->getMasterSignerByXfp(QString::fromStdString(ret.get_device().get_master_fingerprint()));
            AppModel::instance()->setMasterSignerInfo(newsigner);
        }
        DBG_INFO << ret.get_device().get_master_fingerprint() << ret.get_name();
        if (QSignerManagement::instance()->finishCreateSoftwareSignerXprv()) {
            return;
        }
        DBG_INFO << "Not Progress Key: " << ret.get_name();
        QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_NONE);
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
        std::string walletid = "";
        try {
            walletid = ret.get_id();
        }
        catch (const nunchuk::BaseException &ex) {
            DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        }
        catch (std::exception &e) {
            DBG_INFO << "THROW EXCEPTION" << e.what();
        }
        QString wallet_id = QString::fromStdString(walletid);
        AppModel::instance()->walletList()->addWallet(bridge::convertWallet(ret));
        AppModel::instance()->resetSignersChecked();
        AppModel::instance()->walletList()->requestSort();
        int index = AppModel::instance()->walletList()->getWalletIndexById(wallet_id);
        bool replaceFree = false;
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            replaceFree = w->replaceFree();
        }
        DBG_INFO << replaceFree;
        if(-1 != index && replaceFree == false){
            AppModel::instance()->setWalletListCurrentIndex(index);
        }
        if (type == (int)EWARNING::WarningType::NONE_MSG) {
            AppModel::instance()->showToast(
                0, STR_CPP_064, EWARNING::WarningType::SUCCESS_MSG);
        } else {
            AppModel::instance()->showToast(
                code, what, static_cast<EWARNING::WarningType>(type));
        }
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            w->walletCreateDone();
            QString replaceFlow = w->replaceFlow();
            auto newWallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
            newWallet->setNeedBackup(true); // New Wallet needs backup
            newWallet->setNeedRegistered(true); // New Wallet needs registered
            newWallet->setReplaceFlow(replaceFlow);
            newWallet->setScreenFlow(w->screenFlow());
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
                                          qint64 balance)
{
    Q_UNUSED(balance);
    if (auto currentWallet = AppModel::instance()->walletInfo();
        currentWallet && qUtils::strCompare(id, currentWallet->walletId())) {
        currentWallet->RequestGetCoins();
    }
}

void Controller::slotFinishTransactionChanged(const QString &tx_id,
                                              const int status,
                                              const QString &wallet_id,
                                              nunchuk::Transaction tx)
{
    DBG_INFO << tx_id << status << wallet_id;
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(AppModel::instance()->walletList()){
        QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
        if(wallet){
            QTransactionPtr trans = wallet.data()->SyncAssistedTxs(tx);
            if(trans){
                trans->setNunchukTransaction(tx);
                if (status == (int)nunchuk::TransactionStatus::DELETED) {
                    wallet.data()->transactionHistory()->removeTransaction(tx_id);                    
                } else {
                    wallet.data()->transactionHistory()->updateTransaction(tx_id, trans);
                }
                if(AppModel::instance()->transactionInfo()){
                    QString current_tx_wallet_id = AppModel::instance()->transactionInfo()->walletId();
                    QString current_tx_id        = AppModel::instance()->transactionInfo()->txid();
                    if(qUtils::strCompare(wallet_id, current_tx_wallet_id) && qUtils::strCompare(tx_id, current_tx_id)){
                        if (status == (int)nunchuk::TransactionStatus::DELETED) {
                            QEventProcessor::instance()->closeStateScreen(E::STATE_ID_SCR_TRANSACTION_INFO);
                        } else {
                            AppModel::instance()->setTransactionInfo(trans);
                        }
                    }
                }
            }
            // AppModel::instance()->walletList()->requestSort();
        }
    }
    CLIENT_INSTANCE->transactionChanged(wallet_id, tx_id, status, tx.get_height());
    qApp->restoreOverrideCursor();
}

void Controller::slotFinishBlockChanged(const int height,
                                        const QString &hex_header)
{
    AppModel::instance()->setBlockHeight(height);
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

        if(auto trans = AppModel::instance()->transactionInfo()) {
            auto newTx = wallet.data()->transactionHistory()->getTransactionByTxid(trans->txid());
            if(newTx){
                AppModel::instance()->setTransactionInfo(newTx);
            }
        }
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
        QMap<QString,QVariant> data;
        data["state_id"] = state_id;
        AppModel::instance()->startScanDevices(QVariant::fromValue(data));
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
        QMap<QString,QVariant> data;
        data["state_id"] = state_id;
        AppModel::instance()->startScanDevices(QVariant::fromValue(data));
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
        const QString lastWalletId = bridge::nunchukGetSelectedWallet();
        int lastIndex = -1;
        if (ret->rowCount() > 0) {
            lastIndex = lastWalletId.isEmpty()
                    ? 0
                    : AppModel::instance()->walletList()->getWalletIndexById(lastWalletId);
            if (lastIndex < 0 || lastIndex >= ret->rowCount()) {
                lastIndex = 0;
            }
        }
        AppModel::instance()->setWalletListCurrentIndex(lastIndex, true);
        if(ONLINE_MODE == bridge::nunchukCurrentMode() && CLIENT_INSTANCE->isNunchukLoggedIn()){
            emit WalletsMng->getListWalletFinish();
        }
        AppModel::instance()->createPrimaryKeyList();
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
        ret.data()->updateIsSandboxWallet(true);
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

void Controller::slotApplySyncWalletSnapshot(const QString &,
                                             nunchuk::Wallet wallet,
                                             bool walletLoaded)
{
    auto currentWallet = AppModel::instance()->walletInfo();
    if (walletLoaded && currentWallet) {
        currentWallet->setNunchukWallet(wallet);
    }
}

void Controller::slotFinishSyncWalletDb(const QString &wallet_id,
                                        const QStringList &usedAddresses,
                                        const QStringList &usedChangeAddresses,
                                        const QStringList &unusedAddresses,
                                        const QStringList &unusedChangeAddresses)
{
    auto appModel = AppModel::instance();
    auto currentWallet = appModel->walletInfo();
    auto walletList = appModel->walletList();

    QWalletPtr wallet;
    if (currentWallet) {
        if (qUtils::strCompare(currentWallet->walletId(), wallet_id)) {
            wallet = appModel->walletInfoPtr();
        }
    } else if (walletList) {
        wallet = walletList->getWalletById(wallet_id);
    }

    if (wallet) {
        wallet->setUsedAddressList(usedAddresses);
        wallet->setUsedChangeAddressList(usedChangeAddresses);
        wallet->setunUsedAddressList(unusedAddresses);
        wallet->setUnUsedChangeddAddressList(unusedChangeAddresses);
        wallet->startDownloadConversation();

        if (wallet->isReplaced()) {
            emit startGetTransactionHistory(wallet_id);
        } else {
            const QString groupId = wallet->groupId();
            const bool isClaimed = wallet->isClaimed();
            const bool isGroupWallet = wallet->isGroupWallet();
            const bool isUserWallet = wallet->isUserWallet();
            QPointer<Controller> safeThis(this);
            QPointer<Wallet> targetWallet(wallet.data());

            runInThread(
                this,
                [wallet_id, groupId, isClaimed, isGroupWallet, isUserWallet]() -> QJsonArray {
                    using features::transactions::usecases::FetchCancelTransactionInput;
                    using features::transactions::usecases::FetchCancelTransactionUseCase;
                    using features::transactions::usecases::FetchTransactionListInput;
                    using features::transactions::usecases::FetchTransactionListUseCase;

                    FetchTransactionListInput transactionInput;
                    transactionInput.wallet_id = wallet_id;
                    transactionInput.group_id = groupId;
                    transactionInput.isClaimed = isClaimed;
                    FetchTransactionListUseCase fetchTransactions;
                    fetchTransactions.execute(transactionInput);

                    FetchCancelTransactionInput cancelledInput;
                    cancelledInput.wallet_id = wallet_id;
                    cancelledInput.group_id = groupId;
                    cancelledInput.isClaimed = isClaimed;
                    FetchCancelTransactionUseCase fetchCancelledTransactions;
                    fetchCancelledTransactions.execute(cancelledInput);

                    QJsonObject output;
                    QString errorMessage;
                    if (isGroupWallet && Byzantine::instance()->GetAllTransactionNotes(groupId, wallet_id, output, errorMessage)) {
                        return output.value("notes").toArray();
                    }
                    if (isUserWallet && Draco::instance()->assistedWalletGetTxNotes(wallet_id, output, errorMessage)) {
                        return output.value("notes").toArray();
                    }
                    return {};
                },
                [safeThis, targetWallet, wallet_id](const QJsonArray &notes) {
                    if (!safeThis || !targetWallet ||
                        !qUtils::strCompare(targetWallet->walletId(), wallet_id)) {
                        return;
                    }
                    if (auto history = targetWallet->transactionHistory()) {
                        for (const QJsonValue &value : notes) {
                            const QJsonObject note = value.toObject();
                            history->updateTransactionMemo(note.value("transaction_id").toString(),
                                                           note.value("note").toString());
                        }
                    }
                    emit safeThis->startGetTransactionHistory(wallet_id);
                });
        }
    }

    if (walletList) {
        if (auto listedWallet = walletList->getWalletById(wallet_id)) {
            listedWallet->GetCoinControlFromServer();
        }
    }
}

void Controller::slotFinishCreateMiniscriptWallet(nunchuk::Wallet ret, QString what, int type, int code) {
    DBG_INFO << "walletListCurrentIndex";
    if (type != (int)EWARNING::WarningType::EXCEPTION_MSG) {
        std::string walletid = "";
        try {
            walletid = ret.get_id();
        }
        catch (const nunchuk::BaseException &ex) {
            DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        }
        catch (std::exception &e) {
            DBG_INFO << "THROW EXCEPTION" << e.what();
        }
        QString wallet_id = QString::fromStdString(walletid);
        AppModel::instance()->walletList()->addWallet(bridge::convertWallet(ret));
        AppModel::instance()->resetSignersChecked();
        AppModel::instance()->walletList()->requestSort();
        int index = AppModel::instance()->walletList()->getWalletIndexById(wallet_id);
        if (-1 != index) {
            AppModel::instance()->setWalletListCurrentIndex(index);
        }
        AppModel::instance()->showToast(0, STR_CPP_064, EWARNING::WarningType::SUCCESS_MSG);
        if (auto newWallet = AppModel::instance()->walletList()->getWalletById(wallet_id)) {
            newWallet->setNeedBackup(true); // New Wallet needs backup
            newWallet->setNeedRegistered(true); // New Wallet needs registered
        }
    } else {
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
    emit finishedCreateWallet();
}
