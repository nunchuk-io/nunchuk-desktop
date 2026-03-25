#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(std::optional<QString>, wallet_id)                                                                                                                       \
    X(std::optional<nunchuk::Wallet>, wallet)                                                                                                                  \
    X(std::optional<QString>, tx_id)                                                                                                                           \
    X(std::optional<nunchuk::Transaction>, tx)                                                                                                                 \
    X(std::vector<nunchuk::Device>, devices)

DEFINE_USECASE_INPUT(LibSignTransactionInput)

#define FIELDS_RESULT(X) X(nunchuk::Transaction, tx)

DEFINE_USECASE_RESULT(LibSignTransactionResult)

class LibSignTransactionUseCase : public core::usecase::AsyncUseCase<LibSignTransactionUseCase, LibSignTransactionInput, LibSignTransactionResult> {
  protected:
    core::usecase::Result<LibSignTransactionResult> execute(const LibSignTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases