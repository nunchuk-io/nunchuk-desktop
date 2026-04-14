#include "GroupDummyTransactionDetailsViewModel.h"
#include "Models/Premiums/QWalletDummyTx.h"
#include "core/bridge/ExternalBridges.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"

namespace features::transactions::viewmodels {
using namespace features::transactions::usecases;

GroupDummyTransactionDetailsViewModel::GroupDummyTransactionDetailsViewModel(QObject *parent) : GeneralTransactionDetailsViewModel(parent) {
    settransactionInfo(QTransactionPtr(new QDummyTransaction()));
}

void GroupDummyTransactionDetailsViewModel::onInit() {
    QWarningMessage msg;
    QString wallet_id = QString::fromStdString(dummyTx().get_wallet_id());
    nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id, msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        setnunWallet(wallet);
    } else {
        DBG_ERROR << "Failed to retrieve wallet for ID: " << wallet_id << ", warning type: " << msg.type();
    }
    setwalletId(wallet_id);
    auto walletPtr = bridge::convertWallet(nunWallet());
    if (walletPtr) {
        setwalletInfo(walletPtr);
    }
    GUARD_APP_MODEL()
    if (appModel->walletInfoPtr() && !qUtils::strCompare(appModel->walletInfoPtr()->walletId(), walletId())) {
        appModel->setWalletInfo(walletPtr);
    }
    setDummyTxData(dummyTx());
}

void GroupDummyTransactionDetailsViewModel::setDummyTxData(const nunchuk::GroupDummyTransaction &dummyTx) {
    DBG_INFO << "Setting dummy transaction data for transaction ID: " << QString::fromStdString(dummyTx.get_id());
    if (dummyTx.get_id().empty()) {
        return;
    }

    QString request_body = QString::fromStdString(dummyTx.get_request_body());
    QString wallet_id = QString::fromStdString(dummyTx.get_wallet_id());
    DBG_INFO << "Wallet ID: " << wallet_id << ", Request body size: " << request_body.length();
    QWarningMessage msg;
    QString tx_to_sign = qUtils::GetHealthCheckDummyTx(nunWallet(), request_body, msg);

    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        msg.resetWarningMessage();
        nunchuk::Transaction tx = qUtils::DecodeDummyTx(nunWallet(), tx_to_sign, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type() && transactionInfo()) {
            setJsonTxData(dummyTx);
            transactionInfo()->setNunchukTransaction(tx);
            transactionInfo()->setPendingSignatures(dummyTx.get_pending_signatures());
            transactionInfo()->setWalletId(wallet_id);
            DBG_INFO << "Transaction data set successfully for wallet: " << wallet_id << dummyTx.get_pending_signatures();
        }
    }
    if (transactionInfo()) {
        emit transactionInfo() -> nunchukTransactionChanged();
    }
    emit transactionInfoChanged();
}

void GroupDummyTransactionDetailsViewModel::setJsonTxData(const nunchuk::GroupDummyTransaction &dummyTx) {
    QJsonObject txJson;
    txJson["id"] = QString::fromStdString(dummyTx.get_id());
    txJson["wallet_id"] = QString::fromStdString(dummyTx.get_wallet_id());
    txJson["request_body"] = QString::fromStdString(dummyTx.get_request_body());
    QJsonArray signaturesList;
    for (const auto &sig : dummyTx.get_signatures()) {
        QJsonObject sigJson;
        sigJson["xfp"] = QString::fromStdString(sig.get_master_fingerprint());
        sigJson["signature"] = qUtils::CreateRequestToken(QString::fromStdString(sig.get_master_fingerprint()), QString::fromStdString(sig.get_signature()));
        signaturesList.append(sigJson);
    }
    txJson["signatures"] = signaturesList;

    DBG_INFO << txJson;
    transactionInfo()->setTxJson(txJson);
}

