#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                  \
    X(bool, isLoginRequired)    \
    X(QString, wallet_id)       \
    X(QString, address)

DEFINE_USECASE_INPUT(AddressToVerifyInput)


#define FIELDS_RESULT(X)       \
    X(bool, synced)

DEFINE_USECASE_RESULT(AddressToVerifyResult)

class AddressToVerifyUseCase : public core::usecase::AsyncUseCase<AddressToVerifyUseCase, AddressToVerifyInput, AddressToVerifyResult> {
  public:
  core::usecase::Result<AddressToVerifyResult> execute(const AddressToVerifyInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases