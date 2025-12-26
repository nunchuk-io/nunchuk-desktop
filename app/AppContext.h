// app/AppContext.h
#pragma once
#include <QObject>
#include <QHash>
#include <typeindex>

#include "core/ui/UiServices.inc"
#include "core/ui/UiForward.h"

namespace app {
using core::flow::FlowManager;
using core::screen::ScreenManager;
using core::orchestrator::AppOrchestrator;
using core::popup::PopupManager;
using core::toast::ToastManager;
using core::subscreen::SubScreenManager;
class AppContext : public QObject {
    Q_OBJECT
  public:
    explicit AppContext(QObject *parent = nullptr);
    static AppContext* instance();
    FlowManager *flowManager() const;
    ScreenManager *screenManager() const;
    AppOrchestrator *orchestrator() const;
    PopupManager *popupManager() const;
    ToastManager *toastManager() const;
    SubScreenManager *subScreenManager() const;
    AppModel *appModel() const;
    ServiceSetting *serviceSetting() const;

  private:
    ScreenManager *m_screenManager;
    FlowManager *m_flowManager;
    AppOrchestrator *m_orchestrator;
    PopupManager *m_popupManager;
    ToastManager *m_toastManager;
    SubScreenManager *m_subScreenManager;
    AppModel *m_appModel;
    ServiceSetting *m_serviceSetting;
};
} // namespace app
