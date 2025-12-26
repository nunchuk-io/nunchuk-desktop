#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::claiming::usecases {

#define FIELDS_INPUT(X)          \
    X(QString, wallet_id)

DEFINE_USECASE_INPUT(GetClaimingWalletInput)

#define FIELDS_RESULT(X)       \
    X(QJsonObject, wallet)

DEFINE_USECASE_RESULT(GetClaimingWalletResult)

class GetClaimingWalletUseCase : public core::usecase::AsyncUseCase<GetClaimingWalletUseCase, GetClaimingWalletInput, GetClaimingWalletResult> {
  protected:
  core::usecase::Result<GetClaimingWalletResult> execute(const GetClaimingWalletInput &input) override;
};

} // namespace features::claiming::usecases