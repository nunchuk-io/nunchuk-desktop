#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                  \
    X(QString, wallet_id) \
    X(QString, group_id) \
    X(QString, txid) \
    X(bool, isClaimed) \
    X(QString, note)

DEFINE_USECASE_INPUT(UpdateTransactionInput)


#define FIELDS_RESULT(X)       \
    X(bool, synced)

DEFINE_USECASE_RESULT(UpdateTransactionResult)

class UpdateTransactionUseCase : public core::usecase::AsyncUseCase<UpdateTransactionUseCase, UpdateTransactionInput, UpdateTransactionResult> {
  public:
  core::usecase::Result<UpdateTransactionResult> execute(const UpdateTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases