
// core/orchestrator/FeatureOrchestrator.h
#pragma once
#include <QObject>
#include <QVector>
#include "FlowRule.h"
#include "core/flow/FlowManager.h"
#include "core/flow/FlowResult.h"
#include "core/flow/BaseFlow.h"

namespace core::orchestrator {
    using namespace core::flow;
class FeatureOrchestrator : public QObject {
    Q_OBJECT
public:
    explicit FeatureOrchestrator(FlowManager* fm, QObject* parent = nullptr);
    void initRules(const QVector<FlowRule> &rule);
private slots:
    void onFlowStarted(BaseFlow* flow);
    virtual void onFlowFinished(FlowResult result) = 0;
    QVector<FlowRule> rules() const { return m_rules; }
    FlowManager* flowManager() const { return m_flowManager; }
private:
    FlowManager* m_flowManager;
    QVector<FlowRule> m_rules;
};
} // namespace core::orchestrator