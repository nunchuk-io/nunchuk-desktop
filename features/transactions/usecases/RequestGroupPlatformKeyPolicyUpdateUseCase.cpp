#include "RequestGroupPlatformKeyPolicyUpdateUseCase.h"

#include "core/bridge/ExternalBridges.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<RequestGroupPlatformKeyPolicyUpdateResult> RequestGroupPlatformKeyPolicyUpdateUseCase::execute(const RequestGroupPlatformKeyPolicyUpdateInput &input) {
    QWarningMessage msg;
    const auto requirement = bridge::RequestGroupPlatformKeyPolicyUpdate(input.wallet_id, input.policies, msg);
    if (!msg.isSuccess()) {
        return Result<RequestGroupPlatformKeyPolicyUpdateResult>::failure(msg.what(), msg.code());
    }
    RequestGroupPlatformKeyPolicyUpdateResult result;
    result.dumyTx = requirement.get_dummy_transaction();
    result.requires_dummy_transaction = requirement.requires_dummy_transaction();
    result.pending_signatures = result.dumyTx.has_value() ? result.dumyTx.value().get_pending_signatures() : 0;

    return Result<RequestGroupPlatformKeyPolicyUpdateResult>::success(result);
}

} // namespace features::transactions::usecases