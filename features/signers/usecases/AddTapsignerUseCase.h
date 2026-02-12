#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)       \
    X(QString, signer_name)   \
    X(QString, signer_xfp)    \
    X(QString, card_id)       \
    X(QString, version)       \
    X(int, birth_height)  \
    X(bool, is_testnet)    \
    X(QJsonObject, tapsigner)

DEFINE_USECASE_INPUT(AddTapsignerInput)


#define FIELDS_RESULT(X)       \
    X(QJsonObject, tapsigner)

DEFINE_USECASE_RESULT(AddTapsignerResult)

class AddTapsignerUseCase : public core::usecase::AsyncUseCase<AddTapsignerUseCase, AddTapsignerInput, AddTapsignerResult> {
  protected:
  core::usecase::Result<AddTapsignerResult> execute(const AddTapsignerInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases