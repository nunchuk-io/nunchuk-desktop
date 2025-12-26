#include "BaseFlow.h"
#include <QQmlEngine>

namespace core::flow {
BaseFlow::BaseFlow(FlowContext* ctx, QObject *parent) : QObject(parent), m_ctx(ctx) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

} // namespace core::flow
