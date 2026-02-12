#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)          \
    X(QString, name)          \
    X(QString, xpub)          \
    X(QString, publicKey)     \
    X(QString, derivationPath)\
    X(QString, masterFingerprint)\
    X(nunchuk::SignerType, type)  \
    X(std::vector<nunchuk::SignerTag>, tags)\
    X(bool, replaceExisting)

DEFINE_USECASE_INPUT(CreateSignerInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::SingleSigner, signer)

DEFINE_USECASE_RESULT(CreateSignerResult)

class CreateSignerUseCase : public core::usecase::AsyncUseCase<CreateSignerUseCase, CreateSignerInput, CreateSignerResult> {
  protected:
  core::usecase::Result<CreateSignerResult> execute(const CreateSignerInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases