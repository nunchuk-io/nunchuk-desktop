#include "InitializeInheritanceClaimingUseCase.h"
#include "Servers/Draco.h"

namespace features::claiming::usecases {
using namespace core::usecase;

Result<InitializeInheritanceClaimingResult> InitializeInheritanceClaimingUseCase::execute(const InitializeInheritanceClaimingInput &input) {
    QJsonObject output;
    bool ret = ret = Draco::instance()->inheritanceClaimInit(input.magic, output);
    InitializeInheritanceClaimingResult result;
    if (ret) {
        result.magic = input.magic;
        result.wallet_type = output.value("wallet_type").toString();
        result.wallet_id = output.value("wallet_local_id").toString();
        result.inheritance_key_count = output.value("inheritance_key_count").toInt();
        result.key_origins = output.value("key_origins").toArray();
        result.required_key_counts = output.value("required_key_counts").toArray();
        if (result.required_key_counts.size() == 1) {
            result.inheritance_key_count = result.required_key_counts.at(0).toInt(result.inheritance_key_count);
        }
        return Result<InitializeInheritanceClaimingResult>::success(result);
    } else {
        int code = output.value("code").toInt();
        QString message = output.value("message").toString();
        return Result<InitializeInheritanceClaimingResult>::failure(message, code);
    }
}
}