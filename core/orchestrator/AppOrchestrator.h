
// core/orchestrator/AppOrchestrator.h
#pragma once
#include <QObject>
#include <QMap>
#include "FeatureOrchestrator.h"
#include "core/flow/FlowManager.h"

namespace core::orchestrator {
using core::flow::FlowManager;
class AppOrchestrator : public QObject {
    Q_OBJECT
public:
    explicit AppOrchestrator(FlowManager* fm, QObject* parent = nullptr);
private:
    QMap<QString, FeatureOrchestrator*> m_featureOrchestrators;
};
} // namespace app