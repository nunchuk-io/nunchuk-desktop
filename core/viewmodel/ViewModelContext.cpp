#include "ViewModelContext.h"
#include "app/AppContext.h"
#include "core/ui/UiServices.inc"

namespace core::viewmodels {
ViewModelContext::ViewModelContext(QObject *parent)
    : QObject(parent), m_appCtx(app::AppContext::instance()) {
}
ScreenManager *ViewModelContext::screenManager() const {
    if (m_appCtx) {
        return m_appCtx->screenManager();
    }
    return nullptr;
}

ToastManager *ViewModelContext::toastManager() const {
    if (m_appCtx) {
        return m_appCtx->toastManager();
    }
    return nullptr;
}

PopupManager *ViewModelContext::popupManager() const {
    if (m_appCtx) {
        return m_appCtx->popupManager();
    }
    return nullptr;
}

SubScreenManager *ViewModelContext::subScreenManager() const {
    if (m_appCtx) {
        return m_appCtx->subScreenManager();
    }
    return nullptr;
}

AppModel *ViewModelContext::appModel() const {
    if (m_appCtx) {
        return m_appCtx->appModel();
    }
    return nullptr;
}

ServiceSetting *ViewModelContext::serviceSetting() const {
    if (m_appCtx) {
        return m_appCtx->serviceSetting();
    }
    return nullptr;
}

FlowManager *ViewModelContext::flowManager() const {
    if (m_appCtx) {
        return m_appCtx->flowManager();
    }
    return nullptr;
}

QGroupWallets *ViewModelContext::groupWallets() const {
    if (m_appCtx) {
        return m_appCtx->groupWallets();
    }
    return nullptr;
}

} // namespace core::viewmodels