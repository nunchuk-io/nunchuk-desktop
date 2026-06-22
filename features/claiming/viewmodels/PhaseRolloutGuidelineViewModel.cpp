#include "PhaseRolloutGuidelineViewModel.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/flows/ClaimingFlow.h"
#include "generated_qml_keys.hpp"
#include <QLocale>

namespace features::claiming::viewmodels {
using namespace core::viewmodels;

PhaseRolloutGuidelineViewModel::PhaseRolloutGuidelineViewModel(QObject *parent) : ActionViewModel(parent) {}

void PhaseRolloutGuidelineViewModel::onContinueClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::components::rightpannel::service::common::qserviceclaiminheritancereleaseschedule);
}

void PhaseRolloutGuidelineViewModel::back() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->back();
}

} // namespace features::claiming::viewmodels
