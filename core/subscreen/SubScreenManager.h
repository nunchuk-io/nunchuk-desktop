#pragma once
#include <QApplication>
#include <QObject>
#include <QPointer>
#include <QQuickView>
#include <QString>
#include <QThread>
#include <QVariantMap>
#include <QVector>

namespace core::subscreen {
class SubScreenManager : public QObject {
    Q_OBJECT
  public:
    explicit SubScreenManager(QObject *parent = nullptr);

    // Navigation API — safe to call from any thread
    Q_INVOKABLE void show(const QString &screenId);

    Q_INVOKABLE void replace(const QString &screenId);

    Q_INVOKABLE void clearAndShow(const QString &screenId);

    Q_INVOKABLE void back();
    Q_INVOKABLE void clear();
    int count() const;

    void onInitialized(QQuickView *view);

  private:
    void qmlSyncup();

    template <typename Func> void runOnGuiThread(Func &&func) {
        if (QThread::currentThread() == qApp->thread()) {
            func();
            return;
        }
        QPointer<SubScreenManager> guard(this);
        QMetaObject::invokeMethod(
            this,
            [guard, f = std::forward<Func>(func)]() mutable {
                if (guard)
                    f();
            },
            Qt::QueuedConnection);
    }

  private:
    struct Entry {
        QString screenId;
    };

    QVector<Entry> m_stack;
    QQuickView *m_viewer{nullptr};
    QStringList m_QmlOder;
};
} // namespace core::subscreen