#include "ColdcardRefreshDevicesViewModel.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::signers::viewmodels {

ColdcardRefreshDevicesViewModel::ColdcardRefreshDevicesViewModel(QObject *parent) 
: HardwareRefreshDevicesViewModel(parent) {
}

void ColdcardRefreshDevicesViewModel::onViaFileClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    DBG_INFO << "ColdcardRefreshDevicesViewModel::onViaFileClicked " << setupOption();
    switch(setupOption()) {
        case FeatureOption::ClaimOffChain:
            subMng->show(qml::features::signers::offchain::qaddcoldcardguideviafile);
            break;
        default:
            break;
    }
}

void ColdcardRefreshDevicesViewModel::onViaQRClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    switch(setupOption()) {
        case FeatureOption::ClaimOffChain:
            subMng->show(qml::features::signers::offchain::qaddcoldcardguideviaqr);
            break;
        default:
            break;
    }
}

void ColdcardRefreshDevicesViewModel::onViaUSBClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    switch(setupOption()) {
        case FeatureOption::ClaimOffChain:
            // subMng->show(qml::features::signers::qaddcoldcardguideviausb);
            break;
        default:
            break;
    }
}

} // namespace features::signers::viewmodels
