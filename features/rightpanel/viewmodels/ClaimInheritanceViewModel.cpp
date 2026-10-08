#include "ClaimInheritanceViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::rightpanel::viewmodels {

ClaimInheritanceViewModel::ClaimInheritanceViewModel(QObject *parent) : BaseViewModel(parent) {}

void ClaimInheritanceViewModel::onInit() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceinputmagicphrase);
}

} // namespace features::rightpanel::viewmodels
