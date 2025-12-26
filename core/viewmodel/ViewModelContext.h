#pragma once
#include <QObject>
#include "core/ui/UiForward.h"

namespace core::viewmodels {
using app::AppContext;
using core::popup::PopupManager;
using core::screen::ScreenManager;
using core::toast::ToastManager;
using core::subscreen::SubScreenManager;
using core::flow::FlowManager;
class ViewModelContext : public QObject {
    Q_OBJECT
  public:
    explicit ViewModelContext(QObject *parent = nullptr);
    ScreenManager *screenManager() const;
    ToastManager *toastManager() const;
    PopupManager *popupManager() const;
    SubScreenManager *subScreenManager() const;
    AppModel *appModel() const;
    ServiceSetting *serviceSetting() const;
    FlowManager *flowManager() const;

  private:
    AppContext *m_appCtx{nullptr};
};
} // namespace core::viewmodels