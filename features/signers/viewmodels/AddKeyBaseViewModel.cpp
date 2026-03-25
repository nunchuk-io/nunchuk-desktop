#include "AddKeyBaseViewModel.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/KeySetupFlow.h"
#include "generated_qml_keys.hpp"

namespace features::signers::viewmodels {
using namespace features::signers::flows;

AddKeyBaseViewModel::AddKeyBaseViewModel(QObject *parent) : BaseGuideViewModel(parent) {}

void AddKeyBaseViewModel::onInit() {}

void AddKeyBaseViewModel::cancel() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<KeySetupFlow>();
    flowMng->resumeFlow(flow->workFlowId());
    BaseGuideViewModel::cancel();
}

void AddKeyBaseViewModel::close() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<KeySetupFlow>();
    flowMng->resumeFlow(flow->workFlowId());
    BaseGuideViewModel::close();
}

} // namespace features::signers::viewmodels
