#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QJsonArray>
#include <QObject>
#include <QString>
#include <nunchuk.h>
#include <vector>

namespace features::wallets::usecases {

#define FIELDS_INPUT(X) X(QString, wallet_id)

DEFINE_USECASE_INPUT(GetGroupWalletAlertsInput)

#define FIELDS_RESULT(X)                                                                                                                                       \
    X(QJsonArray, alerts)                                                                                                                                      \
    X(int, total_count)

DEFINE_USECASE_RESULT(GetGroupWalletAlertsResult)

class GetGroupWalletAlertsUseCase : public core::usecase::AsyncUseCase<GetGroupWalletAlertsUseCase, GetGroupWalletAlertsInput, GetGroupWalletAlertsResult> {
  public:
    core::usecase::Result<GetGroupWalletAlertsResult> execute(const GetGroupWalletAlertsInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::wallets::usecases
