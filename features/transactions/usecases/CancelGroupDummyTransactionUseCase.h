#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                 \
    X(QString, wallet_id)               \
    X(QString, dummy_transaction_id)

DEFINE_USECASE_INPUT(CancelGroupDummyTransactionInput)

#define FIELDS_RESULT(X) \
    X(bool, cancelled)

DEFINE_USECASE_RESULT(CancelGroupDummyTransactionResult)

class CancelGroupDummyTransactionUseCase
    : public core::usecase::AsyncUseCase<CancelGroupDummyTransactionUseCase, CancelGroupDummyTransactionInput,
                                         CancelGroupDummyTransactionResult> {
  protected:
    core::usecase::Result<CancelGroupDummyTransactionResult> execute(const CancelGroupDummyTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::transactions::usecases