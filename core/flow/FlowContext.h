#pragma once

#include <QObject>
#include "core/ui/UiForward.h"

namespace core::flow {
using app::AppContext;
using core::screen::ScreenManager;
using core::popup::PopupManager;
using core::toast::ToastManager;
using core::subscreen::SubScreenManager;
class FlowContext : public QObject {
    Q_OBJECT
  public:
    explicit FlowContext(AppContext *ctx, QObject *parent = nullptr);
    ScreenManager *screenManager() const;
    ToastManager *toastManager() const;
    PopupManager *popupManager() const;
    SubScreenManager *subScreenManager() const;
    AppModel *appModel() const;
    ServiceSetting *serviceSetting() const;

  private:
    AppContext *m_appCtx{nullptr};
};
} // namespace core::flow