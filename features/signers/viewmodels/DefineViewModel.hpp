#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "core/enums/EnumMacros.h"
#include "RefreshDevicesViewModel.h"
#include "RecoverAnExistingSeedViewModel.h"
#include "WhichTypeOfKeySelectionViewModel.h"
#include "ColdcardRefreshDevicesViewModel.h"
#include "AddBlockstreamJadeViaQRViewModel.h"
#include "AddColdcardViaFileViewModel.h"
#include "AddHardwareExistingKeyViewModel.h"
#include "AddKeyBaseViewModel.h"
#include "AddSoftwareExistingKeyViewModel.h"
#include "AddSoftwareKeyViewModel.h"
#include "AddColdcardViaQRViewModel.h"
#include "RecoverViaXPRVViewModel.h"
#include "RecoverViaSeedViewModel.h"
#include "AddHardwareLoadingViewModel.h"

namespace features::signers::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Signers.ViewModels";

REGISTER_VIEWMODEL(RefreshDevicesViewModel)
REGISTER_VIEWMODEL(RecoverAnExistingSeedViewModel)
REGISTER_VIEWMODEL(WhichTypeOfKeySelectionViewModel)
REGISTER_VIEWMODEL(ColdcardRefreshDevicesViewModel)
REGISTER_VIEWMODEL(AddBlockstreamJadeViaQRViewModel)
REGISTER_VIEWMODEL(AddColdcardViaFileViewModel)
REGISTER_VIEWMODEL(AddHardwareExistingKeyViewModel)
REGISTER_VIEWMODEL(AddKeyBaseViewModel)
REGISTER_VIEWMODEL(AddSoftwareExistingKeyViewModel)
REGISTER_VIEWMODEL(AddSoftwareKeyViewModel)
REGISTER_VIEWMODEL(AddColdcardViaQRViewModel)
REGISTER_VIEWMODEL(RecoverViaXPRVViewModel)
REGISTER_VIEWMODEL(RecoverViaSeedViewModel)
REGISTER_VIEWMODEL(AddHardwareLoadingViewModel)


REGISTER_QML_ENUM_NS(FeatureOption)
REGISTER_QML_ENUM_NS(SignerKeyType)
REGISTER_QML_ENUM_NS(RecoveryOption)
}
} // namespace features::signers::viewmodels
    
