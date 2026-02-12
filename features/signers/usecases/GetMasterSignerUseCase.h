#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)          \
    X(QString, id)          

DEFINE_USECASE_INPUT(GetMasterSignerInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::MasterSigner, signer)

DEFINE_USECASE_RESULT(GetMasterSignerResult)

class GetMasterSignerUseCase : public core::usecase::AsyncUseCase<GetMasterSignerUseCase, GetMasterSignerInput, GetMasterSignerResult> {
  protected:
  core::usecase::Result<GetMasterSignerResult> execute(const GetMasterSignerInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases