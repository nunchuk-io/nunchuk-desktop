#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "RightPanelViewModel.h"
#include "ClaimInheritanceViewModel.h"

namespace features::rightpanel::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.RightPanel.ViewModels";

REGISTER_VIEWMODEL(RightPanelViewModel)
REGISTER_VIEWMODEL(ClaimInheritanceViewModel)

}
} // namespace features::rightpanel::viewmodels
    
