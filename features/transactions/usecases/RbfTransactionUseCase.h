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
    X(QString, psbt)

DEFINE_USECASE_INPUT(RbfTransactionInput)


#define FIELDS_RESULT(X)       \
    X(QString, dummy) \

DEFINE_USECASE_RESULT(RbfTransactionResult)

class RbfTransactionUseCase : public core::usecase::AsyncUseCase<RbfTransactionUseCase, RbfTransactionInput, RbfTransactionResult> {
  public:
  core::usecase::Result<RbfTransactionResult> execute(const RbfTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases