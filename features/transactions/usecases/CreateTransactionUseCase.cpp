#include "CreateTransactionUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<CreateTransactionResult> CreateTransactionUseCase::execute(const CreateTransactionInput &input) {
    CreateTransactionResult result;
    QJsonObject data;
    QString msgerror = "";
    if (qUtils::isValidGroup(input.group_id)) {
        Byzantine::instance()->CreateTransaction(input.group_id, input.wallet_id, input.psbt, input.note, data, msgerror);
    } else if(input.isClaimed) {
        Draco::instance()->ClaimingWalletTransactionSync(input.wallet_id, input.psbt, data, msgerror);
    } else {
        Draco::instance()->assistedWalletCreateTx(input.wallet_id, input.psbt, input.note);
    }
    result.synced = true;
    return Result<CreateTransactionResult>::success(result);
}

} // namespace features::transactions::usecases