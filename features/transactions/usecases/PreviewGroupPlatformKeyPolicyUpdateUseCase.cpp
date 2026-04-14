#include "PreviewGroupPlatformKeyPolicyUpdateUseCase.h"

#include "core/bridge/ExternalBridges.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<PreviewGroupPlatformKeyPolicyUpdateResult> PreviewGroupPlatformKeyPolicyUpdateUseCase::execute(const PreviewGroupPlatformKeyPolicyUpdateInput &input) {
    QWarningMessage msg;
    const auto requirement = bridge::PreviewGroupPlatformKeyPolicyUpdate(input.wallet_id, input.policies, msg);

    if (!msg.isSuccess()) {
        return Result<PreviewGroupPlatformKeyPolicyUpdateResult>::failure(msg.what(), msg.code());
    }

    PreviewGroupPlatformKeyPolicyUpdateResult result;
    result.dumyTx = requirement.get_dummy_transaction();
    result.requires_dummy_transaction = requirement.requires_dummy_transaction();
    result.pending_signatures = result.dumyTx.has_value() ? result.dumyTx.value().get_pending_signatures() : 0;
    result.delay_apply_in_seconds = requirement.get_delay_apply_in_seconds();
  
    return Result<PreviewGroupPlatformKeyPolicyUpdateResult>::success(result);
}

} // namespace features::transactions::usecases
