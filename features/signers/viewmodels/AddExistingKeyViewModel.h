#pragma once
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"
#include "Models/SingleSignerModel.h"
#include "features/signers/usecases/SupportedSignersUseCase.h"

namespace features::signers::viewmodels {
using features::signers::usecases::SupportedSignersUseCase;
class AddExistingKeyViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY_PTR(SingleSignerListModel, signerList)
    DEFINE_QT_PROPERTY(int, accountIndex)
  public:
    explicit AddExistingKeyViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(nunchuk::SignerType, signerType)
    DEFINE_SET_GET(nunchuk::WalletType, walletType)
    DEFINE_SET_GET(std::vector<nunchuk::AddressType>, addressTypes)
    DEFINE_SET_GET(nunchuk::SingleSigner, selectedSigner)
    void createExistingKey();
  protected:
    void onInit() override;
  public slots:
    void selectExistingKey(const QString& xfp, const int account_index);
  protected:
    SupportedSignersUseCase m_supportedSignersUC;
};
} // namespace features::signers::viewmodels
