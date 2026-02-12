#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::inheritance::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, group_id)

DEFINE_USECASE_INPUT(CancelInheritanceInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, timelock)

DEFINE_USECASE_RESULT(CancelInheritanceResult)

class CancelInheritanceUseCase : public core::usecase::AsyncUseCase<CancelInheritanceUseCase, CancelInheritanceInput, CancelInheritanceResult> {
  protected:
  core::usecase::Result<CancelInheritanceResult> execute(const CancelInheritanceInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::inheritance::usecases
