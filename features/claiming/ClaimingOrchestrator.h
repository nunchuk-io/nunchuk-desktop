#pragma once
#include <QObject>
#include "core/orchestrator/FeatureOrchestrator.h"

namespace features::claiming {
using core::flow::FlowManager;
using core::flow::FlowResult;
using core::orchestrator::FeatureOrchestrator; // use the correctly namespaced base class

class ClaimingOrchestrator : public FeatureOrchestrator {
    Q_OBJECT
  public:
    explicit ClaimingOrchestrator(FlowManager* fm, QObject* parent = nullptr);

  private slots:
    void onFlowFinished(FlowResult result) override;
};
} // namespace features::claiming