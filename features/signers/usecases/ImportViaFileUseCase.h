#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)          \
    X(QString, filePath)    \
    X(nunchuk::SignerType, signer_type)

DEFINE_USECASE_INPUT(ImportViaFileInput)


#define FIELDS_RESULT(X)       \
  X(std::vector<nunchuk::SingleSigner>, signers)

DEFINE_USECASE_RESULT(ImportViaFileResult)

class ImportViaFileUseCase : public core::usecase::AsyncUseCase<ImportViaFileUseCase, ImportViaFileInput, ImportViaFileResult> {
  protected:
  core::usecase::Result<ImportViaFileResult> execute(const ImportViaFileInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases