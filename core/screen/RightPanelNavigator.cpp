#include "RightPanelNavigator.h"
#include <QObject>
#include <QApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include "features/ScreenQmlDefine.hpp"

namespace core::screen {
#define JS_RIGHTPANEL_TRANSITION_FUNCTION "rightPanel_Transition" 
RightPanelNavigator::RightPanelNavigator(QObject *parent) : QObject(parent) {}

void RightPanelNavigator::request(const QString &screenId) {
    if (!m_currentScreenId.isEmpty()) {
        m_historyStack.push({m_currentScreenId, m_isTerminal});
    }
    m_currentScreenId = screenId;
    m_isTerminal = false;
    qmlSyncup();
}

void RightPanelNavigator::requestTerminal(const QString &screenId) {
    request(screenId);
    m_isTerminal = true;
}

void RightPanelNavigator::back() {
    if (m_historyStack.isEmpty()) {
        return;
    }
    auto entry = m_historyStack.pop();
    m_currentScreenId = entry.screenId;
    m_isTerminal = entry.isTerminal;
    qmlSyncup();
}

void RightPanelNavigator::reset() {
    m_currentScreenId.clear();
    m_historyStack.clear();
    m_isTerminal = false;
}

void RightPanelNavigator::registerObject(QObject *object) {
    if (!object)
        return;
    if (!m_registeredObjects.contains(object))
        m_registeredObjects.append(object);

    if (m_currentScreenId.isEmpty())
        return;
    qmlSyncup();
}

void RightPanelNavigator::unregisterObject() {
    // BUGFIX: used to also reset m_currentScreenId, so a host recreated mid-flow (e.g. the reactive
    // Loader hosting QServiceClaimAnInheritance rebuilding) would re-register with an empty screen id
    // and registerObject()'s guard below would skip resyncing - silently dropping whatever screen was
    // last requested. Keep m_currentScreenId so re-registration can replay it.
    m_registeredObjects.clear();
}

void RightPanelNavigator::qmlSyncup() {
    if (m_registeredObjects.isEmpty())
        return;
    QObject * rightPanel = m_registeredObjects.first();
    if (!rightPanel) {
        return;
    }
    QString qmlData;
    if (!m_currentScreenId.isEmpty()) {
        if (auto screen = qmlPathForId(m_currentScreenId); screen.has_value()) {
            qmlData = *screen;
        }
    }
    qDebug() << qmlData;
    QMetaObject::invokeMethod(rightPanel, JS_RIGHTPANEL_TRANSITION_FUNCTION, Q_ARG(QVariant, QVariant::fromValue(qmlData)));
}

} // namespace core::subscreen