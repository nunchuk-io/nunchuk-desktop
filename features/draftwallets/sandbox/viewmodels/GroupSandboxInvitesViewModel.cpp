#include "GroupSandboxInvitesViewModel.h"
#include "features/draftwallets/sandbox/usecases/SendSharedWalletInvitationUseCase.h"

namespace features::draftwallets::sandbox::viewmodels {
using namespace features::draftwallets::sandbox::usecases;

GroupSandboxInvitesViewModel::GroupSandboxInvitesViewModel(QObject *parent) : BaseViewModel(parent) {
    setinvitesList({});
}

void GroupSandboxInvitesViewModel::onInit() {
    BaseViewModel::onInit();
}

void GroupSandboxInvitesViewModel::onSendInviteClicked() {
    QStringList invites = invitesList();
    SendSharedWalletInvitationInput input;
    DBG_INFO << "Sending invitations to: " << invites << " (" << invites.size() << ") for group: " << groupId();
    input.emails = invites;
    input.group_id = groupId();

    m_sendSharedWalletInvitationUseCase.executeAsync(input, [this](const core::usecase::Result<SendSharedWalletInvitationResult> &result) {
        if (result.isSuccess()) {
            close();
        } else {
            emit showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

} // namespace features::draftwallets::sandbox::viewmodels
