#include "GetGroupInvitationsUseCase.h"
#include "core/restapi/RestApi.h"

namespace features::draftwallets::sandbox::usecases {
using namespace core::usecase;

Result<GetGroupInvitationsResult> GetGroupInvitationsUseCase::execute(const GetGroupInvitationsInput &input) {
    if (input.group_id.isEmpty()) {
        return Result<GetGroupInvitationsResult>::failure("Group ID is required");
    }

    QJsonObject invitationsData;
    QString error_msg = "";
    bool ret = Shared::instance()->GetGroupInvitations(input.group_id, invitationsData, error_msg);

    if (ret && !invitationsData.isEmpty()) {
        GetGroupInvitationsResult result;
        result.invitations = invitationsData.value("invitations").toArray();
        return Result<GetGroupInvitationsResult>::success(result);
    } else {
        return Result<GetGroupInvitationsResult>::failure(error_msg.isEmpty() ? "Failed to retrieve group invitations" : error_msg);
    }
}

} // namespace features::draftwallets::sandbox::usecases
