#pragma once

namespace core::flow {

inline FlowManager::FlowManager(AppContext *ctx, QObject *parent) : QObject(parent), m_ctx(ctx) {}

inline BaseFlow *FlowManager::currentFlow() const {
    return m_currentUiFlow;
}

template <typename T, typename... Args> T *FlowManager::startFlow(Args &&...args) {
    static_assert(std::is_base_of_v<BaseFlow, T>, "T must inherit BaseFlow");

    // tạo flow
    FlowContext *flowCtx = new FlowContext(m_ctx);
    auto *flow = new T(flowCtx, this, std::forward<Args>(args)...);
    const QString flowId = flow->id();

    // background flow: cho phép nhiều instance (id khác nhau)
    if (flow->isBackground()) {
        if (m_flows.contains(flowId)) {
            delete flow;
            delete flowCtx;
            return qobject_cast<T *>(m_flows.value(flowId));
        }

        m_flows.insert(flowId, flow);
        flow->start();
        emit flowStarted(flow);
        return flow;
    }

    // =========================
    // UI FLOW (DUY NHẤT 1 ACTIVE)
    // =========================
    stopCurrentUiFlow();

    m_flows.insert(flowId, flow);
    m_currentUiFlow = flow;

    flow->start();
    emit flowStarted(flow);
    emit currentFlowChanged(m_currentUiFlow);

    return flow;
}

template <class T> void FlowManager::stopFlow() {
    static_assert(std::is_base_of_v<BaseFlow, T>, "T must inherit BaseFlow");

    // tìm flow theo type
    for (auto it = m_flows.begin(); it != m_flows.end(); ++it) {
        if (qobject_cast<T *>(it.value())) {
            stopFlow(it.key());
            return;
        }
    }
}

inline void FlowManager::stopFlow(const QString &id) {
    if (!m_flows.contains(id))
        return;

    BaseFlow *flow = m_flows.take(id);
    flow->stop();

    if (flow == m_currentUiFlow) {
        m_currentUiFlow = nullptr;
        emit currentFlowChanged(nullptr);
    }

    emit flowStopped(id);
    flow->deleteLater();
}

inline void FlowManager::stopAll() {
    const auto keys = m_flows.keys();
    for (const auto &id : keys)
        stopFlow(id);
}

inline void FlowManager::stopCurrentUiFlow() {
    if (!m_currentUiFlow)
        return;

    stopFlow(m_currentUiFlow->id());
}

} // namespace core::flow