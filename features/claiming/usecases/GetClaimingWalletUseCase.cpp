#include "GetClaimingWalletUseCase.h"
#include "Servers/Draco.h"

namespace features::claiming::usecases {
using namespace core::usecase;

Result<GetClaimingWalletResult> GetClaimingWalletUseCase::execute(const GetClaimingWalletInput &input) {
    QJsonObject output;
    QString errormsg;
    bool ret = ret = Draco::instance()->ClaimingWalletGet(input.wallet_id, output, errormsg);
    GetClaimingWalletResult result;
    result.wallet = output.value("wallet").toObject();
    if (ret) {
        return Result<GetClaimingWalletResult>::success(result);
    } else {
        int response_code = output.value("code").toInt();
        QString error_msg = output.value("message").toString();
        return Result<GetClaimingWalletResult>::failure(QString("Error %1: %2").arg(response_code).arg(error_msg));
    }
}
}