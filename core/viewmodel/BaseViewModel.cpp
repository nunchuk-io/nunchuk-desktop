#include "BaseViewModel.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "core/ui/GuardMacros.h"
#include "core/ui/UiServices.inc"
#include <QTimer>

namespace core::viewmodels {

BaseViewModel::BaseViewModel(QObject *parent) : QObject(parent) {
    m_ctx = new ViewModelContext(this);
    QTimer::singleShot(0, this, [this]() { initialize(); });
}

void BaseViewModel::initialize() {
    GUARD_FLOW_MANAGER()
    flowMng->bindViewModel(this);
    baseConnectSignals();
    onInit();
    m_initialized = true;
}

void BaseViewModel::baseConnectSignals() {
    connect(this, &BaseViewModel::showToast, ctx()->toastManager(), &ToastManager::showToast);
}

ViewModelContext *BaseViewModel::ctx() const {
    Q_ASSERT(m_ctx);
    return m_ctx;
}

void BaseViewModel::cancel() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->clear();
}

void BaseViewModel::back() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->back();
}

void BaseViewModel::next() {
    // Default implementation does nothing
}

void BaseViewModel::close() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->clear();
}

void BaseViewModel::closeToWalletTab() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->clear();
    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
}

void BaseViewModel::closeToChatTab() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->clear();
    QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST);
}

void BaseViewModel::closeToServiceTab() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->clear();
    QEventProcessor::instance()->sendEvent(E::EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST);
}

} // namespace core::viewmodels