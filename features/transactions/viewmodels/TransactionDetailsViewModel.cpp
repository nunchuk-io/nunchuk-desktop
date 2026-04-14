#include "TransactionDetailsViewModel.h"
#include "Models/WalletModel.h"
#include "ViewsEnums.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/transactions/flows/TransactionFlow.h"
#include "generated_qml_keys.hpp"

namespace features::transactions::viewmodels {
using namespace features::transactions::usecases;
using namespace features::signers::usecases;
using features::transactions::flows::TransactionFlow;

TransactionDetailsViewModel::TransactionDetailsViewModel(QObject *parent) : GeneralTransactionDetailsViewModel(parent) {}

TransactionFlow *TransactionDetailsViewModel::currentTransactionFlow() {
    GUARD_FLOW_MANAGER(nullptr)
    return qobject_cast<TransactionFlow *>(flowMng->currentFlow());
}

void TransactionDetailsViewModel::importQrOffChain(TransactionFlow *flow, const QStringList &tags) {
    QWarningMessage msg;
    QString psbt_signed = qUtils::ParseQRTransaction(tags, msg);
    flow->proceedTransaction(psbt_signed);
}

void TransactionDetailsViewModel::importQrOnChainOrNormal(const QStringList &tags) {
    QWarningMessage msg;
    nunchuk::Transaction signedTx = bridge::nunchukImportQRTransaction(walletId(), tags, msg);
    if (msg.isSuccess()) {
        updateTransactionStatus(signedTx);
    } else {
        emit showToast(0, Strings.STR_QML_2117(), EWARNING::WarningType::ERROR_MSG);
    }
}

void TransactionDetailsViewModel::importFileOffChain(TransactionFlow *flow, const QString &filePath) {
    QString psbt_signed = qUtils::ImportDataViaFile(filePath);
    DBG_INFO << "Imported PSBT from file: " << psbt_signed;
    if (psbt_signed.isEmpty()) {
        emit showToast(0, Strings.STR_QML_2117(), EWARNING::WarningType::ERROR_MSG);
        return;
    }
    flow->proceedTransaction(psbt_signed);
}

void TransactionDetailsViewModel::importFileOnChainOrNormal(const QString &filePath) {
    QWarningMessage msg;
    nunchuk::Transaction signedTx = bridge::transaction::ImportTransaction(walletId(), filePath, msg);
    if (msg.isSuccess()) {
        updateTransactionStatus(signedTx);
    } else {
        emit showToast(0, Strings.STR_QML_2117(), EWARNING::WarningType::ERROR_MSG);
    }
}

void TransactionDetailsViewModel::setMemo(const QString &memo) {
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto flow = qobject_cast<TransactionFlow *>(currentFlow);
    if (!flow) {
        return;
    }
    if (flow->isOffClaim()) {
        return;
    } else if (flow->isOnClaim() || flow->isNormal()) {
        GUARD_CLIENT_CONTROLLER()
        QString tx_id = transactionInfo()->txid();
        bridge::nunchukUpdateTransactionMemo(walletId(), tx_id, memo);
        GUARD_WALLET(walletId())
        if (wallet->transactionHistory()) {
            wallet->transactionHistory()->updateTransactionMemo(tx_id, memo);
        }
        wallet->UpdateAssistedTxs(tx_id, memo);
        clientCtrl->updateTransactionMemo(walletId(), tx_id, memo);
    }
}

void TransactionDetailsViewModel::txSignRequest(const QString &xfp) {
    GUARD_APP_MODEL()
    QString tx_id = transactionInfo()->txid();
    if (transactionInfo()->singleSignersAssigned()) {
        QSingleSignerPtr signer = transactionInfo()->singleSignersAssigned()->getSingleSignerByFingerPrint(xfp);
        if (!signer) {
            DBG_INFO << "Signer not found for fingerprint: " << xfp << transactionInfo()->singleSignersAssigned()->rowCount();
            emit finishedSigningTransaction();
            return;
        }
        int signerType = signer.data()->signerType();
        if ((int)ENUNCHUCK::SignerType::SOFTWARE == signerType) {
            auto masterSignerList = appModel->masterSignerListPtr();
            if (!masterSignerList) {
                DBG_INFO << "Master signer list not available";
                emit finishedSigningTransaction();
                return;
            }
            auto software = masterSignerList->getMasterSignerByXfp(xfp);
            if (!software) {
                DBG_INFO << "Software signer not found for fingerprint: " << xfp;
                emit showToast(0, "Software signer not found", EWARNING::WarningType::ERROR_MSG);
                emit finishedSigningTransaction();
                return;
            }
            QDevicePtr device = software->devicePtr();
            if (!device) {
                DBG_INFO << "Software signer device not found for fingerprint: " << xfp;
                emit showToast(0, "Signer device not found", EWARNING::WarningType::ERROR_MSG);
                emit finishedSigningTransaction();
                return;
            }
            auto softwareDeviceList = appModel->softwareSignerDeviceList();
            if (!softwareDeviceList) {
                DBG_INFO << "Software signer device list not available";
                emit finishedSigningTransaction();
                return;
            }
            int deviceIndex = softwareDeviceList->getDeviceIndexByXfp(xfp);
            if (device.data()->needsPassPhraseSent()) {
                QMap<QString, QVariant> passPhraseData;
                passPhraseData["state_id"] = E::STATE_ID_SCR_TRANSACTION_INFO;
                passPhraseData["signer_xfp"] = xfp;
                passPhraseData["wallet_id"] = walletId();
                passPhraseData["tx_id"] = tx_id;
                passPhraseData["device_index"] = deviceIndex;
                passPhraseData["device_xfp"] = device.data()->masterFingerPrint();
                passPhraseData["is_software"] = true;
                QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
            } else {
                startSigningTransaction(device, true);
            }
        } else if ((int)ENUNCHUCK::SignerType::HARDWARE == signerType || (int)ENUNCHUCK::SignerType::COLDCARD_NFC == signerType) {
            auto deviceList = appModel->deviceList();
            if (!deviceList) {
                DBG_INFO << "Device list not available";
                emit finishedSigningTransaction();
                return;
            }
            QDevicePtr device = deviceList->getDeviceByXfp(xfp);
            if (device) {
                startSigningTransaction(device, false);
            } else {
                DBG_INFO << "Device not found for fingerprint: " << xfp;
                emit finishedSigningTransaction();
            }
        } else {
            DBG_INFO << "Signer type is not SOFTWARE or HARDWARE";
            emit finishedSigningTransaction();
        }
    }
}

void TransactionDetailsViewModel::txSignImportQr(const QStringList &tags) {
    auto flow = currentTransactionFlow();
    if (!flow) {
        return;
    }
    if (flow->isOffClaim()) {
        importQrOffChain(flow, tags);
    } else if (flow->isOnClaim() || flow->isNormal()) {
        importQrOnChainOrNormal(tags);
    }
}

void TransactionDetailsViewModel::txSignImportFile(const QString &filePath) {
    auto flow = currentTransactionFlow();
    if (!flow) {
        return;
    }
    QString file_path = qUtils::QGetFilePath(filePath);
    if (file_path.isEmpty()) {
        return;
    }
    if (flow->isOffClaim()) {
        importFileOffChain(flow, file_path);
    } else if (flow->isOnClaim() || flow->isNormal()) {
        importFileOnChainOrNormal(file_path);
    }
}

void TransactionDetailsViewModel::txSignExportQr() {
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto flow = qobject_cast<TransactionFlow *>(currentFlow);
    if (!flow) {
        return;
    }
    if (flow->isOffClaim()) {
        exportQrOffChain();
    } else if (flow->isOnClaim() || flow->isNormal()) {
        exportQrOnChainOrNormal();
    }
}

void TransactionDetailsViewModel::exportQrOffChain() {
    QWarningMessage msg;
    QString psbt = QString::fromStdString(nunTx().get_psbt());
    QStringList qrtags = qUtils::ExportQRTransaction(psbt, msg);
    if (msg.isSuccess() && !qrtags.isEmpty()) {
        setqrExported(qrtags);
    } else {
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}
void TransactionDetailsViewModel::exportQrOnChainOrNormal() {
    QWarningMessage msg;
    QStringList qrtags = bridge::nunchukExportQRTransaction(walletId(), transactionInfo()->txid(), msg);
    if (msg.isSuccess() && !qrtags.isEmpty()) {
        setqrExported(qrtags);
    } else {
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

void TransactionDetailsViewModel::txSignExportBBQr() {
    QWarningMessage msg;
    QStringList qrtags = qUtils::ExportBBQRTransaction(transactionInfo()->psbt(), msg);
    if (msg.isSuccess() && !qrtags.isEmpty()) {
        setqrExported(qrtags);
    } else {
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

void TransactionDetailsViewModel::txSignExportFile(const QString &filePath) {
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto flow = qobject_cast<TransactionFlow *>(currentFlow);
    if (!flow) {
        return;
    }
    QString file_path = qUtils::QGetFilePath(filePath);
    if (file_path.isEmpty()) {
        return;
    }
    if (flow->isOffClaim()) {
        QString psbt = QString::fromStdString(nunTx().get_psbt());
        qUtils::ExportDataViaFile(file_path, psbt);
    } else if (flow->isOnClaim() || flow->isNormal()) {
        QWarningMessage msg;
        bool ret = bridge::nunchukExportTransaction(walletId(), transactionInfo()->txid(), file_path, msg);
        if (!msg.isSuccess()) {
            emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        }
        DBG_INFO << ret << file_path;
    }
}

void TransactionDetailsViewModel::txBroadcast() {
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto flow = qobject_cast<TransactionFlow *>(currentFlow);
    if (!flow) {
        return;
    }
    if (flow->isOffClaim()) {
        flow->manualClaimingTransaction(nunTx());
    } else if (flow->isOnClaim() || flow->isNormal()) {
        QWarningMessage msg;
        LibBroadcastTxInput input;
        input.wallet_id = walletId();
        input.tx_id = transactionInfo()->txid();
        m_broadcastTxUC.executeAsync(input, [this, flow](core::usecase::Result<LibBroadcastTxResult> result) {
            if (result.isSuccess()) {
                const auto &tx = result.value().tx;
                walletInfo()->AssignTagsToTxChange();
                updateTransactionStatus(tx);
                if (flow->isOnClaim()) {
                    GUARD_SUB_SCREEN_MANAGER()
                    subMng->show(qml::features::claiming::qtransactiondetailsclaimed);
                } else {
                    emit showToast(0, Strings.STR_QML_2103(), EWARNING::WarningType::SUCCESS_MSG);
                }
            } else {
                if (nunchuk::NunchukException::SERVER_REQUEST_ERROR == result.code()) {
                    transactionInfo()->setStatus((int)ENUNCHUCK::TransactionStatus::NETWORK_REJECTED);
                    emit showToast(nunchuk::NunchukException::SERVER_REQUEST_ERROR, Strings.STR_QML_2104(), EWARNING::WarningType::EXCEPTION_MSG);
                } else {
                    emit showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
                }
            }
        });
    }
}

void TransactionDetailsViewModel::startSigningTransaction(const QDevicePtr &device, bool isSoftware) {
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto flow = qobject_cast<TransactionFlow *>(currentFlow);
    if (!flow) {
        return;
    }
    LibSignTransactionInput input;
    if (flow->isOffClaim()) {
        input.wallet = nunWallet();
        input.wallet_id = std::nullopt;
        input.tx = nunTx();
        input.tx_id = std::nullopt;
    } else if (flow->isOnClaim() || flow->isNormal()) {
        input.wallet = std::nullopt;
        input.wallet_id = walletId();
        input.tx_id = QString::fromStdString(nunTx().get_txid());
        input.tx = std::nullopt;
    }
    input.devices = {device->originDevice()};
    m_signTransactionUC.executeAsync(input, [this, isSoftware, device](core::usecase::Result<LibSignTransactionResult> result) {
        if (result.isSuccess()) {
            const auto &signedTx = result.value().tx;
            if (isSoftware) {
                QWarningMessage warningmsg;
                bridge::nunchukClearSignerPassphrase(device->masterSignerId(), warningmsg);
            }
            updateTransactionStatus(signedTx);
        } else {
            QString errorMsg = result.error();
            emit showToast(0, errorMsg, EWARNING::WarningType::ERROR_MSG);
            emit finishedSigningTransaction();
        }
    });
}

void TransactionDetailsViewModel::updateTransactionStatus(const nunchuk::Transaction &tx) {
    auto transaction = transactionInfo();
    if (walletInfo()) {
        int walletAddressType = walletInfo()->walletAddressType();
        if (walletAddressType == (int)nunchuk::AddressType::TAPROOT) {
            std::vector<nunchuk::KeysetStatus> from_keyset_status = transaction->keysetStatus();
            std::vector<nunchuk::KeysetStatus> to_keyset_status = tx.get_keyset_status();
            if (from_keyset_status.size() == to_keyset_status.size()) {
                bool hasChangedToReadyToBroadcast = false;
                for (size_t i = 0; i < from_keyset_status.size(); ++i) {
                    const nunchuk::TransactionStatus &from_tx_status = from_keyset_status[i].first;
                    const nunchuk::TransactionStatus &to_tx_status = to_keyset_status[i].first;
                    if (from_tx_status == nunchuk::TransactionStatus::PENDING_SIGNATURES && to_tx_status == nunchuk::TransactionStatus::READY_TO_BROADCAST) {
                        hasChangedToReadyToBroadcast = true;
                    }
                }
                if (!hasChangedToReadyToBroadcast) {
                    for (size_t i = 0; i < from_keyset_status.size(); ++i) {
                        const nunchuk::TransactionStatus &from_tx_status = from_keyset_status[i].first;
                        const nunchuk::TransactionStatus &to_tx_status = to_keyset_status[i].first;
                        if (from_tx_status == nunchuk::TransactionStatus::PENDING_NONCE && to_tx_status == nunchuk::TransactionStatus::PENDING_SIGNATURES) {
                            QString msgtoast = QString("Round 1 completed");
                            showToast(0, msgtoast, EWARNING::WarningType::SUCCESS_MSG);
                            break;
                        }
                    }
                }
            }
        }
    }
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto flow = qobject_cast<TransactionFlow *>(currentFlow);
    if (!flow) {
        return;
    }
    if (flow->isOffClaim()) {
        GUARD_FLOW_MANAGER()
        auto currentFlow = flowMng->currentFlow();
        auto transactionFlow = qobject_cast<TransactionFlow *>(currentFlow);
        if (transactionFlow) {
            transactionFlow->proceedTransaction(tx);
        }
    } else if (flow->isOnClaim() || flow->isNormal()) {
        setnunTx(tx);
        displayTransactionInfo();
        emit finishedSigningTransaction();
        AppModel::instance()->requestSyncWalletDb(walletId());
    }
}

void TransactionDetailsViewModel::proceedTransactionResult() {
    GUARD_FLOW_MANAGER()
    auto currentFlow = flowMng->currentFlow();
    auto transactionFlow = qobject_cast<TransactionFlow *>(currentFlow);
    if (!transactionFlow) {
        return;
    }
    auto tx = transactionFlow->nunTx();
    if (transactionFlow->isOffClaim() && transactionFlow->isAllreadySigned()) {
        tx.set_status(nunchuk::TransactionStatus::READY_TO_BROADCAST);
        setnunTx(tx);
        txBroadcast(); // auto broadcast if all keys signed for off-claim flow
    } else {
        setnunTx(tx);
    }
    displayTransactionInfo();
    emit finishedSigningTransaction();
    AppModel::instance()->requestSyncWalletDb(walletId());
}

} // namespace features::transactions::viewmodels