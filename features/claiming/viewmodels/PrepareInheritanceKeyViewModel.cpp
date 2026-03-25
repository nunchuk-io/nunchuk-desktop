#include "PrepareInheritanceKeyViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;

PrepareInheritanceKeyViewModel::PrepareInheritanceKeyViewModel(QObject *parent) : ActionViewModel(parent) {
    setkey_option("hardware-device");
}

void PrepareInheritanceKeyViewModel::next() {
    if (key_option() == "hardware-device") {
        emit openHardwareAddKeyPopup();
    } else {
        GUARD_SUB_SCREEN_MANAGER()
        subMng->show(qml::features::claiming::onchain::qrecoverinheritancekey);
    }
}

void PrepareInheritanceKeyViewModel::startClaimInheritanceHardwareSetup(const QString &tag) {
    GUARD_SERVICE_SETTING()
    serviceSetting->requestSignerInitialClaimSetup(tag);
    serviceSetting->requestStartAddHardwareKey(tag);
}

} // namespace features::claiming::viewmodels
