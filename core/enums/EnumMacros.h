// EnumMacros.h
#pragma once
#include <QObject>

#define DECLARE_QML_ENUM(NS, ENUM, ...) \
namespace NS {                         \
    Q_NAMESPACE                        \
    enum ENUM { __VA_ARGS__ };         \
    Q_ENUM_NS(ENUM)                    \
}

