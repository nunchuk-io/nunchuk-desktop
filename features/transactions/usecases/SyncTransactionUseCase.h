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

DEFINE_USECASE_INPUT(SyncTransactionInput)


#define FIELDS_RESULT(X)       \
    X(bool, synced)

DEFINE_USECASE_RESULT(SyncTransactionResult)

class SyncTransactionUseCase : public core::usecase::AsyncUseCase<SyncTransactionUseCase, SyncTransactionInput, SyncTransactionResult> {
  public:
  core::usecase::Result<SyncTransactionResult> execute(const SyncTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases