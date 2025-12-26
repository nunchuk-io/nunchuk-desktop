#pragma once

#define VIEWMODEL_MAJOR 1
#define VIEWMODEL_MINOR 0

// Registers a single ViewModel type
#define REGISTER_VIEWMODEL(T) \
    qmlRegisterType<T>(uri, VIEWMODEL_MAJOR, VIEWMODEL_MINOR, #T);
