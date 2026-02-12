#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::inheritance::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, wallet_id) \
    X(QString, group_id)

DEFINE_USECASE_INPUT(GetInheritancePlanInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, inheritance)

DEFINE_USECASE_RESULT(GetInheritancePlanResult)

class GetInheritancePlanUseCase : public core::usecase::AsyncUseCase<GetInheritancePlanUseCase, GetInheritancePlanInput, GetInheritancePlanResult> {
  protected:
  core::usecase::Result<GetInheritancePlanResult> execute(const GetInheritancePlanInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::inheritance::usecases
