#pragma once
#include "core/flow/BaseFlow.h"
#include <QMap>
#include <QObject>
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

    BaseFlow *currentFlow() const;

    template <typename T, typename... Args> T *startFlow(Args &&...args);

    template <class T> void stopFlow();

    void stopFlow(const QString &id);

    void stopAll();

  private:
    void stopCurrentUiFlow();

  signals:
    void flowStarted(BaseFlow *flow);
    void flowStopped(const QString &id);
    void currentFlowChanged(BaseFlow *flow);

  private:
    QMap<QString, BaseFlow *> m_flows;
    AppContext *m_ctx;
    BaseFlow *m_currentUiFlow = nullptr;
};

} // namespace core::flow
#include "FlowManager.inl"