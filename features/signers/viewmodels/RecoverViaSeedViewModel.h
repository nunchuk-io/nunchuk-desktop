#pragma once
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"
#include "features/signers/usecases/CreateSoftwareSignerUseCase.h"
#include "features/signers/usecases/SignerNammingUseCase.h"
#include "features/signers/usecases/SuggestMnemonicsUseCase.h"

namespace features::signers::viewmodels {
using features::signers::usecases::CreateSoftwareSignerUseCase;
using features::signers::usecases::SignerNammingUseCase;
using features::signers::usecases::SuggestMnemonicsUsecase;
class RecoverViaSeedViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, mnemonic)
    DEFINE_QT_PROPERTY(int, walletType)
    DEFINE_QT_PROPERTY(int, addressType)
    DEFINE_QT_PROPERTY(QStringList, suggestMnemonics)
  public:
    explicit RecoverViaSeedViewModel(QObject *parent = nullptr);
    void onInit() override;
  public slots:
    void requestCreateSigner();
    void checkSignerExist();
  signals:
    void notifySignerExist(bool isSoftware, const QString fingerPrint);
  private:
    CreateSoftwareSignerUseCase m_createSoftwareSignerUC;
    SignerNammingUseCase m_signerNammingUC;
    SuggestMnemonicsUsecase m_suggestMnemonicsUC;
};
} // namespace features::signers::viewmodels
