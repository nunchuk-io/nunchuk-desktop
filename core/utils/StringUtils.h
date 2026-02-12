#pragma once

#include <QString>

namespace qUtils {
    inline bool isValidGroup(const QString &groupId) {
        return !groupId.isEmpty() && groupId.length() > 5;
    }
} // namespace qUtils