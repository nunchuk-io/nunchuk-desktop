#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::common::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, wallet_id)        \
    X(QString, group_id)

DEFINE_USECASE_INPUT(TimeLockReplacementGetInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, timelock)

DEFINE_USECASE_RESULT(TimeLockReplacementGetResult)

class TimeLockReplacementGetUseCase : public core::usecase::AsyncUseCase<TimeLockReplacementGetUseCase, TimeLockReplacementGetInput, TimeLockReplacementGetResult> {
  protected:
  core::usecase::Result<TimeLockReplacementGetResult> execute(const TimeLockReplacementGetInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::common::usecases