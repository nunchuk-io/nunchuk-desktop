#pragma once
#include "core/flow/BaseFlow.h"
#include "features/claiming/usecases/GetClaimingWalletUseCase.h"

namespace features::inheritance::offchain::flows {
using core::flow::FlowContext;

class SetupOffChainTimelockFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit SetupOffChainTimelockFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "SetupOffChainTimelockFlow";
    }
  public slots:
    void bind(QObject* vm) override;

    void passInheritanceInfo(const QString& date, const QString& timezone);
    void slotTimelockChanged();
    
  signals:
    void callbackTimelockChanged(const QString& date, const QString& timezone);

  private:
   QString m_date {};
   QString m_timezone {};
};
} // namespace features::inheritance::offchain::flows
