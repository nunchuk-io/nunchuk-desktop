#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "RefreshDevicesViewModel.h"
#include "RecoverAnExistingSeedViewModel.h"

namespace features::signers::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Signers.ViewModels";

REGISTER_VIEWMODEL(RefreshDevicesViewModel)
REGISTER_VIEWMODEL(RecoverAnExistingSeedViewModel)

}
} // namespace features::signers::viewmodels
    
