#pragma once
#include "TypeDefine.h"
#include "HardwareRefreshDevicesViewModel.h"
#include "features/signers/usecases/SignerNammingUseCase.h"

namespace features::signers::viewmodels {
using features::signers::usecases::SignerNammingUseCase;
class ColdcardRefreshDevicesViewModel : public HardwareRefreshDevicesViewModel {
    Q_OBJECT
  public:
    explicit ColdcardRefreshDevicesViewModel(QObject *parent = nullptr);

  public slots:
    void onViaFileClicked();
    void onViaQRClicked();
    void onViaUSBClicked();
  signals:
  private:
    SignerNammingUseCase m_signerNammingUC;
};
} // namespace features::signers::viewmodels
