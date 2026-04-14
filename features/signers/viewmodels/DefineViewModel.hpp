#pragma once
#include "AddAKeyViewModel.h"
#include "AddBlockstreamJadeViaQRViewModel.h"
#include "AddColdcardViaFileViewModel.h"
#include "AddColdcardViaQRViewModel.h"
#include "AddHardwareExistingKeyViewModel.h"
#include "AddHardwareLoadingViewModel.h"
#include "AddKeyBaseViewModel.h"
#include "AddSoftwareExistingKeyViewModel.h"
#include "AddSoftwareKeyViewModel.h"
#include "ColdcardRefreshDevicesViewModel.h"
#include "EditPlatformKeyPoliciesViewModel.h"
#include "GlobalPlatformKeyPoliciesViewModel.h"
#include "PerKeyPlatformKeyPoliciesViewModel.h"
#include "PlatformKeyPoliciesViewModel.h"
#include "RecoverAnExistingSeedViewModel.h"
#include "RecoverViaSeedViewModel.h"
#include "RecoverViaXPRVViewModel.h"
#include "RefreshDevicesViewModel.h"
#include "WhichTypeOfKeySelectionViewModel.h"
#include "core/enums/EnumMacros.h"
#include "core/viewmodel/DefineViewModelMacros.h"

namespace features::signers::viewmodels {
static inline void registerViewModels() {
    const char *uri = "Features.Signers.ViewModels";

    REGISTER_VIEWMODEL(RefreshDevicesViewModel)
    REGISTER_VIEWMODEL(RecoverAnExistingSeedViewModel)
    REGISTER_VIEWMODEL(WhichTypeOfKeySelectionViewModel)
    REGISTER_VIEWMODEL(ColdcardRefreshDevicesViewModel)
    REGISTER_VIEWMODEL(AddBlockstreamJadeViaQRViewModel)
    REGISTER_VIEWMODEL(AddColdcardViaFileViewModel)
    REGISTER_VIEWMODEL(AddHardwareExistingKeyViewModel)
    REGISTER_VIEWMODEL(AddKeyBaseViewModel)
    REGISTER_VIEWMODEL(AddAKeyViewModel)
    REGISTER_VIEWMODEL(AddSoftwareExistingKeyViewModel)
    REGISTER_VIEWMODEL(AddSoftwareKeyViewModel)
    REGISTER_VIEWMODEL(AddColdcardViaQRViewModel)
    REGISTER_VIEWMODEL(EditPlatformKeyPoliciesViewModel)
    REGISTER_VIEWMODEL(GlobalPlatformKeyPoliciesViewModel)
    REGISTER_VIEWMODEL(PerKeyPlatformKeyPoliciesViewModel)
    REGISTER_VIEWMODEL(PlatformKeyPoliciesViewModel)
    REGISTER_VIEWMODEL(RecoverViaXPRVViewModel)
    REGISTER_VIEWMODEL(RecoverViaSeedViewModel)
    REGISTER_VIEWMODEL(AddHardwareLoadingViewModel)

    REGISTER_QML_ENUM_NS(FeatureOption)
    REGISTER_QML_ENUM_NS(SignerKeyType)
    REGISTER_QML_ENUM_NS(RecoveryOption)
}
} // namespace features::signers::viewmodels
