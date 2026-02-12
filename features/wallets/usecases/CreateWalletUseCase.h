#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::wallets::usecases {
using nunchuk::Wallet;
using nunchuk::WalletTemplate;
using nunchuk::WalletType;
using nunchuk::SingleSigner;
using nunchuk::AddressType;

#define FIELDS_INPUT(X)                  \
    /* ===== Common ===== */             \
    X(bool, allowUsedSigner)             \
    X(std::string, decoyPin)             \
    X(WalletTemplate, walletTemplate)    \
    /* ===== Mode: FromParams ===== */   \
    X(std::string, name)                 \
    X(int, m)                            \
    X(int, n)                            \
    X(std::vector<SingleSigner>, signers)\
    X(AddressType, addressType)          \
    X(std::optional<bool>, isEscrow)     \
    X(std::optional<WalletType>, walletType)\
    X(std::string, description)          \
    /* ===== Mode: FromExistingWallet ===== */ \
    X(std::optional<Wallet>, sourceWallet)

DEFINE_USECASE_INPUT(CreateWalletInput)


#define FIELDS_RESULT(X)       \
    X(std::optional<Wallet>, wallet)

DEFINE_USECASE_RESULT(CreateWalletResult)

class CreateWalletUseCase : public core::usecase::AsyncUseCase<CreateWalletUseCase, CreateWalletInput, CreateWalletResult> {
  protected:
  core::usecase::Result<CreateWalletResult> execute(const CreateWalletInput &input) override;

  std::optional<Wallet> createFromParams(const CreateWalletInput& req);
  std::optional<Wallet> createFromWallet(const CreateWalletInput& req);
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::wallets::usecases