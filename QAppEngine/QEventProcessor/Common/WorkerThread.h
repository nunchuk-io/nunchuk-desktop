#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include <QObject>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>
#include <functional>
#include <QGuiApplication>
#include <QtConcurrent>
#include <QDebug>

// Singleton Worker Thread Class
class WorkerThread : public QThread {
    Q_OBJECT

public:
    using Task = std::function<void()>;

    static WorkerThread& instance() {
        static WorkerThread instance; // Singleton
        return instance;
    }

    void runTask(Task task) {
        QMutexLocker locker(&mutex);
        taskQueue.enqueue(task);
        condition.wakeOne();  // Wake up the worker thread
    }

protected:
    void run() override {
        while (true) {
            Task task;
            {
                QMutexLocker locker(&mutex);
                while (taskQueue.isEmpty() && !stopThread) {  // Prevent CPU spinning
                    condition.wait(&mutex);  // Wait for new tasks
                }
                if (stopThread) break;  // Exit loop when stopping
                if (!taskQueue.isEmpty()) {
                    task = taskQueue.dequeue(); // Get next task
                }
            }
            if (task) {
                task();  // Execute task
            }
        }
    }

private:
    WorkerThread() {
        stopThread = false;
        start();  // Start worker thread on creation
    }

    ~WorkerThread() {
        {
            QMutexLocker locker(&mutex);
            stopThread = true; // Signal the thread to stop
            condition.wakeOne(); // Wake up the thread so it can exit
        }
        wait();  // Ensure thread stops before destruction
    }

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

    WorkerThread::instance().runTask([safeExecute, safeRet]() mutable {
        try {
            using ResultType = decltype((*safeExecute)());
            if constexpr (std::is_void_v<ResultType>) {
                (*safeExecute)();
                QMetaObject::invokeMethod(qApp, [safeRet]() { (*safeRet)(); }, Qt::QueuedConnection);
            } else {
                ResultType result = (*safeExecute)();  // Get result
                QMetaObject::invokeMethod(
                    qApp,
                    [safeRet, tmp = std::move(result)]() mutable { (*safeRet)(std::move(tmp)); },
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
    // Run function in a background thread
    QFuture<ResultType> future = QtConcurrent::run([execute = std::forward<Func1>(execute)]() -> ResultType {
        QMutexLocker locker(&sMutex);  // Protect shared data inside execute()
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

    // Watcher to monitor future completion
    auto watcher = QSharedPointer<QFutureWatcher<ResultType>>::create();

    QObject::connect(watcher.get(), &QFutureWatcher<ResultType>::finished, [watcher, ret = std::forward<Func2>(ret)]() mutable {
        try {
            QMutexLocker locker(&sMutex);  // Protect shared data inside ret()
            if constexpr (!std::is_void_v<ResultType>) {
                ResultType result = watcher->result();  // Get result safely
                QMetaObject::invokeMethod(qApp, [ret = std::move(ret), tmp = std::move(result)]() mutable {
                    ret(std::move(tmp));  // Move to prevent deep copy
                }, Qt::QueuedConnection);
            } else {
                QMetaObject::invokeMethod(qApp, [ret = std::move(ret)]() {
                    ret();  // Call function in main thread safely
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
#endif // WORKERTHREAD_H
