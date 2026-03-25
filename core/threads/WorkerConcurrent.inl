#include "WorkerConcurrent.h"

namespace core::threads {

template <typename Result> WorkerConcurrent<Result>::WorkerConcurrent() {
    QObject::connect(&m_watcher, &QFutureWatcherBase::finished, &m_watcher, [this]() { onFinished(); }, Qt::QueuedConnection);
}

template <typename Result> WorkerConcurrent<Result>::~WorkerConcurrent() {
    m_destroyed.store(true);

    // Ngăn signal bắn vào object đã chết
    QObject::disconnect(&m_watcher, nullptr, nullptr, nullptr);

    // KHÔNG wait → không treo
    {
        std::lock_guard lock(m_cbMutex);
        m_callback = nullptr;
    }
}

template <typename Result> bool WorkerConcurrent<Result>::run(Task task, Callback cb) {
    if (m_running.exchange(true)) {
        return false;
    }

    {
        std::lock_guard lock(m_cbMutex);
        m_callback = std::move(cb);
    }

    m_future = QtConcurrent::run(std::move(task));
    m_watcher.setFuture(m_future);
    return true;
}

template <typename Result> bool WorkerConcurrent<Result>::isRunning() const {
    return m_running.load();
}

template <typename Result> void WorkerConcurrent<Result>::onFinished() {
    m_running.store(false);

    if (m_destroyed.load()) {
        return;
    }

    // Check if future is valid and has a result before accessing
    if (!m_future.isFinished() || m_future.isCanceled() || !m_future.isResultReadyAt(0)) {
        return;
    }

    Callback cb;
    {
        std::lock_guard lock(m_cbMutex);
        cb = std::move(m_callback);
        m_callback = nullptr;
    }

    if (cb) {
        try {
            cb(m_future.result());
        } catch (...) {
            // Suppress exceptions from result() or callback
        }
    }
}

} // namespace core::threads
