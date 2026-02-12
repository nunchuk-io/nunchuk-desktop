#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "TimezoneViewModel.h"
#include "BaseGuideViewModel.h"
#include "ExportQrViewModel.h"

namespace features::common::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Common.ViewModels";

REGISTER_VIEWMODEL(TimezoneViewModel)
REGISTER_VIEWMODEL(BaseGuideViewModel)
REGISTER_VIEWMODEL(ExportQrViewModel)

}
} // namespace features::common::viewmodels