#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QJsonObject>

namespace features::home::usecases {

#define FIELDS_INPUT(X) X(bool, anonymous)
DEFINE_USECASE_INPUT(GetHomeReminderInput)

#define FIELDS_RESULT(X) X(QJsonObject, reminder)
DEFINE_USECASE_RESULT(GetHomeReminderResult)

class GetHomeReminderUseCase
    : public core::usecase::AsyncUseCase<GetHomeReminderUseCase, GetHomeReminderInput, GetHomeReminderResult> {
  protected:
    core::usecase::Result<GetHomeReminderResult> execute(const GetHomeReminderInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::home::usecases
