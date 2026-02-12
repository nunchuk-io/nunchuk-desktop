#include "UpdateWalletUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::wallets::usecases {
using namespace core::usecase;

Result<UpdateWalletResult> UpdateWalletUseCase::execute(const UpdateWalletInput &input) {
    QString errormsg = "";
    QJsonObject output;
    bool ret = false;
    if (qUtils::isValidGroup(input.group_id)) {
        ret = Byzantine::instance()->UpdateWallet(input.group_id, input.wallet_id, input.name, input.description, output, errormsg);
    } else if(input.isClaimed) {
        ret = Draco::instance()->ClaimingWalletUpdate(input.wallet_id, input.name, input.description, output, errormsg);
    } else {
        ret = Draco::instance()->assistedWalletUpdate(input.wallet_id, input.name, input.description, output, errormsg);
    }
    UpdateWalletResult result;
    if (ret) {
        return Result<UpdateWalletResult>::success(result);
    } else {
        return Result<UpdateWalletResult>::failure(QString("%1 %2").arg(errormsg).arg(input.wallet_id));
    }
}

} // namespace features::wallets::usecases