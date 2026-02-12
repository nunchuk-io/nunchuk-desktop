// features/auth/OnChainClaimingFlow.h
#pragma once
#include "core/flow/BaseFlow.h"

namespace features::claiming::flows {
using core::flow::FlowContext;
class OnChainClaimingFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit OnChainClaimingFlow(FlowContext* ctx, QObject *parent = nullptr);
    QString id() const override {
        return "OnChainClaimingFlow";
    }
    void passClaimingError(int code, const QString& message);
  public slots:
    void bind(QObject* vm) override;
  private:
    QString getTitleByErrorCode(int code);
  private:
    int m_errorCode{0};
    QString m_errorMessage;

};
} // namespace features::claiming::flows
