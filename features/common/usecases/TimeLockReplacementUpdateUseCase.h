#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::common::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, wallet_id)        \
    X(QString, group_id)    \
    X(QString, timezoneId)  \
    X(double, timestamp) \
    X(QString, verifyToken) \
    X(QString, based) \
    X(double, block_height) \
    X(bool, isShowBlockHeight)

DEFINE_USECASE_INPUT(TimeLockReplacementUpdateGetInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, timelock)

DEFINE_USECASE_RESULT(TimeLockReplacementUpdateGetResult)

class TimeLockReplacementUpdateUseCase : public core::usecase::AsyncUseCase<TimeLockReplacementUpdateUseCase, TimeLockReplacementUpdateGetInput, TimeLockReplacementUpdateGetResult> {
  protected:
  core::usecase::Result<TimeLockReplacementUpdateGetResult> execute(const TimeLockReplacementUpdateGetInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::common::usecases