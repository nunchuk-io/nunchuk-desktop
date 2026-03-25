#include "ClaimInheritanceUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::claiming::usecases {
using namespace core::usecase;

Result<ClaimInheritanceResult> ClaimInheritanceUseCase::execute(const ClaimInheritanceInput &input) {
    QJsonObject output;
    QJsonObject body;
    body["magic"] = input.magic;
    body["bsms"] = input.bsms;
    body["psbt"] = QString::fromStdString(input.txSigned.get_psbt());
    bool ret = Draco::instance()->inheritanceClaimRequest(body, output);
    nunchuk::Transaction tx = input.txSigned;
    QJsonObject transaction = output.value("transaction").toObject();
    QString status = transaction.value("status").toString();
    QString psbt = transaction.value("psbt").toString();
    QString wallet_local_id = transaction.value("wallet_local_id").toString();
    QWarningMessage msg;
    if (status == "PENDING_CONFIRMATION" || status == "CONFIRMED") {
        tx = bridge::transaction::ImportPsbt(wallet_local_id, psbt, msg);
        QString id = transaction.value("transaction_id").toString();
        QString hex = transaction.value("hex").toString();
        QString reject_msg = transaction.value("reject_msg").toString();
        tx = bridge::transaction::UpdateTransaction(input.wallet_id, QString::fromStdString(input.txSigned.get_txid()), id, hex, reject_msg, msg);
    }
    ClaimInheritanceResult result;
    if (ret) {
        if (msg.isSuccess()) {
            result.tx = tx;
        }
        return Result<ClaimInheritanceResult>::success(result);
    } else {
        int response_code = output.value("code").toInt();
        QString error_msg = output.value("message").toString();
        return Result<ClaimInheritanceResult>::failure(QString("Error %1: %2").arg(response_code).arg(error_msg));
    }
}
} // namespace features::claiming::usecases