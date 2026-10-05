// features/auth/OffChainClaimingFlow.h
#pragma once
#include "features/claiming/flows/ClaimingFlow.h"
#include <QMap>

namespace features::claiming::flows {
using core::flow::FlowContext;

class OffChainClaimingFlow : public ClaimingFlow {
    Q_OBJECT
  public:
    explicit OffChainClaimingFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "OffChainClaimingFlow";
    }
    DEFINE_SET_GET(nunchuk::SingleSigner, currentSigner)
    DEFINE_SET_GET(QJsonArray, keyOrigins)
    // Cached signing-challenge message - fetched once for the first key, reused for the rest of the
    // keys in this claim session (see VerifyInheritanceKeyViewModel::onInit()).
    DEFINE_SET_GET(QString, challengeMessage)
    DEFINE_SET_GET(QString, challengeMessageId)

    void proceedResult(const nunchuk::SingleSigner &single) override;
    void proceedAfterFileImportColdcard(const std::vector<nunchuk::SingleSigner> &signers, const QString &signerName) override;
    void proceedAfterQrImportColdcard(const std::vector<nunchuk::SingleSigner> &signers, const QString &signerName) override;
    void proceedAfterSelectExistKey(const QString &xfp) override;
    void proceedAfterRecoverViaSeed(const QString &xfp) override;
    void proceedAfterRecoverViaXprv(const QString &xfp) override;
    void proceedAfterAddedViaUSB(const QString &xfp) override;
    bool isCorrectXFP(const QString &xfp);
    int expectedAccountIndex(const QString &xfp);
    void addKey(const QString &xfp);
    bool isAllKeysAdded();
    void proceedClaimTransactionResult(const CreateTransactionResult &txData) override;
    bool isManualClaiming() override;
  public slots:
    void bind(QObject *vm) override;
    void rollbackSigner();
};
} // namespace features::claiming::flows
