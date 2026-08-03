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
// template <typename Func1, typename Func2>
// void runInThread(Func1&& execute, Func2&& ret) {
//     auto safeExecute = std::make_shared<std::decay_t<Func1>>(std::forward<Func1>(execute));
//     auto safeRet = std::make_shared<std::decay_t<Func2>>(std::forward<Func2>(ret));

//     WorkerThread::instance()->runTask([safeExecute, safeRet]() mutable {
//         try {
//             using ResultType = decltype((*safeExecute)());
//             if constexpr (std::is_void_v<ResultType>) {
//                 (*safeExecute)();
//                 QMetaObject::invokeMethod(qApp, [safeRet]() {
//                     if (appShuttingDown())
//                         return;
//                     (*safeRet)();
//                 }, Qt::QueuedConnection);
//             } else {
//                 ResultType result = (*safeExecute)();  // Get result
//                 QMetaObject::invokeMethod(
//                     qApp,
//                     [safeRet, tmp = std::move(result)]() mutable {
//                         if (appShuttingDown())
//                             return;
//                         (*safeRet)(std::move(tmp));
//                     },
//                     Qt::QueuedConnection
//                     );
//             }
//         } catch (const std::exception& e) {
//             qDebug() << "Exception in execute(): " << e.what();
//         } catch (...) {
//             qDebug() << "Unknown exception in thread execution";
//         }
//     });
// }

// NOTE (known quirk, intentionally left unchanged for now):
// 'static' at namespace scope in a header gives each translation unit its
// OWN copy of this mutex, so runInConcurrent() tasks are only serialized
// within one .cpp file, not globally across files. Whether global
// serialization is required depends on libnunchuk thread-safety (open
// question). Do NOT rely on this mutex for cross-file mutual exclusion.
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

    // P1: raw watcher + deleteLater() in the handler. A QSharedPointer captured
    // in the watcher's own connection creates a reference cycle
    // (watcher -> connection -> lambda -> shared_ptr -> watcher), so the
    // watcher and the captured 'ret' closure were never freed.
    auto* watcher = new QFutureWatcher<ResultType>();

    QObject::connect(watcher, &QFutureWatcher<ResultType>::finished, [watcher, ret = std::forward<Func2>(ret)]() mutable {
        // P2: no sMutex here. The task already finished; this handler only reads
        // the stored result and posts a queued call. Taking sMutex here would
        // block this (usually GUI) thread until an unrelated running task
        // releases the lock.
        try {
            if (!appShuttingDown()) {
                if constexpr (!std::is_void_v<ResultType>) {
                    ResultType result = watcher->result(); // rethrows if execute() threw -> ret skipped
                    QMetaObject::invokeMethod(qApp, [ret = std::move(ret), tmp = std::move(result)]() mutable {
                        if (appShuttingDown()) return;
                        ret(std::move(tmp));
                    }, Qt::QueuedConnection);
                } else {
                    // P4: already finished, does not block; rethrows if execute()
                    // threw so the void branch also skips 'ret' on failure
                    // (previously it reported success even after an exception).
                    watcher->future().waitForFinished();
                    QMetaObject::invokeMethod(qApp, [ret = std::move(ret)]() {
                        if (appShuttingDown()) return;
                        ret();
                    }, Qt::QueuedConnection);
                }
            }
        } catch (const std::exception& e) {
            qDebug() << "Exception in result processing: " << e.what();
        } catch (...) {
            qDebug() << "Unknown exception in result processing";
        }
        watcher->deleteLater();
    });

    watcher->setFuture(future);
}

template <typename Work, typename Done>
void runInThread(QObject* receiver, Work&& work, Done&& done)
{
    using WorkT  = std::decay_t<Work>;
    using Result = std::invoke_result_t<WorkT>;

    auto* watcher = new QFutureWatcher<Result>(receiver);

    QObject::connect(watcher, &QFutureWatcher<Result>::finished, receiver,
                     [watcher, done = std::forward<Done>(done)]() mutable {
                         if (appShuttingDown()) {
                             watcher->deleteLater();
                             return;
                         }

                         // P3: surface exceptions thrown by work() here instead of
                         // letting them escape into the receiver's event loop
                         // (Qt does not support that; the app would terminate).
                         // On failure, 'done' is skipped and the error is logged.
                         try {
                             if constexpr (std::is_void_v<Result>) {
                                 // already finished, does not block; rethrows work() exception
                                 watcher->future().waitForFinished();
                                 done();
                             } else {
                                 done(watcher->result()); // result() rethrows work() exception
                             }
                         } catch (const std::exception& e) {
                             qWarning() << "runInThread: exception in work():" << e.what() << "- done() skipped";
                         } catch (...) {
                             qWarning() << "runInThread: unknown exception in work() - done() skipped";
                         }

                         watcher->deleteLater();
                     });

    watcher->setFuture(QtConcurrent::run(std::forward<Work>(work)));
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
