#include "Result.h"
#include "UseCaseResult.h"

namespace core::usecase {
    
    template <typename Output>
    QString Result<Output>::toLogString() const {
        if (isSuccess()) {
            return QString("Result{Success, value=%1}")
                .arg(valueToString(value()));
        }
        return QString("Result{Error, error=%1}")
            .arg(m_error);
    }
}