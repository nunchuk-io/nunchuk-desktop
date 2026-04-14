#pragma once
#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/draftwallets/sandbox/usecases/GetGroupInvitationsUseCase.h"
#include "features/draftwallets/sandbox/usecases/RemoveInvitationUseCase.h"
#include <QVariant>

namespace features::draftwallets::sandbox::viewmodels {
using core::viewmodels::BaseViewModel;
using features::draftwallets::sandbox::usecases::GetGroupInvitationsUseCase;
using features::draftwallets::sandbox::usecases::RemoveInvitationUseCase;

class GroupSandboxSettingViewModel : public BaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QStringList, invitesList)
    DEFINE_QT_PROPERTY(QVariantList, invitesListJson)
    DEFINE_QT_PROPERTY(QString, groupId)
  public:
    explicit GroupSandboxSettingViewModel(QObject *parent = nullptr);

  protected:
    void onInit() override;

  public slots:
    void onAddInvitesClicked();
    void onRemoveInviteClicked(QString);
    void loadInvitations();

  private:
    GetGroupInvitationsUseCase m_getGroupInvitationsUseCase;
    RemoveInvitationUseCase m_removeInvitationUseCase;
};

} // namespace features::draftwallets::sandbox::viewmodels
