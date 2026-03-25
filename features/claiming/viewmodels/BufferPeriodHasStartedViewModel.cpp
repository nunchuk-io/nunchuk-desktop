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
    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceyourinheritance);
}
} // namespace features::claiming::viewmodels