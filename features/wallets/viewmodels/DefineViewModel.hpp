#pragma once
#include "BBQRRegisterWalletOnColdcardViewModel.h"
#include "FileRegisterWalletOnColdcardViewModel.h"
#include "HomePendingAcceptedViewModel.h"
#include "RegisterWalletOnHardwareViewModel.h"
#include "ReplaceChangeOnChainTimeLockViewModel.h"
#include "ReplaceKeyOrChangeTimelockViewModel.h"
#include "SetupGroupWalletViewModel.h"
#include "WalletConfigViewModel.h"
#include "WalletListViewModel.h"
#include "core/viewmodel/DefineViewModelMacros.h"
namespace features::wallets::viewmodels {
static inline void registerViewModels() {
    const char *uri = "Features.Wallets.ViewModels";

    REGISTER_VIEWMODEL(RegisterWalletOnHardwareViewModel)
    REGISTER_VIEWMODEL(WalletListViewModel)
    REGISTER_VIEWMODEL(ReplaceKeyOrChangeTimelockViewModel)
    REGISTER_VIEWMODEL(ReplaceChangeOnChainTimeLockViewModel)
    REGISTER_VIEWMODEL(BBQRRegisterWalletOnColdcardViewModel)
    REGISTER_VIEWMODEL(FileRegisterWalletOnColdcardViewModel)
    REGISTER_VIEWMODEL(WalletConfigViewModel)
    REGISTER_VIEWMODEL(SetupGroupWalletViewModel)
    REGISTER_VIEWMODEL(HomePendingAcceptedViewModel)
}
} // namespace features::wallets::viewmodels
