#pragma once
#include "core/ui/UiForward.h"
#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::signers::usecases {

class SignerNammingUseCase : public core::usecase::AsyncUseCase<SignerNammingUseCase, QString, QString> {
  public:
    SignerNammingUseCase &addParameter(AppModel *model);
    QString getInheritanceNameKey();
    QString getNormalNameKey(const QString &baseName);

  protected:
    core::usecase::Result<QString> execute(const QString &input) override;
    AppModel *m_model{nullptr};
    int m_nextInheritanceKeyIndex{-1};
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::signers::usecases