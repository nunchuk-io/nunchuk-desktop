#include "AppStrings.h"

namespace core::common::resources {
AppStrings& AppStrings::instance()
{
    // QSTR is exposed to QML as a non-owning context property. Keep the
    // immutable string catalog alive for the whole process so it cannot be
    // destroyed while the QML engine is still evaluating bindings.
    static AppStrings* const s_instance = [] {
        auto* strings = new AppStrings;
        QQmlEngine::setObjectOwnership(strings, QQmlEngine::CppOwnership);
        return strings;
    }();
    return *s_instance;
}

AppStrings::AppStrings(QObject *parent)
    : QObject(parent)
{
}
}  // namespace core::common::resources
