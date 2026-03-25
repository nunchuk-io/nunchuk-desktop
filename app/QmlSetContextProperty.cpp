#include "AppRegister.h"
#include "AppContext.h"
#include <QQmlEngine>
#include "QEventProcessor.h"
#include <QMetaType>
#include "core/common/resources/AppStrings.h"

namespace app {
    void registerContextProperty() {
        QEventProcessor::instance()->setContextProperty("FlowManager", QVariant::fromValue(AppContext::instance()->flowManager()));
        QEventProcessor::instance()->setContextProperty("STR", QVariant::fromValue(&Strings));

        AppContext::instance()->subScreenManager()->onInitialized(QEventProcessor::instance()->getQuickWindow());
    }
}