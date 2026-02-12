#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)          \
    X(std::optional<QJsonObject>, json)\
    X(nunchuk::MasterSigner, signer)

DEFINE_USECASE_INPUT(UpdateMasterSignerInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::MasterSigner, signer)

DEFINE_USECASE_RESULT(UpdateMasterSignerResult)

class UpdateMasterSignerUseCase : public core::usecase::AsyncUseCase<UpdateMasterSignerUseCase, UpdateMasterSignerInput, UpdateMasterSignerResult> {
  protected:
  core::usecase::Result<UpdateMasterSignerResult> execute(const UpdateMasterSignerInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases