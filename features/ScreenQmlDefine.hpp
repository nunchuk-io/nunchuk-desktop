#pragma once
#include <QString>
#include <optional>
#include <algorithm>
#include <QHash>
#include "generated_qml_registry.hpp"
#include "generated_qml_keys.hpp"

// Provide forward declaration in case the registry is defined elsewhere
// The registry maps screen ids to QML paths.
extern const QHash<QString, QString> QML_REGISTRY;

// Fast lookup of QML path by screen id using the global registry map.
// Returns std::nullopt if id is not present.
inline std::optional<QString> qmlPathForId(const QString& id) {
    // QHash lookup is average O(1), suitable for large registries.
    auto it = QML_REGISTRY.constFind(id);
    if (it == QML_REGISTRY.constEnd()) {
        return std::nullopt;
    }
    return *it;
}