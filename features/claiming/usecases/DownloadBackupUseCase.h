#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::claiming::usecases {

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(QString, magic)                                                                                                                                          \
    X(QString, backupPwd)                                                                                                                                      \
    X(QString, backupPwdTwo)                                                                                                                                   \
    X(QString, keyName)                                                                                                                                        \
    X(QString, keyNameTwo)

DEFINE_USECASE_INPUT(DownloadBackupInput)

#define FIELDS_RESULT(X) X(std::vector<nunchuk::SingleSigner>, signers)

DEFINE_USECASE_RESULT(DownloadBackupResult)

class DownloadBackupUseCase : public core::usecase::AsyncUseCase<DownloadBackupUseCase, DownloadBackupInput, DownloadBackupResult> {
  protected:
    core::usecase::Result<DownloadBackupResult> execute(const DownloadBackupInput &input) override;
    std::optional<nunchuk::SingleSigner> downloadKey(const QJsonObject &key, const QString &backup_key, const QString &key_name);
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::claiming::usecases