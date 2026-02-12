#include "UpdateTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<UpdateTransactionResult> UpdateTransactionUseCase::execute(const UpdateTransactionInput &input) {
    UpdateTransactionResult result;
    QJsonObject data;
    QString msgerror = "";
    if (qUtils::isValidGroup(input.group_id)) {
        Byzantine::instance()->UpdateTransaction(input.group_id, input.wallet_id, input.txid, input.note, data, msgerror);
    } else if(input.isClaimed) {
        
    } else {
        Draco::instance()->assistedWalletUpdateTx(input.wallet_id, input.txid, input.note);
    }
    result.synced = true;
    return Result<UpdateTransactionResult>::success(result);
}

} // namespace features::transactions::usecases