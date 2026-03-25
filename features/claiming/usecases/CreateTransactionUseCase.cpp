#include "CreateTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::claiming::usecases {
using namespace core::usecase;

Result<CreateTransactionResult> CreateTransactionUseCase::execute(const CreateTransactionInput &input) {
    QJsonObject body;
    body["magic"] = input.magic;
    body["address"] = input.address;
    body["fee_rate"] = qUtils::QValueFromAmount(input.fee_rate);
    if (input.anti_fee_sniping) {
        body["anti_fee_sniping"] = input.anti_fee_sniping;
    }
    if (input.amount > 0) {
        body["amount"] = qUtils::QValueFromAmount(input.amount);
    }
    if (!input.bsms.isEmpty()) {
        body["bsms"] = input.bsms;
    }
    body["subtract_fee_from_amount"] = input.subtract_fee_from_amount;
    body["message_id"] = input.messageId;

    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = body;


    CreateTransactionResult result;
    QJsonObject output;
    bool ret = Draco::instance()->inheritanceCreateTx(data, input.authos, output);
    
    if (ret) {
        auto transactionObj = output.value("transaction").toObject();
        result.psbt = transactionObj.value("psbt").toString();
        QString sub_amount = QString("%1").arg(output.value("sub_amount").toDouble());
        QString fee = QString("%1").arg(output.value("fee").toDouble());
        QString fee_rate = QString("%1").arg(output.value("fee_rate").toDouble());
        result.change_pos = output.value("change_pos").toInt();
        result.tx_fee = qUtils::QAmountFromValue(fee);               // fee in BTC
        result.tx_fee_rate = qUtils::QAmountFromValue(fee_rate);     // fee_rate in BTC
        result.tx_sub_amount = qUtils::QAmountFromValue(sub_amount); // sub amount in BTC
        result.subtract_fee_from_amount = output.value("subtract_fee_from_amount").toBool();
        return Result<CreateTransactionResult>::success(result);
    } else {
        int response_code = output.value("code").toInt();
        QString error_msg = output.value("message").toString();
        return Result<CreateTransactionResult>::failure(QString("Error %1: %2").arg(response_code).arg(error_msg));
    }
}
}