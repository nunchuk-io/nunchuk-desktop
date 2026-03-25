#include "BaseFlow.h"
#include <QQmlEngine>
#include "core/ui/UiServices.inc"

namespace core::flow {
BaseFlow::BaseFlow(FlowContext* ctx, QObject *parent) : QObject(parent), m_ctx(ctx) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    baseConnectSignals();
}

void BaseFlow::baseConnectSignals() {
    connect(this, &BaseFlow::showToast, ctx()->toastManager(), &ToastManager::showToast);
}

} // namespace core::flow
