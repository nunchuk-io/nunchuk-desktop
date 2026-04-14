#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <vector>
#include <nunchuk.h>

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                 \
    X(QString, wallet_id)

DEFINE_USECASE_INPUT(GetGroupDummyTransactionsInput)

#define FIELDS_RESULT(X)                                \
    X(std::vector<nunchuk::GroupDummyTransaction>, dummy_transactions)

DEFINE_USECASE_RESULT(GetGroupDummyTransactionsResult)

class GetGroupDummyTransactionsUseCase
    : public core::usecase::AsyncUseCase<GetGroupDummyTransactionsUseCase, GetGroupDummyTransactionsInput,
                                         GetGroupDummyTransactionsResult> {
  protected:
    core::usecase::Result<GetGroupDummyTransactionsResult> execute(const GetGroupDummyTransactionsInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::transactions::usecases