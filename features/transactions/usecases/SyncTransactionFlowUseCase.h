#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>
#include "features/transactions/usecases/FetchTransactionUseCase.h"
namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                  \
    X(QString, wallet_id) \
    X(QString, group_id) \
    X(bool, isClaimed) \
    X(nunchuk::Transaction, tx)

DEFINE_USECASE_INPUT(SyncTransactionFlowInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::Transaction, tx) \
    X(QJsonObject, transaction)

DEFINE_USECASE_RESULT(SyncTransactionFlowResult)

class SyncTransactionFlowUseCase : public core::usecase::AsyncUseCase<SyncTransactionFlowUseCase, SyncTransactionFlowInput, SyncTransactionFlowResult> {
  public:
  core::usecase::Result<SyncTransactionFlowResult> execute(const SyncTransactionFlowInput &input) override;

  private:
    void UpdateTransactionSchedule(const QString& wallet_id, const QJsonObject& transaction, const nunchuk::Transaction &tx);
    void UpdateOrReplaceTransaction(const QString& wallet_id, const QJsonObject& transaction, const nunchuk::Transaction &tx);
    nunchuk::Transaction SyncTransaction(const QString& wallet_id, const QJsonObject& transaction, const nunchuk::Transaction &tx, const FetchTransactionInput &input);
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases