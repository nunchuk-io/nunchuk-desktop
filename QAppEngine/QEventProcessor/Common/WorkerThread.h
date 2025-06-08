#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include <QObject>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>
#include <functional>
#include <QApplication>
#include <QtConcurrent>
#include <QDebug>

inline bool appShuttingDown() {
    return !QCoreApplication::instance() || QCoreApplication::closingDown();
}
class WorkerThread : public QThread {
    Q_OBJECT

public:
    using Task = std::function<void()>;

    static WorkerThread *instance();

    void runTask(Task task);

protected:
    void run() override;

private:
    WorkerThread();
    ~WorkerThread();
    bool stopThread;  // Flag to stop the loop
    QQueue<Task> taskQueue;
    QMutex mutex;
    QWaitCondition condition;
};

template <typename ProgressFunc>
struct ProgressReporter {
    ProgressFunc& progressFunc;

    template <typename T>
    void reportProgress(T value) {
        QMetaObject::invokeMethod(qApp, [progressCopy = progressFunc, value]() {
            progressCopy(value);
        }, Qt::QueuedConnection);
    }
};

// Global function to execute tasks in Singleton Thread
template <typename Func1, typename Func2>
void runInThread(Func1&& execute, Func2&& ret) {
    auto safeExecute = std::make_shared<std::decay_t<Func1>>(std::forward<Func1>(execute));
    auto safeRet = std::make_shared<std::decay_t<Func2>>(std::forward<Func2>(ret));

    WorkerThread::instance()->runTask([safeExecute, safeRet]() mutable {
        try {
            using ResultType = decltype((*safeExecute)());
            if constexpr (std::is_void_v<ResultType>) {
                (*safeExecute)();
                QMetaObject::invokeMethod(qApp, [safeRet]() {
                    if (appShuttingDown())
                        return;
                    (*safeRet)();
                }, Qt::QueuedConnection);
            } else {
                ResultType result = (*safeExecute)();  // Get result
                QMetaObject::invokeMethod(
                    qApp,
                    [safeRet, tmp = std::move(result)]() mutable {
                        if (appShuttingDown())
                            return;
                        (*safeRet)(std::move(tmp));
                    },
                    Qt::QueuedConnection
                    );
            }
        } catch (const std::exception& e) {
            qDebug() << "Exception in execute(): " << e.what();
        } catch (...) {
            qDebug() << "Unknown exception in thread execution";
        }
    });
}

static QMutex sMutex;  // Shared mutex for thread safety

template <typename Func1, typename Func2>
void runInConcurrent(Func1&& execute, Func2&& ret) {
    using ResultType = decltype(execute());

    QFuture<ResultType> future = QtConcurrent::run([execute = std::forward<Func1>(execute)]() -> ResultType {
        QMutexLocker locker(&sMutex);
        try {
            return execute();
        } catch (const std::exception& e) {
            qDebug() << "Exception in execute: " << e.what();
            throw;
        } catch (...) {
            qDebug() << "Unknown exception in execute";
            throw;
        }
    });

    auto watcher = QSharedPointer<QFutureWatcher<ResultType>>::create();

    QObject::connect(watcher.get(), &QFutureWatcher<ResultType>::finished, [watcher, ret = std::forward<Func2>(ret)]() mutable {
        try {
            QMutexLocker locker(&sMutex);
            if (appShuttingDown())
                return;

            if constexpr (!std::is_void_v<ResultType>) {
                ResultType result = watcher->result();
                QMetaObject::invokeMethod(qApp, [ret = std::move(ret), tmp = std::move(result)]() mutable {
                    if (appShuttingDown()) return;
                    ret(std::move(tmp));
                }, Qt::QueuedConnection);
            } else {
                QMetaObject::invokeMethod(qApp, [ret = std::move(ret)]() {
                    if (appShuttingDown()) return;
                    ret();
                }, Qt::QueuedConnection);
            }
        } catch (const std::exception& e) {
            qDebug() << "Exception in result processing: " << e.what();
        } catch (...) {
            qDebug() << "Unknown exception in result processing";
        }
    });

    watcher->setFuture(future);
}

template<typename Function>
void timeoutHandler(int timeoutInterval, Function&& f)
{
    QTimer::singleShot(timeoutInterval, std::forward<Function>(f));
}

#define SAFE_QPOINTER_CHECK(objName, ptr) \
if (!(ptr)) return {};                    \
    auto objName = (ptr);                 \
if (!(objName)) return {};


#define SAFE_QPOINTER_CHECK_RETURN_VOID(objName, ptr) \
if (!(ptr)) return;                            \
    auto objName = (ptr);                      \
    if (!(objName)) return;


inline bool isBusy() {
    if (QApplication::overrideCursor() && QApplication::overrideCursor()->shape() == Qt::WaitCursor) {
        qDebug() << "Cursor is currently in wait state.";
        return true;
    }
    return false;
}
#endif // WORKERTHREAD_H
