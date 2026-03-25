// features/auth/KeyProceedFlow.h
#pragma once
#include "core/flow/BaseFlow.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include <nunchuk.h>
#include "features/signers/usecases/GetSingleSignerUseCase.h"
#include "features/signers/usecases/SignerNammingUseCase.h"

namespace features::signers::flows {
using core::flow::FlowContext;
using features::signers::usecases::GetSingleSignerUseCase;
using features::signers::usecases::SignerNammingUseCase;
class KeyProceedFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit KeyProceedFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "KeyProceedFlow";
    }
    DEFINE_SET_GET(nunchuk::WalletType, walletType)
    DEFINE_SET_GET(nunchuk::AddressType, addressType)
    DEFINE_SET_GET(int, accountIndex)
    virtual void proceedResult(const nunchuk::SingleSigner& single) = 0;
    virtual void proceedAfterFileImportColdcard(const std::vector<nunchuk::SingleSigner>& signers, const QString& signerName) = 0;
    virtual void proceedAfterQrImportColdcard(const std::vector<nunchuk::SingleSigner>& signers, const QString& signerName) = 0;
    virtual void proceedAfterSelectExistKey(const QString& xfp) = 0;
    virtual void proceedAfterRecoverViaSeed(const QString& xfp) = 0;
    virtual void proceedAfterRecoverViaXprv(const QString& xfp) = 0;
    virtual void proceedAfterAddedViaUSB(const QString& xfp) = 0;
    void requestGetSigner(const QString& xfp);
    QString getSignerName(const QString& signerName);
  private:
    GetSingleSignerUseCase m_getSingleSignerUC;
    SignerNammingUseCase m_signerNammingUC;
};
} // namespace features::signers::flows
