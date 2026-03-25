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

  private:
    void qmlSyncup();

  private:
    QString m_currentScreenId;
    QList<QObject*> m_registeredObjects;
    QStack<QString> m_historyStack;
};
} // namespace core::screen