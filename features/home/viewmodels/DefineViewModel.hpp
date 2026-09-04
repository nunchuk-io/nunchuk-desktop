#pragma once

#include "HomeReminderViewModel.h"
#include <QJSEngine>
#include <QQmlEngine>

namespace features::home::viewmodels {

static inline void registerViewModels() {
    const char *uri = "Features.Home.ViewModels";
    qmlRegisterSingletonType<HomeReminderViewModel>(
        uri, 1, 0, "HomeReminderViewModel",
        [](QQmlEngine *engine, QJSEngine *) -> QObject * { return new HomeReminderViewModel(engine); });
}

} // namespace features::home::viewmodels
