#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                  \
    X(QString, wallet_id) \
    X(QString, group_id) \
    X(QString, txid) \
    X(bool, isClaimed)

DEFINE_USECASE_INPUT(CancelTransactionInput)


#define FIELDS_RESULT(X)       \
    X(bool, ret)

DEFINE_USECASE_RESULT(CancelTransactionResult)

class CancelTransactionUseCase : public core::usecase::AsyncUseCase<CancelTransactionUseCase, CancelTransactionInput, CancelTransactionResult> {
  public:
  core::usecase::Result<CancelTransactionResult> execute(const CancelTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases