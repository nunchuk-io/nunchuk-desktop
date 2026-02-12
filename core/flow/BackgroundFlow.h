// features/auth/BackgroundFlow.h
#pragma once
#include "core/flow/BaseFlow.h"
#include <QTimer>

namespace core::flow {
class BackgroundFlow : public BaseFlow {
    Q_OBJECT
  public:
    explicit BackgroundFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "BackgroundFlow";
    }

    bool isBackground() const override;
    void start() override;

    void stop() override;

  protected:
    virtual void sync() {}

  private:
    QTimer m_timer;
};
} // namespace core::flow
