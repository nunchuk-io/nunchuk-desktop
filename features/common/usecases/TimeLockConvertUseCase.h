#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::common::usecases {

#define FIELDS_INPUT(X)     \
    X(QString, timezoneId)  \
    X(double, timestamp) \
    X(bool, isShowBlockHeight)

DEFINE_USECASE_INPUT(TimeLockConvertInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, timelock)

DEFINE_USECASE_RESULT(TimeLockConvertResult)

class TimeLockConvertUseCase : public core::usecase::AsyncUseCase<TimeLockConvertUseCase, TimeLockConvertInput, TimeLockConvertResult> {
  protected:
  core::usecase::Result<TimeLockConvertResult> execute(const TimeLockConvertInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::common::usecases