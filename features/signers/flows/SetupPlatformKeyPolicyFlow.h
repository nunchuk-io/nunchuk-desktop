#pragma once
#include "core/flow/BaseFlow.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/transactions/usecases/PreviewGroupPlatformKeyPolicyUpdateUseCase.h"
#include "features/transactions/usecases/RequestGroupPlatformKeyPolicyUpdateUseCase.h"

namespace features::signers::flows {
using core::flow::FlowContext;
using features::transactions::usecases::PreviewGroupPlatformKeyPolicyUpdateUseCase;
using features::transactions::usecases::RequestGroupPlatformKeyPolicyUpdateUseCase;

class SetupPlatformKeyPolicyFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit SetupPlatformKeyPolicyFlow(FlowContext *ctx, QObject *parent = nullptr);

    QString id() const override {
        return "SetupPlatformKeyPolicyFlow";
    }

    DEFINE_SET_GET(QString, selectedPolicyId)
    DEFINE_SET_GET(QString, groupId)
    DEFINE_SET_GET(QVariantMap, policy)
    DEFINE_SET_GET(QVariantList, perKeyPolicies)
    DEFINE_SET_GET(bool, hasAnyChanged)
    DEFINE_SET_GET(QString, walletId)
    DEFINE_SET_GET(QVariant, indexOrKey)
    DEFINE_SET_GET(nunchuk::GroupDummyTransaction, dummyTx)
    DEFINE_SET_GET(int, pendingSignatures)
    DEFINE_SET_GET(bool, requiresDummyTransaction)
    DEFINE_SET_GET(int, afterHours)

    bool SetGroupGlobalPlatformKeyPolicies(const QVariantMap &policy);
    bool SetGroupPerkeyPlatformKeyPolicies(const QVariantList &perkey_policies);

    void PreviewGroupPlatformKeyPolicyUpdate();
    void RequestGroupPlatformKeyPolicyUpdate();
    void RequestGroupPlatformKeyPolicyUpdateLater();
    std::optional<nunchuk::GroupPlatformKeyPolicies> getPoliciesFromWalletConfig();

    void checkingPolicyChangeWithDummyTransaction();
  public slots:
    void bind(QObject *vm) override;
  signals:
    void startDummyTransaction();
    void startConfirmDummyTransaction();

  private:
    void replacePolicy(const QVariantMap &newPolicy);

    PreviewGroupPlatformKeyPolicyUpdateUseCase m_previewGroupPlatformKeyPolicyUpdateUC;
    RequestGroupPlatformKeyPolicyUpdateUseCase m_requestGroupPlatformKeyPolicyUpdateUC;
};
namespace helper {
QVariantMap platformKeyPolicyToVariantMap(const nunchuk::GroupPlatformKeyPolicy &policy);
nunchuk::GroupPlatformKeyPolicies globalPolicyMapToPolicies(const QVariantMap &policy);
nunchuk::GroupPlatformKeyPolicies perkeyPolicyListToPolicies(const QVariantList &perkey_policies);

QVariantList perKeyPolicyToVariantListNormalWallet(const std::vector<nunchuk::SingleSigner> &signers,
                                                   const std::vector<nunchuk::GroupPlatformKeySignerPolicy> &policies, const int platformkey_index);

QVariantList perKeyPolicyToVariantListMiniscriptWallet(const std::map<std::string, nunchuk::SingleSigner> &signers,
                                                       std::vector<nunchuk::GroupPlatformKeySignerPolicy> policies, QStringList platform_key_slots);

QVariantMap singleSignerToVariantMap(const nunchuk::SingleSigner &signer);
QVariantMap platformKeySignerPolicyToVariantMap(const nunchuk::GroupPlatformKeySignerPolicy &signerPolicy);
nunchuk::GroupPlatformKeySignerPolicy filterPolicy(const std::vector<nunchuk::GroupPlatformKeySignerPolicy> &signers_policies, const QString &xfp);
QVariantMap defaultGlobalPolicy();
QVariantList defaultPerkeyPolices(const std::vector<nunchuk::SingleSigner> signers);

} // namespace helper
} // namespace features::signers::flows
