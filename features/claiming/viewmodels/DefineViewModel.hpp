#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "InheritanceUnlockedViewModel.h"
#include "WithdrawSelectWalletViewModel.h"
#include "MagicPhraseAndBackupPasswordViewModel.h"

namespace features::claiming::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Claiming.ViewModels";

REGISTER_VIEWMODEL(InheritanceUnlockedViewModel)
REGISTER_VIEWMODEL(WithdrawSelectWalletViewModel)
REGISTER_VIEWMODEL(MagicPhraseAndBackupPasswordViewModel)

}
} // namespace features::claiming::viewmodels
