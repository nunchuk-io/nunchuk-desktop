#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::wallets::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, wallet_id)     \
    X(QString, group_id)      \
    X(bool, isClaimed)        \
    X(QString, secQuesToken)  \
    X(QString, passwordToken)

DEFINE_USECASE_INPUT(DeleteWalletInput)


#define FIELDS_RESULT(X)       \
    X(QString, dummy)

DEFINE_USECASE_RESULT(DeleteWalletResult)

class DeleteWalletUseCase : public core::usecase::AsyncUseCase<DeleteWalletUseCase, DeleteWalletInput, DeleteWalletResult> {
  public:
  core::usecase::Result<DeleteWalletResult> execute(const DeleteWalletInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::wallets::usecases