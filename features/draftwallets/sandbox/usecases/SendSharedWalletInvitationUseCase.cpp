#include "SendSharedWalletInvitationUseCase.h"
#include "core/restapi/RestApi.h"

namespace features::draftwallets::sandbox::usecases {
using namespace core::usecase;

Result<SendSharedWalletInvitationResult> SendSharedWalletInvitationUseCase::execute(const SendSharedWalletInvitationInput &input) {
    if (input.emails.isEmpty()) {
        return Result<SendSharedWalletInvitationResult>::failure("Email is required");
    }

    QJsonObject request_body;
    QJsonArray emailArray;
    for (const QString &email : input.emails) {
        emailArray.append(email);
    }
    request_body["emails"] = emailArray;
    if (!input.group_id.isEmpty()) {
        request_body["group_id"] = input.group_id;
    }

    QJsonObject invitationData;
    QString error_msg = "";
    bool ret = Shared::instance()->SendSharedWalletInvitation(request_body, invitationData, error_msg);

    if (ret && !invitationData.isEmpty()) {
        SendSharedWalletInvitationResult result;
        result.invitation = invitationData;
        return Result<SendSharedWalletInvitationResult>::success(result);
    } else {
        return Result<SendSharedWalletInvitationResult>::failure(error_msg.isEmpty() ? "Failed to send invitation" : error_msg);
    }
}

} // namespace features::draftwallets::sandbox::usecases
