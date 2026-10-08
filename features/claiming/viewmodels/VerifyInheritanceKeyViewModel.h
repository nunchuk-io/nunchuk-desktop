#pragma once

#include "Models/SingleSignerModel.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/claiming/usecases/SigningChallengeUseCase.h"
#include "features/signers/usecases/SignMessageUseCase.h"
#include <nunchuk.h>

namespace features::claiming::viewmodels {
using core::viewmodels::ActionViewModel;
using features::claiming::usecases::SigningChallengeUseCase;
using features::signers::usecases::SignMessageUseCase;
class VerifyInheritanceKeyViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY_PTR(SingleSignerListModel, signerList)
    DEFINE_QT_PROPERTY(QString, message)
    DEFINE_QT_PROPERTY(QStringList, qrExported)
    DEFINE_QT_PROPERTY(QString, currentXfp)
    DEFINE_QT_PROPERTY(bool, isVerified)
    DEFINE_QT_PROPERTY(bool, isMoreShow)
  public:
    explicit VerifyInheritanceKeyViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(QString, magicWord);
    DEFINE_SET_GET(nunchuk::SingleSigner, currentSigner);
    DEFINE_SET_GET(nunchuk::WalletType, walletType)
    DEFINE_SET_GET(nunchuk::AddressType, addressType)
    DEFINE_SET_GET(QString, messageId)

    void proceedVerification(const nunchuk::SingleSigner &signer);
    void initializeChallengeMessage();
    void onInit() override;
    void createTokenAfterSign(const QString &sig);
  public slots:
    void exportFileSignMessage(const QString &filename);
    void exportQRSignMessage();
    void importFileSignedMessage(const QString &filename);
    void importQRSignedMessage(const QStringList &qrData);
    void back() override;
    void next() override;
    void onSignClicked();
  signals:
    void signalBack();

  private:
    SigningChallengeUseCase m_signingChallengeUC;
    SignMessageUseCase m_signMessageUC;
    // Guards against re-entrant onSignClicked() calls: m_signMessageUC's underlying WorkerConcurrent
    // silently drops a 2nd request while the 1st is in flight (run() returns false, callback never
    // stored), so the 2nd setOverrideCursor() would never get its matching restoreOverrideCursor(),
    // leaving the wait cursor stuck even after the 1st request completes successfully.
    bool m_isSigning{false};
};
} // namespace features::claiming::viewmodels