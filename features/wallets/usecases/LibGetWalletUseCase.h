#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::wallets::usecases {

#define FIELDS_INPUT(X)                  \
    X(QString, wallet_id)

DEFINE_USECASE_INPUT(LibGetWalletInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::Wallet, wallet)

DEFINE_USECASE_RESULT(LibGetWalletResult)

class LibGetWalletUseCase : public core::usecase::AsyncUseCase<LibGetWalletUseCase, LibGetWalletInput, LibGetWalletResult> {
  protected:
  core::usecase::Result<LibGetWalletResult> execute(const LibGetWalletInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::wallets::usecases