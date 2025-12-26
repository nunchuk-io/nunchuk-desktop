#include "AppOrchestrator.h"
#include "features/claiming/ClaimingOrchestrator.h"

namespace core::orchestrator {
AppOrchestrator::AppOrchestrator(FlowManager *fm, QObject *parent) : QObject(parent) {
    m_featureOrchestrators["claiming"] = new features::claiming::ClaimingOrchestrator(fm, this);
}
} // namespace core::orchestrator