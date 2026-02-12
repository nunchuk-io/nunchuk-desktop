#include "AppStrings.h"

namespace core::common::resources {
AppStrings& AppStrings::instance()
{
    static AppStrings s_instance;
    return s_instance;
}

AppStrings::AppStrings(QObject *parent)
    : QObject(parent)
{
}
}  // namespace core::common::resources
