#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::inheritance::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, group_id)

DEFINE_USECASE_INPUT(CreateInheritancePlanInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, timelock)

DEFINE_USECASE_RESULT(CreateInheritancePlanResult)

class CreateInheritancePlanUseCase : public core::usecase::AsyncUseCase<CreateInheritancePlanUseCase, CreateInheritancePlanInput, CreateInheritancePlanResult> {
  protected:
  core::usecase::Result<CreateInheritancePlanResult> execute(const CreateInheritancePlanInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::inheritance::usecases
