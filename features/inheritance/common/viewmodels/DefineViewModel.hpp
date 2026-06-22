#pragma once
#include "CancelInheritancePlanViewModel.h"
#include "CreateInheritancePlanViewModel.h"
#include "InheritancePlanViewModel.h"
#include "UpdateInheritancePlanViewModel.h"
#include "core/viewmodel/DefineViewModelMacros.h"

namespace features::inheritance::common::viewmodels {
static inline void registerViewModels() {
    const char *uri = "Features.Inheritance.Common.ViewModels";

    REGISTER_VIEWMODEL(InheritancePlanViewModel)
    REGISTER_VIEWMODEL(UpdateInheritancePlanViewModel)
    REGISTER_VIEWMODEL(CancelInheritancePlanViewModel)
    REGISTER_VIEWMODEL(CreateInheritancePlanViewModel)
}
} // namespace features::inheritance::common::viewmodels
