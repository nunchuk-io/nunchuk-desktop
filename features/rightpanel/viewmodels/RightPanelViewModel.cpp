#include "RightPanelViewModel.h"
#include "core/ui/UiServices.inc"

namespace features::rightpanel::viewmodels {

RightPanelViewModel::RightPanelViewModel(QObject *parent) 
: BaseViewModel(parent) {
    
}

RightPanelViewModel::~RightPanelViewModel() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->unregisterObject();
}

void RightPanelViewModel::registerObject(QObject *object) {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->registerObject(object);
}

} // namespace features::rightpanel::viewmodels
