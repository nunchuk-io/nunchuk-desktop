#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, keyName) \
    X(QString, xprv) \
    X(bool, isPrimaryKey) \
    X(bool, replace)

DEFINE_USECASE_INPUT(CreateSoftwareSignerXprvInput)


#define FIELDS_RESULT(X)       \
    X(nunchuk::MasterSigner, master)

DEFINE_USECASE_RESULT(CreateSoftwareSignerXprvResult)

class CreateSoftwareSignerXprvUseCase : public core::usecase::AsyncUseCase<CreateSoftwareSignerXprvUseCase, CreateSoftwareSignerXprvInput, CreateSoftwareSignerXprvResult> {
  protected:
  core::usecase::Result<CreateSoftwareSignerXprvResult> execute(const CreateSoftwareSignerXprvInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases