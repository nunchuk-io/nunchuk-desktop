#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::claiming::usecases {
  
#define FIELDS_INPUT(X)          \
    X(QString, magic)            \
    X(QString, bsms)            \
    X(QString, messageId)          \
    X(QStringList, authos)

DEFINE_USECASE_INPUT(ClaimStatusInput)

#define FIELDS_RESULT(X)       \
    X(QJsonObject, inheritance) \
    X(double, balance) \
    X(QJsonObject, buffer_period_countdown)

DEFINE_USECASE_RESULT(ClaimStatusResult)

class ClaimStatusUseCase : public core::usecase::AsyncUseCase<ClaimStatusUseCase, ClaimStatusInput, ClaimStatusResult> {
  protected:
  core::usecase::Result<ClaimStatusResult> execute(const ClaimStatusInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::claiming::usecases