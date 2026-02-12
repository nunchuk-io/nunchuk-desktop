#include "SyncTransactionFlowUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"
#include "features/transactions/usecases/SyncTransactionUseCase.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<SyncTransactionFlowResult> SyncTransactionFlowUseCase::execute(const SyncTransactionFlowInput &input) {
    bool allowSync = input.tx.get_status() == nunchuk::TransactionStatus::PENDING_SIGNATURES ||
    input.tx.get_status() == nunchuk::TransactionStatus::READY_TO_BROADCAST;
    if (!allowSync) {
        SyncTransactionFlowResult result;
        result.tx = input.tx;
        return Result<SyncTransactionFlowResult>::success(result);
    }
    FetchTransactionInput fetchInput;
    fetchInput.wallet_id = input.wallet_id;
    fetchInput.group_id = input.group_id;
    fetchInput.txid = QString::fromStdString(input.tx.get_txid());
    fetchInput.isClaimed = input.isClaimed;
    FetchTransactionUseCase fetchTransactionUC;
    auto result = fetchTransactionUC.execute(fetchInput);
    if (result.isSuccess()) {
        QJsonObject transaction = result.value().transaction;
        UpdateTransactionSchedule(input.wallet_id, transaction, input.tx);
        UpdateOrReplaceTransaction(input.wallet_id, transaction, input.tx);
        SyncTransactionFlowResult syncResult;
        syncResult.transaction = transaction;
        syncResult.tx = SyncTransaction(input.wallet_id, transaction, input.tx, fetchInput);     
        return Result<SyncTransactionFlowResult>::success(syncResult);           
    } else {
        return Result<SyncTransactionFlowResult>::failure(QString("Failed to sync transaction %1: %2").arg(input.wallet_id));
    }  
}

void SyncTransactionFlowUseCase::UpdateTransactionSchedule(const QString& wallet_id, const QJsonObject &transaction, const nunchuk::Transaction &tx) {
    QString status = transaction.value("status").toString();
    QString type = transaction.value("type").toString();
    long int broadcast_time_milis = static_cast<long int>(transaction.value("broadcast_time_milis").toDouble());
    QWarningMessage msg;
    long int current_time_stamp_milis = static_cast<long int>(std::time(nullptr)) * 1000;
    if (type == "SCHEDULED" && broadcast_time_milis > current_time_stamp_milis && broadcast_time_milis / 1000 != tx.get_schedule_time()) {
        bridge::nunchukUpdateTransactionSchedule(wallet_id, QString::fromStdString(tx.get_txid()), broadcast_time_milis / 1000, msg);
    } else if (type != "SCHEDULED" && tx.get_schedule_time() != -1) {
        bridge::nunchukUpdateTransactionSchedule(wallet_id, QString::fromStdString(tx.get_txid()), -1, msg);
    } else {
    }
}

void SyncTransactionFlowUseCase::UpdateOrReplaceTransaction(const QString& wallet_id, const QJsonObject& transaction, const nunchuk::Transaction &tx) {
    QString status = transaction.value("status").toString();
    QString hex = transaction.value("hex").toString();
    QString reject_msg = transaction.value("reject_msg").toString();
    QString psbt = transaction.value("psbt").toString();
    QString transaction_id = transaction.value("transaction_id").toString();
    QString note = transaction.value("note").toString();
    QString replace_txid = transaction.value("replace_txid").toString();
    if (status == "PENDING_CONFIRMATION" || status == "CONFIRMED" || status == "NETWORK_REJECTED") {
        QWarningMessage msg;
        bridge::nunchukImportPsbt(wallet_id, psbt, msg);
        msg.resetWarningMessage();
        bridge::nunchukUpdateTransaction(wallet_id, QString::fromStdString(tx.get_txid()), transaction_id, hex, reject_msg, msg);
    } else if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES") {
        QWarningMessage msg;
        bridge::nunchukImportPsbt(wallet_id, psbt, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            if (replace_txid != "" && !qUtils::strCompare(QString::fromStdString(tx.get_replace_txid()), replace_txid)) {
                bridge::nunchukReplaceTransactionId(wallet_id, transaction_id, replace_txid, msg);
            }
        }
    }
    bridge::transaction::UpdateTransactionMemo(wallet_id, transaction_id, note);
}

nunchuk::Transaction SyncTransactionFlowUseCase::SyncTransaction(const QString& wallet_id, const QJsonObject &transaction, const nunchuk::Transaction &tx,
                                                                 const FetchTransactionInput &fetchInput) {
    QString transaction_id = transaction.value("transaction_id").toString();
    QString psbt = transaction.value("psbt").toString();
    QString status = transaction.value("status").toString();
    QWarningMessage msg;
    nunchuk::Transaction newTx = bridge::nunchukGetOriginTransaction(wallet_id, transaction_id, msg);
    if (msg.isSuccess() && (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES")) {
        QString newpsbt = QString::fromStdString(newTx.get_psbt());
        if (!qUtils::strCompare(psbt, newpsbt)) {
            SyncTransactionInput input;
            input.wallet_id = wallet_id;
            input.group_id = fetchInput.group_id;
            input.txid = transaction_id;
            input.isClaimed = fetchInput.isClaimed;
            input.psbt = psbt;
            input.note = transaction.value("note").toString();
            SyncTransactionUseCase syncTransactionUC;
            syncTransactionUC.execute(input);
        }
    }
    return newTx;
}

} // namespace features::transactions::usecases