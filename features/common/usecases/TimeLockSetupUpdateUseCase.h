#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::common::usecases {

#define FIELDS_INPUT(X)     \
    X(QString, group_id)    \
    X(QString, timezoneId)  \
    X(double, timestamp) \
    X(QString, based) \
    X(double, block_height) \
    X(bool, isShowBlockHeight)

DEFINE_USECASE_INPUT(TimeLockSetupUpdateInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, timelock)

DEFINE_USECASE_RESULT(TimeLockSetupUpdateResult)

class TimeLockSetupUpdateUseCase : public core::usecase::AsyncUseCase<TimeLockSetupUpdateUseCase, TimeLockSetupUpdateInput, TimeLockSetupUpdateResult> {
  protected:
  core::usecase::Result<TimeLockSetupUpdateResult> execute(const TimeLockSetupUpdateInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::common::usecases