#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                  \
    X(QString, wallet_id) \
    X(QString, group_id) \
    X(bool, isClaimed) \
    X(QString, psbt) \
    X(QString, note)

DEFINE_USECASE_INPUT(CreateTransactionInput)


#define FIELDS_RESULT(X)       \
    X(bool, synced)

DEFINE_USECASE_RESULT(CreateTransactionResult)

class CreateTransactionUseCase : public core::usecase::AsyncUseCase<CreateTransactionUseCase, CreateTransactionInput, CreateTransactionResult> {
  public:
  core::usecase::Result<CreateTransactionResult> execute(const CreateTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases