// core/usecase/JsonConverter.h
#pragma once
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMap>
#include <typeinfo>

namespace core::usecase {

struct JsonConverter {
    // ===== Primitive & Qt =====
    static QJsonValue toJsonValue(const QString &v) {
        return v;
    }
    static QJsonValue toJsonValue(const char *v) {
        return QString(v);
    }
    static QJsonValue toJsonValue(int v) {
        return v;
    }
    static QJsonValue toJsonValue(double v) {
        return v;
    }
    static QJsonValue toJsonValue(bool v) {
        return v;
    }
    static QJsonValue toJsonValue(const QStringList &list) {
        QJsonArray arr;
        for (const auto &v : list)
            arr.append(toJsonValue(v));
        return arr;
    }

    static QJsonValue toJsonValue(const QJsonObject &v) {
        return v;
    }
    static QJsonValue toJsonValue(const QJsonArray &v) {
        return v;
    }

    // ===== Containers =====
    template <typename T> static QJsonValue toJsonValue(const QList<T> &list) {
        QJsonArray arr;
        for (const auto &v : list)
            arr.append(toJsonValue(v));
        return arr;
    }

    template <typename T> static QJsonValue toJsonValue(const QVector<T> &list) {
        QJsonArray arr;
        for (const auto &v : list)
            arr.append(toJsonValue(v));
        return arr;
    }

    static QJsonValue toJsonValue(const QMap<QString, QString> &map) {
        QJsonObject obj;
        for (auto it = map.constBegin(); it != map.constEnd(); ++it)
            obj.insert(it.key(), toJsonValue(it.value()));
        return obj;
    }

    template <typename V> static QJsonValue toJsonValue(const QMap<QString, V> &map) {
        QJsonObject obj;
        for (auto it = map.constBegin(); it != map.constEnd(); ++it)
            obj.insert(it.key(), toJsonValue(it.value()));
        return obj;
    }

    template <typename K, typename V> static QJsonValue toJsonValue(const QMap<K, V> &map) {
        QJsonArray arr;
        for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
            QJsonObject item;
            item.insert("key", toJsonValue(it.key()));
            item.insert("value", toJsonValue(it.value()));
            arr.append(item);
        }
        return arr;
    }

    // ===== Custom type có toJson() =====
    template <typename T> static auto toJsonValue(const T &v) -> decltype(v.toJson(), QJsonValue{}) {
        return v.toJson();
    }

    // ===== Fallback: NOT DEFINED =====
    template <typename T> static QJsonValue toJsonValue(const T &) {
        return QJsonValue(QJsonValue::Undefined);
    }
};

} // namespace core::usecase
