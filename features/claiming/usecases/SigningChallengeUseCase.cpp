#include "SigningChallengeUseCase.h"
#include "core/restapi/RestApi.h"

namespace features::claiming::usecases {
using namespace core::usecase;

Result<SigningChallengeResult> SigningChallengeUseCase::execute(const SigningChallengeInput &input) {
    QJsonObject output;
    bool ret = ret = Draco::instance()->inheritanceClaimSigningChallenge(input.magic, output);
    SigningChallengeResult result;
    if (ret) {
        result.message = output.value("message").toObject();
        return Result<SigningChallengeResult>::success(result);
    } else {
        int response_code = output.value("code").toInt();
        QString error_msg = output.value("message").toString();
        return Result<SigningChallengeResult>::failure(QString("Error %1: %2").arg(response_code).arg(error_msg));
    }
}
}