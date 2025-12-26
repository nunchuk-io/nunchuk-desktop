#include "BaseViewModel.h"
#include "core/ui/UiServices.inc"

namespace core::viewmodels {

void BaseViewModel::attachContext(ViewModelContext *ctx) {
    Q_ASSERT(ctx);
    Q_ASSERT(!m_ctx);

    m_ctx = ctx;
    baseConnectSignals();
    onInit();
    m_initialized = true;
}

void BaseViewModel::baseConnectSignals() {
    connect(this, &BaseViewModel::showToast, ctx()->toastManager(), &ToastManager::showToast);
}

void BaseViewModel::cancel() {
    ctx()->subScreenManager()->clear();
}

void BaseViewModel::back() {
    ctx()->subScreenManager()->back();
}

void BaseViewModel::next() {
    // Default implementation does nothing
}

void BaseViewModel::close() {
    ctx()->subScreenManager()->clear();
}

} // namespace core::viewmodels