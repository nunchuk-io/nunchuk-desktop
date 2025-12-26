#pragma once
#include <QJsonObject>
#include <QJsonArray>
#include "JsonConverter.h"

#define UC_DECLARE_FIELD(type, name) type name;
#define UC_JSON_FIELD(type, name) \
    obj[#name] = core::usecase::JsonConverter::toJsonValue(name);
