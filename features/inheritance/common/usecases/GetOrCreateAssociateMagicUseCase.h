#pragma once

#include "core/usecase/DefineUseCase.h"
#include <QJsonObject>
#include <QObject>
#include <QString>

namespace features::inheritance::usecases {

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(QString, wallet_id)                                                                                                                                      \
    X(QString, group_id)                                                                                                                                       \
    X(QJsonArray, beneficiaries)

DEFINE_USECASE_INPUT(GetOrCreateAssociateMagicInput)

#define FIELDS_RESULT(X) X(QJsonObject, output)

DEFINE_USECASE_RESULT(GetOrCreateAssociateMagicResult)

class GetOrCreateAssociateMagicUseCase
    : public core::usecase::AsyncUseCase<GetOrCreateAssociateMagicUseCase, GetOrCreateAssociateMagicInput, GetOrCreateAssociateMagicResult> {
  public:
    core::usecase::Result<GetOrCreateAssociateMagicResult> execute(const GetOrCreateAssociateMagicInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT

} // namespace features::inheritance::usecases
