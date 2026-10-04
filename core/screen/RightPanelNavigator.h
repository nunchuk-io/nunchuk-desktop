#pragma once
#include <QObject>
#include <QString>
#include <QQuickView>
#include <QStack>

namespace core::screen {
class RightPanelNavigator : public QObject {
    Q_OBJECT
  public:
    explicit RightPanelNavigator(QObject *parent = nullptr);

    // Navigation API
    void request(const QString &screenId);
    void back();

    // Register/Unregister API
    void registerObject(QObject *object);
    void unregisterObject();

    // True if a screen was already requested (e.g. host was recreated mid-flow) - callers use this
    // to avoid blindly re-requesting their initial screen and clobbering an in-progress one.
    bool hasActiveScreen() const {
        return !m_currentScreenId.isEmpty();
    }

  private:
    void qmlSyncup();

  private:
    QString m_currentScreenId;
    QList<QObject*> m_registeredObjects;
    QStack<QString> m_historyStack;
};
} // namespace core::screen