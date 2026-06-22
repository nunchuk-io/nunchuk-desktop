#pragma once
#include <QVariantList>

namespace qUtils {

QVariantList compareAndSetChanges(const QVariantList &oldData, const QVariantList &newData);

} // namespace qUtils