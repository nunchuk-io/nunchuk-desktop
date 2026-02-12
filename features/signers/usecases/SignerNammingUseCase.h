#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>
#include "core/ui/UiForward.h"

namespace features::signers::usecases {

class SignerNammingUseCase : public core::usecase::AsyncUseCase<SignerNammingUseCase, QString, QString> {
  public:
    SignerNammingUseCase &addParameter(AppModel *model);
    QString getInheritanceNameKey();

  protected:
    core::usecase::Result<QString> execute(const QString &input) override;
    AppModel *m_model{nullptr};
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases