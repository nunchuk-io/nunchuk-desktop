#pragma once

#include <QFuture>
#include <QFutureWatcher>
#include <QObject>
#include <QtConcurrent>
#include <atomic>
#include <functional>

namespace core::threads {
template <typename Result> 
class WorkerConcurrent final  {

  public:
    using Task = std::function<Result()>;
    using Callback = std::function<void(Result)>;

    WorkerConcurrent() = default;
    ~WorkerConcurrent();

    bool run(Task task, Callback cb);
    
    bool isRunning() const;

  private:
    void onFinished();

  private:
    std::atomic_bool m_alive{true};
    std::atomic_bool m_running{false};

    QFuture<Result> m_future;
    QFutureWatcher<Result> m_watcher;
    Callback m_callback;
};
} // namespace core::threads

#include "WorkerConcurrent.inl"