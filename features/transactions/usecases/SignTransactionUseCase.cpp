#include "SignTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<SignTransactionResult> SignTransactionUseCase::execute(const SignTransactionInput &input) {
    SignTransactionResult result;
    QJsonObject data;
    QString msgerror = "";
    if (qUtils::isValidGroup(input.group_id)) {
        Byzantine::instance()->SignTransaction(input.group_id, input.wallet_id, input.txid, input.psbt, input.note, data, msgerror);
    } else if(input.isClaimed) {
        Draco::instance()->ClaimingWalletTransactionSync(input.wallet_id, input.psbt, data, msgerror);
    } else {
        data = Draco::instance()->assistedWalletSignTx(input.wallet_id, input.txid, input.psbt, input.note);
    }
    QJsonObject transaction = data.value("transaction").toObject();
    if (!transaction.isEmpty()) {
        if (input.isClaimed) {
            UpdateTransactionClaim(transaction, input.wallet_id);
        } else {
            UpdateTransaction(input.txid, transaction,  input.wallet_id);
        }
        result.transaction = transaction;
        return Result<SignTransactionResult>::success(result);
    } else {
        return Result<SignTransactionResult>::failure(QString("Failed to get transaction %1: %2").arg(input.wallet_id));
    }
}

void SignTransactionUseCase::UpdateTransaction(const QString& txid, const QJsonObject &transaction, const QString &wallet_id) {
    QString status = transaction.value("status").toString();
    QString psbt = transaction.value("psbt").toString();
    if (qUtils::strCompare(status, "PENDING_CONFIRMATION") || qUtils::strCompare(status, "CONFIRMED") || qUtils::strCompare(status, "NETWORK_REJECTED")){
        QWarningMessage _msg;
        bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
        QString id = transaction.value("id").toString();
        QString hex = transaction.value("hex").toString();
        QString reject_msg = transaction.value("reject_msg").toString();
        bridge::nunchukUpdateTransaction(wallet_id, txid, id, hex, reject_msg, _msg);
    }
    else if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES") {
        QWarningMessage _msg;
        bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
    }
}

void SignTransactionUseCase::UpdateTransactionClaim(const QJsonObject &transaction, const QString &wallet_id) {
    QString status = transaction.value("status").toString();
    QString psbt = transaction.value("psbt").toString();
    if (qUtils::strCompare(status, "PENDING_CONFIRMATION") || qUtils::strCompare(status, "CONFIRMED") || qUtils::strCompare(status, "NETWORK_REJECTED")){
        QWarningMessage _msg;
        bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
    }
    else if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES") {
        QWarningMessage _msg;
        bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
    }
}

} // namespace features::transactions::usecases