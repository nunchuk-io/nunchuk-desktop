#include "RecoverInheritanceKeyViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;

RecoverInheritanceKeyViewModel::RecoverInheritanceKeyViewModel(QObject *parent) : ActionViewModel(parent) {
    setkey_option("hardware-device");
}

void RecoverInheritanceKeyViewModel::next() {
    GUARD_SUB_SCREEN_MANAGER()
    if (key_option() == "hardware-device") {
        subMng->show(qml::features::claiming::onchain::qrestoreseedphrasetohardwaredevice);
    } else {
        subMng->show(qml::screens::onlinemode::addhardwarekeys::qrecoveranexistingseed);
    }
}

} // namespace features::claiming::viewmodels
