#include "RestoreSeedPhraseToHardwareDeviceViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;

RestoreSeedPhraseToHardwareDeviceViewModel::RestoreSeedPhraseToHardwareDeviceViewModel(QObject *parent) : ActionViewModel(parent) {}

void RestoreSeedPhraseToHardwareDeviceViewModel::next() {
    emit openHardwareAddKeyPopup();
}

void RestoreSeedPhraseToHardwareDeviceViewModel::startClaimInheritanceHardwareSetup(const QString &tag) {
    GUARD_SERVICE_SETTING()
    serviceSetting->requestSignerInitialClaimSetup(tag);
    serviceSetting->requestStartAddHardwareKey(tag);
}

} // namespace features::claiming::viewmodels
