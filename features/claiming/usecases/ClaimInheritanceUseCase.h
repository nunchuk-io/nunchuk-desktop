#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::claiming::usecases {

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(QString, wallet_id)                                                                                                                                      \
    X(nunchuk::Transaction, txSigned)                                                                                                                          \
    X(QString, magic)                                                                                                                                          \
    X(QString, bsms)

DEFINE_USECASE_INPUT(ClaimInheritanceInput)

#define FIELDS_RESULT(X) X(nunchuk::Transaction, tx)

DEFINE_USECASE_RESULT(ClaimInheritanceResult)

class ClaimInheritanceUseCase : public core::usecase::AsyncUseCase<ClaimInheritanceUseCase, ClaimInheritanceInput, ClaimInheritanceResult> {
  protected:
    core::usecase::Result<ClaimInheritanceResult> execute(const ClaimInheritanceInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::claiming::usecases