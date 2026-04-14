#include "GroupSandboxSettingViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/draftwallets/sandbox/flows/GroupSandboxInvitesFlow.h"
#include "features/draftwallets/sandbox/usecases/GetGroupInvitationsUseCase.h"
#include "features/draftwallets/sandbox/usecases/RemoveInvitationUseCase.h"
#include "generated_qml_keys.hpp"

namespace features::draftwallets::sandbox::viewmodels {
using namespace features::draftwallets::sandbox::usecases;
using namespace features::draftwallets::sandbox::flows;

GroupSandboxSettingViewModel::GroupSandboxSettingViewModel(QObject *parent) : BaseViewModel(parent) {}

void GroupSandboxSettingViewModel::onInit() {
    loadInvitations();
}

void GroupSandboxSettingViewModel::onAddInvitesClicked() {
    closeToWalletTab();
    GUARD_FLOW_MANAGER()
    flowMng->stopFlow<GroupSandboxInvitesFlow>();
    auto flow = flowMng->startFlow<GroupSandboxInvitesFlow>();
    if (!flow)
        return;
    flow->setinvitesList(invitesList());
    flow->setgroupId(groupId());
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::draftwallets::qgroupsandboxinvites);
}

void GroupSandboxSettingViewModel::onRemoveInviteClicked(QString invitationId) {
    if (invitationId.isEmpty()) {
        return;
    }

    RemoveInvitationInput input;
    input.invitation_id = invitationId;

    m_removeInvitationUseCase.executeAsync(input, [this, invitationId](const core::usecase::Result<RemoveInvitationResult> &result) {
        if (result.isSuccess()) {
            // Remove the invitation from the list after successful removal
            loadInvitations();
        } else {
            emit showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void GroupSandboxSettingViewModel::loadInvitations() {
    if (groupId().isEmpty()) {
        return;
    }

    GetGroupInvitationsInput input;
    input.group_id = groupId();

    m_getGroupInvitationsUseCase.executeAsync(input, [this](const core::usecase::Result<GetGroupInvitationsResult> &result) {
        if (result.isSuccess()) {
            const auto &data = result.value();
            QStringList invitations;
            for (const QJsonValue &value : data.invitations) {
                QJsonObject invite = value.toObject();
                QString recipient_email = invite.value("recipient_email").toString();

                if (!recipient_email.isEmpty()) {
                    invitations.append(recipient_email);
                }
            }
            DBG_INFO << "loadInvitations: " << invitations;
            setinvitesListJson(data.invitations.toVariantList());
            setinvitesList(invitations);
        } else {
            emit showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

} // namespace features::draftwallets::sandbox::viewmodels
