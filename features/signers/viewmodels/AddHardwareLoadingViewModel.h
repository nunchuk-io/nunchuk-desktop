#pragma once
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"
#include "features/signers/usecases/CreateMasterSignerUseCase.h"
#include "features/signers/usecases/SignerNammingUseCase.h"

namespace features::signers::viewmodels {
using features::signers::usecases::CreateMasterSignerUseCase;
using features::signers::usecases::SignerNammingUseCase;
class AddHardwareLoadingViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(int, proceedLoadingStep)
  public:
    explicit AddHardwareLoadingViewModel(QObject *parent = nullptr);
    void proceedAddSignerViaUSB(const nunchuk::Device& device);
    void setupProgressTitle();
  public slots:
    void slotProceedAddSigner(int progress) override;
    void onInit() override;
  private:
    CreateMasterSignerUseCase m_createMasterSignerUC;
    SignerNammingUseCase m_signerNammingUC;
};
} // namespace features::signers::viewmodels
