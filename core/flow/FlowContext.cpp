#include "FlowContext.h"
#include "app/AppContext.h"
#include "core/ui/UiServices.inc"

namespace core::flow {
FlowContext::FlowContext(app::AppContext *ctx, QObject *parent) : QObject(parent), m_appCtx(ctx) {}

ScreenManager *FlowContext::screenManager() const {
    if (m_appCtx) {
        return m_appCtx->screenManager();
    }
    return nullptr;
}

ToastManager *FlowContext::toastManager() const {
    if (m_appCtx) {
        return m_appCtx->toastManager();
    }
    return nullptr;
}

PopupManager *FlowContext::popupManager() const {
    if (m_appCtx) {
        return m_appCtx->popupManager();
    }
    return nullptr;
}

SubScreenManager *FlowContext::subScreenManager() const {
    if (m_appCtx) {
        return m_appCtx->subScreenManager();
    }
    return nullptr;
}

AppModel *FlowContext::appModel() const {
    if (m_appCtx) {
        return m_appCtx->appModel();
    }
    return nullptr;
}

ServiceSetting *FlowContext::serviceSetting() const {
    if (m_appCtx) {
        return m_appCtx->serviceSetting();
    }
    return nullptr;
}

ClientController *FlowContext::clientController() const {
    if (m_appCtx) {
        return m_appCtx->clientController();
    }
    return nullptr;
}

} // namespace core::flow