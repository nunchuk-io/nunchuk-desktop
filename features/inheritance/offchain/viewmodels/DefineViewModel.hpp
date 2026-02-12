#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "ViewInheritancePlanViewModel.h"
#include "SetupAnOffChainTimelockViewModel.h"

namespace features::inheritance::offchain::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Inheritance.OffChain.ViewModels";

REGISTER_VIEWMODEL(ViewInheritancePlanViewModel)
REGISTER_VIEWMODEL(SetupAnOffChainTimelockViewModel)

}
} // namespace features::inheritance::offchain::viewmodels
    
