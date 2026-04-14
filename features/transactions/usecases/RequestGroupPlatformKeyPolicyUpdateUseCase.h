#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                 \
    X(QString, wallet_id)               \
    X(nunchuk::GroupPlatformKeyPolicies, policies)

DEFINE_USECASE_INPUT(RequestGroupPlatformKeyPolicyUpdateInput)

#define FIELDS_RESULT(X)                                                                                                                                       \
    X(bool, requires_dummy_transaction)                                                                                                                        \
    X(std::optional<nunchuk::GroupDummyTransaction>, dumyTx)                                                                                                   \
    X(int, pending_signatures)

DEFINE_USECASE_RESULT(RequestGroupPlatformKeyPolicyUpdateResult)

class RequestGroupPlatformKeyPolicyUpdateUseCase
    : public core::usecase::AsyncUseCase<RequestGroupPlatformKeyPolicyUpdateUseCase, RequestGroupPlatformKeyPolicyUpdateInput,
                                         RequestGroupPlatformKeyPolicyUpdateResult> {
  protected:
    core::usecase::Result<RequestGroupPlatformKeyPolicyUpdateResult> execute(const RequestGroupPlatformKeyPolicyUpdateInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::transactions::usecases