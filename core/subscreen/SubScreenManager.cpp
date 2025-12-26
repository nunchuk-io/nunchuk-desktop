#include "SubScreenManager.h"
#include <QObject>
#include <QApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include "features/ScreenQmlDefine.hpp"

namespace core::subscreen {
#define JS_SUBSCREEN_TRANSITION_FUNCTION "subScreen_Transition" 
SubScreenManager::SubScreenManager(QObject *parent) : QObject(parent) {}

void SubScreenManager::show(const QString &screenId) {
    m_stack.push_back({screenId});
    qmlSyncup();
}

void SubScreenManager::replace(const QString &screenId) {
    if (!m_stack.isEmpty())
        m_stack.pop_back();

    m_stack.push_back({screenId});
    qmlSyncup();
}

void SubScreenManager::clearAndShow(const QString &screenId) {
    m_stack.clear();
    m_stack.push_back({screenId});
    qmlSyncup();
}

void SubScreenManager::back() {
    if (m_stack.isEmpty())
        return;

    m_stack.pop_back();
    qmlSyncup();
}

void SubScreenManager::clear() {
    if (m_stack.isEmpty())
        return;

    m_stack.clear();
    qmlSyncup();
}

void SubScreenManager::onInitialized(QQuickView *view) {
    m_viewer = view;
}

void SubScreenManager::qmlSyncup() {
    if (!m_viewer)
        return;
    QObject * rootObject = m_viewer->rootObject();
    if (!rootObject) {
        return;
    }
    QObject *subScreenHost = rootObject->findChild<QObject*>("subScreenHost");
    if (!subScreenHost) {
        // DBG_WARN << "SubScreenHost not found";
        return;
    }
    QStringList qmlData;
    for (const auto &entry : m_stack) {
        std::optional<QString> screen = qmlPathForId(entry.screenId);
        if (screen.has_value()) {
            qmlData << *screen;
        }
    }
    if (m_QmlOder != qmlData) {
        m_QmlOder.clear();
        m_QmlOder = qmlData;
        QMetaObject::invokeMethod(subScreenHost, JS_SUBSCREEN_TRANSITION_FUNCTION, Q_ARG(QVariant, QVariant::fromValue(m_QmlOder)),
                                  Q_ARG(QVariant, QVariant::fromValue(m_QmlOder.count())));
    }
}

} // namespace core::subscreen