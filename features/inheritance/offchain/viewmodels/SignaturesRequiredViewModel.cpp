#include "SignaturesRequiredViewModel.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;

SignaturesRequiredViewModel::SignaturesRequiredViewModel(QObject *parent) : SignaturesRequiredBaseViewModel(parent) {
    setamountLimit(qUtils::currencyLocale(10000));
    setpending_signatures(0);
    setdescribeOne(Strings.STR_QML_1012());
    setdescribeTwo(Strings.STR_QML_1013());
}

void SignaturesRequiredViewModel::onInit() {
    // Initialize view model
}

} // namespace features::inheritance::offchain::viewmodels
