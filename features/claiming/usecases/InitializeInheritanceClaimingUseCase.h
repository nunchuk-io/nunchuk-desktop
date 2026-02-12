#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::claiming::usecases {

#define FIELDS_INPUT(X)          \
    X(QString, magic)

DEFINE_USECASE_INPUT(InitializeInheritanceClaimingInput)

#define FIELDS_RESULT(X)            \
    X(QString, magic)               \
    X(QString, wallet_type)         \
    X(QString, wallet_id)           \
    X(int, inheritance_key_count)   \
    X(QJsonArray, key_origins)  \
    X(int, code)    \
    X(QString, message)

DEFINE_USECASE_RESULT(InitializeInheritanceClaimingResult)

class InitializeInheritanceClaimingUseCase : public core::usecase::AsyncUseCase<InitializeInheritanceClaimingUseCase, InitializeInheritanceClaimingInput, InitializeInheritanceClaimingResult> {
  protected:
  core::usecase::Result<InitializeInheritanceClaimingResult> execute(const InitializeInheritanceClaimingInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::claiming::usecases