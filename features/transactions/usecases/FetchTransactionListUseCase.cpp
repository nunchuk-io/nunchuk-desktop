#include "FetchTransactionListUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<FetchTransactionListResult> FetchTransactionListUseCase::execute(const FetchTransactionListInput &input) {
    FetchTransactionListResult result;
    QJsonObject data;
    QString msgerror = "";
    if (qUtils::isValidGroup(input.group_id)) {
        Byzantine::instance()->GetAllTransaction(input.group_id, input.wallet_id, data, msgerror);
    } else if (input.isClaimed) {
        data = Draco::instance()->ClaimingWalletTransactionGetList(input.wallet_id);
    } else {
        data = Draco::instance()->assistedWalletGetListTx(input.wallet_id);
    }
    QJsonArray transactions = data.value("transactions").toArray();
    if (!transactions.isEmpty()) {
        UpdateTransactionList(transactions, input.wallet_id);
        return Result<FetchTransactionListResult>::success(result);
    } else {
        return Result<FetchTransactionListResult>::failure(QString("Failed to get transaction %1: %2").arg(input.wallet_id));
    }
}

void FetchTransactionListUseCase::UpdateTransactionList(const QJsonArray &transactions, const QString &wallet_id) {
    for (QJsonValue js_value : transactions) {
        QJsonObject transaction = js_value.toObject();
        QString status = transaction.value("status").toString();
        QString psbt = transaction.value("psbt").toString();
        QString memo = transaction.value("note").toString();
        ;
        QString type = transaction.value("type").toString();
        QString transaction_id = transaction.value("transaction_id").toString();
        if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES") {
            QWarningMessage _msg;
            QTransactionPtr tran = bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
            if (tran && (int)EWARNING::WarningType::NONE_MSG == _msg.type()) {
                bridge::nunchukUpdateTransactionMemo(wallet_id, transaction_id, memo);
                long int broadcast_time_milis = static_cast<long int>(transaction.value("broadcast_time_milis").toDouble());
                // honey badger feature: schedule broadcast
                long int current_time_stamp_milis = static_cast<long int>(std::time(nullptr)) * 1000;
                if (type == "SCHEDULED" && broadcast_time_milis > current_time_stamp_milis) {
                    bridge::nunchukUpdateTransactionSchedule(wallet_id, transaction_id, broadcast_time_milis / 1000, _msg);
                }
            }
        }
    }
}

} // namespace features::transactions::usecases