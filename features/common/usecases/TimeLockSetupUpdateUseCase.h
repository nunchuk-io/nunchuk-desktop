#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>

namespace features::common::usecases {

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(QString, group_id)                                                                                                                                       \
    X(QString, timezoneId)                                                                                                                                     \
    X(double, timestamp)                                                                                                                                       \
    X(QString, based)                                                                                                                                          \
    X(double, block_height)                                                                                                                                    \
    X(bool, isShowBlockHeight)                                                                                                                                 \
    X(int, fallback_index)

DEFINE_USECASE_INPUT(TimeLockSetupUpdateInput)

#define FIELDS_RESULT(X) X(QJsonObject, timelock) X(QJsonObject, draft_wallet)

DEFINE_USECASE_RESULT(TimeLockSetupUpdateResult)

class TimeLockSetupUpdateUseCase : public core::usecase::AsyncUseCase<TimeLockSetupUpdateUseCase, TimeLockSetupUpdateInput, TimeLockSetupUpdateResult> {
  protected:
    core::usecase::Result<TimeLockSetupUpdateResult> execute(const TimeLockSetupUpdateInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::common::usecases