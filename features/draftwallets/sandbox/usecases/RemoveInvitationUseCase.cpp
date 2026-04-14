#include "RemoveInvitationUseCase.h"
#include "core/restapi/RestApi.h"

namespace features::draftwallets::sandbox::usecases {
using namespace core::usecase;

Result<RemoveInvitationResult> RemoveInvitationUseCase::execute(const RemoveInvitationInput &input) {
    if (input.invitation_id.isEmpty()) {
        return Result<RemoveInvitationResult>::failure("Invitation ID is required");
    }

    QJsonObject removalData;
    QString error_msg = "";
    bool ret = Shared::instance()->RemoveInvitation(input.invitation_id, removalData, error_msg);

    if (ret) {
        RemoveInvitationResult result;
        result.success = true;
        return Result<RemoveInvitationResult>::success(result);
    } else {
        return Result<RemoveInvitationResult>::failure(error_msg.isEmpty() ? "Failed to remove invitation" : error_msg);
    }
}

} // namespace features::draftwallets::sandbox::usecases
