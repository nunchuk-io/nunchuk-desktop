#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>

namespace features::wallets::usecases {

#define FIELDS_INPUT(X) \
    X(QString, wallet_id) \
    X(QString, alert_id)

DEFINE_USECASE_INPUT(MarkGroupWalletAlertViewedInput)

#define FIELDS_RESULT(X) \
    X(QString, dummy)

DEFINE_USECASE_RESULT(MarkGroupWalletAlertViewedResult)

class MarkGroupWalletAlertViewedUseCase : public core::usecase::AsyncUseCase<MarkGroupWalletAlertViewedUseCase, MarkGroupWalletAlertViewedInput, MarkGroupWalletAlertViewedResult> {
  public:
    core::usecase::Result<MarkGroupWalletAlertViewedResult> execute(const MarkGroupWalletAlertViewedInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::wallets::usecases
