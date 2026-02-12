// core/usecase/JsonConverter.h
#pragma once
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <typeinfo>

namespace core::usecase {

struct JsonConverter
{
    // ===== Primitive & Qt =====
    static QJsonValue toJsonValue(const QString& v) { return v; }
    static QJsonValue toJsonValue(const char* v)    { return QString(v); }
    static QJsonValue toJsonValue(int v)             { return v; }
    static QJsonValue toJsonValue(double v)          { return v; }
    static QJsonValue toJsonValue(bool v)            { return v; }

    static QJsonValue toJsonValue(const QJsonObject& v) { return v; }
    static QJsonValue toJsonValue(const QJsonArray& v)  { return v; }

    // ===== Containers =====
    template<typename T>
    static QJsonValue toJsonValue(const QList<T>& list)
    {
        QJsonArray arr;
        for (const auto& v : list)
            arr.append(toJsonValue(v));
        return arr;
    }

    template<typename T>
    static QJsonValue toJsonValue(const QVector<T>& list)
    {
        QJsonArray arr;
        for (const auto& v : list)
            arr.append(toJsonValue(v));
        return arr;
    }

    // ===== Custom type có toJson() =====
    template<typename T>
    static auto toJsonValue(const T& v)
        -> decltype(v.toJson(), QJsonValue{})
    {
        return v.toJson();
    }

    // ===== Fallback: NOT DEFINED =====
    template<typename T>
    static QJsonValue toJsonValue(const T&)
    {
        return QJsonValue(QJsonValue::Undefined);
    }
};

} // namespace core::usecase
