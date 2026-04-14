#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>
#include <optional>

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(QString, wallet_id)                                                                                                                                      \
    X(nunchuk::GroupPlatformKeyPolicies, policies)

DEFINE_USECASE_INPUT(PreviewGroupPlatformKeyPolicyUpdateInput)

#define FIELDS_RESULT(X)                                                                                                                                       \
    X(bool, requires_dummy_transaction)                                                                                                                        \
    X(std::optional<nunchuk::GroupDummyTransaction>, dumyTx)                                                                                                   \
    X(int, pending_signatures)                                                                                                                                 \
    X(int, delay_apply_in_seconds)

DEFINE_USECASE_RESULT(PreviewGroupPlatformKeyPolicyUpdateResult)

class PreviewGroupPlatformKeyPolicyUpdateUseCase
    : public core::usecase::AsyncUseCase<PreviewGroupPlatformKeyPolicyUpdateUseCase, PreviewGroupPlatformKeyPolicyUpdateInput,
                                         PreviewGroupPlatformKeyPolicyUpdateResult> {
  protected:
    core::usecase::Result<PreviewGroupPlatformKeyPolicyUpdateResult> execute(const PreviewGroupPlatformKeyPolicyUpdateInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::transactions::usecases
