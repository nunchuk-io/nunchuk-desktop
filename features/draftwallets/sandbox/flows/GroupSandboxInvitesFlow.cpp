#include "GroupSandboxInvitesFlow.h"
#include "features/draftwallets/sandbox/viewmodels/GroupSandboxInvitesViewModel.h"

namespace features::draftwallets::sandbox::flows {
using namespace features::draftwallets::sandbox::viewmodels;

GroupSandboxInvitesFlow::GroupSandboxInvitesFlow(FlowContext *ctx, QObject *parent) : core::flow::BaseFlow(ctx, parent) {}

void GroupSandboxInvitesFlow::bind(QObject *vm) {
    auto invitesVm = qobject_cast<GroupSandboxInvitesViewModel *>(vm);
    if (invitesVm) {
        invitesVm->setinvitesList(invitesList());
        invitesVm->setgroupId(groupId());
    }
}

} // namespace features::draftwallets::sandbox::flows
