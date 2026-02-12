#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, keyName) \
    X(QString, mnemonic) \
    X(QString, passphrase) \
    X(bool, isPrimaryKey) \
    X(bool, replace)

DEFINE_USECASE_INPUT(CreateSoftwareInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::MasterSigner, master)

DEFINE_USECASE_RESULT(CreateSoftwareResult)

class CreateSoftwareSignerUseCase : public core::usecase::AsyncUseCase<CreateSoftwareSignerUseCase, CreateSoftwareInput, CreateSoftwareResult> {
  protected:
  core::usecase::Result<CreateSoftwareResult> execute(const CreateSoftwareInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases