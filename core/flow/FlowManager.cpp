#include "FlowManager.h"

namespace core::flow {

FlowManager::FlowManager(AppContext* ctx, QObject *parent) : QObject(parent), m_ctx(ctx) {}

void FlowManager::stopFlow(const QString &id) {
    if (!m_flows.contains(id))
        return;
    delete m_flows.take(id);
    emit flowStopped(id);
}
} // namespace core::flow