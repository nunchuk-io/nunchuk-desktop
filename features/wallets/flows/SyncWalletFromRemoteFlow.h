
#pragma once
#include "core/flow/BackgroundFlow.h"
#include "features/wallets/usecases/SyncWalletFromRemoteUseCase.h"
#include <QTimer>

namespace features::wallets::flows {
using core::flow::FlowContext;
using features::wallets::usecases::SyncWalletFromRemoteUseCase;

class SyncWalletFromRemoteFlow : public core::flow::BackgroundFlow {
    Q_OBJECT
  public:
    explicit SyncWalletFromRemoteFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "SyncWalletFromRemoteFlow";
    }
    void sync() override;
  private:
    SyncWalletFromRemoteUseCase m_syncWalletFromRemoteUC;
};
} // namespace features::wallets::flows
