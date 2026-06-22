#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QJsonArray>
#include <QObject>
#include <QString>

namespace features::inheritance::usecases {

#define FIELDS_INPUT(X)

DEFINE_USECASE_INPUT(GetListBufferPeriodsInput)

#define FIELDS_RESULT(X) X(QJsonArray, periods)

DEFINE_USECASE_RESULT(GetListBufferPeriodsResult)

class GetListBufferPeriodsUseCase : public core::usecase::AsyncUseCase<GetListBufferPeriodsUseCase, GetListBufferPeriodsInput, GetListBufferPeriodsResult> {
  public:
    core::usecase::Result<GetListBufferPeriodsResult> execute(const GetListBufferPeriodsInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::inheritance::usecases
