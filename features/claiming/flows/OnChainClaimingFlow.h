// features/auth/OnChainClaimingFlow.h
#pragma once
#include "features/claiming/flows/ClaimingFlow.h"

namespace features::claiming::flows {
using core::flow::FlowContext;
class OnChainClaimingFlow : public ClaimingFlow {
    Q_OBJECT
  public:
    explicit OnChainClaimingFlow(FlowContext* ctx, QObject *parent = nullptr);
    QString id() const override {
        return "OnChainClaimingFlow";
    }
    void passClaimingError(int code, const QString& message);

    void proceedResult(const nunchuk::SingleSigner &single) override {};
    void proceedAfterFileImportColdcard(const std::vector<nunchuk::SingleSigner> &signers, const QString &signerName) override {};
    void proceedAfterQrImportColdcard(const std::vector<nunchuk::SingleSigner> &signers, const QString &signerName) override {};
    void proceedAfterSelectExistKey(const QString &xfp) override {};
    void proceedAfterRecoverViaSeed(const QString &xfp) override {};
    void proceedAfterRecoverViaXprv(const QString &xfp) override {};
    void proceedAfterAddedViaUSB(const QString &xfp) override {};
    void proceedClaimTransactionResult(const CreateTransactionResult &txData) override;
    bool isManualClaiming() override { return true; };
  public slots:
    void bind(QObject* vm) override;
  private:
    QString getTitleByErrorCode(int code);
  private:
    int m_errorCode{0};
    QString m_errorMessage;

};
} // namespace features::claiming::flows
