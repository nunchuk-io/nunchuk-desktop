#include "PlatformKeyPoliciesViewModel.h"
#include "ViewsEnums.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "features/signers/flows/SetupPlatformKeyPolicyFlow.h"
#include "generated_qml_keys.hpp"
namespace features::signers::viewmodels {
using namespace features::signers::flows;

PlatformKeyPoliciesViewModel::PlatformKeyPoliciesViewModel(QObject *parent) : BasePlatformKeyPoliciesViewModel(parent) {
    setselectedPolicyId("global-policy");
}

void PlatformKeyPoliciesViewModel::onInit() {
    QString policySelected = "global-policy";
    GUARD_FLOW_MANAGER()
    GUARD_SHARE_WALLETS()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    QVariantList examplePolicies;
    if (flow) {
        QString groupId = flow->groupId();
        QString walletId = flow->walletId();
        DBG_INFO << "groupId: " << groupId << " and walletId: " << walletId;
        if (walletId != "") {
            GUARD_WALLET(walletId)
            if (wallet) {
                wallet->GetGroupWalletConfig();
                auto walletConfig = wallet->nunchukConfig();
                if (walletConfig.get_platform_key().has_value()) {
                    auto policies = walletConfig.get_platform_key().value().get_policies();
                    bool has_global_policies = policies.get_global().has_value();
                    if (has_global_policies) {
                        policySelected = "global-policy";
                    } else {
                        policySelected = "per-key-policy";
                    }
                }
            }
        } else {
            GUARD_APP_MODEL()
            QString groupId = flow->groupId();
            if (auto group_wallet = sharedWallets->sandboxListPtr()->GetGroup(groupId)) {
                nunchuk::GroupSandbox sanbox = group_wallet->sandbox();
                if (sanbox.get_platform_key().has_value()) {
                    auto policies = sanbox.get_platform_key().value().get_policies();
                    bool has_global_policies = policies.get_global().has_value();
                    if (has_global_policies) {
                        policySelected = "global-policy";
                    } else {
                        policySelected = "per-key-policy";
                    }
                }
            }
        }
    }
    setselectedPolicyId(policySelected);
}

void PlatformKeyPoliciesViewModel::onContinueClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    if (qUtils::strCompare(selectedPolicyId(), "global-policy")) {
        subMng->show(qml::features::signers::qglobalplatformkeypolicies);
    } else {
        subMng->show(qml::features::signers::qperkeyplatformkeypolicies);
    }
}

} // namespace features::signers::viewmodels
