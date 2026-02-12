#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::inheritance::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, group_id)

DEFINE_USECASE_INPUT(UpdateInheritancePlanInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, timelock)

DEFINE_USECASE_RESULT(UpdateInheritancePlanResult)

class UpdateInheritancePlanUseCase : public core::usecase::AsyncUseCase<UpdateInheritancePlanUseCase, UpdateInheritancePlanInput, UpdateInheritancePlanResult> {
  protected:
  core::usecase::Result<UpdateInheritancePlanResult> execute(const UpdateInheritancePlanInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::inheritance::usecases
