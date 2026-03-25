#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)          \
    X(QString, name)          \
    X(std::function<bool(int)>, progress) \
    X(nunchuk::Device, device)

DEFINE_USECASE_INPUT(CreateMasterSignerInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::MasterSigner, signer)

DEFINE_USECASE_RESULT(CreateMasterSignerResult)

class CreateMasterSignerUseCase : public core::usecase::AsyncUseCase<CreateMasterSignerUseCase, CreateMasterSignerInput, CreateMasterSignerResult> {
  protected:
  core::usecase::Result<CreateMasterSignerResult> execute(const CreateMasterSignerInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases