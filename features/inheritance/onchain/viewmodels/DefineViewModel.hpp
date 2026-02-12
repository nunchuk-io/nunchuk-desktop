#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "ViewInheritancePlanViewModel.h"

namespace features::inheritance::onchain::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Inheritance.OnChain.ViewModels";

REGISTER_VIEWMODEL(ViewInheritancePlanViewModel)

}
} // namespace features::inheritance::onchain::viewmodels
    
