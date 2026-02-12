#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)          \
    X(std::optional<QJsonObject>, json)\
    X(nunchuk::SingleSigner, signer)

DEFINE_USECASE_INPUT(UpdateRemoteSignerInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::SingleSigner, signer)

DEFINE_USECASE_RESULT(UpdateRemoteSignerResult)

class UpdateRemoteSignerUseCase : public core::usecase::AsyncUseCase<UpdateRemoteSignerUseCase, UpdateRemoteSignerInput, UpdateRemoteSignerResult> {
  protected:
  core::usecase::Result<UpdateRemoteSignerResult> execute(const UpdateRemoteSignerInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases