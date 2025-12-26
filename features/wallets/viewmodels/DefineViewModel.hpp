#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "RegisterWalletOnHardwareViewModel.h"

namespace features::wallets::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.wallets.ViewModels";

REGISTER_VIEWMODEL(RegisterWalletOnHardwareViewModel)

}
} // namespace features::wallets::viewmodels
    
