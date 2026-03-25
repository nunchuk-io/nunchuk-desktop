#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QMap>
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::signers::usecases {

using SignatureMap = QMap<QString, QString>;

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(QString, message)                                                                                                                                        \
    X(std::vector<nunchuk::SingleSigner>, signers)

DEFINE_USECASE_INPUT(SignMessageInput)

#define FIELDS_RESULT(X) X(SignatureMap, signatures)

DEFINE_USECASE_RESULT(SignMessageResult)

class SignMessageUseCase : public core::usecase::AsyncUseCase<SignMessageUseCase, SignMessageInput, SignMessageResult> {
  protected:
    core::usecase::Result<SignMessageResult> execute(const SignMessageInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases