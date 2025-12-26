// core/flow/BaseFlow.h
#pragma once
#include <QObject>
#include "FlowResult.h"
#include "FlowContext.h"

namespace core::flow {
class BaseFlow : public QObject {
    Q_OBJECT
  public:
    explicit BaseFlow(FlowContext* ctx, QObject *parent = nullptr);
    virtual ~BaseFlow() = default;
    virtual QString id() const = 0;
  public slots:
    virtual void bind(QObject* vm) {
        Q_UNUSED(vm)
    }
  signals:
    void finished(FlowResult event);
  protected:
    FlowContext *ctx() const {
        Q_ASSERT(m_ctx);
        return m_ctx;
    }
  private:
    FlowContext* m_ctx;
};
} // namespace core::flow
