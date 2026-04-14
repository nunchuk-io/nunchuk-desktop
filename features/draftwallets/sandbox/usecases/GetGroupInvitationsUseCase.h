#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QString>

namespace features::draftwallets::sandbox::usecases {

#define FIELDS_INPUT(X) X(QString, group_id)

DEFINE_USECASE_INPUT(GetGroupInvitationsInput)

#define FIELDS_RESULT(X) X(QJsonArray, invitations)

DEFINE_USECASE_RESULT(GetGroupInvitationsResult)

class GetGroupInvitationsUseCase : public core::usecase::AsyncUseCase<GetGroupInvitationsUseCase, GetGroupInvitationsInput, GetGroupInvitationsResult> {
  protected:
    core::usecase::Result<GetGroupInvitationsResult> execute(const GetGroupInvitationsInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::draftwallets::sandbox::usecases
