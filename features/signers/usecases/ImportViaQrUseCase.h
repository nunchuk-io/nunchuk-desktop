#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)             \
    X(QStringList, qr_data)

DEFINE_USECASE_INPUT(ImportViaQrInput)


#define FIELDS_RESULT(X)       \
    X(std::vector<nunchuk::SingleSigner>, signers)

DEFINE_USECASE_RESULT(ImportViaQrResult)

class ImportViaQrUseCase : public core::usecase::AsyncUseCase<ImportViaQrUseCase, ImportViaQrInput, ImportViaQrResult> {
  protected:
  core::usecase::Result<ImportViaQrResult> execute(const ImportViaQrInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases