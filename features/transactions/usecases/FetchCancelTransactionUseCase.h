#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                  \
    X(QString, wallet_id) \
    X(QString, group_id) \
    X(bool, isClaimed)

DEFINE_USECASE_INPUT(FetchCancelTransactionInput)


#define FIELDS_RESULT(X)       \
    X(QJsonArray, transactions)

DEFINE_USECASE_RESULT(FetchCancelTransactionResult)

class FetchCancelTransactionUseCase : public core::usecase::AsyncUseCase<FetchCancelTransactionUseCase, FetchCancelTransactionInput, FetchCancelTransactionResult> {
  public:
  core::usecase::Result<FetchCancelTransactionResult> execute(const FetchCancelTransactionInput &input) override;

  QJsonArray fetchCancelledTransactions(const FetchCancelTransactionInput &input);
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases