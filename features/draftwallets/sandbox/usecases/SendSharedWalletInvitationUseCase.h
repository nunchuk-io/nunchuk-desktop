#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QJsonObject>
#include <QObject>
#include <QString>

namespace features::draftwallets::sandbox::usecases {

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(QStringList, emails)                                                                                                                                          \
    X(QString, group_id)

DEFINE_USECASE_INPUT(SendSharedWalletInvitationInput)

#define FIELDS_RESULT(X) X(QJsonObject, invitation)

DEFINE_USECASE_RESULT(SendSharedWalletInvitationResult)

class SendSharedWalletInvitationUseCase
    : public core::usecase::AsyncUseCase<SendSharedWalletInvitationUseCase, SendSharedWalletInvitationInput, SendSharedWalletInvitationResult> {
  protected:
    core::usecase::Result<SendSharedWalletInvitationResult> execute(const SendSharedWalletInvitationInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::draftwallets::sandbox::usecases
