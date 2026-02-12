#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::inheritance::onchain::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, wallet_id) \
    X(QString, group_id)

DEFINE_USECASE_INPUT(TimeLockSetupGetInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, timelock)

DEFINE_USECASE_RESULT(TimeLockSetupGetResult)

class TimeLockSetupGetUseCase : public core::usecase::AsyncUseCase<TimeLockSetupGetUseCase, TimeLockSetupGetInput, TimeLockSetupGetResult> {
  protected:
  core::usecase::Result<TimeLockSetupGetResult> execute(const TimeLockSetupGetInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::inheritance::onchain::usecases