#include "ServiceSetting.h"

ServiceSetting::ServiceSetting(QObject *parent) : QObject(parent)
{

}

ServiceSetting *ServiceSetting::instance() {
    static ServiceSetting mInstance;
    return &mInstance;
}
