#include "ExportCompletedViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
ExportCompletedViewModel::ExportCompletedViewModel(QObject *parent)
    : ActionViewModel(parent) {
}

void ExportCompletedViewModel::next() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::claiming::offchain::qverifyinheritancekey);
}
} // namespace features::claiming::viewmodels