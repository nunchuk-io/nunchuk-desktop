// core/orchestrator/FlowRule.h
#pragma once
#include <QString>
#include "core/flow/FlowResult.h"

namespace core::flow {
struct FlowRule {
    QString from;
    FlowResult on;
    QString to;
};
} // namespace core::flow
