#pragma once
#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/draftwallets/sandbox/usecases/SendSharedWalletInvitationUseCase.h"
#include <QVariant>

namespace features::draftwallets::sandbox::viewmodels {
using core::viewmodels::BaseViewModel;
using features::draftwallets::sandbox::usecases::SendSharedWalletInvitationUseCase;

class GroupSandboxInvitesViewModel : public BaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QStringList, invitesList)
  public:
    explicit GroupSandboxInvitesViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(QString, groupId)

  protected:
    void onInit() override;

  public slots:
    void onSendInviteClicked();

  private:
    SendSharedWalletInvitationUseCase m_sendSharedWalletInvitationUseCase;
};

} // namespace features::draftwallets::sandbox::viewmodels
