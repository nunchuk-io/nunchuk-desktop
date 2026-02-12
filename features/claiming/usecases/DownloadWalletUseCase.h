#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"

namespace features::claiming::usecases {

#define FIELDS_INPUT(X)          \
    X(QString, magic)            \
    X(QJsonArray, keys)          \
    X(bool, force)

DEFINE_USECASE_INPUT(DownloadWalletInput)

class DownloadWalletUseCase : public core::usecase::AsyncUseCase<DownloadWalletUseCase, DownloadWalletInput, QJsonObject> {
  protected:
  core::usecase::Result<QJsonObject> execute(const DownloadWalletInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::claiming::usecases