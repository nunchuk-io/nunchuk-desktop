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
    X(QString, psbt) \
    X(QString, note)

DEFINE_USECASE_INPUT(SignTransactionInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, transaction) \

DEFINE_USECASE_RESULT(SignTransactionResult)

class SignTransactionUseCase : public core::usecase::AsyncUseCase<SignTransactionUseCase, SignTransactionInput, SignTransactionResult> {
  public:
  core::usecase::Result<SignTransactionResult> execute(const SignTransactionInput &input) override;

  void UpdateTransaction(const QString& txid, const QJsonObject &transaction, const QString &wallet_id);
  void UpdateTransactionClaim(const QJsonObject &transaction, const QString &wallet_id);
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases