#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>

namespace features::wallets::usecases {

#define FIELDS_INPUT(X) \
    X(QString, wallet_id) \
    X(QString, alert_id)

DEFINE_USECASE_INPUT(DismissGroupWalletAlertInput)

#define FIELDS_RESULT(X) \
    X(QString, dummy)

DEFINE_USECASE_RESULT(DismissGroupWalletAlertResult)

class DismissGroupWalletAlertUseCase : public core::usecase::AsyncUseCase<DismissGroupWalletAlertUseCase, DismissGroupWalletAlertInput, DismissGroupWalletAlertResult> {
  public:
    core::usecase::Result<DismissGroupWalletAlertResult> execute(const DismissGroupWalletAlertInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::wallets::usecases