void GroupDummyTransactionDetailsViewModel::signDeviceRequest(const QString &xfp) {
    if (!transactionInfo()) {
        emit finishedSigningTransaction();
        return;
    }

    QString wallet_id = QString::fromStdString(nunWallet().get_id());
    QWarningMessage msg;
    QString request_body = dummyTx().get_request_body().empty() ? "" : QString::fromStdString(dummyTx().get_request_body());
    QString dummyTransactionId = QString::fromStdString(dummyTx().get_id());

    msg.resetWarningMessage();
    QString tx_to_sign = qUtils::GetHealthCheckDummyTx(nunWallet(), request_body, msg);
    DBG_INFO << "tx:" << tx_to_sign << xfp;
    DBG_INFO << "body:" << request_body;

    if ((int)EWARNING::WarningType::NONE_MSG != msg.type()) {
        emit finishedSigningTransaction();
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        return;
    }
    auto walletSigners = nunWallet().get_signers();
    auto signer_it = std::find_if(walletSigners.begin(), walletSigners.end(), [xfp](const nunchuk::SingleSigner &s) {
        QString signerXfp = QString::fromStdString(s.get_master_fingerprint());
        return qUtils::strCompare(signerXfp, xfp);
    });

    if (signer_it == walletSigners.end()) {
        DBG_INFO << "Signer not found for xfp:" << xfp << " in wallet:" << wallet_id << " for dummy transaction:" << walletSigners.size();
        for (const auto &s : walletSigners) {
            DBG_INFO << "Available signer xfp:" << QString::fromStdString(s.get_master_fingerprint());
        }
        emit finishedSigningTransaction();
        return;
    }

    const nunchuk::SingleSigner &signer = *signer_it;
    QMap<QString, QString> signatures;
    QString signature = "";

    DBG_INFO << (int)signer.get_type() << QString::fromStdString(signer.get_master_fingerprint()) << QString::fromStdString(signer.get_master_signer_id());

    switch (signer.get_type()) {
    case nunchuk::SignerType::SOFTWARE: {
        GUARD_APP_MODEL()
        auto masterSignerList = appModel->masterSignerListPtr();
        if (!masterSignerList) {
            DBG_INFO << "Master signer list not available";
            emit finishedSigningTransaction();
            return;
        }
        auto software = masterSignerList->getMasterSignerByXfp(xfp);
        if (!software) {
            emit finishedSigningTransaction();
            return;
        }
        QDevicePtr device = software->devicePtr();
        if (!device) {
            DBG_INFO << "Software signer device not found for fingerprint: " << xfp;
            emit finishedSigningTransaction();
            return;
        }
        msg.resetWarningMessage();
        DBG_INFO << "Signing tx software:" << xfp;
        signature = bridge::SignHealthCheckMessage(nunWallet(), device->originDevice(), signer, tx_to_sign, msg);
        DBG_INFO << "Signature result for software signer xfp:" << xfp << ", signature:" << signature;
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            signatures[xfp] = signature;
        }
    } break;
    case nunchuk::SignerType::HARDWARE:
    case nunchuk::SignerType::COLDCARD_NFC: {
        GUARD_APP_MODEL()
        auto device = appModel->deviceListPtr()->getDeviceByXfp(xfp);
        if (!device) {
            DBG_INFO << "Device not found for xfp:" << xfp;
            break;
        }
        msg.resetWarningMessage();
        DBG_INFO << "Signing tx via HWI for xfp:" << xfp;
        signature = bridge::SignHealthCheckMessage(nunWallet(), device->originDevice(), signer, tx_to_sign, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            signatures[xfp] = signature;
        }
        break;
    }
    default:
        // Not support -> Show popup
        break;
    }

    emit finishedSigningTransaction();
    if (!signatures.isEmpty()) {
        requestUpdateDummyTx(signatures);
    } else {
        DBG_INFO << msg.type();
    }
}

void GroupDummyTransactionDetailsViewModel::setMemo(const QString &memo) {
    Q_UNUSED(memo)
}

void GroupDummyTransactionDetailsViewModel::txSignRequest(const QString &xfp) {
    QtConcurrent::run([this, xfp]() { signDeviceRequest(xfp); });
}

void GroupDummyTransactionDetailsViewModel::txSignImportQr(const QStringList &tags) {
    QWarningMessage msg;
    QString psbt = qUtils::ParseQRTransaction(tags, msg);
    QString wallet_id = QString::fromStdString(nunWallet().get_id());
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type() && psbt != "") {
        msg.resetWarningMessage();
        nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            msg.resetWarningMessage();
            nunchuk::Transaction tx = qUtils::DecodeDummyTx(wallet, psbt, msg);
            if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                QMap<QString, QString> signatures;
                QMap<QString, QString> keys;
                transactionInfo()->setNunchukTransaction(tx);
                transactionInfo()->setWalletId(wallet_id);
                setJsonTxData(dummyTx());
                auto signers = transactionInfo()->singleSignersAssigned();
                for (auto ptr : signers->fullList()) {
                    if (ptr->signerSigned()) {
                        QString signature = qUtils::GetPartialSignature(ptr->originSingleSigner(), QString::fromStdString(tx.get_psbt()));
                        signatures[ptr->masterFingerPrint()] = signature.isEmpty() ? ptr->signature() : signature;
                        keys[ptr->masterFingerPrint()] = QString::fromStdString(ptr->originSingleSigner().get_name());
                    }
                }
                if (!signatures.isEmpty()) {
                    requestUpdateDummyTx(signatures);
                }
                emit transactionInfoChanged();
            }
        }
    }
    if ((int)EWARNING::WarningType::NONE_MSG != msg.type()) {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    emit finishedSigningTransaction();
}

