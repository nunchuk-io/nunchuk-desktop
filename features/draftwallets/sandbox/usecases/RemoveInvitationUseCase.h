#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QJsonObject>
#include <QObject>
#include <QString>

namespace features::draftwallets::sandbox::usecases {

#define FIELDS_INPUT(X) X(QString, invitation_id)

DEFINE_USECASE_INPUT(RemoveInvitationInput)

#define FIELDS_RESULT(X) X(bool, success)

DEFINE_USECASE_RESULT(RemoveInvitationResult)

class RemoveInvitationUseCase : public core::usecase::AsyncUseCase<RemoveInvitationUseCase, RemoveInvitationInput, RemoveInvitationResult> {
  protected:
    core::usecase::Result<RemoveInvitationResult> execute(const RemoveInvitationInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::draftwallets::sandbox::usecases
