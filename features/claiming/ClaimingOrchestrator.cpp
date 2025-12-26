#include "ClaimingOrchestrator.h"

namespace features::claiming {
ClaimingOrchestrator::ClaimingOrchestrator(FlowManager* fm, QObject* parent)
    : FeatureOrchestrator(fm, parent) {
}

void ClaimingOrchestrator::onFlowFinished(FlowResult result) {
    // Handle flow finished logic specific to claiming feature
}
} // namespace features::claiming