#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                 \
    X(QString, wallet_id)               \
    X(QString, dummy_transaction_id)    \
    X(QStringList, signatures)

DEFINE_USECASE_INPUT(SignGroupDummyTransactionInput)

#define FIELDS_RESULT(X)                 \
    X(nunchuk::GroupDummyTransaction, dummy_transaction)

DEFINE_USECASE_RESULT(SignGroupDummyTransactionResult)

class SignGroupDummyTransactionUseCase
    : public core::usecase::AsyncUseCase<SignGroupDummyTransactionUseCase, SignGroupDummyTransactionInput,
                                         SignGroupDummyTransactionResult> {
  protected:
    core::usecase::Result<SignGroupDummyTransactionResult> execute(const SignGroupDummyTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::transactions::usecases