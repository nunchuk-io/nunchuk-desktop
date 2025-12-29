#pragma once

#include <QFuture>
#include <QFutureWatcher>
#include <QObject>
#include <QtConcurrent>
#include <atomic>
#include <functional>

namespace core::threads {
template <typename Result> class WorkerConcurrent final {
  public:
    using Task = std::function<Result()>;
    using Callback = std::function<void(const Result &)>;

    WorkerConcurrent();
    ~WorkerConcurrent();

    bool run(Task task, Callback cb);
    bool isRunning() const;

  private:
    void onFinished();

  private:
    std::atomic_bool m_running{false};
    std::atomic_bool m_destroyed{false};

    QFuture<Result> m_future;
    QFutureWatcher<Result> m_watcher;

    std::mutex m_cbMutex;
    Callback m_callback;
};
} // namespace core::threads

#include "WorkerConcurrent.inl"