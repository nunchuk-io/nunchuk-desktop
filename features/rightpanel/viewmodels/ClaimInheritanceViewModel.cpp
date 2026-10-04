#include "ClaimInheritanceViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::rightpanel::viewmodels {

ClaimInheritanceViewModel::ClaimInheritanceViewModel(QObject *parent) : BaseViewModel(parent) {}

void ClaimInheritanceViewModel::onInit() {
    GUARD_RIGHT_PANEL_NAV()
    // BUGFIX: used to request the magic-phrase screen unconditionally, even if this host was just
    // recreated mid-flow (e.g. the reactive Loader above it rebuilding) - clobbering whatever screen
    // the user had already navigated to (e.g. "proceed options" after "Add inheritance keys"). Only
    // start fresh if there's no screen already in progress.
    if (!rightPanel->hasActiveScreen()) {
        rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceinputmagicphrase);
    }
}

} // namespace features::rightpanel::viewmodels
