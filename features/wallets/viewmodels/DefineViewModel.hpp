#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "RegisterWalletOnHardwareViewModel.h"
#include "WalletListViewModel.h"
#include "ReplaceKeyOrChangeTimelockViewModel.h"
#include "ReplaceChangeOnChainTimeLockViewModel.h"
#include "BBQRRegisterWalletOnColdcardViewModel.h"
#include "FileRegisterWalletOnColdcardViewModel.h"
#include "WalletConfigViewModel.h"
namespace features::wallets::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Wallets.ViewModels";

REGISTER_VIEWMODEL(RegisterWalletOnHardwareViewModel)
REGISTER_VIEWMODEL(WalletListViewModel)
REGISTER_VIEWMODEL(ReplaceKeyOrChangeTimelockViewModel)
REGISTER_VIEWMODEL(ReplaceChangeOnChainTimeLockViewModel)
REGISTER_VIEWMODEL(BBQRRegisterWalletOnColdcardViewModel)
REGISTER_VIEWMODEL(FileRegisterWalletOnColdcardViewModel)
REGISTER_VIEWMODEL(WalletConfigViewModel)

}
} // namespace features::wallets::viewmodels
    
