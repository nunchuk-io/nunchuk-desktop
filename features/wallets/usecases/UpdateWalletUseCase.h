#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::wallets::usecases {

#define FIELDS_INPUT(X)              \
    X(QString, wallet_id)            \
    X(QString, name)                 \
    X(QString, description)        \
    X(QString, group_id)            \
    X(bool, isClaimed)

DEFINE_USECASE_INPUT(UpdateWalletInput)


#define FIELDS_RESULT(X)       \
    X(QString, dummy)

DEFINE_USECASE_RESULT(UpdateWalletResult)

class UpdateWalletUseCase : public core::usecase::AsyncUseCase<UpdateWalletUseCase, UpdateWalletInput, UpdateWalletResult> {
  public:
  core::usecase::Result<UpdateWalletResult> execute(const UpdateWalletInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::wallets::usecases