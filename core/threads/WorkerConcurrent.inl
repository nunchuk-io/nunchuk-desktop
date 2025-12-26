#include "WorkerConcurrent.h"

namespace core::threads {

template <typename Result> WorkerConcurrent<Result>::~WorkerConcurrent() {
    m_alive.store(false);
    m_callback = nullptr;
}

template <typename Result> bool WorkerConcurrent<Result>::run(Task task, Callback cb) {
    if (m_running.exchange(true)) {
        return false;
    }

    m_alive.store(true);
    m_callback = std::move(cb);

    m_future = QtConcurrent::run([task = std::move(task)] {
        return task(); // run to completion
    });

    QObject::connect(
        &m_watcher,
        &QFutureWatcher<Result>::finished,
        [this]() { onFinished(); }
    );

    m_watcher.setFuture(m_future);
    return true;
}

template <typename Result> bool WorkerConcurrent<Result>::isRunning() const {
    return m_running.load();
}

template <typename Result> void WorkerConcurrent<Result>::onFinished() {
    m_running.store(false);

    if (!m_alive.load()) {
        m_callback = nullptr; // ❗ Worker đã chết → bỏ kết quả
        return;
    }

    if (m_callback) {
        m_callback(m_future.result());
    }
    m_callback = nullptr;
}

} // namespace core::threads
