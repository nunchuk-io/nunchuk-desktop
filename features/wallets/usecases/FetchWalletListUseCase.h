#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::wallets::usecases {

#define FIELDS_INPUT(X)                  \


DEFINE_USECASE_INPUT(FetchWalletListInput)


#define FIELDS_RESULT(X)       \
    X(QJsonArray, walletList)

DEFINE_USECASE_RESULT(FetchWalletListResult)

class FetchWalletListUseCase : public core::usecase::AsyncUseCase<FetchWalletListUseCase, FetchWalletListInput, FetchWalletListResult> {
  public:
    core::usecase::Result<FetchWalletListResult> execute(const FetchWalletListInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::wallets::usecases