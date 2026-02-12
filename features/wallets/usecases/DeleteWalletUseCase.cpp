#include "DeleteWalletUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::wallets::usecases {
using namespace core::usecase;

Result<DeleteWalletResult> DeleteWalletUseCase::execute(const DeleteWalletInput &input) {
    QString errormsg = "";
    QJsonObject output;
    bool ret = false;
    if (qUtils::isValidGroup(input.group_id)) {
        ret = Byzantine::instance()->DeleteGroupWallet(input.wallet_id, input.group_id, {}, input.passwordToken, input.secQuesToken, output, errormsg);
    } else if(input.isClaimed) {
        ret = Draco::instance()->ClaimingWalletDelete(input.wallet_id, output, errormsg);
    } else {
        ret = Draco::instance()->DeleteAssistedWallet(input.wallet_id, {}, input.passwordToken, input.secQuesToken, output, errormsg);
    }
    DeleteWalletResult result;
    if (ret) {
        return Result<DeleteWalletResult>::success(result);
    } else {
        return Result<DeleteWalletResult>::failure(QString("%1 %2").arg(errormsg).arg(input.wallet_id));
    }
}
}