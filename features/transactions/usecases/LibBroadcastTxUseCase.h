#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(QString, wallet_id)                                                                                                                                      \
    X(QString, tx_id)

DEFINE_USECASE_INPUT(LibBroadcastTxInput)

#define FIELDS_RESULT(X) X(nunchuk::Transaction, tx)

DEFINE_USECASE_RESULT(LibBroadcastTxResult)

class LibBroadcastTxUseCase : public core::usecase::AsyncUseCase<LibBroadcastTxUseCase, LibBroadcastTxInput, LibBroadcastTxResult> {
  protected:
    core::usecase::Result<LibBroadcastTxResult> execute(const LibBroadcastTxInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases