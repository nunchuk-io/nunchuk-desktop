#include "CreateWalletUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::wallets::usecases {
using namespace core::usecase;

Result<CreateWalletResult> CreateWalletUseCase::execute(const CreateWalletInput &input) {
    CreateWalletResult result;
    std::optional<Wallet> createdWallet;
    if (input.sourceWallet.has_value()) {
        createdWallet = createFromWallet(input);
    } else {
        createdWallet = createFromParams(input);
    }

    if (createdWallet.has_value()) {
        result.wallet = createdWallet;
        return Result<CreateWalletResult>::success(result);
    } else {
        return Result<CreateWalletResult>::failure(QString("Failed to create wallet"));
    }
}

std::optional<Wallet> CreateWalletUseCase::createFromParams(const CreateWalletInput &req) {
    if (req.isEscrow.has_value()) {
        bool isEscrow = req.isEscrow.value();
        return bridge::wallet::CreateWallet(
            req.name,
            req.m,
            req.n,
            req.signers,
            req.addressType,
            isEscrow,
            req.description,
            req.allowUsedSigner,
            req.decoyPin,
            req.walletTemplate
        );
    } else if (req.walletType.has_value()) {
        WalletType walletType = req.walletType.value();
        QWarningMessage msg;
        return bridge::wallet::CreateWallet(
            req.name,
            req.m,
            req.n,
            req.signers,
            req.addressType,
            walletType,
            req.description,
            req.allowUsedSigner,
            req.decoyPin,
            req.walletTemplate
        );
    }
    return std::nullopt;
}

std::optional<Wallet> CreateWalletUseCase::createFromWallet(const CreateWalletInput &req) {
    if (req.sourceWallet.has_value()) {
        Wallet newWallet = req.sourceWallet.value();
        QWarningMessage msg;
        return bridge::wallet::CreateWallet(
            newWallet,
            req.allowUsedSigner,
            req.decoyPin
        );
    }
    return std::nullopt;
}

} // namespace features::wallets::usecases