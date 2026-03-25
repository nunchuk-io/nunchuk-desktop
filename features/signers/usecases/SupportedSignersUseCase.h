#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>
#include "core/ui/UiForward.h"

namespace features::signers::usecases {

#define FIELDS_INPUT(X)                   \
    X(nunchuk::WalletType, wallet_type)   \
    X(std::vector<nunchuk::AddressType>, address_types)   \
    X(nunchuk::SignerType, signer_type)   \
    X(std::vector<nunchuk::SignerTag>, signer_tags)

DEFINE_USECASE_INPUT(SupportedSignersInput)


#define FIELDS_RESULT(X)       \
    X(std::vector<nunchuk::SingleSigner>, signers)

DEFINE_USECASE_RESULT(SupportedSignersResult)

class SupportedSignersUseCase : public core::usecase::AsyncUseCase<SupportedSignersUseCase, SupportedSignersInput, SupportedSignersResult> {
  protected:
    core::usecase::Result<SupportedSignersResult> execute(const SupportedSignersInput &input) override;
  public:
    SupportedSignersUseCase &addParameter(AppModel *model);
    bool isTag(const nunchuk::SingleSigner& signer, const nunchuk::SignerTag& tag);
    bool isType(const nunchuk::SingleSigner& signer, const nunchuk::SignerType& type);

  private:
    AppModel *m_model{nullptr};
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases