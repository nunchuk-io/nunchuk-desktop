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

DEFINE_USECASE_INPUT(FetchTransactionInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, transaction)

DEFINE_USECASE_RESULT(FetchTransactionResult)

class FetchTransactionUseCase : public core::usecase::AsyncUseCase<FetchTransactionUseCase, FetchTransactionInput, FetchTransactionResult> {
  public:
  core::usecase::Result<FetchTransactionResult> execute(const FetchTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases