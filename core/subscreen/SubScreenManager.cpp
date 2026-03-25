#include "SubScreenManager.h"
#include "features/ScreenQmlDefine.hpp"
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>

namespace core::subscreen {
#define JS_SUBSCREEN_TRANSITION_FUNCTION "subScreen_Transition"
SubScreenManager::SubScreenManager(QObject *parent) : QObject(parent) {}

void SubScreenManager::show(const QString &screenId) {
    runOnGuiThread([this, screenId]() {
        m_stack.push_back({screenId});
        qmlSyncup();
    });
}

void SubScreenManager::replace(const QString &screenId) {
    runOnGuiThread([this, screenId]() {
        if (!m_stack.isEmpty())
            m_stack.pop_back();
        m_stack.push_back({screenId});
        qmlSyncup();
    });
}

void SubScreenManager::clearAndShow(const QString &screenId) {
    runOnGuiThread([this, screenId]() {
        m_stack.clear();
        m_stack.push_back({screenId});
        qmlSyncup();
    });
}

void SubScreenManager::back() {
    runOnGuiThread([this]() {
        if (m_stack.isEmpty())
            return;
        m_stack.pop_back();
        qmlSyncup();
    });
}

void SubScreenManager::clear() {
    runOnGuiThread([this]() {
        if (m_stack.isEmpty())
            return;
        m_stack.clear();
        qmlSyncup();
    });
}

void SubScreenManager::onInitialized(QQuickView *view) {
    runOnGuiThread([this, view]() { m_viewer = view; });
}

void SubScreenManager::qmlSyncup() {
    if (!m_viewer)
        return;
    QObject *rootObject = m_viewer->rootObject();
    if (!rootObject) {
        return;
    }
    QObject *subScreenHost = rootObject->findChild<QObject *>("subScreenHost");
    if (!subScreenHost) {
        // DBG_WARN << "SubScreenHost not found";
        return;
    }
    QStringList qmlData;
    if (!m_stack.isEmpty()) {
        const auto &entry = m_stack.back();
        if (auto screen = qmlPathForId(entry.screenId); screen.has_value()) {
            qmlData << *screen;
        }
    }
    if (m_QmlOder != qmlData) {
        m_QmlOder.clear();
        m_QmlOder = qmlData;
        qDebug() << m_QmlOder;
        QMetaObject::invokeMethod(subScreenHost, JS_SUBSCREEN_TRANSITION_FUNCTION, Q_ARG(QVariant, QVariant::fromValue(m_QmlOder)),
                                  Q_ARG(QVariant, QVariant::fromValue(m_QmlOder.count())));
    }
}

} // namespace core::subscreen