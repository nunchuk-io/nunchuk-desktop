#include "JsonUtils.h"

namespace qUtils {

QVariantList compareAndSetChanges(const QVariantList &oldData, const QVariantList &newData) {
    QVariantList assetAllocationList;
    for (size_t i = 0; i < qMax(oldData.size(), newData.size()); ++i) {
        // Your logic for comparing or processing old and new asset allocations
        if (i < oldData.size() && i < newData.size()) {
            // Process old asset allocation at index i
            QVariantMap oldAllocation = oldData[i].toMap();
            QVariantMap newAllocation = newData[i].toMap();
            QVariantMap changes; // To store any changes between old and new allocations
            for (const QString &key : oldAllocation.keys()) {
                QVariant oldValue = oldAllocation.value(key);
                QVariant newValue = newAllocation.value(key);
                if (oldValue != newValue) {
                    // Handle the change in asset allocation for this key
                    changes.insert(key, newValue);
                    changes.insert(key + "Changed", true); // Mark this key as changed
                } else {
                    changes.insert(key, oldValue);          // No change, keep the old value
                    changes.insert(key + "Changed", false); // Mark this key as unchanged
                }
                // ... (your processing logic for each key)
            }
            assetAllocationList.append(changes);
        } else if (i < newData.size() && i >= oldData.size()) {
            // Handle the case where the new allocation has more entries than the old one
            QVariantMap newAllocation = newData[i].toMap();
            QVariantMap changes;
            for (const QString &key : newAllocation.keys()) {
                changes.insert(key, newAllocation.value(key));
                changes.insert(key + "Changed", true); // Mark as changed since it's not in the old allocation
            }
            assetAllocationList.append(changes);
        }
    }
    return assetAllocationList;
}

} // namespace qUtils
