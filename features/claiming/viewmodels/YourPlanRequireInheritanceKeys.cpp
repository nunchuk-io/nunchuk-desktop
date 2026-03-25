#include "YourPlanRequireInheritanceKeys.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
YourPlanRequireInheritanceKeys::YourPlanRequireInheritanceKeys(QObject *parent) : ActionViewModel(parent) {}

void YourPlanRequireInheritanceKeys::next() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::claiming::onchain::qprepareinheritancekey);
}

void YourPlanRequireInheritanceKeys::onAddFirstKeyClicked() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceoptionsproceed);
    close();
}
void YourPlanRequireInheritanceKeys::onAddSecondKeyClicked() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceoptionsproceed);
    close();
}

} // namespace features::claiming::viewmodels