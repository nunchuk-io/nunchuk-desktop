#pragma once
#include "GroupSandboxInvitesViewModel.h"
#include "GroupSandboxSettingViewModel.h"
#include "core/viewmodel/DefineViewModelMacros.h"

namespace features::draftwallets::sandbox::viewmodels {
static inline void registerViewModels() {
    const char *uri = "Features.Draftwallets.Sandbox.ViewModels";

    REGISTER_VIEWMODEL(GroupSandboxInvitesViewModel)
    REGISTER_VIEWMODEL(GroupSandboxSettingViewModel)
}
} // namespace features::draftwallets::sandbox::viewmodels
