#pragma once
#include "core/flow/BaseFlow.h"
#include <QMap>
#include <QObject>
#include <QString>
#include <memory>
#include <QTimer>

namespace app {
class AppContext;
}

namespace core::flow {

using app::AppContext;

class FlowManager : public QObject {
    Q_OBJECT
  public:
    explicit FlowManager(AppContext *ctx, QObject *parent = nullptr);

    BaseFlow *currentFlow() const;

    template <typename T, typename... Args> T *startFlow(Args &&...args);

    template <class T> void stopFlow();

    bool resumeFlow(const QString &id);

    void stopFlow(const QString &id);

    void stopAll();

    BaseFlow *getFlow(const QString &id) const;

  public slots:
    void bindViewModel(QObject *vm);
  private:
    void stopCurrentUiFlow();

  signals:
    void flowStarted(BaseFlow *flow);
    void flowStopped(const QString &id);

  private:
    QMap<QString, BaseFlow *> m_flows;
    AppContext *m_ctx;
    BaseFlow *m_currentUiFlow = nullptr;
    QTimer m_timeWatcher;
};

} // namespace core::flow
#include "FlowManager.inl"