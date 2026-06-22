#pragma once
#include "AssetAllocationViewModel.h"
#include "BeneficiarySchedulesViewModel.h"
#include "DetailBeneficiaryViewModel.h"
#include "FallbackSettingsViewModel.h"
#include "FirstWithdrawalDateViewModel.h"
#include "ReleaseMethodViewModel.h"
#include "ReleaseScheduleViewModel.h"
#include "SetUpBufferPeriodViewModel.h"
#include "SetUpBufferPeriodWithdrawalViewModel.h"
#include "SetupAnOffChainTimelockViewModel.h"
#include "SignaturesRequiredViewModel.h"
#include "ViewInheritancePlanViewModel.h"
#include "WithdrawalRuleViewModel.h"
#include "core/viewmodel/DefineViewModelMacros.h"

namespace features::inheritance::offchain::viewmodels {
static inline void registerViewModels() {
    const char *uri = "Features.Inheritance.OffChain.ViewModels";

    REGISTER_VIEWMODEL(ViewInheritancePlanViewModel)
    REGISTER_VIEWMODEL(SetupAnOffChainTimelockViewModel)
    REGISTER_VIEWMODEL(AssetAllocationViewModel)
    REGISTER_VIEWMODEL(BeneficiarySchedulesViewModel)
    REGISTER_VIEWMODEL(DetailBeneficiaryViewModel)
    REGISTER_VIEWMODEL(FallbackSettingsViewModel)
    REGISTER_VIEWMODEL(FirstWithdrawalDateViewModel)
    REGISTER_VIEWMODEL(ReleaseMethodViewModel)
    REGISTER_VIEWMODEL(ReleaseScheduleViewModel)
    REGISTER_VIEWMODEL(SetUpBufferPeriodViewModel)
    REGISTER_VIEWMODEL(SetUpBufferPeriodWithdrawalViewModel)
    REGISTER_VIEWMODEL(SignaturesRequiredViewModel)
    REGISTER_VIEWMODEL(WithdrawalRuleViewModel)

    REGISTER_QML_ENUM_NS(FallbackOption)
}
} // namespace features::inheritance::offchain::viewmodels
