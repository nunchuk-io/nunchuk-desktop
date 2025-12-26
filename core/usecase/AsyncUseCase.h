#pragma once
#include <QObject>
#include <QString>
#include "Result.h"
#include <atomic>
#include "core/threads/WorkerConcurrent.h"

namespace core::usecase {
using core::threads::WorkerConcurrent;
template <typename Derived, typename Input, typename Output> class AsyncUseCase {
  public:
    using ResultType = Result<Output>;
    using Callback = std::function<void(Result<Output>)>;
    virtual ~AsyncUseCase() = default;

    void executeAsync(const Input &input, Callback cb);
    void executeSync(const Input &input, Callback cb);
  protected:
    virtual Result<Output> execute(const Input &input) = 0;

  private:
    QString logClassName() const;

  private:
    WorkerConcurrent<ResultType> m_worker;
};
} // namespace core::usecase

#include "AsyncUseCase.inl"