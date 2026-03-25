#pragma once
#include "BackupPasswordViewModel.h"
#include "BufferPeriodHasStartedViewModel.h"
#include "ExportCompletedViewModel.h"
#include "InheritanceUnlockedViewModel.h"
#include "MagicPhraseViewModel.h"
#include "PrepareInheritanceKeyViewModel.h"
#include "ProceedOptionsViewModel.h"
#include "RecoverInheritanceKeyViewModel.h"
#include "RestoreSeedPhraseToHardwareDeviceViewModel.h"
#include "VerifyInheritanceKeyViewModel.h"
#include "WidthdrawToAddressViewModel.h"
#include "WithdrawACustomAmountViewModel.h"
#include "WithdrawBitcoinViewModel.h"
#include "WithdrawSelectWalletViewModel.h"
#include "YourPlanRequireInheritanceKeys.h"
#include "core/viewmodel/DefineViewModelMacros.h"

namespace features::claiming::viewmodels {
static inline void registerViewModels() {
    const char *uri = "Features.Claiming.ViewModels";

    REGISTER_VIEWMODEL(InheritanceUnlockedViewModel)
    REGISTER_VIEWMODEL(WithdrawSelectWalletViewModel)
    REGISTER_VIEWMODEL(BackupPasswordViewModel)
    REGISTER_VIEWMODEL(MagicPhraseViewModel)
    REGISTER_VIEWMODEL(ProceedOptionsViewModel)
    REGISTER_VIEWMODEL(VerifyInheritanceKeyViewModel)
    REGISTER_VIEWMODEL(ExportCompletedViewModel)
    REGISTER_VIEWMODEL(YourPlanRequireInheritanceKeys)
    REGISTER_VIEWMODEL(PrepareInheritanceKeyViewModel)
    REGISTER_VIEWMODEL(RecoverInheritanceKeyViewModel)
    REGISTER_VIEWMODEL(RestoreSeedPhraseToHardwareDeviceViewModel)
    REGISTER_VIEWMODEL(BufferPeriodHasStartedViewModel)
    REGISTER_VIEWMODEL(WithdrawBitcoinViewModel)
    REGISTER_VIEWMODEL(WidthdrawToAddressViewModel)
    REGISTER_VIEWMODEL(WithdrawACustomAmountViewModel)
}
} // namespace features::claiming::viewmodels
