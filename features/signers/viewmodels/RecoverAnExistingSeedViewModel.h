#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "features/signers/usecases/CreateSoftwareSignerUseCase.h"
#include "features/signers/usecases/SignerNammingUseCase.h"

namespace features::signers::viewmodels {
using core::viewmodels::ActionViewModel;
using features::signers::usecases::CreateSoftwareSignerUseCase;
using features::signers::usecases::SignerNammingUseCase;
class RecoverAnExistingSeedViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit RecoverAnExistingSeedViewModel(QObject *parent = nullptr);
    ~RecoverAnExistingSeedViewModel() override = default;

  public slots:
    void forceCreateMaster(QVariant msg);
    bool startCreateMaster(QVariant msg);
  signals:
    void notifySignerExist(bool isSoftware, const QString fingerPrint);
  private:
    CreateSoftwareSignerUseCase m_createSoftwareSignerUC;
    SignerNammingUseCase m_signerNammingUC;
};
} // namespace features::signers::viewmodels
