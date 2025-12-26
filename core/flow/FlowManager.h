#pragma once
#include "core/flow/BaseFlow.h"
#include <QObject>
#include <QMap>
#include <QString>
#include <memory>

namespace app {
class AppContext;
}
namespace core::flow {
using app::AppContext;
class FlowManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(BaseFlow *currentFlow READ currentFlow NOTIFY currentFlowChanged)
  public:
    explicit FlowManager(AppContext *ctx, QObject *parent = nullptr);

    BaseFlow *currentFlow() const {
        return m_currentFlow;
    }

    template <typename T> T *startFlow() {
        FlowContext *flowCtx = new FlowContext(m_ctx);
        auto flow = new T(flowCtx, this);
        m_flows[flow->id()] = flow;
        m_currentFlow = flow;
        emit currentFlowChanged(m_currentFlow);
        emit flowStarted(flow);
        return flow;
    }

    void stopFlow(const QString &id);

  signals:
    void flowStarted(BaseFlow *flow);
    void flowStopped(const QString &id);
    void currentFlowChanged(BaseFlow *flow);

  private:
    QMap<QString, BaseFlow *> m_flows;
    AppContext *m_ctx;
    BaseFlow *m_currentFlow = nullptr;
};
} // namespace core::flow
