#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::claiming::usecases {

#define FIELDS_INPUT(X)          \
    X(QString, magic)

DEFINE_USECASE_INPUT(SigningChallengeInput)

#define FIELDS_RESULT(X)       \
    X(QJsonObject, message)

DEFINE_USECASE_RESULT(SigningChallengeResult)

class SigningChallengeUseCase : public core::usecase::AsyncUseCase<SigningChallengeUseCase, SigningChallengeInput, SigningChallengeResult> {
  protected:
  core::usecase::Result<SigningChallengeResult> execute(const SigningChallengeInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::claiming::usecases