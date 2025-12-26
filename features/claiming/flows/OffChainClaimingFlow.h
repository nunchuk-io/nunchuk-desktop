// features/auth/OffChainClaimingFlow.h
#pragma once
#include "core/flow/BaseFlow.h"

namespace features::claiming::flows {
using core::flow::FlowContext;
class OffChainClaimingFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit OffChainClaimingFlow(FlowContext* ctx, QObject *parent = nullptr);
    QString id() const override {
        return "OffChainClaimingFlow";
    }
};
} // namespace features::claiming::flows
