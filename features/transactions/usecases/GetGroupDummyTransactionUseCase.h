#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                 \
    X(QString, wallet_id)               \
    X(QString, dummy_transaction_id)

DEFINE_USECASE_INPUT(GetGroupDummyTransactionInput)

#define FIELDS_RESULT(X)                 \
    X(nunchuk::GroupDummyTransaction, dummy_transaction)

DEFINE_USECASE_RESULT(GetGroupDummyTransactionResult)

class GetGroupDummyTransactionUseCase
    : public core::usecase::AsyncUseCase<GetGroupDummyTransactionUseCase, GetGroupDummyTransactionInput,
                                         GetGroupDummyTransactionResult> {
  protected:
    core::usecase::Result<GetGroupDummyTransactionResult> execute(const GetGroupDummyTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::transactions::usecases