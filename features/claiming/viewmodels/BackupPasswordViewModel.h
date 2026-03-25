#pragma once

#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/claiming/usecases/DownloadBackupUseCase.h"
#include "features/claiming/usecases/SigningChallengeUseCase.h"
#include "features/claiming/viewmodels/BaseClaimingViewModel.h"
#include "features/signers/usecases/SignMessageUseCase.h"
#include "features/signers/usecases/SignerNammingUseCase.h"

namespace features::claiming::viewmodels {
using features::claiming::usecases::DownloadBackupUseCase;
using features::claiming::usecases::SigningChallengeUseCase;
using features::claiming::viewmodels::BaseClaimingViewModel;
using features::signers::usecases::SignerNammingUseCase;
using features::signers::usecases::SignMessageUseCase;
class BackupPasswordViewModel : public BaseClaimingViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, backupPwd)
    DEFINE_QT_PROPERTY(QString, backupPwdTwo)
    DEFINE_QT_PROPERTY(int, remainingCount)
  public:
    explicit BackupPasswordViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(QString, magicWord);
    DEFINE_SET_GET(QString, messageId)
    DEFINE_SET_GET(QString, message)
    DEFINE_SET_GET(QString, bsms)

    void initializeChallengeMessage();
    void startDownloadBackup();
    void createTokenForBackupPassword();
    void handleClaim();
  public slots:
    void onInit() override;
    void next() override;

  private:
    SigningChallengeUseCase m_signingChallengeUC;
    SignMessageUseCase m_signMessageUC;
    DownloadBackupUseCase m_downloadBackupUC;
    SignerNammingUseCase m_signerNammingUC;
};
} // namespace features::claiming::viewmodels