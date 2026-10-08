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
        m_historyStack.push(m_currentScreenId);
    }
    m_currentScreenId = screenId;
    qmlSyncup();
}

void RightPanelNavigator::back() {
    if (m_historyStack.isEmpty()) {
        return;
    }
    m_currentScreenId = m_historyStack.pop();
    qmlSyncup();
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
    m_registeredObjects.clear();
    m_currentScreenId = "";
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