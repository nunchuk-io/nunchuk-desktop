#include "ToastManager.h"
#include <QUuid>
#include "AppModel.h"

namespace core::toast {
ToastManager::ToastManager(QObject* parent)
    : QObject(parent)
{
}

void ToastManager::show(const QString& source,
                        const QVariantMap& data,
                        int durationMs)
{
    const QString id = QUuid::createUuid().toString();

    QVariantMap toast;
    toast["id"] = id;
    toast["source"] = source;
    toast["data"] = data;

    m_toasts.append(toast);
    emit toastsChanged();

    if (durationMs > 0) {
        QTimer::singleShot(durationMs, this, [this, id]() {
            removeToast(id);
        });
    }
}

void ToastManager::removeToast(const QString& id)
{
    for (int i = 0; i < m_toasts.size(); ++i) {
        const auto map = m_toasts[i].toMap();
        if (map["id"].toString() == id) {
            m_toasts.removeAt(i);
            emit toastsChanged();
            return;
        }
    }
}

void ToastManager::clear()
{
    m_toasts.clear();
    emit toastsChanged();
}

QVariantList ToastManager::toasts() const
{
    return m_toasts;
}

void ToastManager::showToast(int code, const QString &what, EWARNING::WarningType type) {
    AppModel::instance()->showToast(code, what, type);
}

} // namespace core::toast