void GroupDummyTransactionDetailsViewModel::txSignImportFile(const QString &filePath) {
    QString psbt = qUtils::ImportDataViaFile(filePath);
    QString wallet_id = QString::fromStdString(nunWallet().get_id());
    if (psbt != "") {
        // Convert tx
        QWarningMessage msg;
        nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            msg.resetWarningMessage();
            nunchuk::Transaction tx = qUtils::DecodeDummyTx(wallet, psbt, msg);
            if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                QMap<QString, QString> signatures;
                QMap<QString, QString> keys;
                transactionInfo()->setNunchukTransaction(tx);
                transactionInfo()->setWalletId(wallet_id);
                setJsonTxData(dummyTx());
                auto signers = transactionInfo()->singleSignersAssigned();
                for (auto ptr : signers->fullList()) {
                    if (ptr->signerSigned()) {
                        QString signature = qUtils::GetPartialSignature(ptr->originSingleSigner(), QString::fromStdString(tx.get_psbt()));
                        signatures[ptr->masterFingerPrint()] = signature.isEmpty() ? ptr->signature() : signature;
                        keys[ptr->masterFingerPrint()] = QString::fromStdString(ptr->originSingleSigner().get_name());
                    }
                }
                if (!signatures.isEmpty()) {
                    requestUpdateDummyTx(signatures);
                }
            }
        }
        if ((int)EWARNING::WarningType::NONE_MSG != msg.type()) {
            AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        }
    }
    emit finishedSigningTransaction();
}

void GroupDummyTransactionDetailsViewModel::txSignExportQr() {
    auto request_body = QString::fromStdString(dummyTx().get_request_body());
    QWarningMessage msg;
    QString psbt = qUtils::GetHealthCheckDummyTx(nunWallet(), request_body, msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        msg.resetWarningMessage();
        QStringList qrtags = qUtils::ExportQRTransaction(psbt, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            msg.resetWarningMessage();
            setqrExported(qrtags);
        }
    }
    if (!msg.isSuccess()) {
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

void GroupDummyTransactionDetailsViewModel::txSignExportBBQr() {
    auto request_body = QString::fromStdString(dummyTx().get_request_body());
    QWarningMessage msg;
    QString psbt = qUtils::GetHealthCheckDummyTx(nunWallet(), request_body, msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        msg.resetWarningMessage();
        QStringList qrtags = qUtils::ExportBBQRTransaction(psbt, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            msg.resetWarningMessage();
            setqrExported(qrtags);
        }
    }
    if (!msg.isSuccess()) {
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

void GroupDummyTransactionDetailsViewModel::txSignExportFile(const QString &filePath) {
    qUtils::ExportDataViaFile(filePath, transactionInfo()->psbt());
}

void GroupDummyTransactionDetailsViewModel::txBroadcast() {}

void GroupDummyTransactionDetailsViewModel::requestUpdateDummyTx(const QMap<QString, QString> &signatures) {
    QString wallet_id = QString::fromStdString(nunWallet().get_id());
    QString dummyTransactionId = QString::fromStdString(dummyTx().get_id());

    QWarningMessage msg;
    QStringList authorizations;

    // Build authorization list from all signatures
    for (auto it = signatures.constBegin(); it != signatures.constEnd(); ++it) {
        QString authorization = qUtils::CreateRequestToken(it.value(), it.key(), msg);
        authorizations.append(authorization);
    }
    msg.resetWarningMessage();
    DBG_INFO << "wallet_id" << wallet_id << "dummyTransactionId" << dummyTransactionId << "authorizations" << authorizations;
    SignGroupDummyTransactionInput input;
    input.wallet_id = wallet_id;
    input.dummy_transaction_id = dummyTransactionId;
    input.signatures = authorizations;

    m_signGroupDummyTransactionUC.executeAsync(input, [this, wallet_id](core::usecase::Result<SignGroupDummyTransactionResult> result) {
        if (result.isSuccess()) {
            auto updatedDummyTx = result.value().dummy_transaction;
            DBG_INFO << "Pending signatures: " << updatedDummyTx.get_pending_signatures()
                     << ", Required signatures: " << updatedDummyTx.get_required_signatures();
            setDummyTxData(updatedDummyTx);
            GUARD_WALLET(wallet_id)
            if (auto dashboard = wallet->dashboard()) {
                dashboard->GetAlertsInfo();
                dashboard->GetHealthCheckInfo();
            }
            if (0 == updatedDummyTx.get_pending_signatures()) {
                wallet->GetGroupWalletConfig();
                wallet->walletConfigChanged();
                emit showToast(0, "Platform key policies have been updated.", EWARNING::WarningType::SUCCESS_MSG);
                close();
            }
        } else {
            // Handle error case, e.g., show an error message to the user
            QString errorMsg = result.error();
            emit showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void GroupDummyTransactionDetailsViewModel::cancelDummyTransaction() {
    CancelGroupDummyTransactionInput input;
    input.wallet_id = QString::fromStdString(dummyTx().get_wallet_id());
    input.dummy_transaction_id = QString::fromStdString(dummyTx().get_id());
    m_cancelGroupDummyTransactionUC.executeAsync(input, [this](core::usecase::Result<CancelGroupDummyTransactionResult> result) {
        if (result.isSuccess()) {
            // Handle success case

        } else {
            // Handle error case, e.g., show an error message to the user
            QString errorMsg = result.error();
            emit showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

} // namespace features::transactions::viewmodels
