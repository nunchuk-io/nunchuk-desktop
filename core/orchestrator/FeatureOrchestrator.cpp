#include "FeatureOrchestrator.h"

namespace core::orchestrator {
FeatureOrchestrator::FeatureOrchestrator(FlowManager *fm, QObject *parent) : QObject(parent), m_flowManager(fm) {
    connect(fm, &FlowManager::flowStarted, this, &FeatureOrchestrator::onFlowStarted);
}

void FeatureOrchestrator::onFlowStarted(BaseFlow* flow) {
    connect(flow, &core::flow::BaseFlow::finished,
            this, &FeatureOrchestrator::onFlowFinished);
}

void FeatureOrchestrator::initRules(const QVector<FlowRule> &rule) {
    m_rules = rule;
}
} // namespace core::orchestrator