#include "WalletConfigViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/SetupPlatformKeyPolicyFlow.h"
#include "generated_qml_keys.hpp"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;
using namespace features::signers::flows;

WalletConfigViewModel::WalletConfigViewModel(QObject *parent) : ActionViewModel(parent) {}

void WalletConfigViewModel::onPlatformKeyConfigClicked(const QString &walletId) {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    GUARD_WALLET(walletId)
    wallet->GetGroupWalletConfig();
    auto walletConfig = wallet->nunchukConfig();
    if (!walletConfig.get_platform_key().has_value()) {
        return;
    }
    auto platformKey = walletConfig.get_platform_key().value();
    auto policies = platformKey.get_policies();
    bool has_global_policies = policies.get_global().has_value();

    flowMng->stopFlow<SetupPlatformKeyPolicyFlow>();
    auto flow = flowMng->startFlow<SetupPlatformKeyPolicyFlow>();
    if (flow) {
        QString groupid = wallet->groupId();
        flow->setentryPoint(Constants::EntryPointWallet);
        flow->setwalletId(walletId);
        flow->setgroupId(groupid);
        if (has_global_policies) {
            closeToWalletTab();
            subMng->show(qml::features::signers::qglobalplatformkeypolicies);
        } else {
            closeToWalletTab();
            subMng->show(qml::features::signers::qperkeyplatformkeypolicies);
        }
    }
}

} // namespace features::wallets::viewmodels
