// features/auth/KeySetupFlow.h
#pragma once
#include "core/flow/BaseFlow.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include <nunchuk.h>

namespace features::signers::flows {
using core::flow::FlowContext;
class KeySetupFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit KeySetupFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "KeySetupFlow";
    }
    DEFINE_SET_GET(QString, workFlowId)
    DEFINE_SET_GET(int, keyType)
    DEFINE_SET_GET(QString, keyName)
    DEFINE_SET_GET(QString, keyTag)
    DEFINE_SET_GET(QString, keyXfp)
    DEFINE_SET_GET(int, setupOption)

    DEFINE_SET_GET(nunchuk::Device, device)
  public slots:
    void bind(QObject *vm) override;
  signals:
    void signalProceedAddSigner(int progress);
};
} // namespace features::signers::flows
