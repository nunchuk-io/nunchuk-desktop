#pragma once
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QVector>
#include <QQuickView>

namespace core::subscreen {
class SubScreenManager : public QObject {
    Q_OBJECT
  public:
    explicit SubScreenManager(QObject *parent = nullptr);

    // Navigation API
    Q_INVOKABLE void show(const QString &screenId);

    Q_INVOKABLE void replace(const QString &screenId);

    Q_INVOKABLE void clearAndShow(const QString &screenId);

    Q_INVOKABLE void back();
    Q_INVOKABLE void clear();

    void onInitialized(QQuickView *view);
  private:
    void qmlSyncup();

  private:
    struct Entry {
        QString screenId;
    };

    QVector<Entry> m_stack;
    QQuickView *m_viewer {nullptr};
    QStringList m_QmlOder;
};
} // namespace core::subscreen