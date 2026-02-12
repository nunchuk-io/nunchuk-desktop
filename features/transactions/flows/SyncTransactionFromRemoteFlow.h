#pragma once
#include "core/flow/BaseFlow.h"

namespace features::transactions::flows {
using core::flow::FlowContext;

class SyncTransactionFromRemoteFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit SyncTransactionFromRemoteFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "SyncTransactionFromRemoteFlow";
    }
};
} // namespace features::transactions::flows
