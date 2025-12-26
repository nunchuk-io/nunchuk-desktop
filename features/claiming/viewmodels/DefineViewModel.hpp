#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "InheritanceUnlockedViewModel.h"
#include "WithdrawSelectWalletViewModel.h"

namespace features::claiming::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Claiming.ViewModels";

REGISTER_VIEWMODEL(InheritanceUnlockedViewModel)
REGISTER_VIEWMODEL(WithdrawSelectWalletViewModel)
}
} // namespace features::claiming::viewmodels
