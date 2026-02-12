#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::wallets::usecases {

#define FIELDS_INPUT(X)                  \
    X(QString, wallet_id)

DEFINE_USECASE_INPUT(GetWalletInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::Wallet, wallet)

DEFINE_USECASE_RESULT(GetWalletResult)

class GetWalletUseCase : public core::usecase::AsyncUseCase<GetWalletUseCase, GetWalletInput, GetWalletResult> {
  protected:
  core::usecase::Result<GetWalletResult> execute(const GetWalletInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::wallets::usecases