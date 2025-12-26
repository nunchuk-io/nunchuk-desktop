#include "AppRegister.h"
#include "AppContext.h"
#include <QQmlEngine>
#include "QEventProcessor.h"
#include <QMetaType>

namespace app {
    void registerContextProperty() {
        QEventProcessor::instance()->setContextProperty("FlowManager", QVariant::fromValue(AppContext::instance()->flowManager()));
        QEventProcessor::instance()->setContextProperty("ScreenManager", QVariant::fromValue(AppContext::instance()->screenManager()));
        QEventProcessor::instance()->setContextProperty("AppOrchestrator", QVariant::fromValue(AppContext::instance()->orchestrator()));
        QEventProcessor::instance()->setContextProperty("PopupManager", QVariant::fromValue(AppContext::instance()->popupManager()));
        QEventProcessor::instance()->setContextProperty("ToastManager", QVariant::fromValue(AppContext::instance()->toastManager()));
        QEventProcessor::instance()->setContextProperty("SubScreenManager", QVariant::fromValue(AppContext::instance()->subScreenManager()));

        AppContext::instance()->subScreenManager()->onInitialized(QEventProcessor::instance()->getQuickWindow());
    }
}