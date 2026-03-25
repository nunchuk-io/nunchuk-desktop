#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X) \
    X(QString, dummy)

DEFINE_USECASE_INPUT(SuggestMnemonicsInput)


#define FIELDS_RESULT(X)       \
    X(QStringList, suggests)

DEFINE_USECASE_RESULT(SuggestMnemonicsResult)

class SuggestMnemonicsUsecase : public core::usecase::AsyncUseCase<SuggestMnemonicsUsecase, SuggestMnemonicsInput, SuggestMnemonicsResult> {
  protected:
  core::usecase::Result<SuggestMnemonicsResult> execute(const SuggestMnemonicsInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases