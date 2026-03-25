#pragma once
#include "TypeDefine.h"
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"
#include "features/signers/usecases/CreateSoftwareSignerXprvUseCase.h"
#include "features/signers/usecases/SignerNammingUseCase.h"

namespace features::signers::viewmodels {
using features::signers::usecases::CreateSoftwareSignerXprvUseCase;
using features::signers::usecases::SignerNammingUseCase;
class RecoverViaXPRVViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, xprv)
    DEFINE_QT_PROPERTY(int, walletType)
    DEFINE_QT_PROPERTY(int, addressType)
  public:
    explicit RecoverViaXPRVViewModel(QObject *parent = nullptr);

  public slots:
    bool validateXPRV(const QString xprv);
    void requestCreateSigner();
    void checkSignerExist();
  signals:
    void notifySignerExist(bool isSoftware, const QString fingerPrint);
  private:
    CreateSoftwareSignerXprvUseCase m_createSoftwareSignerUC;
    SignerNammingUseCase m_signerNammingUC;
};
} // namespace features::signers::viewmodels
