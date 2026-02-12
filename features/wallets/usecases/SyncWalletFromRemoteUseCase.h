#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>
#include "core/ui/UiForward.h"

namespace features::wallets::usecases {
  #define FIELDS_INPUT(X)                  \
    X(bool, input)

DEFINE_USECASE_INPUT(SyncWalletFromRemoteInput)


#define FIELDS_RESULT(X)       \
  X(bool, ret)

DEFINE_USECASE_RESULT(SyncWalletFromRemoteResult)

class SyncWalletFromRemoteUseCase : public core::usecase::AsyncUseCase<SyncWalletFromRemoteUseCase, SyncWalletFromRemoteInput, SyncWalletFromRemoteResult> {
  public:
    core::usecase::Result<SyncWalletFromRemoteResult> execute(const SyncWalletFromRemoteInput &input) override;
    void syncWalletFromRemote(const QJsonObject &walletData);
    void syncSignerFromRemote(const QJsonArray &signers, std::optional<nunchuk::Wallet> local_wallet);
    void syncSignerFromRemote(const QJsonObject &signerData, std::optional<nunchuk::Wallet> local_wallet);
    void walletCached(const QJsonObject &walletData);
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::wallets::usecases