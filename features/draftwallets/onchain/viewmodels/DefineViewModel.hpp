#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "LetConfigureYourWalletViewModel.h"
#include "SetupAnOnChainTimelockViewModel.h"

namespace features::draftwallets::onchain::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Draftwallets.OnChain.ViewModels";

REGISTER_VIEWMODEL(LetConfigureYourWalletViewModel)
REGISTER_VIEWMODEL(SetupAnOnChainTimelockViewModel)

}
} // namespace features::draftwallets::onchain::viewmodels
    
