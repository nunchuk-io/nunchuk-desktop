// app/AppContext.cpp
#include "AppContext.h"
#include <QQmlEngine>

namespace app {
AppContext* AppContext::instance() {
    static AppContext instance;
    return &instance;
}
AppContext::AppContext(QObject *parent) 
: QObject(parent)
, m_screenManager(new ScreenManager(this))
, m_flowManager(new FlowManager(this))
, m_orchestrator(new AppOrchestrator(m_flowManager, this))
, m_popupManager(new PopupManager(this))
, m_toastManager(new ToastManager(this))
, m_subScreenManager(new SubScreenManager(this))
, m_appModel(AppModel::instance())
, m_serviceSetting(ServiceSetting::instance())
, m_clientController(ClientController::instance())
, m_groupWallets(QGroupWallets::instance())
 {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

FlowManager *AppContext::flowManager() const {
    return m_flowManager;
}

ScreenManager *AppContext::screenManager() const {
    return m_screenManager;
}

AppOrchestrator *AppContext::orchestrator() const {
    return m_orchestrator;
}

PopupManager *AppContext::popupManager() const {
    return m_popupManager;
}

ToastManager *AppContext::toastManager() const {
    return m_toastManager;
}

SubScreenManager *AppContext::subScreenManager() const {
    return m_subScreenManager;
}

AppModel *AppContext::appModel() const {
    return m_appModel;
}

ServiceSetting *AppContext::serviceSetting() const {
    return m_serviceSetting;
}

ClientController *AppContext::clientController() const {
    return m_clientController;
}

QGroupWallets *AppContext::groupWallets() const {
    return m_groupWallets;
}

} // namespace app
