#include "TransactionHelper.h"
#include "ViewsEnums.h"
#include "Chats/ClientController.h"
#include "Chats/matrixbrigde.h"
#include "Models/AppModel.h"
#include "QEventProcessor.h"
#include "Servers/Draco.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

#define GUARD_TX_WALLET() \
    auto txInfo = AppModel::instance()->transactionInfo(); \
    if (!txInfo) { \
        DBG_INFO << "No transaction info available"; \
        return; \
    } \
    QString tx_id = txInfo->txid(); \
    QString wallet_id = txInfo->walletId(); \
    auto wallet = AppModel::instance()->walletList()->getWalletById(wallet_id); \
    if (!wallet) { \
        DBG_INFO << "Wallet not found for id:" << wallet_id; \
        return; \
    }


namespace helper::transaction {

// Sign a transaction with QVariant input
void signRequest(const QVariant &msg) {
    GUARD_TX_WALLET();
    QString signerXfp = msg.toString();
    if (txInfo->singleSignersAssigned()) {
        QSingleSignerPtr signer = txInfo->singleSignersAssigned()->getSingleSignerByFingerPrint(signerXfp);
        if (!signer) {
            // FIXME SHOW TOAST ?
            DBG_INFO << "Signer not found for fingerprint: " << signerXfp << txInfo->singleSignersAssigned()->rowCount();
            emit AppModel::instance() -> finishedSigningTransaction();
            return;
        }
        int signerType = signer.data()->signerType();
        if ((int)ENUNCHUCK::SignerType::SOFTWARE == signerType) {
            QDevicePtr device = AppModel::instance()->softwareSignerDeviceList()->getDeviceByXfp(signerXfp);
            int deviceIndex = AppModel::instance()->softwareSignerDeviceList()->getDeviceIndexByXfp(signerXfp);
            if (device) {
                if (device.data()->needsPassPhraseSent()) {
                    QMap<QString, QVariant> passPhraseData;
                    passPhraseData["state_id"] = E::STATE_ID_SCR_TRANSACTION_INFO;
                    passPhraseData["signer_xfp"] = signerXfp;
                    passPhraseData["wallet_id"] = wallet_id;
                    passPhraseData["tx_id"] = tx_id;
                    passPhraseData["device_index"] = deviceIndex;
                    passPhraseData["device_xfp"] = device.data()->masterFingerPrint();
                    passPhraseData["is_software"] = true;
                    QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
                } else {
                    AppModel::instance()->startSigningTransaction(wallet_id, tx_id, device.data()->masterFingerPrint(), true);
                }
            }
        } else if ((int)ENUNCHUCK::SignerType::HARDWARE == signerType || (int)ENUNCHUCK::SignerType::COLDCARD_NFC == signerType) {
            QDevicePtr device = AppModel::instance()->deviceList()->getDeviceByXfp(signerXfp);
            if (device) {
                AppModel::instance()->startSigningTransaction(wallet_id, tx_id, device.data()->masterFingerPrint(), false);
            } else {
                // FIXME SHOW TOAST ?
                DBG_INFO << "Device not found for fingerprint: " << signerXfp;
                emit AppModel::instance() -> finishedSigningTransaction();
            }
        } else {
            // FIXME SHOW TOAST ?
            DBG_INFO << "Signer type is not SOFTWARE or HARDWARE";
            emit AppModel::instance() -> finishedSigningTransaction();
        }
    }
}

// Import a transaction from hex or JSON using QVariant
void importFile(const QVariant &msg) {
    GUARD_TX_WALLET();
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if (file_path != "" && wallet) {
        QWarningMessage msgwarning;
        QTransactionPtr trans = bridge::nunchukImportTransaction(wallet_id, file_path, msgwarning);
        if (trans) {
            AppModel::instance()->setTransactionInfo(trans);
            wallet.data()->SignAsisstedTxs(trans.data()->txid(), trans.data()->psbt(), trans.data()->memo());
            AppModel::instance()->requestSyncWalletDb(wallet_id);
        }
    }
}

// Export a transaction to hex or JSON using QVariant
void exportFile(const QVariant &msg) {
    GUARD_TX_WALLET();
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if (file_path != "") {
        QWarningMessage msgwarning;
        bool ret = bridge::nunchukExportTransaction(wallet_id, txInfo->txid(), file_path,
                                                    msgwarning);
        if ((int)EWARNING::WarningType::NONE_MSG != msgwarning.type()) {
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }

        DBG_INFO << ret << file_path;
    }
}

// Broadcast a transaction to the network using QVariant
void broadcast(const QVariant &msg) {
    GUARD_TX_WALLET();
    if (txInfo->roomId() != "") {
        QNunchukRoom *room = CLIENT_INSTANCE->GetRoomById(txInfo->roomId());
        if (room) {
            QString init_event_id = txInfo->initEventId();
            QWarningMessage msgwarning;
            matrixbrigde::BroadcastTransaction(room->id(), init_event_id, msgwarning);
            DBG_INFO << wallet_id << init_event_id << msgwarning.type();
            if ((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()) {
                if ((int)ENUNCHUCK::TabSelection::CHAT_TAB == AppModel::instance()->tabIndex()) {
                    QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                } else {
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                }
                room->startGetPendingTxs();
                AppModel::instance()->requestSyncWalletDb(wallet_id);
                AppModel::instance()->showToast(0, STR_CPP_085, EWARNING::WarningType::SUCCESS_MSG);
            } else {
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
            }
        }
    } else {
        QWarningMessage msgwarning;
        QTransactionPtr trans = bridge::nunchukBroadcastTransaction(wallet_id, txInfo->txid(), msgwarning);
        DBG_INFO << wallet_id << txInfo->txid() << msgwarning.type();
        if ((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()) {
            if (trans) {
                if (auto wallet = AppModel::instance()->walletList()->getWalletById(wallet_id)) {
                    wallet.data()->AssignTagsToTxChange();
                }
                AppModel::instance()->setTransactionInfo(trans);
                AppModel::instance()->showToast(0, STR_CPP_085, EWARNING::WarningType::SUCCESS_MSG);
            }
        } else {
            if (nunchuk::NunchukException::SERVER_REQUEST_ERROR == msgwarning.code()) {
                txInfo->setStatus((int)ENUNCHUCK::TransactionStatus::NETWORK_REJECTED);
                AppModel::instance()->showToast(nunchuk::NunchukException::SERVER_REQUEST_ERROR, STR_CPP_086, EWARNING::WarningType::EXCEPTION_MSG);
            } else {
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
            }
        }
        QWalletPtr wallet_result = bridge::nunchukGetWallet(wallet_id);
        if (wallet_result && AppModel::instance()->walletList()) {
            AppModel::instance()->walletList()->updateBalance(wallet_id, wallet_result.data()->balanceSats());
        }
        AppModel::instance()->requestSyncWalletDb(wallet_id);
    }
}

// Replace-By-Fee (RBF) using QVariant
void replaceByFee(const QVariant &msg) {
    GUARD_TX_WALLET();
    txInfo->setTxidReplacing(txInfo->txid());
}

// Set or update a memo/label for a transaction using QVariant
void setMemo(const QVariant &msg) {
    GUARD_TX_WALLET();
    QString memo = msg.toString();
    if (memo != "") {
        bridge::nunchukUpdateTransactionMemo(wallet_id, tx_id, memo);
        if (auto wallet = AppModel::instance()->walletList()->getWalletById(wallet_id)) {
            if (wallet->transactionHistory()) {
                wallet->transactionHistory()->updateTransactionMemo(tx_id, memo);
            }
            wallet->UpdateAssistedTxs(tx_id, memo);
        }
        if (CLIENT_INSTANCE) {
            CLIENT_INSTANCE->updateTransactionMemo(wallet_id, tx_id, memo);
        }
    }
}

// Scan a device using QVariant
void scanDevice(const QVariant &msg) {
    QMap<QString,QVariant> data;
    data["state_id"] = E::STATE_ID_SCR_TRANSACTION_INFO;
    AppModel::instance()->startScanDevices(QVariant::fromValue(data));
}

// Remove a transaction using QVariant
void remove(const QVariant &msg) {
    GUARD_TX_WALLET();
    if (txInfo->status() == (int)ENUNCHUCK::TransactionStatus::PENDING_CONFIRMATION) {
        QEventProcessor::instance()->sendEvent(E::EVT_TRANSACTION_INFO_CANCEL_PENDING_TRANSACTION_REQUEST);
    } else {
        DBG_INFO << wallet_id << tx_id;
        bool ret = bridge::nunchukDeleteTransaction(wallet_id, tx_id);
        if (ret) {
            if (wallet) {
                wallet.data()->CancelAssistedTxs(tx_id);
            }
            AppModel::instance()->requestSyncWalletDb(wallet_id);
            if (QEventProcessor::instance()->onsRequester() == E::STATE_ID_SCR_TRANSACTION_HISTORY) {
                QEventProcessor::instance()->sendEvent(E::EVT_TRANSACTION_INFO_BACK_REQUEST);
            } else {
                if ((int)ENUNCHUCK::TabSelection::CHAT_TAB == AppModel::instance()->tabIndex()) {
                    QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                } else {
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                }
            }
            AppSetting::instance()->deleteTransactionKeysetIndex(tx_id);
            AppSetting::instance()->deleteTransactionSigningPath(tx_id);
            AppSetting::instance()->deleteTransactionUsescriptpath(tx_id);

            AppModel::instance()->setTransactionInfo(QTransactionPtr(new Transaction()));
        }
    }
}

// Export a transaction as QR content using QVariant
void exportQr(const QVariant &msg) {
    AppModel::instance()->setQrExported(QStringList());
    GUARD_TX_WALLET();
    QString qrtype = msg.toMap().value("qrtype").toString();
    DBG_INFO << qrtype;
    QWarningMessage msgwarning;
    if (qUtils::strCompare(qrtype, "QR-transaction")) {
        QStringList qrtags =
            bridge::nunchukExportQRTransaction(wallet_id, txInfo->txid(), msgwarning);
        if ((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()) {
            AppModel::instance()->setQrExported(qrtags);
        } else {
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }
    } else if (qUtils::strCompare(qrtype, "BBQR-transaction")) {
        QStringList qrtags = qUtils::ExportBBQRTransaction(txInfo->psbt(), msgwarning);
        DBG_INFO << qrtags;
        if ((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && !qrtags.isEmpty()) {
            AppModel::instance()->setQrExported(qrtags);
        } else {
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }
    }
}

// Import a transaction from QR content using QVariant
void importQr(const QVariant &msg) {
    GUARD_TX_WALLET();
    wallet->SignAsisstedTxs(txInfo->txid(), txInfo->psbt(), txInfo->memo());
    AppModel::instance()->showToast(0, STR_CPP_101, EWARNING::WarningType::SUCCESS_MSG);
}

// Verify an address using QVariant
void verifyAddress(const QVariant &msg) {
    GUARD_TX_WALLET();
    AppModel::instance()->startDisplayAddress(wallet_id, msg.toString());
}

// Cancel a pending request using QVariant
void cancelRequest(const QVariant &msg) {
    GUARD_TX_WALLET();
    if (txInfo->roomId() != "") {
        QNunchukRoom *room = CLIENT_INSTANCE->GetRoomById(txInfo->roomId());
        if (room) {
            QString init_event_id = txInfo->initEventId();
            QWarningMessage warningmsg;
            matrixbrigde::CancelTransaction(room->id(), init_event_id, STR_CPP_077, warningmsg);
            if ((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()) {
                AppModel::instance()->requestSyncWalletDb(txInfo->walletId());
                room->startGetPendingTxs();
                if (QEventProcessor::instance()->onsRequester() == E::STATE_ID_SCR_TRANSACTION_HISTORY) {
                    QEventProcessor::instance()->sendEvent(E::EVT_TRANSACTION_INFO_BACK_REQUEST);
                } else {
                    if ((int)ENUNCHUCK::TabSelection::CHAT_TAB == AppModel::instance()->tabIndex()) {
                        QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                    } else {
                        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_TRANSACTION_INFO);
                    }
                }
            } else {
                AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
            }
        }
    }
}
} // namespace helper::transaction