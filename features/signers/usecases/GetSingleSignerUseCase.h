#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>
#include "core/ui/UiForward.h"

namespace features::signers::usecases {

#define FIELDS_INPUT(X)                   \
    X(QString, xfp)   \
    X(nunchuk::WalletType, wallet_type)   \
    X(nunchuk::AddressType, address_type)   \
    X(int, account_index)

DEFINE_USECASE_INPUT(GetSingleSignerInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::SingleSigner, signer)

DEFINE_USECASE_RESULT(GetSingleSignerResult)

class GetSingleSignerUseCase : public core::usecase::AsyncUseCase<GetSingleSignerUseCase, GetSingleSignerInput, GetSingleSignerResult> {
  protected:
    core::usecase::Result<GetSingleSignerResult> execute(const GetSingleSignerInput &input) override;
  public:

  private:
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases