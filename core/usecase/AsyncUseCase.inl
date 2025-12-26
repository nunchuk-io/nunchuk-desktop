#pragma once

#include "LogUseCase.h"
#include "QOutlog.h"

#define DBG_USECASE     QOutlog().begin(LOG_LEVEL::LOG_INFO)  << '[' << QDateTime::currentDateTime() << ']' << '[' << logClassName() << ']'

namespace core::usecase {
template <typename Derived, typename Input, typename Output> void AsyncUseCase<Derived, Input, Output>::executeAsync(const Input &input, Callback cb) {
    m_worker.run(
        [this, input]() -> ResultType {
            DBG_USECASE << valueToString(input);

            auto ret = this->execute(input);

            DBG_USECASE << ret.toLogString();
            return ret;
        },
        std::move(cb)
    );
}

template <typename Derived, typename Input, typename Output> void AsyncUseCase<Derived, Input, Output>::executeSync(const Input &input, Callback cb) {
    DBG_USECASE << valueToString(input);
    Result<Output> ret = this->execute(input);
    DBG_USECASE << ret.toLogString();
    cb(std::move(ret));
}

template <typename Derived, typename Input, typename Output> QString AsyncUseCase<Derived, Input, Output>::logClassName() const {
    return QString("%1").arg(typeid(Derived).name());
}
} // namespace core::usecase