#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                  \
    X(QString, wallet_id) \
    X(QString, group_id) \
    X(bool, isClaimed)

DEFINE_USECASE_INPUT(FetchTransactionListInput)


#define FIELDS_RESULT(X)       \
    X(QString, dummy)

DEFINE_USECASE_RESULT(FetchTransactionListResult)

class FetchTransactionListUseCase : public core::usecase::AsyncUseCase<FetchTransactionListUseCase, FetchTransactionListInput, FetchTransactionListResult> {
  public:
  core::usecase::Result<FetchTransactionListResult> execute(const FetchTransactionListInput &input) override;

  void UpdateTransactionList(const QJsonArray &transactions, const QString &wallet_id);
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases