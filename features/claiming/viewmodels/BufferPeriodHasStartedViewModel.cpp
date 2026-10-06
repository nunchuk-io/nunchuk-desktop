#include "BufferPeriodHasStartedViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
BufferPeriodHasStartedViewModel::BufferPeriodHasStartedViewModel(QObject *parent)
    : ActionViewModel(parent) {
}

void BufferPeriodHasStartedViewModel::next() {
    GUARD_RIGHT_PANEL_NAV()
    // Returning to the claim's home/result screen - terminal again, same as reaching it the first time.
    rightPanel->requestTerminal(qml::components::rightpannel::service::common::qserviceclaiminheritanceyourinheritance);
}
} // namespace features::claiming::viewmodels