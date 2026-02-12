// features/auth/RegisterWalletOnHardwareFlow.h
#pragma once
#include "core/flow/BaseFlow.h"
#include "features/claiming/usecases/GetClaimingWalletUseCase.h"

namespace features::claiming::flows {
using core::flow::FlowContext;
using features::claiming::usecases::GetClaimingWalletUseCase;

class RegisterWalletOnHardwareFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit RegisterWalletOnHardwareFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "RegisterWalletOnHardwareFlow";
    }
  public slots:
    void bind(QObject* vm) override;

    void withdrawBitcoinClicked(QVariant msg);
    
  private:
    void requestWithdrawBitcoin(QVariant msg);

  private:
    int m_flowEvent {-1};
    GetClaimingWalletUseCase m_getClaimingWalletUC;
};
} // namespace features::claiming::flows
