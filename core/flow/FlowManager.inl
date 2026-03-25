#pragma once

namespace core::flow {

inline FlowManager::FlowManager(AppContext *ctx, QObject *parent) : QObject(parent), m_ctx(ctx) {
    connect(&m_timeWatcher, &QTimer::timeout, this, [this]() {
        // Check for flows that should be stopped
        const auto keys = m_flows.keys();
        DBG_INFO << "FlowManager: Checking flows for timeout. Current flows: " << keys;
        if (m_currentUiFlow) {
            DBG_INFO << "FlowManager: Current UI Flow: " << m_currentUiFlow->id();
        }
    });
    m_timeWatcher.start(30*1000); // Check every second
}

inline BaseFlow *FlowManager::currentFlow() const {
    return m_currentUiFlow;
}

template <typename T, typename... Args> T *FlowManager::startFlow(Args &&...args) {
    static_assert(std::is_base_of_v<BaseFlow, T>, "T must inherit BaseFlow");

    // Create flow to get its id
    FlowContext *flowCtx = new FlowContext(m_ctx);
    auto *flow = new T(flowCtx, this, std::forward<Args>(args)...);
    const QString flowId = flow->id();

    // Check for existing flow
    if (m_flows.contains(flowId)) {
        delete flow;
        delete flowCtx;

        BaseFlow *existFlow = m_flows.value(flowId);

        // Background flow: return existing instance
        if (existFlow->isBackground()) {
            return qobject_cast<T *>(existFlow);
        }

        // UI flow: switch to existing if not already current
        if (existFlow != m_currentUiFlow) {
            m_currentUiFlow = existFlow;
        }
        return qobject_cast<T *>(existFlow);
    }

    // Set flowCtx parent to flow for automatic cleanup
    flowCtx->setParent(flow);

    m_flows.insert(flowId, flow);

    // Background flow
    if (flow->isBackground()) {
        flow->start();
        emit flowStarted(flow);
        return flow;
    }

    // UI flow
    m_currentUiFlow = flow;
    flow->start();
    emit flowStarted(flow);

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

inline bool FlowManager::resumeFlow(const QString &id) {
    auto it = m_flows.find(id);
    if (it == m_flows.end())
        return false;

    BaseFlow *flow = it.value();

    // Background flows don't need UI resume handling
    if (flow->isBackground())
        return true;

    // If already current, nothing to do
    if (m_currentUiFlow == flow)
        return true;

    // Switch current UI flow
    m_currentUiFlow = flow;
    return true;
}

inline void FlowManager::stopFlow(const QString &id) {
    if (!m_flows.contains(id))
        return;

    BaseFlow *flow = m_flows.take(id);
    flow->stop();
    if (flow == m_currentUiFlow) {
        m_currentUiFlow = nullptr;
    }

    emit flowStopped(id);
    flow->deleteLater();
}

inline void FlowManager::stopAll() {
    const auto keys = m_flows.keys();
    for (const auto &id : keys)
        stopFlow(id);
}

inline BaseFlow *FlowManager::getFlow(const QString &id) const {
    return m_flows.value(id, nullptr);
}

inline void FlowManager::bindViewModel(QObject *vm) {
    if (!m_currentUiFlow)
        return;

    m_currentUiFlow->bind(vm);
}

inline void FlowManager::stopCurrentUiFlow() {
    if (!m_currentUiFlow)
        return;

    stopFlow(m_currentUiFlow->id());
}

} // namespace core::flow