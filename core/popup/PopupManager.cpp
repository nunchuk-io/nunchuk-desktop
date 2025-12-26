// core/popup/PopupManager.cpp
#include "PopupManager.h"

namespace core::popup {

PopupManager::PopupManager(QObject* parent)
    : QObject(parent) {}

void PopupManager::show(const QString& popupId,
                        const QVariantMap& data) {
    m_stack.push({popupId, data});
    emit popupChanged();
}

void PopupManager::close() {
    if (!m_stack.isEmpty()) {
        m_stack.pop();
        emit popupChanged();
    }
}

void PopupManager::clear() {
    m_stack.clear();
    emit popupChanged();
}

bool PopupManager::hasPopup() const {
    return !m_stack.isEmpty();
}

QString PopupManager::currentPopupId() const {
    return hasPopup() ? m_stack.top().popupId : QString{};
}

QVariantMap PopupManager::currentData() const {
    return hasPopup() ? m_stack.top().data : QVariantMap{};
}

} // namespace core::popup
//trong ViewModel
// connect(popupManager, &PopupManager::popupAction,
//     this, [](const QString& action, const QVariantMap&) {
// if (action == "ok") {
//     // do something
// }
// });